/*-----------------------------------------------------------------------------
midiScaleChanger 1.0
by Reuben Vinal
based on:
MIDI Gain 1.02
David Viens
davidv@plogue.com
-----------------------------------------------------------------------------*/

#include "midiScaleChanger.hpp"
#include "../common/MIDI.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiScaleChanger (audioMaster);
}

MidiScaleChangerProgram::MidiScaleChangerProgram()
{
    // default Program Values
    n0          = 0.5f; // C
    n1          = 0.f;  // C#
    n2          = 0.5f; // D
    n3          = 0.f;  // D#
    n4          = 0.5f; // E
    n5          = 0.5f; // F
    n6          = 0.f;  // F#
    n7          = 0.5f; // G
    n8          = 0.f;  // G#
    n9          = 0.5f; // A
    n10         = 0.f;  // A#
    n11         = 0.5f; // B
    fRoot       = 0.5f;
    fWrap       = 0.f;
    fChannel    = CHANNEL_TO_FLOAT (-1);
    fAltChannel = CHANNEL_TO_FLOAT (-1);

    // default program name
    strcpy (name, "Default");
}

//-----------------------------------------------------------------------------
MidiScaleChanger::MidiScaleChanger (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams), programs (0)
{
    programs = new MidiScaleChangerProgram[numPrograms];

    if (programs)
    {
        CFxBank* defaultBank = new CFxBank (numPrograms, numParams);
        if (readDefaultBank (PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < numPrograms; i++)
                {
                    n0          = defaultBank->GetProgParm (i, k0);
                    n1          = defaultBank->GetProgParm (i, k1);
                    n2          = defaultBank->GetProgParm (i, k2);
                    n3          = defaultBank->GetProgParm (i, k3);
                    n4          = defaultBank->GetProgParm (i, k4);
                    n5          = defaultBank->GetProgParm (i, k5);
                    n6          = defaultBank->GetProgParm (i, k6);
                    n7          = defaultBank->GetProgParm (i, k7);
                    n8          = defaultBank->GetProgParm (i, k8);
                    n9          = defaultBank->GetProgParm (i, k9);
                    n10         = defaultBank->GetProgParm (i, k10);
                    n11         = defaultBank->GetProgParm (i, k11);
                    fRoot       = defaultBank->GetProgParm (i, kRoot);
                    fWrap       = defaultBank->GetProgParm (i, kWrap);
                    fChannel    = defaultBank->GetProgParm (i, kChannel);
                    fAltChannel = defaultBank->GetProgParm (i, kAltChannel);
                    strcpy (programs[i].name, defaultBank->GetProgramName (i));
                }
            }
        }
        else
        {
            for (int i = 0; i < numPrograms; i++)
            {
                sprintf (programs[i].name, "Program %d", i + 1);
            }
        }
        setProgram (0);
    }

    for (int n = 0; n < 128; n++)
    {
        for (int c = 0; c < 16; c++)
        {
            transposed[n][c]    = n;
            noteOnChannel[n][c] = c;
        }
    }

    settingprog = false;
    init();
}

//-----------------------------------------------------------------------------------------
MidiScaleChanger::~MidiScaleChanger()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiScaleChanger::setProgram (VstInt32 program)
{
    MidiScaleChangerProgram* ap = &programs[program];

    settingprog = true;
    curProgram  = program;
    setParameter (k0, ap->n0);
    setParameter (k1, ap->n1);
    setParameter (k2, ap->n2);
    setParameter (k3, ap->n3);
    setParameter (k4, ap->n4);
    setParameter (k5, ap->n5);
    setParameter (k6, ap->n6);
    setParameter (k7, ap->n7);
    setParameter (k8, ap->n8);
    setParameter (k9, ap->n9);
    setParameter (k10, ap->n10);
    setParameter (k11, ap->n11);
    setParameter (kRoot, ap->fRoot);
    setParameter (kWrap, ap->fWrap);
    setParameter (kChannel, ap->fChannel);
    setParameter (kAltChannel, ap->fAltChannel);
    setParameter (kProg, (float) program / (float) (kNumPrograms - 1));
    settingprog = false;
}

