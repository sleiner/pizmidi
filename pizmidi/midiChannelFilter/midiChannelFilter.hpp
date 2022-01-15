/*-----------------------------------------------------------------------------
midiChannelFilter
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDICHANNELFILTER_H
#define __MIDICHANNELFILTER_H

#include "../common/PizMidi.h"

enum
{
    kChannel,
    kNumParams,
    kNumPrograms = 16
};

class MidiChannelFilterProgram
{
    friend class MidiChannelFilter;

public:
    MidiChannelFilterProgram();
    ~MidiChannelFilterProgram() {}

private:
    float fChannel;
    char name[kVstMaxProgNameLen + 1];
};

class MidiChannelFilter : public PizMidi
{
public:
    MidiChannelFilter (audioMasterCallback audioMaster);
    ~MidiChannelFilter();

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

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    MidiChannelFilterProgram* programs;
};

#endif
