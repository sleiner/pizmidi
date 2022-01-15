/*-----------------------------------------------------------------------------
midiForceToRange
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDIFORCETORANGE_H
#define __MIDIFORCETORANGE_H

#include "../common/PizMidi.h"

enum parameters
{
    //parameters appear in this order
    kLowNote,
    kHighNote,
    kChannel,
    kNumParams, //conveniently counts the parameters
    kNumPrograms = 16
};

class ForceToRangeProgram
{
    friend class ForceToRange;

public:
    ForceToRangeProgram();
    ~ForceToRangeProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen + 1];
};

class ForceToRange : public PizMidi
{
public:
    ForceToRange (audioMasterCallback audioMaster);
    ~ForceToRange();

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

    ForceToRangeProgram* programs;

    float param[kNumParams];
    bool settingProgram;
    //remember the transposition of every note on every channel
    char transposed[128][16];
};

#endif
