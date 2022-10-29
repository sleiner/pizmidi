#ifndef CPURAMPLUGINFILTER_H
#define CPURAMPLUGINFILTER_H
#define _WIN32_WINNT 0x0501

#include <juce_events/juce_events.h>

#include "../_common/PizAudioProcessor.h"

//==============================================================================
class CpuRam : public PizAudioProcessor,
               public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    CpuRam();
    ~CpuRam() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer& buffer,
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
    bool hasEditor() const override
    {
        return true;
    }
    bool acceptsMidi() const override
    {
#if JucePlugin_WantsMidiInput
        return true;
#else
        return false;
#endif
    }
    bool producesMidi() const override
    {
#if JucePlugin_ProducesMidiOutput
        return true;
#else
        return false;
#endif
    }

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
    int getNumPrograms() override
    {
        return 1;
    }
    int getCurrentProgram() override
    {
        return 0;
    }
    void setCurrentProgram (int index) override
    {
    }
    const juce::String getProgramName (int index) override
    {
        return juce::String();
    }
    void changeProgramName (int index, const juce::String& newName) override
    {
    }

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void resetToDefaultSettings();

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    juce::Colour bgcolor;
    bool showGraph;

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;

private:
    // this is our interval - the UI and the host can access this by getting/setting
    // parameter 0.
    float interval;
    float minimize;

    JUCE_LEAK_DETECTOR (CpuRam)
};

#endif
