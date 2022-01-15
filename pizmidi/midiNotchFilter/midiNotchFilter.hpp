/*-----------------------------------------------------------------------------
midiNotchFilter
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDINOTCHFILTER_H
#define __MIDINOTCHFILTER_H

#include "../common/PizMidi.h"

enum
{
    // this is the order the parameters show up in
    kmsg1,
    kch1,
    kvalue1,
    kmsg2,
    kch2,
    kvalue2,
    kmsg3,
    kch3,
    kvalue3,
    kmsg4,
    kch4,
    kvalue4,
    kmode,
    koutch,
    kNumParams,
    kNumPrograms = 16
};

class MidiNotchFilterProgram
{
    friend class MidiNotchFilter;

public:
    MidiNotchFilterProgram();
    ~MidiNotchFilterProgram() {}

private:
    float fmsg[4];
    float fch[4];
    float fvalue[4];
    float fmode;
    float foutch;
    char name[kVstMaxProgNameLen];
};

class MidiNotchFilter : public PizMidi
{
public:
    MidiNotchFilter (audioMasterCallback audioMaster);
    ~MidiNotchFilter();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    float fmsg[4];
    float fch[4];
    float fvalue[4];
    float fmode;
    float foutch;

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    MidiNotchFilterProgram* programs;
};

#endif
