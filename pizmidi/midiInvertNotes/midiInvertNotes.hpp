/*-----------------------------------------------------------------------------
midiInvertNotes
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDIINVERTNOTES_H
#define __MIDIINVERTNOTES_H

#include "../common/PizMidi.h"

enum
{
    kMirror,
    kPower,
    kNumParams,
    kNumPrograms = 16
};

class MidiInvertNotesProgram
{
    friend class MidiInvertNotes;

public:
    MidiInvertNotesProgram();
    ~MidiInvertNotesProgram() {}

private:
    float fMirror;
    float fPower;
    char name[kVstMaxProgNameLen];
};

class MidiInvertNotes : public PizMidi
{
public:
    MidiInvertNotes (audioMasterCallback audioMaster);
    ~MidiInvertNotes();

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
    float fPower;

    //remember the transposition of every note on every channel
    short transposed[128][16];

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    MidiInvertNotesProgram* programs;
};

#endif
