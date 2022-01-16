#include "midiStrum.hpp"

//for fmod()
#include <cmath>

//for seeding rand()
#include <time.h>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new midiStrum (audioMaster);
}

midiStrumProgram::midiStrumProgram()
{
    // default Program Values
    for (int i = 0; i < kNumParams; i++)
        param[i] = 0.f; //just in case
    param[kSpeed]        = 0.5f;
    param[kMaxDelay]     = 0.0f;
    param[kMode]         = 0.35f;
    param[kUpVelocity]   = 0.5f;
    param[kDnVelocity]   = 0.5f;
    param[kStrumOct]     = 0.3f;
    param[kAccel]        = 0.5f;
    param[kStrumChannel] = 0.0f;
    param[kVelRamp]      = 0.5f;
    param[kVelToVel]     = 0.0f;
    // default program name
    strcpy (name, "Default");
}

//-----------------------------------------------------------------------------
midiStrum::midiStrum (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams), programs (0)
{
    programs = new midiStrumProgram[numPrograms];

    usemutedstrum = false;
    init();

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
}

//-----------------------------------------------------------------------------------------
midiStrum::~midiStrum()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void midiStrum::setProgram (VstInt32 program)
{
    midiStrumProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter (i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void midiStrum::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void midiStrum::getProgramName (char* name)
{
    strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool midiStrum::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy (text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void midiStrum::setSampleRate (float sampleRateIn)
{
    PizMidi::setSampleRate (sampleRateIn);
    timezone = roundToInt (sampleRate * 0.02f); //20ms
}

//-----------------------------------------------------------------------------------------
void midiStrum::resume()
{
    PizMidi::resume();
    midiDelayBuffer.clear();
}

//-----------------------------------------------------------------------------------------
void midiStrum::setParameter (VstInt32 index, float value)
{
    if (index < kNumParams)
    {
        if (index == kMaxDelay)
        { //stepsize = "how many fit in half a beat"
            if (value == 0.0f)
                stepsize = 8.0f; //64
            else if (value < 0.05f)
                stepsize = 6.0f; //32T
            else if (value < 0.1f)
                stepsize = 5.333333333f; //64.
            else if (value < 0.15f)
                stepsize = 4.0f; //32
            else if (value < 0.2f)
                stepsize = 3.0f; //16T
            else if (value < 0.25f)
                stepsize = 2.666666667f; //32.
            else if (value < 0.3f)
                stepsize = 2.0f; //16
            else if (value < 0.35f)
                stepsize = 1.5f; //8T
            else if (value < 0.4f)
                stepsize = 1.333333333f; //16.
            else if (value < 0.45f)
                stepsize = 1.0f; //8
            else if (value < 0.5)
                stepsize = 0.75f; //4T
            else if (value < 0.55f)
                stepsize = 0.666666667f; //8.
            else if (value < 0.6f)
                stepsize = 0.5f; //4
            else if (value < 0.65f)
                stepsize = 0.375f; //2T
            else if (value < 0.7f)
                stepsize = 0.333333333f; //4.
            else if (value < 0.75f)
                stepsize = 0.25f; //2 (half note)
            else if (value < 0.8f)
                stepsize = 0.166666667f; //1.5 dotted half
            else if (value < 0.85f)
                stepsize = 0.125f; //1 (whole note)
            else if (value < 0.9f)
                stepsize = 0.0833333333f; //0.75 (dotted whole)
            else if (value < 0.95f)
                stepsize = 0.0625f; //0.5 (2 whole notes)
            else if (value < 1.0f)
                stepsize = 0.0416666667f; //3 bars
            else
                stepsize = 0.03125f; //4 bars
            beatdiv = 1.f / (stepsize * 2.f);
        }
        else if (index == kSync)
        {
            if ((param[kSync] < 0.5f && value >= 0.5f)
                || (param[kSync] >= 0.5f && value < 0.5f))
            {
                //sync type has changed
                updateDisplay();
            }
        }
        midiStrumProgram* ap = &programs[curProgram];
        param[index] = ap->param[index] = value;
    }
}

//-----------------------------------------------------------------------------------------
float midiStrum::getParameter (VstInt32 index)
{
    if (index < kNumParams)
        return param[index];
    return 0.f;
}

//-----------------------------------------------------------------------------------------
void midiStrum::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kMode:
            strcpy (label, "Mode");
            break;
        case kRandom:
            strcpy (label, "Randomness");
            break;
        case kLength:
            strcpy (label, "NoteLength");
            break;
        case kSpeed:
            strcpy (label, "Speed");
            break;
        case kMaxDelay:
            strcpy (label, "Max Time");
            break;
        case kSync:
            strcpy (label, "TempoSync");
            break;
        case kVelToSpeed:
            strcpy (label, "VelToSpeed");
            break;
        case kVelToVel:
            strcpy (label, "VelToVel");
            break;
        case kDnVelocity:
            strcpy (label, "DownVel");
            break;
        case kUpVelocity:
            strcpy (label, "UpVel");
            break;
        case kAccel:
            strcpy (label, "Accel");
            break;
        case kInChannel:
            strcpy (label, "In Chan");
            break;
        case kStrumChannel:
            strcpy (label, "Strum Chan");
            break;
        case kStrumOct:
            strcpy (label, "Strum Octave");
            break;
        case kOutChannel:
            strcpy (label, "Out Chan");
            break;
        case kVelRamp:
            strcpy (label, "VelRamp");
            break;
        case kThru:
            strcpy (label, "Audition");
            break;
        case kSustain:
            strcpy (label, "Use Sustain");
            break;
        default:
            strcpy (label, " ");
            break;
    }
}

//-----------------------------------------------------------------------------------------
void midiStrum::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kMode:
            if (param[index] < 0.5f)
                strcpy (text, "Complicated");
            else
                strcpy (text, "Simple");
            break;
        case kMaxDelay:
            if (param[kSync] < 0.5f)
            {
                sprintf (text, "%d ms", roundToInt (1000.f * (0.1f + 2.9f * param[kMaxDelay])));
            }
            else
            {
                if (param[index] == 0.0f)
                    strcpy (text, "64");
                else if (param[index] < 0.05f)
                    strcpy (text, "32T");
                else if (param[index] < 0.1f)
                    strcpy (text, "dotted64");
                else if (param[index] < 0.15f)
                    strcpy (text, "32");
                else if (param[index] < 0.2f)
                    strcpy (text, "16T");
                else if (param[index] < 0.25f)
                    strcpy (text, "dotted32");
                else if (param[index] < 0.3f)
                    strcpy (text, "16");
                else if (param[index] < 0.35f)
                    strcpy (text, "8T");
                else if (param[index] < 0.4f)
                    strcpy (text, "dotted16");
                else if (param[index] < 0.45f)
                    strcpy (text, "8");
                else if (param[index] < 0.5f)
                    strcpy (text, "4T");
                else if (param[index] < 0.55f)
                    strcpy (text, "dotted8");
                else if (param[index] < 0.6f)
                    strcpy (text, "4");
                else if (param[index] < 0.65f)
                    strcpy (text, "2T");
                else if (param[index] < 0.7f)
                    strcpy (text, "dotted4");
                else if (param[index] < 0.75f)
                    strcpy (text, "2");
                else if (param[index] < 0.8f)
                    strcpy (text, "dotted2");
                else if (param[index] < 0.85f)
                    strcpy (text, "1");
                else if (param[index] < 0.9f)
                    strcpy (text, "dotted1");
                else if (param[index] < 0.95f)
                    strcpy (text, "2bars");
                else if (param[index] < 1.0f)
                    strcpy (text, "3bars");
                else
                    strcpy (text, "4bars");
            }
            break;
        case kInChannel:
        case kStrumChannel:
            sprintf (text, "%d", FLOAT_TO_CHANNEL015 (param[index]) + 1);
            break;
        case kOutChannel:
            if (FLOAT_TO_CHANNEL (param[index]) == -1)
                strcpy (text, "As Input");
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL (param[index]) + 1);
            break;
        case kStrumOct:
            int d;
            d = roundToInt (param[index] * 10.f) * 12;
            sprintf (text, "%s (%d)", getNoteName (d, bottomOctave), d);
            break;
        case kDnVelocity:
        case kUpVelocity:
            sprintf (text, "%d", FLOAT_TO_VELOCITY (param[index]));
            break;
        case kRandom:
        case kSpeed:
        case kVelToSpeed:
        case kVelToVel:
            sprintf (text, "%d%%", roundToInt (param[index] * 100.0f));
            break;
        case kAccel:
            if (param[index] > 0.5f)
                sprintf (text, "+%d%%", roundToInt (param[index] * 200.f) - 100);
            else
                sprintf (text, "%d%%", roundToInt (param[index] * 200.f) - 100);
            break;
        case kLength:
            if (param[index] < 0.5f)
                strcpy (text, "InChannel");
            else
                strcpy (text, "StrumChannel");
            break;
        case kVelRamp:
            if (param[index] > 0.5f)
                sprintf (text, "+%d%%", roundToInt (param[index] * 200.f) - 100);
            else
                sprintf (text, "%d%%", roundToInt (param[index] * 200.f) - 100);
            break;
        case kThru:
            if (param[index] < 0.5f)
                strcpy (text, "No");
            else if (param[index] < 1.f)
                strcpy (text, "Always");
            else
                strcpy (text, "While Strumming");
            break;
        case kSustain:
        case kSync:
            if (param[index] < 0.5f)
                strcpy (text, "No");
            else
                strcpy (text, "Yes");
            break;
        default:
            sprintf (text, " ", param[index]);
            break;
    }
}

