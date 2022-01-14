/*-----------------------------------------------------------------------------
midiChordHold
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDICHORDHOLD_H
#define __MIDICHORDHOLD_H

#include "../common/PizMidi.h"

enum
{
    kParam01,
    kChannel,
    kNumParams
};

class MidiChordHold : public PizMidi
{
public:
    MidiChordHold (audioMasterCallback audioMaster);
    ~MidiChordHold();

    void setProgramName (char* name) { vst_strncpy (_programName, name, kVstMaxProgNameLen); }
    void getProgramName (char* name) { vst_strncpy (name, _programName, kVstMaxProgNameLen); }
    bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
    {
        vst_strncpy (text, _programName, kVstMaxProgNameLen);
        return true;
    }

    void setParameter (VstInt32 index, float value);
    float getParameter (VstInt32 index);
    void getParameterDisplay (VstInt32 index, char* text);
    void getParameterName (VstInt32 index, char* text);

protected:
    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    float param[kNumParams];

    bool held_notes[128][16];
    bool waiting;
    bool holding;
    bool wasOn;
    int lastChannel;

    char _programName[kVstMaxProgNameLen];
};

#endif
