#include "midiStuckNoteKiller.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new midiStuckNoteKiller (audioMaster);
}

//-----------------------------------------------------------------------------
midiStuckNoteKiller::midiStuckNoteKiller (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, 1, kNumParams)
{
    CFxBank* defaultBank = new CFxBank (numPrograms, numParams);
    if (readDefaultBank (PLUG_NAME, defaultBank))
    {
        if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
        {
            fParam01 = defaultBank->GetProgParm (0, 0);
            strcpy (_programName, defaultBank->GetProgramName (0));
        }
    }
    else
    {
        // default values
        fParam01 = 1.0;
        strcpy (_programName, "StuckNoteKiller"); // default program name
    }

    for (int ch = 0; ch < 16; ch++)
    {
        for (int i = 0; i < 128; i++)
            held_notes[i][ch] = false;
    }

    trigger = false;
    init();
}

//-----------------------------------------------------------------------------------------
midiStuckNoteKiller::~midiStuckNoteKiller() {}

//-----------------------------------------------------------------------------------------
void midiStuckNoteKiller::setParameter (VstInt32 index, float value)
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
float midiStuckNoteKiller::getParameter (VstInt32 index)
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
void midiStuckNoteKiller::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kParam01:
            strcpy (label, "Power");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void midiStuckNoteKiller::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kParam01:
            if (fParam01 < 0.5)
                strcpy (text, "Off");
            else
                strcpy (text, "On");
            break;
        default:
            break;
    }
}

void midiStuckNoteKiller::preProcess (void)
{
    //// preparing Proccess
    VstTimeInfo* timeInfo = NULL;
    timeInfo = getTimeInfo (0xffff); //ALL

    if (timeInfo)
    {
        if ((kVstTransportChanged & timeInfo->flags)
            && ! (kVstTransportPlaying & timeInfo->flags))
            trigger = true;
    }

    PizMidi::preProcess();
}

void midiStuckNoteKiller::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    if (trigger)
    {
        for (int ch = 0; ch < 16; ch++)
        {
            for (int i = 0; i < 128; i++)
            {
                if (held_notes[i][ch])
                {
                    VstMidiEvent kill;
                    kill.deltaFrames = 0;
                    kill.midiData[0] = MIDI_NOTEOFF | ch;
                    kill.midiData[1] = i;
                    kill.midiData[2] = 0;
                    outputs[0].push_back (kill);
                    held_notes[i][ch] = false;
                }
            }
        }
        trigger = false;
    }

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        short status = tomod.midiData[0] & 0xf0; // scraping  channel
        short channel = tomod.midiData[0] & 0x0f; // isolating channel
        short data1 = tomod.midiData[1] & 0x7f;
        short data2 = tomod.midiData[2] & 0x7f;

        bool on = fParam01 >= 0.5f;
        bool discard = false;

        if (on && status == MIDI_CONTROLCHANGE && data1 == 123)
        {
            for (int ch = 0; ch < 16; ch++)
            {
                for (int n = 0; n < 128; n++)
                {
                    if (held_notes[n][ch])
                    {
                        VstMidiEvent kill = tomod;
                        kill.midiData[0] = MIDI_NOTEOFF | ch;
                        kill.midiData[1] = n;
                        kill.midiData[2] = 0;
                        outputs[0].push_back (kill);
                        held_notes[n][ch] = false;
                    }
                }
            }
        }
        if (status == MIDI_NOTEON && data2 > 0)
        {
            held_notes[data1][channel] = true;
        }
        else if ((status == MIDI_NOTEOFF) || (status == MIDI_NOTEON && data2 == 0))
        {
            held_notes[data1][channel] = false;
        }
        if (! discard)
            outputs[0].push_back (tomod);
    }
}
