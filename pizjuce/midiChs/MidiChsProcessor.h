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
    ~MidiChsProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

	void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();

    //==============================================================================
    const String getName() const {return JucePlugin_Name;}
	bool hasEditor() const {return true;}
    bool acceptsMidi() const {
#if JucePlugin_WantsMidiInput
        return true;
#else
        return false;
#endif
    }
    bool producesMidi() const {
#if JucePlugin_ProducesMidiOutput
        return true;
#else
        return false;
#endif
    }

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    //==============================================================================
    int getNumPrograms()                                        { return 0; }
    int getCurrentProgram()                                     { return 0; }
    void setCurrentProgram (int index)                          { }
    const String getProgramName (int index)                     { return String(); }
    void changeProgramName (int index, const String& newName)   { }
    double getTailLengthSeconds() const override                { return 0; }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

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
