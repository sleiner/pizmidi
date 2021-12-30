#pragma once

#include "../_common/PizAudioProcessor.h"
#include "MidiMorph.h"

namespace juce { class AudioProcessorEditor; }
namespace juce { class MidiBuffer; }
class MidiMorphGUI;

class MidiMorphPlugInInterface : public PizAudioProcessor {
  public:
    MidiMorphPlugInInterface();

    ~MidiMorphPlugInInterface();

    int lastGUIw;

    int lastGUIh;


  private:
    double sendPos;

    double samplePos;


  public:
    MidiMorph core;

    void prepareToPlay(double sampleRate, int estimatedSamplesPerBlock);

    //
    const String getInputChannelName(const int channelIndex) const;

    //virtual  AudioProcessor::getOutputChannelName  (       )  const
    const String getOutputChannelName(const int channelIndex) const;

    //virtual bool AudioProcessor::isInputChannelStereoPair  (  int  index   )  const
    bool isInputChannelStereoPair(int index) const;

    //virtual bool AudioProcessor::isInputChannelStereoPair  (  int  index   )  const
    bool isOutputChannelStereoPair(int index) const;

    //virtual AudioProcessorEditor* AudioProcessor::createEditor  (    )

    AudioProcessorEditor* createEditor();

    int getNumParameters();

    const String getParameterName(int parameterIndex);

    float getParameter(int parameterIndex);

    //const String AudioProcessor::getParameterText  (  int  parameterIndex   )
    const String getParameterText(int parameterIndex);

    //setParameter  (  int  parameterIndex,
    //  float  newValue
    // )

    void setParameter(int parameterIndex, float newValue);

    //bool AudioProcessor::isParameterAutomatable  (  int  parameterIndex   )
    bool isParameterAutomatable(int parameterIndex);

    int getNumPrograms();

    int getCurrentProgram();

    //  (  int     )
    void setCurrentProgram(int index);

    // AudioProcessor::getProgramName  (  int  index   )
    const String getProgramName(int index);

    //
    void changeProgramName(int index, const String& newName);

    void setStateInformation(const void * data, int sizeInBytes);

    void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //  (       )
    void getStateInformation(JUCE_NAMESPACE::MemoryBlock & destData);

    void releaseResources();

    bool acceptsMidi() const;

    bool producesMidi() const;

    const String getName() const;

    double getTailLengthSeconds() const override { return 0; }

    bool hasEditor() const override { return true; }
};
