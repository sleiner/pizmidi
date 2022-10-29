#ifndef MIDIPADSPLUGINFILTER_H
#define MIDIPADSPLUGINFILTER_H

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

#include "../_common/PizAudioProcessor.h"

#define midiScaler (0.007874016f)

enum parameters
{
    kVelOffset,
    kCCOffset,
    kChIn,
    kChOut,
    kUseTrigger,
    kUseVel,
    kThru,
    kSendAft,
    kMono,
    kNoteOnTrig,
    kNumGlobalParams,
    numPads     = 128,
    xpos        = kNumGlobalParams,
    ypos        = xpos + numPads,
    kNumParams  = kNumGlobalParams + 2 * numPads,
    numPrograms = 32
};

enum layoutNames
{
    onepad,
    fourpads,
    foursliders,
    tenpads,
    twelvepads,
    sixteenpads,
    sixteensliders,
    sixtyfourpads,
    hexagonpads,
    mixerpads,
    arrangeit28,
    arrangeit51,
    arrangeit39,
    arrangeit48,
    arrangeit64,
    arrangeit55,
    numLayouts
};

class PadLayouts
{
    friend class midiPads;

public:
    PadLayouts() : values_ ("PadLayouts"){};
    ~PadLayouts(){};
    static void setPadLayout (juce::ValueTree tree, float x, float y, float w, float h)
    {
        tree.setProperty ("x", x, nullptr);
        tree.setProperty ("y", y, nullptr);
        tree.setProperty ("w", w, nullptr);
        tree.setProperty ("h", h, nullptr);
    }

    void setPadVisible (int prog, int pad, bool visibility)
    {
        values_.getChild (prog).getChildWithName ("Layout").getChild (pad).setProperty ("visible", visibility, nullptr);
    }

private:
    juce::ValueTree values_;
};

class MidiPadsPrograms
{
    friend class midiPads;

public:
    MidiPadsPrograms();
    void set (int prog, const juce::Identifier& name, const juce::var& newValue)
    {
        values_.getChild (prog).getChildWithName ("GlobalValues").setProperty (name, newValue, nullptr);
    }
    const juce::var get (int prog, const juce::Identifier& name)
    {
        return values_.getChild (prog).getChildWithName ("GlobalValues").getProperty (name);
    }
    void setForPad (int prog, int pad, const juce::Identifier& name, const juce::var& newValue)
    {
        values_.getChild (prog).getChild (pad).setProperty (name, newValue, nullptr);
    }
    void setPadLayout (int prog, int pad, float x, float y, float w, float h)
    {
        values_.getChild (prog).getChildWithName ("Layout").getChild (pad).setProperty ("x", x, nullptr);
        values_.getChild (prog).getChildWithName ("Layout").getChild (pad).setProperty ("y", y, nullptr);
        values_.getChild (prog).getChildWithName ("Layout").getChild (pad).setProperty ("w", w, nullptr);
        values_.getChild (prog).getChildWithName ("Layout").getChild (pad).setProperty ("h", h, nullptr);
    }
    void setPadPosition (int prog, int pad, float x, float y)
    {
        values_.getChild (prog).getChildWithName ("Layout").getChild (pad).setProperty ("x", x, nullptr);
        values_.getChild (prog).getChildWithName ("Layout").getChild (pad).setProperty ("y", y, nullptr);
    }
    void setPadSize (int prog, int pad, float w, float h)
    {
        values_.getChild (prog).getChildWithName ("Layout").getChild (pad).setProperty ("w", w, nullptr);
        values_.getChild (prog).getChildWithName ("Layout").getChild (pad).setProperty ("h", h, nullptr);
    }
    void setPadVisible (int prog, int pad, bool visibility)
    {
        values_.getChild (prog).getChildWithName ("Layout").getChild (pad).setProperty ("visible", visibility, nullptr);
    }
    bool getPadVisible (int prog, int pad)
    {
        return values_.getChild (prog).getChildWithName ("Layout").getChild (pad).getProperty ("visible");
    }
    juce::Rectangle<float> getPadBounds (int prog, int pad)
    {
        return { values_.getChild (prog).getChildWithName ("Layout").getChild (pad).getProperty ("x"),
                 values_.getChild (prog).getChildWithName ("Layout").getChild (pad).getProperty ("y"),
                 values_.getChild (prog).getChildWithName ("Layout").getChild (pad).getProperty ("w"),
                 values_.getChild (prog).getChildWithName ("Layout").getChild (pad).getProperty ("h") };
    }
    const juce::var getForPad (int prog, int pad, const juce::Identifier& name)
    {
        return values_.getChild (prog).getChild (pad).getProperty (name);
    }

    const juce::var getForPad (int prog, int pad, const juce::Identifier& name, const juce::var& defaultValue)
    {
        return values_.getChild (prog).getChild (pad).getProperty (name, defaultValue);
    }

