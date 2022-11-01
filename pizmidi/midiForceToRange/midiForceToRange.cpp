#include "midiForceToRange.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new ForceToRange(audioMaster);
}

ForceToRangeProgram::ForceToRangeProgram()
{
    // default program values
    param[kLowNote]  = MIDI_TO_FLOAT(0);
    param[kHighNote] = MIDI_TO_FLOAT(127);
    param[kChannel]  = CHANNEL_TO_FLOAT(ANY_CHANNEL);

    // default program name
    vst_strncpy(name, "default", kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------
ForceToRange::ForceToRange(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    settingProgram = false;
    programs       = new ForceToRangeProgram[numPrograms];
    if (programs)
    {
        CFxBank* defaultBank = new CFxBank(kNumPrograms, kNumParams);
        if (readDefaultBank(PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
                {
                    for (int p = 0; p < kNumParams; p++)
                    {
                        programs[i].param[p] = defaultBank->GetProgParm(i, p);
                    }
                    strcpy(programs[i].name, defaultBank->GetProgramName(i));
                }
            }
        }
        else
        {
            // built-in programs
            for (int i = 0; i < kNumPrograms; i++)
            {
                switch (i)
                {
                    case 0:
                        programs[i].param[kLowNote]  = MIDI_TO_FLOAT(48);
                        programs[i].param[kHighNote] = MIDI_TO_FLOAT(60);
                        sprintf(programs[i].name, "Octave 2");
                        break;
                    case 1:
                        programs[i].param[kLowNote]  = MIDI_TO_FLOAT(60);
                        programs[i].param[kHighNote] = MIDI_TO_FLOAT(72);
                        sprintf(programs[i].name, "Octave 3");
                        break;
                        //etc
                    default:
                        sprintf(programs[i].name, "Program %d", i + 1);
                        break;
                }
            }
        }
        setProgram(0);
    }

    for (int n = 0; n < 128; n++)
    {
        for (int c = 0; c < 16; c++)
        {
            transposed[n][c] = n;
        }
    }

    init();
}

//-----------------------------------------------------------------------------------------
ForceToRange::~ForceToRange()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void ForceToRange::setProgram(VstInt32 program)
{
    if (program < numPrograms)
    {
        settingProgram          = true;
        ForceToRangeProgram* ap = &programs[program];

        curProgram = program;
        for (int i = 0; i < kNumParams; i++)
        {
            setParameter(i, ap->param[i]);
        }
        settingProgram = false;
    }
}

//------------------------------------------------------------------------
void ForceToRange::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void ForceToRange::getProgramName(char* name)
{
    vst_strncpy(name, programs[curProgram].name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool ForceToRange::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < numPrograms)
    {
        vst_strncpy(text, programs[index].name, kVstMaxProgNameLen);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void ForceToRange::setParameter(VstInt32 index, float value)
{
    if (index < numParams)
    {
        ForceToRangeProgram* ap = &programs[curProgram];
        param[index] = ap->param[index] = value;
        if (index == kHighNote)
        {
            if (value < param[kLowNote])
            {
                setParameterAutomated(kLowNote, value);
            }
        }
        else if (index == kLowNote)
        {
            if (value > param[kHighNote])
            {
                setParameterAutomated(kHighNote, value);
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
float ForceToRange::getParameter(VstInt32 index)
{
    if (index < numParams)
    {
        return param[index];
    }
    return 0.f;
}

//-----------------------------------------------------------------------------------------
void ForceToRange::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kLowNote:
            vst_strncpy(label, "LowNote", kVstMaxParamStrLen);
            break;
        case kHighNote:
            vst_strncpy(label, "HighNote", kVstMaxParamStrLen);
            break;
        case kChannel:
            vst_strncpy(label, "Channel", kVstMaxParamStrLen);
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void ForceToRange::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kLowNote:
        case kHighNote:
            sprintf(text, "%s (%d)", getNoteName(FLOAT_TO_MIDI(param[index]), bottomOctave), FLOAT_TO_MIDI(param[index]));
            break;
        case kChannel:
            if (FLOAT_TO_CHANNEL(param[index]) == -1)
            {
                vst_strncpy(text, "Any", kVstMaxParamStrLen);
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL(param[index]) + 1);
            }
            break;
        default:
            break;
    }
}

void ForceToRange::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const char lowlimit      = FLOAT_TO_MIDI(param[kLowNote]);
    const char highlimit     = FLOAT_TO_MIDI(param[kHighNote]);
    const char listenchannel = FLOAT_TO_CHANNEL(param[kChannel]);

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        unsigned char status = tomod.midiData[0] & 0xf0; // scraping  channel
        const char channel   = tomod.midiData[0] & 0x0f; // isolating channel
        const char data1     = tomod.midiData[1] & 0x7f;
        const char data2     = tomod.midiData[2] & 0x7f;

        // make zero-velocity noteons look like "real" noteoffs
        if (status == MIDI_NOTEON && data2 == 0)
        {
            status = MIDI_NOTEOFF;
        }

        bool discard = false;

        if (channel == listenchannel || listenchannel == -1)
        {
            if (status == MIDI_NOTEON || status == MIDI_POLYKEYPRESSURE)
            {
                int newnote = data1;
                if (data1 < lowlimit)
                {
                    while (newnote < lowlimit)
                    {
                        newnote += 12;
                        if (newnote > highlimit)
                        {
                            discard = true;
                        }
                    }
                    tomod.midiData[1] = newnote;
                }
                else if (data1 > highlimit)
                {
                    while (newnote > highlimit)
                    {
                        newnote -= 12;
                        if (newnote < lowlimit)
                        {
                            discard = true;
                        }
                    }
                    tomod.midiData[1] = newnote;
                }
                if (status == MIDI_NOTEON)
                {
                    transposed[data1][channel] = tomod.midiData[1];
                }
            }
            else if (status == MIDI_NOTEOFF)
            {
                // always transpose noteoff by the same amount as the noteon was transposed
                tomod.midiData[1]          = transposed[data1][channel];
                transposed[data1][channel] = data1;
            }
        }
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    }
}
