#ifndef MIDIOUTPLUGINFILTER_H
#define MIDIOUTPLUGINFILTER_H

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>

#include "../_common/PizAudioProcessor.h"

enum midiPCGUIParameters
{
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

class midiPCGUIProgram
{
    friend class midiPCGUI;

public:
    midiPCGUIProgram();
    ~midiPCGUIProgram(){};

    void setName(juce::String newName)
    {
        name = newName;
    }

private:
    float param[numParams];
    juce::String name;
};

//==============================================================================
class midiPCGUI : public PizAudioProcessor,
                  public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    midiPCGUI();
    ~midiPCGUI() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(juce::AudioSampleBuffer& buffer,
                      juce::MidiBuffer& midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;

    //==============================================================================
    double getTailLengthSeconds() const override
    {
        return 0;
    }

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
        return true;
    }

    bool producesMidi() const override
    {
        return true;
    }

    int getNumParameters() override;

    float getParameter(int index) override;
    void setParameter(int index, float newValue) override;

    const juce::String getParameterName(int index) override;
    const juce::String getParameterText(int index) override;

    const juce::String getInputChannelName(int channelIndex) const override;
    const juce::String getOutputChannelName(int channelIndex) const override;
    bool isInputChannelStereoPair(int index) const override;
    bool isOutputChannelStereoPair(int index) const override;

    //==============================================================================
    int getNumPrograms() override
    {
        return 128;
    }

    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void setMidiProgName(int channel, int bank, int prog, const juce::String& newName)
    {
        progNames.setNameFor(channel, bank, prog, newName);
    }

    juce::String getMidiProgName(int channel, int bank, int prog)
    {
        return progNames.getNameFor(channel, bank, prog);
    }

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    void getCurrentProgramStateInformation(juce::MemoryBlock& destData) override;
    void setCurrentProgramStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!
    int actualProgram[16];
    int actualBankMSB[16];
    int actualBankLSB[16];

private:
    // this is our gain - the UI and the host can access this by getting/setting
    // parameter 0.
    float param[numParams];

    struct MidiProgName
    {
        MidiProgName()
        {
            channel = -1;
            bank    = -1;
            program = -1;
            name    = juce::String();
        }

        MidiProgName(int c, int b, int p, const juce::String& n)
        {
            channel = c;
            bank    = b;
            program = p;
            name    = n;
        }

        ~MidiProgName(){};
        int channel;
        int bank;
        int program;
        juce::String name;
    };

    class ProgNames
    {
        friend class midiPCGUI;

    public:
        ProgNames(){};
        ~ProgNames(){};

        juce::String getNameFor(int c, int b, int p)
        {
            for (int i = 0; i < names.size(); i++)
            {
                if (names.getUnchecked(i).bank == b
                    && names.getUnchecked(i).channel == c
                    && names.getUnchecked(i).program == p)
                    return names.getUnchecked(i).name;
            }
            return "Program " + juce::String(p + 1);
        }

        bool getNameIfSet(int c, int b, int p, juce::String& name)
        {
            for (int i = 0; i < names.size(); i++)
            {
                if (names.getUnchecked(i).bank == b
                    && names.getUnchecked(i).channel == c
                    && names.getUnchecked(i).program == p)
                {
                    name = names.getUnchecked(i).name;
                    return true;
                }
            }
            return false;
        }

        void setNameFor(int c, int b, int p, juce::String newName)
        {
            for (int i = 0; i < names.size(); i++)
            {
                if (names.getUnchecked(i).bank == b
                    && names.getUnchecked(i).channel == c
                    && names.getUnchecked(i).program == p)
                {
                    names.remove(i);
                    break;
                }
            }
            names.add(MidiProgName(c, b, p, newName));
        }

    private:
        juce::Array<MidiProgName> names;
    };

    ProgNames progNames;

    juce::String pluginPath;

    midiPCGUIProgram* programs;
    int curProgram;
    bool init;

    enum modes
    {
        continuous,
        triggered
    };

    char mode;
    bool trigger, inc, dec, triggerbank;
    int program, bankmsb, banklsb;
    bool senttrig, sentinc, sentdec, sentbank;
    bool pclisten;
    bool thru;
    bool automated[numParams];

    bool wait;
    int delaytime;
    int counter;
    int triggerdelta;

    JUCE_LEAK_DETECTOR(midiPCGUI)
};

#endif
