#include "midiOverlappingNoteKiller.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiOverlappingNoteKiller(audioMaster);
}

MidiOverlappingNoteKiller::MidiOverlappingNoteKiller(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, 1, kNumParams)
{
    CFxBank* defaultBank = new CFxBank(numPrograms, numParams);
    if (readDefaultBank(PLUG_NAME, defaultBank))
    {
        if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
        {
            fParam01 = defaultBank->GetProgParm(0, 0);
            vst_strncpy(_programName, defaultBank->GetProgramName(0), kVstMaxProgNameLen);
        }
    }
    else
    {
        // built-in programs
        vst_strncpy(_programName, "OverlappingNoteKiller", kVstMaxProgNameLen); // default program name
        fParam01 = 1.0;
    }

    // default values
    for (int ch = 0; ch < 16; ch++)
    {
        for (int i = 0; i < 128; i++)
            held_notes[i][ch] = false;
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiOverlappingNoteKiller::~MidiOverlappingNoteKiller()
{
}

//-----------------------------------------------------------------------------------------
void MidiOverlappingNoteKiller::setParameter(VstInt32 index, float value)
{
    switch (index)
    {
        case kParam01:
            fParam01 = value;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiOverlappingNoteKiller::getParameter(VstInt32 index)
{
    float v = 0;

    switch (index)
    {
        case kParam01:
            v = fParam01;
            break;
        default:
            break;
    }
    return v;
}

//-----------------------------------------------------------------------------------------
void MidiOverlappingNoteKiller::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kParam01:
            strcpy(label, "Power");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiOverlappingNoteKiller::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kParam01:
            if (fParam01 < 0.5)
                strcpy(text, "Off");
            else
                strcpy(text, "On");
            break;
        default:
            break;
    }
}

void MidiOverlappingNoteKiller::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        int status  = tomod.midiData[0] & 0xf0; // scraping  channel
        int channel = tomod.midiData[0] & 0x0f; // isolating channel
        int data1   = tomod.midiData[1] & 0x7f;
        int data2   = tomod.midiData[2] & 0x7f;

        const bool on = fParam01 >= 0.5f;

        bool discard = false;

        if (status == MIDI_NOTEON && data2 == 0)
            status = MIDI_NOTEOFF;

        if (status == MIDI_NOTEON)
        {
            if (on && held_notes[data1][channel])
            {
                //send a noteoff first if the note is already playing
                VstMidiEvent kill = tomod;
                kill.midiData[0]  = MIDI_NOTEOFF | channel;
                kill.midiData[1]  = data1;
                kill.midiData[2]  = 0;
                outputs[0].push_back(kill);
            }
            held_notes[data1][channel] = true;
        }
        else if (status == MIDI_NOTEOFF)
        {
            if (on && ! held_notes[data1][channel])
            {
                //don't send the noteoff if the note isn't playing
                discard = true;
            }
            held_notes[data1][channel] = false;
        }
        if (! discard)
            outputs[0].push_back(tomod);
    }
}
