#ifndef MidiChordsPLUGINFILTER_H
#define MidiChordsPLUGINFILTER_H

#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>

#include "../_common/BankStorage.h"
#include "../_common/ChordFunctions.h"
#include "../_common/key.h"
#include "../_common/midistuff.h"
#include "../_common/PizAudioProcessor.h"

enum parameters
{
    kChannel,
    kLearnChord,
    kFollowInput,
    kTranspose,
    kMode,
    kRoot,
    kGuess,
    kFlats,
    kUseProgCh,
    kLearnChannel,
    kVelocity,
    kInputTranspose,
    kToAllChannels,
    kOutChannel,

    kStrum,
    kSpeed,
    kMaxDelay,
    kVelRamp,
    kAccel,
    //kVelToSpeed,

    numParams,
    kVariation,
    numProgs = 128
};

enum ChordModes
{
    Normal,
    Octave,
    Global,
    numModes
};

struct ChordNote
{
    ChordNote(int channel, int note)
    {
        c = channel;
        n = note;
    }
    ChordNote()
    {
        c = n = -1;
    }
    ~ChordNote()
    {
    }

    int c;
    int n;
};

class GuitarDefinition
{
public:
    GuitarDefinition()
    {
        guitarName = juce::String();
        chordFile  = "Chords.txt";
        numFrets   = 0;
        numStrings = 0;
    }

    GuitarDefinition(juce::String name, juce::String file, int frets, juce::Array<int>& notes)
    {
        guitarName = name;
        chordFile  = file;
        numFrets   = frets;
        stringNotes.addArray(notes);
        numStrings = notes.size();
    }
    ~GuitarDefinition()
    {
    }

    juce::String guitarName;
    int numStrings;
    int numFrets;
    juce::Array<int> stringNotes;
    juce::String chordFile;
};

class MidiChordsPrograms : public BankStorage
{
public:
    MidiChordsPrograms();

    void set(int prog, const juce::Identifier& name, const juce::var& newValue)
    {
        BankStorage::set(0, prog, name.toString(), newValue);
    }

    void clearNoteMatrix(int prog, int trigger)
    {
        auto noteMatrix = values_.getChild(0).getChild(prog).getChild(trigger);
        if (noteMatrix.isValid())
            noteMatrix.removeAllProperties(nullptr);
    }

    void setChordNote(int prog, int trigger, int channel, int note, const bool& newValue)
    {
        channel -= 1;
        auto triggerProp = values_.getChild(0).getChild(prog).getChild(trigger);

        if (! triggerProp.isValid())
        {
            juce::ValueTree noteMatrix("NoteMatrix_T" + juce::String(trigger));
            values_.getChild(0).getChild(prog).addChild(noteMatrix, trigger, nullptr);
        }
        if (note < 32)
        {
            int state = triggerProp.getProperty("Ch" + juce::String(channel) + "_0-31", 0);
            if (newValue)
                state |= 1 << note;
            else
                state &= ~(1 << note);
            triggerProp.setProperty("Ch" + juce::String(channel) + "_0-31", state, nullptr);
        }
        else if (note < 64)
        {
            note -= 32;
            int state = triggerProp.getProperty("Ch" + juce::String(channel) + "_32-63", 0);
            if (newValue)
                state |= 1 << note;
            else
                state &= ~(1 << note);
            triggerProp.setProperty("Ch" + juce::String(channel) + "_32-63", state, nullptr);
        }
        else if (note < 96)
        {
            note -= 64;
            int state = triggerProp.getProperty("Ch" + juce::String(channel) + "_64-95", 0);
            if (newValue)
                state |= 1 << note;
            else
                state &= ~(1 << note);
            triggerProp.setProperty("Ch" + juce::String(channel) + "_64-95", state, nullptr);
        }
        else if (note < 128)
        {
            note -= 96;
            int state = triggerProp.getProperty("Ch" + juce::String(channel) + "_96-127", 0);
            if (newValue)
                state |= 1 << note;
            else
                state &= ~(1 << note);
            triggerProp.setProperty("Ch" + juce::String(channel) + "_96-127", state, nullptr);
        }
    }

