#include "midiInvertNotes.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiInvertNotes(audioMaster);
}

MidiInvertNotesProgram::MidiInvertNotesProgram()
{
    // default Program Values
    fMirror = MIDI_TO_FLOAT(60);
    fPower  = 1.0f;
    // default program name
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiInvertNotes::MidiInvertNotes(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiInvertNotesProgram[numPrograms];

    if (programs)
    {
        CFxBank* defaultBank = new CFxBank(kNumPrograms, kNumParams);
        if (readDefaultBank(PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
                {
                    programs[i].fMirror = defaultBank->GetProgParm(i, 0);
                    programs[i].fPower  = defaultBank->GetProgParm(i, 1);
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
                        programs[i].fMirror = MIDI_TO_FLOAT(60);
                        sprintf(programs[i].name, "Middle C");
                        break;
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
            transposed[n][c] = n;
    }

    init();
}

//-----------------------------------------------------------------------------------------
MidiInvertNotes::~MidiInvertNotes()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiInvertNotes::setProgram(VstInt32 program)
{
    MidiInvertNotesProgram* ap = &programs[program];

    curProgram = program;
    setParameter(kMirror, ap->fMirror);
    setParameter(kPower, ap->fPower);
}

//------------------------------------------------------------------------
void MidiInvertNotes::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiInvertNotes::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiInvertNotes::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiInvertNotes::setParameter(VstInt32 index, float value)
{
    MidiInvertNotesProgram* ap = &programs[curProgram];

    switch (index)
    {
        case kMirror:
            fMirror = ap->fMirror = value;
            break;
        case kPower:
            fPower = ap->fPower = value;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiInvertNotes::getParameter(VstInt32 index)
{
    float v = 0;

    switch (index)
    {
        case kMirror:
            v = fMirror;
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
void MidiInvertNotes::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kMirror:
            strcpy(label, "Center Note");
            break;
        case kPower:
            strcpy(label, "Power");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiInvertNotes::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kMirror:
            strcpy(text, getNoteName(FLOAT_TO_MIDI(fMirror), bottomOctave));
            break;
        case kPower:
            if (fPower < 0.5f)
                strcpy(text, "off");
            else
                strcpy(text, "on");
            break;
        default:
            break;
    }
}

void MidiInvertNotes::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];
        short status       = tomod.midiData[0] & 0xf0;       // scraping  channel
        short channel      = (tomod.midiData[0] & 0x0f) + 1; // isolating channel (1-16)
        short data1        = tomod.midiData[1] & 0x7f;
        short data2        = tomod.midiData[2] & 0x7f;
        if (status == MIDI_NOTEON && data2 == 0)
            status = MIDI_NOTEOFF;

        short mirror = FLOAT_TO_MIDI(fMirror);
        if (fPower >= 0.5f)
        {
            if (status == MIDI_NOTEON || status == MIDI_POLYKEYPRESSURE)
            {
                int newnote = data1 + 2 * (mirror - data1);
                if (newnote > 127)
                    newnote = 127;
                if (newnote < 0)
                    newnote = 0;
                tomod.midiData[1] = newnote;
                if (status == MIDI_NOTEON)
                    transposed[data1][channel] = newnote;
            }
        }
        if (status == MIDI_NOTEOFF)
        {
            // always transpose noteoff by the same amount as the noteon was transposed
            tomod.midiData[1]          = (char) transposed[data1][channel];
            transposed[data1][channel] = data1;
        }
        outputs[0].push_back(tomod);
    }
}
