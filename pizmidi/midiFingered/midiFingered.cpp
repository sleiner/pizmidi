#include "midiFingered.hpp"

#include "../common/MIDI.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiFingered(audioMaster);
}

//-----------------------------------------------------------------------------
MidiFingered::MidiFingered(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, 1, kNumParams)
{
    CFxBank* defaultBank = new CFxBank(kNumPrograms, kNumParams);
    if (readDefaultBank(PLUG_NAME, defaultBank))
    {
        if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
        {
            for (int p = 0; p < kNumParams; p++)
            {
                param[p] = defaultBank->GetProgParm(0, p);
            }
            strcpy(_programName, defaultBank->GetProgramName(0));
        }
    }
    else
    {
        // built-in programs
        strcpy(_programName, "Fingered Portamento"); // default program name
    }

    // default values
    param[kPower]   = 1.0f;
    param[kChannel] = 0.0f;

    for (int ch = 0; ch < 16; ch++)
    {
        for (int i = 0; i < 128; i++)
        {
            held_notes[i][ch] = false;
        }
        voices[ch] = 0;
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiFingered::~MidiFingered()
{
}

//------------------------------------------------------------------------
void MidiFingered::getProgramName(char* name)
{
    vst_strncpy(name, _programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool MidiFingered::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < numPrograms)
    {
        vst_strncpy(text, _programName, kVstMaxProgNameLen);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiFingered::setParameter(VstInt32 index, float value)
{
    param[index] = value;
}

//-----------------------------------------------------------------------------------------
float MidiFingered::getParameter(VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiFingered::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kPower:
            strcpy(label, "Power");
            break;
        case kChannel:
            strcpy(label, "Channel");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiFingered::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kPower:
            if (param[index] < 0.5)
            {
                strcpy(text, "Off");
            }
            else
            {
                strcpy(text, "On");
            }
            break;
        case kChannel:
            if (FLOAT_TO_CHANNEL016(param[index]) == 0)
            {
                strcpy(text, "All");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL016(param[index]));
            }
            break;
        default:
            break;
    }
}

void MidiFingered::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
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

        //make 0-velocity notes look like "real" noteoffs for simplicity
        if (status == MIDI_NOTEON && data2 == 0)
        {
            status = MIDI_NOTEOFF;
        }

        short outchannel = FLOAT_TO_CHANNEL016(param[kChannel]) - 1;
        if (outchannel == -1)
        {
            outchannel = channel;
        }
        bool discard = false;

        bool on = param[kPower] >= 0.5f;
        if (channel != outchannel)
        {
            on = false;
        }

        if (status == MIDI_NOTEOFF)
        {
            //if (held_notes[data1][channel]) {
            held_notes[data1][channel] = false;
            --voices[channel];
            //}
            if (voices[channel] <= 0)
            {
                voices[channel]       = 0;
                VstMidiEvent portaOff = inputs[0][i];
                portaOff.midiData[0]  = MIDI_CONTROLCHANGE | channel;
                portaOff.midiData[1]  = 65;
                portaOff.midiData[2]  = 0;
                if (on)
                {
                    outputs[0].push_back(portaOff);
                }
            }
        }
        else if (status == MIDI_NOTEON)
        {
            held_notes[data1][channel] = true;
            ++voices[channel];
            if (voices[channel] == 1)
            {
                VstMidiEvent portaOn = inputs[0][i];
                portaOn.midiData[0]  = MIDI_CONTROLCHANGE | channel;
                portaOn.midiData[1]  = 65;
                portaOn.midiData[2]  = 127;
                if (on)
                {
                    outputs[0].push_back(portaOn);
                }
            }
        }

        else if (status == MIDI_CONTROLCHANGE)
        {
            if (data1 == MIDI_ALL_NOTES_OFF)
            {
                for (int n = 0; n < 128; n++)
                {
                    if (held_notes[n][channel])
                    {
                        held_notes[n][channel] = false;
                    }
                }
                if (on)
                {
                    VstMidiEvent portaOff = inputs[0][i];
                    portaOff.midiData[0]  = MIDI_CONTROLCHANGE | channel;
                    portaOff.midiData[1]  = 65;
                    portaOff.midiData[2]  = 0;
                    outputs[0].push_back(portaOff);
                }
                voices[channel] = 0;
            }
            //let CC 65 control "Power":
            else if (data1 == MIDI_PORTAMENTO && on)
            {
                discard = true;
                if (data2 < 64)
                {
                    setParameter(kPower, 0.0f);
                }
                else
                {
                    setParameter(kPower, 1.0f);
                }
                updateDisplay();
            }
        }

        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    }
}
