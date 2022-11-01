#include "midiDelay.hpp"

//for fmod()
#include <cmath>

//for seeding rand()
#include <time.h>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiDelay(audioMaster);
}

MidiDelayProgram::MidiDelayProgram()
{
    // default Program Values
    for (int i = 0; i < kNumParams; i++)
    {
        switch (i)
        {
            case kMode:
                param[kMode] = 0.1f;
                break;
            case kChannel:
                param[kChannel] = 0.0f;
                break;
            case kTime:
                param[kTime] = 0.55f;
                break;
            case kDry:
                param[kDry] = 1.0f;
                break;
            case kSync:
                param[kSync] = 1.0f;
                break;
            default:
                param[i] = 0.f;
                break;
        }
    }
    // default program name
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiDelay::MidiDelay(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiDelayProgram[numPrograms];
    if (programs)
    {
        CFxBank* defaultBank = new CFxBank(kNumPrograms, kNumParams);
        if (readDefaultBank(PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
                {
                    for (int p = 0; p < kNumParams; p++)
                    {
                        programs[i].param[p] = defaultBank->GetProgParm(i, p);
                    }
                    strcpy(programs[i].name, defaultBank->GetProgramName(i));
                }
            }
        }
        else
        {
            // built-in programs
            for (int i = 0; i < kNumPrograms; i++)
            {
                sprintf(programs[i].name, "Program %d", i + 1);
            }
        }
        setProgram(0);
    }

    memset(expectingDelayedNoteOff, 0, sizeof(expectingDelayedNoteOff));
    memset(expectingDelayedNoteOn, 0, sizeof(expectingDelayedNoteOn));
    memset(notePlaying, 0, sizeof(notePlaying));
    memset(noteKilled, 0, sizeof(noteKilled));

    for (int n = 0; n < 128; n++)
    {
        for (int ch = 0; ch < 16; ch++)
        {
            lastout[n][ch] = -1;
        }
    }
    srand((unsigned int) time(NULL));

    _ppq           = 0.0;
    _bpm           = 120.0f;
    _beatpos       = 0.0;
    samplesPerBeat = 0;

    wasplaying = false;
    isplaying  = false;

    init();
}

//-----------------------------------------------------------------------------------------
MidiDelay::~MidiDelay()
{
    cleanMidiDelayBuffer();
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiDelay::setProgram(VstInt32 program)
{
    MidiDelayProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter(i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiDelay::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiDelay::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiDelay::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiDelay::resume()
{
    midiDelayBuffer.clear();
    PizMidi::resume();
}

//-----------------------------------------------------------------------------------------
void MidiDelay::setParameter(VstInt32 index, float value)
{
    if (index < kNumParams)
    {
        MidiDelayProgram* ap = &programs[curProgram];
        param[index] = ap->param[index] = value;
        if (index == kTime)
        {
            const float inc = 1.f / 21.f;
            if (param[kTime] < 1 * inc)
            {
                stepsize = 12.0f; //0.3333333 (3 bars)
            }
            else if (param[kTime] < 2 * inc)
            {
                stepsize = 8.0f; //0.5 (2 whole notes)
            }
            else if (param[kTime] < 3 * inc)
            {
                stepsize = 6.0f; //0.75 (dotted whole)
            }
            else if (param[kTime] < 4 * inc)
            {
                stepsize = 4.0f; //1 (whole note)
            }
            else if (param[kTime] < 5 * inc)
            {
                stepsize = 3.0f; //1.5 dotted half
            }
            else if (param[kTime] < 6 * inc)
            {
                stepsize = 2.0f; //2 (half note)
            }
            else if (param[kTime] < 7 * inc)
            {
                stepsize = 1.5f; //4.
            }
            else if (param[kTime] < 8 * inc)
            {
                stepsize = 1.333333333f; //2T
            }
            else if (param[kTime] < 9 * inc)
            {
                stepsize = 1.0f; //4
            }
            else if (param[kTime] < 10 * inc)
            {
                stepsize = 0.75f; //8.
            }
            else if (param[kTime] < 11 * inc)
            {
                stepsize = 0.666666667f; //4T
            }
            else if (param[kTime] < 12 * inc)
            {
                stepsize = 0.5f; //8
            }
            else if (param[kTime] < 13 * inc)
            {
                stepsize = 0.375; //16.
            }
            else if (param[kTime] < 14 * inc)
            {
                stepsize = 0.333333333f; //8T
            }
            else if (param[kTime] < 15 * inc)
            {
                stepsize = 0.25f; //16
            }
            else if (param[kTime] < 16 * inc)
            {
                stepsize = 0.1875f; //32.
            }
            else if (param[kTime] < 17 * inc)
            {
                stepsize = 0.166666667f; //16T
            }
            else if (param[kTime] < 18 * inc)
            {
                stepsize = 0.125f; //32
            }
            else if (param[kTime] < 19 * inc)
            {
                stepsize = 0.09375f; //64.
            }
            else if (param[kTime] < 20 * inc)
            {
                stepsize = 0.083333333f; //32T
            }
            else
            {
                stepsize = 0.0625f; //64
            }
        }
        else if (index == kSync)
        {
            updateDisplay();
        }
    }
}

//-----------------------------------------------------------------------------------------
float MidiDelay::getParameter(VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiDelay::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kSync:
            strcpy(label, "TempoSync");
            break;
        case kMode:
            strcpy(label, "Mode");
            break;
        case kTime:
            strcpy(label, "DelayTime");
            break;
        case kChannel:
            strcpy(label, "Channel");
            break;
        case kDry:
            strcpy(label, "Dry");
            break;
        case kWet:
            strcpy(label, "Wet");
            break;
        case kFeedback:
            strcpy(label, "Feedback");
            break;
        case kLimit:
            strcpy(label, "FdbkLimit");
            break;
        default:
            strcpy(label, " ");
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiDelay::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kFeedback:
        case kDry:
        case kWet:
            sprintf(text, "%d %%", roundToInt(param[index] * 100.f));
            break;

        case kMode:
            if (param[index] == 0.0)
            {
                strcpy(text, "Off");
            }
            else if (param[index] < 0.5)
            {
                strcpy(text, "Notes Only");
            }
            else
            {
                strcpy(text, "Notes & CCs");
            }
            break;
        case kLimit:
            if (param[index] == 0.0)
            {
                strcpy(text, "Off");
            }
            else
            {
                sprintf(text, "%d", roundToInt(param[index] * 15.f) + 1);
            }
            break;
        case kSync:
            if (param[index] < 0.5f)
            {
                strcpy(text, "Off");
            }
            else
            {
                strcpy(text, "On");
            }
            break;
        case kTime:
            if (param[kSync] >= 0.5f)
            {
                const float inc = 1.f / 21.f;
                if (param[index] < 1 * inc)
                {
                    strcpy(text, "3bars");
                }
                else if (param[index] < 2 * inc)
                {
                    strcpy(text, "2bars");
                }
                else if (param[index] < 3 * inc)
                {
                    strcpy(text, "dotted1");
                }
                else if (param[index] < 4 * inc)
                {
                    strcpy(text, "1");
                }
                else if (param[index] < 5 * inc)
                {
                    strcpy(text, "dotted2");
                }
                else if (param[index] < 6 * inc)
                {
                    strcpy(text, "2");
                }
                else if (param[index] < 7 * inc)
                {
                    strcpy(text, "dotted4");
                }
                else if (param[index] < 8 * inc)
                {
                    strcpy(text, "2T");
                }
                else if (param[index] < 9 * inc)
                {
                    strcpy(text, "4");
                }
                else if (param[index] < 10 * inc)
                {
                    strcpy(text, "dotted8");
                }
                else if (param[index] < 11 * inc)
                {
                    strcpy(text, "4T");
                }
                else if (param[index] < 12 * inc)
                {
                    strcpy(text, "8");
                }
                else if (param[index] < 13 * inc)
                {
                    strcpy(text, "dotted16");
                }
                else if (param[index] < 14 * inc)
                {
                    strcpy(text, "8T");
                }
                else if (param[index] < 15 * inc)
                {
                    strcpy(text, "16");
                }
                else if (param[index] < 16 * inc)
                {
                    strcpy(text, "dotted32");
                }
                else if (param[index] < 17 * inc)
                {
                    strcpy(text, "16T");
                }
                else if (param[index] < 18 * inc)
                {
                    strcpy(text, "32");
                }
                else if (param[index] < 19 * inc)
                {
                    strcpy(text, "dotted64");
                }
                else if (param[index] < 20 * inc)
                {
                    strcpy(text, "32T");
                }
                else
                {
                    strcpy(text, "64");
                }
            }
            else
            {
                sprintf(text, "%d ms", roundToInt((1.f - param[index]) * 2000.f));
            }
            break;
        case kChannel:
            if (FLOAT_TO_CHANNEL(param[index]) == -1)
            {
                strcpy(text, "All");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL(param[index]) + 1);
            }
            break;
        default:
            sprintf(text, "%f", param[index]);
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiDelay::cleanMidiDelayBuffer()
{
    midiDelayBuffer.clear();
}

void MidiDelay::preProcess(void)
{
    // preparing Process
    VstTimeInfo* timeInfo = NULL;
    timeInfo              = getTimeInfo(0xffff); //ALL

    if (timeInfo)
    {
        if (kVstTempoValid & timeInfo->flags)
        {
            _bpm = (float) timeInfo->tempo;
        }
        if (kVstPpqPosValid & timeInfo->flags)
        {
            _ppq = timeInfo->ppqPos;
        }
        if (kVstTransportPlaying & timeInfo->flags)
        {
            isplaying = true;
        }
        else
        {
            isplaying = false;
        }
        //dbg("tempo=" << _bpm);

        //        _ppq += (1.0f-2.0f*param[kOffset])/(stepsize*2.0f);
        _beatpos = stepsize * fmod(_ppq, 1.0 / stepsize);
    }

    samplesPerBeat = roundToInt(60.f * sampleRate / _bpm);
    samplesPerTick = roundToInt((float) samplesPerBeat / 960.0f);
    totalSamples   = roundToInt(_ppq * (double) samplesPerBeat);

    _cleanMidiOutBuffers();

    sortMidiEvents(midiDelayBuffer);
}

void MidiDelay::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples)
{
    short listenchannel = FLOAT_TO_CHANNEL(param[kChannel]);

    VstInt32 delay = roundToInt((float) samplesPerBeat * stepsize);
    if (param[kSync] < 0.5f)
    {
        delay = roundToInt(((1.f - param[kTime]) * 1.98f + 0.02f) * sampleRate);
    }

    //process incoming events-------------------------------------------------------
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        int status  = tomod.midiData[0] & 0xf0; // scraping  channel
        int channel = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        int data1   = tomod.midiData[1] & 0x7f;
        int data2   = tomod.midiData[2] & 0x7f;

        dbg("incoming midi event, i=" << i << " status=" << status << " ch=" << channel);

        //make 0-velocity notes into "real" noteoffs for simplicity
        if (status == MIDI_NOTEON && data2 == 0)
        {
            status            = MIDI_NOTEOFF;
            tomod.midiData[0] = MIDI_NOTEOFF | channel;
        }

        bool discard = false;

        //only look at the selected channel
        if (channel == listenchannel || listenchannel == -1 && param[kMode] > 0.f)
        {
            //incoming NoteOff--------------------------------------------------------------
            if (status == MIDI_NOTEOFF)
            {
                if (param[kWet] > 0.f)
                {
                    VstMidiEvent delayed = tomod;
                    if (samples - delayed.deltaFrames - delay > 0)
                    {
                        // delayed note is within the current block, don't need counter
                        delayed.deltaFrames += delay;
                        dbg("delay noteoff within block, deltaFrames=" << delayed.deltaFrames);
                        delayed.midiData[2] = roundToInt((float) data2 * param[kWet]);
                        if (! noteKilled[data1][channel])
                        {
                            outputs[0].push_back(delayed);
                            notePlaying[data1][channel] = false;
                        }
                        else
                        {
                            noteKilled[data1][channel] = false;
                        }
                        delayed.deltaFrames = (delay - (samples - tomod.deltaFrames));
                        if (samples - delayed.deltaFrames - delay <= 0)
                        {
                            delayed.midiData[3] = 127;
                            delayed.flags       = roundToInt(param[kLimit] * 15.f) + 1;
                            if (param[kLimit] == 0.0f)
                            {
                                delayed.flags = 127;
                            }
                            midiDelayBuffer.push_back(delayed);
                        }
                    }
                    else
                    {
                        expectingDelayedNoteOff[data1][channel]++;
                        // delayed note is in a future block, so start counter:
                        discard             = true; //takes the event out of the current block
                        delayed.deltaFrames = (delay - (samples - delayed.deltaFrames));
                        delayed.midiData[3] = 127;
                        delayed.flags       = roundToInt(param[kLimit] * 15.f) + 1;
                        if (param[kLimit] == 0.0f)
                        {
                            delayed.flags = 127;
                        }
                        midiDelayBuffer.push_back(delayed);
                        dbg("delay noteoff in later block, counter=" << delayed.deltaFrames);
                    }
                }
                if (param[kDry] > 0.f)
                {
                    int dryvel = roundToInt((float) data2 * param[kDry]);
                    dbg("dry note off");
                    tomod.midiData[2] = dryvel;
                    if (! noteKilled[data1][channel])
                    {
                        discard = true;
                        outputs[0].push_back(tomod);
                        notePlaying[data1][channel] = false;
                    }
                    else
                    {
                        noteKilled[data1][channel] = false;
                    }
                }
                else
                {
                    discard = true;
                }
            }
            //Incoming NoteOn---------------------------------------------------------------
            else if (status == MIDI_NOTEON)
            {
                if (param[kWet] > 0.f)
                {
                    VstMidiEvent delayed = tomod;
                    delayed.midiData[2]  = roundToInt((float) data2 * param[kWet]);
                    if (delayed.midiData[2] > 0)
                    {
                        if (samples - delayed.deltaFrames - delay > 0)
                        {
                            // delayed note is within the current block, don't need counter
                            delayed.deltaFrames += delay;
                            if (notePlaying[data1][channel])
                            {
                                VstMidiEvent kill;
                                kill.midiData[0] = MIDI_NOTEOFF + channel;
                                kill.midiData[1] = data1;
                                kill.midiData[2] = 0;
                                kill.deltaFrames = delayed.deltaFrames;
                                kill.detune      = 0;
                                outputs[0].push_back(kill);
                                noteKilled[data1][channel] = true;
                                dbg("killed note " << data1);
                            }
                            dbg("delay noteon within block, deltaFrames=" << tomod.deltaFrames);
                            notePlaying[data1][channel] = true;
                            outputs[0].push_back(delayed);
                            delayed.deltaFrames = (delay - (samples - tomod.deltaFrames));
                            //if (samples - delayed.deltaFrames - delay <= 0) {
                            delayed.midiData[3] = 127;
                            delayed.flags       = roundToInt(param[kLimit] * 15.f) + 1;
                            if (param[kLimit] == 0.0f)
                            {
                                delayed.flags = 127;
                            }
                            midiDelayBuffer.push_back(delayed);
                            //}
                        }
                        else
                        {
                            expectingDelayedNoteOn[data1][channel]++;
                            // delayed note is in a future block, so start counter:
                            discard             = true; //takes the event out of the current block
                            delayed.deltaFrames = (delay - (samples - tomod.deltaFrames));
                            delayed.midiData[3] = 127;
                            delayed.flags       = roundToInt(param[kLimit] * 15.f) + 1;
                            if (param[kLimit] == 0.0f)
                            {
                                delayed.flags = 127;
                            }
                            midiDelayBuffer.push_back(delayed);
                            dbg("delay noteoff in later block, counter=" << delayed.deltaFrames);
                        }
                    }
                }
                if (param[kDry] > 0.f)
                {
                    int dryvel = roundToInt((float) data2 * param[kDry]);
                    if (dryvel == 0)
                    {
                        dryvel = 1;
                    }
                    if (notePlaying[data1][channel])
                    {
                        VstMidiEvent kill;
                        kill.midiData[0] = MIDI_NOTEOFF + channel;
                        kill.midiData[1] = data1;
                        kill.midiData[2] = 0;
                        kill.deltaFrames = tomod.deltaFrames;
                        kill.detune      = 0;
                        outputs[0].push_back(kill);
                        noteKilled[data1][channel] = true;
                        dbg("killed note " << data1);
                    }
                    dbg("dry note on");
                    tomod.midiData[2] = dryvel;
                    discard           = true;
                    outputs[0].push_back(tomod);
                    notePlaying[data1][channel] = true;
                }
                else
                {
                    notePlaying[data1][channel] = false;
                    discard                     = true;
                }
            }
            else if (status == MIDI_CONTROLCHANGE && param[kMode] >= 0.5f)
            {
                if (param[kDry] > 0.f)
                {
                    tomod.midiData[2] = roundToInt(param[kDry] * (float) data2);
                    outputs[0].push_back(tomod);
                    lastout[data1][channel] = -1;
                }
                else
                {
                    discard = true;
                }
                if (param[kWet] > 0.f)
                {
                    VstMidiEvent delayed = tomod;
                    if (samples - delayed.deltaFrames - delay > 0)
                    {
                        // delayed note is within the current block, don't need counter
                        delayed.deltaFrames += delay;
                        delayed.midiData[2] = roundToInt((float) data2 * param[kWet]);
                        outputs[0].push_back(delayed);
                        lastout[data1][channel] = delayed.midiData[2];
                        delayed.deltaFrames     = (delay - (samples - tomod.deltaFrames));
                        //if (samples - delayed.deltaFrames - delay <= 0) {
                        delayed.midiData[3] = 127;
                        delayed.flags       = roundToInt(param[kLimit] * 15.f) + 1;
                        if (param[kLimit] == 0.0f)
                        {
                            delayed.flags = 127;
                        }
                        midiDelayBuffer.push_back(delayed);
                        //}
                    }
                    else
                    {
                        discard             = true; //takes the event out of the current block
                        delayed.deltaFrames = (delay - (samples - tomod.deltaFrames));
                        delayed.midiData[3] = 127;
                        delayed.flags       = roundToInt(param[kLimit] * 15.f) + 1;
                        if (param[kLimit] == 0.0f)
                        {
                            delayed.flags = 127;
                        }
                        midiDelayBuffer.push_back(delayed);
                    }
                }
            }
        } // if listenchannel==channel
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    } //for() inputs loop

    //process delay buffer----------------------------------------------------------
    VstMidiEventVec newBuffer;
    newBuffer.clear();
    for (unsigned int i = 0; i < midiDelayBuffer.size(); i++)
    {
        dbg("processing delay buffer");
        VstMidiEvent event = midiDelayBuffer[i];
        int status         = event.midiData[0] & 0xf0; // scraping  channel
        int channel        = event.midiData[0] & 0x0f; // isolating channel (0-15)
        int data1          = event.midiData[1] & 0x7f;
        int data2          = event.midiData[2] & 0x7f;
        bool discard       = false;
        dbg("delta=" << event.deltaFrames);
        if (event.deltaFrames < samples && param[kWet] > 0.f)
        {
            //event is due
            //            OutputDebugString("event is due");
            if (status == MIDI_NOTEON)
            {
                expectingDelayedNoteOn[data1][channel]--;
                if (data2 > 0)
                {
                    if (notePlaying[data1][channel])
                    {
                        VstMidiEvent kill;
                        kill.midiData[0] = MIDI_NOTEOFF + channel;
                        kill.midiData[1] = data1;
                        kill.midiData[2] = 0;
                        kill.deltaFrames = event.deltaFrames;
                        kill.detune      = 0;
                        outputs[0].push_back(kill);
                        noteKilled[data1][channel] = true;
                        dbg("killed note " << data1);
                    }
                    notePlaying[data1][channel] = true;
                    dbg("delayed noteon");
                }
                else
                {
                    discard = true;
                }
            }
            else if (status == MIDI_NOTEOFF)
            {
                expectingDelayedNoteOff[data1][channel]--;
                if (! noteKilled[data1][channel])
                {
                    if (notePlaying[data1][channel])
                    {
                        notePlaying[data1][channel] = false;
                        dbg("delayed noteoff");
                    }
                    else
                    {
                        //discard=true;
                    }
                }
                else
                {
                    noteKilled[data1][channel] = false;
                    discard                    = true;
                }
            }
            else if (status == MIDI_CONTROLCHANGE && param[kMode] >= 0.5f)
            {
                VstMidiEvent out1 = event;
                out1.midiData[2]  = roundToInt(param[kWet] * (float) data2);
                //if (lastout[data1][channel]!=out1.midiData[2])
                outputs[0].push_back(out1);
                lastout[data1][channel] = out1.midiData[2];
                discard                 = true;
            }
            else
            {
                discard = true;
            }
            if (! discard)
            {
                VstMidiEvent out = event;
                out.midiData[2]  = roundToInt((float) data2 * param[kWet]);
                outputs[0].push_back(out);
            }
            event.flags--;

            if (param[kFeedback] > 0.f)
            {
                if (param[kFeedback] < 1.f)
                {
                    event.midiData[2] = roundToInt((float) data2 * param[kFeedback]) - 1;
                    if (status == MIDI_NOTEON && event.midiData[2] < 1)
                    {
                        event.midiData[2] = 1;
                    }
                    else if (event.midiData[2] < 0)
                    {
                        event.midiData[2] = 0;
                    }
                }
                if (samples - event.deltaFrames - delay > 0)
                {
                    event.deltaFrames += delay;
                    if (status == MIDI_NOTEOFF)
                    {
                        if (! noteKilled[data1][channel])
                        {
                            notePlaying[data1][channel] = false;
                            VstMidiEvent out1           = event;
                            out1.midiData[2]            = roundToInt((float) event.midiData[2] * param[kWet]);
                            outputs[0].push_back(out1);
                            event.flags--;
                        }
                        else
                        {
                            noteKilled[data1][channel] = false;
                        }
                    }
                    else if (status == MIDI_NOTEON)
                    {
                        VstMidiEvent out1 = event;
                        out1.midiData[2]  = roundToInt((float) event.midiData[2] * param[kWet]);
                        if (out1.midiData[2] > 0)
                        {
                            if (notePlaying[data1][channel])
                            {
                                VstMidiEvent kill;
                                kill.midiData[0] = MIDI_NOTEOFF + channel;
                                kill.midiData[1] = data1;
                                kill.midiData[2] = 0;
                                kill.deltaFrames = event.deltaFrames;
                                kill.detune      = 0;
                                outputs[0].push_back(kill);
                                noteKilled[data1][channel] = true;
                                dbg("killed note " << data1);
                            }
                            notePlaying[data1][channel] = true;
                            outputs[0].push_back(out1);
                        }
                        else
                        {
                            notePlaying[data1][channel] = false;
                        }
                        event.flags--;
                    }
                    else if (status == MIDI_CONTROLCHANGE && param[kMode] >= 0.5f)
                    {
                        VstMidiEvent out1 = event;
                        out1.midiData[2]  = roundToInt(param[kWet] * (float) data2);
                        //if (lastout[data1][channel]!=out1.midiData[2])
                        outputs[0].push_back(out1);
                        lastout[data1][channel] = out1.midiData[2];
                        event.flags--;
                    }
                    if (param[kFeedback] < 1.f)
                    {
                        event.midiData[3] = roundToInt((float) event.midiData[3] * param[kFeedback]) - 1;
                        if (event.midiData[3] < 0)
                        {
                            event.midiData[3] = 0;
                        }
                    }
                    if (event.midiData[3] > 0 && event.flags > 0)
                    {
                        if (status == MIDI_NOTEOFF)
                        {
                            expectingDelayedNoteOff[data1][channel]++;
                        }
                        else if (status == MIDI_NOTEON)
                        {
                            expectingDelayedNoteOn[data1][channel]++;
                        }
                        event.deltaFrames = (delay - (samples - event.deltaFrames));
                        if (param[kFeedback] < 1.f)
                        {
                            event.midiData[3] = roundToInt((float) event.midiData[3] * param[kFeedback]) - 1;
                            if (event.midiData[3] < 0)
                            {
                                event.midiData[3] = 0;
                            }
                        }
                        midiDelayBuffer.push_back(event);
                    }
                }
                else
                {
                    if (event.midiData[3] > 0 && event.flags > 0)
                    {
                        if (status == MIDI_NOTEOFF)
                        {
                            expectingDelayedNoteOff[data1][channel]++;
                        }
                        else if (status == MIDI_NOTEON)
                        {
                            expectingDelayedNoteOn[data1][channel]++;
                        }
                        event.deltaFrames = (delay - (samples - event.deltaFrames));
                        if (param[kFeedback] < 1.f)
                        {
                            event.midiData[3] = roundToInt((float) event.midiData[3] * param[kFeedback]) - 1;
                            if (event.midiData[3] < 0)
                            {
                                event.midiData[3] = 0;
                            }
                        }
                        newBuffer.push_back(event);
                    }
                }
            }
        }
        else
        {
            event.deltaFrames -= samples;
            newBuffer.push_back(event);
        }
    }
    midiDelayBuffer.clear();
    midiDelayBuffer = newBuffer;

    sortMidiEvents(outputs[0]);

    if (param[kMode] >= 0.5f)
    {
        std::reverse(outputs[0].begin(), outputs[0].end());
        VstMidiEventVec temp = outputs[0];
        outputs[0].clear();
        for (unsigned int i = 0; i < temp.size(); i++)
        {
            if (i == 0)
            {
                outputs[0].push_back(temp[i]);
            }
            else
            {
                int what = modulate(temp[i], temp[i - 1]);
                if (what == 0)
                {
                    outputs[0].push_back(temp[i]);
                    //                    OutputDebugString("yes");
                }
                //                else OutputDebugString("no");
            }
        }
    }

    if (wasplaying && ! isplaying)
    { //just stopped
        dbg("stopped");
    }
    wasplaying = isplaying;
}

int MidiDelay::modulate(VstMidiEvent input, VstMidiEvent mod)
{
    if (input.midiData[0] != mod.midiData[0])
    {
        return 0;
    }
    if (input.midiData[1] != mod.midiData[1])
    {
        return 0;
    }
    if (abs(input.deltaFrames - mod.deltaFrames) <= 20)
    {
        if (input.midiData[2] >= mod.midiData[2])
        {
            return 1;
        }
        return 2;
    }
    return 0;
}
