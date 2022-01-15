/*-----------------------------------------------------------------------------
midiVelocityScale
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDIVELOCITYSCALE_H
#define __MIDIVELOCITYSCALE_H

#include "../common/PizMidi.h"

enum
{
    kVel1,
    kVel2,
    kOffset,
    kOffVel1,
    kOffVel2,
    kOffOffset,
    kNumParams,
    kNumPrograms = 16
};

class MidiVelocityScaleProgram
{
    friend class MidiVelocityScale;

public:
    MidiVelocityScaleProgram();
    ~MidiVelocityScaleProgram() {}

private:
    float fVel1;
    float fVel2;
    float fOffset;
    float fOffVel1;
    float fOffVel2;
    float fOffOffset;
    char name[kVstMaxProgNameLen];
};

class MidiVelocityScale : public PizMidi
{
public:
    MidiVelocityScale (audioMasterCallback audioMaster);
    ~MidiVelocityScale();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    float fVel1;
    float fVel2;
    float fOffset;
    float fOffVel1;
    float fOffVel2;
    float fOffOffset;

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);
    MidiVelocityScaleProgram* programs;
};

#endif