//-----------------------------------------------------------------------------------------
bool midiStrum::init (void)
{
    srand ((unsigned int) time (NULL));

    _ppq           = 0.0;
    _bpm           = 120.f;
    samplesPerBeat = 0;

    for (int n = 0; n < 128; n++)
    {
        noteOrigPos[n]  = 0;
        noteDelay[n]    = 0;
        notePlaying[n]  = false;
        notePlaying2[n] = false;
        held[n]         = 0;
        arp[n]          = -1;
    }
    heldnotes             = 0;
    expectingDelayedNotes = false;

    wasplaying     = false;
    isplaying      = false;
    strumming      = false;
    sustain        = false;
    strumnote      = -1;
    mutedstrumnote = 0;

    //for simple mode, the listening time after the first note
    timezone = roundToInt (sampleRate * 0.02f); //20ms

    return PizMidi::init();
}

void midiStrum::cleanMidiDelayBuffer()
{
    midiDelayBuffer.clear();
    expectingDelayedNotes = false;
}

void midiStrum::preProcess()
{
    // preparing Process
    VstTimeInfo* timeInfo = NULL;
    timeInfo              = getTimeInfo (0xffff); //ALL

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
    }

    samplesPerBeat = roundToInt (60.0f * sampleRate / _bpm);
    totalSamples   = roundToInt (_ppq * (double) samplesPerBeat);

    _cleanMidiOutBuffers();
}

