#include "MidiChords.h"
#include "MidiChordsEditor.h"

//==============================================================================
/**
    This function must be implemented to create a new instance of your
    plugin object.
*/
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiChords();
}

//==============================================================================
MidiChordsPrograms::MidiChordsPrograms()
    : BankStorage ("midiChordsValues", 1, numProgs)
{
    loadDefaults();
}

void MidiChordsPrograms::loadDefaults()
{
    setGlobal ("lastProgram", 0);
    setGlobal ("Version", 4);
    for (int b = 0; b < getNumBanks(); b++)
    {
        for (int p = 0; p < getNumPrograms(); p++)
        {
            set (p, "progIndex", p);

            set (p, "Channel", 0);
            set (p, "LearnChannel", 0);
            set (p, "OutChannel", 0);
            set (p, "FollowInput", true);
            set (p, "Transpose", 0);
            set (p, "ChordMode", Normal);
            set (p, "Root", 60);
            set (p, "Guess", true);
            set (p, "Flats", false);
            set (p, "UsePC", false);
            set (p, "Velocity", 100);
            set (p, "InputTranspose", false);
            set (p, "ToAllChannels", false);

            set (p, "Strum", false);
            set (p, "Speed", 0.5f);
            set (p, "MaxDelay", 0.f);
            set (p, "VelRamp", 0.5f);
            set (p, "Accel", 0.5f);

            set (p, "Name", "Program " + String (p + 1));
            set (p, "lastUIWidth", 600);
            set (p, "lastUIHeight", 400);
            set (p, "lastTrigger", 60);

            set (p, "GuitarView", false);
            set (p, "NumStrings", 6);
            set (p, "NumFrets", 24);
            set (p, "String0", 64);
            set (p, "String1", 59);
            set (p, "String2", 55);
            set (p, "String3", 50);
            set (p, "String4", 45);
            set (p, "String5", 40);
            set (p, "String6", -1);
            set (p, "String7", -1);
            set (p, "String8", -1);
            set (p, "String9", -1);
            set (p, "String10", -1);
            set (p, "String11", -1);
            set (p, "String12", -1);
            set (p, "String13", -1);
            set (p, "String14", -1);
            set (p, "String15", -1);

            for (int t = 0; t < 128; t++)
            {
                set (p, "Bypassed" + String (t), false);
                set (p, "StrumUp" + String (t), true);
                ValueTree noteMatrix ("NoteMatrix_T" + String (t));
                //for (int c=0;c<16;c++) {
                //	noteMatrix.setProperty("Ch"+String(c)+"_0-31", 0, 0);
                //	noteMatrix.setProperty("Ch"+String(c)+"_32-63", 0, 0);
                //	noteMatrix.setProperty("Ch"+String(c)+"_64-95", 0, 0);
                //	noteMatrix.setProperty("Ch"+String(c)+"_96-127", 0, 0);
                //}
                values_.getChild (b).getChild (p).addChild (noteMatrix, t, 0);
            }
        }
    }
}

//==============================================================================
MidiChords::MidiChords() : programs (0), curProgram (0)
{
    DBG ("MidiChords()");

    dataPath = getCurrentPath() + File::getSeparatorString() + "midiChords";
    if (! File (dataPath).exists())
    {
        dataPath = File::getSpecialLocation (File::userApplicationDataDirectory).getFullPathName()
                 + File::getSeparatorString() + "pizmidi" + File::getSeparatorString() + "midiChords";
        if (! File (dataPath).exists())
        {
            dataPath = File::getSpecialLocation (File::commonApplicationDataDirectory).getFullPathName()
                     + File::getSeparatorString() + "pizmidi" + File::getSeparatorString() + "midiChords";
        }
    }

    demo = ! readKeyFile();
    fillChordDatabase();
    programs = new MidiChordsPrograms();
    memset (numChordNotes, 0, sizeof (numChordNotes));
    if (! loadDefaultFxb())
    {
        channel         = 0;
        learnchan       = 0;
        outchan         = 0;
        follow          = true;
        usepc           = false;
        transpose       = 0;
        mode            = Normal;
        root            = 60;
        guess           = true;
        flats           = false;
        inputtranspose  = false;
        ccToAllChannels = false;
        previewVel      = 100;
        lastUIWidth     = 600;
        lastUIHeight    = 400;
        for (int i = 0; i < 128; i++)
            selectChordNote (i, i, true, 1);
        selectTrigger (60);
    }
    memset (notePlaying, 0, sizeof (notePlaying));
    memset (chordNotePlaying, 0, sizeof (chordNotePlaying));
    memset (chordNotePlaying2, 0, sizeof (chordNotePlaying2));
    memset (savedGuitarVoicing, 0, sizeof (savedGuitarVoicing));

    memset (noteDelay, 0, sizeof (noteDelay));
    memset (noteOrigPos, 0, sizeof (noteOrigPos));
    totalSamples          = 0;
    expectingDelayedNotes = false;

    for (int c = 0; c < 16; c++)
        for (int i = 0; i < 128; i++)
            outputNote[c][i] = -1;
    learn       = false;
    learning    = 0;
    playFromGUI = playingFromGUI = stopPlayingFromGUI = false;
    playButtonTrigger                                 = curTrigger;
    init                                              = true;
    fillGuitarPresetList();
    setCurrentProgram (0);
}

MidiChords::~MidiChords()
{
    DBG ("~MidiChords()");
    if (programs)
        deleteAndZero (programs);
}

//==============================================================================
const String MidiChords::getName() const
{
    return JucePlugin_Name;
}

int MidiChords::getNumParameters()
{
    return numParams;
}

void MidiChords::setCurrentProgram (int index)
{
    //save non-parameter info to the old program, except the first time
    if (! init)
        copySettingsToProgram (curProgram);
    init = false;

    //then set the new program
    curProgram = index;
    programs->setGlobal ("lastProgram", index);
    channel         = programs->get (index, "Channel");
    learnchan       = programs->get (index, "LearnChannel");
    outchan         = programs->get (index, "OutChannel");
    follow          = programs->get (index, "FollowInput");
    transpose       = programs->get (index, "Transpose");
    mode            = programs->get (index, "ChordMode");
    root            = programs->get (index, "Root");
    guess           = programs->get (index, "Guess");
    flats           = programs->get (index, "Flats");
    usepc           = programs->get (index, "UsePC");
    previewVel      = programs->get (index, "Velocity");
    inputtranspose  = programs->get (index, "InputTranspose");
    ccToAllChannels = programs->get (index, "ToAllChannels");

    strum     = programs->get (index, "Strum");
    fSpeed    = programs->get (index, "Speed");
    fMaxDelay = programs->get (index, "MaxDelay");
    fVelRamp  = programs->get (index, "VelRamp");
    fAccel    = programs->get (index, "Accel");

    lastUIWidth  = programs->get (index, "lastUIWidth");
    lastUIHeight = programs->get (index, "lastUIHeight");
    learn        = false;

    for (int i = 0; i < 128; i++)
    {
        progKbState[curProgram][i].reset();
        for (int c = 1; c <= 16; c++)
        {
            for (int n = 0; n < 128; n++)
            {
                if (programs->getChordNote (index, i, c, n))
                {
                    progKbState[curProgram][i].noteOn (c, n, 1.f);
                }
            }
        }
    }
    selectTrigger (programs->get (index, "lastTrigger"));
    //if (getGuitarView())
    //	translateToGuitarChord();
    //else
    //	sendChangeMessage();
}

