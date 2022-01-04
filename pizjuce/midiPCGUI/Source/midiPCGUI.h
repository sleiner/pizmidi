#ifndef MIDIOUTPLUGINFILTER_H
#define MIDIOUTPLUGINFILTER_H

#include "juce_core/juce_core.h"
#include "juce_events/juce_events.h"

#include "../_common/PizAudioProcessor.h"

enum midiPCGUIParameters {
	kProgram,
	kBankMSB,
	kBankLSB,
	kMode,
	kTrigger,
	kInc,
	kDec,
	kBankTrigger,
	kPCListen,
	kChannel,
	kThru,
	kMinimize,
	numParams
};

class midiPCGUIProgram {
friend class midiPCGUI;
public:
	midiPCGUIProgram ();
	~midiPCGUIProgram () {};
	void setName(String newName) {name=newName;}
private:
    float param[numParams];
    String name;
};

//==============================================================================
class midiPCGUI  : public PizAudioProcessor,
                   public ChangeBroadcaster
{
public:
    //==============================================================================
    midiPCGUI();
    ~midiPCGUI();

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
    bool acceptsMidi() const {return true;}
    bool producesMidi() const {return true;}

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
    int getNumPrograms()                                        { return 128; }
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);
	void setMidiProgName (int channel, int bank, int prog, const String& newName) {
		progNames.setNameFor(channel,bank,prog,newName);
	}
	String getMidiProgName (int channel, int bank, int prog) {
		return progNames.getNameFor(channel,bank,prog);
	}

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
    void getCurrentProgramStateInformation (MemoryBlock& destData);
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes);

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!
	int actualProgram[16];
	int actualBankMSB[16];
	int actualBankLSB[16];


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    // this is our gain - the UI and the host can access this by getting/setting
    // parameter 0.
    float param[numParams];
	struct MidiProgName
	{
		MidiProgName() {channel=-1;bank=-1;program=-1;name=String();}
		MidiProgName(int c, int b, int p, const String& n)
		{
			channel=c; bank=b; program=p; name=n;
		}
		~MidiProgName(){};
		int channel;
		int bank;
		int program;
		String name;
	};

	class ProgNames
	{
	friend class midiPCGUI;
	public:
		ProgNames() {};
		~ProgNames() {};
		String getNameFor(int c, int b, int p)
		{
			for (int i=0;i<names.size();i++) {
				if (names.getUnchecked(i).bank==b
					&& names.getUnchecked(i).channel==c
					&& names.getUnchecked(i).program==p)
					return names.getUnchecked(i).name;
			}
			return "Program " + String(p+1);
		}

		bool getNameIfSet(int c, int b, int p, String& name)
		{
			for (int i=0;i<names.size();i++) {
				if (names.getUnchecked(i).bank==b
					&& names.getUnchecked(i).channel==c
					&& names.getUnchecked(i).program==p)
				{
					name = names.getUnchecked(i).name;
					return true;
				}
			}
			return false;
		}

		void setNameFor(int c, int b, int p, String newName)
		{
			for (int i=0;i<names.size();i++) {
				if (names.getUnchecked(i).bank==b
					&& names.getUnchecked(i).channel==c
					&& names.getUnchecked(i).program==p)
				{
					names.remove(i);
					break;
				}
			}
			names.add(MidiProgName(c,b,p,newName));
		}
	private:
		Array<MidiProgName> names;
	};

	ProgNames progNames;

	String pluginPath;

    midiPCGUIProgram *programs;
    int curProgram;
    bool init;

	enum modes {continuous, triggered};
	char mode;
	bool trigger,inc,dec,triggerbank;
	int program, bankmsb, banklsb;
	bool senttrig, sentinc, sentdec, sentbank;
	bool pclisten;
	bool thru;
	bool automated[numParams];

	bool wait;
	int delaytime;
	int counter;
	int triggerdelta;
};


#endif
