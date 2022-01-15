#include "midiKeySplit.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiKeySplit (audioMaster);
}

MidiKeySplitProgram::MidiKeySplitProgram()
{
    // default Program Values
    for (int i = 0; i < kNumParams; i++)
    {
        switch (i)
        {
            case kOutChannel1:
                param[i] = CHANNEL_TO_FLOAT015 (0);
                break;
            case kOutChannel2:
                param[i] = CHANNEL_TO_FLOAT015 (1);
                break;
            case kInChannel:
                param[i] = CHANNEL_TO_FLOAT015 (0);
                break;
            case kSplit1:
                param[i] = MIDI_TO_FLOAT (60);
                break;
            case kTransp1:
                param[i] = 0.5f;
                break;
            case kTransp2:
                param[i] = 0.5f;
                break;

            default:
                param[i] = 0.f;
                break;
        }
    }
    // default program name
    strcpy (name, "Default");
}

//-----------------------------------------------------------------------------
MidiKeySplit::MidiKeySplit (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams), programs (0)
{
    programs = new MidiKeySplitProgram[numPrograms];

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

    for (int i = 0; i < 128; i++)
    {
        transposed[i] = -999;
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiKeySplit::~MidiKeySplit()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiKeySplit::setProgram (VstInt32 program)
{
    MidiKeySplitProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
        setParameter (i, ap->param[i]);
}

//------------------------------------------------------------------------
void MidiKeySplit::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiKeySplit::getProgramName (char* name)
{
    strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiKeySplit::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy (text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiKeySplit::setParameter (VstInt32 index, float value)
{
    MidiKeySplitProgram* ap = &programs[curProgram];
    param[index] = ap->param[index] = value;
    //switch(index) {
    //   default: break;
    //}
}

//-----------------------------------------------------------------------------------------
float MidiKeySplit::getParameter (VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiKeySplit::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kInChannel:
            strcpy (label, "InChannel");
            break;
        case kOutChannel1:
            strcpy (label, "1: OutChannel");
            break;
        case kOutChannel2:
            strcpy (label, "2: OutChannel");
            break;
        case kTransp1:
            strcpy (label, "1: Transpose");
            break;
        case kTransp2:
            strcpy (label, "2: Transpose");
            break;
        case kSplit1:
            strcpy (label, "Split Note");
            break;
        default:
            sprintf (label, "param %d", index);
    }
}

//-----------------------------------------------------------------------------------------
void MidiKeySplit::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kOutChannel1:
        case kOutChannel2:
            if (FLOAT_TO_CHANNEL (param[index]) == -1)
                strcpy (text, "As Input");
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL (param[index]) + 1);
            break;
        case kInChannel:
            sprintf (text, "%d", FLOAT_TO_CHANNEL015 (param[index]) + 1);
            break;
        case kTransp2:
        case kTransp1:
            sprintf (text, "%d", roundToInt (96.f * param[index]) - 48);
            break;
        case kSplit1:
            if (param[index] == 0.f)
                strcpy (text, "Learn...");
            else
                sprintf (text, "%s (%d)", getNoteName (FLOAT_TO_MIDI (param[index]), bottomOctave), FLOAT_TO_MIDI (param[index]));
            break;
        default:
            sprintf (text, "%f", param[index]);
    }
}

void MidiKeySplit::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const char ch = FLOAT_TO_CHANNEL015 (param[kInChannel]);
    signed char ch1 = FLOAT_TO_CHANNEL (param[kOutChannel1]);
    signed char ch2 = FLOAT_TO_CHANNEL (param[kOutChannel2]);
    const char split1 = param[kSplit1] == 0.f ? -1 : FLOAT_TO_MIDI (param[kSplit1]);
    const char transp1 = roundToInt (96.f * param[kTransp1]) - 48;
    const char transp2 = roundToInt (96.f * param[kTransp2]) - 48;

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        unsigned char status = tomod.midiData[0] & 0xf0; // scraping  channel
        unsigned char channel = tomod.midiData[0] & 0x0f; // isolating channel
        unsigned char data1 = tomod.midiData[1] & 0x7f;
        unsigned char data2 = tomod.midiData[2] & 0x7f;

        if (status == MIDI_NOTEON && data2 == 0)
            status = MIDI_NOTEOFF;

        if (ch1 == -1)
            ch1 = channel;
        if (ch2 == -1)
            ch2 = channel;

        if (channel == ch)
        {
            if (status == MIDI_NOTEON)
            {
                if (split1 == -1)
                {
                    setParameterAutomated (kSplit1, MIDI_TO_FLOAT (data1));
                }
                if (data1 < split1)
                {
                    transposed[data1] = transp1;
                    tomod.midiData[0] = status | ch1;
                    tomod.midiData[1] = midiLimit (data1 + transp1);
                }
                else
                {
                    transposed[data1] = transp2;
                    tomod.midiData[0] = status | ch2;
                    tomod.midiData[1] = midiLimit (data1 + transp2);
                }
            }
            else if (status == MIDI_NOTEOFF)
            {
                if (data1 < split1)
                {
                    tomod.midiData[0] = status | ch1;
                    tomod.midiData[1] = midiLimit (data1 + transposed[data1]);
                    transposed[data1] = -999;
                }
                else
                {
                    tomod.midiData[0] = status | ch2;
                    tomod.midiData[1] = midiLimit (data1 + transposed[data1]);
                    transposed[data1] = -999;
                }
            }
            else if (status == MIDI_POLYKEYPRESSURE)
            {
                if (data1 < split1)
                {
                    tomod.midiData[0] = status | ch1;
                    tomod.midiData[1] = midiLimit (data1 + transp1);
                }
                else
                {
                    tomod.midiData[0] = status | ch2;
                    tomod.midiData[1] = midiLimit (data1 + transp2);
                }
            }
        }
        outputs[0].push_back (tomod);
    }
}
