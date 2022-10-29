#ifndef MIDIMONITORPLUGINFILTER_H
#define MIDIMONITORPLUGINFILTER_H

#include <juce_audio_devices/juce_audio_devices.h>

#include "../_common/PizAudioProcessor.h"

enum parameters
{
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
class MidiMonitorPlugin : public PizAudioProcessor,
                          public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    MidiMonitorPlugin();
    ~MidiMonitorPlugin() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const juce::String getName() const override
    {
        return JucePlugin_Name;
    }
    bool acceptsMidi() const override
    {
        return JucePlugin_WantsMidiInput != 0;
    }
    bool producesMidi() const override
    {
        return JucePlugin_ProducesMidiOutput != 0;
    }
    bool hasEditor() const override
    {
        return true;
    }

    int getNumParameters() override;

    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const juce::String getParameterName (int index) override;
    const juce::String getParameterText (int index) override;
    const juce::String getInputChannelName (int channelIndex) const override;
    const juce::String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    //==============================================================================
    int getNumPrograms() override
    {
        return kNumPrograms;
    }
    int getCurrentProgram() override
    {
        return 0;
    }
    void setCurrentProgram (int index) override
    {
    }
    const juce::String getProgramName (int index) override
    {
        return programName;
    }
    void changeProgramName (int index, const juce::String& newName) override
    {
        programName = newName;
    }
    double getTailLengthSeconds() const override
    {
        return 0;
    }

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    int lastUIWidth, lastUIHeight;

    bool useNotes, useCC, usePB, usePA, useCP, usePC, useSysEx, useOther, useClock;
    juce::AudioPlayHead::CurrentPositionInfo lastPosInfo;

    juce::MidiMessageCollector* getMessageCollector()
    {
        return &midiCollector;
    }
    juce::MidiMessageSequence* getMessages()
    {
        return messages;
    }

    bool writeMidiFile (juce::File mid);
    void clearLoop();
    void setMaxLines (int lines)
    {
        maxLines = juce::jlimit (1, 5000000, lines);
        sendChangeMessage();
    }
    int getMaxLines()
    {
        return maxLines;
    }
    int getTimeSigNumerator()
    {
        return n;
    }
    int getTimeSigDenominator()
    {
        return d;
    }

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

    juce::MidiMessageCollector midiCollector;
    juce::MidiMessageSequence* loop;
    juce::MidiMessageSequence* messages;
    juce::uint64 samples;
    juce::uint64 iter;
    bool waitingForFirstMessage;

    juce::String programName;

    JUCE_LEAK_DETECTOR (MidiMonitorPlugin)
};

#endif
