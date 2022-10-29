#include "midiNoteMap.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiNoteMap(audioMaster);
}

void MidiNoteMap::Reset()
{
    for (int i = 0; i < 128; i++)
        setParameterAutomated(kNotes + i, MIDI_TO_FLOAT_X(i));
    setParameterAutomated(kClear, 0.4f);
    setParameterAutomated(kReset, 0.4f);
    updateDisplay();
}

void MidiNoteMap::Clear()
{
    for (int i = 0; i < 128; i++)
        setParameterAutomated(kNotes + i, MIDI_TO_FLOAT_X(-1));
    setParameterAutomated(kReset, 0.4f);
    setParameterAutomated(kClear, 0.4f);
    updateDisplay();
}

MidiNoteMapProgram::MidiNoteMapProgram()
{
    // default Program Values
    for (int i = 0; i < 128; i++)
    {
        param[kNotes + i] = MIDI_TO_FLOAT_X(i);
    }
    param[kChannel] = 0.f;
    param[kReset]   = 0.4f;
    param[kClear]   = 0.4f;
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiNoteMap::MidiNoteMap(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams),
      programs(0)
{
    programs = new MidiNoteMapProgram[numPrograms];

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
                sprintf(programs[i].name, "Program %d", i + 1);
        }
        setProgram(0);
    }

    for (int c = 0; c < 16; c++)
        for (int n = 0; n < 128; n++)
            noteOnValue[c][n] = -1;

    init();
}

//-----------------------------------------------------------------------------------------
MidiNoteMap::~MidiNoteMap()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiNoteMap::setProgram(VstInt32 program)
{
    MidiNoteMapProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
        setParameter(i, ap->param[i]);
}

//------------------------------------------------------------------------
void MidiNoteMap::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiNoteMap::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiNoteMap::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiNoteMap::setParameter(VstInt32 index, float value)
{
    MidiNoteMapProgram* ap = &programs[curProgram];
    if (index < kNumParams)
    {
        param[index] = ap->param[index] = value;
    }
}

//-----------------------------------------------------------------------------------------
float MidiNoteMap::getParameter(VstInt32 index)
{
    if (index < kNumParams)
        return param[index];
    return 0;
}

//-----------------------------------------------------------------------------------------
void MidiNoteMap::getParameterName(VstInt32 index, char* label)
{
    if (index >= kNotes)
        sprintf(label, "%s (%d) -->", getNoteName(index - kNotes, bottomOctave), index - kNotes);
    else if (index == kChannel)
        strcpy(label, "Channel");
    else if (index == kReset)
        strcpy(label, "Reset");
    else if (index == kClear)
        strcpy(label, "Clear");
}

//-----------------------------------------------------------------------------------------
void MidiNoteMap::getParameterDisplay(VstInt32 index, char* text)
{
    if (index >= kNotes)
    {
        if (FLOAT_TO_MIDI_X(param[index]) < 0)
            strcpy(text, "--");
        else
            sprintf(text, "%s (%d)", getNoteName(FLOAT_TO_MIDI_X(param[index]), bottomOctave), FLOAT_TO_MIDI_X(param[index]));
    }
    else if (index == kChannel)
    {
        sprintf(text, "%d", FLOAT_TO_CHANNEL015(param[kChannel]) + 1);
    }
    else if (index == kReset)
    {
        if (param[index] < 1.0f)
            strcpy(text, "Reset");
        else
        {
            strcpy(text, "!");
            Reset();
        }
    }
    else if (index == kClear)
    {
        if (param[index] < 1.0f)
            strcpy(text, "Clear");
        else
        {
            strcpy(text, "!");
            Clear();
        }
    }
}

void MidiNoteMap::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    int listenChannel = FLOAT_TO_CHANNEL015(param[kChannel]);
    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];
        const int status   = tomod.midiData[0] & 0xf0; // scraping  channel
        const int channel  = tomod.midiData[0] & 0x0f; // isolating channel (1-16)
        const int note     = tomod.midiData[1] & 0x7f;
        //const int velocity = tomod.midiData[2] & 0x7f;

        bool discard = false;

        if (status < 0xF0)
        {
            if (channel == listenChannel)
            {
                if (isNoteOn(tomod))
                {
                    const int newnote = FLOAT_TO_MIDI_X(param[note - kNotes]);
                    if (newnote > -1)
                        tomod.midiData[1] = newnote;
                    else
                        discard = true;
                    noteOnValue[channel][note] = newnote;
                }
                else if (isNoteOff(tomod))
                {
                    //send same value as noteon
                    if (noteOnValue[channel][note] > -1)
                        tomod.midiData[1] = noteOnValue[channel][note];
                    else
                        discard = true;
                    noteOnValue[channel][note] = -1;
                }
            }
        }
        if (! discard)
            outputs[0].push_back(tomod);
    }
}
