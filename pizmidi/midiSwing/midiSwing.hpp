/*-----------------------------------------------------------------------------
midiSwing
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDISWING_H
#define __MIDISWING_H

#include "../common/PizMidi.h"

//number of overlapping notes handled per pitch, might need more:
#define MAX_VOICES_PER_NOTE 6

enum
{
    kMode,
    kValue,
    kAmount,
    kRadius,
    kShape,
    kOffset,
    kVelocity,
    kProb,
    kRandom,
    kMaxAmount,
    kOverlap,
    kChannel,
    kNumParams,
    kNumPrograms = 16
};

class MidiSwingProgram
{
    friend class MidiSwing;

public:
    MidiSwingProgram();
    ~MidiSwingProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiSwing : public PizMidi
{
public:
    MidiSwing (audioMasterCallback audioMaster);
    ~MidiSwing();

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

    float stepsize;
    float _bpm;
    double _ppq;
    double _beatpos;
    VstInt32 samplesPerBeat;
    VstInt32 samplesPerTick;
    unsigned long long totalSamples;
    inline float closeness (long int samplepos);

    VstInt32 noteOnCounter[128][16][MAX_VOICES_PER_NOTE];
    VstInt32 noteOffCounter[128][16][MAX_VOICES_PER_NOTE];
    unsigned char noteOrigVel[128][16][MAX_VOICES_PER_NOTE];
    unsigned long long noteOrigPos[128][16][MAX_VOICES_PER_NOTE];
    VstInt32 noteDelay[128][16][MAX_VOICES_PER_NOTE];
    VstInt32 noteOffDelay[128][16][MAX_VOICES_PER_NOTE];
    bool notePlaying[128][16][MAX_VOICES_PER_NOTE];
    unsigned char voices[128][16];
    bool noteKilled[128][16];

    VstInt32 CCCounter[128][16];
    VstInt32 CCDelay[128][16];
    unsigned char CCOrigValue[128][16];

    bool wasplaying;
    bool isplaying;
    unsigned char expectingDelayedEvents[16];
    unsigned char expectingDelayedCC[16];
    bool CCRecieved[128][16];

    inline void resetCC (char n, char ch);
    inline void resetNote (char n, char ch, char v, bool off = false);

    MidiSwingProgram* programs;
};

#endif
