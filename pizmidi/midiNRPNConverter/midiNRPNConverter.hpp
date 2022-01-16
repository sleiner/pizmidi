/*-----------------------------------------------------------------------------
midiNRPNConverter
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDINRPNCONVERTER_H
#define __MIDINRPNCONVERTER_H

#include "../common/PizMidi.h"

enum
{
    // this is the order the parameters show up in
    kIn,
    kOut,
    kChin,
    kChout,
    //i*4+5,   i*4+6,     i*4+7,    i*4+8
    kCCin1,
    kNRPNin1,
    kCCout1,
    kNRPNout1,
    kCCin2,
    kNRPNin2,
    kCCout2,
    kNRPNout2,
    kCCin3,
    kNRPNin3,
    kCCout3,
    kNRPNout3,
    kCCin4,
    kNRPNin4,
    kCCout4,
    kNRPNout4,
    kCCin5,
    kNRPNin5,
    kCCout5,
    kNRPNout5,
    kCCin6,
    kNRPNin6,
    kCCout6,
    kNRPNout6,
    kCCin7,
    kNRPNin7,
    kCCout7,
    kNRPNout7,
    kCCin8,
    kNRPNin8,
    kCCout8,
    kNRPNout8,
    kCCin9,
    kNRPNin9,
    kCCout9,
    kNRPNout9,
    kCCin10,
    kNRPNin10,
    kCCout10,
    kNRPNout10,
    kCCin11,
    kNRPNin11,
    kCCout11,
    kNRPNout11,
    kCCin12,
    kNRPNin12,
    kCCout12,
    kNRPNout12,
    kCCin13,
    kNRPNin13,
    kCCout13,
    kNRPNout13,
    kCCin14,
    kNRPNin14,
    kCCout14,
    kNRPNout14,
    kCCin15,
    kNRPNin15,
    kCCout15,
    kNRPNout15,
    kCCin16,
    kNRPNin16,
    kCCout16,
    kNRPNout16,
    kDiscard,
    kNumParams,
    kNumSlots    = 16,
    kNumPrograms = 128,
};

class MidiNRPNConverterProgram
{
    friend class MidiNRPNConverter;

public:
    MidiNRPNConverterProgram();
    ~MidiNRPNConverterProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen];
};

class MidiNRPNConverter : public PizMidi
{
public:
    MidiNRPNConverter (audioMasterCallback audioMaster);
    ~MidiNRPNConverter();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    float param[kNumParams];

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    int chout;
    int nrpn[16];
    int nrpncoarse[16];
    int rpn[16];
    int rpncoarse[16];
    int datacoarse[16];
    int datafine[16];
    int data[16];

    MidiNRPNConverterProgram* programs;
};

#endif
