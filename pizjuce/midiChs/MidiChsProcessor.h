#ifndef MIDICHSPLUGINFILTER_H
#define MIDICHSPLUGINFILTER_H

#include "../_common/PizAudioProcessor.h"

#include <juce_events/juce_events.h>

//==============================================================================
class MidiChsProcessor : public PizAudioProcessor,
                         public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    MidiChsProcessor();
    ~MidiChsProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(juce::AudioSampleBuffer& buffer,
                      juce::MidiBuffer& midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;

    //==============================================================================
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
        return 0;
    }

    int getCurrentProgram() override
    {
        return 0;
    }

    void setCurrentProgram(int index) override
    {
    }

    const juce::String getProgramName(int index) override
    {
        return juce::String();
    }

    void changeProgramName(int index, const juce::String& newName) override
    {
    }

    double getTailLengthSeconds() const override
    {
        return 0;
    }

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;

private:
    //parameters
    float fChannel1;
    float fChannel2;
    float fChannel3;
    float fChannel4;
    float fChannel5;
    float fChannel6;
    float fChannel7;
    float fChannel8;
    float fChannel9;
    float fChannel10;
    float fChannel11;
    float fChannel12;
    float fChannel13;
    float fChannel14;
    float fChannel15;
    float fChannel16;
    float fReset;
    float fClear;
    float bghue;
    float bgsat;
    float bgbri;
    float contrast;

    JUCE_LEAK_DETECTOR(MidiChsProcessor)
};

#endif