void midiStrum::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples)
{
    //calculations------------------------------------------------------------------
    char listenchannel = FLOAT_TO_CHANNEL015 (param[kInChannel]);
    char strumchannel  = FLOAT_TO_CHANNEL015 (param[kStrumChannel]);
    char outchannel    = FLOAT_TO_CHANNEL (param[kOutChannel]);
    if (outchannel == -1)
        outchannel = listenchannel;

    char downstroke = roundToInt (param[kStrumOct] * 10.f) * 12;
    char upstroke   = downstroke + 2;
    //char downupstroke=upstroke+2;

    int mode = strum;
    if (param[kMode] >= 0.5f)
        mode = simple;

    int strumvel      = 0;
    int strumdelta    = -999;
    bool startstrum   = false;
    bool strumwaiting = false;

    //find delay in samples
    float accel  = (2.f * param[kAccel] - 1.f);
    float maxmax = sampleRate * (0.1f + 2.9f * param[kMaxDelay]); //max 3 seconds
    bool tempo   = param[kSync] >= 0.5;
    if (tempo)
        maxmax = beatdiv * samplesPerBeat;
    float maxdelay = (1.f - param[kSpeed]) * maxmax;

    //process delay buffer----------------------------------------------------------
    VstMidiEventVec newBuffer;
    newBuffer.clear();
    if (midiDelayBuffer.size() == 0)
        expectingDelayedNotes = false;
    for (unsigned int i = 0; i < midiDelayBuffer.size(); i++)
    {
        VstMidiEvent event = midiDelayBuffer[i];
        int status         = event.midiData[0] & 0xf0; // scraping  channel
        int channel        = event.midiData[0] & 0x0f; // isolating channel (0-15)
        int data1          = event.midiData[1] & 0x7f;
        int data2          = event.midiData[2] & 0x7f;

        if (status == MIDI_NOTEON && data2 == 0)
            status = MIDI_NOTEOFF;

        if (event.deltaFrames < samples)
        {
            //event is due, send it if output>0 and mode is right
            VstMidiEvent out = event;
            if (status == MIDI_NOTEON)
            {
                if (notePlaying[data1])
                {
                    VstMidiEvent kill = event;
                    kill.midiData[0]  = MIDI_NOTEOFF + channel;
                    kill.midiData[2]  = 0;
                    outputs[0].push_back (kill);
                    notePlaying[data1] = false;
                }
                int velocity = data2;
                int random   = roundToInt (param[kRandom] * (float (rand() % 1000) * 0.001f - 0.5f) * 127.f);
                velocity += random;
                if (velocity > 127)
                    velocity = 127;
                else if (velocity < 1)
                    velocity = 1;
                event.midiData[2] = velocity;
                outputs[0].push_back (out);
                notePlaying[data1] = true;
            }
            else if (status == MIDI_NOTEOFF)
            {
                outputs[0].push_back (out);
                notePlaying[data1] = false;
            }
        }
        else
        {
            //not due yet, try next block
            event.deltaFrames -= samples;
            newBuffer.push_back (event);
        }
    }
    if (wasplaying && ! isplaying)
    {
        newBuffer.clear();
    }
    //put leftover messages back into the buffer
    midiDelayBuffer.clear();
    midiDelayBuffer = newBuffer;

    bool killall = false;

    //process incoming events-------------------------------------------------------
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        int status  = tomod.midiData[0] & 0xf0; // scraping  channel
        int channel = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        int data1   = tomod.midiData[1] & 0x7f;
        int data2   = tomod.midiData[2] & 0x7f;

        //        dbg("incoming midi event, i=" << i << " status=" << status << " ch=" << (int)channel);

        //make 0-velocity notes into "real" noteoffs for simplicity
        if (status == MIDI_NOTEON && data2 == 0)
        {
            status            = MIDI_NOTEOFF;
            tomod.midiData[0] = MIDI_NOTEOFF | channel;
        }

        bool discard = false;
        //all notes off-----------------------------------------------------------------
        if ((channel == listenchannel || channel == strumchannel) && status == MIDI_CONTROLCHANGE && data1 == MIDI_ALL_NOTES_OFF)
        {
            dbg ("panic");
            heldnotes = 0;
            killall   = true;
            for (int n = 0; n < 128; n++)
            {
                held[n]      = 0;
                sustain      = false;
                noteDelay[n] = 0;
                if (notePlaying[n])
                {
                    VstMidiEvent kill;
                    kill.midiData[0] = MIDI_NOTEOFF + outchannel;
                    kill.midiData[1] = n;
                    kill.midiData[2] = 0;
                    kill.deltaFrames = tomod.deltaFrames;
                    kill.detune      = 0;
                    notePlaying[n]   = false;
                    outputs[0].push_back (kill);
                }
            }
        }
        //sustain pedal-----------------------------------------------------------------
        else if (channel == listenchannel && status == MIDI_CONTROLCHANGE && data1 == MIDI_SUSTAIN)
        {
            if (param[kSustain] < 0.5f)
            {
                sustain = false;
            }
            else
            {
                discard         = true;
                bool oldsustain = sustain;
                sustain         = data2 > 63;
                if (oldsustain && ! sustain)
                {
                    //sustain pedal off, end any notes that aren't being held manually
                    //if (param[kLength]>=0.5f) {
                    //????? this part needs work
                    for (int n = 0; n < 128; n++)
                    {
                        if (held[n] == 1)
                        {
                            held[n] = 0;
                            heldnotes--;
                            if (heldnotes < 0)
                                heldnotes = 0;
                            if (notePlaying[n])
                            {
                                VstInt32 delay = roundToInt ((0.1f + param[kRandom]) * (float (rand() % 1000) * 0.001f - 0.5f) * (sampleRate * 0.1f));
                                if ((noteDelay[n] + noteOrigPos[n]) >= (totalSamples + tomod.deltaFrames + delay))
                                {
                                    delay = noteDelay[n];
                                }
                                if (delay > 0)
                                {
                                    if (samples - tomod.deltaFrames - delay > 0)
                                    {
                                        // delayed event is within the current block
                                        VstMidiEvent kill = tomod;
                                        kill.midiData[0]  = MIDI_NOTEOFF + outchannel;
                                        kill.midiData[1]  = n;
                                        kill.midiData[2]  = 0;
                                        kill.deltaFrames  = delay;
                                        outputs[0].push_back (kill);
                                        notePlaying[n] = false;
                                        noteDelay[n]   = 0;
                                        dbg ("strum ended, killed note " << n);
                                    }
                                    else
                                    {
                                        VstMidiEvent delayed = tomod;
                                        delayed.midiData[0]  = MIDI_NOTEOFF + outchannel;
                                        delayed.midiData[1]  = n;
                                        delayed.midiData[2]  = 0;
                                        delayed.deltaFrames += delay - samples;
                                        midiDelayBuffer.push_back (delayed);
                                        expectingDelayedNotes = true;
                                        discard               = true;
                                    }
                                }
                                else
                                {
                                    dbg ("no delay");
                                    VstMidiEvent kill = tomod;
                                    kill.midiData[0]  = MIDI_NOTEOFF + outchannel;
                                    kill.midiData[1]  = n;
                                    kill.midiData[2]  = 0;
                                    outputs[0].push_back (kill);
                                    notePlaying[n] = false;
                                    noteDelay[n]   = 0;
                                    dbg ("strum ended, killed note " << n);
                                }
                            }
                            else
                                noteDelay[n] = 0;
                        }
                    }
                }
                if (heldnotes == 0)
                {
                    strumnote = -1;
                }
            }
        }
        //incoming NoteOff--------------------------------------------------------------
        else if (status == MIDI_NOTEOFF)
        {
            if (channel == listenchannel)
            {
                if (sustain)
                {
                    discard = true;
                    if (held[data1] > 0)
                        held[data1] = 1;
                }
                if (strumchannel == listenchannel
                    && (data1 >= downstroke && data1 < downstroke + 12) && mode != simple)
                {
                    //ignore
                }
                else if (! sustain)
                {
                    if (held[data1])
                        heldnotes--;
                    if (heldnotes < 0)
                        heldnotes = 0;
                    held[data1]       = 0;
                    tomod.midiData[0] = MIDI_NOTEOFF | outchannel;
                    //we assume every noteoff had a matching noteon as the previous event
                    //for each channel/note.
                    //therefore this noteoff belongs to the same voice as the last noteon.

                    VstInt32 delay = noteDelay[data1];
                    if (! expectingDelayedNotes)
                        delay = (VstInt32) (totalSamples - noteOrigPos[data1]) + tomod.deltaFrames + (VstInt32) (sampleRate / 1000.f);
                    //delay = roundToInt((0.1f+param[kRandom])*(float(rand()%1000)*0.001f-0.5f)*(sampleRate*0.1f));
                    if ((noteDelay[data1] + noteOrigPos[data1])
                        >= (totalSamples + tomod.deltaFrames + delay - (VstInt32) (sampleRate / 100.f)))
                    {
                        delay = noteDelay[data1] + (VstInt32) (sampleRate / 1000.f);
                    }
                    if (delay > 0)
                    {
                        dbg ("NoteOff needs delay=" << delay);
                        if (samples - tomod.deltaFrames - delay > 0)
                        {
                            // delayed event is within the current block
                            tomod.deltaFrames += delay;
                            notePlaying[data1] = false;
                            noteDelay[data1]   = 0;
                            tomod.midiData[0]  = MIDI_NOTEOFF | outchannel;
                        }
                        else
                        {
                            dbg ("future");
                            tomod.deltaFrames += (delay - samples);
                            tomod.midiData[0] = MIDI_NOTEOFF | outchannel;
                            midiDelayBuffer.push_back (tomod);
                            expectingDelayedNotes = true;
                            discard               = true;
                        }
                    }
                }
            }
            if (channel == strumchannel && mode != simple)
            {
                if (data1 >= downstroke && data1 < downstroke + 12)
                {
                    discard = true;
                    if (! strumming)
                    {
                    }
                    else
                    {
                        if (mutedstrumnote)
                        {
                            strumming = false;
                            VstMidiEvent kill;
                            kill.midiData[0] = (char) MIDI_NOTEOFF | 15;
                            kill.midiData[1] = mutedstrumnote;
                            kill.midiData[2] = 0;
                            kill.deltaFrames = tomod.deltaFrames;
                            kill.detune      = 0;
                            outputs[0].push_back (kill);
                            notePlaying[mutedstrumnote] = false;
                            mutedstrumnote              = 0;
                        }
                        if (param[kLength] >= 0.5f && strumnote == data1)
                        {
                            //????? this part needs work
                            strumming = false;
                            strumnote = -1;
                            for (int n = 0; n < 128; n++)
                            {
                                if (notePlaying[n] || noteDelay[n] > 0)
                                {
                                    VstInt32 delay = noteDelay[n];
                                    if (! expectingDelayedNotes)
                                        delay = (VstInt32) (totalSamples - noteOrigPos[n]) + tomod.deltaFrames + (VstInt32) (sampleRate / 1000.f);
                                    //VstInt32 delay = roundToInt((0.1f+param[kRandom])*(float(rand()%1000)*0.001f-0.5f)*(sampleRate*0.1f));
                                    //if ((noteDelay[n]+noteOrigPos[n]) >= (totalSamples+tomod.deltaFrames+delay)) {
                                    //    delay=noteDelay[n];                                    }
                                    if (delay > 0)
                                    {
                                        if (samples - tomod.deltaFrames - delay > 0)
                                        {
                                            // delayed event is within the current block
                                            VstMidiEvent kill = tomod;
                                            kill.midiData[0]  = MIDI_NOTEOFF | outchannel;
                                            kill.midiData[1]  = n;
                                            kill.midiData[2]  = 0;
                                            kill.deltaFrames += delay;
                                            notePlaying[n] = false;
                                            noteDelay[n]   = 0;
                                            outputs[0].push_back (kill);
                                            dbg ("strum ended, killed note " << n);
                                        }
                                        else
                                        {
                                            // future
                                            VstMidiEvent delayed = tomod;
                                            delayed.midiData[0]  = MIDI_NOTEOFF | outchannel;
                                            delayed.midiData[1]  = n;
                                            delayed.midiData[2]  = 0;
                                            delayed.deltaFrames += delay - samples;
                                            midiDelayBuffer.push_back (delayed);
                                            expectingDelayedNotes = true;
                                            discard               = true;
                                        }
                                    }
                                    else
                                    {
                                        dbg ("no delay");
                                        VstMidiEvent kill = tomod;
                                        kill.midiData[0]  = MIDI_NOTEOFF + outchannel;
                                        kill.midiData[1]  = n;
                                        kill.midiData[2]  = 0;
                                        outputs[0].push_back (kill);
                                        notePlaying[n] = false;
                                        noteDelay[n]   = 0;
                                        dbg ("strum ended, killed note " << n);
                                    }
                                }
                                else
                                    noteDelay[n] = 0;
                            }
                        }
                    }
                }
                else
                {
                    if (arp[data1] > -1)
                    {
                        discard           = false;
                        tomod.midiData[0] = MIDI_NOTEOFF | outchannel;
                        tomod.midiData[1] = arp[data1];
                        if (notePlaying[arp[data1]])
                        {
                            notePlaying[arp[data1]] = false;
                            noteDelay[arp[data1]]   = 0;
                        }
                        arp[data1] = -1;
                    }
                }
            }
        }
        //Incoming NoteOn---------------------------------------------------------------
        else if (status == MIDI_NOTEON)
        {
            if (channel == listenchannel)
            {
                if (mode == strum)
                {
                    if (strumchannel == listenchannel
                        && (data1 >= downstroke && data1 < downstroke + 12))
                    {
                        //ignore
                    }
                    else
                    {
                        if (held[data1] == 0)
                            heldnotes++;
                        held[data1] = 2;
                        if (param[kThru] < 0.5f)
                            discard = true;
                        else
                        {
                            tomod.midiData[0] = MIDI_NOTEON | outchannel;
                            if (strumming)
                                tomod.midiData[2] = 30;
                            notePlaying[data1] = true;
                        }
                        if (param[kThru] == 1.f)
                        {
                            if (strumming)
                            {
                                tomod.midiData[0]  = MIDI_NOTEON | outchannel;
                                tomod.midiData[2]  = 30;
                                notePlaying[data1] = true;
                            }
                            else
                                discard = true;
                        }
                    }
                }
                else if (mode == simple)
                {
                    //collect held notes in this block, then strum
                    if (held[data1] == 0)
                        heldnotes++;
                    held[data1]  = 2;
                    discard      = true;
                    strumwaiting = true;
                    if (strumdelta == -999)
                    {
                        //this should be the first received note
                        strumdelta = tomod.deltaFrames;
                        strumvel   = data2;
                        dbg ("first " << tomod.deltaFrames);
                    }
                    if (tomod.deltaFrames - strumdelta < timezone)
                    {
                        //keep collecting
                        dbg ("here " << tomod.deltaFrames);
                    }
                    else
                    {
                        //first note in next strum
                        startstrum = true;
                        strumdelta = -999;
                        dbg ("there " << tomod.deltaFrames);
                    }
                }
            }
            if (channel == strumchannel)
            {
                if (data1 >= downstroke && data1 < downstroke + 12)
                {
                    bool up = data1 == upstroke      //D
                           || data1 == upstroke + 1  //D#
                           || data1 == upstroke + 3  //F
                           || data1 == upstroke + 4  //F#
                           || data1 == upstroke + 7  //A
                           || data1 == upstroke + 8; //A#
                    if (mutedstrumnote)
                    {
                        VstMidiEvent kill;
                        kill.midiData[0]            = (char) MIDI_NOTEOFF | 15;
                        kill.midiData[1]            = mutedstrumnote;
                        kill.midiData[2]            = 0;
                        kill.deltaFrames            = tomod.deltaFrames;
                        kill.detune                 = 0;
                        notePlaying[mutedstrumnote] = false;
                        outputs[0].push_back (kill);
                        mutedstrumnote = 0;
                    }
                    if (heldnotes == 0)
                    {
                        discard = true;
                        if (usemutedstrum)
                        {
                            strumming = true;
                            strumnote = data1;
                            //trigger "muted/open strings" notes
                            if (up)
                                mutedstrumnote = 64; //open high E string
                            else
                                mutedstrumnote = 40; //open low E string
                            VstMidiEvent strum;
                            strum.midiData[0] = (char) MIDI_NOTEON | 15;
                            strum.midiData[1] = mutedstrumnote;
                            strum.midiData[2] = data2;
                            strum.deltaFrames = tomod.deltaFrames;
                            strum.detune      = 0;
                            outputs[0].push_back (strum);
                        }
                    }
                    else
                    {
                        strumming    = true;
                        strumnote    = data1;
                        discard      = true;
                        int velocity = data2;
                        if (mode == strum)
                        {
                            int n = 0;
                            if (up)
                                n = 127;
                            int chordpos = 0;
                            while (chordpos < heldnotes)
                            {
                                //to iterate through the number of notes in the held chord
                                if (n > 127 || n < 0)
                                {
                                    heldnotes = 0;
                                    break;
                                }
                                else if (held[n])
                                {
                                    dbg ("play chord, notes = " << heldnotes);
                                    if (up)
                                        velocity =
                                            FLOAT_TO_VELOCITY (param[kUpVelocity] * (1.f - param[kVelToVel]) + param[kVelToVel] * VELOCITY_TO_FLOAT (data2));
                                    else
                                        velocity =
                                            FLOAT_TO_VELOCITY (param[kDnVelocity] * (1.f - param[kVelToVel]) + param[kVelToVel] * VELOCITY_TO_FLOAT (data2));
                                    dbg ("v=" << velocity);

                                    //delay based on chord position, in samples:
                                    VstInt32 delay = 0;
                                    if (heldnotes > 1)
                                    {
                                        float x = (float) (chordpos) / (float) (heldnotes - 1);
                                        delay   = roundToInt ((accel * 0.3f * sin (PI * x) + x) * maxdelay);
                                        velocity += roundToInt ((2.f * param[kVelRamp] - 1.f) * (x * 127.f - 64.f));
                                        if (velocity > 127)
                                            velocity = 127;
                                        else if (velocity < 1)
                                            velocity = 1;
                                        float veldelay = 1.f - (param[kVelToSpeed]) * MIDI_TO_FLOAT (data2);
                                        delay          = roundToInt (veldelay * (float) delay);
                                    }
                                    dbg ("held note " << n << " delay=" << delay);
                                    if (delay > 0)
                                    {
                                        if (samples - tomod.deltaFrames - delay > 0)
                                        {
                                            // delayed note is within the current block, don't need counter
                                            //tomod.deltaFrames += delay;

                                            //if the note is already playing, first send a noteoff to avoid overlap
                                            if (notePlaying[n])
                                            {
                                                VstMidiEvent kill;
                                                kill.midiData[0] = MIDI_NOTEOFF | outchannel;
                                                kill.midiData[1] = n;
                                                kill.midiData[2] = 0;
                                                kill.deltaFrames = tomod.deltaFrames + delay;
                                                kill.detune      = 0;
                                                outputs[0].push_back (kill);
                                                notePlaying[n] = false;
                                                dbg ("KILL " << n);
                                            }
                                            int random = roundToInt (param[kRandom] * (float (rand() % 1000) * 0.001f - 0.5f) * 127.f);
                                            dbg ("random vel=" << random);
                                            velocity += random;
                                            if (velocity > 127)
                                                velocity = 127;
                                            else if (velocity < 1)
                                                velocity = 1;
                                            notePlaying[n] = true;
                                            VstMidiEvent strum;
                                            strum.midiData[0] = MIDI_NOTEON | outchannel;
                                            strum.midiData[1] = n;
                                            strum.midiData[2] = velocity;
                                            strum.deltaFrames = tomod.deltaFrames + delay;
                                            strum.detune      = 0;
                                            outputs[0].push_back (strum);
                                        }
                                        else
                                        {
                                            VstMidiEvent delayed;
                                            delayed.midiData[0] = MIDI_NOTEON | outchannel;
                                            delayed.midiData[1] = n;
                                            delayed.midiData[2] = velocity;
                                            delayed.deltaFrames = tomod.deltaFrames + delay - samples;
                                            midiDelayBuffer.push_back (delayed);
                                            expectingDelayedNotes = true;
                                        }
                                        noteDelay[n] = delay;
                                    } //(delay>0)
                                    else if (velocity > 0)
                                    {
                                        dbg ("no delay");
                                        noteDelay[n] = 0;
                                        if (notePlaying[n])
                                        {
                                            VstMidiEvent kill;
                                            kill.midiData[0] = MIDI_NOTEOFF + outchannel;
                                            kill.midiData[1] = n;
                                            kill.midiData[2] = 0;
                                            kill.deltaFrames = tomod.deltaFrames;
                                            kill.detune      = 0;
                                            outputs[0].push_back (kill);
                                            dbg ("incoming noteon killed note " << n);
                                            notePlaying[n] = false;
                                        }
                                        int random = roundToInt (param[kRandom] * (float (rand() % 1000) * 0.001f - 0.5f) * 127.f);
                                        dbg ("random vel=" << random);
                                        velocity += random;
                                        if (velocity > 127)
                                            velocity = 127;
                                        else if (velocity < 1)
                                            velocity = 1;
                                        notePlaying[n] = true;
                                        VstMidiEvent strum;
                                        strum.midiData[0] = MIDI_NOTEON | outchannel;
                                        strum.midiData[1] = n;
                                        strum.midiData[2] = velocity;
                                        strum.deltaFrames = tomod.deltaFrames;
                                        strum.detune      = 0;
                                        outputs[0].push_back (strum);
                                    } //(no delay)
                                    chordpos++;
                                } //(held[n])
                                if (up)
                                    --n;
                                else
                                    ++n;
                            } //(chordpos<heldnotes)
                        }
                    }
                }
                else if (strumchannel != listenchannel)
                {
                    int firstnote = downstroke + 12;
                    int chordpos  = 0;
                    int n         = 0;
                    discard       = true;
                    while (chordpos < heldnotes)
                    {
                        if (n > 127 || n < 0)
                        {
                            heldnotes = 0;
                            break;
                        }
                        else if (held[n])
                        {
                            if (chordpos == data1 - firstnote)
                            {
                                discard           = false;
                                tomod.midiData[0] = MIDI_NOTEON | outchannel;
                                tomod.midiData[1] = n;
                                notePlaying[n]    = true;
                                arp[data1]        = n;
                            }
                            chordpos++;
                        }
                        n++;
                    }
                }
            } //channel==strumchannel
        }     // status==noteon
        if (! discard)
            outputs[0].push_back (tomod);

        //simple strumming:
        if (mode == simple && startstrum)
        {
            dbg ("zing ");
            strumwaiting = false;
            startstrum   = false;
            strumming    = true;
            bool up      = false;
            int n        = 0;
            if (up)
                n = 127;
            int chordpos = 0;
            while (chordpos < heldnotes)
            {
                //to iterate through the number of notes in the held chord
                if (n > 127 || n < 0)
                {
                    heldnotes = 0;
                    break;
                }
                if (held[n])
                {
                    int velocity = strumvel;
                    if (up)
                        velocity = FLOAT_TO_VELOCITY (param[kUpVelocity] * (1.f - param[kVelToVel]) + param[kVelToVel] * VELOCITY_TO_FLOAT (strumvel));
                    else
                        velocity = FLOAT_TO_VELOCITY (param[kDnVelocity] * (1.f - param[kVelToVel]) + param[kVelToVel] * VELOCITY_TO_FLOAT (strumvel));
                    //delay based on chord position, in samples:
                    VstInt32 delay = 0;
                    if (heldnotes > 1)
                    {
                        float x = (float) (chordpos) / (float) (heldnotes - 1);
                        delay   = roundToInt ((accel * 0.3f * sin (PI * x) + x) * maxdelay);
                        velocity += roundToInt ((2.f * param[kVelRamp] - 1.f) * (x * 127.f - 64.f));
                        if (velocity > 127)
                            velocity = 127;
                        else if (velocity < 1)
                            velocity = 1;
                        float veldelay = 1.f - (param[kVelToSpeed]) * MIDI_TO_FLOAT (strumvel);
                        delay          = roundToInt (veldelay * (float) delay);
                    }
                    if (delay > 0)
                    {
                        if (samples - strumdelta - delay > 0)
                        {
                            // delayed note is within the current block, don't need counter
                            //if the note is already playing, first send a noteoff to avoid overlap
                            if (notePlaying[n])
                            {
                                VstMidiEvent kill;
                                kill.midiData[0] = MIDI_NOTEOFF | outchannel;
                                kill.midiData[1] = n;
                                kill.midiData[2] = 0;
                                kill.deltaFrames = strumdelta + delay;
                                kill.detune      = 0;
                                outputs[0].push_back (kill);
                                notePlaying[n] = false;
                                dbg ("KILLY " << n);
                            }
                            int random = roundToInt (param[kRandom] * (float (rand() % 1000) * 0.001f - 0.5f) * 127.f);
                            velocity += random;
                            if (velocity > 127)
                                velocity = 127;
                            else if (velocity < 1)
                                velocity = 1;
                            notePlaying[n] = true;
                            VstMidiEvent strum;
                            strum.midiData[0] = MIDI_NOTEON | outchannel;
                            strum.midiData[1] = n;
                            strum.midiData[2] = velocity;
                            strum.deltaFrames = strumdelta + delay;
                            strum.detune      = 0;
                            outputs[0].push_back (strum);
                        }
                        else
                        {
                            VstMidiEvent delayed;
                            delayed.midiData[0] = MIDI_NOTEON | outchannel;
                            delayed.midiData[1] = n;
                            delayed.midiData[2] = velocity;
                            delayed.deltaFrames = strumdelta + delay - samples;
                            midiDelayBuffer.push_back (delayed);
                            expectingDelayedNotes = true;
                        }
                        noteDelay[n] = delay;
                    } //(delay>0)
                    else if (velocity > 0)
                    {
                        noteDelay[n] = 0;
                        if (notePlaying[n])
                        {
                            VstMidiEvent kill;
                            kill.midiData[0] = MIDI_NOTEOFF + outchannel;
                            kill.midiData[1] = n;
                            kill.midiData[2] = 0;
                            kill.deltaFrames = strumdelta;
                            kill.detune      = 0;
                            outputs[0].push_back (kill);
                            notePlaying[n] = false;
                            dbg ("KILL MASTER!!! " << n);
                        }
                        int random = roundToInt (param[kRandom] * (float (rand() % 1000) * 0.001f - 0.5f) * 127.f);
                        velocity += random;
                        if (velocity > 127)
                            velocity = 127;
                        else if (velocity < 1)
                            velocity = 1;
                        notePlaying[n] = true;
                        VstMidiEvent strum;
                        strum.midiData[0] = MIDI_NOTEON | outchannel;
                        strum.midiData[1] = n;
                        strum.midiData[2] = velocity;
                        strum.deltaFrames = strumdelta;
                        strum.detune      = 0;
                        outputs[0].push_back (strum);
                    } //(no delay)
                    chordpos++;
                } //(held[n])
                if (up)
                    --n;
                else
                    ++n;
            } //(chordpos<heldnotes)
        }
    } //for() inputs loop

    //in case we're at the end of the block and strumming hasn't started yet
    if (mode == simple && strumwaiting)
    {
        dbg ("ouch ");
        strumwaiting = false;
        startstrum   = false;
        strumming    = true;
        bool up      = false;
        int n        = 0;
        if (up)
            n = 127;
        int chordpos = 0;
        while (chordpos < heldnotes)
        {
            //to iterate through the number of notes in the held chord
            if (n > 127 || n < 0)
            {
                heldnotes = 0;
                break;
            }
            if (held[n])
            {
                int velocity = strumvel;
                if (up)
                    velocity = FLOAT_TO_VELOCITY (param[kUpVelocity] * (1.f - param[kVelToVel]) + param[kVelToVel] * VELOCITY_TO_FLOAT (strumvel));
                else
                    velocity = FLOAT_TO_VELOCITY (param[kDnVelocity] * (1.f - param[kVelToVel]) + param[kVelToVel] * VELOCITY_TO_FLOAT (strumvel));
                //delay based on chord position, in samples:
                VstInt32 delay = 0;
                if (heldnotes > 1)
                {
                    float x = (float) (chordpos) / (float) (heldnotes - 1);
                    delay   = roundToInt ((accel * 0.3f * sin (PI * x) + x) * maxdelay);
                    velocity += roundToInt ((2.f * param[kVelRamp] - 1.f) * (x * 127.f - 64.f));
                    if (velocity > 127)
                        velocity = 127;
                    else if (velocity < 1)
                        velocity = 1;
                    float veldelay = 1.f - (param[kVelToSpeed]) * MIDI_TO_FLOAT (strumvel);
                    delay          = roundToInt (veldelay * (float) delay);
                }
                if (delay > 0)
                {
                    if (samples - strumdelta - delay > 0)
                    {
                        // delayed note is within the current block, don't need counter
                        //if the note is already playing, first send a noteoff to avoid overlap
                        if (notePlaying[n])
                        {
                            VstMidiEvent kill;
                            kill.midiData[0] = MIDI_NOTEOFF | outchannel;
                            kill.midiData[1] = n;
                            kill.midiData[2] = 0;
                            kill.deltaFrames = strumdelta + delay;
                            kill.detune      = 0;
                            outputs[0].push_back (kill);
                            dbg ("Killo " << n);
                        }
                        int random = roundToInt (param[kRandom] * (float (rand() % 1000) * 0.001f - 0.5f) * 127.f);
                        velocity += random;
                        if (velocity > 127)
                            velocity = 127;
                        else if (velocity < 1)
                            velocity = 1;
                        notePlaying[n] = true;
                        VstMidiEvent strum;
                        strum.midiData[0] = MIDI_NOTEON | outchannel;
                        strum.midiData[1] = n;
                        strum.midiData[2] = velocity;
                        strum.deltaFrames = strumdelta + delay;
                        strum.detune      = 0;
                        outputs[0].push_back (strum);
                    }
                    else
                    {
                        VstMidiEvent delayed;
                        delayed.midiData[0] = MIDI_NOTEON | outchannel;
                        delayed.midiData[1] = n;
                        delayed.midiData[2] = velocity;
                        delayed.deltaFrames = strumdelta + delay;
                        midiDelayBuffer.push_back (delayed);
                        expectingDelayedNotes = true;
                    }
                    noteDelay[n]   = delay;
                    noteOrigPos[n] = totalSamples + strumdelta;
                } //(delay>0)
                else if (velocity > 0)
                {
                    noteDelay[n] = 0;
                    if (notePlaying[n])
                    {
                        VstMidiEvent kill;
                        kill.midiData[0] = MIDI_NOTEOFF + outchannel;
                        kill.midiData[1] = n;
                        kill.midiData[2] = 0;
                        kill.deltaFrames = strumdelta;
                        kill.detune      = 0;
                        outputs[0].push_back (kill);
                        notePlaying[n] = false;
                        dbg ("killkillkilly " << n);
                    }
                    int random = roundToInt (param[kRandom] * (float (rand() % 1000) * 0.001f - 0.5f) * 127.f);
                    velocity += random;
                    if (velocity > 127)
                        velocity = 127;
                    else if (velocity < 1)
                        velocity = 1;
                    notePlaying[n] = true;
                    VstMidiEvent strum;
                    strum.midiData[0] = MIDI_NOTEON | outchannel;
                    strum.midiData[1] = n;
                    strum.midiData[2] = velocity;
                    strum.deltaFrames = strumdelta;
                    strum.detune      = 0;
                    outputs[0].push_back (strum);
                } //(no delay)
                chordpos++;
            } //(held[n])
            if (up)
                --n;
            else
                ++n;
        } //(chordpos<heldnotes)
    }
    if (! wasplaying && isplaying)
    {
        dbg ("playing");
    }
    else if (wasplaying && ! isplaying)
    { //just stopped
        sustain = false;
        killall = true;
        for (int n = 0; n < 128; n++)
        {
            //for (int v=0;v<MAX_VOICES_PER_NOTE;v++) {
            if (notePlaying[n])
            {
                VstMidiEvent kill;
                kill.midiData[0] = MIDI_NOTEOFF + outchannel;
                kill.midiData[1] = n;
                kill.midiData[2] = 0;
                kill.deltaFrames = samples - 1;
                kill.detune      = 0;
                outputs[0].push_back (kill);
                notePlaying[n] = false;
                noteDelay[n]   = 0;
                dbg ("stopped, killed note " << n);
            }
            //}
            held[n] = 0;
        }
        heldnotes = 0;
    }
    wasplaying = isplaying;
