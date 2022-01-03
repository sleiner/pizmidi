#ifndef MIDIOUTPLUGINFILTER_H
#define MIDIOUTPLUGINFILTER_H

#include <memory>

#include "juce_audio_devices/juce_audio_devices.h"

#include "../_common/PizAudioProcessor.h"

using namespace juce;

enum parameters {
    kPower,
    kClock,
    kHostOut,
    kMTC,
    kStamped,
	kChannel,
    numParams
};

class JuceProgram {
friend class MidiOutFilter;
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
/**
    A simple plugin filter that just applies a gain change to the audio
    passing through it.

*/
class MidiOutFilter  : public PizAudioProcessor,
                       public ChangeBroadcaster
{
public:
    //==============================================================================
    MidiOutFilter();
    ~MidiOutFilter();

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
    bool acceptsMidi() const {return true;}
    bool producesMidi() const {return true;}
	bool hasEditor() const {return true;}

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
    std::unique_ptr<MidiOutput> midiOutput;

    double startAt;
    double startMTCAt;
    bool sendclock;
    bool sendmtc;
    int mtcNumber;

    bool wasPlaying;
    int samplesToNextClock;
    int samplesToNextMTC;

    AudioPlayHead::CurrentPositionInfo lastPosInfo;

};


#endif
