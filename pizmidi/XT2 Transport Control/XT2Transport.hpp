/*-----------------------------------------------------------------------------
XT2 Transport Control
by Reuben Vinal
-----------------------------------------------------------------------------*/

#ifndef __XT2TRANSPORTCONTROL_H
#define __XT2TRANSPORTCONTROL_H

#include "../common/PizMidi.h"

#define XT_WINDOW_TITLE "energyXT"
#define XT_EXE_NAME     "energyXT.exe"

enum
{
    kPauseNote,
    kPauseMsg,
    kStopNote,
    kStopMsg,
    kRewindNote,
    kRewindMsg,
    kRecordNote,
    kRecordMsg,
    kChannel,
    kNumParams,
    kNumPrograms = 16
};

class XT2TransportControlProgram
{
    friend class XT2TransportControl;

public:
    XT2TransportControlProgram();
    ~XT2TransportControlProgram() {}

private:
    float fPauseNote;
    float fPauseMsg;
    float fStopNote;
    float fStopMsg;
    float fRewindNote;
    float fRewindMsg;
    float fRecordNote;
    float fRecordMsg;
    float fChannel;
    char name[32];
};

class XT2TransportControl : public PizMidi
{
public:
    XT2TransportControl (audioMasterCallback audioMaster);
    ~XT2TransportControl();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    float fPauseNote;
    float fPauseMsg;
    float fStopNote;
    float fStopMsg;
    float fRewindNote;
    float fRewindMsg;
    float fRecordNote;
    float fRecordMsg;
    float fChannel;

    void XT2_Transport (int button);

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    XT2TransportControlProgram* programs;
};

#endif
