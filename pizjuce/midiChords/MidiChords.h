#ifndef MidiChordsPLUGINFILTER_H
#define MidiChordsPLUGINFILTER_H

#include "juce_core/juce_core.h"
#include "juce_data_structures/juce_data_structures.h"

#include "../_common/BankStorage.h"
#include "../_common/ChordFunctions.h"
#include "../_common/PizAudioProcessor.h"
#include "../_common/key.h"
#include "../_common/midistuff.h"

using namespace juce;

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
    ChordNote (int channel, int note)
    {
        c = channel;
        n = note;
    }
    ChordNote()
    {
        c = n = -1;
    }
    ~ChordNote() {}

    int c;
    int n;
};

class GuitarDefinition
{
public:
    GuitarDefinition()
    {
        guitarName = String();
        chordFile = "Chords.txt";
        numFrets = 0;
        numStrings = 0;
    }

    GuitarDefinition (String name, String file, int frets, Array<int>& notes)
    {
        guitarName = name;
        chordFile = file;
        numFrets = frets;
        stringNotes.addArray (notes);
        numStrings = notes.size();
    }
    ~GuitarDefinition() {}

    String guitarName;
    int numStrings;
    int numFrets;
    Array<int> stringNotes;
    String chordFile;
};

class MidiChordsPrograms : public BankStorage
{
public:
    MidiChordsPrograms();

    void set (int prog, const Identifier& name, const var& newValue)
    {
        BankStorage::set (0, prog, name.toString(), newValue);
    }

    void clearNoteMatrix (int prog, int trigger)
    {
        auto noteMatrix = values_.getChild (0).getChild (prog).getChild (trigger);
        if (noteMatrix.isValid())
            noteMatrix.removeAllProperties (0);
    }

    void setChordNote (int prog, int trigger, int channel, int note, const bool& newValue)
    {
        channel -= 1;
        auto triggerProp = values_.getChild (0).getChild (prog).getChild (trigger);

        if (! triggerProp.isValid())
        {
            ValueTree noteMatrix ("NoteMatrix_T" + String (trigger));
            values_.getChild (0).getChild (prog).addChild (noteMatrix, trigger, 0);
        }
        if (note < 32)
        {
            int state = triggerProp.getProperty ("Ch" + String (channel) + "_0-31", 0);
            if (newValue)
                state |= 1 << note;
            else
                state &= ~(1 << note);
            triggerProp.setProperty ("Ch" + String (channel) + "_0-31", state, 0);
        }
        else if (note < 64)
        {
            note -= 32;
            int state = triggerProp.getProperty ("Ch" + String (channel) + "_32-63", 0);
            if (newValue)
                state |= 1 << note;
            else
                state &= ~(1 << note);
            triggerProp.setProperty ("Ch" + String (channel) + "_32-63", state, 0);
        }
        else if (note < 96)
        {
            note -= 64;
            int state = triggerProp.getProperty ("Ch" + String (channel) + "_64-95", 0);
            if (newValue)
                state |= 1 << note;
            else
                state &= ~(1 << note);
            triggerProp.setProperty ("Ch" + String (channel) + "_64-95", state, 0);
        }
        else if (note < 128)
        {
            note -= 96;
            int state = triggerProp.getProperty ("Ch" + String (channel) + "_96-127", 0);
            if (newValue)
                state |= 1 << note;
            else
                state &= ~(1 << note);
            triggerProp.setProperty ("Ch" + String (channel) + "_96-127", state, 0);
        }
    }

    const var get (int prog, const Identifier& name)
    {
        return BankStorage::get (0, prog, name.toString());
    }

    bool getChordNote (int prog, int trigger, int ch, int note)
    {
        ch -= 1;
        auto triggerProp = values_.getChild (0).getChild (prog).getChild (trigger);

        if (! triggerProp.isValid())
            return false;
        if (note < 32)
        {
            int state = triggerProp.getProperty ("Ch" + String (ch) + "_0-31", 0);
            return (state & (1 << note)) != 0;
        }
        else if (note < 64)
        {
            note -= 32;
            int state = triggerProp.getProperty ("Ch" + String (ch) + "_32-63", 0);
            return (state & (1 << note)) != 0;
        }
        else if (note < 96)
        {
            note -= 64;
            int state = triggerProp.getProperty ("Ch" + String (ch) + "_64-95", 0);
            return (state & (1 << note)) != 0;
        }
        else if (note < 128)
        {
            note -= 96;
            int state = triggerProp.getProperty ("Ch" + String (ch) + "_96-127", 0);
            return (state & (1 << note)) != 0;
        }
        return false;
    }

    void fixupVelocity()
    {
        for (int b = 0; b < values_.getNumChildren(); b++)
        {
            auto bank = values_.getChild (b);
            for (int p = 0; p < bank.getNumChildren(); p++)
            {
                if (BankStorage::get (b, p, "Velocity") == var (0))
                {
                    BankStorage::set (b, p, "Velocity", 100);
                }
            }
        }
    }

private:
    void loadDefaults();
};

