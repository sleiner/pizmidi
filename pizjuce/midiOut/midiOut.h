#ifndef MIDIOUTPLUGINFILTER_H
#define MIDIOUTPLUGINFILTER_H

#include <memory>

#include <juce_audio_devices/juce_audio_devices.h>

#include "../_common/PizArray.h"
#include "../_common/PizAudioProcessor.h"

enum parameters
{
    kPower,
    kClock,
    kHostOut,
    kMTC,
    kStamped,
    kChannel,
    numParams
};

class JuceProgram
{
    friend class MidiOutFilter;

public:
    JuceProgram();

    ~JuceProgram()
    {
    }

private:
    float param[numParams];
    juce::String icon;
    juce::MidiDeviceInfo device;
    juce::String name;
};

//==============================================================================
/**
    A simple plugin filter that just applies a gain change to the audio
    passing through it.

*/
class MidiOutFilter : public PizAudioProcessor,
                      public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    MidiOutFilter();
    ~MidiOutFilter() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(juce::AudioSampleBuffer& buffer,
                      juce::MidiBuffer& midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;

    //==============================================================================
    double getTailLengthSeconds() const override
    {
        return 0;
    }

    const juce::String getName() const override
    {
        return JucePlugin_Name;
    }

    bool acceptsMidi() const override
    {
        return true;
    }

    bool producesMidi() const override
    {
        return true;
    }

    bool hasEditor() const override
    {
        return true;
    }

    int getNumParameters() override;

    float getParameter(int index) override;
    void setParameter(int index, float newValue) override;

    const juce::String getParameterName(int index) override;
    const juce::String getParameterText(int index) override;

    const juce::String getInputChannelName(int channelIndex) const override;
    const juce::String getOutputChannelName(int channelIndex) const override;
    bool isInputChannelStereoPair(int index) const override;
    bool isOutputChannelStereoPair(int index) const override;

    //==============================================================================
    int getNumPrograms() override
    {
        return 1;
    }

    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    void getCurrentProgramStateInformation(juce::MemoryBlock& destData) override;
    void setCurrentProgramStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    PizArray<juce::MidiDeviceInfo> devices;
    juce::String icon;

    void setActiveDevice(juce::String name);
    void setActiveDevice(juce::MidiDeviceInfo device);

    juce::MidiDeviceInfo getActiveDevice()
    {
        return activeDevice;
    }

    juce::MidiDeviceInfo getDeviceByName(juce::String name) const;

private:
    // this is our gain - the UI and the host can access this by getting/setting
    // parameter 0.
    float param[numParams];
    juce::MidiDeviceInfo activeDevice;

    JuceProgram* programs;
    int curProgram;
    bool init;
    std::unique_ptr<juce::MidiOutput> midiOutput;

    double startAt;
    double startMTCAt;
    bool sendclock;
    bool sendmtc;
    int mtcNumber;

    bool wasPlaying;
    int samplesToNextClock;
    int samplesToNextMTC;

    juce::AudioPlayHead::CurrentPositionInfo lastPosInfo;

    JUCE_LEAK_DETECTOR(MidiOutFilter)
};

#endif
