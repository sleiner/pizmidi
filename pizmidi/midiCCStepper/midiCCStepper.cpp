#include "midiCCStepper.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiCCStepper(audioMaster);
}

MidiCCStepperProgram::MidiCCStepperProgram()
{
    // default Program Values
    for (int i = 0; i < kNumParams; i++)
    {
        switch (i)
        {
            case kTimePower:
                param[i] = 0.f;
                break;
            case kValuePower:
                param[i] = 0.f;
                break;
            case kHighCC:
                param[i] = 1.f;
                break;
            case kCCOffset:
                param[i] = 0.5f;
                break;
            case kSync:
                param[i] = 0.f;
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
MidiCCStepper::MidiCCStepper(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams)
{
    programs = new MidiCCStepperProgram[numPrograms];

    if (programs)
    {
        setProgram(0);
    }

    lastTime = 0 - samplesPerStep;

    init();
}

//-----------------------------------------------------------------------------------------
MidiCCStepper::~MidiCCStepper()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiCCStepper::setProgram(VstInt32 program)
{
    MidiCCStepperProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter(i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiCCStepper::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiCCStepper::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiCCStepper::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiCCStepper::setParameter(VstInt32 index, float value)
{
    MidiCCStepperProgram* ap = &programs[curProgram];
    param[index] = ap->param[index] = value;
    switch (index)
    {
        case kLowCC:
            if (value > param[kHighCC])
            {
                setParameterAutomated(kHighCC, value);
            }
            break;
        case kHighCC:
            if (value < param[kLowCC])
            {
                setParameterAutomated(kLowCC, value);
            }
            break;
        case kTimeQ: //beatdiv = "how many fit in a beat"
            if (param[index] == 0.0f)
            {
                beatdiv = 32.0f; //128th
            }
            else if (param[index] < 0.05f)
            {
                beatdiv = 24.f; //64th triplet
            }
            else if (param[index] < 0.1f)
            {
                beatdiv = 21.33333333f; //dotted 128th
            }
            else if (param[index] < 0.15f)
            {
                beatdiv = 16.f; //64th
            }
            else if (param[index] < 0.2f)
            {
                beatdiv = 12.f; //32nd triplet
            }
            else if (param[index] < 0.25f)
            {
                beatdiv = 10.66666667f; //dotted 64th
            }
            else if (param[index] < 0.3f)
            {
                beatdiv = 8.f; //32nd
            }
            else if (param[index] < 0.35f)
            {
                beatdiv = 6.f; //16th triplet
            }
            else if (param[index] < 0.4f)
            {
                beatdiv = 5.333333333f; //dotted 32nd
            }
            else if (param[index] < 0.45)
            {
                beatdiv = 4.f; //16th
            }
            else if (param[index] < 0.5f)
            {
                beatdiv = 3.f; //8th triplet
            }
            else if (param[index] < 0.55f)
            {
                beatdiv = 2.666666667f; //dotted 16th
            }
            else if (param[index] < 0.6f)
            {
                beatdiv = 2.f; //8th
            }
            else if (param[index] < 0.65f)
            {
                beatdiv = 1.5f; //quarter triplet
            }
            else if (param[index] < 0.7f)
            {
                beatdiv = 1.333333333f; //dotted 8th
            }
            else if (param[index] < 0.75f)
            {
                beatdiv = 1.f; //quarter
            }
            else if (param[index] < 0.8f)
            {
                beatdiv = 0.75f; //half triplet
            }
            else if (param[index] < 0.85f)
            {
                beatdiv = 0.666666667f; //dotted quarter
            }
            else if (param[index] < 0.9f)
            {
                beatdiv = 0.5; //half
            }
            else if (param[index] < 0.95f)
            {
                beatdiv = 0.375f; //whole triplet
            }
            else if (param[index] < 1.0f)
            {
                beatdiv = 0.333333333f; //dotted half
            }
            else
            {
                beatdiv = 0.25f; //whole
            }
            break;
            //case kBeatOffset:
            //     if (param[kSync]<0.5f)
            //        leftOverSamples = roundToInt((float)samplesPerStep*(param[kBeatOffset]/beatdiv));
            //     break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiCCStepper::getParameter(VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiCCStepper::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kValuePower:
            strcpy(label, "Val Quantize");
            break;
        case kValueQ:
            strcpy(label, "ValQ Steps");
            break;
        case kTimePower:
            strcpy(label, "Time Quantize");
            break;
        case kTimeQ:
            strcpy(label, "Note Value");
            break;
        case kSync:
            strcpy(label, "Host Sync");
            break;
        case kBeatOffset:
            strcpy(label, "Beat Offset");
            break;
        case kLowCC:
            strcpy(label, "Low CC");
            break;
        case kHighCC:
            strcpy(label, "High CC");
            break;
        case kCCOffset:
            strcpy(label, "CC Shift");
            break;
        case kChannel:
            strcpy(label, "Channel");
            break;
        case kThru:
            strcpy(label, "NotesThru");
            break;
        case kSendAfter:
            strcpy(label, "SendCCAfter");
            break;
        default:
            sprintf(label, "param %d", index);
    }
}

//-----------------------------------------------------------------------------------------
void MidiCCStepper::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kTimeQ:
            if (param[index] == 0.0f)
            {
                strcpy(text, "128");
            }
            else if (param[index] < 0.05f)
            {
                strcpy(text, "64 triplet");
            }
            else if (param[index] < 0.1f)
            {
                strcpy(text, "dotted 128");
            }
            else if (param[index] < 0.15f)
            {
                strcpy(text, "64");
            }
            else if (param[index] < 0.2f)
            {
                strcpy(text, "32 triplet");
            }
            else if (param[index] < 0.25f)
            {
                strcpy(text, "dotted 64");
            }
            else if (param[index] < 0.3f)
            {
                strcpy(text, "32");
            }
            else if (param[index] < 0.35f)
            {
                strcpy(text, "16 triplet");
            }
            else if (param[index] < 0.4f)
            {
                strcpy(text, "dotted 32");
            }
            else if (param[index] < 0.45)
            {
                strcpy(text, "16");
            }
            else if (param[index] < 0.5f)
            {
                strcpy(text, "8 triplet");
            }
            else if (param[index] < 0.55f)
            {
                strcpy(text, "dotted 16");
            }
            else if (param[index] < 0.6f)
            {
                strcpy(text, "8");
            }
            else if (param[index] < 0.65f)
            {
                strcpy(text, "4 triplet");
            }
            else if (param[index] < 0.7f)
            {
                strcpy(text, "dotted 8");
            }
            else if (param[index] < 0.75f)
            {
                strcpy(text, "4");
            }
            else if (param[index] < 0.8f)
            {
                strcpy(text, "2 triplet");
            }
            else if (param[index] < 0.85f)
            {
                strcpy(text, "dotted 4");
            }
            else if (param[index] < 0.9f)
            {
                strcpy(text, "2");
            }
            else if (param[index] < 0.95f)
            {
                strcpy(text, "whole triplet");
            }
            else if (param[index] < 1.0f)
            {
                strcpy(text, "dotted 2");
            }
            else
            {
                strcpy(text, "whole");
            }
            break;
        case kValueQ:
            sprintf(text, "%d", roundToInt(param[kValueQ] * 63.f) + 2);
            if (roundToInt(param[kValueQ] * 63.f) == 63)
            {
                sprintf(text, "128");
            }
            break;
        case kLowCC:
            sprintf(text, "%d", FLOAT_TO_MIDI(param[index]));
            break;
        case kHighCC:
            sprintf(text, "%d", FLOAT_TO_MIDI(param[index]));
            break;
        case kCCOffset:
            sprintf(text, "%d", roundToInt(254.f * param[index]) - 127);
            break;
        case kChannel:
            if (FLOAT_TO_CHANNEL(param[index]) == -1)
            {
                strcpy(text, "Any");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL(param[index]) + 1);
            }
            break;
        case kThru:
            if (param[index] < 0.5f)
            {
                strcpy(text, "Block Notes");
            }
            else
            {
                strcpy(text, "Notes Thru");
            }
            break;
        case kTimePower:
            if (param[index] < 0.5f)
            {
                strcpy(text, "Off");
            }
            else
            {
                strcpy(text, "On");
            }
            break;
        case kSendAfter:
            if (param[index] < 0.5f)
            {
                strcpy(text, "Off");
            }
            else
            {
                strcpy(text, "On");
            }
            break;
        case kValuePower:
            if (param[index] < 0.5f)
            {
                strcpy(text, "Off");
            }
            else
            {
                strcpy(text, "On");
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
        default:
            sprintf(text, "%f", param[index]);
    }
}

//-----------------------------------------------------------------------------------------
bool MidiCCStepper::init(void)
{
    isplaying       = false;
    wasplaying      = false;
    _bpm            = 120;
    _ppq            = 0.;
    totalSamples    = 0;
    totalSteps      = 0;
    samplesPerStep  = 0;
    leftOverSamples = 0;

    for (int ch = 0; ch < 16; ch++)
    {
        for (int n = 0; n < 128; n++)
        {
            lastcc[n][ch] = -1;
        }
        expectingCC[ch] = false;
    }

    return PizMidi::init();
}

void MidiCCStepper::preProcess()
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
    }

    samplesPerStep = roundToInt(60.f * sampleRate / (beatdiv * _bpm));
    samplesPerBeat = roundToInt(60.f * sampleRate / _bpm);

    _ppq += param[kBeatOffset] / beatdiv;
    totalSteps = (int) (_ppq * beatdiv);

    _cleanMidiOutBuffers();
}

void MidiCCStepper::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const int lowcc    = FLOAT_TO_MIDI(param[kLowCC]);
    const int hicc     = FLOAT_TO_MIDI(param[kHighCC]);
    const int ccoffset = roundToInt(param[kCCOffset] * 254.f) - 127;
    bool von           = param[kValuePower] >= 0.5f;
    if (roundToInt(param[kValueQ] * 63.f))
    {
        von = false; //128 steps
    }
    const float vq       = (float) (roundToInt(param[kValueQ] * 62.f) + 1); //number of steps (2..64)
    const float invvq    = 127.f / vq;
    const bool ton       = param[kTimePower] >= 0.5f;
    const bool sync      = param[kSync] >= 0.5f;
    const bool sendAfter = param[kSendAfter] >= 0.5f;

    //const float stepsPerBlock = (float)sampleFrames/(float)samplesPerStep;
    int stepsInThisBlock = 0;
    int stepsSent        = 0;

    int s = leftOverSamples;
    if (sync)
    {
        leftOverSamples = roundToInt(((double) totalSteps + 1.0 - _ppq * beatdiv) * (double) samplesPerStep);
        if (isplaying)
        {
            s = leftOverSamples;
            while (s < sampleFrames)
            {
                ++stepsInThisBlock;
                s += samplesPerStep;
            }
        }
    }
    //else {
    //    while (s<sampleFrames) {
    //          ++stepsInThisBlock;
    //          s+=samplesPerStep;
    //    }
    //}

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        int status  = tomod.midiData[0] & 0xf0; // scraping  channel
        int channel = tomod.midiData[0] & 0x0f; // isolating channel
        int data1   = tomod.midiData[1] & 0x7f;
        int data2   = tomod.midiData[2] & 0x7f;

        //reset these for every event
        bool discard = false;
        int outcc    = (data1 + ccoffset) & 0x7f;
        int ch       = FLOAT_TO_CHANNEL(param[kChannel]);
        if (ch == -1)
        {
            ch = channel; //any channel
        }

        if (channel == ch)
        {
            if (status == MIDI_CONTROLCHANGE)
            {
                if (data1 >= lowcc && data1 <= hicc)
                {
                    if (von)
                    {
                        tomod.midiData[2] = roundToInt(invvq * roundToInt((float) data2 * MIDISCALER * vq));
                        if (tomod.midiData[2] == lastcc[data1][ch])
                        {
                            discard = true;
                        }
                    }
                    if (ton)
                    {
                        if (sync)
                        {
                            //sample and hold
                            if (stepsInThisBlock > stepsSent)
                            {
                                //this event is after the step, so send the previous step if any
                                if (lastcc[data1][ch] > 0)
                                {
                                    VstMidiEvent cc;
                                    cc.midiData[0] = MIDI_CONTROLCHANGE | ch;
                                    cc.midiData[1] = outcc;
                                    cc.midiData[2] = lastcc[data1][ch];
                                    cc.deltaFrames = leftOverSamples + (samplesPerStep * stepsSent);
                                    outputs[0].push_back(cc);
                                }
                                ++stepsSent;
                                //then make sure the new value will be sent next time
                                expectingCC[channel] = true;
                            }
                            if (sendAfter)
                            {
                                expectingCC[channel] = true;
                            }
                            discard           = true;
                            lastcc[data1][ch] = tomod.midiData[2];
                        }
                        else
                        {
                            if (samplesPerStep < tomod.deltaFrames + totalSamples - lastTime)
                            {
                                //ok, since we got one here...
                                lastTime          = tomod.deltaFrames + totalSamples;
                                discard           = false;
                                lastcc[data1][ch] = -1;
                            }
                            else
                            {
                                discard           = true;
                                expectingCC[ch]   = true;
                                lastcc[data1][ch] = tomod.midiData[2];
                            }
                        }
                    }
                    else
                    {
                        lastcc[data1][ch] = tomod.midiData[2];
                    }
                    tomod.midiData[1] = outcc;
                }
            }
            else if (status == MIDI_NOTEON || status == MIDI_NOTEOFF)
            {
                discard = true;
            }
        }
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    }

    if (sync)
    {
        if (stepsInThisBlock > stepsSent)
        {
            //leftover ccs to send
            for (int ch = 0; ch < 16; ch++)
            {
                if (expectingCC[ch])
                {
                    for (int n = 0; n < 128; n++)
                    {
                        if (lastcc[n][ch] > 0)
                        {
                            int sample = leftOverSamples + (samplesPerStep * stepsSent);
                            if (sample >= sampleFrames)
                            {
                                sample = sampleFrames - 1;
                            }
                            VstMidiEvent cc;
                            cc.midiData[0] = MIDI_CONTROLCHANGE | ch;
                            cc.midiData[1] = (n + ccoffset) & 0x7f;
                            cc.midiData[2] = lastcc[n][ch] & 0x7f;
                            cc.deltaFrames = sample;
                            if (sendAfter)
                            {
                                outputs[0].push_back(cc);
                            }
                            lastcc[n][ch] = -1;
                        }
                    }
                    expectingCC[ch] = false;
                }
            }
        }
    }
    else
    {
        VstInt32 delta = (VstInt32) (lastTime + samplesPerStep - totalSamples);
        if (delta < sampleFrames)
        {
            //leftover ccs to send
            for (int ch = 0; ch < 16; ch++)
            {
                if (expectingCC[ch])
                {
                    for (int n = 0; n < 128; n++)
                    {
                        if (lastcc[n][ch] > 0)
                        {
                            VstMidiEvent cc;
                            cc.midiData[0] = MIDI_CONTROLCHANGE | ch;
                            cc.midiData[1] = (n + ccoffset) & 0x7f;
                            cc.midiData[2] = lastcc[n][ch] & 0x7f;
                            cc.deltaFrames = delta;
                            if (sendAfter)
                            {
                                outputs[0].push_back(cc);
                                lastTime = delta + totalSamples;
                            }
                            lastcc[n][ch] = -1;
                        }
                    }
                    expectingCC[ch] = false;
                }
            }
        }
    }
    leftOverSamples = s - sampleFrames;
    totalSamples += sampleFrames;

    if (wasplaying && ! isplaying)
    {
        //just stopped
    }
    wasplaying = isplaying;
}