    const juce::var get(int prog, const juce::Identifier& name)
    {
        return BankStorage::get(0, prog, name.toString());
    }

    bool getChordNote(int prog, int trigger, int ch, int note)
    {
        ch -= 1;
        auto triggerProp = values_.getChild(0).getChild(prog).getChild(trigger);

        if (! triggerProp.isValid())
            return false;
        if (note < 32)
        {
            int state = triggerProp.getProperty("Ch" + juce::String(ch) + "_0-31", 0);
            return (state & (1 << note)) != 0;
        }
        else if (note < 64)
        {
            note -= 32;
            int state = triggerProp.getProperty("Ch" + juce::String(ch) + "_32-63", 0);
            return (state & (1 << note)) != 0;
        }
        else if (note < 96)
        {
            note -= 64;
            int state = triggerProp.getProperty("Ch" + juce::String(ch) + "_64-95", 0);
            return (state & (1 << note)) != 0;
        }
        else if (note < 128)
        {
            note -= 96;
            int state = triggerProp.getProperty("Ch" + juce::String(ch) + "_96-127", 0);
            return (state & (1 << note)) != 0;
        }
        return false;
    }

    void fixupVelocity()
    {
        for (int b = 0; b < values_.getNumChildren(); b++)
        {
            auto bank = values_.getChild(b);
            for (int p = 0; p < bank.getNumChildren(); p++)
            {
                if (BankStorage::get(b, p, "Velocity") == juce::var(0))
                {
                    BankStorage::set(b, p, "Velocity", 100);
                }
            }
        }
    }

private:
    void loadDefaults();
};

