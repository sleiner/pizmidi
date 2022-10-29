#include "PizLooper.h"
#include <vector>

using juce::jmax;
using juce::jmin;
using juce::roundToInt;

void PizLooper::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    lastPosInfo.ppqPosition = 0;
    lastPosInfo.bpm         = 0;
}

void PizLooper::releaseResources()
{
    samples = 0;
}

void PizLooper::killNotes(int slot)
{
    kill_all_notes[slot] = true;
}

void PizLooper::endHangingNotesInLoop(juce::MidiBuffer& buffer, int samplePos, int slot, int voice, bool kill)
{
    noteOffBuffer[slot].clear();
    if (slot == curProgram)
        kbstate.reset();
    samplePos     = jmax(0, samplePos);
    int numVoices = voice == -1 ? polyphony : 1;
    if (voice == -1)
        voice = 0;
    while (numVoices--)
    {
        const int numNotes = programs[slot].loop.playingNotes[voice].size();
        for (int i = numNotes; --i >= 0;)
        {
            LoopNote n                = programs[slot].loop.playingNotes[voice].getUnchecked(i);
            juce::MidiMessage noteoff = juce::MidiMessage::noteOff(n.lastOutputChannel, n.lastOutputNoteNumber);
            buffer.addEvent(noteoff, samplePos);
            DBG("ending note");
            --playingNote[noteoff.getChannel() - 1][noteoff.getNoteNumber()];
            jassert(playingNote[noteoff.getChannel() - 1][noteoff.getNoteNumber()] >= 0);
            programs[slot].loop.setNoteOff(n.note, voice);
        }
        voice++;
    }
    if (kill)
    {
        //just in case????????
        for (int ch = 0; ch < 16; ch++)
        {
            for (int n = 0; n < 128; n++)
            {
                while (playingNote[ch][n] > 0)
                {
                    --playingNote[ch][n];
                    juce::MidiMessage noteoff = juce::MidiMessage::noteOff(ch + 1, n);
                    buffer.addEvent(noteoff, samplePos);
                    DBG("killing note");
                }
            }
        }
    }
}

void PizLooper::transposePlayingNotes(juce::MidiBuffer& buffer, int samplePos, int slot, int voice, int numSamples)
{
    samplePos     = jmax(0, samplePos);
    int numVoices = voice == -1 ? polyphony : 1;
    if (voice == -1)
        voice = 0;
    while (numVoices--)
    {
        const int numNotes = programs[slot].loop.playingNotes[voice].size();
        for (int i = numNotes; --i >= 0;)
        {
            bool kill         = false;
            LoopNote n        = (programs[slot].loop.playingNotes[voice].getUnchecked(i));
            const int oldNote = programs[slot].loop.getTranspositionOfNote(n.note, voice);
            const int channel = n.lastOutputChannel;
            const int newNote = tRules[slot]->getTransposedNote(n.note->message.getNoteNumber(), voice, kill, channel);
            if (oldNote != newNote)
            {
                buffer.addEvent(juce::MidiMessage::noteOff(channel, oldNote), jmax(0, samplePos - 1));
                DBG("transposing note (end old)");
                --playingNote[channel - 1][oldNote];
                int offIndex = getIndexOfNoteOff(n.note, slot, voice);
                if (! kill)
                {
                    buffer.addEvent(juce::MidiMessage::noteOn(channel, newNote, n.note->message.getVelocity()), jmin(samplePos + 1, numSamples - 1));
                    DBG("transposing note (start new)");
                    programs[slot].loop.sentNoteAs(n.note, voice, newNote, channel, n.noteOffSample);
                    if (offIndex != -1)
                    {
                        jassert(n.noteOffSample >= 0);
                        noteOffBuffer[slot].set(offIndex, LoopNote(n.note, newNote, channel, n.noteOffSample, voice));
                    }
                    ++playingNote[channel - 1][newNote];
                }
                else
                {
                    programs[slot].loop.setNoteOff(n.note, voice);
                    noteOffBuffer[slot].remove(offIndex);
                }
            }
        }
        voice++;
    }
}

