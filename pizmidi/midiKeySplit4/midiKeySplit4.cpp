#include "midiKeySplit4.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiKeySplit4 (audioMaster);
}

MidiKeySplit4Program::MidiKeySplit4Program()
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
                param[i] = CHANNEL_TO_FLOAT015 (0);
                break;
            case kOutChannel3:
                param[i] = CHANNEL_TO_FLOAT015 (0);
                break;
            case kOutChannel4:
                param[i] = CHANNEL_TO_FLOAT015 (0);
                break;
            case kInChannel:
                param[i] = CHANNEL_TO_FLOAT015 (0);
                break;
            case kSplit1:
                param[i] = MIDI_TO_FLOAT_X (60);
                break;
            case kSplit2:
                param[i] = MIDI_TO_FLOAT_X (127);
                break;
            case kSplit3:
                param[i] = MIDI_TO_FLOAT_X (127);
                break;
            case kTransp1:
                param[i] = 0.5f;
                break;
            case kTransp2:
                param[i] = 0.5f;
                break;
            case kTransp3:
                param[i] = 0.5f;
                break;
            case kTransp4:
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
MidiKeySplit4::MidiKeySplit4 (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams), programs (0)
{
    programs = new MidiKeySplit4Program[numPrograms];

    for (int i = 0; i < 128; i++)
    {
        transposed[i] = -999;
    }

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

    init();
}

//-----------------------------------------------------------------------------------------
MidiKeySplit4::~MidiKeySplit4()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiKeySplit4::setProgram (VstInt32 program)
{
    MidiKeySplit4Program* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
        setParameter (i, ap->param[i]);
}

//------------------------------------------------------------------------
void MidiKeySplit4::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiKeySplit4::getProgramName (char* name)
{
    strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiKeySplit4::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy (text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiKeySplit4::setParameter (VstInt32 index, float value)
{
    MidiKeySplit4Program* ap = &programs[curProgram];
    param[index] = ap->param[index] = value;
    //switch(index) {
    //    case kSplit1:
    //        if (value>param[kSplit2]) setParameterAutomated(kSplit2,value); break;
    //    case kSplit2:
    //        if (value<param[kSplit1]) setParameterAutomated(kSplit1,value);
    //        if (value>param[kSplit3]) setParameterAutomated(kSplit3,value);
    //        break;
    //    case kSplit3:
    //        if (value<param[kSplit2]) setParameterAutomated(kSplit2,value);
    //        break;
    //
    //   default: break;
    //}
}

//-----------------------------------------------------------------------------------------
float MidiKeySplit4::getParameter (VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiKeySplit4::getParameterName (VstInt32 index, char* label)
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
        case kOutChannel3:
            strcpy (label, "3: OutChannel");
            break;
        case kOutChannel4:
            strcpy (label, "4: OutChannel");
            break;
        case kTransp1:
            strcpy (label, "1: Transpose");
            break;
        case kTransp2:
            strcpy (label, "2: Transpose");
            break;
        case kTransp3:
            strcpy (label, "3: Transpose");
            break;
        case kTransp4:
            strcpy (label, "4: Transpose");
            break;
        case kSplit1:
            strcpy (label, "1/2: Split Note");
            break;
        case kSplit2:
            strcpy (label, "2/3: Split Note");
            break;
        case kSplit3:
            strcpy (label, "3/4: Split Note");
            break;
        default:
            sprintf (label, "param %d", index);
    }
}

//-----------------------------------------------------------------------------------------
void MidiKeySplit4::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kOutChannel1:
        case kOutChannel2:
        case kOutChannel3:
        case kOutChannel4:
            if (FLOAT_TO_CHANNEL (param[index]) == -1)
                strcpy (text, "As Input");
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL (param[index]) + 1);
            break;
        case kInChannel:
            sprintf (text, "%d", FLOAT_TO_CHANNEL015 (param[index]) + 1);
            break;
        case kTransp1:
        case kTransp2:
        case kTransp3:
        case kTransp4:
            sprintf (text, "%d", roundToInt (96.f * param[index]) - 48);
            break;
        case kSplit1:
        case kSplit2:
        case kSplit3:
            if (FLOAT_TO_MIDI_X (param[index]) == learn)
                strcpy (text, "Learn...");
            else
                sprintf (text, "%s (%d)", getNoteName (FLOAT_TO_MIDI_X (param[index]), bottomOctave), FLOAT_TO_MIDI_X (param[index]));
            break;
        default:
            sprintf (text, "%f", param[index]);
    }
}

