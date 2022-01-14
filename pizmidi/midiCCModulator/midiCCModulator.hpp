#ifndef __MIDICCMODULATOR_H
#define __MIDICCMODULATOR_H

#include "../common/PizMidi.h"

enum
{
    kPower,
    kInCC,
    kModCC,
    kSlider,
    kOutCC,
    kAmount,
    kMode,
    kOutput,
    kChannel,
    kThru,
    kNumParams,
    kNumPrograms = 16
};

class MidiCCModulatorProgram
{
    friend class MidiCCModulator;

public:
    MidiCCModulatorProgram();
    ~MidiCCModulatorProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiCCModulator : public PizMidi
{
public:
    MidiCCModulator (audioMasterCallback audioMaster);
    ~MidiCCModulator();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

    virtual void resume();

protected:
    bool init (void);
    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    virtual void preProcess();

    MidiCCModulatorProgram* programs;

    VstInt32 modulate (char input, char mod, bool notes = false);

    //parameters
    float param[kNumParams];

    //values calculated from parameters
    bool on, thru, bipolar;
    signed char modcc;
    enum modes
    {
        uniadd,
        biadd,
        mult,
        sub,
        comp,
        bite,
        invcomp,
        gate,
        block,
        AND,
        NAND,
        OR,
        NOR,
        XOR,
        XNOR,
        equal,
        bNOT,
        bOR,
        bAND,
        bXOR,
        lshift,
        rshift,
        average,
        invbite,
        wrap,
        random
    };
    char mode;
    bool modnotes;

    //temp values
    signed char lastmod, lastout, lastin;
    char dbg[128];
    bool slidermoved;
    bool playing;
    bool wasplaying;
};

#endif
