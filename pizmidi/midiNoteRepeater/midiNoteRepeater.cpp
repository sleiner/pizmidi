#include "midiNoteRepeater.hpp"

//for fmod()
#include <cmath>

#include <bitset>

//for seeding rand()
#include <time.h>

void printbits (char* string, char n)
{
    unsigned int i;
    i = 1 << (sizeof (n) * 8 - 1);

    while (i > 0)
    {
        if (n & i)
            strcat (string, "1");
        else
            strcat (string, "0");
        i >>= 1;
    }
}

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiDelay (audioMaster);
}

MidiDelayProgram::MidiDelayProgram()
{
    // default Program Values
    for (int i = 0; i < kNumParams; i++)
    {
        switch (i)
        {
            case kChannel:
                param[kChannel] = 0.0f;
                break;
            case kTime:
                param[kTime] = 0.55f;
                break;
            case kWet:
                param[kWet] = 1.f;
                break;
            case kDry:
                param[kDry] = 1.f;
                break;
            case kLimit:
                param[kLimit] = 0.01f;
                break;
            case kSync:
                param[kSync] = 1.f;
                break;
            case kFeedback:
                param[kFeedback] = 1.f;
                break;
            case kOffset:
                param[kOffset] = 0.f;
                break;
            default:
                param[i] = 0.f;
                break;
        }
    }
    // default program name
    strcpy (name, "Default");
}

//-----------------------------------------------------------------------------
MidiDelay::MidiDelay (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams), programs (0)
{
    programs = new MidiDelayProgram[numPrograms];
    if (programs)
    {
        CFxBank* defaultBank = new CFxBank (kNumPrograms, kNumParams);
        if (readDefaultBank (PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
                {
                    for (int p = 0; p < kNumParams; p++)
                    {
                        programs[i].param[p] = defaultBank->GetProgParm (i, p);
                    }
                    strcpy (programs[i].name, defaultBank->GetProgramName (i));
                }
            }
        }
        else
        {
            // built-in programs
            for (int i = 0; i < kNumPrograms; i++)
            {
                sprintf (programs[i].name, "Program %d", i + 1);
            }
        }
        setProgram (0);
    }

    memset (notePlaying, 0, sizeof (notePlaying));
    memset (noteKilled, 0, sizeof (noteKilled));
    memset (repeating, 0, sizeof (repeating));

    for (int n = 0; n < 128; n++)
    {
        for (int ch = 0; ch < 16; ch++)
        {
            lastout[n][ch] = -1;
            noteOnChannel[n][ch] = ch;
        }
    }
    srand ((unsigned int) time (NULL));

    _ppq = 0.0;
    _bpm = 120.0f;
    _beatpos = 0.0;
    samplesPerBeat = 0;
    counter = 0;
    wasplaying = false;
    isplaying = false;

    init();
}

