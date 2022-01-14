#ifndef __MIDIDUPLICATEBLOCKER_H
#define __MIDIDUPLICATEBLOCKER_H

#include "../common/PizMidi.h"

enum
{
    //parameters appear in this order
    kPower,
    kChannel,
    kTimer,
    kNumParams, //conveniently counts the parameters
    kNumPrograms = 8
};

class MidiDuplicateBlockerProgram
{
    friend class MidiDuplicateBlocker;

public:
    MidiDuplicateBlockerProgram();
    ~MidiDuplicateBlockerProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen + 1];
};

class MidiDuplicateBlocker : public PizMidi
{
public:
    MidiDuplicateBlocker (audioMasterCallback audioMaster);
    ~MidiDuplicateBlocker();

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
    //parameters
    float param[kNumParams];

    short lastCC[16][128];
    short lastNote[16];
    short lastProgCh[16];
    short lastPolyAft[16][128];
    short lastChanAft[16];
    unsigned short lastPB[16];

    unsigned long long CCtimer[16][128];
    unsigned long long Notetimer[16];
    unsigned long long ProgChtimer[16];
    unsigned long long PolyAfttimer[16][128];
    unsigned long long ChanAfttimer[16];
    unsigned long long PBtimer[16];

    bool playing[16][128];

    unsigned int time;
    unsigned long long totalTime;

    enum modes
    {
        off,
        cc,
        note,
        pb,
        pc,
        cp,
        aft,
        all
    };
    char mode;

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    MidiDuplicateBlockerProgram* programs;
};

#endif
