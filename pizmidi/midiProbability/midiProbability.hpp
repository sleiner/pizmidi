#ifndef __MIDIPROBABILITY_H
#define __MIDIPROBABILITY_H

#include "../common/PizMidi.h"

enum parameters
{
    kPower,
    kStep1,
    kOffset1,
    kMode1,
    kProb1,
    kStep2,
    kOffset2,
    kMode2,
    kProb2,
    kRadius,
    kSkip,
    kOctUp,
    kOctDn,
    kRandT,
    kTransp,
    kRandV,
    kOffsetV,
    kChan,
    kTranspAmt,
    kVelAmt,
    kChCh,
    kChannel,

    kNumParams,
    numParamsPerSlot = kMode2 - kMode1,
    numSlots = 2,
    kNumPrograms = 16
};

class MidiProbabilityProgram
{
    friend class MidiProbability;

public:
    MidiProbabilityProgram();
    ~MidiProbabilityProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiProbability : public PizMidi
{
public:
    MidiProbability (audioMasterCallback audioMaster);
    ~MidiProbability();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterLabel (VstInt32 index, char* label);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    float param[kNumParams];

    bool init (void);
    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples);

    virtual void preProcess (void);

    float stepsize[numSlots];
    float _bpm;
    double _ppq;
    double _beatpos[numSlots];
    VstInt32 samplesPerBeat;
    inline float closeness (long int samplepos);

    bool notePlaying[128][16];
    enum effects
    {
        off = -1,
        disc = 0,
        octup,
        octdown,
        randtr,
        transp,
        randvel,
        offsetvel,
        chan,
        multi,
        numEffects
    };
    signed char noteAffected[128][16][numEffects];

    char mode;

    bool wasplaying;
    bool isplaying;

    MidiProbabilityProgram* programs;
};

#endif