//------------------------------------------------------------------------
void MidiScaleChanger::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiScaleChanger::getProgramName (char* name)
{
    strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiScaleChanger::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy (text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiScaleChanger::setParameter (VstInt32 index, float value)
{
    MidiScaleChangerProgram* ap = &programs[curProgram];

    switch (index)
    {
        case k0:
            n0 = ap->n0  = value;
            notetable[0] = roundToInt (value * 25.f) - 13;
            break;
        case k1:
            n1 = ap->n1  = value;
            notetable[1] = roundToInt (value * 25.f) - 13;
            break;
        case k2:
            n2 = ap->n2  = value;
            notetable[2] = roundToInt (value * 25.f) - 13;
            break;
        case k3:
            n3 = ap->n3  = value;
            notetable[3] = roundToInt (value * 25.f) - 13;
            break;
        case k4:
            n4 = ap->n4  = value;
            notetable[4] = roundToInt (value * 25.f) - 13;
            break;
        case k5:
            n5 = ap->n5  = value;
            notetable[5] = roundToInt (value * 25.f) - 13;
            break;
        case k6:
            n6 = ap->n6  = value;
            notetable[6] = roundToInt (value * 25.f) - 13;
            break;
        case k7:
            n7 = ap->n7  = value;
            notetable[7] = roundToInt (value * 25.f) - 13;
            break;
        case k8:
            n8 = ap->n8  = value;
            notetable[8] = roundToInt (value * 25.f) - 13;
            break;
        case k9:
            n9 = ap->n9  = value;
            notetable[9] = roundToInt (value * 25.f) - 13;
            break;
        case k10:
            n10 = ap->n10 = value;
            notetable[10] = roundToInt (value * 25.f) - 13;
            break;
        case k11:
            n11 = ap->n11 = value;
            notetable[11] = roundToInt (value * 25.f) - 13;
            break;
        case kRoot:
            root  = roundToInt (value * 96.f) - 48;
            fRoot = ap->fRoot = value;
            updateDisplay();
            break;
        case kWrap:
            wrap  = value >= 0.5f;
            fWrap = ap->fWrap = value;
            break;
        case kChannel:
            fChannel = ap->fChannel = value;
            break;
        case kAltChannel:
            fAltChannel = ap->fAltChannel = value;
            break;
        case kProg:
            fProg = value;
            if (! settingprog)
            {
                setProgram ((int) (fProg * (kNumPrograms - 1)));
                updateDisplay();
            }
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiScaleChanger::getParameter (VstInt32 index)
{
    switch (index)
    {
        case k0:
            return n0;
        case k1:
            return n1;
        case k2:
            return n2;
        case k3:
            return n3;
        case k4:
            return n4;
        case k5:
            return n5;
        case k6:
            return n6;
        case k7:
            return n7;
        case k8:
            return n8;
        case k9:
            return n9;
        case k10:
            return n10;
        case k11:
            return n11;
        case kRoot:
            return fRoot;
        case kWrap:
            return fWrap;
        case kChannel:
            return fChannel;
        case kAltChannel:
            return fAltChannel;
        case kProg:
            return fProg;
        default:
            return 0.f;
    }
}

//-----------------------------------------------------------------------------------------
void MidiScaleChanger::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case k0:
        case k1:
        case k2:
        case k3:
        case k4:
        case k5:
        case k6:
        case k7:
        case k8:
        case k9:
        case k10:
        case k11:
            strcpy (label, getNoteNameWithoutOctave (index));
            break;
        case kRoot:
            strcpy (label, "Root Note");
            break;
        case kWrap:
            strcpy (label, "OctaveWrap");
            break;
        case kChannel:
            strcpy (label, "OutChannel");
            break;
        case kAltChannel:
            strcpy (label, "AltChannel");
            break;
        case kProg:
            strcpy (label, "Program");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiScaleChanger::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case k0:
        case k1:
        case k2:
        case k3:
        case k4:
        case k5:
        case k6:
        case k7:
        case k8:
        case k9:
        case k10:
        case k11:
            if (notetable[index] == -13)
                strcpy (text, "--");
            else if (notetable[index] < 0)
            {
                sprintf (text, "%d (%s)", notetable[index], getNoteNameWithoutOctave (midiNoteWrap (root + index + notetable[index])));
            }
            else
            {
                sprintf (text, "+%d (%s)", notetable[index], getNoteNameWithoutOctave (midiNoteWrap (root + index + notetable[index])));
            }
            break;
        case kRoot:
            if (root < 0)
                sprintf (text, "%d (%s)", root, getNoteNameWithoutOctave (midiNoteWrap (root)));
            else
                sprintf (text, "+%d (%s)", root, getNoteNameWithoutOctave (midiNoteWrap (root)));
            break;
        case kWrap:
            if (wrap)
                strcpy (text, "On");
            else
                strcpy (text, "Off");
            break;
        case kChannel:
            if (FLOAT_TO_CHANNEL016 (fChannel) == 0)
                strcpy (text, "As Input");
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL016 (fChannel));
            break;
        case kAltChannel:
            if (FLOAT_TO_CHANNEL016 (fAltChannel) == 0)
                strcpy (text, "None");
            else
                sprintf (text, "%d", FLOAT_TO_CHANNEL016 (fAltChannel));
            break;
        case kProg:
            getProgramNameIndexed (0, roundToInt (fProg * (kNumPrograms - 1)), text);
            break;
        default:
            break;
    }
}

void MidiScaleChanger::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    const int nchannel = FLOAT_TO_CHANNEL (fAltChannel);

    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        VstMidiEvent tomod = inputs[0][i];
        const int status   = tomod.midiData[0] & 0xf0; // scraping  channel
        const int channel  = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        const int data1    = tomod.midiData[1] & 0x7f;
        bool discard       = false;
        int tchannel       = FLOAT_TO_CHANNEL (fChannel);
        if (tchannel == -1)
            tchannel = channel;

        if (isNoteOn (tomod))
        {
            int n = data1 % 12;
            if (notetable[n] == -13)
            {
                if (nchannel == -1)
                {
                    discard                    = true;
                    transposed[data1][channel] = -999;
                }
                else
                {
                    tomod.midiData[0] = MIDI_NOTEON | nchannel;
                }
            }
            else
            {
                int newdata1 = root + data1 + notetable[n];
                if (newdata1 > 127 || newdata1 < 0)
                {
                    if (wrap)
                    {
                        newdata1                   = midiNoteWrap (newdata1);
                        transposed[data1][channel] = newdata1;
                        tomod.midiData[0]          = MIDI_NOTEON | tchannel;
                        tomod.midiData[1]          = newdata1;
                    }
                    else
                    {
                        discard                    = true;
                        transposed[data1][channel] = -999;
                    }
                }
                else
                {
                    transposed[data1][channel] = newdata1;
                    tomod.midiData[0]          = MIDI_NOTEON | tchannel;
                    tomod.midiData[1]          = (char) newdata1;
                }
            }
            noteOnChannel[data1][channel] = tomod.midiData[0] & 0x0f;
        }
        else if (isNoteOff (tomod))
        {
            // always transpose noteoff by the same amount that the noteon was transposed by (and send on the same channel)
            if (transposed[data1][channel] == -999)
                discard = true;
            else
            {
                tomod.midiData[0] = MIDI_NOTEOFF | noteOnChannel[data1][channel];
                tomod.midiData[1] = transposed[data1][channel];
            }
            transposed[data1][channel]    = data1;
            noteOnChannel[data1][channel] = channel;
        }
        else if (status == MIDI_POLYKEYPRESSURE)
        {
            int n        = data1 % 12;
            int newdata1 = root + data1 + notetable[n];
            if (newdata1 > 127 || newdata1 < 0)
            {
                if (wrap)
                {
                    newdata1          = midiNoteWrap (newdata1);
                    tomod.midiData[0] = MIDI_NOTEON | tchannel;
                    tomod.midiData[1] = newdata1;
                }
                else
                {
                    discard = true;
                }
            }
            else
            {
                tomod.midiData[0] = MIDI_NOTEON | tchannel;
                tomod.midiData[1] = (char) newdata1;
            }
        }
        if (! discard)
            outputs[0].push_back (tomod);
    }
}
