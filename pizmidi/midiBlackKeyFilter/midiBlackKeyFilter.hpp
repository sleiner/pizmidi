/*-----------------------------------------------------------------------------
midiBlackKeyFilter
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDIBLACKKEYFILTER_H
#define __MIDIBLACKKEYFILTER_H

#include "../common/PizMidi.h"

enum parameters
{
    //parameters appear in this order
    kInChannel,
    kWhiteChannel,
    kWhiteVelocity,
    kBlackChannel,
    kBlackVelocity,
    kNumParams, //conveniently counts the parameters
    kNumPrograms = 16
};

class MidiBlackKeyFilterProgram
{
    friend class MidiBlackKeyFilter;

public:
    MidiBlackKeyFilterProgram();
    ~MidiBlackKeyFilterProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen + 1];
};

class MidiBlackKeyFilter : public PizMidi
{
public:
    MidiBlackKeyFilter (audioMasterCallback audioMaster);
    ~MidiBlackKeyFilter();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    MidiBlackKeyFilterProgram* programs;

    //parameters
    float param[kNumParams];

    int playingOnChannel[128][16];
};

#endif
