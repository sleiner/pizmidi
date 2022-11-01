#include "midiPitchBendToNotes.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiPitchBendToNotes(audioMaster);
}

MidiPitchBendToNotesProgram::MidiPitchBendToNotesProgram()
{
    // default Program Values
    fChannel = CHANNEL_TO_FLOAT(0);
    fPower   = 1.f;

    vst_strncpy(name, "Pitch Bend To Notes", kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------
MidiPitchBendToNotes::MidiPitchBendToNotes(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiPitchBendToNotesProgram[numPrograms];

    if (programs)
    {
        CFxBank* defaultBank = new CFxBank(numPrograms, numParams);
        if (readDefaultBank(PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < numPrograms; i++)
                {
                    programs[i].fChannel = defaultBank->GetProgParm(i, 0);
                    programs[i].fPower   = defaultBank->GetProgParm(i, 1);
                    strcpy(programs[i].name, defaultBank->GetProgramName(i));
                }
            }
        }
        else
        {
            // built-in programs
            for (int i = 0; i < numPrograms; i++)
            {
                sprintf(programs[i].name, "Program %d", i + 1);
            }
        }
        setProgram(0);
    }

    pbrange = 24.f;
    for (int ch = 0; ch < 16; ch++)
    {
        transpose[ch] = 0;
        rpn[ch]       = -999;
        rpncoarse[ch] = -999;
        for (int n = 0; n < 128; n++)
        {
            lastTranspose[n][ch] = NOT_PLAYING;
        }
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiPitchBendToNotes::~MidiPitchBendToNotes()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiPitchBendToNotes::setProgram(VstInt32 program)
{
    MidiPitchBendToNotesProgram* ap = &programs[program];

    curProgram = program;
    setParameter(kChannel, ap->fChannel);
    setParameter(kPower, ap->fPower);
}

//------------------------------------------------------------------------
void MidiPitchBendToNotes::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiPitchBendToNotes::getProgramName(char* name)
{
    vst_strncpy(name, programs[curProgram].name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool MidiPitchBendToNotes::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < numPrograms)
    {
        vst_strncpy(text, programs[index].name, kVstMaxProgNameLen);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiPitchBendToNotes::setParameter(VstInt32 index, float value)
{
    MidiPitchBendToNotesProgram* ap = &programs[curProgram];

    switch (index)
    {
        case kChannel:
            fChannel = ap->fChannel = value;
            break;
        case kPower:
            fPower = ap->fPower = value;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiPitchBendToNotes::getParameter(VstInt32 index)
{
    float v = 0;

    switch (index)
    {
        case kChannel:
            v = fChannel;
            break;
        case kPower:
            v = fPower;
            break;
        default:
            break;
    }
    return v;
}

//-----------------------------------------------------------------------------------------
void MidiPitchBendToNotes::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kChannel:
            vst_strncpy(label, "Channel", kVstMaxParamStrLen);
            break;
        case kPower:
            vst_strncpy(label, "Power", kVstMaxParamStrLen);
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiPitchBendToNotes::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kChannel:
            if (FLOAT_TO_CHANNEL(fChannel) == ANY_CHANNEL)
            {
                vst_strncpy(text, "No Change", kVstMaxParamStrLen);
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL(fChannel) + 1);
            }
            break;
        case kPower:
            if (fPower < 0.5f)
            {
                vst_strncpy(text, "Off", kVstMaxParamStrLen);
            }
            else
            {
                vst_strncpy(text, "On", kVstMaxParamStrLen);
            }
            break;
        default:
            break;
    }
}

void MidiPitchBendToNotes::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    if (fPower >= 0.5f)
    {
        // process incoming events
        for (unsigned int i = 0; i < inputs[0].size(); i++)
        {
            //copying event "i" from input (with all its fields)
            VstMidiEvent tomod     = inputs[0][i];
            unsigned char status   = tomod.midiData[0] & 0xf0; // scraping  channel
            const char in_channel  = tomod.midiData[0] & 0x0f; // isolating channel
            const char data1       = tomod.midiData[1] & 0x7f;
            const char data2       = tomod.midiData[2] & 0x7f;
            const char out_channel = (char) FLOAT_TO_CHANNEL(fChannel);

            if ((status == MIDI_NOTEON) && (data2 == 0))
            {
                status = MIDI_NOTEOFF;
            }

            if (status == MIDI_PITCHBEND)
            {
                if (in_channel == 0) //"actual" pitch wheel
                {
                    tomod.midiData[0] = out_channel | MIDI_PITCHBEND;
                    outputs[0].push_back(tomod);
                }
                else
                {
                    transpose[in_channel] = roundToInt(pbrange * (float) (CombineBytes(data1, data2) - 8192) / 16383.f);
                }
                //for (int i=0;i<128;i++)
                //{
                //	if (NOT_PLAYING != lastTranspose[i][in_channel])
                //	{
                //		if (lastTranspose[i][in_channel] != transpose[in_channel])
                //		{
                //			tomod.midiData[0] = out_channel | MIDI_NOTEOFF;
                //			tomod.midiData[1] = data1 + lastTranspose[i][in_channel];
                //			tomod.midiData[2] = 0;
                //			outputs[0].push_back(tomod);

                //			lastTranspose[i][in_channel] = transpose[in_channel];
                //			tomod.midiData[0] = out_channel | MIDI_NOTEON;
                //			tomod.midiData[1] = data1 + transpose[in_channel];
                //			tomod.midiData[2] = 64;
                //			outputs[0].push_back(tomod);
                //		}
                //	}
                //}
            }
            else if (status == MIDI_NOTEON)
            {
                lastTranspose[data1][in_channel] = transpose[in_channel];
                tomod.midiData[0]                = out_channel | MIDI_NOTEON;
                tomod.midiData[1]                = (data1 + transpose[in_channel]) & 0x7f;
                outputs[0].push_back(tomod);
            }
            else if (status == MIDI_NOTEOFF)
            {
                tomod.midiData[0] = out_channel | MIDI_NOTEOFF;
                tomod.midiData[1] = (data1 + lastTranspose[data1][in_channel]) & 0x7f;
                outputs[0].push_back(tomod);
                lastTranspose[data1][in_channel] = NOT_PLAYING;
            }
            else if (status == MIDI_CONTROLCHANGE)
            {
                if (data1 == MIDI_ALL_NOTES_OFF)
                {
                    for (int n = 0; n < 128; n++)
                    {
                        lastTranspose[n][in_channel] = NOT_PLAYING;
                    }
                }
                else if (data1 == 101)
                {
                    rpncoarse[in_channel] = data2;
                }
                else if (data1 == 100 && rpncoarse[in_channel] >= 0)
                {
                    rpn[in_channel] = data2 | (rpncoarse[in_channel] << 7);
                }
                else if (rpn[in_channel] == 0)
                {
                    if (data1 == 6)
                    { //data entry slider (coarse)
                        pbrange = (float) (data2 * 2);
                    }
                }
                tomod.midiData[0] = out_channel | status;
                outputs[0].push_back(tomod);
            }
            else
            {
                tomod.midiData[0] = out_channel | status;
                outputs[0].push_back(tomod);
            }
        }
    }
}
