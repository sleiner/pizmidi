#include "midiTranspose.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiTranspose(audioMaster);
}

MidiTransposeProgram::MidiTransposeProgram()
{
    // default Program Values
    for (int i = 0; i < kNumParams; i++)
    {
        switch (i)
        {
            case kTranspose:
                param[i] = 0.5f;
                break;
            case kRange:
                param[i] = 0.5f;
                break;
            case kInChannel:
                param[i] = CHANNEL_TO_FLOAT(ANY_CHANNEL);
                break;
            case kImmediate:
                param[i] = 0.f;
                break;

            default:
                param[i] = 0.f;
                break;
        }
    }
    // default program name
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiTranspose::MidiTranspose(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiTransposeProgram[numPrograms];

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
                sprintf(programs[i].name, "Program %d", i + 1);
            }
        }
        setProgram(0);
    }

    for (int c = 0; c < 16; c++)
    {
        for (int i = 0; i < 128; i++)
        {
            transposed[c][i] = -999;
            velocity[c][i]   = 0;
        }
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiTranspose::~MidiTranspose()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiTranspose::setProgram(VstInt32 program)
{
    MidiTransposeProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter(i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiTranspose::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiTranspose::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiTranspose::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiTranspose::setParameter(VstInt32 index, float value)
{
    MidiTransposeProgram* ap = &programs[curProgram];
    param[index] = ap->param[index] = value;
    //switch(index) {
    //   default: break;
    //}
}

//-----------------------------------------------------------------------------------------
float MidiTranspose::getParameter(VstInt32 index)
{
    return param[index];
}

//-----------------------------------------------------------------------------------------
void MidiTranspose::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kInChannel:
            strcpy(label, "InChannel");
            break;
        case kRange:
            strcpy(label, "Range");
            break;
        case kImmediate:
            strcpy(label, "Immediate");
            break;
        case kTranspose:
            strcpy(label, "Transpose");
            break;
        case kUsePB:
            strcpy(label, "PitchBend");
            break;
        case kUseCC:
            strcpy(label, "Transp CC");
            break;
        default:
            sprintf(label, "param %d", index);
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiTranspose::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kInChannel:
            if (FLOAT_TO_CHANNEL(param[index]) == -1)
            {
                strcpy(text, "All");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL(param[index]) + 1);
            }
            break;
        case kTranspose:
            sprintf(text, "%d", getTranspose());
            if (getTranspose() > 0)
            {
                sprintf(text, "+%d", getTranspose());
            }
            else
            {
                sprintf(text, "%d", getTranspose());
            }
            break;
        case kRange:
            sprintf(text, "± %d", roundToInt(MAX_TRANSPOSE * param[index]));
            break;
        case kUsePB:
            if (param[index] < 0.5f)
            {
                strcpy(text, "Thru");
            }
            else
            {
                strcpy(text, "Use");
            }
            break;
        case kImmediate:
            if (param[index] < 0.5f)
            {
                strcpy(text, "Off");
            }
            else
            {
                strcpy(text, "On");
            }
            break;
        case kUseCC:
            if (FLOAT_TO_MIDI_X(param[index]) == -1)
            {
                strcpy(text, "Off");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_MIDI_X(param[index]));
            }
            break;
        default:
            sprintf(text, "%f", param[index]);
            break;
    }
}

void MidiTranspose::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const int ch       = FLOAT_TO_CHANNEL(param[kInChannel]);
    const bool split   = param[kImmediate] >= 0.5f;
    const bool usePB   = param[kUsePB] >= 0.5f;
    const int transpCC = FLOAT_TO_MIDI_X(param[kUseCC]);
    int transp         = getTranspose();

    if (split && transp != lastTranspose)
    {
        for (int c = 0; c < 16; c++)
        {
            for (int n = 0; n < 128; n++)
            {
                if (velocity[c][n])
                {
                    VstMidiEvent noteoff;
                    noteoff.midiData[0] = MIDI_NOTEOFF | c;
                    noteoff.midiData[1] = midiLimit(n + transposed[c][n]);
                    noteoff.midiData[2] = 0;
                    noteoff.deltaFrames = 0;
                    outputs[0].push_back(noteoff);

                    VstMidiEvent noteon;
                    noteon.midiData[0] = MIDI_NOTEON | c;
                    noteon.midiData[1] = midiLimit(n + transp);
                    noteon.midiData[2] = velocity[c][n];
                    noteon.deltaFrames = 0;
                    transposed[c][n]   = transp;
                    outputs[0].push_back(noteon);
                }
            }
        }
    }
    lastTranspose = transp;

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
        {
            status = MIDI_NOTEOFF;
        }

        if (channel == ch || ch == ANY_CHANNEL)
        {
            if (status == MIDI_NOTEON)
            {
                int note = data1 + transp;
                if (note < 128 && note >= 0)
                {
                    tomod.midiData[1] = data1 + transp;
                    outputs[0].push_back(tomod);
                }
                velocity[channel][data1]   = tomod.midiData[2];
                transposed[channel][data1] = transp;
            }
            else if (status == MIDI_NOTEOFF)
            {
                int note = data1 + transposed[channel][data1];
                if (note < 128 && note >= 0)
                {
                    tomod.midiData[1] = data1 + transposed[channel][data1];
                    outputs[0].push_back(tomod);
                }
                velocity[channel][data1]   = 0;
                transposed[channel][data1] = -999;
            }
            else if (status == MIDI_POLYKEYPRESSURE)
            {
                int note = data1 + transp;
                if (note < 128 && note >= 0)
                {
                    tomod.midiData[1] = data1 + transp;
                    outputs[0].push_back(tomod);
                }
            }
            else if (status == MIDI_CONTROLCHANGE && transpCC == tomod.midiData[1]
                     || status == MIDI_PITCHBEND && usePB)
            {
                if (status == MIDI_CONTROLCHANGE)
                {
                    setParameterAutomated(kTranspose, MIDI_TO_FLOAT(tomod.midiData[2]));
                    outputs[0].push_back(tomod);
                }
                else if (status == MIDI_PITCHBEND)
                {
                    float bend = (float) CombineBytes(tomod.midiData[1], tomod.midiData[2]) / 16383.f;
                    setParameterAutomated(kTranspose, bend);
                }
                transp = getTranspose();
                if (split && transp != lastTranspose)
                {
                    for (int c = 0; c < 16; c++)
                    {
                        for (int n = 0; n < 128; n++)
                        {
                            if (velocity[c][n])
                            {
                                VstMidiEvent noteoff;
                                noteoff.midiData[0] = MIDI_NOTEOFF | c;
                                noteoff.midiData[1] = midiLimit(n + transposed[c][n]);
                                noteoff.midiData[2] = 0;
                                noteoff.deltaFrames = tomod.deltaFrames;
                                outputs[0].push_back(noteoff);

                                VstMidiEvent noteon;
                                noteon.midiData[0] = MIDI_NOTEON | c;
                                noteon.midiData[1] = midiLimit(n + transp);
                                noteon.midiData[2] = velocity[c][n];
                                noteon.deltaFrames = tomod.deltaFrames;
                                transposed[c][n]   = transp;
                                outputs[0].push_back(noteon);
                            }
                        }
                    }
                }
                lastTranspose = transp;
            }
            else
            {
                outputs[0].push_back(tomod);
            }
        }
    }
}
