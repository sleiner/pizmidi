/*-----------------------------------------------------------------------------
midiGain example 1.0
by Reuben Vinal

originally based on:
MIDI Gain 1.02
David Viens
davidv@plogue.com
-----------------------------------------------------------------------------*/
#ifndef __PLUGIN_H
#define __PLUGIN_H

#include "MIDI.h"
#include "public.sdk/source/vst2.x/audioeffectx.h"
#include <vector>

typedef std::vector<VstMidiEvent> VstMidiEventVec;

#define MAX_EVENTS_PER_TIMESLICE 256
struct MyVstEvents
{
    VstInt32 numEvents;
    VstInt32 reserved;
    VstEvent* events[MAX_EVENTS_PER_TIMESLICE];
};

enum parameters
{
    kGain,
    kNumParams,
    kNumPrograms = 1
};

//max 1!!! it cant have more than one midi stream in VST 2.x!!
#define PLUG_MIDI_INPUTS 1 //(only 1 or 0!)
#define PLUG_MIDI_OUTPUTS 1 //(only 1 or 0!)

// you absolutely MUST change all the following!!!!////
#define PLUG_IDENT 'mGEx'
#define PLUG_NAME "midiGain"
#define PLUG_VENDOR "Insert Piz Here"

#define PLUG_VERSION 1000
////////////////////////////////////////////////////////

class MidiGainProgram
{
    friend class MidiGain;

public:
    MidiGainProgram();
    ~MidiGainProgram() {}

private:
    float param[kNumParams];
    char name[kVstMaxProgNameLen + 1];
};

class MidiGain : public AudioEffectX
{
public:
    MidiGain (audioMasterCallback audioMaster);
    ~MidiGain();

    virtual void process (float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);

    virtual VstInt32 processEvents (VstEvents* events);

    virtual void setProgram (VstInt32 program);
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterLabel (VstInt32 index, char* label);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);

    virtual bool getVendorString (char* text);
    virtual bool getProductString (char* text);
    virtual bool getEffectName (char* name);
    virtual VstInt32 getVendorVersion();

    virtual void setSampleRate (float sampleRate);
    virtual void setBlockSize (VstInt32 blockSize);

    virtual void resume();

    virtual bool getInputProperties (VstInt32 index, VstPinProperties* properties);
    virtual bool getOutputProperties (VstInt32 index, VstPinProperties* properties);

    virtual VstInt32 canDo (char* text);

protected:
    float param[kNumParams];
    MidiGainProgram* programs;

    bool init();
    void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs);

    void preProcess();
    void postProcess();

    VstMidiEventVec* _midiEventsIn;
    VstMidiEventVec* _midiEventsOut;
    void _cleanMidiInBuffers();
    void _cleanMidiOutBuffers();

    MyVstEvents* _vstEventsToHost;
    VstMidiEvent* _vstMidiEventsToHost;
};

#endif
