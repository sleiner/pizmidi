/*-----------------------------------------------------------------------------
midiNoteToggle
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDINOTETOGGLE_H
#define __MIDINOTETOGGLE_H

#include "../common/PizMidi.h"

enum
{
    kPower,
    kLowNote,
    kHighNote,
    kChannel,
    kNumParams,
    kNumPrograms = 128
};

class MidiNoteToggle : public PizMidi
{
public:
    MidiNoteToggle (audioMasterCallback audioMaster);
    ~MidiNoteToggle();

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

    float param[kNumParams];

    bool held_notes[128];

    CFxBank* programs;
};

#endif
