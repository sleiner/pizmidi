#include "midiForceToKey.hpp"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiForceToKey(audioMaster);
}

MidiForceToKeyProgram::MidiForceToKeyProgram()
{
    // default Program Values
    n0         = 1.0f; // C
    n1         = 1.0f; // C#
    n2         = 1.0f; // D
    n3         = 1.0f; // D#
    n4         = 1.0f; // E
    n5         = 1.0f; // F
    n6         = 1.0f; // F#
    n7         = 1.0f; // G
    n8         = 1.0f; // G#
    n9         = 1.0f; // A
    n10        = 1.0f; // A#
    n11        = 1.0f; // B
    fMode      = (float) nearest / (float) (numModes - 1);
    fTranspose = 0.5f;
    fChannel   = CHANNEL_TO_FLOAT(-1);
    fRoot      = MIDI_TO_FLOAT(60);
    fNChannel  = CHANNEL_TO_FLOAT(-1);
    fNChMode   = 0.f;
    fUsePC     = 0.f;

    // default program name
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiForceToKey::MidiForceToKey(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiForceToKeyProgram[numPrograms];

    if (programs)
    {
        CFxBank* defaultBank = new CFxBank(numPrograms, numParams);
        if (readDefaultBank(PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < numPrograms; i++)
                {
                    n0         = defaultBank->GetProgParm(i, k0);
                    n1         = defaultBank->GetProgParm(i, k1);
                    n2         = defaultBank->GetProgParm(i, k2);
                    n3         = defaultBank->GetProgParm(i, k3);
                    n4         = defaultBank->GetProgParm(i, k4);
                    n5         = defaultBank->GetProgParm(i, k5);
                    n6         = defaultBank->GetProgParm(i, k6);
                    n7         = defaultBank->GetProgParm(i, k7);
                    n8         = defaultBank->GetProgParm(i, k8);
                    n9         = defaultBank->GetProgParm(i, k9);
                    n10        = defaultBank->GetProgParm(i, k10);
                    n11        = defaultBank->GetProgParm(i, k11);
                    fMode      = defaultBank->GetProgParm(i, kMode);
                    fTranspose = defaultBank->GetProgParm(i, kTranspose);
                    fChannel   = defaultBank->GetProgParm(i, kChannel);
                    fRoot      = defaultBank->GetProgParm(i, kRoot);
                    fNChannel  = defaultBank->GetProgParm(i, kNChannel);
                    fNChMode   = defaultBank->GetProgParm(i, kNChMode);
                    strcpy(programs[i].name, defaultBank->GetProgramName(i));
                }
            }
        }
        else
        {
            // built-in programs
            for (int i = 0; i < numPrograms; i++)
            {
                switch (i)
                {
                    case 0:
                        programs[i].n0  = 1.0f; // C
                        programs[i].n1  = 0.0f; // C#
                        programs[i].n2  = 1.0f; // D
                        programs[i].n3  = 0.0f; // D#
                        programs[i].n4  = 1.0f; // E
                        programs[i].n5  = 1.0f; // F
                        programs[i].n6  = 0.0f; // F#
                        programs[i].n7  = 1.0f; // G
                        programs[i].n8  = 0.0f; // G#
                        programs[i].n9  = 1.0f; // A
                        programs[i].n10 = 0.0f; // A#
                        programs[i].n11 = 1.0f; // B
                        sprintf(programs[i].name, "C Major");
                        break;
                    case 1:
                        programs[i].n0  = 1.0f; // C
                        programs[i].n1  = 1.0f; // C#
                        programs[i].n2  = 0.0f; // D
                        programs[i].n3  = 1.0f; // D#
                        programs[i].n4  = 0.0f; // E
                        programs[i].n5  = 1.0f; // F
                        programs[i].n6  = 1.0f; // F#
                        programs[i].n7  = 0.0f; // G
                        programs[i].n8  = 1.0f; // G#
                        programs[i].n9  = 0.0f; // A
                        programs[i].n10 = 1.0f; // A#
                        programs[i].n11 = 0.0f; // B
                        sprintf(programs[i].name, "C# Major");
                        break;
                    case 2:
                        programs[i].n0  = 0.0f; // C
                        programs[i].n1  = 1.0f; // C#
                        programs[i].n2  = 1.0f; // D
                        programs[i].n3  = 0.0f; // D#
                        programs[i].n4  = 1.0f; // E
                        programs[i].n5  = 0.0f; // F
                        programs[i].n6  = 1.0f; // F#
                        programs[i].n7  = 1.0f; // G
                        programs[i].n8  = 0.0f; // G#
                        programs[i].n9  = 1.0f; // A
                        programs[i].n10 = 0.0f; // A#
                        programs[i].n11 = 1.0f; // B
                        sprintf(programs[i].name, "D Major");
                        break;
                    case 3:
                        programs[i].n0  = 1.0f; // C
                        programs[i].n1  = 0.0f; // C#
                        programs[i].n2  = 1.0f; // D
                        programs[i].n3  = 1.0f; // D#
                        programs[i].n4  = 0.0f; // E
                        programs[i].n5  = 1.0f; // F
                        programs[i].n6  = 0.0f; // F#
                        programs[i].n7  = 1.0f; // G
                        programs[i].n8  = 1.0f; // G#
                        programs[i].n9  = 0.0f; // A
                        programs[i].n10 = 1.0f; // A#
                        programs[i].n11 = 0.0f; // B
                        sprintf(programs[i].name, "Eb Major");
                        break;
                    case 4:
                        programs[i].n0  = 0.0f; // C
                        programs[i].n1  = 1.0f; // C#
                        programs[i].n2  = 0.0f; // D
                        programs[i].n3  = 1.0f; // D#
                        programs[i].n4  = 1.0f; // E
                        programs[i].n5  = 0.0f; // F
                        programs[i].n6  = 1.0f; // F#
                        programs[i].n7  = 0.0f; // G
                        programs[i].n8  = 1.0f; // G#
                        programs[i].n9  = 1.0f; // A
                        programs[i].n10 = 0.0f; // A#
                        programs[i].n11 = 1.0f; // B
                        sprintf(programs[i].name, "E Major");
                        break;
                    case 5:
                        programs[i].n0  = 1.0f; // C
                        programs[i].n1  = 0.0f; // C#
                        programs[i].n2  = 1.0f; // D
                        programs[i].n3  = 0.0f; // D#
                        programs[i].n4  = 1.0f; // E
                        programs[i].n5  = 1.0f; // F
                        programs[i].n6  = 0.0f; // F#
                        programs[i].n7  = 1.0f; // G
                        programs[i].n8  = 0.0f; // G#
                        programs[i].n9  = 1.0f; // A
                        programs[i].n10 = 1.0f; // A#
                        programs[i].n11 = 0.0f; // B
                        sprintf(programs[i].name, "F Major");
                        break;
                    case 6:
                        programs[i].n0  = 0.0f; // C
                        programs[i].n1  = 1.0f; // C#
                        programs[i].n2  = 0.0f; // D
                        programs[i].n3  = 1.0f; // D#
                        programs[i].n4  = 0.0f; // E
                        programs[i].n5  = 1.0f; // F
                        programs[i].n6  = 1.0f; // F#
                        programs[i].n7  = 0.0f; // G
                        programs[i].n8  = 1.0f; // G#
                        programs[i].n9  = 0.0f; // A
                        programs[i].n10 = 1.0f; // A#
                        programs[i].n11 = 1.0f; // B
                        sprintf(programs[i].name, "F# Major");
                        break;
                    case 7:
                        programs[i].n0  = 1.0f; // C
                        programs[i].n1  = 0.0f; // C#
                        programs[i].n2  = 1.0f; // D
                        programs[i].n3  = 0.0f; // D#
                        programs[i].n4  = 1.0f; // E
                        programs[i].n5  = 0.0f; // F
                        programs[i].n6  = 1.0f; // F#
                        programs[i].n7  = 1.0f; // G
                        programs[i].n8  = 0.0f; // G#
                        programs[i].n9  = 1.0f; // A
                        programs[i].n10 = 0.0f; // A#
                        programs[i].n11 = 1.0f; // B
                        sprintf(programs[i].name, "G Major");
                        break;
                    case 8:
                        programs[i].n0  = 1.0f; // C
                        programs[i].n1  = 1.0f; // C#
                        programs[i].n2  = 0.0f; // D
                        programs[i].n3  = 1.0f; // D#
                        programs[i].n4  = 0.0f; // E
                        programs[i].n5  = 1.0f; // F
                        programs[i].n6  = 0.0f; // F#
                        programs[i].n7  = 1.0f; // G
                        programs[i].n8  = 1.0f; // G#
                        programs[i].n9  = 0.0f; // A
                        programs[i].n10 = 1.0f; // A#
                        programs[i].n11 = 0.0f; // B
                        sprintf(programs[i].name, "G# Major");
                        break;
                    case 9:
                        programs[i].n0  = 0.0f; // C
                        programs[i].n1  = 1.0f; // C#
                        programs[i].n2  = 1.0f; // D
                        programs[i].n3  = 0.0f; // D#
                        programs[i].n4  = 1.0f; // E
                        programs[i].n5  = 0.0f; // F
                        programs[i].n6  = 1.0f; // F#
                        programs[i].n7  = 0.0f; // G
                        programs[i].n8  = 1.0f; // G#
                        programs[i].n9  = 1.0f; // A
                        programs[i].n10 = 0.0f; // A#
                        programs[i].n11 = 1.0f; // B
                        sprintf(programs[i].name, "A Major");
                        break;
                    case 10:
                        programs[i].n0  = 1.0f; // C
                        programs[i].n1  = 0.0f; // C#
                        programs[i].n2  = 1.0f; // D
                        programs[i].n3  = 1.0f; // D#
                        programs[i].n4  = 0.0f; // E
                        programs[i].n5  = 1.0f; // F
                        programs[i].n6  = 0.0f; // F#
                        programs[i].n7  = 1.0f; // G
                        programs[i].n8  = 0.0f; // G#
                        programs[i].n9  = 1.0f; // A
                        programs[i].n10 = 1.0f; // A#
                        programs[i].n11 = 0.0f; // B
                        sprintf(programs[i].name, "Bb Major");
                        break;
                    case 11:
                        programs[i].n0  = 0.0f; // C
                        programs[i].n1  = 1.0f; // C#
                        programs[i].n2  = 0.0f; // D
                        programs[i].n3  = 1.0f; // D#
                        programs[i].n4  = 1.0f; // E
                        programs[i].n5  = 0.0f; // F
                        programs[i].n6  = 1.0f; // F#
                        programs[i].n7  = 0.0f; // G
                        programs[i].n8  = 1.0f; // G#
                        programs[i].n9  = 0.0f; // A
                        programs[i].n10 = 1.0f; // A#
                        programs[i].n11 = 1.0f; // B
                        sprintf(programs[i].name, "B Major");
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
        {
            transposed[n][c] = n;
        }
    }

    settingprog = false;
    init();
}

//-----------------------------------------------------------------------------------------
MidiForceToKey::~MidiForceToKey()
{
    if (programs)
    {
        delete[] programs;
    }
}

//------------------------------------------------------------------------
void MidiForceToKey::setProgram(VstInt32 program)
{
    MidiForceToKeyProgram* ap = &programs[program];

    settingprog = true;
    curProgram  = program;
    setParameter(k0, ap->n0);
    setParameter(k1, ap->n1);
    setParameter(k2, ap->n2);
    setParameter(k3, ap->n3);
    setParameter(k4, ap->n4);
    setParameter(k5, ap->n5);
    setParameter(k6, ap->n6);
    setParameter(k7, ap->n7);
    setParameter(k8, ap->n8);
    setParameter(k9, ap->n9);
    setParameter(k10, ap->n10);
    setParameter(k11, ap->n11);
    setParameter(kMode, ap->fMode);
    setParameter(kTranspose, ap->fTranspose);
    setParameter(kChannel, ap->fChannel);
    setParameter(kRoot, ap->fRoot);
    setParameter(kNChannel, ap->fNChannel);
    setParameter(kNChMode, ap->fNChMode);
    setParameter(kUsePC, ap->fUsePC);
    setParameter(kProg, (float) program / (float) (kNumPrograms - 1));
    settingprog = false;
}

//------------------------------------------------------------------------
void MidiForceToKey::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiForceToKey::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiForceToKey::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiForceToKey::setParameter(VstInt32 index, float value)
{
    MidiForceToKeyProgram* ap = &programs[curProgram];

    switch (index)
    {
        case k0:
            n0 = ap->n0 = value;
            break;
        case k1:
            n1 = ap->n1 = value;
            break;
        case k2:
            n2 = ap->n2 = value;
            break;
        case k3:
            n3 = ap->n3 = value;
            break;
        case k4:
            n4 = ap->n4 = value;
            break;
        case k5:
            n5 = ap->n5 = value;
            break;
        case k6:
            n6 = ap->n6 = value;
            break;
        case k7:
            n7 = ap->n7 = value;
            break;
        case k8:
            n8 = ap->n8 = value;
            break;
        case k9:
            n9 = ap->n9 = value;
            break;
        case k10:
            n10 = ap->n10 = value;
            break;
        case k11:
            n11 = ap->n11 = value;
            break;
        case kMode:
            fMode = ap->fMode = value;
            mode              = roundToInt(fMode * (float) (numModes - 1));
            break;
        case kTranspose:
            fTranspose = ap->fTranspose = value;
            break;
        case kChannel:
            fChannel = ap->fChannel = value;
            break;
        case kRoot:
            fRoot = ap->fRoot = value;
            break;
        case kNChannel:
            fNChannel = ap->fNChannel = value;
            break;
        case kNChMode:
            fNChMode = ap->fNChMode = value;
            break;
        case kUsePC:
            fUsePC = value;
            for (int i = 0; i < kNumPrograms; i++)
            {
                programs[i].fUsePC = value;
            }
            break;
        case kProg:
            fProg = value;
            if (! settingprog)
            {
                setProgram((int) (fProg * (kNumPrograms - 1)));
                updateDisplay();
            }
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiForceToKey::getParameter(VstInt32 index)
{
    float v = 0;

    switch (index)
    {
        case k0:
            v = n0;
            break;
        case k1:
            v = n1;
            break;
        case k2:
            v = n2;
            break;
        case k3:
            v = n3;
            break;
        case k4:
            v = n4;
            break;
        case k5:
            v = n5;
            break;
        case k6:
            v = n6;
            break;
        case k7:
            v = n7;
            break;
        case k8:
            v = n8;
            break;
        case k9:
            v = n9;
            break;
        case k10:
            v = n10;
            break;
        case k11:
            v = n11;
            break;
        case kMode:
            v = fMode;
            break;
        case kTranspose:
            v = fTranspose;
            break;
        case kChannel:
            v = fChannel;
            break;
        case kRoot:
            v = fRoot;
            break;
        case kNChannel:
            v = fNChannel;
            break;
        case kNChMode:
            v = fNChMode;
            break;
        case kUsePC:
            v = fUsePC;
            break;
        case kProg:
            v = fProg;
            break;
        default:
            break;
    }
    return v;
}

//-----------------------------------------------------------------------------------------
void MidiForceToKey::getParameterName(VstInt32 index, char* label)
{
    switch (index)
    {
        case k0:
            strcpy(label, "C");
            break;
        case k1:
            strcpy(label, "C#");
            break;
        case k2:
            strcpy(label, "D");
            break;
        case k3:
            strcpy(label, "D#");
            break;
        case k4:
            strcpy(label, "E");
            break;
        case k5:
            strcpy(label, "F");
            break;
        case k6:
            strcpy(label, "F#");
            break;
        case k7:
            strcpy(label, "G");
            break;
        case k8:
            strcpy(label, "G#");
            break;
        case k9:
            strcpy(label, "A");
            break;
        case k10:
            strcpy(label, "A#");
            break;
        case k11:
            strcpy(label, "B");
            break;
        case kMode:
            strcpy(label, "Mode");
            break;
        case kTranspose:
            strcpy(label, "Transpose");
            break;
        case kChannel:
            strcpy(label, "Tr.Channel");
            break;
        case kRoot:
            strcpy(label, "Root Note");
            break;
        case kNChannel:
            strcpy(label, "N.Channel");
            break;
        case kNChMode:
            strcpy(label, "N.Ch.Mode");
            break;
        case kUsePC:
            strcpy(label, "PC Listen");
            break;
        case kProg:
            strcpy(label, "Program");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiForceToKey::getParameterDisplay(VstInt32 index, char* text)
{
    switch (index)
    {
        case k0:
            if (n0 >= 0.5f)
            {
                strcpy(text, "on");
            }
            else
            {
                strcpy(text, "off");
            }
            break;
        case k1:
            if (n1 >= 0.5f)
            {
                strcpy(text, "on");
            }
            else
            {
                strcpy(text, "off");
            }
            break;
        case k2:
            if (n2 >= 0.5f)
            {
                strcpy(text, "on");
            }
            else
            {
                strcpy(text, "off");
            }
            break;
        case k3:
            if (n3 >= 0.5f)
            {
                strcpy(text, "on");
            }
            else
            {
                strcpy(text, "off");
            }
            break;
        case k4:
            if (n4 >= 0.5f)
            {
                strcpy(text, "on");
            }
            else
            {
                strcpy(text, "off");
            }
            break;
        case k5:
            if (n5 >= 0.5f)
            {
                strcpy(text, "on");
            }
            else
            {
                strcpy(text, "off");
            }
            break;
        case k6:
            if (n6 >= 0.5f)
            {
                strcpy(text, "on");
            }
            else
            {
                strcpy(text, "off");
            }
            break;
        case k7:
            if (n7 >= 0.5f)
            {
                strcpy(text, "on");
            }
            else
            {
                strcpy(text, "off");
            }
            break;
        case k8:
            if (n8 >= 0.5f)
            {
                strcpy(text, "on");
            }
            else
            {
                strcpy(text, "off");
            }
            break;
        case k9:
            if (n9 >= 0.5f)
            {
                strcpy(text, "on");
            }
            else
            {
                strcpy(text, "off");
            }
            break;
        case k10:
            if (n10 >= 0.5f)
            {
                strcpy(text, "on");
            }
            else
            {
                strcpy(text, "off");
            }
            break;
        case k11:
            if (n11 >= 0.5f)
            {
                strcpy(text, "on");
            }
            else
            {
                strcpy(text, "off");
            }
            break;
        case kMode:
            if (mode == off)
            {
                strcpy(text, "Off");
            }
            else if (mode == nearest)
            {
                strcpy(text, "Nearest");
            }
            else if (mode == alwaysup)
            {
                strcpy(text, "Always Up");
            }
            else if (mode == alwaysdown)
            {
                strcpy(text, "Always Down");
            }
            else if (mode == block)
            {
                strcpy(text, "Block");
            }
            break;
        case kTranspose:
            sprintf(text, "%d", roundToInt(fTranspose * 100) - 50);
            break;
        case kChannel:
            if (FLOAT_TO_CHANNEL016(fChannel) == 0)
            {
                strcpy(text, "None");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL016(fChannel));
            }
            break;
        case kRoot:
            strcpy(text, getNoteName(FLOAT_TO_MIDI(fRoot), bottomOctave));
            break;
        case kNChannel:
            if (FLOAT_TO_CHANNEL016(fNChannel) == 0)
            {
                strcpy(text, "None");
            }
            else
            {
                sprintf(text, "%d", FLOAT_TO_CHANNEL016(fNChannel));
            }
            break;
        case kNChMode:
            if (fNChMode >= 0.5f)
            {
                strcpy(text, "Toggle");
            }
            else
            {
                strcpy(text, "Momentary");
            }
            break;
        case kUsePC:
            if (fUsePC >= 0.5f)
            {
                strcpy(text, "Yes");
            }
            else
            {
                strcpy(text, "No");
            }
            break;
        case kProg:
            getProgramNameIndexed(0, roundToInt(fProg * (kNumPrograms - 1)), text);
            break;
        default:
            break;
    }
}

void MidiForceToKey::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const int tchannel  = FLOAT_TO_CHANNEL(fChannel);
    const int nchannel  = FLOAT_TO_CHANNEL(fNChannel);
    int transposey      = roundToInt(fTranspose * 100.f) - 50;
    bool noteswitch[12] = { n0 >= 0.5f, n1 >= 0.5f, n2 >= 0.5f, n3 >= 0.5f, n4 >= 0.5f, n5 >= 0.5f, n6 >= 0.5f, n7 >= 0.5f, n8 >= 0.5f, n9 >= 0.5f, n10 >= 0.5f, n11 >= 0.5f };

    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        VstMidiEvent tomod = inputs[0][i];
        const int status   = tomod.midiData[0] & 0xf0; // scraping  channel
        const int channel  = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        const int data1    = tomod.midiData[1] & 0x7f;
        //int data2	   = tomod.midiData[2] & 0x7f;
        bool discard = false;

        if (status == MIDI_PROGRAMCHANGE)
        {
            if (data1 < kNumPrograms && fUsePC >= 0.5f)
            {
                setProgram(data1);
                updateDisplay();
            }
        }
        //set key notes based on "note channel"
        if (channel == nchannel)
        {
            discard = true;
            if (isNoteOn(tomod))
            {
                int n = data1 % 12;
                if (fNChMode < 0.5f && ! noteswitch[n])
                {
                    noteswitch[n] = true;
                    setParameterAutomated(k0 + n, 1.f);
                }
                else
                {
                    noteswitch[n] = ! noteswitch[n];
                    setParameterAutomated(k0 + n, noteswitch[n] ? 1.f : 0.f);
                }
            }
            else if (isNoteOff(tomod) && fNChMode < 0.5f)
            {
                int n         = data1 % 12;
                noteswitch[n] = false;
                setParameterAutomated(k0 + n, 0.f);
            }
        }
        else
        {
            if (isNoteOn(tomod))
            {
                if (! noteswitch[data1 % 12])
                {
                    dbg("wrong note " << data1);
                    int transpose = 0;
                    int j         = -1;
                    switch (mode)
                    {
                            //nearest note, down when tied (same as ndc)
                        case nearest:
                            discard = true;
                            while (j < 12)
                            {
                                if (noteswitch[(data1 + j) % 12])
                                {
                                    transpose = j;
                                    discard   = false;
                                    break;
                                }
                                if (j < 0)
                                {
                                    j = -j;
                                }
                                else
                                {
                                    j = -j - 1;
                                }
                            }
                            break;
                            //always up
                        case alwaysup:
                            j       = 1;
                            discard = true;
                            while (j < 12)
                            {
                                if (noteswitch[(data1 + j) % 12])
                                {
                                    transpose = j;
                                    discard   = false;
                                    break;
                                }
                                j++;
                            }
                            break;
                            //always down
                        case alwaysdown:
                            discard = true;
                            while (j < 12)
                            {
                                if (noteswitch[(data1 + j) % 12])
                                {
                                    transpose = j;
                                    discard   = false;
                                    break;
                                }
                                j--;
                            }
                            break;
                            //block wrong notes
                        case block:
                            dbg("block note");
                            discard                    = true;
                            transposed[data1][channel] = -999;
                            break;
                        case off:
                        default:
                            break;
                    }
                    tomod.midiData[1] = data1 + transpose;
                }
                //transpose based on notes on "transpose channel"
                if (channel == tchannel)
                {
                    dbg("tchannel");
                    discard     = true;
                    int root    = FLOAT_TO_MIDI(fRoot);
                    int m       = 0;
                    int counter = 0;
                    if (tomod.midiData[1] > root)
                    {
                        while (counter < (tomod.midiData[1] - root))
                        {
                            m++;
                            if (noteswitch[(root + m) % 12])
                            {
                                counter++;
                            }
                            if (tomod.midiData[1] - m == root)
                            {
                                break;
                            }
                        }
                    }
                    else if (tomod.midiData[1] < root)
                    {
                        while (counter > (tomod.midiData[1] - root))
                        {
                            m++;
                            if (noteswitch[(root - m) % 12])
                            {
                                counter--;
                            }
                            if (tomod.midiData[1] + m == root)
                            {
                                break;
                            }
                        }
                    }
                    transposey = counter;
                    setParameterAutomated(kTranspose, ((float) (transposey + 50)) * 0.01f);
                }
            }
        }
        if (! discard)
        {
            dbg("keep event");
            if (isNoteOn(tomod) || status == MIDI_POLYKEYPRESSURE)
            {
                if (transposey > 0)
                {
                    //move the note up to the right scale degree
                    int counter = 0;
                    int m       = 0;
                    while (counter < transposey)
                    {
                        m++;
                        if (noteswitch[(tomod.midiData[1] + m) % 12])
                        {
                            counter++;
                        }
                        if ((tomod.midiData[1] + m) == 127)
                        {
                            break;
                        }
                    }
                    tomod.midiData[1] += m;
                }
                else if (transposey < 0)
                {
                    //move the note down the scale
                    int counter = 0;
                    int m       = 0;
                    while (counter > transposey)
                    {
                        m++;
                        if (noteswitch[(tomod.midiData[1] - m) % 12])
                        {
                            counter--;
                        }
                        if ((tomod.midiData[1] - m) == 0)
                        {
                            break;
                        }
                    }
                    tomod.midiData[1] -= m;
                }
                if (isNoteOn(tomod))
                {
                    transposed[data1][channel] = tomod.midiData[1];
                }
            }
            else if (isNoteOff(tomod))
            {
                dbg("noteoff " << data1 << " transposed " << transposed[data1][channel]);
                if (channel == tchannel)
                {
                    discard = true;
                }
                // always transpose noteoff by the same amount as the noteon was transposed
                if (transposed[data1][channel] == -999)
                {
                    discard = true;
                }
                else
                {
                    tomod.midiData[1] = transposed[data1][channel];
                }
                transposed[data1][channel] = data1;
            }
            if (! discard)
            {
                outputs[0].push_back(tomod);
            }
        }
    }
}
