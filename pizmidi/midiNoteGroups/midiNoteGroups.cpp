#include "midiNoteGroups.hpp"

//for sort()
#include <algorithm>

//for fmod()
#include <cmath>

//for seeding rand()
#include <time.h>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new MidiNoteGroups(audioMaster);
}

MidiNoteGroupsProgram::MidiNoteGroupsProgram()
{
    // default Program Values
    for (int i = 0; i < kNumParams; i++)
        param[i] = 0.f; //just in case
    param[kChannel] = 0.f;
    param[kThru]    = 1.f;
    for (int s = 0; s < kNumSlots; s++)
        param[kNote + s * 3] = MIDI_TO_FLOAT3(off);

    // default program name
    strcpy(name, "Default");
}

//-----------------------------------------------------------------------------
MidiNoteGroups::MidiNoteGroups(audioMasterCallback audioMaster)
    : PizMidi(audioMaster, kNumPrograms, kNumParams), programs(0)
{
    programs = new MidiNoteGroupsProgram[numPrograms];
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

    wasplaying = false;
    isplaying  = false;
    if (programs)
        setProgram(0);

    init();
}

//-----------------------------------------------------------------------------------------
MidiNoteGroups::~MidiNoteGroups()
{
    if (programs)
        delete[] programs;
}

//------------------------------------------------------------------------
void MidiNoteGroups::setProgram(VstInt32 program)
{
    MidiNoteGroupsProgram* ap = &programs[program];

    curProgram = program;
    for (int i = 0; i < kNumParams; i++)
    {
        setParameter(i, ap->param[i]);
    }
}