void PizLooper::processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages)
{
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // time info
    bool wasplaying;
    juce::AudioPlayHead::CurrentPositionInfo pos;
    if (getPlayHead() != nullptr && getPlayHead()->getCurrentPosition(pos))
        wasplaying = lastPosInfo.isPlaying || lastPosInfo.isRecording;
    else
    { //no timeinfo
        zeromem(&pos, sizeof(pos));
        wasplaying             = true;
        pos.isPlaying          = true;
        pos.timeSigNumerator   = 4;
        pos.timeSigDenominator = 4;
        pos.bpm                = 120.0;
        pos.ppqPosition        = lastPosInfo.ppqPosition + samplesInLastBuffer * (120.0 / (60.0 * getSampleRate()));
    }

    if (reaper)
        pos.ppqPosition = GetPlayPosition() * lastPosInfo.bpm / 60.0;

    if (lastPosInfo.bpm == 0)
        lastPosInfo.bpm = pos.bpm;
    if (getParameter(kSync) >= 0.5f)
    { //sample sync
        //use previous tempo
        pos.ppqPosition = lastPosInfo.ppqPosition + samplesInLastBuffer * (lastPosInfo.bpm / (60.0 * getSampleRate()));
        pos.isPlaying   = true;
    }

    //updating time info here!!!!
    lastPosInfo         = pos;
    samplesInLastBuffer = buffer.getNumSamples();

    const bool playing     = lastPosInfo.isPlaying || lastPosInfo.isRecording;
    const bool justStopped = ! playing && wasplaying;
    const double num       = (double) lastPosInfo.timeSigNumerator;
    const double den       = (double) lastPosInfo.timeSigDenominator;
    const double tempo     = lastPosInfo.bpm;
    const double ppq       = lastPosInfo.ppqPosition;
    const double ppqPerBar = (num * 4.0 / den);
    //const double beats = (fmod (ppq, ppqPerBar) / ppqPerBar) * num;
    const int bar = (int) (ppq / ppqPerBar) + 1;
    //const int beat = ((int) beats) + 1;
    //const int tick = ((int) (fmod (beats, 1.0) * 960.0));
    //const double ppqOfLastBar = (double)(bar-1)*ppqPerBar;
    const double ppqOfNextBar   = (double) (bar) *ppqPerBar;
    const double ppqPerSample   = tempo / (60.0 * getSampleRate());
    const double samplesPerPpq  = 1.0 / ppqPerSample;
    const double looplengthstep = getLoopLengthStep();
    const double ppqOfLastStep  = floor(ppq / looplengthstep) * looplengthstep;
    double targetLength         = looplengthstep * (double) roundToInt(param[kFixedLength] * 32.f);
    const bool overdub          = param[kRecMode] >= 0.5f;
    const bool inclength        = param[kRecMode] >= 0.8f;

    //use this to quantize input
    double quantization   = 0.0;
    double eventoffset    = 0.0;
    double receventoffset = 0.0;
    double quant          = 0.0;
    if (param[kQuantize] > 0.0f)
    {
        if (param[kQuantize] < 0.3f)
            quantization = 0.5; //8th note
        else if (param[kQuantize] < 0.6f)
            quantization = 0.25; //16th
        else if (param[kQuantize] < 0.9f)
            quantization = 0.125; //32nd
        else
            quantization = 0.0625; //64th

        quant = fmod(ppq, quantization); //ppq since last q step
        if (quant >= 0.5 * quantization)
            eventoffset = quantization - quant;
        else
            eventoffset = -quant;
    }
    if (fmod(ppq, 0.5) >= 0.5 * 0.5)
        receventoffset = 0.5 - fmod(ppq, 0.5);
    else
        receventoffset = -fmod(ppq, 0.5);

    const int numevents = midiMessages.getNumEvents();
    std::vector<bool> played(numevents, false);
    std::vector<bool> usedForScale(numevents, false);
    std::vector<bool> usedForTranspose(numevents, false);

    juce::MidiBuffer kbBuffer;
    kbstate.processNextMidiBuffer(kbBuffer, 0, buffer.getNumSamples(), true);

    juce::MidiBuffer midiout;
    for (int slot = 0; slot < slotLimit; slot++)
    {
        Loop* loop = &(programs[slot].loop);

        if (justStopped)
            endHangingNotesInLoop(midiout, 0, slot);
        if (loop->getNumEvents() > 0 && tRules[slot]->update())
        {
            //transposition changed
            if (getParameterForSlot(kImmediateTranspose, slot) >= 0.5f)
                transposePlayingNotes(midiout, 0, slot, -1, buffer.getNumSamples());
            tRules[slot]->update();
        }

        int channel          = roundToInt(getParameterForSlot(kChannel, slot) * 16.0f) - 1;
        const int firstch    = channel == -1 ? 0 : channel;
        const int lastch     = channel == -1 ? 15 : channel;
        const float noteTrig = getParameterForSlot(kNoteTrig, slot);
        int root             = floatToMidi(getParameterForSlot(kRoot, slot));

        //recording stuff
        if ((getParameterForSlot(kRecord, slot) >= 0.5f && lastrec[slot] < 0.5f) && overdub)
            recstart[slot] = ppq + receventoffset;
        if (! overdub)
            recordAndPlay[slot] = false;
        bool otherThanFirstOverdub = false;
        bool firstOverdub          = false;
        if (overdub && ! inclength && ! targetLength)
            targetLength = programs[slot].looplength;
        if (getParameterForSlot(kRecord, slot) >= 0.5f && targetLength && (overdub || lastrec[slot] >= 0.5f))
        {
            const double recordedTime = (ppq + ppqPerSample * buffer.getNumSamples()) - recstart[slot];
            if (recordedTime >= targetLength)
            {
                //recorded selected length, stop recording, unless overdubbing
                if (! overdub)
                    setParameterForSlot(kRecord, slot, 0.f);
                else
                {
                    if (targetLength > oldlength[slot])
                        setLoopLength(slot, targetLength);
                    if (! recordAndPlay[slot])
                        firstOverdub = true;
                    else
                        otherThanFirstOverdub = true;
                    recordAndPlay[slot] = true;
                }
            }
        }
        if (getParameterForSlot(kRecord, slot) != lastrec[slot] || (otherThanFirstOverdub || firstOverdub))
        {
            if ((getParameterForSlot(kRecord, slot) < 0.5 && lastrec[slot] >= 0.5) || firstOverdub)
            {
                //stop recording
                if (overdub && ! recordAndPlay[slot])
                    firstOverdub = true;
                bool play_on_stop = getParameterForSlot(kTrigger, slot) == 0.0
                                 || (getParameterForSlot(kTrigger, slot) >= 0.3f && getParameterForSlot(kTrigger, slot) < 0.4f);
                if (! overdub)
                {
                    setParameterForSlot(kLoopEnd, slot, 0.5f);
                    setParameterForSlot(kLoopStart, slot, 0.5f);
                }
                if (play_on_stop)
                {
                    //setParameterForSlot(kPlay,slot,1.0f);
                    //processGroups(slot);
                    //updateHostDisplay();
                }
                recstop[slot]   = ppq + receventoffset;
                oldlength[slot] = getLoopLength(slot);
                if (! overdub || firstOverdub || (overdub && ! targetLength))
                {
                    programs[slot].looplength = recstop[slot] - recstart[slot];
                    if (play_on_stop && ! isSlotPlaying(slot))
                    {
                        setParameterForSlot(kPlay, slot, 1.0f);
                        processGroups(slot);
                        updateHostDisplay();
                        lastplay[slot]               = true;
                        measurePlayFromHere[slot][0] = programs[slot].measureFromHere;
                        barTriggerSample[slot][0]    = 0;

                        //start playing
                        if (noteTrig == 0.0f && root > -1)
                        {
                            currentPoly[slot]++;
                            polytrigger[slot][0] = root;
                        }
                        tRules[slot]->trigger(root, 1.f, 0);
                    }
                    //if looplength isn't exactly an integer number of looplengthsteps,
                    //move the end so length is a whole number of steps
                    double lengthadjustment = fmod(programs[slot].looplength, looplengthstep);
                    if (lengthadjustment > 0)
                    {
                        //if not too close to last step, move end ahead
                        if (lengthadjustment > looplengthstep * 0.25)
                        {
                            programs[slot].looplength += looplengthstep - lengthadjustment;
                            //also move the playstart (so playback won't start until the actual loop end point)
                            if (play_on_stop)
                            {
                                //measurePlayFromHere[slot][0]=ppqOfLastStep;
                                //measurePlayFromHere[slot][0] += looplengthstep-lengthadjustment;
                            }
                        }
                        //otherwise, move the end back
                        else
                        {
                            programs[slot].looplength -= lengthadjustment;
                            if (play_on_stop)
                            {
                                //measurePlayFromHere[slot][0]=ppqOfLastStep;
                                //measurePlayFromHere[slot][0] -= lengthadjustment;
                            }
                            //todo: also wrap recorded events back to the start!!!!!!!!!!?????
                        }
                    }
                    loop->setLength(getLoopLength(slot) + getLoopStart(slot));
                }
                if (! recordAndPlay[slot])
                {
                    for (int ch = firstch; ch <= lastch; ch++)
                    {
                        for (int i = 0; i < 128; i++)
                        {
                            // prevent hanging notes in the loop
                            if (recNoteOn[slot][ch][i] >= 0)
                            {
                                DBG("hanging rec note " + juce::String(ppq - measureDubFromHere[slot]));
                                loop->addEvent(juce::MidiMessage::noteOff(ch + 1, i),
                                               (int) ((ppq - measureDubFromHere[slot]) * 960.0));
                                loop->updateMatchedPairs();
                                recNoteOn[slot][ch][i] = -1;
                            }
                        }
                    }
                }
                if (overdub)
                {
                    if (programs[slot].looplength > oldlength[slot] && oldlength[slot] > 0.0)
                    {
                        //overdubbing over a shorter loop, so repeat old sequence to fill
                        programs[slot].looplength += fmod(programs[slot].looplength, oldlength[slot]);
                        loop->setLength(getLoopLength(slot) + getLoopStart(slot));
                        int i = 1;
                        while (i < ((int) programs[slot].looplength / (int) oldlength[slot]))
                        {
                            loop->addSequence(oldloop[slot],
                                              (programs[slot].loopstart + i * oldlength[slot]) * 960.0,
                                              (programs[slot].loopstart + i * oldlength[slot]) * 960.0,
                                              (programs[slot].loopstart + (i + 1) * oldlength[slot]) * 960.0);
                            loop->updateMatchedPairs();
                            i++;
                        }
                    }
                    else if (oldlength[slot] > getLoopLength(slot))
                    {
                        programs[slot].looplength = oldlength[slot];
                        loop->setLength(getLoopLength(slot) + getLoopStart(slot));
                    }
                }
                newLoop = true;
            }
            if ((getParameterForSlot(kRecord, slot) >= 0.5f && lastrec[slot] < 0.5f) || recordAndPlay[slot])
            {
                //start recording, or start new overdubbing loop
                if (overdub)
                    setParameterForSlot(kPlay, slot, 1.f);
                if (! overdub || loop->getNumEvents() == 0)
                {
                    //replace (or overdubbing to an empty loop)
                    setParameterForSlot(kPlay, slot, 0.0f);
                    processGroups(slot);
                    loop->clear();
                    oldloop[slot].clear();
                    programs[slot].measureFromHere = ppqOfLastStep;
                    DBG("measureFromHere = ppqOfLastStep = " + juce::String(ppqOfLastStep));
                    if (! overdub || loop->getNumEvents() == 0)
                    {
                        programs[slot].looplength = 0;
                        recstart[slot]            = ppq + receventoffset;
                        programs[slot].loopstart  = recstart[slot] - programs[slot].measureFromHere;
                        DBG("loopstart=" + juce::String(programs[slot].loopstart));
                    }
                    //move the start ahead if near the next step (and not in sample sync mode)
                    if ((recstart[slot] - ppqOfLastStep) > looplengthstep * 0.875 && param[kSync] < 0.5f)
                    {
                        programs[slot].measureFromHere += looplengthstep;
                        DBG("measureFromHere += looplengthstep = " + juce::String(programs[slot].measureFromHere));
                        programs[slot].loopstart = 0; //programs[slot].measureFromHere;
                        recstart[slot]           = programs[slot].measureFromHere;
                    }
                    for (int ch = firstch; ch <= lastch; ch++)
                    {
                        for (int i = 0; i < 128; i++)
                        {
                            if (hanging[slot].count[ch][i] > 0)
                            {
                                juce::MidiMessage noteon = juce::MidiMessage(0x90 | ch, i, hanging[slot].vel[ch][i], 0);
                                loop->addEvent(noteon, roundToInt((getLoopStart(slot)) * 960.0) - (int) (programs[slot].measureFromHere * 960.0));
                                loop->updateMatchedPairs();
                            }
                        }
                    }
                    measureDubFromHere[slot] = programs[slot].measureFromHere;
                    DBG("measureDubFromHere = measureFromHere = " + juce::String(measureDubFromHere[slot]));
                }
                else
                { //overdub
                    oldloop[slot]  = *loop;
                    recstart[slot] = ppq + receventoffset;
                    //loopstart=recstart;
                    measureDubFromHere[slot] = getPpqOfLastLoopStart(ppq, slot);
                    DBG("measureDubFromHere = ppqOfLastLoopStart = " + juce::String(measureDubFromHere[slot]));
                    //if ((recstart[slot]-ppqOfLastStep)>looplengthstep*0.5f && param[kSync]<0.5f) {
                    //	measureDubFromHere[slot] += looplengthstep;
                    //	DBG("measureDubFromHere += looplengthstep = " + String(measureDubFromHere[slot]));
                    //	recstart[slot] = measureDubFromHere[slot];
                    //}
                    for (int ch = firstch; ch <= lastch; ch++)
                    {
                        for (int i = 0; i < 128; i++)
                        {
                            if (hanging[slot].count[ch][i] > 0 && recNoteOn[slot][ch][i] == -1)
                            {
                                DBG("creating noteon at " + juce::String(getLoopStart(slot) - programs[slot].measureFromHere));
                                juce::MidiMessage noteon = juce::MidiMessage(0x90 | ch, i, hanging[slot].vel[ch][i], 0);
                                loop->addEvent(noteon, roundToInt((recstart[slot]) * 960.0) - (int) (measureDubFromHere[slot] * 960.0));
                                loop->updateMatchedPairs();
                            }
                        }
                    }
                }
            }
            lastrec[slot] = getParameterForSlot(kRecord, slot);

            //update GUI
            currentLength    = getLoopLength(curProgram);
            currentNumEvents = loop->getNumEvents();
            sendChangeMessage();
        }

        if (kill_all_notes[slot])
        {
            endHangingNotesInLoop(midiout, 0, slot);
            kill_all_notes[slot] = false;
        }

        const bool waitForBar = getParameterForSlot(kWaitForBar, slot) >= 0.5;

        //midi input part
        if (! midiMessages.isEmpty())
        {
            int message_number  = 0;
            const int scalechan = (getParameterForSlot(kUseScaleChannel, slot) >= 0.5)
                                    ? roundToInt(getParameterForSlot(kScaleChannel, slot) * 15.0f) + 1
                                    : 0;
            const int trchan    = (getParameterForSlot(kUseTrChannel, slot) >= 0.5f)
                                    ? roundToInt(getParameterForSlot(kTransposeChannel, slot) * 15.0f) + 1
                                    : 0;
            for (auto&& msgMetadata : midiMessages)
            {
                auto midi_message  = msgMetadata.getMessage();
                auto sample_number = msgMetadata.samplePosition;

                bool discard            = param[kThru] < 0.5f;
                const bool isForChannel = channel == -1 || midi_message.isForChannel(channel + 1);
                const int notenum       = midi_message.getNoteNumber();
                const int eventchan     = midi_message.getChannel() - 1;
                bool dontrecord         = false;
                bool usedForScaleTr     = false;
                if (midi_message.isAllNotesOff())
                {
                    if (isForChannel)
                    {
                        endHangingNotesInLoop(midiout, 0, slot, -1, true);
                        for (int ch = 0; ch < 16; ch++)
                        {
                            for (int n = 0; n < 128; n++)
                            {
                                recNoteOn[slot][ch][n] = -1;
                            }
                        }
                        capturingScale[slot]     = 0;
                        capturingTranspose[slot] = 0;
                        dontrecord               = true;
                    }
                }
                if (! played[message_number])
                {
                    if (midi_message.isController())
                    {
                        if (recCC == -2)
                        {
                            setParameter(kRecCC, float((midi_message.getControllerNumber() + 2) / 129.f));
                            dontrecord             = true;
                            played[message_number] = true;
                        }
                        else if (playCC == -2)
                        {
                            setParameter(kPlayCC, float((midi_message.getControllerNumber() + 2) / 129.f));
                            dontrecord             = true;
                            played[message_number] = true;
                        }
                        else if (recCC == midi_message.getControllerNumber())
                        {
                            if (midi_message.getControllerValue() > 63 != getParameter(kRecordMain) >= 0.5f)
                                setParameter(kRecordMain, midi_message.getControllerValue() > 63 ? 1.f : 0.f);
                            dontrecord             = true;
                            played[message_number] = true;
                        }
                        else if (playCC == midi_message.getControllerNumber())
                        {
                            if (midi_message.getControllerValue() > 63 != getParameter(kPlayMain) >= 0.5f)
                                setParameter(kPlayMain, midi_message.getControllerValue() > 63 ? 1.f : 0.f);
                            dontrecord             = true;
                            played[message_number] = true;
                        }
                    }
                    else if (midi_message.isProgramChange())
                    {
                        dontrecord                 = true;
                        kill_all_notes[curProgram] = false;
                        played[message_number]     = true;
                        setActiveSlot(midi_message.getProgramChangeNumber());
                    }
                }

                if (scalechan > 0 && midi_message.isForChannel(scalechan))
                {
                    if (midi_message.isNoteOn() && ! usedForScale[message_number])
                    {
                        played[message_number]       = true;
                        dontrecord                   = true;
                        discard                      = true;
                        usedForScale[message_number] = true;
                        if (capturingScale[slot] == 0)
                        {
                            for (int s = 0; s < numSlots; s++)
                            {
                                if (getParameterForSlot(kUseScaleChannel, s) >= 0.5f
                                    && scalechan == roundToInt(getParameterForSlot(kScaleChannel, s) * 15.0f) + 1)
                                    for (int i = 0; i < 12; i++)
                                        setParameterForSlot(kNote0 + i, s, 0.f);
                            }
                        }
                        capturingScale[slot]++;
                        for (int s = 0; s < numSlots; s++)
                        {
                            if (getParameterForSlot(kUseScaleChannel, s) >= 0.5f
                                && scalechan == roundToInt(getParameterForSlot(kScaleChannel, s) * 15.0f) + 1)
                            {
                                capturingScale[s] = capturingScale[slot];
                                setParameterForSlot(kNote0 + notenum % 12, s, 1.f);
                            }
                            if (s == curProgram)
                                sendChangeMessage();
                        }
                        if (trchan == 0 || ! midi_message.isForChannel(trchan))
                        {
                            if (tRules[slot]->update())
                            {
                                //transposition changed
                                if (getParameterForSlot(kImmediateTranspose, slot) >= 0.5f)
                                    transposePlayingNotes(midiout, sample_number, slot, -1, buffer.getNumSamples());
                                tRules[slot]->update();
                            }
                        }
                    }
                    else if (midi_message.isNoteOff() && ! usedForScale[message_number])
                    {
                        played[message_number]       = true;
                        dontrecord                   = true;
                        discard                      = true;
                        usedForScaleTr               = true;
                        usedForScale[message_number] = true;
                        capturingScale[slot]--;
                        if (capturingScale[slot] < 1)
                            capturingScale[slot] = 0;
                        for (int s = 0; s < numSlots; s++)
                            if (getParameterForSlot(kUseScaleChannel, s) >= 0.5f
                                && scalechan == roundToInt(getParameterForSlot(kScaleChannel, s) * 15.0f) + 1)
                                capturingScale[s] = capturingScale[slot];
                    }
                }
                if (trchan > 0 && midi_message.isForChannel(trchan))
                {
                    if (midi_message.isNoteOn() && ! usedForTranspose[message_number])
                    {
                        played[message_number]           = true;
                        dontrecord                       = true;
                        discard                          = true;
                        usedForTranspose[message_number] = true;
                        if (capturingTranspose[slot] == 0)
                        {
                            lowestScaleInputNote[slot] = 127;
                        }
                        if (getParameterForSlot(kUseTrChannel, slot) >= 0.5f)
                        {
                            capturingTranspose[slot]++;
                            for (int s = 0; s < numSlots; s++)
                            {
                                if (getParameterForSlot(kUseTrChannel, s) >= 0.5f
                                    && trchan == roundToInt(getParameterForSlot(kTransposeChannel, s) * 15.0f) + 1)
                                    capturingTranspose[s] = capturingTranspose[slot];
                            }
                        }
                        if (notenum < lowestScaleInputNote[slot] && getParameterForSlot(kUseTrChannel, slot) >= 0.5f)
                        {
                            lowestScaleInputNote[slot] = notenum;
                            for (int s = 0; s < numSlots; s++)
                            {
                                if (getParameterForSlot(kUseTrChannel, s) >= 0.5f
                                    && trchan == roundToInt(getParameterForSlot(kTransposeChannel, s) * 15.0f) + 1)
                                {
                                    lowestScaleInputNote[s] = notenum;
                                    const int r             = floatToMidi(getParameterForSlot(kRoot, s));
                                    setParameterForSlot(kOctave, s, (float) (lowestScaleInputNote[slot] / 12 - r / 12 + 4) * 0.125f);
                                    setParameterForSlot(kTranspose, s, (float) (lowestScaleInputNote[slot] % 12 - r % 12 + 12) / 24.f);
                                }
                            }
                        }
                        if (tRules[slot]->update())
                        {
                            //transposition changed
                            if (getParameterForSlot(kImmediateTranspose, slot) >= 0.5f)
                                transposePlayingNotes(midiout, sample_number, slot, -1, buffer.getNumSamples());
                            tRules[slot]->update();
                        }
                    }
                    else if (midi_message.isNoteOff() && ! usedForTranspose[message_number])
                    {
                        played[message_number]           = true;
                        dontrecord                       = true;
                        discard                          = true;
                        usedForScaleTr                   = true;
                        usedForTranspose[message_number] = true;
                        capturingTranspose[slot]--;
                        if (capturingTranspose[slot] < 1)
                            capturingTranspose[slot] = 0;
                        for (int s = 0; s < numSlots; s++)
                            if (getParameterForSlot(kUseTrChannel, s) >= 0.5f
                                && trchan == roundToInt(getParameterForSlot(kTransposeChannel, s) * 15.0f) + 1)
                                capturingTranspose[s] = capturingTranspose[slot];
                    }
                }

                // note triggering:
                if (processTriggerNote(slot, midi_message, root, midiout, sample_number, ppqOfNextBar, ppqPerSample, ppqOfLastStep, looplengthstep))
                {
                    discard = dontrecord = played[message_number] = true;
                }
                if (isNoteTriggeringAnySlot(midi_message))
                {
                    discard = true;
                }
                if ((isForChannel || (slot == curProgram && param[kMonitor] >= 0.5f)) && ! dontrecord)
                {
                    //message is recorded here!
                    juce::MidiMessage m(midi_message);
                    if (param[kMonitor] >= 0.5f && param[kThru] >= 0.5f && channel > 0 && slot == curProgram)
                    {
                        m.setChannel(channel + 1);
                    }
                    recordMessage(m, slot, playing, ppq, eventoffset, ppqPerSample, sample_number);
                    if (overdub && isSlotPlaying(slot))
                    {
                        played[message_number] = true;
                    }
                }
                //transpose using current slot settings if "Monitor" is on
                if (midi_message.isNoteOn() && param[kThru] >= 0.5f)
                {
                    if (discard)
                        inputNoteTransposition[eventchan][notenum] = -1;
                    else
                    {
                        if (param[kMonitor] >= 0.5f)
                            midi_message.setNoteNumber(tRules[curProgram]->getTransposedNote(notenum, 0, discard, midi_message.getChannel(), true));
                        inputNoteTransposition[eventchan][notenum] = midi_message.getNoteNumber();
                    }
                }
                else if (midi_message.isNoteOff() && param[kThru] >= 0.5f)
                {
                    if (inputNoteTransposition[eventchan][notenum] == -1)
                        discard = true;
                    else
                        midi_message.setNoteNumber(inputNoteTransposition[eventchan][notenum]);
                }
                //if thru is on, copy original message
                if (! discard && ! played[message_number])
                {
                    played[message_number] = true;
                    const int curChannel   = roundToInt(getParameterForSlot(kChannel, curProgram) * 16.0f);
                    if (curChannel > 0 && param[kMonitor] >= 0.5f)
                        midi_message.setChannel(curChannel);
                    midiout.addEvent(midi_message, sample_number);
                    DBG("event thru");
                }
                message_number++;
            }
        }
        //play/record GUI keyboard through current slot:
        if (slot == curProgram)
        {
            for (auto&& msgMetadata : kbBuffer)
            {
                auto midi_message  = msgMetadata.getMessage();
                auto sample_number = msgMetadata.samplePosition;

                bool discard            = true;
                const bool isForChannel = channel == -1 || midi_message.isForChannel(channel + 1);
                const int notenum       = midi_message.getNoteNumber();
                const int eventchan     = midi_message.getChannel() - 1;
                if (isForChannel)
                {
                    discard = false;
                    recordMessage(midi_message, slot, playing, ppq, eventoffset, ppqPerSample, sample_number);
                }
                if (midi_message.isNoteOn())
                {
                    midi_message.setNoteNumber(tRules[slot]->getTransposedNote(notenum, 0, discard, midi_message.getChannel(), true));
                    if (discard)
                        inputNoteTransposition[eventchan][notenum] = -1;
                    else
                        inputNoteTransposition[eventchan][notenum] = midi_message.getNoteNumber();
                }
                else if (midi_message.isNoteOff())
                {
                    midi_message.setNoteNumber(inputNoteTransposition[eventchan][notenum]);
                    if (inputNoteTransposition[eventchan][notenum] == -1)
                        discard = true;
                }
                if (! discard)
                {
                    midiout.addEvent(midi_message, sample_number);
                    DBG("GUI keyboard thru");
                }
            }
        }

        //play state changed:
        if (getParameterForSlot(kPlay, slot) != lastplay[slot])
        {
            if (isSlotPlaying(slot) && lastplay[slot] < 0.5f)
            {
                //start playing
                if (noteTrig == 0.0f && root > -1)
                {
                    currentPoly[slot]++;
                    polytrigger[slot][0] = root;
                }
                if (getParameterForSlot(kTrigger, slot) < 0.1f)
                {
                    //if in sync
                    if (fmod(getLoopLength(slot), looplengthstep) > 0)
                        measurePlayFromHere[slot][0] = ppqOfLastStep;
                }
                else
                    measurePlayFromHere[slot][0] = ppq;
                if (! waitForBar)
                {
                    barTriggerSample[slot][0] = 0;
                    tRules[slot]->trigger(root, 1.f, 0);
                }
                else
                {
                    barTriggerSample[slot][0]    = (int) ((ppqOfNextBar) / ppqPerSample);
                    measurePlayFromHere[slot][0] = ppqOfNextBar;
                    if (slot == curProgram)
                    {
                        info->s = "Playing at next bar";
                        info->sendChangeMessage();
                    }
                }
            }
            else if (! isSlotPlaying(slot) && lastplay[slot] >= 0.5f)
            {
                //stop playing
                if (! waitForBar)
                {
                    if (overdub)
                    {
                        setParameterForSlot(kRecord, slot, 0.f);
                        lastrec[slot] = 0.0f;
                    }
                    barStopSample[slot][0] = -1;
                    if (slot == curProgram)
                    {
                        info->s = "Stopped";
                        info->sendChangeMessage();
                    }
                    if (noteTrig == 0.0f)
                    {
                        currentPoly[slot]    = 0;
                        polytrigger[slot][0] = -1;
                    }
                    lastPlayedIndex[slot][0] = -1;
                    tRules[slot]->release(0);
                    endHangingNotesInLoop(midiout, buffer.getNumSamples() - 1, slot);
                }
                else
                {
                    barStopSample[slot][0] = (int) ((ppqOfNextBar) / ppqPerSample);
                    if (slot == curProgram)
                    {
                        info->s = "Stopping at next bar";
                        info->sendChangeMessage();
                    }
                }
            }
            lastplay[slot] = isSlotPlaying(slot);
        }

        //loop playing!!!
        bool not_done = true;
        while (not_done)
        {
            not_done = false;
            if (playing
                && (isSlotPlaying(slot) || barStopSample[slot][0] >= ppq * ppqPerSample)
                && loop->getNumEvents() > 0)
            {
                //DBG("playing slot "+String(slot));
                const double stretch     = getStretchMultiplier(slot);
                const double newend      = (double) (roundToInt(getParameterForSlot(kLoopEnd, slot) * 16.0f) - 8);
                const double newstart    = (double) (roundToInt(getParameterForSlot(kLoopStart, slot) * 16.0f) - 8);
                const double beatshift   = (double) (roundToInt(getParameterForSlot(kShift, slot) * 16.0f) - 8);
                const int targetNumLoops = roundToInt(getParameterForSlot(kNumLoops, slot) * 64.f);
                const int nextSlot       = roundToInt(getParameterForSlot(kNextSlot, slot) * (float) numSlots) - 1;
                if (beatshift != lastshift[slot]
                    || lastend[slot] != newend
                    || laststart[slot] != newstart
                    || lastchannel[slot] != channel
                    || laststretch[slot] != stretch)
                {
                    //to be safe, end any playing notes when doing a time-based operation
                    //todo: check if this is still needed with the noteoffbuffer
                    //endHangingNotesInLoop(midiout, 0, slot);
                    for (int instance = 0; instance < polyphony; instance++)
                        lastPlayedIndex[slot][instance] = -1;
                    lastshift[slot]   = beatshift;
                    lastend[slot]     = newend;
                    laststart[slot]   = newstart;
                    lastchannel[slot] = channel;
                    laststretch[slot] = stretch;
                }
                const double newlength = getLoopLength(slot) + lastend[slot] - laststart[slot];
                for (int instance = 0; instance < polyphony; instance++)
                {
                    if (polytrigger[slot][instance] >= 0)
                    {
                        //DBG("instance "+String(instance));
                        //play the event that goes here, if any
                        const float tr        = getParameterForSlot(kTrigger, slot);
                        const bool unsyncloop = tr >= 0.2f && tr < 0.4f;
                        const bool oneshot    = tr >= 0.1f && tr < 0.2f;
                        const bool syncloop   = ! unsyncloop && ! oneshot;
                        bool stopPlaying      = false;
                        double lastLoopTime   = -1;
                        //int s = ;
                        juce::int64 startSample = (int) (samplesPerPpq * ppq);
                        juce::int64 endSample   = startSample + buffer.getNumSamples() - 1;
                        int stopAtSample        = buffer.getNumSamples();
                        int banana              = 0;
#if 0
						if (barTriggerSample[slot][instance]>endSample) {
							//waiting for next bar, don't start yet
							stopPlaying=true;
						}
						else if (barTriggerSample[slot][instance]>=startSample) {
							startSample = barTriggerSample[slot][instance];
						}
						if (barStopSample[slot][instance]>=startSample && barStopSample[slot][instance]<=endSample) {
							stopAtSample = (int)(barStopSample[slot][instance] - startSample);
						}
						if(!stopPlaying)
						{
							int i = (int)(startSample - (int)(samplesPerPpq*ppq));
							//==================================================================================
							//time calculation
							double looptime = fmod((ppq+(double)i*ppqPerSample-measurePlayFromHere[slot][instance])*laststretch[slot]-lastshift[slot], newlength)
								+ laststart[slot];
							double endlooptime = fmod((ppq+(double)buffer.getNumSamples()*ppqPerSample-measurePlayFromHere[slot][instance])*laststretch[slot]-lastshift[slot], newlength) + laststart[slot];
							while (looptime<(getLoopStart(slot)+laststart[slot])) {
								looptime+=newlength;
								endlooptime+=newlength;
							}
							double totallooptime = (ppq+(double)i*ppqPerSample-lastshift[slot]-measurePlayFromHere[slot][instance])*laststretch[slot] + laststart[slot];
							double totallooptimeatend = (ppq+(double)buffer.getNumSamples()*ppqPerSample-lastshift[slot]-measurePlayFromHere[slot][instance])*laststretch[slot] + laststart[slot];
							if (unsyncloop)
							{
								looptime=fmod((ppq+(double)i*ppqPerSample-measurePlayFromHere[slot][instance])*stretch-beatshift,newlength)+getLoopStart(slot)+newstart;
								totallooptime=(ppq+(double)i*ppqPerSample-beatshift-measurePlayFromHere[slot][instance])*stretch+getLoopStart(slot)+newstart;
								endlooptime=fmod((ppq+(double)buffer.getNumSamples()*ppqPerSample-measurePlayFromHere[slot][instance])*stretch-beatshift,newlength)+getLoopStart(slot)+newstart;
								totallooptimeatend=(ppq+(double)buffer.getNumSamples()*ppqPerSample-beatshift-measurePlayFromHere[slot][instance])*stretch+getLoopStart(slot)+newstart;
							}
							double totalloopcount = totallooptime / newlength;
							int loopcount = jmax(0,(int)totalloopcount - startLoopCount[slot][instance]);
							if (oneshot)
							{
								looptime=(ppq+(double)i*ppqPerSample-beatshift-measurePlayFromHere[slot][instance])*stretch+getLoopStart(slot)+newstart;
								endlooptime=(ppq+(double)buffer.getNumSamples()*ppqPerSample-beatshift-measurePlayFromHere[slot][instance])*stretch+getLoopStart(slot)+newstart;
								if ((ppq+(double)buffer.getNumSamples()*ppqPerSample-measurePlayFromHere[slot][instance])*stretch >= newlength)
								{
									stopAtSample = roundToInt(0.00104166666666666666666666666667*newlength*samplesPerPpq/stretch);
								}
								//handle beatshift>0:
								else if (looptime>getLoopEnd(slot)) {
									looptime-=newlength;
									endlooptime-=newlength;
								}
								//handle beatshift<0:
								else if (looptime<newstart) {
									looptime+=newlength;
									endlooptime+=newlength;
								}
							}
							else
							{
								if (lastLoopCount[slot][instance]==-1)
								{
									//starting first loop
									startLoopCount[slot][instance] = (int)ceil(totalloopcount);
									lastLoopCount[slot][instance] = 0;
								}
								else if (lastLoopCount[slot][instance]!=loopcount)
								{
									if (targetNumLoops>0 && loopcount==targetNumLoops) {
										lastLoopCount[slot][instance]=-1;
										stopPlaying=true;
										if (polytrigger[slot][instance]!=-1)
										{
											currentPoly[slot]--;
											polytrigger[slot][instance]=-1;
											jassert(currentPoly[slot]>=0);
											if (currentPoly[slot]==0) {
												setParameterForSlot(kPlay,slot,0.0);
												processGroups(slot);
											}
											lastPlayedIndex[slot][instance]=-1;
										}
										if (nextSlot>-1 && !isSlotPlaying(nextSlot))
										{
											setParameterForSlot(kPlay,nextSlot,1.0);
											processGroups(nextSlot);
											if (nextSlot<slot) {
												polytrigger[nextSlot][0] = floatToMidi(getParameterForSlot(kRoot,nextSlot));
												slot = nextSlot;
												loop = &(programs[slot].loop);
												channel = roundToInt(getParameterForSlot(kChannel,slot)*16.0f)-1;
												lastPlayedIndex[slot][0]=-1;
												not_done = true;
											}
										}
									}
									else
										lastLoopCount[slot][instance] = loopcount;
								}
							}
							if (instance==0)
								loopPpq[slot]=looptime;
							if (stopPlaying)
								break;
							if (looptime<lastLoopTime) {
								lastPlayedIndex[slot][instance]=-1;
								banana=0;
							}
							else if (lastPlayedIndex[slot][instance]+1 >= loop->getNumEvents()) {
								lastPlayedIndex[slot][instance]=-1;
								banana++;
							}
							else if (lastPlayedIndex[slot][instance]+1 >= loop->getNextIndexAtTime((int)(getLoopEnd(slot)*960.0))
								&& banana<3) {
								lastPlayedIndex[slot][instance]=-1;
								banana++;
							}
							lastLoopTime = looptime;
							//DBG("looptime="+String(looptime));
							//==================================================================================
							if (loop->getNumEvents()>0 && banana<9)
							{
								bool stop = false;
								int index;
								if (lastPlayedIndex[slot][instance] == -1) {
									index = loop->getNextIndexAtTime((int)(looptime*960.0));
									if (index==loop->getNumEvents()) {
										if (looptime>endlooptime)
											index = loop->getNextIndexAtTime((int)(newstart*960.0));
										if (index==loop->getNumEvents())
											stop=true;
									}
									else if (loop->getEventTime(index)<=(int)(endlooptime*960.0)) {
										lastPlayedIndex[slot][instance] = index-1;
										banana++;
									}
								}
								else if (loop->getEventTime(lastPlayedIndex[slot][instance]+1)<=(int)(endlooptime*960.0))
									index = lastPlayedIndex[slot][instance]+1;
								else
									index = lastPlayedIndex[slot][instance];
								if (loop->getEventTime(index)>(int)(endlooptime*960.0)) {
									DBG("  "+String(loop->getEventTime(index))+ " "+String((int)(looptime*960.0)));
									stop=true;
								}
								if (lastPlayedIndex[slot][instance]>=index) {
									//DBG("  "+String(lastPlayedIndex[slot][instance])+" "+String(index));
									stop=true;
								}
								while (index<(loop->getNumEvents()) && !stop)
								{
									if (loop->getEventTime(index)<=(int)(endlooptime*960.0))
									{
										if(lastPlayedIndex[slot][instance]<index)
										{
											if (i<stopAtSample)
											{
												DBG("playing loop event "+String(index)+ " at time "+String(loop->getEventTime(index)));
												i = roundToInt((loop->getEventTime(index)*0.00104166666666666666666666666667 - looptime)*samplesPerPpq);
												if (i<0)
													i+=roundToInt(samplesPerPpq*newlength/stretch);
												//i = roundToInt(((0.00104166666666666666666666666667*loop->getEventTime(index)-laststart[slot])/laststretch[slot]+ measurePlayFromHere[slot][instance]+lastshift[slot])*samplesPerPpq);
												playLoopEvent(slot,instance,index,channel,stretch,i,samplesPerPpq,midiout);
												lastPlayedIndex[slot][instance]=index;
											}
											else
												stop = true;
										}
										index++;
										if (looptime>endlooptime && loop->getEventTime(index)>(int)(getLoopEnd(slot)*960.0))
											index = loop->getNextIndexAtTime((int)(newstart*960.0));

									}
									else {
										stop=true;
									}
								} //events
							}
							else
								i = buffer.getNumSamples();
						}
						if (stopAtSample < buffer.getNumSamples())
						{
							if (oneshot) {
								if (polytrigger[slot][instance]!=-1)
								{
									currentPoly[slot]--;
									polytrigger[slot][instance]=-1;
									if (currentPoly[slot]==0) {
										setParameterForSlot(kPlay,slot,0.0);
										processGroups(slot);
									}
									lastPlayedIndex[slot][instance]=-1;
									lastLoopCount[slot][instance]=-1;
									endHangingNotesInLoop(midiout,stopAtSample,slot,instance);
								}
							}
							else {
								if (noteTrig==0.0f) {
									currentPoly[slot]--;
									polytrigger[slot][0]=-1;
								}
								lastPlayedIndex[slot][instance]=-1;
								lastLoopCount[slot][instance]=-1;
								endHangingNotesInLoop(midiout,stopAtSample,slot,instance);
								if (overdub) setParameterForSlot(kRecord,slot,0.f);
							}
						}
#else

                        if (barTriggerSample[slot][instance] > endSample)
                        {
                            //waiting for next bar, don't start yet
                            stopPlaying = true;
                        }
                        else if (barTriggerSample[slot][instance] >= startSample)
                        {
                            startSample = barTriggerSample[slot][instance];
                        }
                        if (barStopSample[slot][instance] >= startSample && barStopSample[slot][instance] <= endSample)
                        {
                            stopAtSample = jmin(buffer.getNumSamples(), (int) (barStopSample[slot][instance] - startSample));
                        }
                        for (int i = (int) startSample - (int) (samplesPerPpq * ppq); i < stopAtSample; i++)
                        {
                            //s++;
                            if (! stopPlaying)
                            {
                                //==================================================================================
                                //time calculation
                                double looptime = (fmod((ppq + (double) i * ppqPerSample - measurePlayFromHere[slot][instance]) * laststretch[slot] - lastshift[slot], newlength) + laststart[slot]);
                                while (looptime < (getLoopStart(slot) + laststart[slot]))
                                    looptime += newlength;
                                double totallooptime = (ppq + (double) i * ppqPerSample - lastshift[slot] - measurePlayFromHere[slot][instance]) * laststretch[slot] + laststart[slot];
                                if (unsyncloop)
                                {
                                    looptime      = fmod((ppq + (double) i * ppqPerSample - measurePlayFromHere[slot][instance]) * stretch - beatshift, newlength) + getLoopStart(slot) + newstart;
                                    totallooptime = (ppq + (double) i * ppqPerSample - beatshift - measurePlayFromHere[slot][instance]) * stretch + getLoopStart(slot) + newstart;
                                }
                                double totalloopcount = totallooptime / newlength;
                                int loopcount         = jmax(0, (int) totalloopcount - startLoopCount[slot][instance]);
                                if (oneshot)
                                {
                                    looptime = (ppq + (double) i * ppqPerSample - beatshift - measurePlayFromHere[slot][instance]) * stretch + getLoopStart(slot) + newstart;
                                    if ((ppq + (double) i * ppqPerSample - measurePlayFromHere[slot][instance]) * stretch > newlength)
                                    {
                                        //pattern is over, so stop playing
                                        stopPlaying = true;
                                        if (polytrigger[slot][instance] != -1)
                                        {
                                            currentPoly[slot]--;
                                            polytrigger[slot][instance] = -1;
                                            if (currentPoly[slot] == 0)
                                            {
                                                setParameterForSlot(kPlay, slot, 0.0);
                                                processGroups(slot);
                                            }
                                            lastPlayedIndex[slot][instance] = -1;
                                            lastLoopCount[slot][instance]   = -1;
                                            endHangingNotesInLoop(midiout, i, slot, instance);
                                        }
                                    }
                                    //handle beatshift>0:
                                    else if (looptime > getLoopEnd(slot))
                                        looptime -= newlength;
                                    //handle beatshift<0:
                                    else if (looptime < newstart)
                                        looptime += newlength;
                                }
                                else
                                {
                                    if (lastLoopCount[slot][instance] == -1)
                                    {
                                        //starting first loop
                                        startLoopCount[slot][instance] = (int) ceil(totalloopcount);
                                        lastLoopCount[slot][instance]  = 0;
                                    }
                                    else if (lastLoopCount[slot][instance] != loopcount)
                                    {
                                        if (targetNumLoops > 0 && loopcount == targetNumLoops)
                                        {
                                            lastLoopCount[slot][instance] = -1;
                                            stopPlaying                   = true;
                                            if (polytrigger[slot][instance] != -1)
                                            {
                                                currentPoly[slot]--;
                                                polytrigger[slot][instance] = -1;
                                                jassert(currentPoly[slot] >= 0);
                                                if (currentPoly[slot] == 0)
                                                {
                                                    setParameterForSlot(kPlay, slot, 0.0);
                                                    processGroups(slot);
                                                }
                                                lastPlayedIndex[slot][instance] = -1;
                                            }
                                            if (nextSlot > -1 && ! isSlotPlaying(nextSlot))
                                            {
                                                setParameterForSlot(kPlay, nextSlot, 1.0);
                                                processGroups(nextSlot);
                                                if (nextSlot < slot)
                                                {
                                                    polytrigger[nextSlot][0] = floatToMidi(getParameterForSlot(kRoot, nextSlot));
                                                    slot                     = nextSlot;
                                                    loop                     = &(programs[slot].loop);
                                                    channel                  = roundToInt(getParameterForSlot(kChannel, slot) * 16.0f) - 1;
                                                    lastPlayedIndex[slot][0] = -1;
                                                    not_done                 = true;
                                                }
                                            }
                                        }
                                        else
                                            lastLoopCount[slot][instance] = loopcount;
                                    }
                                }
                                if (instance == 0)
                                    loopPpq[slot] = looptime;
                                if (stopPlaying)
                                    break;
                                if (looptime < lastLoopTime)
                                {
                                    lastPlayedIndex[slot][instance] = -1;
                                    banana                          = 0;
                                }
                                else if (lastPlayedIndex[slot][instance] + 1 >= loop->getNumEvents())
                                {
                                    lastPlayedIndex[slot][instance] = -1;
                                    banana++;
                                }
                                else if (lastPlayedIndex[slot][instance] + 1 >= loop->getNextIndexAtTime((int) (getLoopEnd(slot) * 960.0))
                                         && banana < 3)
                                {
                                    lastPlayedIndex[slot][instance] = -1;
                                    banana++;
                                }
                                lastLoopTime = looptime;
                                //DBG("looptime="+String(looptime));
                                //==================================================================================
                                if (loop->getNumEvents() > 0 && banana < 9)
                                {
                                    const int loopTime960 = (int) (looptime * 960.0);
                                    bool stop             = false;
                                    int index;
                                    if (lastPlayedIndex[slot][instance] == -1)
                                    {
                                        index = loop->getNextIndexAtTime(loopTime960);
                                        if (loop->getEventTime(index) <= loopTime960)
                                        {
                                            lastPlayedIndex[slot][instance] = index - 1;
                                            banana++;
                                        }
                                    }
                                    else if (loop->getEventTime(lastPlayedIndex[slot][instance] + 1) <= loopTime960)
                                        index = lastPlayedIndex[slot][instance] + 1;
                                    else
                                        index = lastPlayedIndex[slot][instance];
                                    if (loop->getEventTime(index) > loopTime960)
                                    {
                                        //DBG("  "+String(loop->getEventTime(index))+ " "+String((int)(looptime*960.0)));
                                        stop = true;
                                    }
                                    if (lastPlayedIndex[slot][instance] >= index)
                                    {
                                        //DBG("  "+String(lastPlayedIndex[slot][instance])+" "+String(index));
                                        stop = true;
                                    }
                                    while (index < (loop->getNumEvents()) && ! stop)
                                    {
                                        if (loop->getEventTime(index) <= loopTime960)
                                        {
                                            if (lastPlayedIndex[slot][instance] < index)
                                            {
                                                DBG("playing loop event " + juce::String(index) + " at time " + juce::String(loop->getEventTime(index)));
                                                playLoopEvent(slot, instance, index, channel, stretch, i, samplesPerPpq, midiout);
                                                lastPlayedIndex[slot][instance] = index;
                                            }
                                            index++;
                                        }
                                        else
                                        {
                                            stop = true;
                                        }
                                    } //events
                                }
                            }
                        } //samples
                        if (stopAtSample < buffer.getNumSamples())
                        {
                            if (noteTrig == 0.0f)
                            {
                                currentPoly[slot]--;
                                polytrigger[slot][0] = -1;
                            }
                            lastPlayedIndex[slot][instance] = -1;
                            lastLoopCount[slot][instance]   = -1;
                            endHangingNotesInLoop(midiout, stopAtSample, slot, instance);
                            if (overdub)
                                setParameterForSlot(kRecord, slot, 0.f);
                        }

#endif
                    }
                } //instances

                //update GUI
                if (slot == curProgram && currentPoly[slot] > 0)
                {
                    info->s = "Playing (Poly=" + juce::String(currentPoly[slot]) + ", Loop=" + juce::String(1 + lastLoopCount[slot][0]) + ")";
                    info->sendChangeMessage();
                }
            }
        }
        //noteoff buffer for notes that extend past the end of the loop section
        processNoteOffBuffer(slot, midiout, buffer.getNumSamples());
        loop->addSequence(temploop[slot]);
        loop->updateMatchedPairs();
        temploop[slot].clear();
    } //slots

    midiMessages.clear();
    midiMessages = midiout;

    if (midiOutput)
        midiOutput->sendBlockOfMessages(midiMessages, juce::Time::getMillisecondCounterHiRes() + 1.0, getSampleRate());

    samples += buffer.getNumSamples();
}

