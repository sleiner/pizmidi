/*-----------------------------------------------------------------------------
midiAlias VST
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDIALIAS_H
#define __MIDIALIAS_H

#include "../common/PizMidi.h"

enum
{
    kMirror,
    kZero,
    kShift,
    kPower,
    kNumParams,
    kNumPrograms = 16
};

class MidiAliasProgram
{
    friend class MidiAlias;

public:
    MidiAliasProgram();
    ~MidiAliasProgram() {}

private:
    float fMirror;
    float fZero;
    float fShift;
    float fPower;
    char name[24];
};

class MidiAlias : public PizMidi
{
public:
    MidiAlias (audioMasterCallback audioMaster);
    ~MidiAlias();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    float fMirror;
    float fZero;
    float fShift;
    float fPower;

    //remember the transposition of every note on every channel
    short transposed[128][16];

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    MidiAliasProgram* programs;
};

#endif
