#include "midiChannelize.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiChannelize (audioMaster);
}

MidiChannelizeProgram::MidiChannelizeProgram()
{
    // default Program Values
    fChannel = CHANNEL_TO_FLOAT016 (1);

    vst_strncpy (name, "MIDI Channeliser", kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------
MidiChannelize::MidiChannelize (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams), programs (0)
{
    programs = new MidiChannelizeProgram[numPrograms];

    if (programs)
    {
        CFxBank* defaultBank = new CFxBank (numPrograms, numParams);
        if (readDefaultBank (PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < numPrograms; i++)
                {
                    programs[i].fChannel = defaultBank->GetProgParm (i, 0);
                    strcpy (programs[i].name, defaultBank->GetProgramName (i));
                }
            }
        }
        else
        {
            // built-in programs
            for (int i = 0; i < numPrograms; i++)
            {
                sprintf (programs[i].name, "Program %d", i + 1);
            }
        }
        setProgram (0);
    }

    for (int c = 0; c < 16; c++)
    {
        for (int i = 0; i < 128; i++)
            lastChannel[c][i] = c;
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiChannelize::~MidiChannelize()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiChannelize::setProgram (VstInt32 program)
{
    MidiChannelizeProgram* ap = &programs[program];

    curProgram = program;
    setParameter (kChannel, ap->fChannel);
}

//------------------------------------------------------------------------
void MidiChannelize::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiChannelize::getProgramName (char* name)
{
    vst_strncpy (name, programs[curProgram].name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool MidiChannelize::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < numPrograms)
    {
        vst_strncpy (text, programs[index].name, kVstMaxProgNameLen);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiChannelize::setParameter (VstInt32 index, float value)
{
    MidiChannelizeProgram* ap = &programs[curProgram];

    switch (index)
    {
        case kChannel:
            fChannel = ap->fChannel = value;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiChannelize::getParameter (VstInt32 index)
{
    switch (index)
    {
        case kChannel:
            return fChannel;
        default:
            return 0;
    }
}

//-----------------------------------------------------------------------------------------
void MidiChannelize::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kChannel:
            vst_strncpy (label, "Channel", kVstMaxParamStrLen);
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiChannelize::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kChannel:
            if (FLOAT_TO_CHANNEL016 (fChannel) < 1)
                vst_strncpy (text, "No Change", kVstMaxParamStrLen);
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL016 (fChannel));
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiChannelize::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const int out_channel = FLOAT_TO_CHANNEL (fChannel);
    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod   = inputs[0][i];
        const int status     = tomod.midiData[0] & 0xf0; // scraping  channel
        const int in_channel = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        const int data1      = tomod.midiData[1] & 0x7f;
        //const int data2	 = tomod.midiData[2] & 0x7f;
        if (tomod.midiData[0] < 0xF0)
        {
            if (isNoteOff (tomod))
            {
                tomod.midiData[0] = status | lastChannel[in_channel][data1];
            }
            else if (out_channel >= 0)
            {
                tomod.midiData[0] = status | out_channel;
            }

            if (isNoteOn (tomod))
                lastChannel[in_channel][data1] = out_channel;
        }
        outputs[0].push_back (tomod);
    }
}
