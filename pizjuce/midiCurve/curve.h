#ifndef MidiCurvePLUGINFILTER_H
#define MidiCurvePLUGINFILTER_H

#include "../_common/BankStorage.h"
#include "../_common/PizAudioProcessor.h"

#define MAX_ENVELOPE_POINTS   (32)
#define MAX_ENVELOPE_LENGTH   (127.0f)
#define MAX_ENVELOPE_GAIN     (127.0f)
#define MAX_ENVELOPE_DOT_SIZE (8)
#define midiScaler            (0.007874015748031496062992125984252)
#define fmidiScaler           (0.007874015748f)

enum parameters
{
    kNumPointParams = MAX_ENVELOPE_POINTS * 2, //x1,y1,x2,y2...
    kActive         = kNumPointParams,         //active1,active2,...
    kCC             = kActive + MAX_ENVELOPE_POINTS,
    kCCNumber,
    kVelocity,
    kChannelPressure,
    kAftertouch,
    kChannel,
    kPitchBend,
    kNumParams,
    kNumPrograms = 128
};

class MidiCurvePrograms : public BankStorage
{
public:
    MidiCurvePrograms();

    void set (int prog, const juce::Identifier& name, const juce::var& newValue)
    {
        BankStorage::set (0, prog, name.toString(), newValue);
    }
    const juce::var get (int prog, const juce::Identifier& name)
    {
        return BankStorage::get (0, prog, name.toString());
    }

    void loadProgram (int prog, juce::InputStream& stream)
    {
        values_.removeChild (values_.getChild (prog), nullptr);
        values_.addChild (juce::ValueTree::readFromStream (stream), prog, nullptr);
        values_.getChild (prog).setProperty ("progIndex", prog, nullptr);
    }
};

//==============================================================================
class MidiCurve : public PizAudioProcessor,
                  public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    MidiCurve();
    ~MidiCurve() override;

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
    bool hasEditor() const override
    {
        return true;
    }
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

    int getNumParameters() override
    {
        return kNumParams;
    }

    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const juce::String getParameterName (int index) override;
    const juce::String getParameterText (int index) override;

    const juce::String getInputChannelName (int channelIndex) const override;
    const juce::String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;
    double getTailLengthSeconds() const override
    {
        return 0;
    }

    //==============================================================================

    int getNumPrograms() override
    {
        return kNumPrograms;
    }
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getCurrentProgramStateInformation (juce::MemoryBlock& destData) override;
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes) override;
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    int lastUIWidth, lastUIHeight;
    class LastMsgToDisplay : public ChangeBroadcaster
    {
    public:
        LastMsgToDisplay()
        {
            lastCCOut = lastCCIn = -1;
        }
        ~LastMsgToDisplay() override
        {
        }
        int lastCCOut, lastCCIn;
    } lastMsg;
    float getPointValue (int n, int y);
    bool isPointActive (int point);
    bool isPointControl (int point);
    int getPrevActivePoint (int currentPoint);
    int getNextActivePoint (int currentPoint);
    void resetPoints (bool copyToProgram = true);
    juce::Path path;

private:
    float param[kNumParams];

    class midiPoint
    {
    public:
        midiPoint (float x, float y, bool active, bool control)
        {
            p.setXY (x, y);
            isControl = control;
            isActive  = active;
        }
        midiPoint()
        {
            p.setXY (0.f, 0.f);
            isControl = false;
            isActive  = true;
        }
        ~midiPoint(){};

        juce::Point<float> p;
        bool isControl;
        bool isActive;
    };

    struct PointComparator
    {
        int compareElements (midiPoint a, midiPoint b)
        {
            return juce::roundToInt (a.p.getX() * 127.f) - juce::roundToInt (b.p.getX() * 127.f);
        }
    } pointComparator;

    juce::Array<midiPoint> points;
    void updatePath();

    MidiCurvePrograms* programs;
    int curProgram;

    bool init;

    void copySettingsToProgram (int index);
    float findValue (float input);
    double linearInterpolate (double x, double y1, double y2, double x1, double x2);
    int findPBValue (int input);

    JUCE_LEAK_DETECTOR (MidiCurve)
};

#endif
