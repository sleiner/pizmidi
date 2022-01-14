/*-----------------------------------------------------------------------------
midiPitchBendToNotes
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDIPITCHBENDTONOTES_H
#define __MIDIPITCHBENDTONOTES_H

#include "../common/PizMidi.h"

enum
{
    kPower,
    kChannel,
    kNumParams,
    kNumPrograms = 1
};

#define NOT_PLAYING (-999)

class MidiPitchBendToNotesProgram
{
    friend class MidiPitchBendToNotes;

public:
    MidiPitchBendToNotesProgram();
    ~MidiPitchBendToNotesProgram() {}

private:
    float fPower;
    float fChannel;
    char name[kVstMaxProgNameLen + 1];
};

class MidiPitchBendToNotes : public PizMidi
{
public:
    MidiPitchBendToNotes (audioMasterCallback audioMaster);
    ~MidiPitchBendToNotes();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    float fPower;
    float fOutPBRange;
    float fChannel;

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    MidiPitchBendToNotesProgram* programs;

    float pbrange;
    int transpose[16];
    int lastTranspose[128][16];
    int rpn[16];
    int rpncoarse[16];
};

#endif
