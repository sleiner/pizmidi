/*-----------------------------------------------------------------------------
midiVelocityScale 1.0
by Reuben Vinal
based on:
  MIDI Gain 1.02
  David Viens
  davidv@plogue.com
-----------------------------------------------------------------------------*/

#include "midiVelocityScale.hpp"
#include "../common/MIDI.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiVelocityScale (audioMaster);
}

MidiVelocityScaleProgram::MidiVelocityScaleProgram()
{
    // default Program Values
    fVel1 = MIDI_TO_FLOAT (1);
    fVel2 = MIDI_TO_FLOAT (127);
    fOffset = MIDI_TO_FLOAT (63);
    fOffVel1 = MIDI_TO_FLOAT (0);
    fOffVel2 = MIDI_TO_FLOAT (127);
    fOffOffset = MIDI_TO_FLOAT (63);
    // default program name
    strcpy (name, "Default");
}

//-----------------------------------------------------------------------------
MidiVelocityScale::MidiVelocityScale (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams)
{
    programs = new MidiVelocityScaleProgram[numPrograms];

    if (programs)
    {
        CFxBank* defaultBank = new CFxBank (kNumPrograms, kNumParams);
        if (readDefaultBank (PLUG_NAME, defaultBank))
        {
            if ((VstInt32) defaultBank->GetFxID() == PLUG_IDENT)
            {
                for (int i = 0; i < kNumPrograms; i++)
                {
                    programs[i].fVel1 = defaultBank->GetProgParm (i, 0);
                    programs[i].fVel2 = defaultBank->GetProgParm (i, 1);
                    programs[i].fOffset = defaultBank->GetProgParm (i, 2);
                    programs[i].fOffVel1 = defaultBank->GetProgParm (i, 3);
                    programs[i].fOffVel2 = defaultBank->GetProgParm (i, 4);
                    programs[i].fOffOffset = defaultBank->GetProgParm (i, 5);
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
MidiVelocityScale::~MidiVelocityScale()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiVelocityScale::setProgram (VstInt32 program)
{
    MidiVelocityScaleProgram* ap = &programs[program];

    curProgram = program;
    setParameter (kVel1, ap->fVel1);
    setParameter (kVel2, ap->fVel2);
    setParameter (kOffset, ap->fOffset);
    setParameter (kOffVel1, ap->fOffVel1);
    setParameter (kOffVel2, ap->fOffVel2);
    setParameter (kOffOffset, ap->fOffOffset);
}

//------------------------------------------------------------------------
void MidiVelocityScale::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiVelocityScale::getProgramName (char* name)
{
    strcpy (name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiVelocityScale::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy (text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiVelocityScale::setParameter (VstInt32 index, float value)
{
    MidiVelocityScaleProgram* ap = &programs[curProgram];

    switch (index)
    {
        case kVel1:
            fVel1 = ap->fVel1 = value;
            break;
        case kVel2:
            fVel2 = ap->fVel2 = value;
            break;
        case kOffset:
            fOffset = ap->fOffset = value;
            break;
        case kOffVel1:
            fOffVel1 = ap->fOffVel1 = value;
            break;
        case kOffVel2:
            fOffVel2 = ap->fOffVel2 = value;
            break;
        case kOffOffset:
            fOffOffset = ap->fOffOffset = value;
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
float MidiVelocityScale::getParameter (VstInt32 index)
{
    float v = 0;

    switch (index)
    {
        case kVel1:
            v = fVel1;
            break;
        case kVel2:
            v = fVel2;
            break;
        case kOffset:
            v = fOffset;
            break;
        case kOffVel1:
            v = fOffVel1;
            break;
        case kOffVel2:
            v = fOffVel2;
            break;
        case kOffOffset:
            v = fOffOffset;
            break;
        default:
            break;
    }
    return v;
}

//-----------------------------------------------------------------------------------------
void MidiVelocityScale::getParameterName (VstInt32 index, char* label)
{
    switch (index)
    {
        case kVel1:
            strcpy (label, "Velocity 1");
            break;
        case kVel2:
            strcpy (label, "Velocity 2");
            break;
        case kOffset:
            strcpy (label, "Offset");
            break;
        case kOffVel1:
            strcpy (label, "ReleaseVel1");
            break;
        case kOffVel2:
            strcpy (label, "ReleaseVel2");
            break;
        case kOffOffset:
            strcpy (label, "Rel.Offset");
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------
void MidiVelocityScale::getParameterDisplay (VstInt32 index, char* text)
{
    switch (index)
    {
        case kVel1:
            if (FLOAT_TO_MIDI (fVel1) == 0)
                sprintf (text, "%d", 1);
            else
                sprintf (text, "%d", FLOAT_TO_MIDI (fVel1));
            break;
        case kVel2:
            if (FLOAT_TO_MIDI (fVel2) == 0)
                sprintf (text, "%d", 1);
            else
                sprintf (text, "%d", FLOAT_TO_MIDI (fVel2));
            break;
        case kOffset:
            sprintf (text, "%d", (signed int) FLOAT_TO_MIDI (fOffset) - 63);
            break;
        case kOffVel1:
            sprintf (text, "%d", FLOAT_TO_MIDI (fOffVel1));
            break;
        case kOffVel2:
            sprintf (text, "%d", FLOAT_TO_MIDI (fOffVel2));
            break;
        case kOffOffset:
            sprintf (text, "%d", (signed int) FLOAT_TO_MIDI (fOffOffset) - 63);
            break;
        default:
            break;
    }
}

void MidiVelocityScale::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames)
{
    // process incoming events
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];
        short status = tomod.midiData[0] & 0xf0; // scraping  channel
        short channel = (tomod.midiData[0] & 0x0f) + 1; // isolating channel (1-16)
        //int data1      = tomod.midiData[1] & 0x7f;
        int data2 = tomod.midiData[2] & 0x7f;
        if (status == MIDI_NOTEON && data2 == 0)
        {
            status = MIDI_NOTEOFF;
            tomod.midiData[0] = MIDI_NOTEOFF | channel - 1;
        }

        if (status == MIDI_NOTEON)
        {
            int vel1 = FLOAT_TO_MIDI (fVel1);
            if (vel1 == 0)
                vel1 = 1;
            int vel2 = FLOAT_TO_MIDI (fVel2);
            if (vel2 == 0)
                vel2 = 1;
            signed int offset = FLOAT_TO_MIDI (fOffset) - 63;

            float newvelocity = ((float) data2) * (((float) (vel2 - vel1)) / 126) + ((float) vel1 - ((float) (vel2 - vel1)) / 126) + offset;
            if (newvelocity > 127)
                newvelocity = 127;
            if (newvelocity < 1)
                newvelocity = 1;
            tomod.midiData[2] = roundToInt (newvelocity);
        }

        else if (status == MIDI_NOTEOFF)
        {
            int vel1 = FLOAT_TO_MIDI (fOffVel1);
            int vel2 = FLOAT_TO_MIDI (fOffVel2);
            signed int offset = FLOAT_TO_MIDI (fOffOffset) - 63;

            float newvelocity = (float) (MAP_TO_MIDI (data2, vel1, vel2, 0, 127) + offset);
            if (newvelocity > 127)
                newvelocity = 127;
            if (newvelocity < 0)
                newvelocity = 0;
            tomod.midiData[2] = roundToInt (newvelocity);
        }
        outputs[0].push_back (tomod);
    }
}