#if 1
    //sort combined buffer, then deal with overlapping notes
    VstMidiEventVec temp = outputs[0];
    outputs[0].clear();
    sortMidiEvents (temp);
    for (unsigned int i = 0; i < temp.size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = temp[i];

        int status  = tomod.midiData[0] & 0xf0; // scraping  channel
        int channel = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        int data1   = tomod.midiData[1] & 0x7f;
        int data2   = tomod.midiData[2] & 0x7f;

        //make 0-velocity notes into "real" noteoffs for simplicity
        if (status == MIDI_NOTEON && data2 == 0)
        {
            status            = MIDI_NOTEOFF;
            tomod.midiData[0] = MIDI_NOTEOFF | channel;
        }

        //bool discard = false;

        if (channel == outchannel)
        {
            if (status == MIDI_NOTEON)
            {
                if (notePlaying2[data1] || (notePlaying[data1] && killall))
                {
                    VstMidiEvent kill = tomod;
                    kill.midiData[0]  = MIDI_NOTEOFF + outchannel;
                    kill.midiData[2]  = 0;
                    outputs[0].push_back (kill);
                    notePlaying2[data1] = false;
                    notePlaying[data1]  = false;
                }
                if (! killall)
                {
                    notePlaying2[data1] = true;
                    notePlaying[data1]  = true;
                    outputs[0].push_back (tomod);
                }
            }
            else if (status == MIDI_NOTEOFF)
            {
                if (notePlaying2[data1])
                    outputs[0].push_back (tomod);
                notePlaying2[data1] = false;
                notePlaying[data1]  = false;
            }
            else
                outputs[0].push_back (tomod);
        }
        else
            outputs[0].push_back (tomod);
    }
#endif
}
