#include "midiAlias.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiAlias(audioMaster);
}

MidiAliasProgram::MidiAliasProgram()
{
    // default Program Values
    fMirror = MIDI_TO_FLOAT(127);
    fZero   = 0.f;
    fShift  = 0.5f;
    fPower  = 1.0f;
    // default program name
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiAlias::MidiAlias(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams)
{
    programs = new MidiAliasProgram[numPrograms];

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
                    programs[i].fZero   = defaultBank->GetProgParm(i, 0);
                    programs[i].fShift  = defaultBank->GetProgParm(i, 0);
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
                sprintf(programs[i].name, "Program %d", i + 1);
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
MidiAlias::~MidiAlias()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiAlias::setProgram(VstInt32 program)
{
    MidiAliasProgram* ap = &programs[program];

    curProgram = program;
    setParameter(kMirror, ap->fMirror);
    setParameter(kZero, ap->fZero);
    setParameter(kShift, ap->fShift);
    setParameter(kPower, ap->fPower);
}

//------------------------------------------------------------------------
void MidiAlias::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiAlias::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiAlias::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiAlias::setParameter(VstInt32 index, float value)
{
    MidiAliasProgram* ap = &programs[curProgram];

    switch (index)
    {
        case kMirror:
            fMirror = ap->fMirror = value;
            if (fZero > fMirror)
            {
                setParameterAutomated(kZero, fMirror);
            }
            break;
        case kZero:
            fZero = ap->fZero = value;
            if (fZero > fMirror)
            {
                setParameterAutomated(kMirror, fZero);
            }
            break;
        case kShift:
            fShift = ap->fShift = value;
            break;
        case kPower:
            fPower = ap->fPower = value;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiAlias::getParameter(VstInt32 index)
{
    float v = 0;

    switch (index)
    {
        case kMirror:
            v = fMirror;
            break;
        case kZero:
            v = fZero;
            break;
        case kShift:
            v = fShift;
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
void MidiAlias::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case kMirror:
            strcpy(label, "Nyquist Note");
            break;
        case kZero:
            strcpy(label, "Zero Note");
            break;
        case kShift:
            strcpy(label, "Pre Shift");
            break;
        case kPower:
            strcpy(label, "Power");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiAlias::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case kMirror:
            sprintf(text, "%d (%s)", FLOAT_TO_MIDI(fMirror), getNoteName(FLOAT_TO_MIDI(fMirror), bottomOctave));
            break;
        case kZero:
            sprintf(text, "%d (%s)", FLOAT_TO_MIDI(fZero), getNoteName(FLOAT_TO_MIDI(fZero), bottomOctave));
            break;
        case kShift:
            sprintf(text, "%s%d", fShift > 0.5f ? "+" : "", roundToInt(fShift * 200.f) - 100);
            break;
        case kPower:
            if (fPower < 0.5f)
            {
                strcpy(text, "Off");
            }
            else
            {
                strcpy(text, "On");
            }
            break;
        default:
            break;
    }
}

void MidiAlias::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const int nyquist = FLOAT_TO_MIDI(fMirror);
    const int zero    = FLOAT_TO_MIDI(fZero);
    const int shift   = roundToInt(fShift * 200.f) - 100;

    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];
        const int status   = tomod.midiData[0] & 0xf0; // scraping  channel
        const int channel  = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        const int data1    = tomod.midiData[1] & 0x7f;
        bool discard       = false;

        if (isNoteOn(tomod) || status == MIDI_POLYKEYPRESSURE)
        {
            int newnote = data1;
            if (fPower >= 0.5f)
            {
                if (nyquist == zero)
                {
                    newnote = zero;
                }
                else
                {
                    newnote += shift;
                    int counter = 128;
                    while (newnote > nyquist || newnote < zero)
                    {
                        newnote += 2 * (nyquist - newnote);
                        if (newnote < zero)
                        {
                            newnote -= 2 * (newnote - zero);
                        }
                        counter--;
                        if (counter == 0)
                        {
                            discard = true;
                            break;
                        }
                    }
                }
                tomod.midiData[1] = (char) newnote;
            }
            if (isNoteOn(tomod))
            {
                transposed[data1][channel] = newnote;
            }
        }
        else if (isNoteOff(tomod))
        {
            // always transpose noteoff by the same amount as the noteon was transposed
            tomod.midiData[1]          = (char) transposed[data1][channel];
            transposed[data1][channel] = data1;
        }
        if (! discard)
        {
            outputs[0].push_back(tomod);
        }
    }
}
