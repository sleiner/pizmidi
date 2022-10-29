#ifndef MIDIINPLUGINFILTER_H
#define MIDIINPLUGINFILTER_H

#include <memory>

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>

#include "../_common/PizArray.h"
#include "../_common/PizAudioProcessor.h"

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
    juce::String icon;
    juce::MidiDeviceInfo device;
    juce::String name;
};

//==============================================================================
class MidiInFilter : public PizAudioProcessor,
                     public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    MidiInFilter();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer& buffer,
                       juce::MidiBuffer& midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const juce::String getName() const override { return JucePlugin_Name; }
    bool hasEditor() const override { return true; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    double getTailLengthSeconds() const override { return 0; }

    int getNumParameters() override;

    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const juce::String getParameterName (int index) override;
    const juce::String getParameterText (int index) override;

    const juce::String getInputChannelName (int channelIndex) const override;
    const juce::String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void getCurrentProgramStateInformation (juce::MemoryBlock& destData) override;
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    PizArray<juce::MidiDeviceInfo> devices;
    juce::String icon;

    void setActiveDevice (juce::String name);
    void setActiveDevice (juce::MidiDeviceInfo device);
    juce::MidiDeviceInfo getActiveDevice() { return activeDevice; }
    juce::MidiDeviceInfo getDeviceByName (juce::String name) const;

private:
    // this is our gain - the UI and the host can access this by getting/setting
    // parameter 0.
    float param[numParams];
    juce::MidiDeviceInfo activeDevice;

    JuceProgram* programs;
    int curProgram;
    bool init;
    std::unique_ptr<juce::MidiInput> midiInput;
    juce::MidiMessageCollector collector;

    bool wasPlaying;

    juce::AudioPlayHead::CurrentPositionInfo lastPosInfo;

    JUCE_LEAK_DETECTOR (MidiInFilter)
};

#endif
