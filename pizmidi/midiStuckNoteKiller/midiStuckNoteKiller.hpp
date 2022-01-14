/*-----------------------------------------------------------------------------
midiStuckNoteKiller
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDISTUCKNOTEKILLER_H
#define __MIDISTUCKNOTEKILLER_H

#include "../common/PizMidi.h"

enum
{
    kParam01,
    kNumParams
};

class midiStuckNoteKiller : public PizMidi
{
public:
    midiStuckNoteKiller (audioMasterCallback audioMaster);
    ~midiStuckNoteKiller();

    void setProgramName (char* name) { vst_strncpy (_programName, name, kVstMaxProgNameLen); }
    void getProgramName (char* name) { vst_strncpy (name, _programName, kVstMaxProgNameLen); }
    bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
    {
        vst_strncpy (text, _programName, kVstMaxProgNameLen);
        return true;
    }

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);
    virtual void preProcess (void);

    float fParam01;

    bool held_notes[128][16];
    bool trigger;

    char _programName[32];
};

#endif
