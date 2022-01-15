/*-----------------------------------------------------------------------------
midiConverter3
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __MIDICONVERTER3_H
#define __MIDICONVERTER3_H

#include "../common/PizMidi.h"

enum
{
    kIn,
    kCCin,
    kNRPNin,
    kChin,
    kLowLimit,
    kHighLimit,
    kRangeMode,
    kLow,
    kHigh,
    kOffset,
    kOut,
    kCCout,
    kNRPNout,
    kChout,
    kThru,

    kNumParams,
    kNumPrograms = 128,
};

class MidiConverterProgram
{
    friend class MidiConverter;

public:
    MidiConverterProgram();
    ~MidiConverterProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiConverter : public PizMidi
{
public:
    MidiConverter (audioMasterCallback audioMaster);
    ~MidiConverter();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

    virtual void resume();

protected:
    float param[kNumParams];

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    int nrpn[16];
    int nrpncoarse[16];
    int rpn[16];
    int rpncoarse[16];
    int datacoarse[16];
    int datafine[16];
    int data[16];

    int cc14msb[32][16];
    int cc14lsb[32][16];

    enum modes
    {
        drop,
        cc,
        cc14,
        pc,
        pcinc,
        pcdec,
        cp,
        pa,
        pb,
        pblsb,
        NRPN,
        NRPNlsb,
        RPN,
        RPNlsb,
        nonn,
        nonv,
        noffn,
        noffv,
        clock,
        songposition,
        songselect,
        undefined
    };
    int inmode, outmode;

    MidiConverterProgram* programs;

    VstInt32 counter;
    int smooth (int newvalue, int oldvalue, float inertia, bool is14bit = true);
    bool done[16];

    int lastpb;
    int targetpb;
    int smoothcc[16];
    int lastcc;
    int targetcc;

    int lastPC[16];
    int lastSentPC[16];
};

#endif
