#ifndef MIDIINPLUGINFILTER_H
#define MIDIINPLUGINFILTER_H

#include <memory>

#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_core/juce_core.h"
#include "juce_events/juce_events.h"

#include "../_common/PizArray.h"
#include "../_common/PizAudioProcessor.h"

using namespace juce;

enum
{
    kPower,
    kHostIn,
    kChannel,
    numParams
};

class JuceProgram
{
    friend class MidiInFilter;

public:
    JuceProgram();
    ~JuceProgram() {}

private:
    float param[numParams];
    String icon;
    MidiDeviceInfo device;
    String name;
};

//==============================================================================
class MidiInFilter : public PizAudioProcessor,
                     public ChangeBroadcaster
{
public:
    //==============================================================================
    MidiInFilter();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const String getName() const override { return JucePlugin_Name; }
    bool hasEditor() const override { return true; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    double getTailLengthSeconds() const override { return 0; }

    int getNumParameters() override;

    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const String getParameterName (int index) override;
    const String getParameterText (int index) override;

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void getCurrentProgramStateInformation (MemoryBlock& destData) override;
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    PizArray<MidiDeviceInfo> devices;
    String icon;

    void setActiveDevice (String name);
    void setActiveDevice (MidiDeviceInfo device);
    MidiDeviceInfo getActiveDevice() { return activeDevice; }
    MidiDeviceInfo getDeviceByName (String name) const;

    //==============================================================================
    juce_UseDebuggingNewOperator

        private :
        // this is our gain - the UI and the host can access this by getting/setting
        // parameter 0.
        float param[numParams];
    MidiDeviceInfo activeDevice;

    JuceProgram* programs;
    int curProgram;
    bool init;
    std::unique_ptr<MidiInput> midiInput;
    MidiMessageCollector collector;

    bool wasPlaying;

    AudioPlayHead::CurrentPositionInfo lastPosInfo;
};

#endif