//-----------------------------------------------------------------------------------------
MidiDelay::~MidiDelay()
{
    cleanMidiDelayBuffer();
    tempBuffer.clear();
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiDelay::setProgram (VstInt32 program)
{
    MidiDelayProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter (i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiDelay::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiDelay::getProgramName (char* name)
{
    strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiDelay::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy (text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiDelay::resume()
{
    midiDelayBuffer.clear();
    counter = roundToInt (param[kOffset] * (maxCount - 1));
    PizMidi::resume();
}

//-----------------------------------------------------------------------------------------
void MidiDelay::setParameter (VstInt32 index, float value)
{
    if (index < kNumParams)
    {
        MidiDelayProgram* ap = &programs[curProgram];
        param[index] = ap->param[index] = value;
        if (index == kTime)
        {
            const float inc = 1.f / 21.f;
            if (param[kTime] < 1 * inc)
                stepsize = 12.0f; //0.3333333 (3 bars)
            else if (param[kTime] < 2 * inc)
                stepsize = 8.0f; //0.5 (2 whole notes)
            else if (param[kTime] < 3 * inc)
                stepsize = 6.0f; //0.75 (dotted whole)
            else if (param[kTime] < 4 * inc)
                stepsize = 4.0f; //1 (whole note)
            else if (param[kTime] < 5 * inc)
                stepsize = 3.0f; //1.5 dotted half
            else if (param[kTime] < 6 * inc)
                stepsize = 2.0f; //2 (half note)
            else if (param[kTime] < 7 * inc)
                stepsize = 1.5f; //4.
            else if (param[kTime] < 8 * inc)
                stepsize = 1.333333333f; //2T
            else if (param[kTime] < 9 * inc)
                stepsize = 1.0f; //4
            else if (param[kTime] < 10 * inc)
                stepsize = 0.75f; //8.
            else if (param[kTime] < 11 * inc)
                stepsize = 0.666666667f; //4T
            else if (param[kTime] < 12 * inc)
                stepsize = 0.5f; //8
            else if (param[kTime] < 13 * inc)
                stepsize = 0.375; //16.
            else if (param[kTime] < 14 * inc)
                stepsize = 0.333333333f; //8T
            else if (param[kTime] < 15 * inc)
                stepsize = 0.25f; //16
            else if (param[kTime] < 16 * inc)
                stepsize = 0.1875f; //32.
            else if (param[kTime] < 17 * inc)
                stepsize = 0.166666667f; //16T
            else if (param[kTime] < 18 * inc)
                stepsize = 0.125f; //32
            else if (param[kTime] < 19 * inc)
                stepsize = 0.09375f; //64.
            else if (param[kTime] < 20 * inc)
                stepsize = 0.083333333f; //32T
            else
                stepsize = 0.0625f; //64
        }
        else if (index == kCount || index == kOffset)
        {
            counter = roundToInt (param[kOffset] * (maxCount - 1));
        }
        else if (index == kSync)
            updateDisplay();
    }
}

//-----------------------------------------------------------------------------------------
float MidiDelay::getParameter (VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiDelay::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kSync:
            strcpy (label, "TempoSync");
            break;
        case kTime:
            strcpy (label, "RepeatTime");
            break;
        case kChannel:
            strcpy (label, "InChannel");
            break;
        case kOutChannel:
            strcpy (label, "WetChannel");
            break;
        case kDry:
            strcpy (label, "Dry");
            break;
        case kWet:
            strcpy (label, "Wet");
            break;
        case kFeedback:
            strcpy (label, "Feedback");
            break;
        case kLimit:
            strcpy (label, "Repeats");
            break;
        case kOffset:
            strcpy (label, "StartCount");
            break;
        case kCount:
            strcpy (label, "Count");
            break;
        case kNoteFilter:
            strcpy (label, "Note");
            break;
        default:
            strcpy (label, " ");
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiDelay::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kPattern:
            printbits (text, (char) (roundToInt (param[kPattern] * 255.f)));
            break;
        case kFeedback:
        case kDry:
        case kWet:
            sprintf (text, "%d %%", roundToInt (param[index] * 100.f));
            break;

        case kLimit:
            sprintf (text, "%dx", roundToInt (param[index] * 100.f));
            break;
        case kSync:
            if (param[index] < 0.5f)
                strcpy (text, "Off");
            else
                strcpy (text, "On");
            break;
        case kTime:
            if (param[kSync] >= 0.5f)
            {
                const float inc = 1.f / 21.f;
                if (param[index] < 1 * inc)
                    strcpy (text, "3bars");
                else if (param[index] < 2 * inc)
                    strcpy (text, "2bars");
                else if (param[index] < 3 * inc)
                    strcpy (text, "dotted1");
                else if (param[index] < 4 * inc)
                    strcpy (text, "1");
                else if (param[index] < 5 * inc)
                    strcpy (text, "dotted2");
                else if (param[index] < 6 * inc)
                    strcpy (text, "2");
                else if (param[index] < 7 * inc)
                    strcpy (text, "dotted4");
                else if (param[index] < 8 * inc)
                    strcpy (text, "2T");
                else if (param[index] < 9 * inc)
                    strcpy (text, "4");
                else if (param[index] < 10 * inc)
                    strcpy (text, "dotted8");
                else if (param[index] < 11 * inc)
                    strcpy (text, "4T");
                else if (param[index] < 12 * inc)
                    strcpy (text, "8");
                else if (param[index] < 13 * inc)
                    strcpy (text, "dotted16");
                else if (param[index] < 14 * inc)
                    strcpy (text, "8T");
                else if (param[index] < 15 * inc)
                    strcpy (text, "16");
                else if (param[index] < 16 * inc)
                    strcpy (text, "dotted32");
                else if (param[index] < 17 * inc)
                    strcpy (text, "16T");
                else if (param[index] < 18 * inc)
                    strcpy (text, "32");
                else if (param[index] < 19 * inc)
                    strcpy (text, "dotted64");
                else if (param[index] < 20 * inc)
                    strcpy (text, "32T");
                else
                    strcpy (text, "64");
            }
            else
            {
                sprintf (text, "%d ms", roundToInt ((1.f - param[index]) * 2000.f));
            }
            break;
        case kChannel:
            if (FLOAT_TO_CHANNEL (param[index]) == -1)
                strcpy (text, "All");
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL (param[index]) + 1);
            break;
        case kOutChannel:
            if (FLOAT_TO_CHANNEL (param[index]) == -1)
                strcpy (text, "As Input");
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL (param[index]) + 1);
            break;
        case kNoteFilter:
            if (FLOAT_TO_MIDI_X (param[index]) == -1)
                strcpy (text, "All");
            else
                sprintf (text, "%s (%d)", getNoteName (FLOAT_TO_MIDI_X (param[index]), bottomOctave), FLOAT_TO_MIDI_X (param[index]));
            break;
        case kCount:
            sprintf (text, "%d", roundToInt (param[index] * (maxCount - 1)) + 1);
            break;
        case kOffset:
            sprintf (text, "%d", roundToInt (param[index] * (maxCount - 1)) + 1);
            break;
        default:
            sprintf (text, "%f", param[index]);
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiDelay::cleanMidiDelayBuffer()
{
    midiDelayBuffer.clear();
}

void MidiDelay::preProcess (void)
{
    // preparing Process
    VstTimeInfo* timeInfo = NULL;
    timeInfo = getTimeInfo (0xffff); //ALL

    if (timeInfo)
    {
        if (kVstTempoValid & timeInfo->flags)
            _bpm = (float) timeInfo->tempo;
        if (kVstPpqPosValid & timeInfo->flags)
            _ppq = timeInfo->ppqPos;
        if (kVstTransportPlaying & timeInfo->flags)
            isplaying = true;
        else
            isplaying = false;
        _beatpos = stepsize * fmod (_ppq, 1.0 / stepsize);
    }

    samplesPerBeat = roundToInt (60.f * sampleRate / _bpm);
    samplesPerTick = roundToInt ((float) samplesPerBeat / 960.0f);
    totalSamples = roundToInt (_ppq * (double) samplesPerBeat);

    _cleanMidiOutBuffers();

    sortMidiEvents (midiDelayBuffer);
}

void MidiDelay::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples)
{
    int listenchannel = FLOAT_TO_CHANNEL (param[kChannel]);
    int targetCount = roundToInt (param[kCount] * (maxCount - 1)) + 1;
    VstInt32 delay = roundToInt ((float) samplesPerBeat * stepsize);
    if (param[kSync] < 0.5f)
        delay = roundToInt (((1.f - param[kTime]) * 1.98f + 0.02f) * sampleRate);
    int repeats = roundToInt (param[kLimit] * 100.f) - 1;

    //process incoming events-------------------------------------------------------
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        int status = tomod.midiData[0] & 0xf0; // scraping  channel
        int channel = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        int data1 = tomod.midiData[1] & 0x7f;
        int data2 = tomod.midiData[2] & 0x7f;

        int wetChannel = FLOAT_TO_CHANNEL (param[kOutChannel]);
        if (wetChannel == -1)
            wetChannel = channel;

        dbg ("incoming midi event, i=" << i << " status=" << status << " ch=" << channel);

        //make 0-velocity notes into "real" noteoffs for simplicity
        if (status == MIDI_NOTEON && data2 == 0)
        {
            status = MIDI_NOTEOFF;
            tomod.midiData[0] = MIDI_NOTEOFF | channel;
        }

        bool discard = false;

        //only look at the selected channel
        if (channel == listenchannel || listenchannel == -1)
        {
            bool isRightNote = FLOAT_TO_MIDI_X (param[kNoteFilter]) == -1 || data1 == FLOAT_TO_MIDI_X (param[kNoteFilter]);
            //incoming NoteOff--------------------------------------------------------------
            if (status == MIDI_NOTEOFF && isRightNote)
            {
                if (/*param[kWet]>0.f && */ repeating[data1][channel])
                {
                    repeating[data1][channel] = false;
                    tomod.midiData[0] = MIDI_NOTEOFF + noteOnChannel[data1][channel];
                    notePlaying[data1][wetChannel] = false;
                    noteOnChannel[data1][channel] = channel;
                    if (repeats > 0)
                    {
                        //repeating[data1][channel]=true;
                        VstMidiEvent delayed = tomod;
                        delayed.deltaFrames = (delay - (samples - delayed.deltaFrames));
                        delayed.midiData[3] = 127;
                        delayed.flags = repeats;
                        midiDelayBuffer.push_back (delayed);
                    }
                    dbg ("wet note off");
                }
                else if (param[kDry] > 0.f || notePlaying[data1][channel])
                {
                    int dryvel = roundToInt ((float) data2 * param[kDry]);
                    dbg ("dry note off");
                    tomod.midiData[0] = MIDI_NOTEOFF + channel;
                    tomod.midiData[2] = dryvel;
                    discard = true;
                    notePlaying[data1][channel] = false;
                    if (! noteKilled[data1][channel])
                        outputs[0].push_back (tomod);
                    else
                        noteKilled[data1][channel] = false;
                }
                else
                    discard = true;
            }
            //Incoming NoteOn---------------------------------------------------------------
            else if (status == MIDI_NOTEON && isRightNote)
            {
                ++counter;
                if (counter < targetCount)
                {
                    if (param[kDry] > 0.f)
                    {
                        if (notePlaying[data1][channel])
                        {
                            VstMidiEvent kill;
                            kill.midiData[0] = MIDI_NOTEOFF + channel;
                            kill.midiData[1] = data1;
                            kill.midiData[2] = 0;
                            kill.deltaFrames = tomod.deltaFrames;
                            kill.detune = 0;
                            outputs[0].push_back (kill);
                            noteKilled[data1][channel] = true;
                            dbg ("365: killed note " << data1);
                        }
                        int dryvel = roundToInt ((float) data2 * param[kDry]);
                        if (dryvel == 0)
                            dryvel = 1;
                        tomod.midiData[2] = dryvel;
                        discard = true;
                        outputs[0].push_back (tomod);
                        notePlaying[data1][channel] = true;
                        dbg ("373: dry note " << data1);
                    }
                    else
                    {
                        discard = true;
                    }
                }
                else
                {
                    counter = 0;
                    if (param[kWet] > 0.f)
                    {
                        if (notePlaying[data1][wetChannel])
                        {
                            VstMidiEvent kill;
                            kill.midiData[0] = MIDI_NOTEOFF + wetChannel;
                            kill.midiData[1] = data1;
                            kill.midiData[2] = 0;
                            kill.deltaFrames = tomod.deltaFrames;
                            kill.detune = 0;
                            outputs[0].push_back (kill);
                            noteKilled[data1][wetChannel] = true;
                            dbg ("392: killed note " << data1);
                        }
                        discard = true;
                        if (repeats > -1)
                        {
                            repeating[data1][channel] = true;
                            noteOnChannel[data1][channel] = wetChannel;
                            tomod.midiData[0] = MIDI_NOTEON + wetChannel;
                            tomod.midiData[2] = max (1, roundToInt (param[kWet] * (float) data2));
                            outputs[0].push_back (tomod);
                            notePlaying[data1][wetChannel] = true;
                            dbg ("400: wet note " << data1);
                            if (repeats > 0)
                            {
                                VstMidiEvent delayed = tomod;
                                delayed.midiData[2] = roundToInt ((float) data2 * param[kFeedback]);
                                if (delayed.midiData[2] > 0)
                                {
                                    delayed.deltaFrames = (delay - (samples - tomod.deltaFrames));
                                    delayed.midiData[3] = 127;
                                    delayed.flags = repeats;
                                    midiDelayBuffer.push_back (delayed);
                                }
                            }
                        }
                        else
                        {
                            noteKilled[data1][wetChannel] = true;
                            notePlaying[data1][wetChannel] = false;
                        }
                    }
                }
            }
            else if (status == MIDI_CONTROLCHANGE && data1 == MIDI_ALL_NOTES_OFF)
            {
                counter = roundToInt (param[kOffset] * (maxCount - 1));
                cleanMidiDelayBuffer();
                memset (notePlaying, 0, sizeof (notePlaying));
                memset (noteKilled, 0, sizeof (noteKilled));
                memset (repeating, 0, sizeof (repeating));

                for (int n = 0; n < 128; n++)
                {
                    for (int ch = 0; ch < 16; ch++)
                    {
                        lastout[n][ch] = -1;
                    }
                }
            }
        } // if listenchannel==channel
        if (! discard)
            outputs[0].push_back (tomod);
    } //for() inputs loop

    //process delay buffer----------------------------------------------------------
    tempBuffer.clear();
    for (unsigned int i = 0; i < midiDelayBuffer.size(); i++)
    {
        VstMidiEvent event = midiDelayBuffer[i];
        int status = event.midiData[0] & 0xf0; // scraping  channel
        int channel = event.midiData[0] & 0x0f; // isolating channel (0-15)
        int data1 = event.midiData[1] & 0x7f;
        int data2 = event.midiData[2] & 0x7f;
        bool discard = false;

        if (event.deltaFrames < samples)
        {
            if (status == MIDI_NOTEON && param[kWet] > 0.f)
            {
                int velocity = roundToInt ((float) data2 * param[kWet]);
                if (velocity > 0)
                {
                    if (notePlaying[data1][channel])
                    {
                        VstMidiEvent kill;
                        kill.midiData[0] = MIDI_NOTEOFF + channel;
                        kill.midiData[1] = data1;
                        kill.midiData[2] = 0;
                        kill.deltaFrames = event.deltaFrames;
                        kill.detune = 0;
                        outputs[0].push_back (kill);
                        noteKilled[data1][channel] = true;
                        dbg ("468: killed note " << data1);
                    }
                    notePlaying[data1][channel] = true;
                    dbg ("471: delayed noteon " << data1);
                }
                else
                {
                    repeating[data1][channel] = false;
                    discard = true;
                }
            }
            else if (status == MIDI_NOTEOFF)
            {
                if (! noteKilled[data1][channel])
                {
                    if (notePlaying[data1][channel])
                    {
                        notePlaying[data1][channel] = false;
                        dbg ("delayed noteoff");
                    }
                    else
                    {
                        //discard=true;
                    }
                }
                else
                {
                    dbg ("489: killed noteoff");
                    noteKilled[data1][channel] = false;
                    //discard=true;
                }
            }
            else
                discard = true;
            if (! discard)
            {
                VstMidiEvent out = event;
                out.midiData[2] = roundToInt ((float) data2 * param[kWet]);
                outputs[0].push_back (out);
            }
            event.flags--;

            if (param[kFeedback] > 0.f)
            {
                if (param[kFeedback] < 1.f)
                {
                    event.midiData[2] = roundToInt ((float) data2 * param[kFeedback]) - 1;
                    if (status == MIDI_NOTEON && event.midiData[2] < 1)
                        event.midiData[2] = 1;
                    else if (event.midiData[2] < 0)
                        event.midiData[2] = 0;
                }
                if (samples - event.deltaFrames - delay > 0)
                {
                    event.deltaFrames += delay;
                    if (status == MIDI_NOTEOFF)
                    {
                        if (! noteKilled[data1][channel])
                        {
                            notePlaying[data1][channel] = false;
                            VstMidiEvent out1 = event;
                            out1.midiData[2] = roundToInt ((float) event.midiData[2] * param[kWet]);
                            outputs[0].push_back (out1);
                            event.flags--;
                        }
                        else
                            noteKilled[data1][channel] = false;
                    }
                    else if (status == MIDI_NOTEON)
                    {
                        VstMidiEvent out1 = event;
                        out1.midiData[2] = roundToInt ((float) event.midiData[2] * param[kWet]);
                        if (out1.midiData[2] > 0)
                        {
                            if (notePlaying[data1][channel])
                            {
                                VstMidiEvent kill;
                                kill.midiData[0] = MIDI_NOTEOFF + channel;
                                kill.midiData[1] = data1;
                                kill.midiData[2] = 0;
                                kill.deltaFrames = event.deltaFrames;
                                kill.detune = 0;
                                outputs[0].push_back (kill);
                                noteKilled[data1][channel] = true;
                                dbg ("537: killed note " << data1);
                            }
                            notePlaying[data1][channel] = true;
                            dbg ("540: feedback note " << data1);
                            outputs[0].push_back (out1);
                        }
                        else
                            notePlaying[data1][channel] = false;
                        event.flags--;
                    }
                    if (param[kFeedback] < 1.f)
                    {
                        event.midiData[3] = roundToInt ((float) event.midiData[3] * param[kFeedback]) - 1;
                        if (event.midiData[3] < 0)
                            event.midiData[3] = 0;
                    }
                    if (event.midiData[3] > 0 && event.flags > 0)
                    {
                        //if (status==MIDI_NOTEOFF) expectingDelayedNoteOff[data1][channel]++;
                        //else if (status==MIDI_NOTEON) expectingDelayedNoteOn[data1][channel]++;
                        event.deltaFrames = (delay - (samples - event.deltaFrames));
                        if (param[kFeedback] < 1.f)
                        {
                            event.midiData[3] = roundToInt ((float) event.midiData[3] * param[kFeedback]) - 1;
                            if (event.midiData[3] < 0)
                                event.midiData[3] = 0;
                        }
                        midiDelayBuffer.push_back (event);
                    }
                }
                else
                {
                    if (event.midiData[3] > 0 && event.flags > 0)
                    {
                        //if (status==MIDI_NOTEOFF) expectingDelayedNoteOff[data1][channel]++;
                        //else if (status==MIDI_NOTEON) expectingDelayedNoteOn[data1][channel]++;
                        event.deltaFrames = (delay - (samples - event.deltaFrames));
                        if (param[kFeedback] < 1.f)
                        {
                            event.midiData[3] = roundToInt ((float) event.midiData[3] * param[kFeedback]) - 1;
                            if (event.midiData[3] < 0)
                                event.midiData[3] = 0;
                        }
                        tempBuffer.push_back (event);
                    }
                }
            }
        }
        else
        {
            event.deltaFrames -= samples;
            tempBuffer.push_back (event);
        }
    }
    midiDelayBuffer.clear();
    midiDelayBuffer = tempBuffer;

    sortMidiEvents (outputs[0]);

    if (wasplaying && ! isplaying)
    { //just stopped
        dbg ("stopped");
    }
    wasplaying = isplaying;
}
