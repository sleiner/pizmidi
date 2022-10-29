#ifndef MidiChordAnalyzerPLUGINFILTER_H
#define MidiChordAnalyzerPLUGINFILTER_H

#include "../_common/BankStorage.h"
#include "../_common/ChordFunctions.h"
#include "../_common/key.h"
#include "../_common/midistuff.h"
#include "../_common/PizAudioProcessor.h"

#include <juce_data_structures/juce_data_structures.h>
#include <juce_events/juce_events.h>

enum parameters
{
    kChannel,
    kFlats,

    numParams,
    numProgs = 1
};

class MidiChordAnalyzerPrograms : public BankStorage
{
public:
    MidiChordAnalyzerPrograms();

private:
    void loadDefaultValues();
};

//==============================================================================
class MidiChordAnalyzer : public PizAudioProcessor,
                          public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    MidiChordAnalyzer();
    ~MidiChordAnalyzer() override;

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

    bool acceptsMidi() const override
    {
        return true;
    }

    bool producesMidi() const override
    {
        return true;
    }

    //==============================================================================
    int getNumPrograms() override
    {
        return numProgs;
    }

    int getCurrentProgram() override
    {
        return curProgram;
    }

    void setCurrentProgram(int index) override;

    const juce::String getProgramName(int index) override
    {
        return programs->get(0, index, "Name");
    }

    void changeProgramName(int index, const juce::String& newName) override
    {
        programs->set(0, index, "Name", newName);
    }

    void copySettingsToProgram(int index);

    double getTailLengthSeconds() const override
    {
        return 0;
    }

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    void getCurrentProgramStateInformation(juce::MemoryBlock& destData) override;
    void setCurrentProgramStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    juce::MidiKeyboardState chordKbState;
    //String getCurrentChordName();
    int lastUIWidth, lastUIHeight;

private:
    MidiChordAnalyzerPrograms* programs;
    int curProgram;
    int curTrigger;

    bool init;

    int channel;
    bool flats;
    juce::AudioPlayHead::CurrentPositionInfo lastPosInfo;

    JUCE_LEAK_DETECTOR(MidiChordAnalyzer)
};

#endif
