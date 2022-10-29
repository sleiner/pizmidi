#include "midiExactDelay.hpp"

//for fmod()
#include <cmath>

//for seeding rand()
#include <time.h>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiExactDelay(audioMaster);
}

MidiExactDelayProgram::MidiExactDelayProgram()
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
            case kWet:
                param[kWet] = 1.0f;
                break;
            case kSync:
                param[kSync] = 0.0f;
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
MidiExactDelay::MidiExactDelay(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiExactDelayProgram[numPrograms];
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
    srand((unsigned int) time(NULL));

    wasplaying = false;
    isplaying  = false;

    init();
}

//-----------------------------------------------------------------------------------------
MidiExactDelay::~MidiExactDelay()
{
    cleanMidiDelayBuffer();
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiExactDelay::setProgram(VstInt32 program)
{
    MidiExactDelayProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter(i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiExactDelay::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiExactDelay::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiExactDelay::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiExactDelay::resume()
{
    midiDelayBuffer.clear();
    PizMidi::resume();
}

//-----------------------------------------------------------------------------------------
void MidiExactDelay::setParameter(VstInt32 index, float value)
{
    MidiExactDelayProgram* ap = &programs[curProgram];
    param[index] = ap->param[index] = value;
    if (index == kSync)
    {
        if (param[index] < 0.3f)
            timebase = sec;
        else if (param[index] < 0.6f)
            timebase = beat;
        else
            timebase = samp;
        updateDisplay();
    }
    else if (index == kBeatDiv)
    {
        const float inc = 1.f / 22.f;
        if (param[kBeatDiv] < 1 * inc)
            Ticks = 0.f; //None
        else if (param[kBeatDiv] < 2 * inc)
            Ticks = 30.f; //128
        else if (param[kBeatDiv] < 3 * inc)
            Ticks = 40.f; //64T
        else if (param[kBeatDiv] < 4 * inc)
            Ticks = 45.f; //128.
        else if (param[kBeatDiv] < 5 * inc)
            Ticks = 60.f; //64
        else if (param[kBeatDiv] < 6 * inc)
            Ticks = 80.f; //32T
        else if (param[kBeatDiv] < 7 * inc)
            Ticks = 90.f; //64.
        else if (param[kBeatDiv] < 8 * inc)
            Ticks = 120.f; //32
        else if (param[kBeatDiv] < 9 * inc)
            Ticks = 160.f; //16T
        else if (param[kBeatDiv] < 10 * inc)
            Ticks = 180.f; //32.
        else if (param[kBeatDiv] < 11 * inc)
            Ticks = 240.f; //16
        else if (param[kBeatDiv] < 12 * inc)
            Ticks = 320.f; //8T
        else if (param[kBeatDiv] < 13 * inc)
            Ticks = 360.f; //16.
        else if (param[kBeatDiv] < 14 * inc)
            Ticks = 480.f; //8
        else if (param[kBeatDiv] < 15 * inc)
            Ticks = 640.f; //4T
        else if (param[kBeatDiv] < 16 * inc)
            Ticks = 720.f; //8.
        else if (param[kBeatDiv] < 17 * inc)
            Ticks = 960.f; //4
        else if (param[kBeatDiv] < 18 * inc)
            Ticks = 1280.f; //2T
        else if (param[kBeatDiv] < 19 * inc)
            Ticks = 1440.f; //4.
        else if (param[kBeatDiv] < 20 * inc)
            Ticks = 1920.f; //2 (half note)
        else if (param[kBeatDiv] < 21 * inc)
            Ticks = 2560.f; //whole triplet
        else
            Ticks = 2880.f; //1.5 dotted half
    }
}

//-----------------------------------------------------------------------------------------
float MidiExactDelay::getParameter(VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiExactDelay::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kSync:
            strcpy(label, "TimeMode");
            break;
        case kMode:
            strcpy(label, "Mode");
            break;
        case kChannel:
            strcpy(label, "Channel");
            break;
        case kWet:
            strcpy(label, "Output");
            break;

        case kSeconds:
            strcpy(label, "Seconds");
            break;
        case kTenths:
            strcpy(label, "Tenths");
            break;
        case kHundredths:
            strcpy(label, "Hundredth");
            break;
        case kThousandths:
            strcpy(label, "Thousandths");
            break;

        case kBars:
            strcpy(label, "Bars");
            break;
        case kBeats:
            strcpy(label, "Beats");
            break;
        case kBeatDiv:
            strcpy(label, "NoteValue");
            break;
        case kTicks:
            strcpy(label, "Ticks");
            break;

        case k1000Samples:
            strcpy(label, "1000 samples");
            break;
        case k100Samples:
            strcpy(label, "100 samples");
            break;
        case k10Samples:
            strcpy(label, "10 samples");
            break;
        case kSamples:
            strcpy(label, "samples");
            break;

        default:
            strcpy(label, " ");
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiExactDelay::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kMode:
            if (param[index] == 0.0f)
                strcpy(text, "Off");
            else if (param[index] < 0.5f)
                strcpy(text, "Notes Only");
            else if (param[index] < 1.0f)
                strcpy(text, "Notes & CCs");
            else
                strcpy(text, "Everything");
            break;
        case kSync:
            if (timebase == sec)
                strcpy(text, "Seconds");
            else if (timebase == beat)
                strcpy(text, "NoteValue");
            else
                strcpy(text, "Samples");
            break;
        case kBeatDiv:
            if (Ticks == 2880.f)
                strcpy(text, "dotted 2");
            else if (Ticks == 2560.f)
                strcpy(text, "whole triplet");
            else if (Ticks == 1920.f)
                strcpy(text, "2");
            else if (Ticks == 1440.f)
                strcpy(text, "Dotted 4");
            else if (Ticks == 1280.f)
                strcpy(text, "2 Triplet");
            else if (Ticks == 960.f)
                strcpy(text, "4");
            else if (Ticks == 720.f)
                strcpy(text, "Dotted 8");
            else if (Ticks == 640.f)
                strcpy(text, "4 Triplet");
            else if (Ticks == 480.f)
                strcpy(text, "8");
            else if (Ticks == 360.f)
                strcpy(text, "Dotted 16");
            else if (Ticks == 320.f)
                strcpy(text, "8 Triplet");
            else if (Ticks == 240.f)
                strcpy(text, "16");
            else if (Ticks == 180.f)
                strcpy(text, "Dotted 32");
            else if (Ticks == 160.f)
                strcpy(text, "16 Triplet");
            else if (Ticks == 120.f)
                strcpy(text, "32");
            else if (Ticks == 90.f)
                strcpy(text, "Dotted 64");
            else if (Ticks == 80.f)
                strcpy(text, "32 Triplet");
            else if (Ticks == 60.f)
                strcpy(text, "64");
            else if (Ticks == 45.f)
                strcpy(text, "Dotted 128");
            else if (Ticks == 40.f)
                strcpy(text, "64 Triplet");
            else if (Ticks == 30.f)
                strcpy(text, "128");
            else
                strcpy(text, "--");
            break;
        case kBars:
            sprintf(text, "%d", roundToInt(param[index] * 32.f));
            break;
        case kBeats:
            sprintf(text, "%d", roundToInt(param[index] * 32.f));
            break;
        case kTicks:
            sprintf(text, "%d", roundToInt(param[kTicks] * 100.f));
            break;
        case k1000Samples:
            sprintf(text, "%d000", roundToInt(param[index] * 100.f));
            break;
        case k100Samples:
            sprintf(text, "%d00", roundToInt(param[index] * 9.f));
            break;
        case k10Samples:
            sprintf(text, "%d0", roundToInt(param[index] * 9.f));
            break;
        case kSamples:
            sprintf(text, "%d", roundToInt(param[index] * 9.f));
            break;
        case kSeconds:
            sprintf(text, "%d s", roundToInt(param[kSeconds] * 100.f));
            break;
        case kTenths:
            sprintf(text, "0.%d s", roundToInt(param[kTenths] * 9.f));
            break;
        case kHundredths:
            sprintf(text, "0.0%d s", roundToInt(param[kHundredths] * 9.f));
            break;
        case kThousandths:
            sprintf(text, "0.00%d s", roundToInt(param[kThousandths] * 9.f));
            break;
        case kWet:
            sprintf(text, "%d %%", roundToInt(param[index] * 100.f));
            break;
        case kChannel:
            if (FLOAT_TO_CHANNEL(param[index]) == -1)
                strcpy(text, "All");
            else
                sprintf(text, "%d", FLOAT_TO_CHANNEL(param[index]) + 1);
            break;
        default:
            sprintf(text, "%f", param[index]);
            break;
    }
}

void MidiExactDelay::cleanMidiDelayBuffer()
{
    midiDelayBuffer.clear();
}

void MidiExactDelay::preProcess(void)
{
    // preparing Process
    VstTimeInfo* timeInfo = NULL;
    timeInfo              = getTimeInfo(0xffff); //ALL

    int numerator   = 4;
    int denominator = 4;
    float bpm       = 120.f;

    if (timeInfo)
    {
        //if (kVstPpqPosValid & timeInfo->flags) _ppq = timeInfo->ppqPos;
        if (kVstTempoValid & timeInfo->flags)
            bpm = (float) timeInfo->tempo;
        if (kVstTimeSigValid & timeInfo->flags)
        {
            numerator   = timeInfo->timeSigNumerator;
            denominator = timeInfo->timeSigDenominator;
        }

        if (kVstTransportPlaying & timeInfo->flags)
            isplaying = true;
        else
            isplaying = false;
    }
    ppqPerBar           = ((float) numerator * 4.f / (float) denominator);
    samplesPerBeat      = roundToInt(60.0f * sampleRate / bpm);
    samplesPerNumerator = roundToInt((float) samplesPerBeat * (float) numerator * 0.25f);
    samplesPerTick      = roundToInt((float) samplesPerBeat / 960.0f);

    _cleanMidiOutBuffers();

    sortMidiEvents(midiDelayBuffer);
}

void MidiExactDelay::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples)
{
    short listenchannel = FLOAT_TO_CHANNEL(param[kChannel]);

    //calculate delay time
    VstInt32 delay = 0;
    if (timebase == sec)
    {
        delay = roundToInt((float) roundToInt(param[kSeconds] * 100.f) * sampleRate);
        delay += roundToInt((float) roundToInt(param[kTenths] * 9.f) * sampleRate * 0.1f);
        delay += roundToInt((float) roundToInt(param[kHundredths] * 9.f) * sampleRate * 0.01f);
        delay += roundToInt((float) roundToInt(param[kThousandths] * 9.f) * sampleRate * 0.001f);
    }
    else if (timebase == beat)
    {
        delay = roundToInt((float) samplesPerBeat * (float) roundToInt(param[kBars] * 32.f) * ppqPerBar);
        delay += roundToInt((float) samplesPerNumerator * (float) roundToInt(param[kBeats] * 32.f));
        delay += roundToInt((float) samplesPerTick * (Ticks + param[kTicks] * 100.f));
    }
    else if (timebase == samp)
    {
        delay = roundToInt(param[k1000Samples] * 100.f) * 1000;
        delay += roundToInt(param[k100Samples] * 9.f) * 100;
        delay += roundToInt(param[k10Samples] * 9.f) * 10;
        delay += roundToInt(param[kSamples] * 9.f);
    }

    //process delay buffer----------------------------------------------------------
    VstMidiEventVec newBuffer;
    newBuffer.clear();
    for (unsigned int i = 0; i < midiDelayBuffer.size(); i++)
    {
        VstMidiEvent event = midiDelayBuffer[i];
        short status       = event.midiData[0] & 0xf0; // scraping  channel
        //short channel    = event.midiData[0] & 0x0f;   // isolating channel (0-15)
        //short data1      = event.midiData[1] & 0x7f;
        short data2 = event.midiData[2] & 0x7f;

        if (status == MIDI_NOTEON && data2 == 0)
            status = MIDI_NOTEOFF;

        if (event.deltaFrames < samples && param[kWet] > 0.f)
        {
            //event is due, send it if output>0 and mode is right
            VstMidiEvent out = event;
            out.midiData[2]  = roundToInt((float) data2 * param[kWet]);
            if (status == MIDI_NOTEON && param[kMode] > 0.0f)
            {
                if (out.midiData[2] < 1)
                    out.midiData[2] = 1;
                outputs[0].push_back(out);
            }
            else if (status == MIDI_NOTEOFF && param[kMode] > 0.0f)
            {
                outputs[0].push_back(out);
            }
            else if (status == MIDI_CONTROLCHANGE && param[kMode] >= 0.5f)
            {
                outputs[0].push_back(out);
            }
            else if (param[kMode] == 1.0f)
            {
                outputs[0].push_back(out);
            }
        }
        else
        {
            //not due yet, try next block
            event.deltaFrames -= samples;
            newBuffer.push_back(event);
        }
    }
    //put leftover messages back into the buffer
    midiDelayBuffer.clear();
    midiDelayBuffer = newBuffer;

    //process incoming events-------------------------------------------------------
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        short status  = tomod.midiData[0] & 0xf0; // scraping  channel
        short channel = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        //short data1     = tomod.midiData[1] & 0x7f;
        short data2 = tomod.midiData[2] & 0x7f;

        if (status == MIDI_NOTEON && data2 == 0)
            status = MIDI_NOTEOFF;

        //only look at the selected channel
        if (channel == listenchannel || listenchannel == -1 && param[kMode] > 0.f)
        {
            VstMidiEvent delayed = tomod;
            if (samples - delayed.deltaFrames - delay > 0)
            {
                // delayed note is within the current block, don't need counter
                delayed.deltaFrames += delay;
                delayed.midiData[2] = roundToInt((float) data2 * param[kWet]);
                if (status == MIDI_NOTEON)
                {
                    if (param[kMode] > 0.0f)
                    {
                        if (delayed.midiData[2] < 1)
                            delayed.midiData[2] = 1;
                        outputs[0].push_back(delayed);
                    }
                }
                else if (status == MIDI_NOTEOFF)
                {
                    if (param[kMode] > 0.0f)
                        outputs[0].push_back(delayed);
                }
                else if (status == MIDI_CONTROLCHANGE)
                {
                    if (param[kMode] >= 0.5f)
                        outputs[0].push_back(delayed);
                }
                else if (param[kMode] == 1.0f)
                {
                    outputs[0].push_back(delayed);
                }
            }
            else
            {
                // delayed event is in a future block, so start counter:
                delayed.deltaFrames = (delay - (samples - delayed.deltaFrames));
                midiDelayBuffer.push_back(delayed);
            }
        } // if listenchannel==channel
    }     //for() inputs loop

    sortMidiEvents(outputs[0]);

    if (wasplaying && ! isplaying)
    { //just stopped
        dbg("stopped");
    }
    wasplaying = isplaying;
}
