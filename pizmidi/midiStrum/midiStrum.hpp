/*-----------------------------------------------------------------------------
midiStrum
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDISTRUM_H
#define __MIDISTRUM_H

#include "../common/PizMidi.h"

//number of overlapping notes handled per pitch, might need more:
#define MAX_VOICES_PER_NOTE 24

enum
{
    kMode,
    kLength,
    kSpeed,
    kMaxDelay,
    kSync,
    kVelToSpeed,
    kVelToVel,
    kDnVelocity,
    kUpVelocity,
    kVelRamp,
    kRandom,
    kAccel,
    kInChannel,
    kStrumChannel,
    kStrumOct,
    kOutChannel,
    kThru,
    kSustain,
    kNumParams,
    kNumPrograms = 16
};

class midiStrumProgram
{
    friend class midiStrum;

public:
    midiStrumProgram();
    ~midiStrumProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class midiStrum : public PizMidi
{
public:
    midiStrum (audioMasterCallback audioMaster);
    ~midiStrum();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

    virtual void setSampleRate (float sampleRate);
    virtual void resume();

protected:
    float param[kNumParams];

    bool init();
    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 samples);
    virtual void preProcess();

    float stepsize;
    float beatdiv;
    float _bpm;
    double _ppq;
    unsigned long samplesPerBeat;
    unsigned long long totalSamples;

    unsigned long long noteOrigPos[128];
    VstInt32 noteDelay[128];
    bool notePlaying[128];
    bool notePlaying2[128];
    unsigned short voices[128];
    short heldnotes;
    int held[128]; //0=not held; 1=sustained with pedal; 2=manually held
    bool strumming;
    int strumnote;
    bool sustain;
    int mutedstrumnote;
    bool usemutedstrum;
    int arp[128];

    //for simple mode, the listening time after the first note
    int timezone;

    enum modes
    {
        strum,
        simple
    };

    bool wasplaying;
    bool isplaying;
    bool expectingDelayedNotes;

    inline void resetNote (char n, char v, bool off = false);

    midiStrumProgram* programs;

    VstMidiEventVec midiDelayBuffer;
    void cleanMidiDelayBuffer();
};

#endif