void PizLooper::processNoteOffBuffer(int slot, juce::MidiBuffer& midiout, int numSamples)
{
    Loop* loop = &(programs[slot].loop);
    for (int i = noteOffBuffer[slot].size(); --i >= 0;)
    {
        LoopNote* e = &(noteOffBuffer[slot].getReference(i));
        if (loop->isNotePlaying(e->note, e->voice))
        {
            if (e->noteOffSample < numSamples)
            {
                jassert(e->noteOffSample >= 0);
                midiout.addEvent(juce::MidiMessage::noteOff(e->lastOutputChannel, e->lastOutputNoteNumber), jmax(0, e->noteOffSample));
                DBG("noteoff from buffer");
                --playingNote[e->lastOutputChannel - 1][e->lastOutputNoteNumber];
                loop->setNoteOff(e->note);
                noteOffBuffer[slot].remove(i);
            }
            else
            {
                e->subtractFromTime(numSamples);
                jassert(e->noteOffSample >= 0);
            }
        }
        else
        {
            DBG("unmatched noteoff " + juce::String(e->lastOutputNoteNumber) + " in buffer "
                + juce::String(noteOffBuffer[slot].size()));
            noteOffBuffer[slot].remove(i);
        }
    }
}

bool PizLooper::isNoteTriggeringAnySlot(juce::MidiMessage const& message)
{
    if (message.isNoteOnOrOff())
    {
        const int notenum = message.getNoteNumber();
        if (param[kSingleLoop] >= 0.5f)
        {
            //for "play active slot only" only trigger settings for current slot matter
            if (getParameterForSlot(kNoteTrig, curProgram) > 0.f
                && notenum >= floatToMidi(getParameterForSlot(kNLow, curProgram))
                && notenum <= floatToMidi(getParameterForSlot(kNHigh, curProgram))
                && message.isForChannel(message.isForChannel(roundToInt(getParameterForSlot(kTrigChan, curProgram) * 15.0f) + 1)))
                return true;
            if (getParameterForSlot(kUseScaleChannel, curProgram) >= 0.5
                && message.isForChannel(roundToInt(getParameterForSlot(kScaleChannel, curProgram) * 15.0f) + 1))
                return true;
            if (getParameterForSlot(kUseTrChannel, curProgram) >= 0.5
                && message.isForChannel(roundToInt(getParameterForSlot(kTransposeChannel, curProgram) * 15.0f) + 1))
                return true;
            return false;
        }
        for (int slot = 0; slot < numSlots; slot++)
        {
            if (getParameterForSlot(kNoteTrig, slot) > 0.f
                && notenum >= floatToMidi(getParameterForSlot(kNLow, slot))
                && notenum <= floatToMidi(getParameterForSlot(kNHigh, slot))
                && message.isForChannel(message.isForChannel(roundToInt(getParameterForSlot(kTrigChan, slot) * 15.0f) + 1)))
                return true;
            if (getParameterForSlot(kUseScaleChannel, slot) >= 0.5
                && message.isForChannel(roundToInt(getParameterForSlot(kScaleChannel, slot) * 15.0f) + 1))
                return true;
            if (getParameterForSlot(kUseTrChannel, slot) >= 0.5
                && message.isForChannel(roundToInt(getParameterForSlot(kTransposeChannel, slot) * 15.0f) + 1))
                return true;
        }
    }
    return false;
}

