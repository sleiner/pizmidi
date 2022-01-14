#ifndef __MIDINOTESTOCC_H
#define __MIDINOTESTOCC_H

#include "../common/PizMidi.h"

enum
{
    kNCC,
    kNoteHi,
    kNoteLo,
    kNCCHi,
    kNCCLo,
    kNCCoff,
    kVCC,
    kVeloHi,
    kVeloLo,
    kVCCHi,
    kVCCLo,
    kVCCoff,
    kChin,
    kChout,
    kThru,
    kNumParams,
    kNumPrograms = 16
};

class MidiNotesToCCProgram
{
    friend class MidiNotesToCC;

public:
    MidiNotesToCCProgram();
    ~MidiNotesToCCProgram() {}

private:
    float fNCC;
    float fNoteHi;
    float fNoteLo;
    float fNCCHi;
    float fNCCLo;
    float fNCCoff;
    float fVCC;
    float fVeloHi;
    float fVeloLo;
    float fVCCHi;
    float fVCCLo;
    float fVCCoff;
    float fChin;
    float fChout;
    float fThru;
    char name[kVstMaxProgNameLen];
};

class MidiNotesToCC : public PizMidi
{
public:
    MidiNotesToCC (audioMasterCallback audioMaster);
    ~MidiNotesToCC();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    float fNCC;
    float fNoteHi;
    float fNoteLo;
    float fNCCHi;
    float fNCCLo;
    float fNCCoff;
    float fVCC;
    float fVeloHi;
    float fVeloLo;
    float fVCCHi;
    float fVCCLo;
    float fVCCoff;
    float fChin;
    float fChout;
    float fThru;

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    int notePlaying[128][16];
    enum
    {
        sentNone = 0,
        sentCC,
        sentNote,
        sentBoth
    };

    MidiNotesToCCProgram* programs;
};

#endif
