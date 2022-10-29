#include "midiNoteToggle.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiNoteToggle(audioMaster);
}

//-----------------------------------------------------------------------------
MidiNoteToggle::MidiNoteToggle(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new CFxBank(numPrograms, numParams);
    if (! readDefaultBank(PLUG_NAME, programs, PLUG_IDENT))
    {
        // built-in programs
        for (int i = 0; i < numPrograms; i++)
        {
            programs->SetProgramName(i, "Note Toggle");
            // default values
            programs->SetProgParm(i, kPower, 1.f);
            programs->SetProgParm(i, kLowNote, 0.f);
            programs->SetProgParm(i, kHighNote, MIDI_TO_FLOAT(127));
            programs->SetProgParm(i, kChannel, CHANNEL_TO_FLOAT016(0));
        }
    }

    for (int i = 0; i < 128; i++)
        held_notes[i] = false;

    init();
}

//-----------------------------------------------------------------------------------------
MidiNoteToggle::~MidiNoteToggle()
{
    if (programs)
        delete programs;
}

void MidiNoteToggle::setProgram(VstInt32 program)
{
    for (int i = 0; i < numParams; i++)
    {
        setParameter(i, programs->GetProgParm(program, i));
    }
    curProgram = program;
    updateDisplay();
}

void MidiNoteToggle::setProgramName(char* name)
{
    programs->SetProgramName(curProgram, name);
    updateDisplay();
}

void MidiNoteToggle::getProgramName(char* name)
{
    strcpy(name, programs->GetProgramName(curProgram));
}

bool MidiNoteToggle::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < numPrograms)
    {
        strcpy(text, programs->GetProgramName(index));
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiNoteToggle::setParameter(VstInt32 index, float value)
{
    if (index < numParams)
    {
        switch (index)
        {
            case kPower:
                param[kPower] = value;
                break;
            case kLowNote:
                if (value > param[kHighNote])
                    setParameterAutomated(kHighNote, value);
                param[kLowNote] = value;
                break;
            case kHighNote:
                if (value < param[kLowNote])
                    setParameterAutomated(kLowNote, value);
                param[kHighNote] = value;
                break;
            case kChannel:
                param[kChannel] = value;
                break;
            default:
                break;
        }
        for (int i = 0; i < numPrograms; i++)
            programs->SetProgParm(i, index, value);
    }
}

//-----------------------------------------------------------------------------------------
float MidiNoteToggle::getParameter(VstInt32 index)
{
    if (index < numParams)
        return param[index];
    return 0;
}

//-----------------------------------------------------------------------------------------
void MidiNoteToggle::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kPower:
            strcpy(label, "Power");
            break;
        case kLowNote:
            strcpy(label, "Low Note");
            break;
        case kHighNote:
            strcpy(label, "High Note");
            break;
        case kChannel:
            strcpy(label, "Channel");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiNoteToggle::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kPower:
            if (param[index] < 0.5f)
                strcpy(text, "Off");
            else
                strcpy(text, "On");
            break;
        case kLowNote:
            sprintf(text, "%d (%s)", FLOAT_TO_MIDI(param[index]), getNoteName(FLOAT_TO_MIDI(param[index]), bottomOctave));
            break;
        case kHighNote:
            sprintf(text, "%d (%s)", FLOAT_TO_MIDI(param[index]), getNoteName(FLOAT_TO_MIDI(param[index]), bottomOctave));
            break;
        case kChannel:
            if (FLOAT_TO_CHANNEL016(param[index]) == 0)
                strcpy(text, "All");
            else
                sprintf(text, "%d", FLOAT_TO_CHANNEL016(param[index]));
            break;
        default:
            break;
    }
}

void MidiNoteToggle::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        const int status  = tomod.midiData[0] & 0xf0; // scraping  channel
        const int channel = tomod.midiData[0] & 0x0f; // isolating channel
        const int data1   = tomod.midiData[1] & 0x7f;
        const int data2   = tomod.midiData[2] & 0x7f;

        const bool on        = param[kPower] >= 0.5f;
        const int lownote    = FLOAT_TO_MIDI(param[kLowNote]);
        const int highnote   = FLOAT_TO_MIDI(param[kHighNote]);
        const int outchannel = FLOAT_TO_CHANNEL(param[kChannel]) == -1 ? channel : FLOAT_TO_CHANNEL(param[kChannel]);

        bool discard = false;

        if (on && channel == outchannel)
        {
            if (status == MIDI_NOTEON && data2 > 0)
            {
                if (data1 >= lownote && data1 <= highnote)
                {
                    if (held_notes[data1])
                    {
                        tomod.midiData[0] = MIDI_NOTEOFF | outchannel;
                        held_notes[data1] = false;
                    }
                    else
                        held_notes[data1] = true;
                }
            }
            else if ((status == MIDI_NOTEOFF) || (status == MIDI_NOTEON && data2 == 0))
            {
                if (data1 >= lownote && data1 <= highnote)
                    discard = true;
            }
            //else if (status==MIDI_PROGRAMCHANGE)
            //{
            //	setProgram(data1);
            //}
        }
        if (! discard)
            outputs[0].push_back(tomod);
    }
}