bool PizLooper::processTriggerNote(const int slot, juce::MidiMessage& message, int root, juce::MidiBuffer& midiout, const int sample, const double ppqOfNextBar, const double ppqPerSample, const double ppqOfLastStep, const double looplengthstep)
{
    if (! message.isForChannel(roundToInt(getParameterForSlot(kTrigChan, slot) * 15.0f) + 1))
        return false;

    if (param[kSingleLoop] >= 0.5f && curProgram != slot)
        return false;

    const float velosens  = getParameterForSlot(kVeloSens, slot);
    const float veloscale = velosens * message.getFloatVelocity() * 2.f + (1.f - velosens);
    const int notenum     = message.getNoteNumber();
    const float noteTrig  = getParameterForSlot(kNoteTrig, slot);
    const bool waitForBar = getParameterForSlot(kWaitForBar, slot) >= 0.5;
    bool usedNote         = false;
    if (message.isNoteOn())
    {
        const bool mono = ((noteTrig > 0.0f && noteTrig < 0.1f)
                           || (noteTrig >= 0.2f && noteTrig < 0.3f));

        const int nlow  = floatToMidi(getParameterForSlot(kNLow, slot));
        const int nhigh = floatToMidi(getParameterForSlot(kNHigh, slot));
        //midilearn
        if (nlow == -1)
        {
            setParameterForSlot(kNLow, slot, midiToFloat(notenum));
        }
        if (nhigh == -1)
        {
            setParameterForSlot(kNHigh, slot, midiToFloat(notenum));
        }
        if (root == -1)
        {
            setParameterForSlot(kRoot, slot, midiToFloat(notenum));
            root = notenum;
        }
        if (notenum >= nlow && notenum <= nhigh
            && noteTrig > 0.0f)
        {
            usedNote  = true;
            bool done = false;
            if (getParameterForSlot(kNoteToggle, slot) >= 0.5f)
            {
                //toggle mode, stop playing if it's already playing
                for (int v = 0; v < polyphony; v++)
                {
                    if (polytrigger[slot][v] == notenum)
                    {
                        polytrigger[slot][v] = -1;
                        currentPoly[slot]--;
                        if (currentPoly[slot] <= 0)
                        {
                            currentPoly[slot] = 0;
                            setParameterForSlot(kPlay, slot, 0.0f);
                            processGroups(slot);
                            lastplay[slot] = 0.f;
                        }
                        if (! waitForBar)
                        {
                            barStopSample[slot][v] = sample + samples;
                            endHangingNotesInLoop(midiout, sample, slot, v);
                            lastPlayedIndex[slot][v] = -1;
                            tRules[slot]->release(v);
                        }
                        else
                        {
                            barStopSample[slot][v] = (int) ((ppqOfNextBar) / ppqPerSample);
                        }
                        if (slot == curProgram)
                        {
                            info->s = "Stopped";
                            info->sendChangeMessage();
                        }
                        done = true;
                    }
                }
            }

            if (currentPoly[slot] < polyphony && ! done)
            {
                //start playing if there are free voices
                //todo: voice stealing
                int use = -12345;
                if (mono)
                {
                    currentPoly[slot] = 1;
                    use               = 0;
                }
                else
                {
                    for (int v = 0; v < polyphony; v++)
                    {
                        if (polytrigger[slot][v] == -1)
                        {
                            use = v;
                            break;
                        }
                    }
                }
                if (use != -12345)
                {
                    polytrigger[slot][use] = notenum;
                    if (! mono)
                        currentPoly[slot]++;
                    setParameterForSlot(kPlay, slot, 1.0f);
                    processGroups(slot);
                    lastplay[slot] = 1.f;
                    if (getParameterForSlot(kTrigger, slot) < 0.1f)
                    {
                        //start in sync
                        if (fmod(getLoopLength(slot), looplengthstep) > 0)
                        {
                            measurePlayFromHere[slot][use] = ppqOfLastStep;
                        }
                    }
                    else //sample accurate start
                        measurePlayFromHere[slot][use] = lastPosInfo.ppqPosition + sample * ppqPerSample;

                    if (! waitForBar)
                    {
                        barTriggerSample[slot][use] = (int) ((lastPosInfo.ppqPosition) / ppqPerSample);
                        tRules[slot]->trigger(notenum, veloscale, use);
                    }
                    else
                    {
                        barTriggerSample[slot][use] = (int) ((ppqOfNextBar) / ppqPerSample);
                        tRules[slot]->trigger(notenum, veloscale, use);
                        measurePlayFromHere[slot][use] = ppqOfNextBar;
                    }
                }
            }
        }
    }
    else if (message.isNoteOff())
    {
        for (int v = 0; v < polyphony; v++)
        {
            if (notenum == polytrigger[slot][v] && noteTrig > 0.0f)
            {
                usedNote = true;
                if (getParameterForSlot(kNoteToggle, slot) < 0.5f)
                {
                    //only use if not in toggle mode
                    polytrigger[slot][v] = -1;
                    currentPoly[slot]--;
                    if (currentPoly[slot] <= 0)
                    {
                        currentPoly[slot] = 0;
                        setParameterForSlot(kPlay, slot, 0.0f);
                        processGroups(slot);
                        lastplay[slot] = 0.f;
                    }
                    if (! waitForBar)
                    {
                        barStopSample[slot][v] = sample + samples;
                        endHangingNotesInLoop(midiout, sample, slot, v);
                        lastPlayedIndex[slot][v] = -1;
                        tRules[slot]->release(v);
                    }
                    else
                    {
                        barStopSample[slot][v] = (int) ((ppqOfNextBar) / ppqPerSample);
                    }
                    if (slot == curProgram)
                    {
                        info->s = "Stopped";
                        info->sendChangeMessage();
                    }
                }
            }
        }
    }
    return usedNote;
}

