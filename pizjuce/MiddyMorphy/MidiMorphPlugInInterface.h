#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "../_common/PizAudioProcessor.h"
#include "MidiMorph.h"

class MidiMorphGUI;

class MidiMorphPlugInInterface : public PizAudioProcessor
{
public:
    MidiMorphPlugInInterface();

    ~MidiMorphPlugInInterface() override;

    int lastGUIw;

    int lastGUIh;

    MidiMorph core;

    void prepareToPlay (double sampleRate, int estimatedSamplesPerBlock) override;

    const juce::String getInputChannelName (int channelIndex) const override;

    const juce::String getOutputChannelName (int channelIndex) const override;

    bool isInputChannelStereoPair (int index) const override;

    bool isOutputChannelStereoPair (int index) const override;

    juce::AudioProcessorEditor* createEditor() override;

    int getNumParameters() override;

    const juce::String getParameterName (int parameterIndex) override;

    float getParameter (int parameterIndex) override;

    const juce::String getParameterText (int parameterIndex) override;

    void setParameter (int parameterIndex, float newValue) override;

    bool isParameterAutomatable (int parameterIndex);

    int getNumPrograms() override;

    int getCurrentProgram() override;

    void setCurrentProgram (int index) override;

    const juce::String getProgramName (int index) override;

    void changeProgramName (int index, const juce::String& newName) override;

    void setStateInformation (const void* data, int sizeInBytes) override;

    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages) override;

    void getStateInformation (juce::MemoryBlock& destData) override;

    void releaseResources() override;

    bool acceptsMidi() const override;

    bool producesMidi() const override;

    const juce::String getName() const override;

    double getTailLengthSeconds() const override
    {
        return 0;
    }

    bool hasEditor() const override
    {
        return true;
    }

private:
    double sendPos;
    double samplePos;
};
