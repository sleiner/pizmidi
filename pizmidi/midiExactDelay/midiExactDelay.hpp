/*-----------------------------------------------------------------------------
midiExactDelay
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDIEXACTDELAY_H
#define __MIDIEXACTDELAY_H

#include "../common/PizMidi.h"

enum
{
    kSync,
    kSeconds,
    kTenths,
    kHundredths,
    kThousandths,
    kBars,
    kBeats,
    kBeatDiv,
    kTicks,
    k1000Samples,
    k100Samples,
    k10Samples,
    kSamples,
    kWet,
    kMode,
    kChannel,
    kNumParams,
    kNumPrograms = 16
};

class MidiExactDelayProgram
{
    friend class MidiExactDelay;

public:
    MidiExactDelayProgram();
    ~MidiExactDelayProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiExactDelay : public PizMidi
{
public:
    MidiExactDelay (audioMasterCallback audioMaster);
    ~MidiExactDelay();

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
    float param[kNumParams];

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples);

    virtual void preProcess (void);

    VstMidiEventVec midiDelayBuffer;
    void cleanMidiDelayBuffer();

    bool wasplaying;
    bool isplaying;

    VstInt32 samplesPerBeat;
    VstInt32 samplesPerNumerator;
    VstInt32 samplesPerTick;
    enum timemodes
    {
        sec,
        beat,
        samp
    };
    int timebase;
    float ppqPerBar;
    float Ticks;

    MidiExactDelayProgram* programs;
};

#endif
