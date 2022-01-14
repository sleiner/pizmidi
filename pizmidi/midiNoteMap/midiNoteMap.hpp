/*-----------------------------------------------------------------------------
midiNoteMap
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MidiNoteMap_NOGUI_H
#define __MidiNoteMap_NOGUI_H

#include "../common/PizMidi.h"

enum
{
    kChannel,
    kReset,
    kClear,
    kNotes,

    kNumParams = kNotes + 128,
    kNumPrograms = 128
};

class MidiNoteMapProgram
{
    friend class MidiNoteMap;

public:
    MidiNoteMapProgram();
    ~MidiNoteMapProgram() {}

private:
    float param[kNumParams];
    char name[32];
};

class MidiNoteMap : public PizMidi
{
public:
    MidiNoteMap (audioMasterCallback audioMaster);
    ~MidiNoteMap();

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
    int noteOnValue[16][128];

    void Reset();
    void Clear();

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    MidiNoteMapProgram* programs;
};

#endif
