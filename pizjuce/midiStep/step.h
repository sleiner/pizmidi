#ifndef MidiStepPLUGINFILTER_H
#define MidiStepPLUGINFILTER_H
#include "../_common/midistuff.h"
#include "../_common/PizAudioProcessor.h"
#include "MidiLoop.h"

#define numLoops (16)

enum parameters
{
    kRecord,
    kRecActive,
    kActiveLoop,
    kThru,
    kNumGlobalParams,

    kRecArm     = kNumGlobalParams,
    kChannel    = kRecArm + numLoops,
    kTriggerKey = kChannel + numLoops,
    kTranspose  = kTriggerKey + numLoops,
    kOutChannel = kTranspose + numLoops,
    kNumParams  = kOutChannel + numLoops
};

class JuceProgram
{
    friend class MidiStep;

public:
    JuceProgram();
    ~JuceProgram()
    {
    }

private:
    float param[kNumParams];

    int lastUIWidth, lastUIHeight;
    juce::String name;
};

//==============================================================================
class MidiStep : public PizAudioProcessor,
                 public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    MidiStep();
    ~MidiStep() override;

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

    int getNumParameters() override
    {
        return kNumParams;
    }

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
    //void getCurrentProgramStateInformation (MemoryBlock& destData);
    //void setCurrentProgramStateInformation (const void* data, int sizeInBytes);
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    juce::AudioPlayHead::CurrentPositionInfo lastPosInfo;
    int lastUIWidth, lastUIHeight;
    juce::String loopDir;
    int activeLoop;
    Loop* getActiveLoop()
    {
        return loop[activeLoop];
    }
    bool writeMidiFile(int index, juce::File& file);
    bool readMidiFile(int index, juce::File& mid);
    juce::MidiKeyboardState keyboardState;

private:
    float param[kNumParams];
    JuceProgram* programs;
    int curProgram;
    bool init;

    juce::OwnedArray<Loop> loop;
    juce::uint64 samples;
    juce::Array<juce::File> midiFiles;
    bool wasRecording;

    JUCE_LEAK_DETECTOR(MidiStep)
};

#endif
