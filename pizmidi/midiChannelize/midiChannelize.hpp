/*-----------------------------------------------------------------------------
midiChannelize
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDICHANNELIZE_H
#define __MIDICHANNELIZE_H

#include "../common/PizMidi.h"

enum
{
    kChannel,
    kNumParams,
    kNumPrograms = 16
};

class MidiChannelizeProgram
{
    friend class MidiChannelize;

public:
    MidiChannelizeProgram();
    ~MidiChannelizeProgram() {}

private:
    float fChannel;
    char name[kVstMaxProgNameLen + 1];
};

class MidiChannelize : public PizMidi
{
public:
    MidiChannelize (audioMasterCallback audioMaster);
    ~MidiChannelize();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    float fChannel;

    int lastChannel[16][128];

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    MidiChannelizeProgram* programs;
};

#endif
