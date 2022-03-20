#ifndef IMAGEPLUGINFILTER_H
#define IMAGEPLUGINFILTER_H

#include "juce_data_structures/juce_data_structures.h"

#include "piz/BankStorage.h"
#include "piz/PizAudioProcessor.h"

using namespace juce;

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
                          public ChangeBroadcaster
{
public:
    //==============================================================================
    imagePluginFilter();
    ~imagePluginFilter() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const String getName() const override { return JucePlugin_Name; }
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
    bool hasEditor() const override { return true; }
    double getTailLengthSeconds() const override { return 0; }

    int getNumParameters() override { return kNumParams; }

    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const String getParameterName (int index) override;
    const String getParameterText (int index) override;

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    //==============================================================================

    int getNumPrograms() override { return 128; }
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getCurrentProgramStateInformation (MemoryBlock& destData) override;
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes) override;
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void setBankColours (Colour colour, Colour text);
    void applySizeToBank (int h, int w);
    void clearAllImages();

    void setCurrentBank (int index, int program = -1);
    int getCurrentBank() { return curBank; }

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

    bool getUsePC() { return usePC; }
    bool getNoteInput() { return noteInput; }

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;
    Colour bgcolor;
    Colour textcolor;
    String icon;
    String text;
    bool fullscreen;
    String iconPath;

    //==============================================================================
    juce_UseDebuggingNewOperator

        private : float param[kNumParams];

    //JuceProgram *programs;
    ImageBank* programs;
    int curBank;
    int curProgram;
    bool noteInput;
    bool usePC;

    bool init;
};

#endif
