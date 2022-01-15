#include "midiBlackKeyFilter.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiBlackKeyFilter (audioMaster);
}

MidiBlackKeyFilterProgram::MidiBlackKeyFilterProgram()
{
    // default program values
    param[kInChannel] = CHANNEL_TO_FLOAT (ANY_CHANNEL);
    param[kWhiteChannel] = CHANNEL_TO_FLOAT (ANY_CHANNEL);
    param[kWhiteVelocity] = 0.5f;
    param[kBlackChannel] = CHANNEL_TO_FLOAT (ANY_CHANNEL);
    param[kBlackVelocity] = 0.5f;

    // default program name
    vst_strncpy (name, "Default", kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------
MidiBlackKeyFilter::MidiBlackKeyFilter (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams)
{
    programs = new MidiBlackKeyFilterProgram[numPrograms];

    if (programs)
    {
        CFxBank* defaultBank = new CFxBank (kNumPrograms, kNumParams);
        if (readDefaultBank (PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
                {
                    for (int p = 0; p < kNumParams; p++)
                    {
                        programs[i].param[p] = defaultBank->GetProgParm (i, p);
                    }
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
        }
        setProgram (0);
    }

    for (int n = 0; n < 128; n++)
    {
        for (int c = 0; c < 16; c++)
            playingOnChannel[n][c] = -1;
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiBlackKeyFilter::~MidiBlackKeyFilter()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiBlackKeyFilter::setProgram (VstInt32 program)
{
    if (program < numPrograms)
    {
        MidiBlackKeyFilterProgram* ap = &programs[program];

        curProgram = program;
        for (int i = 0; i < kNumParams; i++)
        {
            setParameter (i, ap->param[i]);
        }
    }
}

//------------------------------------------------------------------------
void MidiBlackKeyFilter::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiBlackKeyFilter::getProgramName (char* name)
{
    vst_strncpy (name, programs[curProgram].name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool MidiBlackKeyFilter::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < numPrograms)
    {
        vst_strncpy (text, programs[index].name, kVstMaxProgNameLen);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiBlackKeyFilter::setParameter (VstInt32 index, float value)
{
    if (index < numParams)
    {
        MidiBlackKeyFilterProgram* ap = &programs[curProgram];
        param[index] = ap->param[index] = value;
    }
}

//-----------------------------------------------------------------------------------------
float MidiBlackKeyFilter::getParameter (VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiBlackKeyFilter::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kInChannel:
            vst_strncpy (label, "InputCh", kVstMaxParamStrLen);
            break;
        case kWhiteChannel:
            vst_strncpy (label, "WhiteCh", kVstMaxParamStrLen);
            break;
        case kWhiteVelocity:
            vst_strncpy (label, "WhiteVel", kVstMaxParamStrLen);
            break;
        case kBlackChannel:
            vst_strncpy (label, "BlackCh", kVstMaxParamStrLen);
            break;
        case kBlackVelocity:
            vst_strncpy (label, "BlackVel", kVstMaxParamStrLen);
            break;
        default:
            sprintf (label, "param %d", index);
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiBlackKeyFilter::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kWhiteVelocity:
        case kBlackVelocity:
            sprintf (text, "%d %", roundToInt (param[index] * 200.f));
            break;
        case kInChannel:
            if (FLOAT_TO_CHANNEL (param[index]) == ANY_CHANNEL)
                vst_strncpy (text, "Any", kVstMaxParamStrLen);
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL (param[index]) + 1);
            break;
        case kWhiteChannel:
        case kBlackChannel:
            if (FLOAT_TO_CHANNEL (param[index]) == ANY_CHANNEL)
                vst_strncpy (text, "NoChange", kVstMaxParamStrLen);
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL (param[index]) + 1);
            break;
        default:
            sprintf (text, "%f", param[index]);
            break;
    }
}

void MidiBlackKeyFilter::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const int listenchannel = FLOAT_TO_CHANNEL (param[kInChannel]);
    int BlackChannel = FLOAT_TO_CHANNEL (param[kBlackChannel]);
    int WhiteChannel = FLOAT_TO_CHANNEL (param[kWhiteChannel]);

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        unsigned int status = tomod.midiData[0] & 0xf0; // scraping  channel
        const int channel = tomod.midiData[0] & 0x0f; // isolating channel
        const int data1 = tomod.midiData[1] & 0x7f;
        const int data2 = tomod.midiData[2] & 0x7f;

        // make zero-velocity noteons look like "real" noteoffs
        if (status == MIDI_NOTEON && data2 == 0)
            status = MIDI_NOTEOFF;

        if (channel == listenchannel || listenchannel == -1)
        {
            if (status == MIDI_NOTEON)
            {
                if (isBlackKey (data1))
                {
                    if (BlackChannel == ANY_CHANNEL)
                        BlackChannel = channel;
                    tomod.midiData[0] = MIDI_NOTEON | BlackChannel;
                    tomod.midiData[2] = midiLimit (roundToInt ((float) data2 * 2.f * param[kBlackVelocity]));
                }
                else
                {
                    if (WhiteChannel == ANY_CHANNEL)
                        WhiteChannel = channel;
                    tomod.midiData[0] = MIDI_NOTEON | WhiteChannel;
                    tomod.midiData[2] = midiLimit (roundToInt ((float) data2 * 2.f * param[kWhiteVelocity]));
                }
                if (tomod.midiData[2] > 0)
                    playingOnChannel[data1][channel] = tomod.midiData[0] & 0x0f;
            }
            else if (status == MIDI_NOTEOFF)
            {
                if (isBlackKey (data1))
                {
                    if (BlackChannel == ANY_CHANNEL)
                        BlackChannel = channel;
                    tomod.midiData[0] = MIDI_NOTEOFF | BlackChannel;
                }
                else
                {
                    if (WhiteChannel == ANY_CHANNEL)
                        WhiteChannel = channel;
                    tomod.midiData[0] = MIDI_NOTEOFF | WhiteChannel;
                }
                playingOnChannel[data1][channel] = -1;
            }
        }
        outputs[0].push_back (tomod);
    }
}
