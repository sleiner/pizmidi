/*-----------------------------------------------------------------------------
midiChannelFilter 1.1
by Reuben Vinal
-----------------------------------------------------------------------------*/

#include "midiChannelFilter.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiChannelFilter (audioMaster);
}

MidiChannelFilterProgram::MidiChannelFilterProgram()
{
    // default Program Values
    fChannel = CHANNEL_TO_FLOAT016 (1);

    vst_strncpy (name, "MIDI Channel Filter", kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------
MidiChannelFilter::MidiChannelFilter (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams),
      programs (0)
{
    programs = new MidiChannelFilterProgram[numPrograms];

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

    init();
}

//-----------------------------------------------------------------------------------------
MidiChannelFilter::~MidiChannelFilter()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiChannelFilter::setProgram (VstInt32 program)
{
    MidiChannelFilterProgram* ap = &programs[program];

    curProgram = program;
    setParameter (kChannel, ap->fChannel);
}

//------------------------------------------------------------------------
void MidiChannelFilter::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiChannelFilter::getProgramName (char* name)
{
    vst_strncpy (name, programs[curProgram].name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool MidiChannelFilter::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < numPrograms)
    {
        vst_strncpy (text, programs[index].name, kVstMaxProgNameLen);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiChannelFilter::setParameter (VstInt32 index, float value)
{
    MidiChannelFilterProgram* ap = &programs[curProgram];

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
float MidiChannelFilter::getParameter (VstInt32 index)
{
    float v = 0;

    switch (index)
    {
        case kChannel:
            v = fChannel;
            break;
        default:
            break;
    }
    return v;
}

//-----------------------------------------------------------------------------------------
void MidiChannelFilter::getParameterName (VstInt32 index, char* label)
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
void MidiChannelFilter::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kChannel:
            if (FLOAT_TO_CHANNEL016 (fChannel) < 1)
                vst_strncpy (text, "All", kVstMaxParamStrLen);
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL016 (fChannel));
            break;
        default:
            break;
    }
}

void MidiChannelFilter::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    // process incoming events
    VstMidiEventVec::iterator it;
    for (it = inputs[0].begin(); it < inputs[0].end(); it++)
    {
        const int status = it->midiData[0] & 0xf0; // scraping  channel
        const int in_channel = (it->midiData[0] & 0x0f) + 1; // isolating channel (1-16)
        const int out_channel = FLOAT_TO_CHANNEL016 (fChannel);

        if (status < 0xF0)
        {
            if ((out_channel == 0) || (out_channel == in_channel))
            {
                outputs[0].push_back (*it);
            }
        }
        else
            outputs[0].push_back (*it);
    }
}
