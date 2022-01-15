/*-----------------------------------------------------------------------------
midiCCReset
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDICCRESET_H
#define __MIDICCRESET_H

#include "../common/PizMidi.h"

enum
{
    //parameters appear in this order
    kTrigger,
    kUsePlay,
    kProgChSend,
    kCapture,
    kClear,
    //kTimeout,
    kChannel,
    kUsePC,
    kBlockDup,
    kThru,
    //kUseMIDIReset,
    kNumParams, //conveniently counts the parameters
    kNumPrograms = 128
};

class MidiCCResetProgram
{
    friend class MidiCCReset;

public:
    MidiCCResetProgram();
    ~MidiCCResetProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen + 1];
    short lastCC[16][128];
};

class MidiCCReset : public PizMidi
{
public:
    MidiCCReset (audioMasterCallback audioMaster);
    ~MidiCCReset();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
    virtual VstInt32 getChunk (void** data, bool isPreset = false);
    virtual VstInt32 setChunk (void* data, VstInt32 byteSize, bool isPreset = false);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    //parameters
    float param[kNumParams];

    short lastCC[16][128];
    short lastSentCC[16][128];
    bool trigger;
    bool triggeredFromPreset;

    bool init (void);
    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);
    virtual void preProcess (void);

    MidiCCResetProgram* programs;
    bool firstTime;
};

#endif