float MidiChords::getParameter (int index)
{
    switch (index)
    {
        case kChannel:
            return ((float) channel) / 16.f;
        case kLearnChannel:
            return ((float) learnchan) / 16.f;
        case kOutChannel:
            return ((float) outchan) / 16.f;
        case kVelocity:
            return ((float) (previewVel - 1)) / 126.f;
        case kUseProgCh:
            return usepc ? 1.f : 0.f;
        case kLearnChord:
            return learn ? 1.f : 0.f;
        case kFollowInput:
            return follow ? 1.f : 0.f;
        case kTranspose:
            return ((float) transpose + 48) / 96.f;
        case kMode:
            return (float) mode / (float) (numModes - 1);
        case kRoot:
            return (float) root / 127.f;
        case kGuess:
            return guess ? 1.f : 0.f;
        case kFlats:
            return flats ? 1.f : 0.f;
        case kInputTranspose:
            return inputtranspose ? 1.f : 0.f;
        case kToAllChannels:
            return ccToAllChannels ? 1.f : 0.f;
        case kStrum:
            return strum ? 1.f : 0.f;
        case kSpeed:
            return fSpeed;
        case kMaxDelay:
            return fMaxDelay;
        case kVelRamp:
            return fVelRamp;
        case kAccel:
            return fAccel;

        default:
            return 0.0f;
    }
}

void MidiChords::setParameter (int index, float newValue)
{
    if (index == kChannel)
    {
        channel = roundToInt (newValue * 16.f);
        sendChangeMessage();
    }
    else if (index == kLearnChannel)
    {
        learnchan = roundToInt (newValue * 16.f);
        sendChangeMessage();
    }
    else if (index == kOutChannel)
    {
        outchan = roundToInt (newValue * 16.f);
        sendChangeMessage();
    }
    else if (index == kVelocity)
    {
        previewVel = roundToInt (newValue * 126.f) + 1;
        sendChangeMessage();
    }
    else if (index == kUseProgCh)
    {
        usepc = newValue > 0.f;
        sendChangeMessage();
    }
    else if (index == kLearnChord)
    {
        learn    = newValue > 0.f;
        learning = 0;
        sendChangeMessage();
    }
    else if (index == kFollowInput)
    {
        follow = newValue > 0.f;
        sendChangeMessage();
    }
    else if (index == kTranspose)
    {
        int newTranspose = roundToInt (newValue * 96.f) - 48;
        if (transpose != newTranspose)
        {
            transpose = newTranspose;
            sendChangeMessage();
        }
    }
    else if (index == kMode)
    {
        if (getGuitarView())
            setSavedGuitarVoicing (true);
        mode = roundToInt (newValue * (numModes - 1));
        if (mode == Global)
            selectTrigger (root);
        else
            selectTrigger (curTrigger);
        sendChangeMessage();
    }
    else if (index == kRoot)
    {
        root = roundToInt (newValue * 127.f);
        if (mode == Global)
        {
            selectTrigger (root);
        }
        sendChangeMessage();
    }
    else if (index == kGuess)
    {
        guess = newValue > 0.f;
        sendChangeMessage();
    }
    else if (index == kFlats)
    {
        flats = newValue > 0.f;
        sendChangeMessage();
    }
    else if (index == kInputTranspose)
    {
        inputtranspose = newValue > 0.f;
        sendChangeMessage();
    }
    else if (index == kToAllChannels)
    {
        ccToAllChannels = newValue > 0.f;
        sendChangeMessage();
    }
    else if (index == kStrum)
    {
        strum = newValue > 0.f;
        sendChangeMessage();
    }
    else if (index == kSpeed)
    {
        fSpeed = newValue;
        sendChangeMessage();
    }
    else if (index == kMaxDelay)
    {
        fMaxDelay = newValue;
        sendChangeMessage();
    }
    else if (index == kVelRamp)
    {
        fVelRamp = newValue;
        sendChangeMessage();
    }
    else if (index == kAccel)
    {
        fAccel = newValue;
        sendChangeMessage();
    }
}

const String MidiChords::getParameterName (int index)
{
    if (index == kChannel)
        return "Channel";
    if (index == kLearnChannel)
        return "LearnChannel";
    if (index == kOutChannel)
        return "OutChannel";
    if (index == kUseProgCh)
        return "UseProgCh";
    if (index == kLearnChord)
        return "Learn";
    if (index == kFollowInput)
        return "FollowInput";
    if (index == kTranspose)
        return "Transpose";
    if (index == kMode)
        return "TriggerMode";
    if (index == kRoot)
        return "RootNote";
    if (index == kGuess)
        return "GuessChord";
    if (index == kFlats)
        return "Flats";
    if (index == kVelocity)
        return "PreviewVelocity";
    if (index == kInputTranspose)
        return "TranspInput";
    if (index == kToAllChannels)
        return "ToAllChannels";
    if (index == kStrum)
        return "Strum";
    if (index == kSpeed)
        return "Speed";
    if (index == kMaxDelay)
        return "MaxTime";
    if (index == kVelRamp)
        return "VelRamp";
    if (index == kAccel)
        return "Accel";
    return String();
}

const String MidiChords::getParameterText (int index)
{
    if (index == kChannel)
        return channel == 0 ? "Any" : String (channel);
    if (index == kLearnChannel)
        return learnchan == 0 ? "All" : String (learnchan);
    if (index == kOutChannel)
        return outchan == 0 ? "Multi" : String (outchan);
    if (index == kUseProgCh)
        return usepc ? "Yes" : "No";
    if (index == kLearnChord)
        return learn ? "Yes" : "No";
    if (index == kFollowInput)
        return follow ? "Yes" : "No";
    if (index == kTranspose)
    {
        if (transpose > 0)
            return "+" + String (transpose);
        return String (transpose);
    }
    if (index == kMode)
    {
        if (mode == Normal)
            return "Full";
        if (mode == Octave)
            return "Octave";
        if (mode == Global)
            return "Global";
    }
    if (index == kRoot)
        return String (root);
    if (index == kGuess)
        return guess ? "Yes" : "No";
    if (index == kFlats)
        return flats ? "Yes" : "No";
    if (index == kVelocity)
        return String (previewVel);
    if (index == kInputTranspose)
        return inputtranspose ? "Yes" : "No";
    if (index == kToAllChannels)
        return ccToAllChannels ? "Yes" : "No";
    if (index == kStrum)
        return strum ? "On" : "Off";
    if (index == kSpeed)
        return String (roundToInt (fSpeed * 100.f)) + "%";
    if (index == kMaxDelay)
        return String (roundToInt (1000.f * (0.1f + 2.9f * fMaxDelay))) + " ms";
    if (index == kVelRamp)
    {
        const int ramp = roundToInt (fVelRamp * 200.f) - 100;
        if (ramp > 0)
            return "+" + String (ramp);
        return String (ramp);
    }
    if (index == kAccel)
    {
        const int accel = roundToInt (fAccel * 200.f) - 100;
        if (accel > 0)
            return "+" + String (accel);
        return String (accel);
    }
    return String();
}

