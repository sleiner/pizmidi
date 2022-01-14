/*-----------------------------------------------------------------------------
midiProgramChange
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDIPROGRAMCHANGE_H
#define __MIDIPROGRAMCHANGE_H

#include "../common/PizMidi.h"

enum parameters
{
    kProgram,
    kBankMSB,
    kBankLSB,
    kMode,
    kTrigger,
    kInc,
    kDec,
    kBankTrigger,
    kPCListen,
    kChannel,
    kThru,

    kNumParams,
    kNumPrograms = 128
};

class MidiProgramChangeProgram
{
    friend class MidiProgramChange;

public:
    MidiProgramChangeProgram();
    ~MidiProgramChangeProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiProgramChange : public PizMidi
{
public:
    MidiProgramChange (audioMasterCallback audioMaster);
    ~MidiProgramChange();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

    virtual void setSampleRate (float sampleRate);

protected:
    float param[kNumParams];

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples);

    MidiProgramChangeProgram* programs;

    enum modes
    {
        continuous,
        triggered
    };
    char mode;
    bool trigger, inc, dec, triggerbank;
    signed short program, bankmsb, banklsb;
    bool senttrig, sentinc, sentdec, sentbank;
    bool pclisten;
    bool thru;
    bool automated[kNumParams];

    bool wait;
    VstInt32 delaytime;
    VstInt32 counter;
    VstInt32 triggerdelta;
};

#endif
