#include "midiSostenuto.hpp"

MidiSostenutoProgram::MidiSostenutoProgram()
{
    // default Program Values
    fParam01 = MIDI_TO_FLOAT (66.1);
    fParam02 = MIDI_TO_FLOAT (0);
    fParam03 = MIDI_TO_FLOAT (127);
    fParam04 = 0.0f;
    fParam05 = CHANNEL_TO_FLOAT015 (0);
    strcpy (name, "sostenuto");
}

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiSostenuto (audioMaster);
}

//-----------------------------------------------------------------------------
MidiSostenuto::MidiSostenuto (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams), programs (0)
{
    programs = new MidiSostenutoProgram[numPrograms];

    if (programs)
    {
        CFxBank* defaultBank = new CFxBank (kNumPrograms, kNumParams);
        if (readDefaultBank (PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
                {
                    programs[i].fParam01 = defaultBank->GetProgParm (i, 0);
                    programs[i].fParam02 = defaultBank->GetProgParm (i, 1);
                    programs[i].fParam03 = defaultBank->GetProgParm (i, 2);
                    programs[i].fParam04 = defaultBank->GetProgParm (i, 3);
                    programs[i].fParam05 = defaultBank->GetProgParm (i, 4);
                    strcpy (programs[i].name, defaultBank->GetProgramName (i));
                }
            }
        }
        else
        {
            // built-in programs
            for (int i = 0; i < kNumPrograms; i++)
            {
                sprintf (programs[i].name, "Program %d", i + 1);
            }
            setProgram (0);
        }
    }

    sostenuto = 0;
    discard = 0;
    for (int i = 0; i < 128; i++)
    {
        held_notes[i] = 0;
        sustained_notes[i] = 0;
        noteon_queue[i] = 0;
        noteon_queue_velocity[i] = 0;
    }
    CCvalue_current = 0;
    CCvalue_prev = 0;

    init();
}

//-----------------------------------------------------------------------------------------
MidiSostenuto::~MidiSostenuto()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiSostenuto::setProgram (VstInt32 program)
{
    MidiSostenutoProgram* ap = &programs[program];
    curProgram = program;
    setParameter (kParam01, ap->fParam01);
    setParameter (kParam02, ap->fParam02);
    setParameter (kParam03, ap->fParam03);
    setParameter (kParam04, ap->fParam04);
    setParameter (kParam05, ap->fParam05);
}

//------------------------------------------------------------------------
void MidiSostenuto::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiSostenuto::getProgramName (char* name)
{
    strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiSostenuto::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy (text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiSostenuto::setParameter (VstInt32 index, float value)
{
    MidiSostenutoProgram* ap = &programs[curProgram];
    switch (index)
    {
        case kParam01:
            fParam01 = ap->fParam01 = value;
            break;
        case kParam02:
            fParam02 = ap->fParam02 = value;
            break;
        case kParam03:
            fParam03 = ap->fParam03 = value;
            break;
        case kParam04:
            fParam04 = ap->fParam04 = value;
            break;
        case kParam05:
            fParam05 = ap->fParam05 = value;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiSostenuto::getParameter (VstInt32 index)
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
        case kParam04:
            v = fParam04;
            break;
        case kParam05:
            v = fParam05;
            break;
        default:
            break;
    }
    return v;
}

//-----------------------------------------------------------------------------------------
void MidiSostenuto::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kParam01:
            strcpy (label, "Pedal CC");
            break;
        case kParam02:
            strcpy (label, "Low Note");
            break;
        case kParam03:
            strcpy (label, "High Note");
            break;
        case kParam04:
            strcpy (label, "Pedal Position");
            break;
        case kParam05:
            strcpy (label, "Channel");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiSostenuto::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kParam01:
            sprintf (text, "%d", FLOAT_TO_MIDI (fParam01));
            break;
        case kParam02:
            strcpy (text, getNoteName (FLOAT_TO_MIDI (fParam02), bottomOctave));
            break;
        case kParam03:
            strcpy (text, getNoteName (FLOAT_TO_MIDI (fParam03), bottomOctave));
            break;
        case kParam04:
            if (FLOAT_TO_MIDI (fParam04) >= PEDAL_THRESHOLD)
                strcpy (text, "on");
            else
                strcpy (text, "off");
            break;
        case kParam05:
            sprintf (text, "%d", FLOAT_TO_CHANNEL015 (fParam05) + 1);
            break;
        default:
            break;
    }
}

