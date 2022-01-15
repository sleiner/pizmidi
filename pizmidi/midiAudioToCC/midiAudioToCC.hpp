/*-----------------------------------------------------------------------------
midiAudioToCC
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDIAUDIOTOCC_H
#define __MIDIAUDIOTOCC_H

#include "../common/PizMidi.h"

enum
{
    kGain,
    kCC,
    kCC2,
    kChannel,
    kMode,
    kOffset,
    kRate,
    kEnvelopeL,
    kEnvelopeR,
    kNumParams,
    kNumPrograms = 16
};

class MidiAudioToCCProgram
{
    friend class MidiAudioToCC;

public:
    MidiAudioToCCProgram();
    ~MidiAudioToCCProgram() {}

private:
    float fGain;
    float fCC;
    float fCC2;
    float fChannel;
    float fMode;
    float fOffset;
    float fRate;
    char name[kVstMaxProgNameLen];
};

class MidiAudioToCC : public PizMidi
{
public:
    MidiAudioToCC (audioMasterCallback audioMaster);
    ~MidiAudioToCC();

    virtual void process (float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterLabel (VstInt32 index, char* label);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    float fGain;
    float fCC;
    float fCC2;
    float fMode;
    float fChannel;
    float fOffset;
    float fRate;
    float fEnvelopeL;
    float fEnvelopeR;

    unsigned int numChannels; // remember the current number of channels being analyzed
    unsigned int samp[2];
    double guiContinualRMS[2]; // the accumulation for continual RMS for GUI display
    double continualRMS[2];
    int oldenv[2];
    int olderenv[2];
    int smooth (int dnew, int old, int older, float inertia);

    virtual void preProcess (void);
    float _bpm;

    MidiAudioToCCProgram* programs;
};

#endif
