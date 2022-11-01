#ifndef DEMOJUCEPLUGINFILTER_H
#define DEMOJUCEPLUGINFILTER_H

#include "../_common/PizAudioProcessor.h"

#define denorm 0.000000000000001f

//==============================================================================
/**
    A simple plugin filter that just applies a gain change to the audio
    passing through it.

*/
class WebBrowserFilter : public PizAudioProcessor,
                         public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    WebBrowserFilter();
    ~WebBrowserFilter() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(juce::AudioSampleBuffer& buffer,
                      juce::MidiBuffer& midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const juce::String getName() const override;

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

    bool acceptsMidi() const override;
    bool producesMidi() const override;

    double getTailLengthSeconds() const override
    {
        return 0;
    }

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

    juce::String getURL()
    {
        return URL;
    }

    void setURL(const juce::String& newURL)
    {
        URL = newURL;
    }

    bool initialPageLoaded;

private:
    // this is our gain - the UI and the host can access this by getting/setting
    // parameter 0.
    float gain;
    juce::String URL;
    float lastinL;
    float lastoutL;
    float lastinR;
    float lastoutR;
    float R;

    JUCE_LEAK_DETECTOR(WebBrowserFilter)
};

#endif