void PizLooper::playLoopEvent(int slot, int instance, int eventindex, int channel, double stretch, int samplepos, double samplesPerPpq, juce::MidiBuffer& midiout)
{
    Loop* loop               = &(programs[slot].loop);
    double eventTime         = loop->getEventTime(eventindex);
    Loop::mehPtr e           = loop->getEventPointer(eventindex);
    juce::MidiMessage msgout = e->message;
    const juce::uint8* data  = msgout.getRawData();
    if ((channel == -1
         || msgout.isForChannel(channel + 1)
         || getParameterForSlot(kFiltChan, slot) < 0.5f)
        && data[0] < 0xf0)
    {
        bool kill         = false;
        int note          = -1;
        int outputchannel = msgout.getChannel();
        if (getParameterForSlot(kFiltChan, slot) < 0.5f && channel != -1)
        {
            outputchannel = channel + 1;
        }
        if (msgout.isNoteOnOrOff())
        {
            note = msgout.getNoteNumber();
            jassert(note > -1);
            //if (msgout.isNoteOn() && loop->getTimeOfMatchingKeyUp(eventindex)<=loop->getEventTime(eventindex))
            //	kill=true;
            if (msgout.isNoteOn())
            {
                if (loop->isNotePlaying(e, instance))
                {
                    midiout.addEvent(juce::MidiMessage::noteOff(loop->getChannelOfNote(e, instance), loop->getTranspositionOfNote(e, instance)), jmax(0, samplepos - 1));
                    noteOffBuffer[slot].remove(getIndexOfNoteOff(e, slot, instance));
                    DBG("note " + juce::String(loop->getTranspositionOfNote(e, instance)) + " already playing"
                        + juce::String(noteOffBuffer[slot].size()));
                }
                int newNote          = tRules[slot]->getTransposedNote(note, instance, kill, outputchannel);
                const float velscale = tRules[slot]->velscale[instance];
                if (! kill)
                {
                    msgout.setChannel(outputchannel);
                    msgout.setNoteNumber(newNote);
                    msgout.multiplyVelocity(velscale * getParameterForSlot(kVelocity, slot) * 2.0f * getParameter(kMasterVelocity) * 2.0f);
                    int noteOffSample = samplepos + (int) (samplesPerPpq * (loop->getTimeOfMatchingKeyUp(eventindex) - loop->getEventTime(eventindex)) / (stretch * 960.0));
                    jassert(noteOffSample > samplepos);
                    if (loop->getIndexOfMatchingKeyUp(eventindex) != -1)
                    {
                        noteOffBuffer[slot].add(LoopNote(e, newNote, outputchannel, noteOffSample, instance));
                    }
                    loop->sentNoteAs(e, instance, newNote, outputchannel, noteOffSample);
                    playingNote[msgout.getChannel() - 1][msgout.getNoteNumber()] += 1;
                }
            }
            else if (msgout.isNoteOff())
            {
                //don't transpose noteoff differently from noteon!!!!!!!!!!!!!!!!!!!!
                if (loop->isNotePlaying(e, instance))
                {
                    msgout.setNoteNumber(loop->getTranspositionOfNote(e, instance));
                    msgout.setChannel(loop->getChannelOfNote(e, instance));
                    loop->setNoteOff(e, instance);
                    noteOffBuffer[slot].remove(getIndexOfNoteOff(e, slot, instance));
                    playingNote[msgout.getChannel() - 1][msgout.getNoteNumber()] -= 1;
                }
                else
                {
                    kill = true;
                    //DBG("- unmatched noteoff " + String(loop->getTranspositionOfNote(e,instance)) + " nob:"
                    //	+ String(noteOffBuffer[slot].size()));
                }
            }
        }
        if (! kill)
        {
            jassert(samplepos >= 0);
            if (msgout.isNoteOnOrOff())
            {
                jassert(playingNote[msgout.getChannel() - 1][msgout.getNoteNumber()] >= 0);
                playingNote[msgout.getChannel() - 1][msgout.getNoteNumber()] = jmax(0, playingNote[msgout.getChannel() - 1][msgout.getNoteNumber()]);
            }
            else
                msgout.setChannel(outputchannel);
            // this is where notes from the loop are added to the output!!!!
            jassert(samplepos >= 0);
            midiout.addEvent(msgout, samplepos);
            DBG("playing event");
            //if (slot==curProgram)
            //	kbstate.processNextMidiEvent(e->message);
        }
    }
}

