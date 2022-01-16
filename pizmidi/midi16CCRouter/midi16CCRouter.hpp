/*-----------------------------------------------------------------------------
midi16CCRouter VST 1.1
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDI16CCROUTER_H
#define __MIDI16CCROUTER_H

#include "../common/PizMidi.h"

enum
{
    //parameters appear in this order
    numCCs       = 16,
    kChi         = numCCs * 2,
    kCho         = numCCs * 2 + 1,
    kPC          = numCCs * 2 + 2,
    kNumParams   = numCCs * 2 + 3, //conveniently counts the parameters
    kNumPrograms = 128
};

class Midi16CCRouterProgram
{
    friend class Midi16CCRouter;

public:
    Midi16CCRouterProgram();
    ~Midi16CCRouterProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen + 1];
};

class Midi16CCRouter : public PizMidi
{
public:
    Midi16CCRouter (audioMasterCallback audioMaster);
    ~Midi16CCRouter();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    //parameters
    float param[kNumParams];

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    Midi16CCRouterProgram* programs;
    bool automated;
};

#endif
