#ifndef MIDIMONITORPLUGINFILTER_H
#define MIDIMONITORPLUGINFILTER_H

#include "juce_audio_devices/juce_audio_devices.h"

#include "../_common/PizAudioProcessor.h"

using namespace juce;

enum parameters {
	kBgHue,
	kBgSat,
	kBgBri,
	kContrast,
	kPower,
	kThru,
	kBytes,
	kWrap,
	kTime,
	kTimeMode,
	kTicks,
	kFrames,
	kShowms,
	kShowhrs,
    kShowsubfr,
	kNumParams,
	kNumPrograms = 1
};
//==============================================================================
class MidiMonitorPlugin  : public PizAudioProcessor,
                           public ChangeBroadcaster
{
public:
    //==============================================================================
    MidiMonitorPlugin();
    ~MidiMonitorPlugin();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

	void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();

    //==============================================================================
    const String getName() const {return JucePlugin_Name;}
	bool acceptsMidi() const {return JucePlugin_WantsMidiInput!=0;}
	bool producesMidi() const {return JucePlugin_ProducesMidiOutput!=0;}
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
    int getNumPrograms()                                        { return kNumPrograms; }
    int getCurrentProgram()                                     { return 0; }
    void setCurrentProgram (int index)                          { }
    const String getProgramName (int index)                     { return programName; }
    void changeProgramName (int index, const String& newName)   { programName = newName;}
    double getTailLengthSeconds() const override                { return 0; }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

    //==============================================================================
    int lastUIWidth, lastUIHeight;

    bool useNotes, useCC, usePB, usePA, useCP, usePC, useSysEx, useOther, useClock;
    AudioPlayHead::CurrentPositionInfo lastPosInfo;

    MidiMessageCollector* getMessageCollector ()  { return &midiCollector; }
	MidiMessageSequence* getMessages ()  { return messages; }

    bool writeMidiFile(File mid);
    void clearLoop();
	void setMaxLines(int lines)
	{
		maxLines = jlimit(1,5000000,lines);
		sendChangeMessage();
	}
	int getMaxLines() {return maxLines;}
	int getTimeSigNumerator() {return n;}
	int getTimeSigDenominator() {return d;}

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    float bghue;
    float bgsat;
    float bgbri;
    float contrast;
    float power;
    float thru;
    float bytes;
    float wrap;
	float showtime;
	float ticks;
	float frames;
	float mode;

	int maxLines;
	int n, d;

    MidiMessageCollector midiCollector;
    MidiMessageSequence* loop;
	MidiMessageSequence* messages;
    uint64 samples;
	uint64 iter;
    bool waitingForFirstMessage;

	String programName;
};


#endif
