/*-----------------------------------------------------------------------------
midiCCToggle
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDICCTOGGLE_H
#define __MIDICCTOGGLE_H

#include "../common/PizMidi.h"

enum
{
    kParam01,
    kParam02,
    kParam03,
    kParam05,
    kParam06,
    kParam07,
    kNumParams,
    kNumPrograms = 16
};

class MidiCCToggle : public PizMidi
{
public:
    MidiCCToggle (audioMasterCallback audioMaster);
    ~MidiCCToggle();

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

    float fParam01;
    float fParam02;
    float fParam03;
    float fParam05;
    float fParam06;
    float fParam07;

    bool held_notes[128][16];

    CFxBank* programs;
};

#endif
