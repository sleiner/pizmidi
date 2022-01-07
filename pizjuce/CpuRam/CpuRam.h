#ifndef CPURAMPLUGINFILTER_H
#define CPURAMPLUGINFILTER_H
#define _WIN32_WINNT 0x0501

#include "juce_events/juce_events.h"

#include "../_common/PizAudioProcessor.h"


//==============================================================================
class CpuRam  : public PizAudioProcessor,
                public ChangeBroadcaster
{
public:
    //==============================================================================
    CpuRam();
    ~CpuRam();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

	void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();

    //==============================================================================
    double getTailLengthSeconds() const override {return 0;}
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
    int getNumPrograms()                                        { return 1; }
    int getCurrentProgram()                                     { return 0; }
    void setCurrentProgram (int index)                          { }
    const String getProgramName (int index)                     { return String(); }
    void changeProgramName (int index, const String& newName)   { }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
	void resetToDefaultSettings();

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    Colour bgcolor;
	bool showGraph;

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    // this is our interval - the UI and the host can access this by getting/setting
    // parameter 0.
    float interval;
    float minimize;
};


#endif
