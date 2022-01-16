/*-----------------------------------------------------------------------------
midi2qwerty16 VST
by Reuben Vinal
-----------------------------------------------------------------------------*/
#ifndef __MIDI2QWERTY16_H
#define __MIDI2QWERTY16_H

#include "../common/PizMidi.h"
#include <string>

enum
{
    kMsg1,
    kNote1,
    kMod1,
    kKey1, // 0-3
    kMsg2,
    kNote2,
    kMod2,
    kKey2, // 4-7
    kMsg3,
    kNote3,
    kMod3,
    kKey3, // 8-11
    kMsg4,
    kNote4,
    kMod4,
    kKey4, //12-15
    kMsg5,
    kNote5,
    kMod5,
    kKey5, //16-19
    kMsg6,
    kNote6,
    kMod6,
    kKey6, //20-23
    kMsg7,
    kNote7,
    kMod7,
    kKey7, //24-27
    kMsg8,
    kNote8,
    kMod8,
    kKey8, //28-31
    kMsg9,
    kNote9,
    kMod9,
    kKey9, //32-35
    kMsg10,
    kNote10,
    kMod10,
    kKey10, //36-39
    kMsg11,
    kNote11,
    kMod11,
    kKey11, //40-43
    kMsg12,
    kNote12,
    kMod12,
    kKey12, //44-47
    kMsg13,
    kNote13,
    kMod13,
    kKey13, //48-51
    kMsg14,
    kNote14,
    kMod14,
    kKey14, //52-55
    kMsg15,
    kNote15,
    kMod15,
    kKey15, //56-59
    kMsg16,
    kNote16,
    kMod16,
    kKey16, //60-63
    kChannel,
    kProg,
    kNumParams,
    kNumPrograms = 128,
    kNumSlots    = 16
};

class midi2qwertyProgram
{
    friend class midi2qwerty;

public:
    midi2qwertyProgram();
    ~midi2qwertyProgram() {}

private:
    float param[kNumParams - 1];
    char name[32];
};

class midi2qwerty : public PizMidi
{
public:
    midi2qwerty (audioMasterCallback audioMaster);
    ~midi2qwerty();

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

protected:
    enum msgTypes
    {
        msgOff,
        msgNote,
        msgPCUp,
        msgPCDn,
        msgCCMax,
        msgCCMin,
        msgCCHigh,
        msgCCLow,
        msgCCSliderUp,
        msgCCSliderDn
    };

    float param[kNumParams];
    bool settingprog;

    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames);

    void key2 (int vkeycode, int modifiers, bool keyup = false, bool instantaneous = false);
    void keystroke (int vkeycode);
    //void mouseclick(int button);
    bool keydown[kNumSlots];

    int lastPC[16];

    midi2qwertyProgram* programs;
};

#endif
