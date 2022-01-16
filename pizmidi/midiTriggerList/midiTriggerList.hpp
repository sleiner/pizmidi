/*-----------------------------------------------------------------------------
midiTriggerList
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDITRIGGERLIST_H
#define __MIDITRIGGERLIST_H

#include "../common/PizMidi.h"

//number of overlapping notes handled per pitch, might need more:
#define MAX_VOICES_PER_NOTE 24

enum
{
    kTriggerType,
    kTriggerNum,
    //kTriggerVal,
    kCCOff,
    kProgCh,
    kChannel,

    kThru,

    kOutputType,
    kOutputNum,
    kOutputVal,

    kNumGlobalParams = kThru + 1,
    kNumSlots        = 8,
    kNumParams       = kNumGlobalParams + kNumSlots * 3,
    kNumPrograms     = 128
};

class MidiTriggerListProgram
{
    friend class MidiTriggerList;

public:
    MidiTriggerListProgram();
    ~MidiTriggerListProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiTriggerList : public PizMidi
{
public:
    MidiTriggerList (audioMasterCallback audioMaster);
    ~MidiTriggerList();

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
    MidiTriggerListProgram* programs;
    bool notesPlaying[128];
};

#endif