void MidiKeySplit4::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const char ch      = FLOAT_TO_CHANNEL015 (param[kInChannel]);
    signed char ch1    = FLOAT_TO_CHANNEL (param[kOutChannel1]);
    signed char ch2    = FLOAT_TO_CHANNEL (param[kOutChannel2]);
    signed char ch3    = FLOAT_TO_CHANNEL (param[kOutChannel3]);
    signed char ch4    = FLOAT_TO_CHANNEL (param[kOutChannel4]);
    const char split1  = FLOAT_TO_MIDI_X (param[kSplit1]);
    const char split2  = FLOAT_TO_MIDI_X (param[kSplit2]);
    const char split3  = FLOAT_TO_MIDI_X (param[kSplit3]);
    const char transp1 = roundToInt (96.f * param[kTransp1]) - 48;
    const char transp2 = roundToInt (96.f * param[kTransp2]) - 48;
    const char transp3 = roundToInt (96.f * param[kTransp3]) - 48;
    const char transp4 = roundToInt (96.f * param[kTransp4]) - 48;

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        int status  = tomod.midiData[0] & 0xf0; // scraping  channel
        int channel = tomod.midiData[0] & 0x0f; // isolating channel
        int data1   = tomod.midiData[1] & 0x7f;
        int data2   = tomod.midiData[2] & 0x7f;

        if (status == MIDI_NOTEON && data2 == 0)
            status = MIDI_NOTEOFF;

        if (ch1 == -1)
            ch1 = channel;
        if (ch2 == -1)
            ch2 = channel;
        if (ch3 == -1)
            ch3 = channel;
        if (ch4 == -1)
            ch4 = channel;

        if (channel == ch)
        {
            if (status == MIDI_NOTEON)
            {
                if (split1 == learn)
                {
                    setParameterAutomated (kSplit1, MIDI_TO_FLOAT_X (data1));
                }
                if (split2 == learn)
                {
                    setParameterAutomated (kSplit2, MIDI_TO_FLOAT_X (data1));
                }
                if (split3 == learn)
                {
                    setParameterAutomated (kSplit3, MIDI_TO_FLOAT_X (data1));
                }

                if (data1 < split1)
                {
                    transposed[data1] = transp1;
                    tomod.midiData[0] = status | ch1;
                    tomod.midiData[1] = midiLimit (data1 + transp1);
                }
                else if (data1 < split2)
                {
                    transposed[data1] = transp2;
                    tomod.midiData[0] = status | ch2;
                    tomod.midiData[1] = midiLimit (data1 + transp2);
                }
                else if (data1 < split3)
                {
                    transposed[data1] = transp3;
                    tomod.midiData[0] = status | ch3;
                    tomod.midiData[1] = midiLimit (data1 + transp3);
                }
                else
                {
                    transposed[data1] = transp4;
                    tomod.midiData[0] = status | ch4;
                    tomod.midiData[1] = midiLimit (data1 + transp4);
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
                else if (data1 < split2)
                {
                    tomod.midiData[0] = status | ch2;
                    tomod.midiData[1] = midiLimit (data1 + transposed[data1]);
                    transposed[data1] = -999;
                }
                else if (data1 < split3)
                {
                    tomod.midiData[0] = status | ch3;
                    tomod.midiData[1] = midiLimit (data1 + transposed[data1]);
                    transposed[data1] = -999;
                }
                else
                {
                    tomod.midiData[0] = status | ch4;
                    tomod.midiData[1] = midiLimit (data1 + transposed[data1]);
                    transposed[data1] = -999;
                }
            }
            if (status == MIDI_POLYKEYPRESSURE)
            {
                if (data1 < split1)
                {
                    tomod.midiData[0] = status | ch1;
                    tomod.midiData[1] = midiLimit (data1 + transp1);
                }
                else if (data1 < split2)
                {
                    tomod.midiData[0] = status | ch2;
                    tomod.midiData[1] = midiLimit (data1 + transp2);
                }
                else if (data1 < split3)
                {
                    tomod.midiData[0] = status | ch3;
                    tomod.midiData[1] = midiLimit (data1 + transp3);
                }
                else
                {
                    tomod.midiData[0] = status | ch4;
                    tomod.midiData[1] = midiLimit (data1 + transp4);
                }
            }
        }
        outputs[0].push_back (tomod);
    }
}
