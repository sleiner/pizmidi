/*-----------------------------------------------------------------------------
	midiScaleChanger
	by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDISCALECHANGER_H
#define __MIDISCALECHANGER_H

#include "../common/PizMidi.h"

enum parameters
{
    k0,
    k1,
    k2,
    k3,
    k4,
    k5,
    k6,
    k7,
    k8,
    k9,
    k10,
    k11,
    kRoot,
    kWrap,
    kChannel,
    kAltChannel,
    kProg,
    kNumParams,
    kNumPrograms = 128
};

class MidiScaleChangerProgram
{
    friend class MidiScaleChanger;

public:
    MidiScaleChangerProgram();
    ~MidiScaleChangerProgram() {}

private:
    float n0;
    float n1;
    float n2;
    float n3;
    float n4;
    float n5;
    float n6;
    float n7;
    float n8;
    float n9;
    float n10;
    float n11;
    float fRoot;
    float fWrap;
    float fChannel;
    float fAltChannel;
    char name[kVstMaxProgNameLen];
};

class MidiScaleChanger : public PizMidi
{
public:
    MidiScaleChanger (audioMasterCallback audioMaster);
    ~MidiScaleChanger();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    //parameters
    float n0;
    float n1;
    float n2;
    float n3;
    float n4;
    float n5;
    float n6;
    float n7;
    float n8;
    float n9;
    float n10;
    float n11;
    float fRoot;
    float fWrap;
    float fChannel;
    float fAltChannel;
    float fProg;

    //remember the transposition of every note on every channel
    int transposed[128][16];
    int noteOnChannel[128][16];
    int notetable[12];
    int root;
    bool settingprog;
    bool wrap;

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    MidiScaleChangerProgram* programs;
};

#endif
