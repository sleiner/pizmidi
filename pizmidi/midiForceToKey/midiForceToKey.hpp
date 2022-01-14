/*-----------------------------------------------------------------------------
midiForceToKey
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDIFORCETOKEY_H
#define __MIDIFORCETOKEY_H

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
    kMode,
    kTranspose,
    kChannel,
    kRoot,
    kNChannel,
    kNChMode,
    kUsePC,
    kProg,
    kNumParams,
    kNumPrograms = 128
};

enum modes
{
    off,
    nearest,
    alwaysup,
    alwaysdown,
    block,
    numModes
};

class MidiForceToKeyProgram
{
    friend class MidiForceToKey;

public:
    MidiForceToKeyProgram();
    ~MidiForceToKeyProgram() {}

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
    float fMode;
    float fTranspose;
    float fChannel;
    float fRoot;
    float fNChannel;
    float fNChMode;
    float fUsePC;
    char name[kVstMaxProgNameLen];
};

class MidiForceToKey : public PizMidi
{
public:
    MidiForceToKey (audioMasterCallback audioMaster);
    ~MidiForceToKey();

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
    float fMode;
    float fTranspose;
    float fChannel;
    float fRoot;
    float fNChannel;
    float fNChMode;
    float fUsePC;
    float fProg;

    int mode;

    //remember the transposition of every note on every channel
    int transposed[128][16];

    bool settingprog;

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    MidiForceToKeyProgram* programs;
};

#endif