double PizLooper::getLoopLengthStep()
{
    const double ppqPerBeat = 4.0 / (double) lastPosInfo.timeSigDenominator;
    if (getParameter(kRecStep) < 0.1f)
    {
        //loop in 1-bar increments
        return (double) lastPosInfo.timeSigNumerator * ppqPerBeat;
    }
    else if (param[kRecStep] < 0.2f)
    {
        return 3 * ppqPerBeat; //3-beat increments
    }
    else if (param[kRecStep] < 0.3f)
    {
        return 2 * ppqPerBeat; //2-beat increments
    }
    else if (param[kRecStep] < 0.4f)
    {
        return ppqPerBeat; //1-beat increments
    }
    else if (param[kRecStep] < 0.5f)
    {
        return 0.5; //8th note increments
    }
    else if (param[kRecStep] < 0.6f)
    {
        return 0.25; //16th note increments
    }
    else
    {
        return 0.0010416666666666666666666666666667; //1 tick (1/960)
    }
}

double PizLooper::getPpqOfLastLoopStart(double ppq, int slot)
{
    const double looplengthstep = getLoopLengthStep();
    const double numSteps       = (double) roundToInt(param[kFixedLength] * 32.f);
    if (programs[slot].looplength > numSteps * looplengthstep)
        return floor(ppq / (programs[slot].looplength)) * programs[slot].looplength;
    else if (numSteps)
        return floor(ppq / (looplengthstep * numSteps)) * looplengthstep * numSteps;
    else if (programs[slot].looplength)
        return floor(ppq / (programs[slot].looplength)) * programs[slot].looplength;
    else
        return floor(ppq / (looplengthstep)) * looplengthstep;
}

