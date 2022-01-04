#ifndef MidiChordAnalyzerPLUGINFILTER_H
#define MidiChordAnalyzerPLUGINFILTER_H

#include "juce_data_structures/juce_data_structures.h"
#include "juce_events/juce_events.h"

#include "../_common/PizAudioProcessor.h"
#include "../_common/midistuff.h"
#include "../_common/ChordFunctions.h"
#include "../_common/key.h"
#include "../_common/BankStorage.h"

enum parameters {
    kChannel,
	kFlats,

    numParams,
	numProgs = 1
};


class MidiChordAnalyzerPrograms : public BankStorage {
public:
	MidiChordAnalyzerPrograms ();

private:
    void loadDefaultValues();
};

//==============================================================================
class MidiChordAnalyzer  : public PizAudioProcessor,
                    public ChangeBroadcaster
{
public:
    //==============================================================================
    MidiChordAnalyzer();
    ~MidiChordAnalyzer();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

	void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();

    //==============================================================================
    const String getName() const;
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

	bool acceptsMidi() const {return true;}
    bool producesMidi() const {return true;}

    //==============================================================================
    int getNumPrograms()                                        { return numProgs; }
    int getCurrentProgram()                                     { return curProgram; }
    void setCurrentProgram (int index);
    const String getProgramName (int index)                     { return programs->get(0,index,"Name"); }
    void changeProgramName (int index, const String& newName)   { programs->set(0,index,"Name",newName); }
	void copySettingsToProgram(int index);
    double getTailLengthSeconds() const override                { return 0; }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
    void getCurrentProgramStateInformation (MemoryBlock& destData);
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes);

    //==============================================================================
	MidiKeyboardState chordKbState;
	//String getCurrentChordName();
	int lastUIWidth, lastUIHeight;
    //==============================================================================
    juce_UseDebuggingNewOperator

private:
	MidiChordAnalyzerPrograms* programs;
	int curProgram;
	int curTrigger;

	bool init;

    int channel;
	bool flats;
	AudioPlayHead::CurrentPositionInfo lastPosInfo;
};

#endif
