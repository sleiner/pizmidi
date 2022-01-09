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
    ~MidiMonitorPlugin() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

	void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const String getName() const override {return JucePlugin_Name;}
	bool acceptsMidi() const override {return JucePlugin_WantsMidiInput!=0;}
	bool producesMidi() const override {return JucePlugin_ProducesMidiOutput!=0;}
	bool hasEditor() const override {return true;}

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
    int getNumPrograms() override                                        { return kNumPrograms; }
    int getCurrentProgram() override                                     { return 0; }
    void setCurrentProgram (int index) override                          { }
    const String getProgramName (int index) override                     { return programName; }
    void changeProgramName (int index, const String& newName) override   { programName = newName;}
    double getTailLengthSeconds() const override                { return 0; }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

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
