#include "midiTriggerList.hpp"

//for sort()
#include <algorithm>

//for fmod()
#include <cmath>

//for seeding rand()
#include <time.h>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new MidiTriggerList (audioMaster);
}

MidiTriggerListProgram::MidiTriggerListProgram()
{
    // default Program Values
    for (int i = 0; i < kNumParams; i++)
        param[i] = 0.f; //just in case
    param[kTriggerType] = 0.3f;
    param[kTriggerNum] = 0.47f; //middle C
    for (int i = 0; i < kNumSlots; i++)
        param[kOutputType + i * 3] = 0.3f;

    // default program name
    strcpy (name, "Default");
}

//-----------------------------------------------------------------------------
MidiTriggerList::MidiTriggerList (audioMasterCallback audioMaster)
    : PizMidi (audioMaster, kNumPrograms, kNumParams), programs (0)
{
    programs = new MidiTriggerListProgram[numPrograms];
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
            // built-in programs
            for (int i = 0; i < kNumPrograms; i++)
            {
                sprintf (programs[i].name, "Program %d", i + 1);
            }
        }
        setProgram (0);
    }

    init();

    if (programs)
        setProgram (0);
}

//-----------------------------------------------------------------------------------------
MidiTriggerList::~MidiTriggerList()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiTriggerList::setProgram (VstInt32 program)
{
    MidiTriggerListProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter (i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiTriggerList::setProgramName (char* name)
{
    vst_strncpy (programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiTriggerList::getProgramName (char* name)
{
    vst_strncpy (name, programs[curProgram].name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
bool MidiTriggerList::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        vst_strncpy (text, programs[index].name, kVstMaxProgNameLen);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiTriggerList::setParameter (VstInt32 index, float value)
{
    if (index < kNumParams)
    {
        MidiTriggerListProgram* ap = &programs[0];
        switch (index)
        {
            case kTriggerType:
                updateDisplay();
                break;
            case kThru:
            case kProgCh:
            case kChannel:
                for (int i = 0; i < kNumPrograms; i++)
                    (ap++)->param[index] = value;
                param[index] = value;
                break;
            default:
                if (index % kOutputType == 0)
                    updateDisplay();
                break;
        }
        ap = &programs[curProgram];
        param[index] = ap->param[index] = value;
    }
}

//-----------------------------------------------------------------------------------------
float MidiTriggerList::getParameter (VstInt32 index)
{
    if (index < kNumParams)
        return param[index];
    return 0.f;
}

//-----------------------------------------------------------------------------------------
void MidiTriggerList::getParameterName (VstInt32 index, char* label)
{
    if (index < kNumParams)
    {
        if (index == kTriggerType)
            strcpy (label, "Trigger Type");
        else if (index == kTriggerNum)
            strcpy (label, "Trigger Num");
        //        else if (index==kTriggerVal) strcpy(label, "Trigger Value");
        else if (index == kCCOff)
            strcpy (label, "Send CC Off");
        else if (index == kProgCh)
            strcpy (label, "Use ProgCh");
        else if (index == kChannel)
            strcpy (label, "Channel");
        else if (index == kThru)
            strcpy (label, "Thru");

        else
        {
            for (int i = 0; i < kNumSlots; i++)
            {
                if (index == kOutputNum + i * 3)
                    sprintf (label, "Slot %d Num", i + 1);
                else if (index == kOutputType + i * 3)
                    sprintf (label, "Slot %d Type", i + 1);
                else if (index == kOutputVal + i * 3)
                    sprintf (label, "Slot %d Value", i + 1);
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
void MidiTriggerList::getParameterDisplay (VstInt32 index, char* text)
{
    float v = param[index];
    if (index < kNumParams)
    {
        if (index == kTriggerType)
        {
            if (v < 0.25f)
                strcpy (text, "Learn Next");
            else if (v < 0.50f)
                strcpy (text, "Note On/Off");
            else if (v < 0.75f)
                strcpy (text, "CC 127/0");
            else
                strcpy (text, "CC > 0");
        }
        else if (index == kTriggerNum)
        {
            if (param[kTriggerType] < 0.25f)
                strcpy (text, "Waiting...");
            else if (param[kTriggerType] < 0.5f)
                sprintf (text, "%s (%d)", getNoteName (FLOAT_TO_MIDI (v), bottomOctave), FLOAT_TO_MIDI (v));
            else
                sprintf (text, "%d", FLOAT_TO_MIDI (v));
        }
        //        else if (index==kTriggerVal) {
        //            if (FLOAT_TO_MIDI2(v)==-1) strcpy(text, "Any");
        //            else sprintf(text, "%d", FLOAT_TO_MIDI2(v));
        //        }
        else if (index == kCCOff)
        {
            if (v < 0.5f)
                strcpy (text, "No");
            else
                strcpy (text, "Yes");
        }
        else if (index == kProgCh)
        {
            if (v < 0.5f)
                strcpy (text, "No");
            else
                strcpy (text, "Yes");
        }
        else if (index == kChannel)
        {
            sprintf (text, "%d", FLOAT_TO_CHANNEL015 (v) + 1);
        }
        else if (index == kThru)
        {
            if (v < 0.33333f)
                strcpy (text, "All Thru");
            else if (v < 0.66667f)
                strcpy (text, "Block Trigger");
            else
                strcpy (text, "Block All");
        }

        else
        {
            for (int i = 0; i < kNumSlots; i++)
            {
                if (index == kOutputType + i * 3)
                {
                    if (v < 0.25f)
                        strcpy (text, "Learn");
                    else if (v < 0.5f)
                        strcpy (text, "Off");
                    else if (v < 0.75f)
                        strcpy (text, "Note");
                    else
                        strcpy (text, "CC");
                }
                else if (index == kOutputNum + i * 3)
                {
                    if (param[kOutputType + i * 3] < 0.25f)
                        strcpy (text, "Waiting...");
                    else if (param[kOutputType + i * 3] < 0.5f)
                        strcpy (text, "--");
                    else if (param[kOutputType + i * 3] < 0.75f)
                        sprintf (text, "%s (%d)", getNoteName (FLOAT_TO_MIDI (v), bottomOctave), FLOAT_TO_MIDI (v));
                    else
                        sprintf (text, "%d", FLOAT_TO_MIDI (v));
                }
                else if (index == kOutputVal + i * 3)
                {
                    if (FLOAT_TO_MIDI_X (v) == -1)
                        strcpy (text, "As Input");
                    else
                        sprintf (text, "%d", FLOAT_TO_MIDI_X (v));
                }
            }
        }
    }
}

void MidiTriggerList::processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples)
{
    //process incoming events-------------------------------------------------------
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        const int channel = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        const int data1 = tomod.midiData[1] & 0x7f;
        const int data2 = tomod.midiData[2] & 0x7f;
        const int status = (tomod.midiData[0] & 0xf0) == MIDI_NOTEON && data2 == 0 ? MIDI_NOTEOFF : tomod.midiData[0] & 0xf0; // scraping  channel

        //        int trigval = FLOAT_TO_MIDI2(param[kTriggerVal]);
        //        if (trigval==-1) trigval=data2;
        int trignum = FLOAT_TO_MIDI (param[kTriggerNum]);
        int listenchannel = FLOAT_TO_CHANNEL015 (param[kChannel]);
        bool discard = param[kThru] >= 0.5f;

        if (channel == listenchannel)
        {
            //midi learn
            if (param[kTriggerType] < 0.25f)
            {
                if (status == MIDI_CONTROLCHANGE)
                {
                    if (data2 == 127)
                        setParameterAutomated (kTriggerType, 1.f);
                    else if (data2 > 0)
                        setParameterAutomated (kTriggerType, 0.6f);
                }
                else if (status == MIDI_NOTEON)
                    setParameterAutomated (kTriggerType, 0.4f);
                if (param[kTriggerType] >= 0.25f)
                {
                    setParameterAutomated (kTriggerNum, MIDI_TO_FLOAT (data1));
                }
            }
            else
            {
                for (int i = 0; i < kNumSlots; i++)
                {
                    if (param[kOutputType + i * 3] < 0.25f)
                    {
                        if (status == MIDI_CONTROLCHANGE)
                            setParameterAutomated (kOutputType + i * 3, 1.f);
                        else if (status == MIDI_NOTEON)
                            setParameterAutomated (kOutputType + i * 3, 0.6f);
                        if (param[kOutputType + i * 3] >= 0.25f)
                        {
                            setParameterAutomated (kOutputNum + i * 3, MIDI_TO_FLOAT (data1));
                            setParameterAutomated (kOutputVal + i * 3, MIDI_TO_FLOAT (data2));
                        }
                        break;
                    }
                }
            }
            //triggering
            if (param[kTriggerType] >= 0.25f)
            {
                if (status == MIDI_CONTROLCHANGE)
                {
                    if (param[kTriggerType] >= 0.5f && data1 == trignum)
                    {
                        if (param[kThru] >= 0.33333f)
                            discard = true;
                        if ((param[kTriggerType] >= 0.75f && data2 > 0) || data2 == 127)
                        {
                            for (int i = 0; i < kNumSlots; i++)
                            {
                                if (param[kOutputType + i * 3] >= 0.5f)
                                {
                                    int outval = FLOAT_TO_MIDI_X (param[kOutputVal + i * 3]);
                                    if (outval == -1)
                                        outval = data2;
                                    int outnum = FLOAT_TO_MIDI (param[kOutputNum + i * 3]);
                                    bool isNote = param[kOutputType + i * 3] < 0.75f;
                                    if (isNote && notesPlaying[outnum])
                                    {
                                        //kill if playing
                                        VstMidiEvent m = tomod;
                                        m.midiData[0] = MIDI_NOTEOFF | channel;
                                        m.midiData[1] = outnum;
                                        m.midiData[2] = 0;
                                        outputs[0].push_back (m);
                                    }
                                    VstMidiEvent m = tomod;
                                    m.midiData[0] = (isNote ? MIDI_NOTEON : MIDI_CONTROLCHANGE) | channel;
                                    m.midiData[1] = outnum;
                                    m.midiData[2] = outval;
                                    outputs[0].push_back (m);
                                    if (isNote)
                                        notesPlaying[outnum] = true;
                                }
                            }
                        }
                        else if (data2 == 0)
                        {
                            for (int i = 0; i < kNumSlots; i++)
                            {
                                if (param[kOutputType + i * 3] >= 0.5f)
                                {
                                    int outnum = FLOAT_TO_MIDI (param[kOutputNum + i * 3]);
                                    bool isNote = param[kOutputType + i * 3] < 0.75f;
                                    if (isNote && notesPlaying[outnum])
                                    {
                                        //kill if playing
                                        VstMidiEvent m = tomod;
                                        m.midiData[0] = MIDI_NOTEOFF | channel;
                                        m.midiData[1] = outnum;
                                        m.midiData[2] = 0;
                                        outputs[0].push_back (m);
                                    }
                                    VstMidiEvent m = tomod;
                                    m.midiData[0] = (isNote ? MIDI_NOTEOFF : MIDI_CONTROLCHANGE) | channel;
                                    m.midiData[1] = outnum;
                                    m.midiData[2] = 0;
                                    if (isNote || param[kCCOff] >= 0.5f)
                                        outputs[0].push_back (m);
                                    if (isNote)
                                        notesPlaying[outnum] = false;
                                }
                            }
                        }
                    }
                }
                else if (status == MIDI_NOTEON)
                {
                    if (param[kTriggerType] < 0.5f && data1 == FLOAT_TO_MIDI (param[kTriggerNum]))
                    {
                        if (param[kThru] >= 0.33333f)
                            discard = true;
                        for (int i = 0; i < kNumSlots; i++)
                        {
                            if (param[kOutputType + i * 3] >= 0.5f)
                            {
                                int outnum = FLOAT_TO_MIDI (param[kOutputNum + i * 3]);
                                bool isNote = param[kOutputType + i * 3] < 0.75f;
                                int outval = FLOAT_TO_MIDI_X (param[kOutputVal + i * 3]);
                                if (outval == -1)
                                    outval = data2;
                                if (isNote && notesPlaying[outnum])
                                {
                                    //kill if playing
                                    VstMidiEvent m = tomod;
                                    m.midiData[0] = MIDI_NOTEOFF | channel;
                                    m.midiData[1] = outnum;
                                    m.midiData[2] = 0;
                                    outputs[0].push_back (m);
                                }
                                VstMidiEvent m = tomod;
                                m.midiData[0] = (isNote ? MIDI_NOTEON : MIDI_CONTROLCHANGE) | channel;
                                m.midiData[1] = outnum;
                                m.midiData[2] = outval;
                                outputs[0].push_back (m);
                                if (isNote)
                                    notesPlaying[outnum] = true;
                            }
                        }
                    }
                }
                else if (status == MIDI_NOTEOFF)
                {
                    if (param[kTriggerType] < 0.5f && data1 == FLOAT_TO_MIDI (param[kTriggerNum]))
                    {
                        if (param[kThru] > 0.33333f)
                            discard = true;
                        for (int i = 0; i < kNumSlots; i++)
                        {
                            if (param[kOutputType + i * 3] >= 0.5f)
                            {
                                int outnum = FLOAT_TO_MIDI (param[kOutputNum + i * 3]);
                                bool isNote = param[kOutputType + i * 3] < 0.75f;
                                VstMidiEvent m = tomod;
                                m.midiData[0] = (isNote ? MIDI_NOTEOFF : MIDI_CONTROLCHANGE) | channel;
                                m.midiData[1] = outnum;
                                m.midiData[2] = data2;
                                if (isNote || param[kCCOff] >= 0.5f)
                                    outputs[0].push_back (m);
                                if (isNote)
                                    notesPlaying[outnum] = false;
                            }
                        }
                    }
                }
            }
            if (status == MIDI_PROGRAMCHANGE)
            {
                //end playing notes before changing program
                for (int i = 0; i < 128; i++)
                {
                    if (notesPlaying[i])
                    {
                        VstMidiEvent m = tomod;
                        m.midiData[0] = MIDI_NOTEOFF | channel;
                        m.midiData[1] = i;
                        m.midiData[2] = 0;
                        notesPlaying[i] = false;
                    }
                }
                if (param[kProgCh] >= 0.5f)
                    setProgram (data1);
            }
        }

        if (! discard)
            outputs[0].push_back (tomod);

    } //for() inputs loop
}
