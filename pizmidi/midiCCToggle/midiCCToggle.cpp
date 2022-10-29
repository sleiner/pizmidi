#include "midiCCToggle.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiCCToggle(audioMaster);
}

//-----------------------------------------------------------------------------
MidiCCToggle::MidiCCToggle(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams),
      programs(0)
{
    programs = new CFxBank(numPrograms, numParams);
    if (! readDefaultBank(PLUG_NAME, programs, PLUG_IDENT))
    {
        // built-in programs
        for (int i = 0; i < numPrograms; i++)
        {
            programs->SetProgramName(i, "CC Toggle");
            // default values
            programs->SetProgParm(i, kParam01, 1.0f);
            programs->SetProgParm(i, kParam02, 0.f);
            programs->SetProgParm(i, kParam03, MIDI_TO_FLOAT(127));
            programs->SetProgParm(i, kParam05, MIDI_TO_FLOAT(127));
            programs->SetProgParm(i, kParam06, MIDI_TO_FLOAT(0));
            programs->SetProgParm(i, kParam07, CHANNEL_TO_FLOAT016(0));
        }
    }

    for (int ch = 0; ch < 16; ch++)
    {
        for (int i = 0; i < 128; i++)
        {
            held_notes[i][ch] = false;
        }
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiCCToggle::~MidiCCToggle()
{
    if (programs)
    {
        delete programs;
    }
}

void MidiCCToggle::setProgram(VstInt32 program)
{
    for (int i = 0; i < numParams; i++)
    {
        setParameter(i, programs->GetProgParm(program, i));
    }
    curProgram = program;
    updateDisplay();
}

void MidiCCToggle::setProgramName(char* name)
{
    programs->SetProgramName(curProgram, name);
    updateDisplay();
}

void MidiCCToggle::getProgramName(char* name)
{
    strcpy(name, programs->GetProgramName(curProgram));
}

bool MidiCCToggle::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < numPrograms)
    {
        strcpy(text, programs->GetProgramName(index));
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiCCToggle::setParameter(VstInt32 index, float value)
{
    switch (index)
    {
        case kParam01:
            fParam01 = value;
            break;
        case kParam02:
            if (value > fParam03)
            {
                setParameterAutomated(kParam03, value);
            }
            fParam02 = value;
            break;
        case kParam03:
            if (value < fParam02)
            {
                setParameterAutomated(kParam02, value);
            }
            fParam03 = value;
            break;
        case kParam05:
            fParam05 = value;
            break;
        case kParam06:
            fParam06 = value;
            break;
        case kParam07:
            fParam07 = value;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiCCToggle::getParameter(VstInt32 index)
{
    float v = 0;

    switch (index)
    {
        case kParam01:
            v = fParam01;
            break;
        case kParam02:
            v = fParam02;
            break;
        case kParam03:
            v = fParam03;
            break;
        case kParam05:
            v = fParam05;
            break;
        case kParam06:
            v = fParam06;
            break;
        case kParam07:
            v = fParam07;
            break;
        default:
            break;
    }
    return v;
}

//-----------------------------------------------------------------------------------------
void MidiCCToggle::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kParam01:
            strcpy(label, "Power");
            break;
        case kParam02:
            strcpy(label, "Low CC");
            break;
        case kParam03:
            strcpy(label, "High CC");
            break;
        case kParam05:
            strcpy(label, "On Value");
            break;
        case kParam06:
            strcpy(label, "Off Value");
            break;
        case kParam07:
            strcpy(label, "Channel");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiCCToggle::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kParam01:
            if (fParam01 < 0.5)
            {
                strcpy(text, "Off");
            }
            else
            {
                strcpy(text, "On");
            }
            break;
        case kParam02:
            sprintf(text, "%d", FLOAT_TO_MIDI(fParam02));
            break;
        case kParam03:
            sprintf(text, "%d", FLOAT_TO_MIDI(fParam03));
            break;
        case kParam05:
            sprintf(text, "%d", FLOAT_TO_MIDI(fParam05));
            break;
        case kParam06:
            sprintf(text, "%d", FLOAT_TO_MIDI(fParam06));
            break;
        case kParam07:
            if (FLOAT_TO_CHANNEL016(fParam07) == 0)
            {
                strcpy(text, "All");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL016(fParam07));
            }
            break;
        default:
            break;
    }
}

void MidiCCToggle::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        short status  = tomod.midiData[0] & 0xf0; // scraping  channel
        short channel = tomod.midiData[0] & 0x0f; // isolating channel
        short data1   = tomod.midiData[1] & 0x7f;
        short data2   = tomod.midiData[2] & 0x7f;

        bool on          = fParam01 >= 0.5f;
        short lownote    = FLOAT_TO_MIDI(fParam02);
        short highnote   = FLOAT_TO_MIDI(fParam03);
        short onvalue    = FLOAT_TO_MIDI(fParam05);
        short offvalue   = FLOAT_TO_MIDI(fParam06);
        short outchannel = FLOAT_TO_CHANNEL016(fParam07) - 1;

        if (outchannel == -1)
        {
            outchannel = channel;
        }
        bool discard = false;

        if (on && channel == outchannel)
        {
            if (status == MIDI_CONTROLCHANGE && data2 > 63)
            {
                if (data1 >= lownote && data1 <= highnote)
                {
                    if (held_notes[data1][channel])
                    {
                        tomod.midiData[0]          = MIDI_CONTROLCHANGE | outchannel;
                        tomod.midiData[2]          = (char) offvalue;
                        held_notes[data1][channel] = false;
                    }
                    else
                    {
                        tomod.midiData[2]          = (char) onvalue;
                        held_notes[data1][channel] = true;
                    }
                }
            }
            else if (status == MIDI_CONTROLCHANGE && data2 < 64)
            {
                discard = true;
            }
        }
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    }
}
