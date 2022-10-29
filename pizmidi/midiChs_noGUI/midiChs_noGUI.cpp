#include "midiChs_noGUI.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiChs(audioMaster);
}

void MidiChs::Reset()
{
    setParameterAutomated(kChannel1, CHANNEL_TO_FLOAT(0));
    setParameterAutomated(kChannel2, CHANNEL_TO_FLOAT(1));
    setParameterAutomated(kChannel3, CHANNEL_TO_FLOAT(2));
    setParameterAutomated(kChannel4, CHANNEL_TO_FLOAT(3));
    setParameterAutomated(kChannel5, CHANNEL_TO_FLOAT(4));
    setParameterAutomated(kChannel6, CHANNEL_TO_FLOAT(5));
    setParameterAutomated(kChannel7, CHANNEL_TO_FLOAT(6));
    setParameterAutomated(kChannel8, CHANNEL_TO_FLOAT(7));
    setParameterAutomated(kChannel9, CHANNEL_TO_FLOAT(8));
    setParameterAutomated(kChannel10, CHANNEL_TO_FLOAT(9));
    setParameterAutomated(kChannel11, CHANNEL_TO_FLOAT(10));
    setParameterAutomated(kChannel12, CHANNEL_TO_FLOAT(11));
    setParameterAutomated(kChannel13, CHANNEL_TO_FLOAT(12));
    setParameterAutomated(kChannel14, CHANNEL_TO_FLOAT(13));
    setParameterAutomated(kChannel15, CHANNEL_TO_FLOAT(14));
    setParameterAutomated(kChannel16, CHANNEL_TO_FLOAT(15));
    setParameterAutomated(kClear, 0.4f);
    setParameterAutomated(kReset, 0.4f);
    updateDisplay();
}

void MidiChs::Clear()
{
    setParameterAutomated(kChannel1, 0);
    setParameterAutomated(kChannel2, 0);
    setParameterAutomated(kChannel3, 0);
    setParameterAutomated(kChannel4, 0);
    setParameterAutomated(kChannel5, 0);
    setParameterAutomated(kChannel6, 0);
    setParameterAutomated(kChannel7, 0);
    setParameterAutomated(kChannel8, 0);
    setParameterAutomated(kChannel9, 0);
    setParameterAutomated(kChannel10, 0);
    setParameterAutomated(kChannel11, 0);
    setParameterAutomated(kChannel12, 0);
    setParameterAutomated(kChannel13, 0);
    setParameterAutomated(kChannel14, 0);
    setParameterAutomated(kChannel15, 0);
    setParameterAutomated(kChannel16, 0);
    setParameterAutomated(kReset, 0.4f);
    setParameterAutomated(kClear, 0.4f);
    updateDisplay();
}

MidiChsProgram::MidiChsProgram()
{
    // default Program Values
    for (int i = 0; i < 16; i++)
    {
        param[i] = CHANNEL_TO_FLOAT(i);
    }
    param[kReset] = 0.4f;
    param[kClear] = 0.4f;
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiChs::MidiChs(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams),
      programs(0)
{
    programs = new MidiChsProgram[numPrograms];

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
                switch (i)
                {
                    case 0:
                        sprintf(programs[i].name, "MIDI chs");
                        break;
                    case 1:
                        for (int j = 0; j < 16; j++)
                            programs[i].param[j] = 0;
                        sprintf(programs[i].name, "--");
                }
            }
        }
        setProgram(0);
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiChs::~MidiChs()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiChs::setProgram(VstInt32 program)
{
    MidiChsProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
        setParameter(i, ap->param[i]);
}

//------------------------------------------------------------------------
void MidiChs::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiChs::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiChs::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiChs::setParameter(VstInt32 index, float value)
{
    MidiChsProgram* ap = &programs[curProgram];
    if (index < kNumParams)
    {
        param[index] = ap->param[index] = value;
    }
}

//-----------------------------------------------------------------------------------------
float MidiChs::getParameter(VstInt32 index)
{
    if (index < kNumParams)
        return param[index];
    return 0;
}

//-----------------------------------------------------------------------------------------
void MidiChs::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kChannel1:
            strcpy(label, "1 ->");
            break;
        case kChannel2:
            strcpy(label, "2 ->");
            break;
        case kChannel3:
            strcpy(label, "3 ->");
            break;
        case kChannel4:
            strcpy(label, "4 ->");
            break;
        case kChannel5:
            strcpy(label, "5 ->");
            break;
        case kChannel6:
            strcpy(label, "6 ->");
            break;
        case kChannel7:
            strcpy(label, "7 ->");
            break;
        case kChannel8:
            strcpy(label, "8 ->");
            break;
        case kChannel9:
            strcpy(label, "9 ->");
            break;
        case kChannel10:
            strcpy(label, "10 ->");
            break;
        case kChannel11:
            strcpy(label, "11 ->");
            break;
        case kChannel12:
            strcpy(label, "12 ->");
            break;
        case kChannel13:
            strcpy(label, "13 ->");
            break;
        case kChannel14:
            strcpy(label, "14 ->");
            break;
        case kChannel15:
            strcpy(label, "15 ->");
            break;
        case kChannel16:
            strcpy(label, "16 ->");
            break;
        case kReset:
            strcpy(label, "Reset");
            break;
        case kClear:
            strcpy(label, "Clear");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiChs::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kChannel1:
        case kChannel2:
        case kChannel3:
        case kChannel4:
        case kChannel5:
        case kChannel6:
        case kChannel7:
        case kChannel8:
        case kChannel9:
        case kChannel10:
        case kChannel11:
        case kChannel12:
        case kChannel13:
        case kChannel14:
        case kChannel15:
        case kChannel16:
            if (FLOAT_TO_CHANNEL(param[index]) < 0)
                strcpy(text, "--");
            else
                sprintf(text, "%d", 1 + FLOAT_TO_CHANNEL(param[index]));
            break;
        case kReset:
            if (param[index] < 1.0f)
                strcpy(text, "Reset");
            else
            {
                strcpy(text, "!");
                Reset();
            }
            break;
        case kClear:
            if (param[index] < 1.0f)
                strcpy(text, "Clear");
            else
            {
                strcpy(text, "!");
                Clear();
            }
            break;
        default:
            break;
    }
}

void MidiChs::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];
        const int status   = tomod.midiData[0] & 0xf0; // scraping  channel
        const int channel  = tomod.midiData[0] & 0x0f; // isolating channel (1-16)
        //const int data1      = tomod.midiData[1] & 0x7f;
        //const int data2	     = tomod.midiData[2] & 0x7f;

        bool discard = false;

        if (status < 0xF0)
        {
            for (int ch = 0; ch < 16; ch++)
            {
                if (channel == ch)
                {
                    const int newch = FLOAT_TO_CHANNEL(param[ch]);
                    if (newch > -1)
                        tomod.midiData[0] = status | newch;
                    else
                        discard = true;
                }
            }
        }
        if (! discard)
            outputs[0].push_back(tomod);
    }
}
