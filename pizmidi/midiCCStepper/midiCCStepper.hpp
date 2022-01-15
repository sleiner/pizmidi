#ifndef __MIDICCSTEPPER_H
#define __MIDICCSTEPPER_H

#include "../common/PizMidi.h"

enum
{
    kValuePower,
    kValueQ,
    kTimePower,
    kTimeQ,
    kSync,
    kBeatOffset,
    kSendAfter,
    kLowCC,
    kHighCC,
    kCCOffset,
    kChannel,
    kThru,
    kNumParams,
    kNumPrograms = 16
};

class MidiCCStepperProgram
{
    friend class MidiCCStepper;

public:
    MidiCCStepperProgram();
    ~MidiCCStepperProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiCCStepper : public PizMidi
{
public:
    MidiCCStepper (audioMasterCallback audioMaster);
    ~MidiCCStepper();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    bool init (void);
    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    virtual void preProcess (void);

    MidiCCStepperProgram* programs;

    //parameters
    float param[kNumParams];

    float beatdiv;
    float _bpm;
    double _ppq;
    double lastppq;
    unsigned long long totalSamples;
    unsigned long totalSteps;
    unsigned long samplesPerStep;
    unsigned long samplesPerBeat;
    unsigned long leftOverSamples;

    signed char lastcc[128][16];
    bool expectingCC[16];

    bool isplaying;
    bool wasplaying;

    char dbg[128];

    unsigned long long lastTime;
};

#endif
