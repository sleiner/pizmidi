/*-----------------------------------------------------------------------------
midiFingered
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDIFINGERED_H
#define __MIDIFINGERED_H

#include "../common/PizMidi.h"

enum
{
    kPower,
    kChannel,
    kNumParams,
    kNumPrograms = 1
};

class MidiFingered : public PizMidi
{
public:
    MidiFingered (audioMasterCallback audioMaster);
    ~MidiFingered();

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

    virtual void setProgramName (char* name) { vst_strncpy (_programName, name, kVstMaxProgNameLen); }
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

protected:
    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    float param[kNumParams];

    bool held_notes[128][16];
    short voices[16];
    char _programName[kVstMaxProgNameLen];
};

#endif
