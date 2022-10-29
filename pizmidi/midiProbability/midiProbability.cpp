#include "midiProbability.hpp"

//for fmod()
#include <cmath>

//for seeding rand()
#include <time.h>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiProbability(audioMaster);
}

MidiProbabilityProgram::MidiProbabilityProgram()
{
    // default Program Values
    for (int i = 0; i < kNumParams; i++)
    {
        param[i] = 0.f;
    }
    param[kPower]     = 0.35f;
    param[kMode1]     = 1.0f;
    param[kStep1]     = 0.5f;
    param[kOffset1]   = 0.0f;
    param[kProb1]     = 0.0f;
    param[kMode2]     = 1.0f;
    param[kStep2]     = 14.5f / 22.f;
    param[kOffset2]   = 0.0f;
    param[kProb2]     = 0.0f;
    param[kRadius]    = 0.5f;
    param[kRandV]     = 0.0f;
    param[kOctUp]     = 0.0f;
    param[kOctDn]     = 0.0f;
    param[kRandT]     = 0.0f;
    param[kSkip]      = 0.0f;
    param[kChan]      = 0.0f;
    param[kChCh]      = 0.0f;
    param[kTransp]    = 0.0f;
    param[kOffsetV]   = 0.0f;
    param[kTranspAmt] = 0.5f;
    param[kVelAmt]    = 0.5f;
    param[kChannel]   = 0.0f;
    // default program name
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiProbability::MidiProbability(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiProbabilityProgram[numPrograms];

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
                    vst_strncpy(programs[i].name, defaultBank->GetProgramName(i), kVstMaxProgNameLen);
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

    init();
}

//-----------------------------------------------------------------------------------------
MidiProbability::~MidiProbability()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiProbability::setProgram(VstInt32 program)
{
    if (program < numPrograms)
    {
        MidiProbabilityProgram* ap = &programs[program];
        curProgram                 = program;
        for (int i = 0; i < kNumParams; i++)
        {
            setParameter(i, ap->param[i]);
        }
    }
}

//------------------------------------------------------------------------
void MidiProbability::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiProbability::getProgramName(char* name)
{
    vst_strncpy(name, programs[curProgram].name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool MidiProbability::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        vst_strncpy(text, programs[index].name, kVstMaxProgNameLen);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiProbability::setParameter(VstInt32 index, float value)
{
    if (index < numParams)
    {
        MidiProbabilityProgram* ap = &programs[curProgram];
        param[index] = ap->param[index] = value;
        const float inc                 = 1.f / 22.f;
        if (index == kStep1)
        { //stepsize = "how many fit in half a beat"
            if (param[index] < 1 * inc)
            {
                stepsize[0] = 0.0416666667f; //0.3333333 (3 bars)
            }
            else if (param[index] < 2 * inc)
            {
                stepsize[0] = 0.0625f; //0.5 (2 whole notes)
            }
            else if (param[index] < 3 * inc)
            {
                stepsize[0] = 0.0833333333f; //0.75 (dotted whole)
            }
            else if (param[index] < 4 * inc)
            {
                stepsize[0] = 0.125f; //1 (whole note)
            }
            else if (param[index] < 5 * inc)
            {
                stepsize[0] = 0.166666667f; //1.5 dotted half
            }
            else if (param[index] < 6 * inc)
            {
                stepsize[0] = 0.25f; //2 (half note)
            }
            else if (param[index] < 7 * inc)
            {
                stepsize[0] = 0.333333333f; //4.
            }
            else if (param[index] < 8 * inc)
            {
                stepsize[0] = 0.375f; //2T
            }
            else if (param[index] < 9 * inc)
            {
                stepsize[0] = 0.5f; //4
            }
            else if (param[index] < 10 * inc)
            {
                stepsize[0] = 0.666666667f; //8.
            }
            else if (param[index] < 11 * inc)
            {
                stepsize[0] = 0.75f; //4T
            }
            else if (param[index] < 12 * inc)
            {
                stepsize[0] = 1.0f; //8
            }
            else if (param[index] < 13 * inc)
            {
                stepsize[0] = 1.333333333f; //16.
            }
            else if (param[index] < 14 * inc)
            {
                stepsize[0] = 1.5f; //8T
            }
            else if (param[index] < 15 * inc)
            {
                stepsize[0] = 2.0f; //16
            }
            else if (param[index] < 16 * inc)
            {
                stepsize[0] = 2.666666667f; //32.
            }
            else if (param[index] < 17 * inc)
            {
                stepsize[0] = 3.0f; //16T
            }
            else if (param[index] < 18 * inc)
            {
                stepsize[0] = 4.0f; //32
            }
            else if (param[index] < 19 * inc)
            {
                stepsize[0] = 5.333333333f; //64.
            }
            else if (param[index] < 20 * inc)
            {
                stepsize[0] = 6.0f; //32T
            }
            else if (param[index] < 21 * inc)
            {
                stepsize[0] = 8.0f; //64
            }
            else
            {
                stepsize[0] = 99.0f;
            }
            stepsize[0] = stepsize[0] * 2.0f;
        }
        if (index == kStep2)
        { //stepsize = "how many fit in half a beat"
            if (param[index] < 1 * inc)
            {
                stepsize[1] = 0.0416666667f; //0.3333333 (3 bars)
            }
            else if (param[index] < 2 * inc)
            {
                stepsize[1] = 0.0625f; //0.5 (2 whole notes)
            }
            else if (param[index] < 3 * inc)
            {
                stepsize[1] = 0.0833333333f; //0.75 (dotted whole)
            }
            else if (param[index] < 4 * inc)
            {
                stepsize[1] = 0.125f; //1 (whole note)
            }
            else if (param[index] < 5 * inc)
            {
                stepsize[1] = 0.166666667f; //1.5 dotted half
            }
            else if (param[index] < 6 * inc)
            {
                stepsize[1] = 0.25f; //2 (half note)
            }
            else if (param[index] < 7 * inc)
            {
                stepsize[1] = 0.333333333f; //4.
            }
            else if (param[index] < 8 * inc)
            {
                stepsize[1] = 0.375f; //2T
            }
            else if (param[index] < 9 * inc)
            {
                stepsize[1] = 0.5f; //4
            }
            else if (param[index] < 10 * inc)
            {
                stepsize[1] = 0.666666667f; //8.
            }
            else if (param[index] < 11 * inc)
            {
                stepsize[1] = 0.75f; //4T
            }
            else if (param[index] < 12 * inc)
            {
                stepsize[1] = 1.0f; //8
            }
            else if (param[index] < 13 * inc)
            {
                stepsize[1] = 1.333333333f; //16.
            }
            else if (param[index] < 14 * inc)
            {
                stepsize[1] = 1.5f; //8T
            }
            else if (param[index] < 15 * inc)
            {
                stepsize[1] = 2.0f; //16
            }
            else if (param[index] < 16 * inc)
            {
                stepsize[1] = 2.666666667f; //32.
            }
            else if (param[index] < 17 * inc)
            {
                stepsize[1] = 3.0f; //16T
            }
            else if (param[index] < 18 * inc)
            {
                stepsize[1] = 4.0f; //32
            }
            else if (param[index] < 19 * inc)
            {
                stepsize[1] = 5.333333333f; //64.
            }
            else if (param[index] < 20 * inc)
            {
                stepsize[1] = 6.0f; //32T
            }
            else if (param[index] < 21 * inc)
            {
                stepsize[1] = 8.0f; //64
            }
            else
            {
                stepsize[1] = 99.0f;
            }
            stepsize[1] = stepsize[1] * 2.0f;
        }
        else if (index == kProb1)
        {
            //force top & bottom values to be what they say they are
            if (roundToInt(param[index] * 100.0f) == 0)
            {
                param[index] = 0.0f;
            }
            else if (roundToInt(param[index] * 100.0f) == 100)
            {
                param[index] = 1.0f;
            }
        }
        else if (index == kProb2)
        {
            //force top & bottom values to be what they say they are
            if (roundToInt(param[index] * 100.0f) == 0)
            {
                param[index] = 0.0f;
            }
            else if (roundToInt(param[index] * 100.0f) == 100)
            {
                param[index] = 1.0f;
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
float MidiProbability::getParameter(VstInt32 index)
{
    if (index < numParams)
    {
        return param[index];
    }
    return 0.f;
}

//-----------------------------------------------------------------------------------------
void MidiProbability::getParameterName(VstInt32 index, char* label)
{
    if (index < numParams)
    {
        switch (index)
        {
            case kPower:
                strcpy(label, "Power");
                break;

            case kStep1:
                strcpy(label, "1: Step");
                break;
            case kOffset1:
                strcpy(label, "1: StepShift");
                break;
            case kMode1:
                strcpy(label, "1: Mode");
                break;
            case kProb1:
                strcpy(label, "1: Prob");
                break;

            case kStep2:
                strcpy(label, "2: Step");
                break;
            case kOffset2:
                strcpy(label, "2: StepShift");
                break;
            case kMode2:
                strcpy(label, "2: Mode");
                break;
            case kProb2:
                strcpy(label, "2: Prob2");
                break;

            case kRadius:
                strcpy(label, "TimeFocus");
                break;
            case kSkip:
                strcpy(label, "Multi-Skip");
                break;
            case kOctUp:
                strcpy(label, "Multi-OctUp");
                break;
            case kOctDn:
                strcpy(label, "Multi-OctDn");
                break;
            case kRandT:
                strcpy(label, "Multi-RndTrnsp");
                break;
            case kRandV:
                strcpy(label, "Multi-RndVeloc");
                break;
            case kTransp:
                strcpy(label, "Multi-Transpose");
                break;
            case kOffsetV:
                strcpy(label, "Multi-OffsetVel");
                break;
            case kChan:
                strcpy(label, "Multi-Chnlze");
                break;
            case kTranspAmt:
                strcpy(label, "Transpose");
                break;
            case kVelAmt:
                strcpy(label, "VelOffset");
                break;
            case kChCh:
                strcpy(label, "ChnlizeChan");
                break;
            case kChannel:
                strcpy(label, "InputChan");
                break;
            default:
                break;
        }
    }
}

//-----------------------------------------------------------------------------------------
void MidiProbability::getParameterDisplay(VstInt32 index, char* text)
{
    if (index < numParams)
    {
        const float inc = 1.f / 22.f;
        switch (index)
        {
            case kPower:
                if (param[index] < 0.25f)
                {
                    strcpy(text, "Off");
                }
                else if (param[index] < 0.5f)
                {
                    strcpy(text, "On (Notes)");
                }
                else if (param[index] < 0.75f)
                {
                    strcpy(text, "On (Notes&CCs)");
                }
                else
                {
                    strcpy(text, "On (Everything)");
                }
                break;
            case kMode1:
                if (param[index] < 0.1f)
                {
                    strcpy(text, "Off");
                }
                else if (param[index] < 0.2f)
                {
                    strcpy(text, "Skip");
                }
                else if (param[index] < 0.3f)
                {
                    strcpy(text, "OctaveUp");
                }
                else if (param[index] < 0.4f)
                {
                    strcpy(text, "OctaveDown");
                }
                else if (param[index] < 0.5f)
                {
                    strcpy(text, "RandTransp");
                }
                else if (param[index] < 0.6f)
                {
                    strcpy(text, "Transpose");
                }
                else if (param[index] < 0.7f)
                {
                    strcpy(text, "RandVeloc");
                }
                else if (param[index] < 0.8f)
                {
                    strcpy(text, "OffsetVel");
                }
                else if (param[index] < 0.9f)
                {
                    strcpy(text, "Channelize");
                }
                else
                {
                    strcpy(text, "Multi");
                }
                break;
            case kMode2:
                if (param[index] < 0.1f)
                {
                    strcpy(text, "Off");
                }
                else if (param[index] < 0.2f)
                {
                    strcpy(text, "Skip");
                }
                else if (param[index] < 0.3f)
                {
                    strcpy(text, "OctaveUp");
                }
                else if (param[index] < 0.4f)
                {
                    strcpy(text, "OctaveDown");
                }
                else if (param[index] < 0.5f)
                {
                    strcpy(text, "RandTransp");
                }
                else if (param[index] < 0.6f)
                {
                    strcpy(text, "Transpose");
                }
                else if (param[index] < 0.7f)
                {
                    strcpy(text, "RandVeloc");
                }
                else if (param[index] < 0.8f)
                {
                    strcpy(text, "OffsetVel");
                }
                else if (param[index] < 0.9f)
                {
                    strcpy(text, "Channelize");
                }
                else
                {
                    strcpy(text, "Multi");
                }
                break;
            case kStep1:
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
                else if (param[index] < 21 * inc)
                {
                    strcpy(text, "64");
                }
                else
                {
                    strcpy(text, "All Notes");
                }
                break;
            case kStep2:
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
                else if (param[index] < 21 * inc)
                {
                    strcpy(text, "64");
                }
                else
                {
                    strcpy(text, "All Notes");
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
            case kTranspAmt:
                sprintf(text, "%d", roundToInt(72.f * param[index]) - 36);
                break;
            case kVelAmt:
                sprintf(text, "%d", roundToInt(128.f * param[index]) - 64);
                break;
            case kChCh:
                if (FLOAT_TO_CHANNEL(param[index]) == -1)
                {
                    strcpy(text, "Random");
                }
                else
                {
                    sprintf(text, "%d", FLOAT_TO_CHANNEL(param[index]) + 1);
                }
                break;
            default:
                sprintf(text, "%d", roundToInt(param[index] * 100.0f));
                break;
        }
    }
}

//-----------------------------------------------------------------------------------------
void MidiProbability::getParameterLabel(VstInt32 index, char* label)
{
    if (index < numParams)
    {
        switch (index)
        {
            case kPower:
            case kMode1:
            case kMode2:
            case kStep1:
            case kStep2:
            case kChannel:
            case kChCh:
            case kTranspAmt:
            case kVelAmt:
                strcpy(label, " ");
                break;
            default:
                strcpy(label, "%");
                break;
        }
    }
}

//-----------------------------------------------------------------------------------------
bool MidiProbability::init(void)
{
    srand((unsigned int) time(NULL));

    _ppq = 0.0;
    _bpm = 120.0f;
    for (int slot = 0; slot < numSlots; slot++)
    {
        _beatpos[slot] = 0.0;
    }
    samplesPerBeat = 0;

    for (int ch = 0; ch < 16; ch++)
    {
        for (int n = 0; n < 128; n++)
        {
            notePlaying[n][ch] = false;
            for (int m = 0; m < numEffects; m++)
            {
                noteAffected[n][ch][m] = 0;
            }
        }
    }

    wasplaying = false;
    isplaying  = false;

    return PizMidi::init();
}

void MidiProbability::preProcess(void)
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

        for (int i = 0; i < numSlots; i++)
        {
            double offset = (1.0f - 2.0f * param[kOffset1 + numParamsPerSlot * i]) / (stepsize[i] * 2.0f);
            _beatpos[i]   = stepsize[i] * fmod(_ppq + offset, (1.0 / (double) stepsize[i]));
        }
    }

    samplesPerBeat = roundToInt(60.0f * sampleRate / _bpm);

    //if (reaper) {
    // convert a time into beats.
    // if measures is non-NULL, measures will be set to the measure count, return value will be beats since measure.
    // if cml is non-NULL, will be set to current measure length in beats (i.e. time signature numerator)
    // if fullbeats is non-NULL, and measures is non-NULL, fullbeats will get the full beat count (same value returned if measures is NULL).
    // if cdenom is non-NULL, will be set to the current time signature denominator.
    //double (*TimeMap2_timeToBeats)(ReaProject *proj, double tpos, int *measures, int *cml, double *fullbeats, int *cdenom);
    //double timeInSeconds = GetPlayPosition();
    //int bars=0;
    //double beats = TimeMap2_timeToBeats(NULL, timeInSeconds, &bars, NULL, NULL, NULL);
    //if (isplaying) {DEBUG_OUT("Bar: " << bars << " Beat: " << beats);}
    //}

    _cleanMidiOutBuffers();
}

void MidiProbability::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples)
{
    char listenchannel = FLOAT_TO_CHANNEL(param[kChannel]);
    char chch          = FLOAT_TO_CHANNEL(param[kChCh]);

    //process incoming events-------------------------------------------------------
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        int status        = tomod.midiData[0] & 0xf0; // scraping  channel
        const int channel = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        const int data1   = tomod.midiData[1] & 0x7f;
        const int data2   = tomod.midiData[2] & 0x7f;

        //make 0-velocity notes into "real" noteoffs for simplicity
        if (status == MIDI_NOTEON && data2 == 0)
        {
            status            = MIDI_NOTEOFF;
            tomod.midiData[0] = MIDI_NOTEOFF | channel;
        }

        bool discard = false;
        int outch    = channel;
        int outd1    = data1;
        int outd2    = data2;

        if (chch == -1)
        {
            chch = rand() % 16;
        }

        //only look at the selected channel
        if (channel == listenchannel || listenchannel == -1)
        {
            //Incoming NoteOn---------------------------------------------------------------
            if (status == MIDI_NOTEON)
            {
                bool stop = false; //use this to enforce slot precedence?????????
                // find the location of the note within the beat, in samples:
                for (int slot = 0; slot < numSlots; slot++)
                {
                    VstInt32 beatpos_samples = roundToInt(_beatpos[slot] * samplesPerBeat) + tomod.deltaFrames;
                    // find "closeness" to selected stepsize
                    float c = closeness(beatpos_samples);
                    if (param[kStep1 + numParamsPerSlot * slot] >= (21.f / 22.f))
                    {
                        c = 1.0f; //apply to every note
                    }
                    if (stop)
                    {
                        c = 0.0f;
                    }

                    float prob = param[kProb1 + numParamsPerSlot * slot];

                    int index = kMode1 + numParamsPerSlot * slot;
                    if (param[index] < 0.1f)
                    {
                        mode = off;
                    }
                    else if (param[index] < 0.2f)
                    {
                        mode = disc;
                    }
                    else if (param[index] < 0.3f)
                    {
                        mode = octup;
                    }
                    else if (param[index] < 0.4f)
                    {
                        mode = octdown;
                    }
                    else if (param[index] < 0.5f)
                    {
                        mode = randtr;
                    }
                    else if (param[index] < 0.6f)
                    {
                        mode = transp;
                    }
                    else if (param[index] < 0.7f)
                    {
                        mode = randvel;
                    }
                    else if (param[index] < 0.8f)
                    {
                        mode = offsetvel;
                    }
                    else if (param[index] < 0.9f)
                    {
                        mode = chan;
                    }
                    else
                    {
                        mode = multi;
                    }

                    if (param[kPower] < 0.25f)
                    {
                        mode = off;
                    }

                    //probability
                    //don't do anything unless prob > 0%, and mode is not "off"
                    if (roundToInt(100.0f * prob) > 0 && mode != off)
                    {
                        float p = c * 100.0f * prob;
                        float R = (float) (rand() % 99);
                        if (R < p)
                        {
                            char transpose = 0;
                            char vel       = 0;
                            if (mode == chan || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kChan] || mode == chan)
                                {
                                    outch                              = chch;
                                    tomod.midiData[0]                  = MIDI_NOTEON | outch;
                                    noteAffected[data1][channel][chan] = outch + 1;
                                    stop                               = true;
                                }
                            }
                            if (mode == disc || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kSkip] || mode == disc)
                                {
                                    discard                            = true;
                                    noteAffected[data1][channel][disc] = 1;
                                    stop                               = true;
                                }
                            }
                            if (mode == octup || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kOctUp] || mode == octup)
                                {
                                    outd1 += 12;
                                    noteAffected[data1][channel][octup] = 12;
                                    stop                                = true;
                                }
                            }
                            if (mode == octdown || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kOctDn] || mode == octdown)
                                {
                                    outd1 -= 12;
                                    noteAffected[data1][channel][octdown] = -12;
                                    stop                                  = true;
                                }
                            }
                            if (mode == randtr || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kRandT] || mode == randtr)
                                {
                                    transpose = rand() % 24 - 12;
                                    if (outd1 + transpose < 0)
                                    {
                                        transpose = -transpose;
                                    }
                                    else if (outd1 + transpose > 127)
                                    {
                                        transpose = -transpose;
                                    }
                                    outd1 += transpose;
                                    noteAffected[data1][channel][randtr] = transpose;
                                    stop                                 = true;
                                }
                            }
                            if (mode == transp || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kTransp] || mode == transp)
                                {
                                    transpose = roundToInt(72.f * param[kTranspAmt]) - 36;
                                    if (outd1 + transpose < 0)
                                    {
                                        transpose = -transpose;
                                    }
                                    else if (outd1 + transpose > 127)
                                    {
                                        transpose = -transpose;
                                    }
                                    outd1 += transpose;
                                    noteAffected[data1][channel][transp] = transpose;
                                    stop                                 = true;
                                }
                            }
                            if (mode == randvel || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kRandV] || mode == randvel)
                                {
                                    vel = rand() % 128 - 64;
                                    if (outd2 + vel < 1)
                                    {
                                        vel = -vel;
                                    }
                                    else if (outd2 + vel > 127)
                                    {
                                        vel = -vel;
                                    }
                                    outd2 += vel;
                                    tomod.midiData[2]                     = outd2;
                                    noteAffected[data1][channel][randvel] = vel;
                                    stop                                  = true;
                                }
                            }
                            if (mode == offsetvel || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kRandV] || mode == offsetvel)
                                {
                                    vel = roundToInt(param[kVelAmt] * 128.f) - 64;
                                    if (outd2 + vel < 1)
                                    {
                                        vel = -vel;
                                    }
                                    else if (outd2 + vel > 127)
                                    {
                                        vel = -vel;
                                    }
                                    outd2 += vel;
                                    tomod.midiData[2]                       = outd2;
                                    noteAffected[data1][channel][offsetvel] = vel;
                                    stop                                    = true;
                                }
                            }
                            if (outd1 > 127)
                            {
                                outd1 = 127;
                            }
                            else if (outd1 < 0)
                            {
                                outd1 = 0;
                            }
                            tomod.midiData[1] = outd1;
                        }
                    }
                }
                if (! discard)
                {
                    notePlaying[outd1][outch] = true;
                }
            }
            //incoming NoteOff--------------------------------------------------------------
            else if (status == MIDI_NOTEOFF)
            {
                if (noteAffected[data1][channel][chan] != 0)
                {
                    outch                              = noteAffected[data1][channel][chan] - 1;
                    tomod.midiData[0]                  = MIDI_NOTEOFF | outch;
                    noteAffected[data1][channel][chan] = 0;
                }
                if (noteAffected[data1][channel][disc] == 1)
                {
                    discard                            = true;
                    noteAffected[data1][channel][disc] = 0;
                }
                if (noteAffected[data1][channel][octup] != 0)
                {
                    outd1 += 12;
                    noteAffected[data1][channel][octup] = 0;
                }
                if (noteAffected[data1][channel][octdown] != 0)
                {
                    outd1 -= 12;
                    noteAffected[data1][channel][octdown] = 0;
                }
                if (noteAffected[data1][channel][randtr] != 0)
                {
                    outd1 += (noteAffected[data1][channel][randtr]);
                    noteAffected[data1][channel][randtr] = 0;
                }
                if (noteAffected[data1][channel][transp] != 0)
                {
                    outd1 += (noteAffected[data1][channel][transp]);
                    noteAffected[data1][channel][randtr] = 0;
                }
                if (outd1 > 127)
                {
                    outd1 = 127;
                }
                else if (outd1 < 0)
                {
                    outd1 = 0;
                }
                tomod.midiData[1] = outd1;
                if (! discard)
                {
                    notePlaying[outd1][outch] = false;
                }
            }
            //Incoming CC-------------------------------------------------------------
            else if (status == MIDI_CONTROLCHANGE)
            {
                bool stop = false;
                for (int slot = 0; slot < numSlots; slot++)
                {
                    VstInt32 beatpos_samples = roundToInt(_beatpos[slot] * samplesPerBeat) + tomod.deltaFrames;
                    // find "closeness" to selected stepsize
                    float c = closeness(beatpos_samples);
                    if (param[kStep1 + numParamsPerSlot * slot] >= (21.f / 22.f))
                    {
                        c = 1.0f; //apply to every note
                    }
                    if (stop)
                    {
                        c = 0.0f;
                    }

                    float prob = param[kProb1 + numParamsPerSlot * slot];

                    int index = kMode1 + numParamsPerSlot * slot;
                    if (param[index] < 0.1f)
                    {
                        mode = off;
                    }
                    else if (param[index] < 0.2f)
                    {
                        mode = disc;
                    }
                    else if (param[index] < 0.3f)
                    {
                        mode = octup;
                    }
                    else if (param[index] < 0.4f)
                    {
                        mode = octdown;
                    }
                    else if (param[index] < 0.5f)
                    {
                        mode = randtr;
                    }
                    else if (param[index] < 0.6f)
                    {
                        mode = transp;
                    }
                    else if (param[index] < 0.7f)
                    {
                        mode = randvel;
                    }
                    else if (param[index] < 0.8f)
                    {
                        mode = offsetvel;
                    }
                    else if (param[index] < 0.9f)
                    {
                        mode = chan;
                    }
                    else
                    {
                        mode = multi;
                    }

                    if (param[kPower] < 0.5f)
                    {
                        mode = off;
                    }

                    //probability
                    //don't do anything unless prob > 0%, and mode is not "off"
                    if (roundToInt(100.0f * prob) > 0 && mode != off)
                    {
                        float p = c * 100.0f * prob;
                        float R = (float) (rand() % 99);
                        if (R < p)
                        {
                            char transpose = 0;
                            char vel       = 0;
                            if (mode == chan || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kChan] || mode == chan)
                                {
                                    outch             = chch;
                                    tomod.midiData[0] = MIDI_CONTROLCHANGE | outch;
                                    stop              = true;
                                }
                            }
                            if (mode == disc || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kSkip] || mode == disc)
                                {
                                    discard = true;
                                    stop    = true;
                                }
                            }
                            if (mode == randtr || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kRandT] || mode == randtr)
                                {
                                    transpose = rand() % 24 - 12;
                                    if (outd1 + transpose < 0)
                                    {
                                        transpose = -transpose;
                                    }
                                    else if (outd1 + transpose > 127)
                                    {
                                        transpose = -transpose;
                                    }
                                    outd1 += transpose;
                                    tomod.midiData[1]                    = outd1;
                                    noteAffected[data1][channel][randtr] = transpose;
                                    stop                                 = true;
                                }
                            }
                            if (mode == randvel || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kRandV] || mode == randvel)
                                {
                                    vel = rand() % 128 - 64;
                                    if (outd2 + vel < 1)
                                    {
                                        vel = -vel;
                                    }
                                    else if (outd2 + vel > 127)
                                    {
                                        vel = -vel;
                                    }
                                    outd2 += vel;
                                    tomod.midiData[2]                     = outd2;
                                    noteAffected[data1][channel][randvel] = vel;
                                    stop                                  = true;
                                }
                            }
                        }
                    }
                }
            }
            //Other-------------------------------------------------------------------
            else
            {
                bool stop = false;
                for (int slot = 0; slot < numSlots; slot++)
                {
                    VstInt32 beatpos_samples = roundToInt(_beatpos[slot] * samplesPerBeat) + tomod.deltaFrames;
                    // find "closeness" to selected stepsize
                    float c = closeness(beatpos_samples);
                    if (param[kStep1 + numParamsPerSlot * slot] >= (21.f / 22.f))
                    {
                        c = 1.0f; //apply to every note
                    }
                    if (stop)
                    {
                        c = 0.0f;
                    }

                    float prob = param[kProb1 + numParamsPerSlot * slot];

                    int index = kMode1 + numParamsPerSlot * slot;
                    if (param[index] == 0.0f)
                    {
                        mode = off;
                    }
                    else if (param[index] < 0.1f)
                    {
                        mode = disc;
                    }
                    else if (param[index] < 0.2f)
                    {
                        mode = octup;
                    }
                    else if (param[index] < 0.3f)
                    {
                        mode = octdown;
                    }
                    else if (param[index] < 0.4f)
                    {
                        mode = randtr;
                    }
                    else if (param[index] < 0.5f)
                    {
                        mode = randvel;
                    }
                    else if (param[index] < 0.6f)
                    {
                        mode = chan;
                    }
                    // else if (param[index] < 0.7f)
                    // {
                    //     mode = ;
                    // }
                    // else if (param[index] < 0.8f)
                    // {
                    //     mode = ;
                    // }
                    // else if (param[index] < 0.9f)
                    // {
                    //     mode = ;
                    // }
                    // else if (param[index] < 1.0f)
                    // {
                    //     mode = ;
                    // }
                    else
                    {
                        mode = multi;
                    }

                    if (param[kPower] < 0.75f)
                    {
                        mode = off;
                    }

                    //probability
                    //don't do anything unless prob > 0%, and mode is not "off"
                    if (roundToInt(100.0f * prob) > 0 && mode != off)
                    {
                        float p = c * 100.0f * prob;
                        float R = (float) (rand() % 99);
                        if (R < p)
                        {
                            char transpose = 0;
                            char vel       = 0;
                            if (mode == chan || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kChan] || mode == chan)
                                {
                                    outch             = chch;
                                    tomod.midiData[0] = status | outch;
                                    stop              = true;
                                }
                            }
                            if (mode == disc || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kSkip] || mode == disc)
                                {
                                    discard = true;
                                    stop    = true;
                                }
                            }
                            if (mode == randtr || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kRandT] || mode == randtr)
                                {
                                    transpose = rand() % 24 - 12;
                                    if (outd1 + transpose < 0)
                                    {
                                        transpose = -transpose;
                                    }
                                    else if (outd1 + transpose > 127)
                                    {
                                        transpose = -transpose;
                                    }
                                    outd1 += transpose;
                                    tomod.midiData[1]                    = outd1;
                                    noteAffected[data1][channel][randtr] = transpose;
                                    stop                                 = true;
                                }
                            }
                            if (mode == randvel || mode == multi)
                            {
                                float r = (float) (rand() % 99);
                                if (r < p * param[kRandV] || mode == randvel)
                                {
                                    vel = rand() % 128 - 64;
                                    if (outd2 + vel < 1)
                                    {
                                        vel = -vel;
                                    }
                                    else if (outd2 + vel > 127)
                                    {
                                        vel = -vel;
                                    }
                                    outd2 += vel;
                                    tomod.midiData[2]                     = outd2;
                                    noteAffected[data1][channel][randvel] = vel;
                                    stop                                  = true;
                                }
                            }
                        }
                    }
                }
            }
        } // if listenchannel==channel
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    } //for() inputs loop

    if (wasplaying && ! isplaying)
    { //just stopped
        dbg("stopped");
        for (int ch = 0; ch < 16; ch++)
        {
            for (int n = 0; n < 128; n++)
            {
                if (notePlaying[n][ch])
                {
                    VstMidiEvent kill;
                    kill.midiData[0] = MIDI_NOTEOFF + ch;
                    kill.midiData[1] = n;
                    kill.midiData[2] = 0;
                    kill.deltaFrames = samples - 1;
                    kill.detune      = 0;
                    outputs[0].push_back(kill);
                    dbg("stopped, killed note " << n);
                }
            }
        }
    }
    wasplaying = isplaying;
}

inline float MidiProbability::closeness(long int samplepos)
{
    //	double x = (double)samplepos/(double)samplesPerBeat;
    //float r = 4+16.0f*getParameter(kRadius);
    float result = 0;
    //	if (param[kShape]<0.25f) //parabola
    //    	result = 1 - (2*r*x - r) * (2*r*x - r);
    //	else if (param[kShape]<0.5f) //square

    if (abs(samplepos - samplesPerBeat / 2) <= samplesPerBeat / (param[kRadius] * 13 + 3))
    {
        //if (abs(samplepos-samplesPerBeat/2) <= samplesPerBeat/(0.5f*13+3) ) {
        result = 1.0f;
    }
    //    else if (param[kShape]<0.75f) //linear up
    //         result = (r+2.0f)*x-(r+1.0f)*0.5f;
    //    else //linear down
    //         result = r*0.5f+1.5f-(r+2.0f)*x;
    if (result < 0.0001f)
    {
        result = 0;
    }
    //DEBUG_OUT(x << " " << result);
    return result;
}