    MidiPadsPrograms (juce::ValueTree& tree)
    {
        values_.getChild (0).getParent() = tree.createCopy();
    }

private:
    juce::ValueTree values_;

    float x[numPrograms][numPads];
    float y[numPrograms][numPads];
};

//==============================================================================
class midiPads : public PizAudioProcessor,
                 public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    midiPads();
    ~midiPads() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer& buffer,
                       juce::MidiBuffer& midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;

    //==============================================================================
    double getTailLengthSeconds() const override { return 0; }
    const juce::String getName() const override { return JucePlugin_Name; }
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

    bool hasEditor() const override { return true; }

    int getNumParameters() override { return kNumParams; }

    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const juce::String getParameterName (int index) override;
    const juce::String getParameterText (int index) override;

    const juce::String getInputChannelName (int channelIndex) const override;
    const juce::String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    //==============================================================================
#if ONEPROGRAM
    int getNumPrograms()
    {
        return 1;
    }
#else
    int getNumPrograms() override
    {
        return numPrograms;
    }
#endif
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    void copySettingsToProgram (int index);

    //==============================================================================
    void getCurrentProgramStateInformation (juce::MemoryBlock& destData) override;
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes) override;
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void saveXmlPatch (int index, juce::File file);
    void saveXmlBank (juce::File file);
    bool loadXmlPatch (int index, juce::File file);
    bool loadXmlBank (juce::File file);
    bool loadFxp (juce::File file);
    bool loadFxb (juce::File file);
    void loadDefaultPrograms();

    juce::Rectangle<int> getPadBounds (int index);
    void setPadPosition (int index, float x, float y);
    void setPadSize (int index, float w, float h);

    juce::String getGlobalParamValueName (int index)
    {
        switch (index)
        {
            case kVelOffset:
                return "VelOffset";
            case kCCOffset:
                return "CCOffset";
            case kChIn:
                return "ChIn";
            case kChOut:
                return "ChOut";
            case kUseTrigger:
                return "UseTrigger";
            case kUseVel:
                return "UseVel";
            case kThru:
                return "Thru";
            case kSendAft:
                return "SendAft";
            case kMono:
                return "Mono";
            case kNoteOnTrig:
                return "NoteOnTrig";
            default:
                return {};
        }
    }

    bool isPadVisible (int index);
    void setPadVisible (int index, bool visibility);
    void setLayout (int prog, int layoutIndex)
    {
        int index = programs->values_.getChild (prog).indexOf (programs->values_.getChild (prog).getChildWithName ("Layout"));
        programs->values_.getChild (prog).removeChild (index, nullptr);
        programs->values_.getChild (prog).addChild (layouts->values_.getChild (layoutIndex).createCopy(), index, nullptr);
    }
    void loadXmlLayout (juce::File file);
    void saveXmlLayout (juce::File file);
    void copyPadSettings (int source, int dest);
    void setProperty (int pad, const juce::Identifier& name, const juce::var& newValue)
    {
        programs->setForPad (curProgram, pad, name, newValue);
    }
    const juce::var getProperty (int pad, const juce::Identifier& name, const juce::var defaultReturnValue = 0)
    {
        return programs->getForPad (curProgram, pad, name, defaultReturnValue);
    }

    //==============================================================================
    int lastUIWidth, lastUIHeight;

    int Ydata1[numPads];
    int Ycc[numPads];
    int Ytype[numPads];
    int Ydata2[numPads];
    int Yoff[numPads];
    int trigger[numPads];
    int Xcc[numPads];
    int Xoff[numPads];
    bool SendOff[numPads];
    bool UseX[numPads];
    bool UseY[numPads];
    bool UseYCC[numPads];
    bool UseXPB[numPads];
    bool toggle[numPads];

    float midilisten[numPads];
    float send[numPads];
    float sendoff[numPads];
    float bghue;
    float bgsat;
    float bgbri;
    float contrast;
    juce::String icon[numPads];
    juce::String text[numPads];
    juce::Colour padcolor[numPads];
    bool buttondown[numPads];
    bool isplaying[128];
    int squares;
    bool showdots[numPads];
    bool showvalues[numPads];
    bool editmode;
    bool usemouseup;
    bool hex;
    bool triggered[numPads];
    bool togglestate[numPads];
    int lastx[numPads];
    int lasty[numPads];
    float roundness[numPads];
    float iconsize[numPads];
    bool trig;
    bool dotmoved[numPads];
    int lastxccvalue[numPads];
    int lastyccvalue[numPads];
    bool centeredText[numPads];
    juce::String pluginPath, layoutPath, presetPath, bankPath, iconPath;

private:
    float param[kNumParams];
    int triggervel;

    MidiPadsPrograms* programs;
    PadLayouts* layouts;
    void fillLayouts();
    int curProgram;

    bool init;

    JUCE_LEAK_DETECTOR (midiPads)
};

#endif
