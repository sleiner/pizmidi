/*-----------------------------------------------------------------------------
midiSostenuto
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDISOSTENUTO_H
#define __MIDISOSTENUTO_H

#include "../common/PizMidi.h"

enum
{
    kParam01,
    kParam02,
    kParam03,
    kParam04,
    kParam05,
    kNumParams,
    kNumPrograms = 1
};

class MidiSostenutoProgram
{
    friend class MidiSostenuto;

public:
    MidiSostenutoProgram();
    ~MidiSostenutoProgram() {}

private:
    float fParam01;
    float fParam02;
    float fParam03;
    float fParam04;
    float fParam05;
    char name[kVstMaxProgNameLen];
};

class MidiSostenuto : public PizMidi
{
public:
    MidiSostenuto (audioMasterCallback audioMaster);
    ~MidiSostenuto();

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

    float fParam01;
    float fParam02;
    float fParam03;
    float fParam04;
    float fParam05;

    short sostenuto;
    short discard;
    short held_notes[128];
    short sustained_notes[128];
    short noteon_queue[128];
    short noteon_queue_velocity[128];
    short CCvalue_current;
    short CCvalue_prev;

    static const short PEDAL_THRESHOLD = 64; //threshold for the pedal to be "down"

    MidiSostenutoProgram* programs;
};

#endif
