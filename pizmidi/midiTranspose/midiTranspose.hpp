#ifndef __MIDITRANSPOSE_H
#define __MIDITRANSPOSE_H

#include "../common/PizMidi.h"

enum
{
    kTranspose,
    kRange,
    kImmediate,
    kInChannel,
    kUsePB,
    kUseCC,
    kNumParams,
    kNumPrograms = 16
};

#define MAX_TRANSPOSE (72.f)

class MidiTransposeProgram
{
    friend class MidiTranspose;

public:
    MidiTransposeProgram();
    ~MidiTransposeProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiTranspose : public PizMidi
{
public:
    MidiTranspose (audioMasterCallback audioMaster);
    ~MidiTranspose();

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

    MidiTransposeProgram* programs;

    int transposed[16][128];
    int velocity[16][128];
    int lastTranspose;

    //parameters
    float param[kNumParams];

    inline int getTranspose()
    {
        return roundToInt ((float) (roundToInt (MAX_TRANSPOSE * param[kRange]) * 2) * (param[kTranspose] - 0.5f));
    }
};

#endif
