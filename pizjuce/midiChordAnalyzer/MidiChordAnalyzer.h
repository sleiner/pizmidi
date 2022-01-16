#ifndef MidiChordAnalyzerPLUGINFILTER_H
#define MidiChordAnalyzerPLUGINFILTER_H

#include "juce_data_structures/juce_data_structures.h"
#include "juce_events/juce_events.h"

#include "../_common/BankStorage.h"
#include "../_common/ChordFunctions.h"
#include "../_common/key.h"
#include "../_common/midistuff.h"
#include "../_common/PizAudioProcessor.h"

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
                          public ChangeBroadcaster
{
public:
    //==============================================================================
    MidiChordAnalyzer();
    ~MidiChordAnalyzer() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const String getName() const override;
    bool hasEditor() const override { return true; }

    int getNumParameters() override;

    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const String getParameterName (int index) override;
    const String getParameterText (int index) override;

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }

    //==============================================================================
    int getNumPrograms() override { return numProgs; }
    int getCurrentProgram() override { return curProgram; }
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override { return programs->get (0, index, "Name"); }
    void changeProgramName (int index, const String& newName) override { programs->set (0, index, "Name", newName); }
    void copySettingsToProgram (int index);
    double getTailLengthSeconds() const override { return 0; }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void getCurrentProgramStateInformation (MemoryBlock& destData) override;
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    MidiKeyboardState chordKbState;
    //String getCurrentChordName();
    int lastUIWidth, lastUIHeight;
    //==============================================================================
    juce_UseDebuggingNewOperator

        private : MidiChordAnalyzerPrograms* programs;
    int curProgram;
    int curTrigger;

    bool init;

    int channel;
    bool flats;
    AudioPlayHead::CurrentPositionInfo lastPosInfo;
};

#endif