//==============================================================================
class MidiChords : public PizAudioProcessor,
                   public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    MidiChords();
    ~MidiChords() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(juce::AudioSampleBuffer& buffer,
                      juce::MidiBuffer& midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const juce::String getName() const override;
    bool hasEditor() const override
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

    bool acceptsMidi() const override
    {
        return true;
    }
    bool producesMidi() const override
    {
        return true;
    }

    //==============================================================================
    int getNumPrograms() override
    {
        return numProgs;
    }
    int getCurrentProgram() override
    {
        return curProgram;
    }
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override
    {
        return programs->get(index, "Name");
    }
    void changeProgramName(int index, const juce::String& newName) override
    {
        programs->set(index, "Name", newName);
    }
    double getTailLengthSeconds() const override
    {
        return 0;
    }

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    void getCurrentProgramStateInformation(juce::MemoryBlock& destData) override;
    void setCurrentProgramStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    juce::MidiKeyboardState progKbState[numProgs][128];
    juce::MidiKeyboardState chordKbState;
    juce::MidiKeyboardState triggerKbState;
    juce::MidiKeyboardState* getCurrentKbState()
    {
        return &(progKbState[curProgram][curTrigger]);
    }
    void selectTrigger(int index);
    void selectChordNote(int index, int note, bool on, int ch = -1);
    int getCurrentTrigger()
    {
        return curTrigger;
    }
    bool isTriggerNotePlaying(int channel, int note)
    {
        return notePlaying[channel][note];
    }
    bool isTriggerNotePlaying(int note)
    {
        for (int i = 0; i < 16; i++)
            if (notePlaying[i][note])
                return true;
        return false;
    }
    int getLearnChannel()
    {
        return learnchan;
    }
    void setNoteBypassed(int note, bool bypass)
    {
        programs->set(curProgram, "Bypassed" + juce::String(note), bypass);
    }
    bool isNoteBypassed(int note)
    {
        return programs->get(curProgram, "Bypassed" + juce::String(note));
    }
    void clearAllChords();
    void resetAllChords();
    void copyChordToAllTriggers(bool absolute);
    void clearChord(int trigger);
    void resetChord(int trigger);
    void transposeAll(bool up);
    void transposeChord(int trigger, bool up);
    void transposeCurrentChordByOctave(bool up);
    void applyChannelToChord();
    juce::String getCurrentChordName();
    void savePreset(juce::String name);
    void playCurrentChord(bool on)
    {
        if (on)
            playFromGUI = true;
        else
            stopPlayingFromGUI = true;
    }
    bool isPreviewChordPlaying()
    {
        return playFromGUI;
    }
    void translateToGuitarChord(bool force = false);
    void setGuitarView(bool on)
    {
        if (getGuitarView())
            savedGuitarVoicing[curTrigger] = true;
        programs->set(curProgram, "GuitarView", on);
        if (on /* && !savedGuitarVoicing[curTrigger]*/)
        {
            translateToGuitarChord();
        }
    }
    bool getGuitarView()
    {
        return programs->get(curProgram, "GuitarView");
    }
    void setSavedGuitarVoicing(bool isValid)
    {
        savedGuitarVoicing[curTrigger] = isValid;
    }
    void setStringValue(int string, int note, bool translate)
    {
        programs->set(curProgram, "String" + juce::String(string), note);
        if (translate)
            translateToGuitarChord();
    }
    int getStringValue(int string)
    {
        return programs->get(curProgram, "String" + juce::String(string));
    }
    void setNumFrets(int frets, bool translate)
    {
        programs->set(curProgram, "NumFrets", frets);
        if (translate)
            translateToGuitarChord();
    }
    int getNumFrets()
    {
        return programs->get(curProgram, "NumFrets");
    }
    void setNumStrings(int strings, bool translate)
    {
        int n = getNumStrings();
        if (n == strings)
            return;
        while (n < strings)
        {
            setStringValue(n, 0, false);
            ++n;
        }
        while (n > strings)
        {
            --n;
            setStringValue(n, -1, false);
        }
        programs->set(curProgram, "NumStrings", strings);
        if (translate)
            translateToGuitarChord();
    }
    int getNumStrings()
    {
        return programs->get(curProgram, "NumStrings");
    }
    void setStrumDirection(bool up)
    {
        programs->set(curProgram, "StrumUp" + juce::String(curTrigger), up);
        sendChangeMessage();
    }
    bool getStrumDirection()
    {
        return programs->get(curProgram, "StrumUp" + juce::String(curTrigger));
    }
    void readChorderPreset(juce::File file);
    bool readKeyFile(juce::File file = juce::File());
    bool demo;

    juce::Array<GuitarDefinition> guitarPresets;
    void fillGuitarPresetList();

    void toggleUsePC(bool use);

    juce::String dataPath;
    int lastUIWidth, lastUIHeight;

private:
    MidiChordsPrograms* programs;
    int curProgram;
    int curTrigger;
    //StringArray chordNames;

    bool init;

    int channel;
    int learnchan;
    int outchan;
    bool learn;
    bool follow;
    bool usepc;
    int transpose;
    int previewVel;
    int mode;
    int root;
    bool guess;
    bool flats;
    bool inputtranspose;
    bool ccToAllChannels;

    bool strum;
    float fSpeed;
    float fMaxDelay;
    float fVelRamp;
    float fAccel;
    //float fVelToSpeed;

    bool savedGuitarVoicing[128];
    bool playingFromGUI, playFromGUI, stopPlayingFromGUI;
    int playButtonTrigger;
    int learning;
    bool notePlaying[16][128]; //trigger note
    int outputNote[16][128];
    int numChordNotes[numProgs][128];

    juce::Array<ChordNote> playingChord[128];
    bool chordNotePlaying[16][128];
    bool chordNotePlaying2[16][128];
    juce::Array<int> ignoreNextNoteOff[16];

    void copySettingsToProgram(int index);

    juce::MidiBuffer delayBuffer;
    int noteDelay[16][128];
    juce::uint64 noteOrigPos[16][128];
    juce::uint64 totalSamples;
    bool expectingDelayedNotes;
    juce::AudioPlayHead::CurrentPositionInfo lastPosInfo;

    JUCE_LEAK_DETECTOR(MidiChords)
};

#endif
