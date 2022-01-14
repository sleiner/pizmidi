#pragma once

#include "../_common/PizAudioProcessor.h"
#include "MidiMorph.h"

namespace juce
{
    class AudioProcessorEditor;
}
namespace juce
{
    class MidiBuffer;
}
class MidiMorphGUI;

class MidiMorphPlugInInterface : public PizAudioProcessor
{
public:
    MidiMorphPlugInInterface();

    ~MidiMorphPlugInInterface() override;

    int lastGUIw;

    int lastGUIh;

private:
    double sendPos;

    double samplePos;

public:
    MidiMorph core;

    void prepareToPlay (double sampleRate, int estimatedSamplesPerBlock) override;

    //
    const String getInputChannelName (const int channelIndex) const override;

    //virtual  AudioProcessor::getOutputChannelName  (       )  const
    const String getOutputChannelName (const int channelIndex) const override;

    //virtual bool AudioProcessor::isInputChannelStereoPair  (  int  index   )  const
    bool isInputChannelStereoPair (int index) const override;

    //virtual bool AudioProcessor::isInputChannelStereoPair  (  int  index   )  const
    bool isOutputChannelStereoPair (int index) const override;

    //virtual AudioProcessorEditor* AudioProcessor::createEditor  (    )

    AudioProcessorEditor* createEditor() override;

    int getNumParameters() override;

    const String getParameterName (int parameterIndex) override;

    float getParameter (int parameterIndex) override;

    //const String AudioProcessor::getParameterText  (  int  parameterIndex   )
    const String getParameterText (int parameterIndex) override;

    //setParameter  (  int  parameterIndex,
    //  float  newValue
    // )

    void setParameter (int parameterIndex, float newValue) override;

    //bool AudioProcessor::isParameterAutomatable  (  int  parameterIndex   )
    bool isParameterAutomatable (int parameterIndex);

    int getNumPrograms() override;

    int getCurrentProgram() override;

    //  (  int     )
    void setCurrentProgram (int index) override;

    // AudioProcessor::getProgramName  (  int  index   )
    const String getProgramName (int index) override;

    //
    void changeProgramName (int index, const String& newName) override;

    void setStateInformation (const void* data, int sizeInBytes) override;

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;

    //  (       )
    void getStateInformation (JUCE_NAMESPACE::MemoryBlock& destData) override;

    void releaseResources() override;

    bool acceptsMidi() const override;

    bool producesMidi() const override;

    const String getName() const override;

    double getTailLengthSeconds() const override { return 0; }

    bool hasEditor() const override { return true; }
};
