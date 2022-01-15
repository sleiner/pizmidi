#ifndef __MIDIKEYSPLIT_H
#define __MIDIKEYSPLIT_H

#include "../common/PizMidi.h"

enum
{
    kOutChannel1,
    kTransp1,
    kSplit1,
    kOutChannel2,
    kTransp2,
    kInChannel,
    kNumParams,
    kNumPrograms = 16
};

class MidiKeySplitProgram
{
    friend class MidiKeySplit;

public:
    MidiKeySplitProgram();
    ~MidiKeySplitProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiKeySplit : public PizMidi
{
public:
    MidiKeySplit (audioMasterCallback audioMaster);
    ~MidiKeySplit();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    MidiKeySplitProgram* programs;

    int transposed[128];

    //parameters
    float param[kNumParams];
};

#endif
