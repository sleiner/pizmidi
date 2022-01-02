#ifndef MIDIINPLUGINFILTER_H
#define MIDIINPLUGINFILTER_H

#include <memory>

#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_events/juce_events.h"

#include "../_common/PizAudioProcessor.h"

using namespace juce;

enum {
    kPower,
    kHostIn,
	kChannel,
    numParams
};

class JuceProgram {
friend class MidiInFilter;
public:
	JuceProgram ();
	~JuceProgram () {}
private:
    float param[numParams];
    String icon;
	String device;
    String name;
};

//==============================================================================
class MidiInFilter  : public PizAudioProcessor,
                      public ChangeBroadcaster
{
public:
    //==============================================================================
    MidiInFilter();

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
    bool acceptsMidi() const {return true;}
    bool producesMidi() const {return true;}
    double getTailLengthSeconds() const override { return 0; }

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
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
    void getCurrentProgramStateInformation (MemoryBlock& destData);
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes);

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    StringArray devices;
    String icon;

	void setActiveDevice(String name);
	String getActiveDevice() {return activeDevice;}

	//==============================================================================
    juce_UseDebuggingNewOperator

private:
    // this is our gain - the UI and the host can access this by getting/setting
    // parameter 0.
    float param[numParams];
	String activeDevice;

    JuceProgram *programs;
    int curProgram;
    bool init;
    std::unique_ptr<MidiInput> midiInput;
	MidiMessageCollector collector;

    bool wasPlaying;

    AudioPlayHead::CurrentPositionInfo lastPosInfo;

};


#endif
