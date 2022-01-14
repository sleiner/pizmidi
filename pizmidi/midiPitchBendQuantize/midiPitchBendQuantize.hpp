/*-----------------------------------------------------------------------------
midiPitchBendQuantize
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDIPITCHBENDQUANTIZE_H
#define __MIDIPITCHBENDQUANTIZE_H

#include "../common/PizMidi.h"
#include <stdlib.h>
#include <string>

enum
{
    //parameters appear in this order
    kNumSteps = 25,
    kStep = 0,
    kChannel = kNumSteps,
    kNumParams, //counts the parameters
    kNumPrograms = 128
};

class MidiPitchBendQuantizeProgram
{
    friend class MidiPitchBendQuantize;

public:
    MidiPitchBendQuantizeProgram();
    ~MidiPitchBendQuantizeProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen + 1];
};

class MidiPitchBendQuantize : public PizMidi
{
public:
    MidiPitchBendQuantize (audioMasterCallback audioMaster);
    ~MidiPitchBendQuantize();

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

    MidiPitchBendQuantizeProgram* programs;

    //parameters
    float param[kNumParams];

    int step[kNumSteps];

    void fillpatch (long p, const char* name, int p0, int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14, int p15, int p16, int p17, int p18, int p19, int p20, int p21, int p22, int p23, int p24, int p25);
};

#endif
