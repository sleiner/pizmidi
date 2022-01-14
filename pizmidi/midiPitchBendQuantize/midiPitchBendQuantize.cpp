#include "midiPitchBendQuantize.hpp"

inline float pbToParam (int pb)
{
    if (pb < 0)
        return 0.f;
    return (0.99f / 16383.f) * (float) pb + 0.01f;
}

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiPitchBendQuantize (audioMaster);
}

MidiPitchBendQuantizeProgram::MidiPitchBendQuantizeProgram()
{
    // default program values
    for (int i = 0; i < kNumParams; i++)
    {
        switch (i)
        {
            case kChannel:
                param[kChannel] = CHANNEL_TO_FLOAT (-1); // any channel
                break;
            default:
                param[i] = 0.f;
                break;
        }
    }

    // default program name
    vst_strncpy (name, "Default", kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------
MidiPitchBendQuantize::MidiPitchBendQuantize (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams), programs (0)
{
    memset (step, 0, sizeof (step));

    programs = new MidiPitchBendQuantizeProgram[numPrograms];

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
            //built-in programs
            int i = 0;
            fillpatch (i++, "Equal spacing", 16383, 15700, 15018, 14335, 13653, 12970, 12287, 11605, 10922, 10239, 9557, 8874, 8192, 7509, 6826, 6144, 5461, 4778, 4096, 3413, 2731, 2048, 1365, 683, 0, ANY_CHANNEL);
            fillpatch (i++, "Major Scale", 16383, 15700, -1, 14335, -1, 12970, -1, 11605, 10922, -1, 9557, -1, 8192, 7509, -1, 6144, -1, 4778, -1, 3413, 2731, -1, 1365, -1, 0, ANY_CHANNEL);
            fillpatch (i++, "Minor Scale",
                       16383, //+12  A
                       -1, //+11  G#
                       15018, //+10  G
                       -1, //+9   F#
                       13653, //+8   F
                       12970, //+7   E
                       -1, //+6   D#
                       11605, //+5   D
                       -1, //+4   C#
                       10239, //+3   C
                       9557, //+2   B
                       -1, //+1   A#
                       8192, //center A
                       -1, //-1   G#
                       6826, //-2   G
                       -1, //-3   F#
                       5461, //-4   F
                       4778, //-5   E
                       -1, //-6   D#
                       3413, //-7   D
                       -1, //-8   C#
                       2048, //-9   C
                       1365, //-10  B
                       -1, //-11  A#
                       0, //-12  A
                       ANY_CHANNEL);
            setProgram (0);
        }
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiPitchBendQuantize::~MidiPitchBendQuantize()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiPitchBendQuantize::setProgram (VstInt32 program)
{
    MidiPitchBendQuantizeProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter (i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiPitchBendQuantize::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiPitchBendQuantize::getProgramName (char* name)
{
    vst_strncpy (name, programs[curProgram].name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool MidiPitchBendQuantize::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < numPrograms)
    {
        vst_strncpy (text, programs[index].name, kVstMaxProgNameLen);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiPitchBendQuantize::setParameter (VstInt32 index, float value)
{
    if (index < kNumParams)
    {
        MidiPitchBendQuantizeProgram* ap = &programs[curProgram];
        param[index] = ap->param[index] = value;
        if (index < kNumSteps)
        {
            if (value < 0.01f)
                step[index] = -99;
            else
            {
                float slope = 1.f / 0.99f;
                step[index] = roundToInt ((slope * (value - 0.01f)) * 16383.f);
            }
        }
        else if (index == kChannel)
        {
            for (int p = 0; p < kNumPrograms; p++)
                programs[p].param[kChannel] = value;
        }
    }
}

//-----------------------------------------------------------------------------------------
float MidiPitchBendQuantize::getParameter (VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiPitchBendQuantize::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kChannel:
            vst_strncpy (label, "Channel", kVstMaxParamStrLen);
            break;
        default:
            if (index < 12)
                sprintf (label, "q+%d", 12 - index);
            else
                sprintf (label, "q%d", 12 - index);
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiPitchBendQuantize::getParameterDisplay (VstInt32 index, char* text)
{
    if (index < kNumSteps)
    {
        if (step[index] < 0)
            vst_strncpy (text, "Off", kVstMaxParamStrLen);
        else
        {
            if (step[index] == 8192)
                sprintf (text, "%d (center)", step[index]);
            else if (step[index] == 0)
                sprintf (text, "%d (min)", step[index]);
            else if (step[index] == 16383)
                sprintf (text, "%d (max)", step[index]);
            else
                sprintf (text, "%d", step[index]);
        }
    }
    else if (index == kChannel)
    {
        if (FLOAT_TO_CHANNEL (param[index]) == -1)
            vst_strncpy (text, "Any", kVstMaxParamStrLen);
        else
            sprintf (text, "%d", FLOAT_TO_CHANNEL (param[index]) + 1);
    }
    else
        sprintf (text, "%f", param[index]);
}

void MidiPitchBendQuantize::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const char listenchannel = FLOAT_TO_CHANNEL (param[kChannel]);

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        unsigned char status = tomod.midiData[0] & 0xf0; // scraping  channel
        const char channel = tomod.midiData[0] & 0x0f; // isolating channel
        const char data1 = tomod.midiData[1] & 0x7f;
        const char data2 = tomod.midiData[2] & 0x7f;

        if (channel == listenchannel || listenchannel == -1)
        {
            if (status == MIDI_PITCHBEND)
            {
                int PB = data1 | (data2 << 7);
                int diff = 99999;
                for (int s = 0; s < kNumSteps; s++)
                {
                    if (step[s] >= 0)
                    {
                        int d = step[s] - PB;
                        if (abs (d) < abs (diff))
                            diff = d;
                    }
                }
                if (diff < 99999)
                {
                    PB += diff;
                    tomod.midiData[1] = PB & 0x007f;
                    tomod.midiData[2] = (PB & 0x3f80) >> 7;
                    outputs[0].push_back (tomod);
                }
            }
            else
                outputs[0].push_back (tomod);
        }
    }
}

void MidiPitchBendQuantize::fillpatch (long p, const char* name, int p0, int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14, int p15, int p16, int p17, int p18, int p19, int p20, int p21, int p22, int p23, int p24, int p25)
{
    vst_strncpy (programs[p].name, name, kVstMaxProgNameLen);
    programs[p].param[0] = pbToParam (p0);
    programs[p].param[1] = pbToParam (p1);
    programs[p].param[2] = pbToParam (p2);
    programs[p].param[3] = pbToParam (p3);
    programs[p].param[4] = pbToParam (p4);
    programs[p].param[5] = pbToParam (p5);
    programs[p].param[6] = pbToParam (p6);
    programs[p].param[7] = pbToParam (p7);
    programs[p].param[8] = pbToParam (p8);
    programs[p].param[9] = pbToParam (p9);
    programs[p].param[10] = pbToParam (p10);
    programs[p].param[11] = pbToParam (p11);
    programs[p].param[12] = pbToParam (p12);
    programs[p].param[13] = pbToParam (p13);
    programs[p].param[14] = pbToParam (p14);
    programs[p].param[15] = pbToParam (p15);
    programs[p].param[16] = pbToParam (p16);
    programs[p].param[17] = pbToParam (p17);
    programs[p].param[18] = pbToParam (p18);
    programs[p].param[19] = pbToParam (p19);
    programs[p].param[20] = pbToParam (p20);
    programs[p].param[21] = pbToParam (p21);
    programs[p].param[22] = pbToParam (p22);
    programs[p].param[23] = pbToParam (p23);
    programs[p].param[24] = pbToParam (p24);
    programs[p].param[25] = CHANNEL_TO_FLOAT (p25);
}
