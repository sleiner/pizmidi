#ifndef IMAGEPLUGINFILTER_H
#define IMAGEPLUGINFILTER_H

#include <juce_data_structures/juce_data_structures.h>

#include "../_common/BankStorage.h"
#include "../_common/PizAudioProcessor.h"

enum parameters
{
    kChannel,
    kNumParams
};

class ImageBank : public BankStorage
{
public:
    ImageBank();

private:
    void loadDefaultValues();
};

//==============================================================================
/**
    A simple plugin filter that just applies a gain change to the audio
    passing through it.

*/
class imagePluginFilter : public PizAudioProcessor,
                          public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    imagePluginFilter();
    ~imagePluginFilter() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer& buffer,
                       juce::MidiBuffer& midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const juce::String getName() const override
    {
        return JucePlugin_Name;
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
    bool hasEditor() const override
    {
        return true;
    }
    double getTailLengthSeconds() const override
    {
        return 0;
    }

    int getNumParameters() override
    {
        return kNumParams;
    }

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
        return 128;
    }
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getCurrentProgramStateInformation (juce::MemoryBlock& destData) override;
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes) override;
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void setBankColours (juce::Colour colour, juce::Colour text);
    void applySizeToBank (int h, int w);
    void clearAllImages();

    void setCurrentBank (int index, int program = -1);
    int getCurrentBank()
    {
        return curBank;
    }

    void setNoteInput (bool use)
    {
        noteInput = use;
        programs->setGlobal ("noteInput", use);
    }

    void setUsePC (bool use)
    {
        usePC = use;
        programs->setGlobal ("usePC", use);
    }

    bool getUsePC()
    {
        return usePC;
    }
    bool getNoteInput()
    {
        return noteInput;
    }

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;
    juce::Colour bgcolor;
    juce::Colour textcolor;
    juce::String icon;
    juce::String text;
    bool fullscreen;
    juce::String iconPath;

    //==============================================================================

private:
    float param[kNumParams];

    //JuceProgram *programs;
    ImageBank* programs;
    int curBank;
    int curProgram;
    bool noteInput;
    bool usePC;

    bool init;

    JUCE_LEAK_DETECTOR (imagePluginFilter)
};

#endif
