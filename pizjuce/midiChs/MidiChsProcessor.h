#ifndef MIDICHSPLUGINFILTER_H
#define MIDICHSPLUGINFILTER_H

#include "juce_events/juce_events.h"

#include "../_common/PizAudioProcessor.h"


//==============================================================================
class MidiChsProcessor  : public PizAudioProcessor,
                          public ChangeBroadcaster
{
public:
    //==============================================================================
    MidiChsProcessor();
    ~MidiChsProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

	void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const String getName() const override {return JucePlugin_Name;}
	bool hasEditor() const override {return true;}
    bool acceptsMidi() const override {
#if JucePlugin_WantsMidiInput
        return true;
#else
        return false;
#endif
    }
    bool producesMidi() const override {
#if JucePlugin_ProducesMidiOutput
        return true;
#else
        return false;
#endif
    }

    int getNumParameters() override;

    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const String getParameterName (int index) override;
    const String getParameterText (int index) override;

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    //==============================================================================
    int getNumPrograms() override                                        { return 0; }
    int getCurrentProgram() override                                     { return 0; }
    void setCurrentProgram (int index) override                          { }
    const String getProgramName (int index) override                     { return String(); }
    void changeProgramName (int index, const String& newName) override   { }
    double getTailLengthSeconds() const override                { return 0; }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;

    //==============================================================================
    juce_UseDebuggingNewOperator

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

};


#endif
