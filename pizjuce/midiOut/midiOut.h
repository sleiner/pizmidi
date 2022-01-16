#ifndef MIDIOUTPLUGINFILTER_H
#define MIDIOUTPLUGINFILTER_H

#include <memory>

#include "juce_audio_devices/juce_audio_devices.h"

#include "piz/JArray.h"
#include "piz/PizAudioProcessor.h"

using namespace juce;

enum parameters
{
    kPower,
    kClock,
    kHostOut,
    kMTC,
    kStamped,
    kChannel,
    numParams
};

class JuceProgram
{
    friend class MidiOutFilter;

public:
    JuceProgram();
    ~JuceProgram() {}

private:
    float param[numParams];
    String icon;
    MidiDeviceInfo device;
    String name;
};

//==============================================================================
/**
    A simple plugin filter that just applies a gain change to the audio
    passing through it.

*/
class MidiOutFilter : public PizAudioProcessor,
                      public ChangeBroadcaster
{
public:
    //==============================================================================
    MidiOutFilter();
    ~MidiOutFilter() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;

    //==============================================================================
    double getTailLengthSeconds() const override { return 0; }
    const String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    bool hasEditor() const override { return true; }

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
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void getCurrentProgramStateInformation (MemoryBlock& destData) override;
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    piz::JArray<MidiDeviceInfo> devices;
    String icon;

    void setActiveDevice (String name);
    void setActiveDevice (MidiDeviceInfo device);
    MidiDeviceInfo getActiveDevice() { return activeDevice; }
    MidiDeviceInfo getDeviceByName (String name) const;

    //==============================================================================
    juce_UseDebuggingNewOperator

        private :
        // this is our gain - the UI and the host can access this by getting/setting
        // parameter 0.
        float param[numParams];
    MidiDeviceInfo activeDevice;

    JuceProgram* programs;
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
