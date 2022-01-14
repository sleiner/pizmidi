/*-----------------------------------------------------------------------------
midiChs_noGUI
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDICHS_NOGUI_H
#define __MIDICHS_NOGUI_H

#include "../common/PizMidi.h"

enum
{
    kChannel1,
    kChannel2,
    kChannel3,
    kChannel4,
    kChannel5,
    kChannel6,
    kChannel7,
    kChannel8,
    kChannel9,
    kChannel10,
    kChannel11,
    kChannel12,
    kChannel13,
    kChannel14,
    kChannel15,
    kChannel16,
    kReset,
    kClear,
    kNumParams,
    kNumPrograms = 8
};

class MidiChsProgram
{
    friend class MidiChs;

public:
    MidiChsProgram();
    ~MidiChsProgram() {}

private:
    float param[kNumParams];
    char name[32];
};

class MidiChs : public PizMidi
{
public:
    MidiChs (audioMasterCallback audioMaster);
    ~MidiChs();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    float param[kNumParams];

    void Reset();
    void Clear();

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    MidiChsProgram* programs;
};

#endif
