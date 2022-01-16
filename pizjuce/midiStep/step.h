#ifndef MidiStepPLUGINFILTER_H
#define MidiStepPLUGINFILTER_H
#include "MidiLoop.h"
#include "piz/midistuff.h"
#include "piz/PizAudioProcessor.h"

#define numLoops (16)

enum parameters
{
    kRecord,
    kRecActive,
    kActiveLoop,
    kThru,
    kNumGlobalParams,

    kRecArm     = kNumGlobalParams,
    kChannel    = kRecArm + numLoops,
    kTriggerKey = kChannel + numLoops,
    kTranspose  = kTriggerKey + numLoops,
    kOutChannel = kTranspose + numLoops,
    kNumParams  = kOutChannel + numLoops
};

class JuceProgram
{
    friend class MidiStep;

public:
    JuceProgram();
    ~JuceProgram() {}

private:
    float param[kNumParams];

    int lastUIWidth, lastUIHeight;
    String name;
};

//==============================================================================
class MidiStep : public PizAudioProcessor,
                 public ChangeBroadcaster
{
public:
    //==============================================================================
    MidiStep();
    ~MidiStep() override;

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

    int getNumParameters() override { return kNumParams; }

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
    //void getCurrentProgramStateInformation (MemoryBlock& destData);
    //void setCurrentProgramStateInformation (const void* data, int sizeInBytes);
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    AudioPlayHead::CurrentPositionInfo lastPosInfo;
    int lastUIWidth, lastUIHeight;
    String loopDir;
    int activeLoop;
    Loop* getActiveLoop() { return loop[activeLoop]; }
    bool writeMidiFile (int index, File& file);
    bool readMidiFile (int index, File& mid);
    MidiKeyboardState keyboardState;

    //==============================================================================
    juce_UseDebuggingNewOperator

        private : float param[kNumParams];
    JuceProgram* programs;
    int curProgram;
    bool init;

    OwnedArray<Loop> loop;
    uint64 samples;
    Array<File> midiFiles;
    bool wasRecording;
};

#endif
