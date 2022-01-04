#ifndef MidiStepPLUGINFILTER_H
#define MidiStepPLUGINFILTER_H
#include "../_common/PizAudioProcessor.h"
#include "../_common/midistuff.h"
#include "MidiLoop.h"

#define numLoops (16)

enum parameters {
	kRecord,
	kRecActive,
	kActiveLoop,
	kThru,
	kNumGlobalParams,

	kRecArm=kNumGlobalParams,
	kChannel=kRecArm+numLoops,
	kTriggerKey=kChannel+numLoops,
	kTranspose=kTriggerKey+numLoops,
	kOutChannel=kTranspose+numLoops,
    kNumParams=kOutChannel+numLoops
};

class JuceProgram {
friend class MidiStep;
public:
	JuceProgram ();
	~JuceProgram () {}
private:
    float param[kNumParams];

    int lastUIWidth, lastUIHeight;
	String name;
};


//==============================================================================
class MidiStep  : public PizAudioProcessor,
                  public ChangeBroadcaster
{
public:
    //==============================================================================
    MidiStep();
    ~MidiStep();

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

    int getNumParameters()    { return kNumParams; }

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;


    //==============================================================================

    int getNumPrograms()      { return 1; }
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    //void getCurrentProgramStateInformation (MemoryBlock& destData);
    //void setCurrentProgramStateInformation (const void* data, int sizeInBytes);
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);


    //==============================================================================
    AudioPlayHead::CurrentPositionInfo lastPosInfo;
    int lastUIWidth, lastUIHeight;
	String loopDir;
	int activeLoop;
	Loop* getActiveLoop() {return loop[activeLoop];}
    bool writeMidiFile(int index, File &file);
    bool readMidiFile(int index, File &mid);
	MidiKeyboardState keyboardState;


    //==============================================================================
    juce_UseDebuggingNewOperator



private:
    float param[kNumParams];
    JuceProgram *programs;
    int curProgram;
    bool init;

    OwnedArray<Loop> loop;
    uint64 samples;
	Array<File> midiFiles;
	bool wasRecording;
};



#endif
