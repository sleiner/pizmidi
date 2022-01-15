/*
Originally based on:
  MIDI Gain 1.02
  David Viens
  davidv@plogue.com
*/

#ifndef __PIZ_MIDI_PLUGIN_H
#define __PIZ_MIDI_PLUGIN_H

#include "MIDI.h"
#include "PizPluginInfo.h"
#include "pizvstbase.h"
#include "public.sdk/source/vst2.x/audioeffectx.h"
#include <algorithm>

class PizMidi : public AudioEffectX
{
public:
    PizMidi (audioMasterCallback audioMaster, VstInt32 numPrograms, VstInt32 numParams);
    ~PizMidi();

    virtual VstInt32 processEvents (VstEvents* events);
    virtual void process (float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);

    virtual void setProgramName (char* name) = 0;
    virtual void getProgramName (char* name) = 0;
    virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text) = 0;

    virtual void setParameter (VstInt32 index, float value) = 0;
    virtual void setParameterAutomated (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index) = 0;
    virtual void getParameterLabel (VstInt32 index, char* label);
    virtual void getParameterDisplay (VstInt32 index, char* text) = 0;
    virtual void getParameterName (VstInt32 index, char* text) = 0;

    virtual bool getVendorString (char* text);
    virtual bool getProductString (char* text);
    virtual VstInt32 getVendorVersion();
    virtual bool getEffectName (char* name);

    virtual void setSampleRate (float sampleRate);
    virtual void setBlockSize (VstInt32 blockSize);
    virtual void resume();

    virtual VstInt32 canDo (char* text);
    virtual bool getInputProperties (VstInt32 index, VstPinProperties* properties);
    virtual bool getOutputProperties (VstInt32 index, VstPinProperties* properties);
    virtual VstInt32 getNumMidiInputChannels()
    {
#if PLUG_MIDI_INPUTS
        return 16;
#else
        return 0;
#endif
    }

    virtual VstInt32 getNumMidiOutputChannels()
    {
#if PLUG_MIDI_OUTPUTS
        return 16;
#else
        return 0;
#endif
    }

protected:
    bool init();
    virtual void processMidiEvents (VstMidiEventVec* inputs, VstMidiEventVec* outputs, VstInt32 sampleFrames) {}

    virtual void preProcess();
    virtual void postProcess();

    void copySysex();

    VstMidiEventVec* _midiEventsIn;
    VstSysexEventVec* _midiSysexEventsIn;
    void _cleanMidiInBuffers();

    VstMidiEventVec* _midiEventsOut;
    VstSysexEventVec* _midiSysexEventsOut;
    void _cleanMidiOutBuffers();

    MyVstEvents* _vstEventsToHost;
    VstMidiEvent* _vstMidiEventsToHost;
    VstMidiSysexEvent* _vstSysexEventsToHost;

    int numinputs, numoutputs, bottomOctave;

    static bool midiSort (VstMidiEvent first, VstMidiEvent second)
    {
        return first.deltaFrames < second.deltaFrames;
    }

    static void sortMidiEvents (VstMidiEventVec _vec)
    {
        std::stable_sort (_vec.begin(), _vec.end(), midiSort);
    }
};

#endif
