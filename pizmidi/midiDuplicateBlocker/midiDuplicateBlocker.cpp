#include "midiDuplicateBlocker.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiDuplicateBlocker(audioMaster);
}

MidiDuplicateBlockerProgram::MidiDuplicateBlockerProgram()
{
    // default program values
    param[kPower]   = 0.2f;                   // CCs
    param[kChannel] = CHANNEL_TO_FLOAT016(0); // any channel
    param[kTimer]   = 0.0f;

    // default program name
    vst_strncpy(name, "default", kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------
MidiDuplicateBlocker::MidiDuplicateBlocker(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    for (int ch = 0; ch < 16; ch++)
    {
        lastNote[ch]     = 255;
        lastProgCh[ch]   = 255;
        lastChanAft[ch]  = 255;
        lastPB[ch]       = 0xFFFF;
        Notetimer[ch]    = 0;
        ProgChtimer[ch]  = 0;
        ChanAfttimer[ch] = 0;
        PBtimer[ch]      = 0;
        for (int i = 0; i < 128; i++)
        {
            lastCC[ch][i]       = 255;
            lastPolyAft[ch][i]  = 255;
            CCtimer[ch][i]      = 0;
            PolyAfttimer[ch][i] = 0;
            playing[ch][i]      = false;
        }
    }

    time      = 1000;
    totalTime = 0;

    programs = new MidiDuplicateBlockerProgram[numPrograms];
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

    init();
}

//-----------------------------------------------------------------------------------------
MidiDuplicateBlocker::~MidiDuplicateBlocker()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiDuplicateBlocker::setProgram(VstInt32 program)
{
    MidiDuplicateBlockerProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter(i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiDuplicateBlocker::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiDuplicateBlocker::getProgramName(char* name)
{
    vst_strncpy(name, programs[curProgram].name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool MidiDuplicateBlocker::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < numPrograms)
    {
        vst_strncpy(text, programs[index].name, kVstMaxProgNameLen);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiDuplicateBlocker::setSampleRate(float sampleRateIn)
{
    PizMidi::setSampleRate(sampleRateIn);
    time = roundToInt(param[kTimer] * 2.f * sampleRate); //max 2 seconds
}

//-----------------------------------------------------------------------------------------
void MidiDuplicateBlocker::setParameter(VstInt32 index, float value)
{
    MidiDuplicateBlockerProgram* ap = &programs[curProgram];
    param[index] = ap->param[index] = value;
    if (index == kPower)
    {
        if (value == 0.f)
        {
            mode = off;
        }
        else if (value < 0.15f)
        {
            mode = cc;
        }
        else if (value < 0.3f)
        {
            mode = note;
        }
        else if (value < 0.45f)
        {
            mode = pb;
        }
        else if (value < 0.6f)
        {
            mode = pc;
        }
        else if (value < 0.75f)
        {
            mode = cp;
        }
        else if (value < 0.9f)
        {
            mode = aft;
        }
        else
        {
            mode = all;
        }
    }
    else if (index == kTimer)
    {
        time = roundToInt(value * 2.f * sampleRate); //max 2 seconds
    }
}

//-----------------------------------------------------------------------------------------
float MidiDuplicateBlocker::getParameter(VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiDuplicateBlocker::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kPower:
            strcpy(label, "Mode");
            break;
        case kChannel:
            strcpy(label, "Channel");
            break;
        case kTimer:
            strcpy(label, "Timer");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiDuplicateBlocker::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kChannel:
            if (FLOAT_TO_CHANNEL016(param[index]) < 1)
            {
                vst_strncpy(text, "All", kVstMaxParamStrLen);
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL016(param[index]));
            }
            break;
        case kPower:
            switch (mode)
            {
                case off:
                    strcpy(text, "Off");
                    break;
                case cc:
                    strcpy(text, "CCs");
                    break;
                case note:
                    strcpy(text, "Notes");
                    break;
                case pb:
                    strcpy(text, "PitchBend");
                    break;
                case pc:
                    strcpy(text, "ProgChange");
                    break;
                case cp:
                    strcpy(text, "ChanPressure");
                    break;
                case aft:
                    strcpy(text, "PolyAftertouch");
                    break;
                case all:
                    strcpy(text, "All");
                    break;
            }
            break;
        case kTimer:
            if (time == 0)
            {
                strcpy(text, "Off");
            }
            else
            {
                sprintf(text, "%f sec", (float) time / sampleRate);
            }
            break;
        default:
            break;
    }
}

void MidiDuplicateBlocker::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const short listenchannel = FLOAT_TO_CHANNEL016(param[kChannel]);

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        const short status   = tomod.midiData[0] & 0xf0;       // scraping  channel
        const short channel  = (tomod.midiData[0] & 0x0f) + 1; // isolating channel (1-16)
        const short data1    = tomod.midiData[1] & 0x7f;
        const short data2    = tomod.midiData[2] & 0x7f;
        const VstInt32 delta = tomod.deltaFrames;

        bool discard = false;

        //only look at the selected channel
        if (channel == listenchannel || listenchannel == 0)
        {
            if (status == MIDI_CONTROLCHANGE)
            {
                if (lastCC[channel - 1][data1] == data2 && (mode == cc || mode == all))
                {
                    if (time == 0 || (totalTime + delta < CCtimer[channel - 1][data1]))
                    {
                        discard = true;
                    }
                    else
                    {
                        CCtimer[channel - 1][data1] = totalTime + delta + time;
                    }
                }
                lastCC[channel - 1][data1] = data2;
            }
            else if (status == MIDI_NOTEON && data2 > 0)
            {
                if (lastNote[channel - 1] == data1 && (mode == note || mode == all))
                {
                    if (time == 0 || (totalTime + delta < Notetimer[channel - 1]))
                    {
                        discard = true;
                    }
                    else
                    {
                        Notetimer[channel - 1]  = totalTime + delta + time;
                        playing[channel][data1] = true;
                    }
                }
                else
                {
                    playing[channel][data1] = true;
                }
                lastNote[channel - 1] = data1;
            }
            else if (status == MIDI_NOTEOFF || (status == MIDI_NOTEON && data2 == 0))
            {
                if (mode == note || mode == all)
                {
                    if (playing[channel][data1])
                    {
                        playing[channel][data1] = false;
                    }
                    else
                    {
                        discard = true;
                    }
                }
            }
            else if (status == MIDI_PROGRAMCHANGE)
            {
                if (lastProgCh[channel - 1] == data1 && (mode == pc || mode == all))
                {
                    discard = true;
                }
                lastProgCh[channel - 1] = data1;
            }
            else if (status == MIDI_CHANNELPRESSURE)
            {
                if (lastChanAft[channel - 1] == data1 && (mode == cp || mode == all))
                {
                    discard = true;
                }
                lastChanAft[channel - 1] = data1;
            }
            else if (status == MIDI_POLYKEYPRESSURE)
            {
                if (lastPolyAft[channel - 1][data1] == data2 && (mode == aft || mode == all))
                {
                    discard = true;
                }
                lastPolyAft[channel - 1][data1] = data2;
            }
            else if (status == MIDI_PITCHBEND)
            {
                if (lastPB[channel - 1] == (data1 | (data2 << 7)) && (mode == pb || mode == all))
                {
                    discard = true;
                }
                lastPB[channel - 1] = (data1 | (data2 << 7));
            }
        }
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    }

    totalTime += sampleFrames;
}
