/*-----------------------------------------------------------------------------
midiOverlappingNoteKiller
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDIOVERLAPPINGNOTEKILLER_H
#define __MIDIOVERLAPPINGNOTEKILLER_H

#include "../common/PizMidi.h"

enum
{
    kParam01,
    kNumParams
};

class MidiOverlappingNoteKiller : public PizMidi
{
public:
    MidiOverlappingNoteKiller (audioMasterCallback audioMaster);
    ~MidiOverlappingNoteKiller();

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
    void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    float fParam01;

    bool held_notes[128][16];

    char _programName[32];
};

#endif
