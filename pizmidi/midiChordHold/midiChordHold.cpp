#include "midiChordHold.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiChordHold(audioMaster);
}

//-----------------------------------------------------------------------------
MidiChordHold::MidiChordHold(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, 1, kNumParams)
{
    CFxBank* defaultBank = new CFxBank(numPrograms, numParams);
    if (readDefaultBank(PLUG_NAME, defaultBank))
    {
        if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
        {
            for (int i = 0; i < kNumParams; i++)
            {
                param[i] = defaultBank->GetProgParm(0, i);
            }
            strcpy(_programName, defaultBank->GetProgramName(0));
        }
    }
    else
    {
        // built-in programs
        strcpy(_programName, "Chord Holder"); // default program name
        param[kParam01] = 1.0f;
        param[kChannel] = 0.0f;
    }

    // default values
    for (int ch = 0; ch < 16; ch++)
    {
        for (int i = 0; i < 128; i++)
            held_notes[i][ch] = false;
    }

    holding     = false;
    waiting     = true;
    wasOn       = false;
    lastChannel = FLOAT_TO_CHANNEL015(param[kChannel]);

    init();
}

//-----------------------------------------------------------------------------------------
MidiChordHold::~MidiChordHold()
{
}

//-----------------------------------------------------------------------------------------
void MidiChordHold::setParameter(VstInt32 index, float value)
{
    if (index < kNumParams)
        param[index] = value;
}

//-----------------------------------------------------------------------------------------
float MidiChordHold::getParameter(VstInt32 index)
{
    if (index < kNumParams)
        return param[index];
    return 0;
}

//-----------------------------------------------------------------------------------------
void MidiChordHold::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kParam01:
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
void MidiChordHold::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kParam01:
            if (param[index] < 0.5)
                strcpy(text, "Off");
            else
                strcpy(text, "On");
            break;
        case kChannel:
            sprintf(text, "%d", FLOAT_TO_CHANNEL015(param[kChannel]) + 1);
            break;
        default:
            break;
    }
}

void MidiChordHold::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const int inputChannel = FLOAT_TO_CHANNEL015(param[kChannel]);
    const bool on          = param[kParam01] >= 0.5f;

    if ((! on && wasOn) || (inputChannel != lastChannel))
    {
        //turning power off, kill all notes
        if (holding)
        {
            for (int n = 0; n < 128; n++)
            {
                for (int c = 0; c < 16; c++)
                {
                    if (held_notes[n][c])
                    {
                        VstMidiEvent kill;
                        kill.midiData[0] = MIDI_NOTEOFF | c;
                        kill.midiData[1] = n;
                        kill.midiData[2] = 0;
                        kill.deltaFrames = 0;
                        outputs[0].push_back(kill);
                        held_notes[n][c] = false;
                    }
                }
            }
            holding = false;
            waiting = true;
        }
    }

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        int status  = tomod.midiData[0] & 0xf0; // scraping  channel
        int channel = tomod.midiData[0] & 0x0f; // isolating channel
        int data1   = tomod.midiData[1] & 0x7f;
        int data2   = tomod.midiData[2] & 0x7f;

        bool discard = false;

        if (status == MIDI_NOTEON && data2 == 0)
            status = MIDI_NOTEOFF;

        if (status == MIDI_NOTEON)
        {
            if (on && channel == inputChannel)
            {
                if (holding)
                {
                    //start next held chord, kill existing notes first
                    for (int n = 0; n < 128; n++)
                    {
                        if (held_notes[n][channel])
                        {
                            VstMidiEvent kill = tomod;
                            kill.midiData[0]  = MIDI_NOTEOFF | channel;
                            kill.midiData[1]  = n;
                            kill.midiData[2]  = 0;
                            outputs[0].push_back(kill);
                            held_notes[n][channel] = false;
                        }
                    }
                    holding = false;
                    waiting = true;
                    dbg("next chord, wait for noteoff");
                }
            }
            held_notes[data1][channel] = true;
        }
        else if (status == MIDI_NOTEOFF)
        {
            if (on && channel == inputChannel)
            {
                if (holding)
                {
                    //ignore noteoff
                    discard = true;
                    dbg("holding");
                }
                else if (waiting)
                {
                    holding = true;
                    waiting = false;
                    discard = true;
                    dbg("waiting (noteoff)");
                }
                else
                {
                    held_notes[data1][channel] = false;
                    dbg("not holding or waiting");
                }
            }
            else
                held_notes[data1][channel] = false;
        }
        else if (status == MIDI_CONTROLCHANGE)
        {
            if (data1 == MIDI_ALL_NOTES_OFF)
            {
                for (int n = 0; n < 128; n++)
                {
                    for (int c = 0; c < 16; c++)
                    {
                        if (held_notes[n][c])
                        {
                            VstMidiEvent kill;
                            kill.midiData[0] = MIDI_NOTEOFF | c;
                            kill.midiData[1] = n;
                            kill.midiData[2] = 0;
                            kill.deltaFrames = 0;
                            outputs[0].push_back(kill);
                            held_notes[n][c] = false;
                        }
                    }
                }
                holding = false;
                waiting = true;
            }
        }
        if (! discard)
            outputs[0].push_back(tomod);
    }
    wasOn = on;
}
