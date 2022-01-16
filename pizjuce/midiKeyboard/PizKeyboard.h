#ifndef PizKeyboardPLUGINFILTER_H
#define PizKeyboardPLUGINFILTER_H

#include "../_common/PizAudioProcessor.h"

enum parameters
{
    kWidth,
    kChannel,
    kVelocity,
    kUseY,
    kToggleInput,
    kHidePanel,
    kQwertyAnywhere,
    kCapsLock,
    kUseProgCh,
    kSendHeldNotes,
    kClearHeldNotes,
    kShowNumbers,

    numParams
};

static const char* const keymap = "zsxdcvgbhnjmq2w3er5t6y7ui9o0p[+]";
static const int keymapLength   = String (keymap).length();

//==============================================================================
class PizKeyboard : public PizAudioProcessor,
                    public ChangeBroadcaster
{
public:
    //==============================================================================
    PizKeyboard();
    ~PizKeyboard() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const String getName() const override;
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

    bool acceptsMidi() const override;
    bool producesMidi() const override;

    //==============================================================================
    int getNumPrograms() override { return 128; }
    int getCurrentProgram() override { return curProgram; }
    double getTailLengthSeconds() const override { return 0; }
    void setCurrentProgram (int index) override
    {
        lastProgram = curProgram;
        curProgram  = index;
    }
    const String getProgramName (int index) override { return "State " + String (index + 1); }
    void changeProgramName (int index, const String& newName) override {}

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // These properties are public so that our editor component can access them

    // this is kept up to date with the midi messages that arrive, and the UI component
    // registers with it so it can represent the incoming messages
    MidiKeyboardState progKbState[128];
    MidiKeyboardState editorKbState;

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;
    int keyPosition;
    int octave;

    //==============================================================================
    juce_UseDebuggingNewOperator

        private : int lastProgram;
    int curProgram;
    float width;
    float velocity;
    bool useY;
    int channel;
    bool toggle;
    bool hide;
    bool qwerty;
    bool capslock;
    bool usepc;
    bool sendHeldNotes;
    bool clearHeldNotes;
    bool showNumbers;

    bool qwertyState[32]; //32==keymapLength
    bool ccqwertyState[2];
    bool ccState[2];

    bool isCapsLockOn();
};

#endif