const String MidiChords::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String MidiChords::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool MidiChords::isInputChannelStereoPair (int index) const
{
    return true;
}

bool MidiChords::isOutputChannelStereoPair (int index) const
{
    return true;
}

//==============================================================================
void MidiChords::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void MidiChords::releaseResources()
{
    delayBuffer.clear();
}

void MidiChords::processBlock (AudioSampleBuffer& buffer,
                               MidiBuffer& midiMessages)
{
    bool wasplaying;
    AudioPlayHead::CurrentPositionInfo pos;
    if (getPlayHead() != 0 && getPlayHead()->getCurrentPosition (pos))
        wasplaying = lastPosInfo.isPlaying || lastPosInfo.isRecording;
    else
    { //no timeinfo
        zeromem (&pos, sizeof (pos));
        wasplaying             = true;
        pos.isPlaying          = true;
        pos.timeSigNumerator   = 4;
        pos.timeSigDenominator = 4;
        pos.bpm                = 120.0;
        pos.ppqPosition        = lastPosInfo.ppqPosition + buffer.getNumSamples() * (120.0 / (60.0 * getSampleRate()));
    }
    lastPosInfo           = pos;
    double samplesPerBeat = 60.0 * getSampleRate() / pos.bpm;
    totalSamples          = roundToInt (pos.ppqPosition * samplesPerBeat);
    if (delayBuffer.getNumEvents() == 0)
        expectingDelayedNotes = false;

    MidiBuffer output;
    if ((playFromGUI != playingFromGUI) || stopPlayingFromGUI)
    {
        int ch = jmax (channel, 1);
        if (playFromGUI && ! playingFromGUI)
        {
            midiMessages.addEvent (MidiMessage::noteOn (ch, curTrigger, (uint8) previewVel), 0);
            playingFromGUI    = true;
            playFromGUI       = false;
            playButtonTrigger = curTrigger;
        }

        else if (stopPlayingFromGUI)
        {
            midiMessages.addEvent (MidiMessage::noteOff (ch, playButtonTrigger), 0);
            playingFromGUI     = false;
            playFromGUI        = false;
            stopPlayingFromGUI = false;
        }
    }
    for (auto&& msgMetadata : midiMessages)
    {
        auto m      = msgMetadata.getMessage();
        auto sample = msgMetadata.samplePosition;

        bool blockOriginalEvent = false;
        if (m.isProgramChange() && usepc)
        { // && (m.isForChannel(channel) || channel==0)) {
            if (m.getProgramChangeNumber() < getNumPrograms())
                setCurrentProgram (m.getProgramChangeNumber());
        }
        if (channel == 0 || m.isForChannel (channel))
        {
            if (m.isNoteOn())
            {
                if (isNoteBypassed (m.getNoteNumber()))
                {
                    blockOriginalEvent = true;
                    output.addEvent (m, sample);
                    playingChord[m.getNoteNumber()].add (ChordNote (m.getChannel(), m.getNoteNumber()));
                    chordNotePlaying[m.getChannel()][m.getNoteNumber()] = true;
                }
                else
                {
                    blockOriginalEvent = true;
                    const int ch       = m.getChannel() - 1;
                    const int tnote    = inputtranspose ? (m.getNoteNumber() - transpose) : m.getNoteNumber();
                    const uint8 v      = m.getVelocity();
                    if (! learn)
                    {
                        bool triggered = false;
                        int trigger    = tnote;
                        if (mode == Octave)
                            trigger = 60 + tnote % 12;
                        else if (mode == Global)
                            trigger = root;
                        for (int c = 1; c <= 16; c++)
                        {
                            for (int n = 0; n < 128; n++)
                            {
                                bool usedNote = false;
                                if (progKbState[curProgram][trigger].isNoteOn (c, n) && ! usedNote)
                                {
                                    usedNote          = outchan > 0;
                                    int outputChannel = outchan == 0 ? c : outchan;
                                    triggered         = true;
                                    int newNote       = n + transpose;
                                    if (mode == Global)
                                        newNote += tnote - root;
                                    else if (mode == Octave)
                                        newNote += 12 * (tnote / 12 - 60 / 12);
                                    if (chordNotePlaying[outputChannel - 1][newNote])
                                    {
                                        if (! strum)
                                        {
                                            noteDelay[outputChannel - 1][newNote]   = 0;
                                            noteOrigPos[outputChannel - 1][newNote] = totalSamples + sample;
                                            ignoreNextNoteOff[outputChannel - 1].addIfNotAlreadyThere (newNote);
                                        }
                                        else
                                        {
                                            if (! expectingDelayedNotes)
                                                ignoreNextNoteOff[outputChannel - 1].addIfNotAlreadyThere (newNote);
                                        }
                                        playingChord[tnote].add (ChordNote (outputChannel, newNote));
                                    }
                                    else if (newNote < 128 && newNote >= 0)
                                    {
                                        if (! strum)
                                        {
                                            output.addEvent (MidiMessage::noteOn (outputChannel, newNote, v), sample);
                                            noteDelay[outputChannel - 1][newNote]        = 0;
                                            noteOrigPos[outputChannel - 1][newNote]      = totalSamples + sample;
                                            chordNotePlaying[outputChannel - 1][newNote] = true;
                                        }
                                        playingChord[tnote].add (ChordNote (outputChannel, newNote));
                                    }
                                }
                            }
                        }
                        if (strum)
                        {
                            int chordpos   = 0;
                            int heldnotes  = playingChord[tnote].size();
                            bool upstroke  = programs->get (curProgram, "StrumUp" + String (trigger));
                            float accel    = (2.f * fAccel - 1.f);
                            float maxmax   = (float) getSampleRate() * (0.1f + 2.9f * fMaxDelay);
                            float maxdelay = (1.f - fSpeed) * maxmax;

                            while (chordpos < heldnotes)
                            {
                                int p        = upstroke ? chordpos : (heldnotes - 1 - chordpos);
                                int delay    = 0;
                                int n        = playingChord[tnote][p].n;
                                int c        = playingChord[tnote][p].c;
                                int velocity = v;
                                if (playingChord[tnote].size() > 1)
                                {
                                    const float x = (float) (chordpos) / (float) (heldnotes - 1);
                                    delay         = roundToInt ((accel * 0.3f * sin (MathConstants<float>::pi * x) + x) * maxdelay);
                                    velocity += roundToInt ((2.f * fVelRamp - 1.f) * (x * 127.f - 64.f));
                                    velocity = jlimit (1, 127, velocity);
                                    //float veldelay = 1.f-(fVelToSpeed)*MIDI_TO_FLOAT(strumvel);
                                    //delay=roundToInt(veldelay*(float)delay);
                                }
                                if (delay > 0)
                                {
                                    if (chordNotePlaying[c - 1][n])
                                        delayBuffer.addEvent (MidiMessage::noteOff (c, n), sample + delay - 1);
                                    delayBuffer.addEvent (MidiMessage::noteOn (c, n, (uint8) velocity), sample + delay);
                                    expectingDelayedNotes = true;
                                }
                                else
                                {
                                    int s  = sample;
                                    int s2 = sample;
                                    if (chordNotePlaying[c - 1][n])
                                    {
                                        if (sample == 0)
                                            s2 += 1;
                                        if (sample == buffer.getNumSamples() - 1)
                                            s -= 1;
                                        output.addEvent (MidiMessage::noteOff (c, n), s);
                                    }
                                    output.addEvent (MidiMessage::noteOn (c, n, (uint8) velocity), s2);
                                    chordNotePlaying[c - 1][n] = true;
                                    sample                     = s2;
                                }
                                noteDelay[c - 1][n]   = delay;
                                noteOrigPos[c - 1][n] = totalSamples + sample;
                                chordpos++;
                            }
                        }

                        if (triggered)
                            sendChangeMessage();
                        notePlaying[ch][tnote] = true;
                        if (follow && mode != Global)
                            selectTrigger (trigger);
                    }
                }
            }
            else if (m.isNoteOff())
            {
                blockOriginalEvent = true;
                const int ch       = m.getChannel() - 1;
                const int note     = inputtranspose ? (m.getNoteNumber() - transpose) : m.getNoteNumber();
                if (! learn)
                {
                    int numNotes = playingChord[note].size();
                    if (numNotes == 0)
                    {
                        if (outputNote[ch][note] != -1)
                        {
                            output.addEvent (MidiMessage::noteOff (ch + 1, outputNote[ch][note]), sample);
                            outputNote[ch][note] = -1;
                        }
                    }
                    else
                    {
                        for (int i = 0; i < numNotes; i++)
                        {
                            const int chordNote = playingChord[note][i].n;
                            const int c         = playingChord[note][i].c - 1;

                            int delay = noteDelay[c][chordNote];
                            if (! expectingDelayedNotes && strum)
                            {
                                // strum time is shorter than the input note
                                delay = 0; //  (int)(totalSamples - noteOrigPos[c][chordNote])+sample +(int)(getSampleRate()/1000.f);
                            }
                            if ((noteDelay[c][chordNote] + noteOrigPos[c][chordNote])
                                >= (totalSamples + delay - (int) (getSampleRate() / 100.f)))
                            {
                                // strum time is longer than input note -> arpeggio
                                delay = noteDelay[c][chordNote] + (int) (getSampleRate() / 1000.f);
                            }
                            if (! ignoreNextNoteOff[c].contains (chordNote))
                            {
                                if (delay > 0)
                                {
                                    delayBuffer.addEvent (MidiMessage::noteOff (c + 1, chordNote), sample + delay);
                                }
                                else
                                {
                                    output.addEvent (MidiMessage::noteOff (c + 1, chordNote), sample);
                                    chordNotePlaying[c][chordNote] = false;
                                }
                            }
                            else
                                ignoreNextNoteOff[c].removeAllInstancesOf (chordNote);
                        }
                        playingChord[note].clear();
                    }
                    notePlaying[ch][note] = false;
                    sendChangeMessage();
                }
            }
            else if (m.getChannel() > 0)
            {
                if (ccToAllChannels)
                {
                    for (int ch = 1; ch <= 16; ch++)
                    {
                        m.setChannel (ch);
                        output.addEvent (m, sample);
                        blockOriginalEvent = true;
                    }
                }
                else
                    blockOriginalEvent = false;
            }
            else
                blockOriginalEvent = false;
        }

        if (learnchan == 0 || m.isForChannel (learnchan))
        {
            if (m.isNoteOn())
            {
                blockOriginalEvent = true;
                const int ch       = m.getChannel();
                const int tnote    = m.getNoteNumber();
                const uint8 v      = m.getVelocity();
                if (learn)
                {
                    if (learning == 0)
                    {
                        progKbState[curProgram][curTrigger].reset();
                        chordKbState.reset();
                    }
                    learning++;
                    selectChordNote (curTrigger, tnote, true, ch);
                    int newNote = tnote + transpose;
                    if (mode == Global)
                        newNote += tnote - root;
                    else if (mode == Octave)
                        newNote += 12 * (tnote % 12);
                    if (newNote < 128 && newNote >= 0)
                    {
                        output.addEvent (MidiMessage::noteOn (ch, newNote, v), sample);
                        outputNote[ch][tnote] = newNote;
                    }
                }
            }
            else if (m.isNoteOff())
            {
                blockOriginalEvent = true;
                const int ch       = m.getChannel();
                const int note     = m.getNoteNumber();
                if (learn)
                {
                    learning--;
                    if (outputNote[ch][note] < 128 && outputNote[ch][note] >= 0)
                    {
                        output.addEvent (MidiMessage::noteOff (ch, outputNote[ch][note]), sample);
                        outputNote[ch][note] = -1;
                    }
                    if (learning < 1)
                        setParameterNotifyingHost (kLearnChord, 0.f);
                }
            }
        }
        if (! blockOriginalEvent)
            output.addEvent (m, sample);
    }

    //process delay buffer----------------------------------------------------------
    expectingDelayedNotes = false;
    MidiBuffer newBuffer;
    for (auto&& msgMetadata : delayBuffer)
    {
        auto m      = msgMetadata.getMessage();
        auto sample = msgMetadata.samplePosition;

        if (sample < buffer.getNumSamples())
        {
            //event is due, send it
            if (m.isNoteOn())
            {
                if (chordNotePlaying[m.getChannel() - 1][m.getNoteNumber()])
                {
                    ignoreNextNoteOff[m.getChannel() - 1].add (m.getNoteNumber());
                    output.addEvent (MidiMessage::noteOff (m.getChannel(), m.getNoteNumber()), sample);
                }
                output.addEvent (m, sample);
                chordNotePlaying[m.getChannel() - 1][m.getNoteNumber()] = true;
            }
            else if (m.isNoteOff())
            {
                const int c = m.getChannel() - 1;
                const int n = m.getNoteNumber();
                if (! ignoreNextNoteOff[c].contains (n))
                {
                    output.addEvent (m, sample);
                    chordNotePlaying[c][n] = false;
                }
                else
                    ignoreNextNoteOff[c].removeAllInstancesOf (n);
            }
        }
        else
        {
            //not due yet
            newBuffer.addEvent (m, sample - buffer.getNumSamples());
            if (m.isNoteOn())
                expectingDelayedNotes = true;
        }
    }
    if (wasplaying && ! pos.isPlaying)
    {
        //
    }
    //put leftover messages back into the buffer
    delayBuffer.clear();
    delayBuffer = newBuffer;

    midiMessages.clear();
    midiMessages = output;

    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
AudioProcessorEditor* MidiChords::createEditor()
{
    return new MidiChordsEditor (this);
}

//==============================================================================
void MidiChords::getStateInformation (MemoryBlock& destData)
{
    copySettingsToProgram (curProgram);
    programs->dumpTo (destData);
}

void MidiChords::getCurrentProgramStateInformation (MemoryBlock& destData)
{
    copySettingsToProgram (curProgram);
    programs->dumpProgramTo (0, curProgram, destData);
}

void MidiChords::setStateInformation (const void* data, int sizeInBytes)
{
    MemoryInputStream m (data, sizeInBytes, false);
    ValueTree vt = ValueTree::readFromStream (m);

    programs->loadFrom (vt);
    if (vt.isValid())
    {
        if ((int) vt.getChildWithName ("GlobalSettings").getProperty ("Version", 0) >= 4)
        {
            programs->loadFrom (vt);
            programs->fixupVelocity();
        }
        else
        {
            for (int index = 0; index < vt.getNumChildren(); index++)
            {
                programs->set (index, "Channel", vt.getChild (index).getProperty ("Channel"));
                programs->set (index, "LearnChannel", vt.getChild (index).getProperty ("LearnChannel"));
                programs->set (index, "UsePC", vt.getChild (index).getProperty ("UsePC"));
                programs->set (index, "FollowInput", vt.getChild (index).getProperty ("FollowInput"));
                programs->set (index, "Name", vt.getChild (index).getProperty ("Name"));
                programs->set (index, "lastUIHeight", vt.getChild (index).getProperty ("lastUIHeight"));
                programs->set (index, "lastUIWidth", vt.getChild (index).getProperty ("lastUIWidth"));
                programs->set (index, "lastTrigger", vt.getChild (index).getProperty ("lastTrigger"));
                programs->set (index, "Transpose", vt.getChild (index).getProperty ("Transpose"));
                programs->set (index, "ChordMode", vt.getChild (index).getProperty ("ChordMode"));
                programs->set (index, "Root", vt.getChild (index).getProperty ("Root"));
                programs->set (index, "Guess", vt.getChild (index).getProperty ("Guess"));
                programs->set (index, "Flats", vt.getChild (index).getProperty ("Flats"));
                programs->set (index, "Velocity", 100);

                for (int trigger = 0; trigger < 128; trigger++)
                {
                    for (int ch = 0; ch < 16; ch++)
                    {
                        for (int n = 0; n < 128; n++)
                        {
                            if (n < 32)
                            {
                                int state = vt.getChild (index).getChild (ch).getProperty ("T" + String (trigger) + "_0-31", 0);
                                if ((state & (1 << n)) != 0)
                                    programs->setChordNote (index, trigger, ch + 1, n, true);
                            }
                            else if (n < 64)
                            {
                                int note  = n - 32;
                                int state = vt.getChild (index).getChild (ch).getProperty ("T" + String (trigger) + "_32-63", 0);
                                if ((state & (1 << note)) != 0)
                                    programs->setChordNote (index, trigger, ch + 1, n, true);
                            }
                            else if (n < 96)
                            {
                                int note  = n - 64;
                                int state = vt.getChild (index).getChild (ch).getProperty ("T" + String (trigger) + "_64-95", 0);
                                if ((state & (1 << note)) != 0)
                                    programs->setChordNote (index, trigger, ch + 1, n, true);
                            }
                            else if (n < 128)
                            {
                                int note  = n - 96;
                                int state = vt.getChild (index).getChild (ch).getProperty ("T" + String (trigger) + "_96-127", 0);
                                if ((state & (1 << note)) != 0)
                                    programs->setChordNote (index, trigger, ch + 1, n, true);
                            }
                        }
                    }
                }
            }
        }
    }
    init = true;
    setCurrentProgram (vt.getProperty ("lastProgram", 0));
}

void MidiChords::setCurrentProgramStateInformation (const void* data, int sizeInBytes)
{
    MemoryInputStream m (data, sizeInBytes, false);
    ValueTree vt = ValueTree::readFromStream (m);
    if ((int) vt.getProperty ("Velocity") == 0)
        vt.setProperty ("Velocity", 100, 0);

    bool successfully_loaded = programs->loadNoteMatrixFrom (vt, curProgram);
    if (! successfully_loaded)
    {
        programs->set (curProgram, "Channel", vt.getProperty ("Channel"));
        programs->set (curProgram, "LearnChannel", vt.getProperty ("LearnChannel"));
        programs->set (curProgram, "UsePC", vt.getProperty ("UsePC"));
        programs->set (curProgram, "FollowInput", vt.getProperty ("FollowInput"));
        programs->set (curProgram, "Name", vt.getProperty ("Name"));
        programs->set (curProgram, "lastUIHeight", vt.getProperty ("lastUIHeight"));
        programs->set (curProgram, "lastUIWidth", vt.getProperty ("lastUIWidth"));
        programs->set (curProgram, "lastTrigger", vt.getProperty ("lastTrigger"));
        programs->set (curProgram, "Transpose", vt.getProperty ("Transpose"));
        programs->set (curProgram, "ChordMode", vt.getProperty ("ChordMode"));
        programs->set (curProgram, "Root", vt.getProperty ("Root"));
        programs->set (curProgram, "Guess", vt.getProperty ("Guess"));
        programs->set (curProgram, "Flats", vt.getProperty ("Flats"));
        programs->set (curProgram, "Velocity", 100);

        programs->set (curProgram, "progIndex", curProgram);
        for (int trigger = 0; trigger < 128; trigger++)
        {
            programs->clearNoteMatrix (curProgram, trigger);
            for (int ch = 0; ch < 16; ch++)
            {
                for (int n = 0; n < 128; n++)
                {
                    if (n < 32)
                    {
                        int state = vt.getChild (curProgram).getChild (ch).getProperty ("T" + String (trigger) + "_0-31", 0);
                        if ((state & (1 << n)) != 0)
                            programs->setChordNote (curProgram, trigger, ch + 1, n, true);
                    }
                    else if (n < 64)
                    {
                        int note  = n - 32;
                        int state = vt.getChild (curProgram).getChild (ch).getProperty ("T" + String (trigger) + "_32-63", 0);
                        if ((state & (1 << note)) != 0)
                            programs->setChordNote (curProgram, trigger, ch + 1, n, true);
                    }
                    else if (n < 96)
                    {
                        int note  = n - 64;
                        int state = vt.getChild (curProgram).getChild (ch).getProperty ("T" + String (trigger) + "_64-95", 0);
                        if ((state & (1 << note)) != 0)
                            programs->setChordNote (curProgram, trigger, ch + 1, n, true);
                    }
                    else if (n < 128)
                    {
                        int note  = n - 96;
                        int state = vt.getChild (curProgram).getChild (ch).getProperty ("T" + String (trigger) + "_96-127", 0);
                        if ((state & (1 << note)) != 0)
                            programs->setChordNote (curProgram, trigger, ch + 1, n, true);
                    }
                }
            }
        }
    }
    init = true;
    setCurrentProgram (curProgram);
}

void MidiChords::selectTrigger (int index)
{
    if (getGuitarView())
        savedGuitarVoicing[curTrigger] = true;

    if (index >= 0 && index < 128)
    {
        if (mode == Octave)
            index = 60 + index % 12;
        triggerKbState.noteOff (1, curTrigger, 1.f);
        curTrigger = index;
        triggerKbState.noteOn (1, index, 1.f);
        chordKbState.reset();
        for (int c = 1; c <= 16; c++)
        {
            for (int i = 0; i < 128; i++)
                if (progKbState[curProgram][index].isNoteOn (c, i))
                    chordKbState.noteOn (c, i, 1.f);
        }
        programs->set (curProgram, "lastTrigger", index);
        if (getGuitarView())
            translateToGuitarChord();
        else
            sendChangeMessage();
    }
}

void MidiChords::selectChordNote (int index, int note, bool on, int ch)
{
    if (ch == -1)
        ch = jmax (1, learnchan);
    if (note >= 0 && note < 128)
    {
        if (on)
        {
            progKbState[curProgram][index].noteOn (ch, note, 1.f);
            if (index == curTrigger)
                chordKbState.noteOn (ch, note, 1.f);
            ++numChordNotes[curProgram][index];
        }
        else
        {
            progKbState[curProgram][index].noteOff (ch, note, 1.f);
            if (index == curTrigger)
                chordKbState.noteOff (ch, note, 1.f);
            --numChordNotes[curProgram][index];
        }
        programs->setChordNote (curProgram, index, ch, note, on);
        sendChangeMessage();
    }
}

void MidiChords::copySettingsToProgram (int index)
{
    programs->set (index, "Channel", channel);
    programs->set (index, "LearnChannel", learnchan);
    programs->set (index, "OutChannel", outchan);
    programs->set (index, "UsePC", usepc);
    programs->set (index, "FollowInput", follow);
    programs->set (index, "Name", getProgramName (index));
    programs->set (index, "lastUIHeight", lastUIHeight);
    programs->set (index, "lastUIWidth", lastUIWidth);
    programs->set (index, "lastTrigger", curTrigger);
    programs->set (index, "Transpose", transpose);
    programs->set (index, "ChordMode", mode);
    programs->set (index, "Root", root);
    programs->set (index, "Guess", guess);
    programs->set (index, "Flats", flats);
    programs->set (index, "Velocity", previewVel);
    programs->set (index, "InputTranspose", inputtranspose);
    programs->set (index, "ToAllChannels", ccToAllChannels);

    programs->set (index, "Strum", strum);
    programs->set (index, "Speed", fSpeed);
    programs->set (index, "MaxDelay", fMaxDelay);
    programs->set (index, "VelRamp", fVelRamp);
    programs->set (index, "Accel", fAccel);

    for (int i = 0; i < 128; i++)
    {
        programs->clearNoteMatrix (index, i);
        for (int c = 1; c <= 16; c++)
        {
            for (int n = 0; n < 128; n++)
            {
                if (progKbState[index][i].isNoteOn (c, n))
                    programs->setChordNote (index, i, c, n, true);
            }
        }
    }
}

void MidiChords::clearAllChords()
{
    for (int i = 0; i < 128; i++)
    {
        numChordNotes[curProgram][i] = 0;
        progKbState[curProgram][i].reset();
        programs->clearNoteMatrix (curProgram, i);
    }
    chordKbState.reset();
    sendChangeMessage();
}

void MidiChords::resetAllChords()
{
    int ch = jmax (1, learnchan);
    for (int i = 0; i < 128; i++)
    {
        numChordNotes[curProgram][i] = 0;
        progKbState[curProgram][i].reset();
        progKbState[curProgram][i].noteOn (ch, i, 1.f);
        programs->clearNoteMatrix (curProgram, i);
        programs->setChordNote (curProgram, i, ch, i, true);
    }
    chordKbState.reset();
    chordKbState.noteOn (ch, curTrigger, 1.f);
    if (getGuitarView())
        translateToGuitarChord();
    else
        sendChangeMessage();
}

void MidiChords::copyChordToAllTriggers (bool absolute)
{
    for (int i = 0; i < 128; i++)
    {
        if (i != curTrigger)
        {
            progKbState[curProgram][i].reset();
            programs->clearNoteMatrix (curProgram, i);
            for (int ch = 1; ch <= 16; ch++)
            {
                for (int n = 0; n < 128; n++)
                {
                    if (progKbState[curProgram][curTrigger].isNoteOn (ch, n))
                    {
                        if (absolute)
                        {
                            progKbState[curProgram][i].noteOn (ch, n, 1.f);
                            programs->setChordNote (curProgram, i, ch, n, true);
                        }
                        else
                        {
                            int note = n - curTrigger + i;
                            if (note >= 0 && note < 128)
                            {
                                progKbState[curProgram][i].noteOn (ch, note, 1.f);
                                programs->setChordNote (curProgram, i, ch, note, true);
                            }
                        }
                    }
                }
            }
        }
    }
}

void MidiChords::clearChord (int trigger)
{
    numChordNotes[curProgram][trigger] = 0;
    progKbState[curProgram][trigger].reset();
    programs->clearNoteMatrix (curProgram, trigger);
    if (trigger == curTrigger)
    {
        chordKbState.reset();
        sendChangeMessage();
    }
}

void MidiChords::resetChord (int trigger)
{
    int ch                             = jmax (1, learnchan);
    numChordNotes[curProgram][trigger] = 0;
    progKbState[curProgram][trigger].reset();
    progKbState[curProgram][trigger].noteOn (ch, trigger, 1.f);
    programs->clearNoteMatrix (curProgram, trigger);
    programs->setChordNote (curProgram, trigger, ch, trigger, true);
    if (trigger == curTrigger)
    {
        chordKbState.reset();
        chordKbState.noteOn (ch, curTrigger, 1.f);
        if (getGuitarView())
            translateToGuitarChord();
        else
            sendChangeMessage();
    }
}

void MidiChords::transposeAll (bool up)
{
    chordKbState.reset();
    for (int i = 0; i < 128; i++)
    {
        bool oldState[16][128];
        for (int c = 0; c < 16; c++)
        {
            for (int n = 0; n < 128; n++)
                oldState[c][n] = progKbState[curProgram][i].isNoteOn (c + 1, n);
        }
        progKbState[curProgram][i].reset();
        for (int c = 0; c < 16; c++)
        {
            for (int n = 0; n < 128; n++)
            {
                if (oldState[c][n])
                {
                    if (up)
                    {
                        progKbState[curProgram][i].noteOn (c + 1, n + 1, 1.f);
                        programs->setChordNote (curProgram, i, c + 1, n + 1, true);
                        if (i == curTrigger)
                            chordKbState.noteOn (c + 1, n + 1, 1.f);
                    }
                    else
                    {
                        progKbState[curProgram][i].noteOn (c + 1, n - 1, 1.f);
                        programs->setChordNote (curProgram, i, c + 1, n - 1, true);
                        if (i == curTrigger)
                            chordKbState.noteOn (c + 1, n - 1, 1.f);
                    }
                }
                else
                {
                    if (up)
                        programs->setChordNote (curProgram, i, c + 1, n + 1, false);
                    else
                        programs->setChordNote (curProgram, i, c + 1, n - 1, false);
                }
            }
        }
    }
    if (getGuitarView())
        translateToGuitarChord();
    else
        sendChangeMessage();
}

void MidiChords::transposeChord (int trigger, bool up)
{
    bool oldState[16][128];
    for (int c = 0; c < 16; c++)
    {
        for (int n = 0; n < 128; n++)
            oldState[c][n] = progKbState[curProgram][trigger].isNoteOn (c + 1, n);
    }
    progKbState[curProgram][trigger].reset();
    if (trigger == curTrigger)
        chordKbState.reset();
    for (int c = 0; c < 16; c++)
    {
        for (int n = 0; n < 128; n++)
        {
            if (oldState[c][n])
            {
                int newNote = n;
                if (up)
                {
                    newNote += 1;
                    progKbState[curProgram][trigger].noteOn (c + 1, newNote, 1.f);
                    programs->setChordNote (curProgram, trigger, c + 1, newNote, true);
                    if (trigger == curTrigger)
                        chordKbState.noteOn (c + 1, newNote, 1.f);
                }
                else
                {
                    newNote -= 1;
                    progKbState[curProgram][trigger].noteOn (c + 1, newNote, 1.f);
                    programs->setChordNote (curProgram, trigger, c + 1, newNote, true);
                    if (trigger == curTrigger)
                        chordKbState.noteOn (c + 1, newNote, 1.f);
                }
            }
            else
            {
                if (up)
                    programs->setChordNote (curProgram, trigger, c + 1, n + 1, false);
                else
                    programs->setChordNote (curProgram, trigger, c + 1, n - 1, false);
            }
        }
    }

    if (trigger == curTrigger)
    {
        if (getGuitarView())
            translateToGuitarChord();
        else
            sendChangeMessage();
    }
}

void MidiChords::transposeCurrentChordByOctave (bool up)
{
    bool oldState[16][128];
    for (int c = 0; c < 16; c++)
    {
        for (int n = 0; n < 128; n++)
            oldState[c][n] = progKbState[curProgram][curTrigger].isNoteOn (c + 1, n);
    }
    progKbState[curProgram][curTrigger].reset();
    chordKbState.reset();
    for (int c = 0; c < 16; c++)
    {
        for (int n = 0; n < 128; n++)
        {
            if (oldState[c][n])
            {
                if (up)
                {
                    progKbState[curProgram][curTrigger].noteOn (c + 1, n + 12, 1.f);
                    programs->setChordNote (curProgram, curTrigger, c + 1, n + 12, true);
                    chordKbState.noteOn (c + 1, n + 12, 1.f);
                }
                else
                {
                    progKbState[curProgram][curTrigger].noteOn (c + 1, n - 12, 1.f);
                    programs->setChordNote (curProgram, curTrigger, c + 1, n - 12, true);
                    chordKbState.noteOn (c + 1, n - 12, 1.f);
                }
            }
            else
            {
                if (up)
                    programs->setChordNote (curProgram, curTrigger, c + 1, n + 12, false);
                else
                    programs->setChordNote (curProgram, curTrigger, c + 1, n - 12, false);
            }
        }
    }
    if (getGuitarView())
        translateToGuitarChord();
    sendChangeMessage();
}

void MidiChords::applyChannelToChord()
{
    if (learnchan > 0 && ! getGuitarView())
    {
        programs->clearNoteMatrix (curProgram, curTrigger);
        chordKbState.reset();
        for (int n = 0; n < 128; n++)
        {
            if (progKbState[curProgram][curTrigger].isNoteOnForChannels (0xffff, n))
            {
                for (int ch = 1; ch <= 16; ch++)
                {
                    if (progKbState[curProgram][curTrigger].isNoteOn (ch, n))
                        progKbState[curProgram][curTrigger].noteOff (ch, n, 1.f);
                }
                progKbState[curProgram][curTrigger].noteOn (learnchan, n, 1.f);
                programs->setChordNote (curProgram, curTrigger, learnchan, n, true);
                chordKbState.noteOn (learnchan, n, 1.f);
            }
        }
        sendChangeMessage();
    }
}

void MidiChords::savePreset (String name)
{
    String contents = "Mode:";
    switch (mode)
    {
        case Global:
            contents += "Global\n";
            break;
        case Octave:
            contents += "Octave\n";
            break;
        case Normal:
        default:
            contents += "Full\n";
            break;
    }
    for (int t = 0; t < 128; t++)
    {
        bool usingTrigger = false;
        for (int n = 0; n < 128; n++)
        {
            for (int c = 1; c <= 16; c++)
            {
                if (progKbState[curProgram][t].isNoteOn (c, n))
                {
                    if (! usingTrigger)
                    {
                        contents += String (t) + ":";
                        if (getGuitarView())
                            contents += " guitar";
                        usingTrigger = true;
                    }
                    contents += String (" ") + String (n - t) + "." + String (c);
                }
            }
        }
        if (usingTrigger)
            contents += "\n";
    }
    File chordFile (dataPath + File::getSeparatorString() + "mappings" + File::getSeparatorString() + name + ".chords");

    FileChooser myChooser ("Save preset...", chordFile, "*.chords");

    if (myChooser.browseForFileToSave (true))
    {
        File file (myChooser.getResult());
        if (! file.hasFileExtension ("chords"))
            file = file.withFileExtension ("chords");

        file.create();
        file.replaceWithText (contents);
    }
}

bool MidiChords::readKeyFile (File file)
{
    DBG ("readKeyFile()");
    bool copy = false;
    if (file.exists())
    {
        copy = true;
    }
    else
    {
        file = File (dataPath + File::getSeparatorString() + "midiChordsKey.txt");
        if (! file.exists())
            file = File (getCurrentPath() + File::getSeparatorString() + "midiChordsKey.txt");
        //if (!file.exists())
        //	file = File(folderPath+File::getSeparatorString()+"midiChordsKey.txt");
    }

    if (file.exists())
    {
        if (file.hasFileExtension ("zip"))
        {
            if (ZipFile (file).uncompressTo (File (dataPath)))
                file = File (dataPath + File::getSeparatorString() + "midiChordsKey.txt");
        }
        auto xmlKey = decodeEncryptedXml (file.loadFileAsString(), "3,5097efd7266b329e878e65129df0c8fe3ffc188abd24625e3709c65b304f5e2bdfa157e6c3cc6de5c29e36138c29f2516b56c55827aa238135bf1e23b6cab4f7");
        if (! xmlKey->getStringAttribute ("product").compare ("midiChords 1.x")
            && xmlKey->getStringAttribute ("username").isNotEmpty()
            && xmlKey->getStringAttribute ("time").isNotEmpty())
        {
            demo = false;
            //infoString = "Registered to " + xmlKey->getStringAttribute("username");
            if (copy)
                file.copyFileTo (File (dataPath + File::getSeparatorString() + "midiChordsKey.txt"));
            sendChangeMessage();
        }
        else
        {
            //infoString = "Bad midiChordsKey";
            sendChangeMessage();
        }
        return true;
    }
    return false;
}

void MidiChords::readChorderPreset (File file)
{
    XmlDocument xmlDoc (file);
    auto e (xmlDoc.getDocumentElement());
    if (e != nullptr)
    {
        MemoryBlock data;
        data.loadFromHexString (e->getAllSubText());

        struct ChorderMapping
        {
            uint32 header0;
            uint8 kbSect0[16];
            uint32 header1;
            uint8 kbSect1[16];
            uint32 header2;
            uint8 kbSect2[16];
            uint32 header3;
            uint8 kbSect3[16];
            uint32 header4;
            uint8 kbSect4[16];
            uint32 header5;
            uint8 kbSect5[16];
            uint32 header6;
            uint8 kbSect6[16];
            uint32 header7;
            uint8 kbSect7[16];
        };

        int ch = jmax (learnchan, 1);
        //chordKbState.reset();
        ChorderMapping map;
        for (int i = 0; i < 128; i++)
        {
            data.copyTo (&map, i * sizeof (map), sizeof (map));
            progKbState[curProgram][i].reset();
            programs->clearNoteMatrix (curProgram, i);
            for (int m = 0; m < 16; m++)
            {
                for (int n = 0; n < 8; n++)
                {
                    if (map.kbSect0[m] & (1 << n))
                    {
                        progKbState[curProgram][i].noteOn (ch, n + m * 8, 1.f);
                        //chordKbState.noteOn(ch,n+m*8,1.f);
                        programs->setChordNote (curProgram, i, ch, n + m * 8, true);
                    }
                }
            }
        }
        int otherSettings[5]; // 0: ?, 1: mode, 2: Layer switch, 3: Thru, 4: Used Layers
        data.copyTo (&otherSettings, 128 * sizeof (map), sizeof (otherSettings));

        if (otherSettings[1] == 0)
            mode = Normal;
        else if (otherSettings[1] == 1)
            mode = Octave;
        else if (otherSettings[1] == 2)
            mode = Global;
        programs->set (curProgram, "ChordMode", mode);
        changeProgramName (curProgram, file.getFileNameWithoutExtension());

        selectTrigger (curTrigger);
    }
}

void MidiChords::translateToGuitarChord (bool force)
{
    bool doIt = force;
    if (! force)
    {
        int guitarChord[maxStrings];
        for (int s = 0; s < maxStrings; s++)
            guitarChord[s] = -1;
        bool oldState[16][128];
        for (int c = 0; c < 16; c++)
        {
            for (int n = 0; n < 128; n++)
            {
                oldState[c][n] = progKbState[curProgram][curTrigger].isNoteOn (c + 1, n);
                if (oldState[c][n] && getGuitarView() && c < 12)
                {
                    if (guitarChord[c] >= 0)
                    {
                        doIt = true;
                        break;
                    }
                    guitarChord[c] = n;
                }
            }
        }
        for (int s = 0; s < maxStrings; s++)
        {
            if (guitarChord[s] >= 0)
            {
                if (guitarChord[s] > getStringValue (s) + getNumFrets()
                    || guitarChord[s] < getStringValue (s)
                    || s >= getNumStrings())
                {
                    doIt = true;
                    break;
                }
            }
        }
    }
    if (! doIt)
    {
        sendChangeMessage();
        return;
    }

    bool tempState[16][128];
    memset (tempState, 0, sizeof (tempState));
    const int numFrets   = (int) programs->get (curProgram, "NumFrets");
    const int numStrings = (int) programs->get (curProgram, "NumStrings");
    Array<int> unusedNotes;
    Array<int> unusedStrings;
    for (int s = 0; s < numStrings; s++)
        unusedStrings.add (s);

    for (int n = 127; n >= 0; n--)
    {
        if (progKbState[curProgram][curTrigger].isNoteOnForChannels (0xffff, n))
        {
            int zeroFretNote;
            bool foundString = false;
            int s            = 0;
            while (s < numStrings && ! foundString)
            {
                zeroFretNote = (int) programs->get (curProgram, "String" + String (s));
                if (n >= zeroFretNote && n <= zeroFretNote + numFrets && unusedStrings.contains (s))
                {
                    tempState[s][n] = true;
                    foundString     = true;
                    unusedStrings.removeAllInstancesOf (s);
                }
                s++;
            }
            if (! foundString)
            {
                unusedNotes.addIfNotAlreadyThere (n);
            }
        }
    }

    if (unusedStrings.size() > 0)
    {
        for (int n = 0; n < unusedNotes.size(); n++)
        {
            for (int i = 0; i < unusedStrings.size(); i++)
            {
                int zeroFretNote = (int) programs->get (curProgram, "String" + String (unusedStrings[i]));
                if (n >= zeroFretNote && n <= zeroFretNote + numFrets && unusedStrings.contains (0))
                {
                    tempState[unusedStrings[i]][n] = true;
                    unusedStrings.removeAllInstancesOf (0);
                }
            }
        }
    }

    //todo: redistribute notes if more strings available and stretching over too many frets

    progKbState[curProgram][curTrigger].reset();
    chordKbState.reset();
    programs->clearNoteMatrix (curProgram, curTrigger);
    for (int c = 0; c < 16; c++)
    {
        for (int n = 0; n < 128; n++)
        {
            if (tempState[c][n])
            {
                progKbState[curProgram][curTrigger].noteOn (c + 1, n, 1.f);
                programs->setChordNote (curProgram, curTrigger, c + 1, n, true);
                chordKbState.noteOn (c + 1, n, 1.f);
            }
        }
    }
    sendChangeMessage();
}

void MidiChords::fillGuitarPresetList()
{
    guitarPresets.clearQuick();
    File chordPath (dataPath + File::getSeparatorString() + "guitars");
    Array<File> files;
    chordPath.findChildFiles (files, File::findFiles, true);
    for (int i = 0; i < files.size(); i++)
    {
        StringArray lines;
        lines.addLines (files[i].loadFileAsString());
        lines.sort (true);
        for (int line = 0; line < lines.size(); line++)
        {
            if (lines[line].trim().isNotEmpty())
            {
                if (! lines[line].startsWithChar (';'))
                {
                    StringArray s;
                    s.addTokens (lines[line], ",", "\"");
                    Array<int> notes;
                    for (int n = 3; n < s.size(); n++)
                        notes.add (s[n].trim().getIntValue());
                    guitarPresets.add (GuitarDefinition (s[0].trim().removeCharacters ("\""), s[1].trim(), s[2].trim().upToFirstOccurrenceOf (" frets", false, true).getIntValue(), notes));
                }
            }
        }
    }
}

void MidiChords::toggleUsePC (bool use)
{
    for (int i = 0; i < getNumPrograms(); i++)
        programs->set (i, "UsePC", use);

    setParameterNotifyingHost (kUseProgCh, use ? 1.f : 0.f);
}