void MidiSostenuto::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    short outchannel = FLOAT_TO_CHANNEL015 (fParam05);
    int n;
    discard = 0;

    // use "Pedal Position" parameter as the pedal
    CCvalue_current = FLOAT_TO_MIDI (fParam04);
    if (CCvalue_current >= PEDAL_THRESHOLD && CCvalue_prev < PEDAL_THRESHOLD)
    {
        sostenuto = 1;
        for (n = 0; n < 128; n++)
        {
            sustained_notes[n] = held_notes[n];
        }
    }
    else if (CCvalue_current < PEDAL_THRESHOLD && CCvalue_prev >= PEDAL_THRESHOLD && sostenuto == 1)
    {
        sostenuto = 0;
        for (n = 0; n < 128; n++)
        {
            if (sustained_notes[n] == 1)
            {
                sustained_notes[n] = 0;
                if (held_notes[n] != 1)
                { //don't turn it off if it's being held manually
                    VstMidiEvent noteoffplease = inputs[0][0];
                    noteoffplease.midiData[0] = MIDI_NOTEOFF + outchannel;
                    noteoffplease.midiData[1] = n;
                    noteoffplease.midiData[2] = 0;
                    outputs[0].push_back (noteoffplease);
                }
            }
        }
    }
    CCvalue_prev = CCvalue_current;

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        short status = tomod.midiData[0] & 0xf0; // scraping  channel
        short channel = tomod.midiData[0] & 0x0f; // isolating channel
        short data1 = tomod.midiData[1] & 0x7f;
        short data2 = tomod.midiData[2] & 0x7f;
        short listenCC = FLOAT_TO_MIDI (fParam01);
        short lownote = FLOAT_TO_MIDI (fParam02);
        short highnote = FLOAT_TO_MIDI (fParam03);

        if (channel == outchannel)
        { //only look at the selected channel
            if (status == MIDI_CONTROLCHANGE && data1 == MIDI_ALL_NOTES_OFF)
            { //handle midi panic
                setParameter (kParam04, MIDI_TO_FLOAT (0));
                sostenuto = 0;
                for (n = 0; n < 128; n++)
                {
                    if (sustained_notes[n] == 1)
                    {
                        sustained_notes[n] = 0;
                        VstMidiEvent noteoffplease = inputs[0][i];
                        noteoffplease.midiData[0] = MIDI_NOTEOFF + outchannel;
                        noteoffplease.midiData[1] = n;
                        noteoffplease.midiData[2] = 0;
                        outputs[0].push_back (noteoffplease);
                    }
                }
            }

            if (status == MIDI_NOTEON && data2 > 0)
            {
                if (data1 >= lownote && data1 <= highnote)
                {
                    if (sostenuto == 1)
                    {
                        if (sustained_notes[data1] == 1)
                        {
                            tomod.midiData[0] = MIDI_NOTEOFF + outchannel;
                            noteon_queue[data1] = 1;
                            noteon_queue_velocity[data1] = data2;
                        }
                    }
                    held_notes[data1] = 1;
                }
            }

            else if ((status == MIDI_NOTEOFF) || (status == MIDI_NOTEON && data2 == 0))
            {
                held_notes[data1] = 0;
                if (sustained_notes[data1] == 1)
                {
                    discard = 1;
                }
            }

            else if (status == MIDI_CONTROLCHANGE && data1 == listenCC)
            {
                setParameterAutomated (kParam04, MIDI_TO_FLOAT (data2)); //energyXT2 used to move the slider with the CC. anybody else?
                discard = 1; //don't send the CC through
                if (data2 >= PEDAL_THRESHOLD && sostenuto == 0)
                {
                    sostenuto = 1;
                    for (n = 0; n < 128; n++)
                    {
                        sustained_notes[n] = held_notes[n];
                    }
                }

                else if (data2 < PEDAL_THRESHOLD && sostenuto == 1)
                {
                    sostenuto = 0;
                    for (n = 0; n < 128; n++)
                    {
                        if (sustained_notes[n] == 1)
                        {
                            sustained_notes[n] = 0;
                            if (held_notes[n] != 1)
                            { //don't turn it off if it's being held manually
                                VstMidiEvent noteoffplease = inputs[0][i];
                                noteoffplease.midiData[0] = MIDI_NOTEOFF + outchannel;
                                noteoffplease.midiData[1] = n;
                                noteoffplease.midiData[2] = 0;
                                outputs[0].push_back (noteoffplease);
                            }
                        }
                    }
                }
            }
        }

        //pushing back our copied midi event from input, modified or not!
        if (discard != 1)
            outputs[0].push_back (tomod);
        //else discard = 0;

        //create queued events
        for (n = 0; n < 128; n++)
        {
            if (noteon_queue[n] == 1)
            {
                VstMidiEvent noteonplease = inputs[0][i];
                noteonplease.midiData[0] = MIDI_NOTEON + outchannel;
                noteonplease.midiData[1] = n;
                noteonplease.midiData[2] = (char) noteon_queue_velocity[n];
                outputs[0].push_back (noteonplease);
                noteon_queue[n] = 0;
            }
        }
    }
}