//------------------------------------------------------------------------
void MidiNoteGroups::setProgramName(char* name)
{
    vst_strncpy(programs[curProgram].name, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------
void MidiNoteGroups::getProgramName(char* name)
{
    strcpy(name, programs[curProgram].name);
}

//-----------------------------------------------------------------------------------------
bool MidiNoteGroups::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text)
{
    if (index < kNumPrograms)
    {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------
void MidiNoteGroups::setParameter(VstInt32 index, float value)
{
    if (index < kNumParams)
    {
        MidiNoteGroupsProgram* ap = &programs[0];
        switch (index)
        {
            case kThru:
            case kProgCh:
            case kChannel:
                for (int i = 0; i < kNumPrograms; i++)
                    (ap++)->param[index] = value;
                param[index] = value;
                break;
            default:
                ap           = &programs[curProgram];
                param[index] = ap->param[index] = value;
                break;
        }
    }
}

//-----------------------------------------------------------------------------------------
float MidiNoteGroups::getParameter(VstInt32 index)
{
    if (index < kNumParams)
        return param[index];
    return 0.f;
}

//-----------------------------------------------------------------------------------------
void MidiNoteGroups::getParameterName(VstInt32 index, char* label)
{
    if (index < kNumParams)
    {
        if (index == kProgCh)
            strcpy(label, "Use ProgCh");
        else if (index == kChannel)
            strcpy(label, "Channel");
        else if (index == kThru)
            strcpy(label, "Thru");

        else
        {
            for (int i = 0; i < kNumSlots; i++)
            {
                if (index == kNote + i * 3)
                    sprintf(label, "Note %d", i + 1);
                else if (index == kPlay + i * 3)
                    sprintf(label, " %d: PlayGroup", i + 1);
                else if (index == kChoke + i * 3)
                    sprintf(label, " %d: ChokeGroup", i + 1);
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
void MidiNoteGroups::getParameterDisplay(VstInt32 index, char* text)
{
    float v = param[index];
    if (index < kNumParams)
    {
        if (index == kProgCh)
        {
            if (v < 0.5f)
                strcpy(text, "No");
            else
                strcpy(text, "Yes");
        }
        else if (index == kChannel)
        {
            sprintf(text, "%d", FLOAT_TO_CHANNEL015(v) + 1);
        }
        else if (index == kThru)
        {
            if (v < 0.5f)
                strcpy(text, "No");
            else
                strcpy(text, "Yes");
        }

        else
        {
            for (int i = 0; i < kNumSlots; i++)
            {
                if (index == kNote + i * 3)
                {
                    if (FLOAT_TO_MIDI3(v) == off)
                        strcpy(text, "Off");
                    else if (FLOAT_TO_MIDI3(v) == learn)
                        strcpy(text, "Learn Next");
                    else
                        sprintf(text, "%s (%d)", getNoteName(FLOAT_TO_MIDI3(v), bottomOctave), FLOAT_TO_MIDI3(v));
                }
                else if (index == kPlay + i * 3)
                {
                    if (FLOAT_TO_GROUP(v) == -1)
                        strcpy(text, "None");
                    else
                        sprintf(text, "%d", FLOAT_TO_GROUP(v));
                }
                else if (index == kChoke + i * 3)
                {
                    if (FLOAT_TO_GROUP(v) == -1)
                        strcpy(text, "None");
                    else
                        sprintf(text, "%d", FLOAT_TO_GROUP(v));
                }
            }
        }
    }
}

void MidiNoteGroups::preProcess(void)
{
    // preparing Process
    VstTimeInfo* timeInfo = NULL;
    timeInfo              = getTimeInfo(0xffff); //ALL

    if (timeInfo)
    {
        //        if (kVstTempoValid & timeInfo->flags) _bpm = (float)timeInfo->tempo;
        //        if (kVstPpqPosValid & timeInfo->flags) _ppq = timeInfo->ppqPos;
        if (kVstTransportPlaying & timeInfo->flags)
            isplaying = true;
        else
            isplaying = false;
    }

    _cleanMidiOutBuffers();
}

void MidiNoteGroups::processMidiEvents(VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples)
{
    //process incoming events-------------------------------------------------------
    for (unsigned int i = 0; i < inputs[0].size(); i++)
    {
        //copying event "i" from input (with all its fields)
        VstMidiEvent tomod = inputs[0][i];

        const int channel = tomod.midiData[0] & 0x0f; // isolating channel (0-15)
        const int data1   = tomod.midiData[1] & 0x7f;
        const int data2   = tomod.midiData[2] & 0x7f;
        const int status  = (tomod.midiData[0] & 0xf0) == MIDI_NOTEON && data2 == 0 ? MIDI_NOTEOFF : tomod.midiData[0] & 0xf0; // scraping  channel

        int listenchannel = FLOAT_TO_CHANNEL015(param[kChannel]);
        bool discard      = param[kThru] < 0.5f;

        Slot slot[kNumSlots];
        for (int s = 0; s < kNumSlots; s++)
        {
            slot[s].note       = FLOAT_TO_MIDI3(param[kNote + s * 3]);
            slot[s].playgroup  = FLOAT_TO_GROUP(param[kPlay + s * 3]);
            slot[s].chokegroup = FLOAT_TO_GROUP(param[kChoke + s * 3]);
        }

        if (channel == listenchannel)
        {
            if (status == MIDI_NOTEON)
            {
                for (int i = 0; i < kNumSlots; i++)
                {
                    if (slot[i].note == learn)
                    {
                        discard = true;
                        setParameterAutomated(kNote + i * 3, MIDI_TO_FLOAT3(data1));
                    }
                    if (data1 == slot[i].note)
                    {
                        discard = false;
                        if (slot[i].chokegroup > -1)
                        {
                            //end other notes in choke group
                            for (int s = 0; s < kNumSlots; s++)
                            {
                                if (slot[s].chokegroup == slot[i].chokegroup)
                                {
                                    if (notesPlaying[slot[s].note])
                                    {
                                        VstMidiEvent m = tomod;
                                        m.midiData[0]  = MIDI_NOTEOFF | channel;
                                        m.midiData[1]  = slot[s].note;
                                        m.midiData[2]  = 0;
                                        outputs[0].push_back(m);
                                        notesPlaying[slot[s].note] = false;
                                    }
                                }
                            }
                        }
                        if (slot[i].playgroup > -1)
                        {
                            //play other notes in play group
                            for (int s = 0; s < kNumSlots; s++)
                            {
                                if (slot[s].playgroup == slot[i].playgroup)
                                {
                                    if (! notesPlaying[slot[s].note])
                                    {
                                        VstMidiEvent m = tomod;
                                        m.midiData[0]  = MIDI_NOTEON | channel;
                                        m.midiData[1]  = slot[s].note;
                                        m.midiData[2]  = data2;
                                        outputs[0].push_back(m);
                                        notesPlaying[slot[s].note] = true;
                                    }
                                }
                            }
                        }
                    }
                }
                if (notesPlaying[data1])
                    discard = true;
                if (! discard)
                    notesPlaying[data1] = true;
            }
            else if (status == MIDI_NOTEOFF)
            {
                for (int i = 0; i < kNumSlots; i++)
                {
                    if (data1 == slot[i].note)
                    {
                        discard = false;
                        if (slot[i].playgroup > -1)
                        {
                            //end other notes in play group
                            for (int s = 0; s < kNumSlots; s++)
                            {
                                if (slot[s].playgroup == slot[i].playgroup)
                                {
                                    if (notesPlaying[slot[s].note])
                                    {
                                        VstMidiEvent m = tomod;
                                        m.midiData[0]  = MIDI_NOTEOFF | channel;
                                        m.midiData[1]  = slot[s].note;
                                        m.midiData[2]  = 0;
                                        outputs[0].push_back(m);
                                        notesPlaying[slot[s].note] = false;
                                    }
                                }
                            }
                        }
                    }
                }
                if (! notesPlaying[data1])
                    discard = true;
                if (! discard)
                    notesPlaying[data1] = false;
            }
            else if (status == MIDI_PROGRAMCHANGE)
            {
                //end playing notes before changing program
                for (int i = 0; i < 128; i++)
                {
                    if (notesPlaying[i])
                    {
                        VstMidiEvent m  = tomod;
                        m.midiData[0]   = MIDI_NOTEOFF | channel;
                        m.midiData[1]   = i;
                        m.midiData[2]   = 0;
                        notesPlaying[i] = false;
                    }
                }
                if (param[kProgCh] >= 0.5f)
                    setProgram(data1);
            }
        }

        if (! discard)
            outputs[0].push_back(tomod);

    } //for() inputs loop

    wasplaying = isplaying;
}