//==============================================================================
class MidiChords : public PizAudioProcessor,
                   public ChangeBroadcaster
{
public:
    //==============================================================================
    MidiChords();
    ~MidiChords() override;

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

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }

    //==============================================================================
    int getNumPrograms() override { return numProgs; }
    int getCurrentProgram() override { return curProgram; }
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override { return programs->get (index, "Name"); }
    void changeProgramName (int index, const String& newName) override { programs->set (index, "Name", newName); }
    double getTailLengthSeconds() const override { return 0; }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void getCurrentProgramStateInformation (MemoryBlock& destData) override;
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    MidiKeyboardState progKbState[numProgs][128];
    MidiKeyboardState chordKbState;
    MidiKeyboardState triggerKbState;
    MidiKeyboardState* getCurrentKbState() { return &(progKbState[curProgram][curTrigger]); }
    void selectTrigger (int index);
    void selectChordNote (int index, int note, bool on, int ch = -1);
    int getCurrentTrigger() { return curTrigger; }
    bool isTriggerNotePlaying (int channel, int note) { return notePlaying[channel][note]; }
    bool isTriggerNotePlaying (int note)
    {
        for (int i = 0; i < 16; i++)
            if (notePlaying[i][note])
                return true;
        return false;
    }
    int getLearnChannel() { return learnchan; }
    void setNoteBypassed (int note, bool bypass)
    {
        programs->set (curProgram, "Bypassed" + String (note), bypass);
    }
    bool isNoteBypassed (int note)
    {
        return programs->get (curProgram, "Bypassed" + String (note));
    }
    void clearAllChords();
    void resetAllChords();
    void copyChordToAllTriggers (bool absolute);
    void clearChord (int trigger);
    void resetChord (int trigger);
    void transposeAll (bool up);
    void transposeChord (int trigger, bool up);
    void transposeCurrentChordByOctave (bool up);
    void applyChannelToChord();
    String getCurrentChordName();
    void savePreset (String name);
    void playCurrentChord (bool on)
    {
        if (on)
            playFromGUI = true;
        else
            stopPlayingFromGUI = true;
    }
    bool isPreviewChordPlaying() { return playFromGUI; }
    void translateToGuitarChord (bool force = false);
    void setGuitarView (bool on)
    {
        if (getGuitarView())
            savedGuitarVoicing[curTrigger] = true;
        programs->set (curProgram, "GuitarView", on);
        if (on /* && !savedGuitarVoicing[curTrigger]*/)
        {
            translateToGuitarChord();
        }
    }
    bool getGuitarView()
    {
        return programs->get (curProgram, "GuitarView");
    }
    void setSavedGuitarVoicing (bool isValid)
    {
        savedGuitarVoicing[curTrigger] = isValid;
    }
    void setStringValue (int string, int note, bool translate)
    {
        programs->set (curProgram, "String" + String (string), note);
        if (translate)
            translateToGuitarChord();
    }
    int getStringValue (int string)
    {
        return programs->get (curProgram, "String" + String (string));
    }
    void setNumFrets (int frets, bool translate)
    {
        programs->set (curProgram, "NumFrets", frets);
        if (translate)
            translateToGuitarChord();
    }
    int getNumFrets()
    {
        return programs->get (curProgram, "NumFrets");
    }
    void setNumStrings (int strings, bool translate)
    {
        int n = getNumStrings();
        if (n == strings)
            return;
        while (n < strings)
        {
            setStringValue (n, 0, false);
            ++n;
        }
        while (n > strings)
        {
            --n;
            setStringValue (n, -1, false);
        }
        programs->set (curProgram, "NumStrings", strings);
        if (translate)
            translateToGuitarChord();
    }
    int getNumStrings()
    {
        return programs->get (curProgram, "NumStrings");
    }
    void setStrumDirection (bool up)
    {
        programs->set (curProgram, "StrumUp" + String (curTrigger), up);
        sendChangeMessage();
    }
    bool getStrumDirection()
    {
        return programs->get (curProgram, "StrumUp" + String (curTrigger));
    }
    void readChorderPreset (File file);
    bool readKeyFile (File file = File());
    bool demo;

    Array<GuitarDefinition> guitarPresets;
    void fillGuitarPresetList();

    void toggleUsePC (bool use);

    String dataPath;
    int lastUIWidth, lastUIHeight;
    //==============================================================================
    juce_UseDebuggingNewOperator

        private : MidiChordsPrograms* programs;
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

    Array<ChordNote> playingChord[128];
    bool chordNotePlaying[16][128];
    bool chordNotePlaying2[16][128];
    Array<int> ignoreNextNoteOff[16];

    void copySettingsToProgram (int index);

    MidiBuffer delayBuffer;
    int noteDelay[16][128];
    uint64 noteOrigPos[16][128];
    uint64 totalSamples;
    bool expectingDelayedNotes;
    AudioPlayHead::CurrentPositionInfo lastPosInfo;
};

#endif
