#ifndef __MIDIKEYSPLIT4_H
#define __MIDIKEYSPLIT4_H

#include "../common/PizMidi.h"

enum
{
    kOutChannel1,
    kTransp1,
    kSplit1,
    kOutChannel2,
    kTransp2,
    kSplit2,
    kOutChannel3,
    kTransp3,
    kSplit3,
    kOutChannel4,
    kTransp4,
    kInChannel,

    kNumParams,
    kNumPrograms = 16,
    learn        = -1
};

class MidiKeySplit4Program
{
    friend class MidiKeySplit4;

public:
    MidiKeySplit4Program();
    ~MidiKeySplit4Program() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiKeySplit4 : public PizMidi
{
public:
    MidiKeySplit4 (audioMasterCallback audioMaster);
    ~MidiKeySplit4();

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
    MidiKeySplit4Program* programs;

    //parameters
    float param[kNumParams];

    int transposed[128];
};

#endif
