/*-----------------------------------------------------------------------------
midiNoteGroups
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDINOTEGROUPS_H
#define __MIDINOTEGROUPS_H

#include "../common/PizMidi.h"

enum
{
    kProgCh,
    kChannel,
    kThru,

    kNote,
    kChoke,
    kPlay,

    kNumGlobalParams = kThru + 1,
    kNumSlots        = 8,
    kNumParams       = kNumGlobalParams + kNumSlots * 3,
    kNumPrograms     = 128,

    off   = -1,
    learn = -2
};

#define FLOAT_TO_GROUP(i) (roundToInt ((i) *64.0f) - 1)

#define FLOAT_TO_MIDI3(i) (roundToInt ((i) *129.f) - 2)
#define MIDI_TO_FLOAT3(i) (float (i + 2) * 0.0077519379844961240310077519379845f)

class MidiNoteGroupsProgram
{
    friend class MidiNoteGroups;

public:
    MidiNoteGroupsProgram();
    ~MidiNoteGroupsProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiNoteGroups : public PizMidi
{
public:
    MidiNoteGroups (audioMasterCallback audioMaster);
    ~MidiNoteGroups();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    float param[kNumParams];

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples);

    virtual void preProcess (void);

    bool wasplaying;
    bool isplaying;

    MidiNoteGroupsProgram* programs;
    bool notesPlaying[128];

    struct Slot
    {
        int note;
        int playgroup;
        int chokegroup;
    };
};

#endif
