#ifndef BIGCLOCK_PLUGINFILTER_H
#define BIGCLOCK_PLUGINFILTER_H

#include "juce_events/juce_events.h"
#include "piz/PizAudioProcessor.h"

enum parameters
{
    kBarsBeats,
    kLook,
    kShowms,
    kShowhrs,
    kTicks,
    kFrames,
    kSamples,
    kShowsubfr,
    kClockMode,
    kRunWatch,
    kClearWatch,
    kNumParams
};

enum clockmodes
{
    HostTimeMode,
    RecTimeMode,
    StopwatchMode,
    PluginTimeMode,
    ActualTimeMode,
    numModes
};

class smpte
{
public:
    smpte()
    {
        hr   = 0;
        mn   = 0;
        sc   = 0;
        fr   = 0.0;
        rate = 24.0;
    }
    smpte (int h, int m, int s, double f, double r = 24.0)
    {
        hr = h;
        mn = m;
        sc = s, fr = f;
        rate = r;
    }
    ~smpte(){};
    long double getTimeInSeconds()
    {
        if (rate == 29.97)
        {
            //return ?????????????????;
        }
        return 3600.0 * (long double) hr + 60.0 * (long double) mn + (long double) sc + (long double) (fr / rate);
    }
    int64 getCurrentFrame()
    {
        if (rate == 29.97)
        {
            int64 frameNumber = int64 (this->getTimeInSeconds() * rate);
            frameNumber += 18 * (frameNumber / 17982) + 2 * (((frameNumber % 17982) - 2) / 1798);
            return frameNumber;
        }
        return int64 (this->getTimeInSeconds() * rate);
    }
    int getSubFrames()
    {
        return (int) ((fr * 100.0)) % 100;
    }

private:
    int hr;
    int mn;
    int sc;
    double fr;
    double rate;
};

class cue
{
public:
    double time;
    double ppq;
    String text;
    bool enabled;
    cue()
    {
        time    = 0.0;
        text    = String();
        ppq     = 0.0;
        enabled = true;
    }
    cue (double t, double p, String s, bool e = true)
    {
        time    = t;
        text    = s;
        ppq     = p;
        enabled = e;
    }
    ~cue(){};
};

class compareCues
{
public:
    int compareElements (cue* one, cue* two)
    {
        return int ((one->time - two->time) * 100.0);
    }
};

//==============================================================================
/**
    A simple plugin filter that just applies a gain change to the audio
    passing through it.

*/
class BigClockFilter : public PizAudioProcessor,
                       public ChangeBroadcaster,
                       public Timer
{
public:
    //==============================================================================
    BigClockFilter();
    ~BigClockFilter() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const String getName() const override { return JucePlugin_Name; }
    double getTailLengthSeconds() const override { return 0; }
    bool hasEditor() const override { return true; }
    bool acceptsMidi() const override
    {
#if JucePlugin_WantsMidiInput
        return true;
#else
        return false;
#endif
    }
    bool producesMidi() const override
    {
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
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int index) override {}
    const String getProgramName (int index) override { return String(); }
    void changeProgramName (int index, const String& newName) override {}

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void timerCallback() override;

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    Colour bgcolor;

    // this keeps a copy of the last set of time info that was acquired during an audio
    // callback - the UI component will read this and display it.
    AudioPlayHead::CurrentPositionInfo lastPosInfo;

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;
    double sampleRate;
    double rectime;
    uint32 plugintime;
    uint32 watchtime; //stopwatch start time
    int mode;

    OwnedArray<cue> cues;
    //cue cues[128];
    bool showcues;
    void addCue (double time, double ppq, String text);
    String getCue (double ppq, bool barsbeats);
    void setCueEnabled (int index, bool state);
    const double secondsToPpq (const double seconds, const double bpm);
    const String ppqToString (const double sppq,
                              const int numerator,
                              const int denominator,
                              const double bpm,
                              const bool mode);
    void saveCues (File cuefile);
    void loadCues (File cuefile);

    double smpteStringToSeconds (String smpte, double fps);
    String secondsToSmpteString (double seconds, double fps);
    double barsbeatsStringToPpq (String barsbeats, int n = 4, int d = 4);
    String ppqToBarsbeatsString (double ppq, int n = 4, int d = 4);

    //==============================================================================
    juce_UseDebuggingNewOperator

        private : float barsbeats;
    float look;
    float showms;
    float showhrs;
    float ticks;
    float frames;
    float samples;
    float showsubfr;
    bool runwatch;

    bool recording;
    uint32 lastTimerTime;

    compareCues c;
};

#endif
