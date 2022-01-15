/*-----------------------------------------------------------------------------
midiSimpleLFO
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDISIMPLELFO_H
#define __MIDISIMPLELFO_H

#include "../common/PizMidi.h"

enum
{
    kWave,
    kCC,
    kChannel,
    kSync,
    kFreq,
    kPhase,
    kRange,
    kOffset,
    kTrigger,
    kPower,
    kNumParams,
    kNumPrograms = 16
};

class MidiSimpleLFOProgram
{
    friend class MidiSimpleLFO;

public:
    MidiSimpleLFOProgram();
    ~MidiSimpleLFOProgram() {}

private:
    float fWave;
    float fCC;
    float fChannel;
    float fSync;
    float fFreq;
    float fPhase;
    float fRange;
    float fOffset;
    float fTrigger;
    float fPower;
    char name[kVstMaxProgNameLen];
};

class MidiSimpleLFO : public PizMidi
{
public:
    MidiSimpleLFO (audioMasterCallback audioMaster);
    ~MidiSimpleLFO();

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
    float fWave;
    float fCC;
    float fSync;
    float fFreq;
    float fPhase;
    float fChannel;
    float fRange;
    float fOffset;
    float fTrigger;
    float fPower;

    VstInt32 samp;
    int oldenv;
    int step;

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    virtual void preProcess (void);
    float _bpm;
    //float _bar;
    float _ppq;
    float lastppq;
    bool barstarted;
    float ppqPerBar;
    bool triggerme;
    float beats;
    double _beats;
    char numerator;
    double beatsPerSample;
    bool playing;
    bool ccsent;

    MidiSimpleLFOProgram* programs;

    int data2;
    bool on;
    bool retrigger;
    int voices;
    //int modinput;
};

#endif
