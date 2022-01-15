/*-----------------------------------------------------------------------------
midiNoteRepeater
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDINOTEREPEATER_H
#define __MIDINOTEREPEATER_H

#include "../common/PizMidi.h"

#define maxCount 100

enum
{
    kSync,
    kTime,
    kOffset,
    kCount,
    kFeedback,
    kLimit,
    kWet,
    kDry,
    kNoteFilter,
    kChannel,
    kOutChannel,
    kNumParams,

    kPattern,
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
    VstMidiEventVec tempBuffer;
    void cleanMidiDelayBuffer();

    float stepsize;
    float _bpm;
    double _ppq;
    double _beatpos;
    VstInt32 samplesPerBeat;
    VstInt32 samplesPerTick;
    unsigned long long totalSamples;

    bool notePlaying[128][16];
    bool noteKilled[128][16];
    bool repeating[128][16];
    int noteOnChannel[128][16];
    char lastout[128][16];

    bool wasplaying;
    bool isplaying;

    int counter;

    MidiDelayProgram* programs;
};

#endif
