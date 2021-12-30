#ifndef IMAGEPLUGINFILTER_H
#define IMAGEPLUGINFILTER_H

#include "juce_data_structures/juce_data_structures.h"

#include "../_common/PizAudioProcessor.h"
#include "../_common/BankStorage.h"

using namespace juce;

enum parameters {
    kChannel,
    kNumParams
};

class ImageBank : public BankStorage{
public:
	ImageBank ();

private:
    void loadDefaultValues();
};

//==============================================================================
/**
    A simple plugin filter that just applies a gain change to the audio
    passing through it.

*/
class imagePluginFilter  : public PizAudioProcessor,
                  public ChangeBroadcaster
{
public:
    //==============================================================================
    imagePluginFilter();
    ~imagePluginFilter();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

	void processBlock (AudioSampleBuffer& buffer,
                                     MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();

    //==============================================================================
    const String getName() const {return JucePlugin_Name;}
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
	bool hasEditor() const {return true;}
    double getTailLengthSeconds() const override { return 0; }

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

    int getNumPrograms()      { return 128; }
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getCurrentProgramStateInformation (MemoryBlock& destData);
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes);
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

	void setBankColours (Colour colour, Colour text);
	void applySizeToBank(int h, int w);
	void clearAllImages();

	void setCurrentBank(int index, int program=-1);
	int getCurrentBank() {return curBank;}

	void setNoteInput(bool use)
	{
		noteInput = use;
		programs->setGlobal("noteInput",use);
	}

	void setUsePC(bool use)
	{
		usePC = use;
		programs->setGlobal("usePC",use);
	}

	bool getUsePC() {return usePC;}
	bool getNoteInput() {return noteInput;}

	//==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;
    Colour bgcolor;
	Colour textcolor;
    String icon;
    String text;
	bool fullscreen;
	String iconPath;

    //==============================================================================
    juce_UseDebuggingNewOperator



private:
    float param[kNumParams];

    //JuceProgram *programs;
    ImageBank *programs;
	int curBank;
    int curProgram;
	bool noteInput;
	bool usePC;

    bool init;

};



#endif