void PizLooper::recordMessage(const juce::MidiMessage& midi_message, int slot, bool playing, double ppq, double eventoffset, double ppqPerSample, int sample_number)
{
    const int eventchan = midi_message.getChannel() - 1;
    const int notenum   = midi_message.getNoteNumber();
    Loop* loop          = &(programs[slot].loop);
    if (midi_message.isNoteOff())
    {
        hanging[slot].count[eventchan][notenum] -= 1;
        if (hanging[slot].count[eventchan][notenum] <= 0)
        {
            hanging[slot].vel[eventchan][notenum]   = 0;
            hanging[slot].count[eventchan][notenum] = 0;
        }
    }
    else if (midi_message.isNoteOn())
    {
        hanging[slot].vel[eventchan][notenum] = midi_message.getVelocity();
        hanging[slot].count[eventchan][notenum] += 1;
    }
    if (getParameterForSlot(kRecord, slot) >= 0.5 && playing)
    {
        //add quantized event to loop (only quantizing notes)
        if (midi_message.isNoteOn())
        {
            recNoteOn[slot][eventchan][notenum] = (int) (measureDubFromHere[slot] * 960.0);
            if (getParameter(kQuantize) == 0.f)
            {
                eventoffset                      = ((double) sample_number) * ppqPerSample;
                loop->Qdelta[notenum][eventchan] = 0;
            }
            else
            {
                loop->Qdelta[notenum][eventchan] = eventoffset;
            }
            int t = roundToInt((ppq + eventoffset - measureDubFromHere[slot]) * 960.0);
            if (param[kRecMode] >= 0.5f && programs[slot].looplength && isSlotPlaying(slot))
            {
                t = roundToInt((fmod((ppq + eventoffset - measurePlayFromHere[slot][0]), programs[slot].looplength)) * 960.0);
                recNoteOn[slot][eventchan][notenum] += roundToInt((ppq + eventoffset - measureDubFromHere[slot]) * 960.0) - t;
            }
            if (t > (int) (960.0 * (programs[slot].looplength + programs[slot].loopstart)))
            {
                t -= (int) (960.0 * programs[slot].looplength);
                loop->Qdelta[notenum][eventchan] -= programs[slot].looplength;
            }
            else if (t < (int) (960.0 * programs[slot].loopstart))
            {
                t += (int) (960.0 * programs[slot].looplength);
                loop->Qdelta[notenum][eventchan] += programs[slot].looplength;
            }
            temploop[slot].addEvent(midi_message, t);
            temploop[slot].updateMatchedPairs();
        }
        else if (midi_message.isNoteOff())
        {
            if (recNoteOn[slot][eventchan][notenum] >= 0)
            {
                int t;
                if (getParameter(kQuantize) == 0.f)
                    t = (int) ((ppq + loop->Qdelta[notenum][eventchan] + ((double) sample_number) * ppqPerSample) * 960.0)
                      - recNoteOn[slot][eventchan][notenum];
                else
                    t = (int) ((ppq + loop->Qdelta[notenum][eventchan]) * 960.0)
                      - recNoteOn[slot][eventchan][notenum];
                jassert(t > 0);
                temploop[slot].addEvent(midi_message, t);
                temploop[slot].updateMatchedPairs();
                recNoteOn[slot][eventchan][notenum] = -1;
            }
        }
        else
        {
            int t = roundToInt((ppq + ((double) sample_number) * ppqPerSample) * 960.0) - (int) (measureDubFromHere[slot] * 960.0);
            jassert(t > 0);
            if (t > (int) (960.0 * (programs[slot].looplength + programs[slot].loopstart)))
            {
                t -= (int) (960.0 * programs[slot].looplength);
            }
            temploop[slot].addEvent(midi_message, t);
        }
    }
}
