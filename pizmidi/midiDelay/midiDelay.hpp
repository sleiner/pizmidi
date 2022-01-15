/*-----------------------------------------------------------------------------
midiDelay
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDIDELAY_H
#define __MIDIDELAY_H

#include "../common/PizMidi.h"

enum
{
    kSync,
    kTime,
    kFeedback,
    kLimit,
    kWet,
    kDry,
    kMode,
    kChannel,
    kNumParams,
    kNumPrograms = 16
};

class MidiDelayProgram
{
    friend class MidiDelay;

public:
    MidiDelayProgram();
    ~MidiDelayProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiDelay : public PizMidi
{
public:
    MidiDelay (audioMasterCallback audioMaster);
    ~MidiDelay();

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

    float stepsize;
    float _bpm;
    double _ppq;
    double _beatpos;
    VstInt32 samplesPerBeat;
    VstInt32 samplesPerTick;
    unsigned long long totalSamples;

    int expectingDelayedNoteOff[128][16];
    int expectingDelayedNoteOn[128][16];
    bool notePlaying[128][16];
    bool noteKilled[128][16];

    int modulate (VstMidiEvent input, VstMidiEvent mod);
    char lastout[128][16];

    bool wasplaying;
    bool isplaying;

    MidiDelayProgram* programs;

    char dbg[128];
};

#endif
