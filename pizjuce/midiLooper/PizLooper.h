#ifndef PIZLOOPERPLUGINFILTER_H
#define PIZLOOPERPLUGINFILTER_H

#include <memory>

#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_data_structures/juce_data_structures.h"

#include "piz/JArray.h"
#include "piz/key.h"
#include "piz/PizAudioProcessor.h"

#include "MidiLoop.h"

//==============================================================================
enum parameters
{
    kThru = 0,        //midi thru
    kFile,            //clear/load loop from midi file
    kSave,            //save midi file on demand
    kSync,            //sync mode
    kRecStep,         //loop length step
    kQuantize,        //input quantize
    kFixedLength,     //fixed-length recording time
    kRecMode,         //replace/overdub
    kSingleLoop,      //loop play follows slot selection
    kPlayMain,        //plays the active slot
    kRecordMain,      //records to the active slot
    kMasterVelocity,  //applies velocity scale to all played notes
    kMasterTranspose, //applies transposition to all played notes (after forcing to scale)
    kMidiOutDevice,
    kParamsToHost,
    kPlayCC,
    kRecCC,
    kMonitor,

    numGlobalParams,

    kRecord = numGlobalParams, //record toggle
    kPlay,                     //play toggle
    kTranspose,                //live transposition by semitone
    kOctave,                   //live transposition by octave
    kVelocity,                 //live velocity offset
    kVeloSens,                 //velocity sensitivity for note triggering
    kLoopStart,                //loop start offset
    kLoopEnd,                  //loop end offset
    kShift,                    //beat shift
    kStretch,                  //time stretch
    kTrigger,                  //trigger mode
    kNoteTrig,                 //midi note triggering
    kRoot,                     //root (no transposition) note for note triggering
    kNLow,                     // low
    kNHigh,                    //     & high notes: range to use for note triggering
    kFullRelease,              //always play loop to end (not implemented)
    kWaitForBar,               //play/stop on next bar
    kNumLoops,                 //number of times to play slot, 0=>loop forever
    kNextSlot,                 //slot to play after this one
    kPlayGroup,
    kMuteGroup,
    kForceToKey,       //on/off
    kScaleChannel,     //midi channel for force to scale
    kTransposeChannel, //midi channel for transpose
    kUseScaleChannel,  //toggle using midi channel for force to scale
    kUseTrChannel,     //toggle using midi channel for transposing
    kNote0,
    kNote1,
    kNote2,
    kNote3,
    kNote4,
    kNote5,
    kNote6,
    kNote7,
    kNote8,
    kNote9,
    kNote10,
    kNote11,
    kForceToScaleMode,
    kImmediateTranspose,
    kTranspose10,
    kReverse,    //forward/reverse playback
    kNoteToggle, //note triggers toggle or momentary
    kChannel,    //midi channel (input filter, output transformer)
    kTrigChan,   //midi channel for note triggering
    kFiltChan,   //channel filter/transformer switch
    numParamsPerSlot = kFiltChan + 1 - numGlobalParams,
    numPrograms      = 128,
    numSlots         = numPrograms,

    numParams = numParamsPerSlot * numSlots + numGlobalParams,
};

enum ForceToKeyModes
{
    nearest,
    alwaysup,
    alwaysdown,
    block,
    numForceToKeyModes
};

class PianoRollSettings
{
public:
    PianoRollSettings (PianoRollSettings* fallback_settings = nullptr) : values_ ("PRSettingsContainer"),
                                                                         undo_manager_ (nullptr),
                                                                         fallback_settings_ (fallback_settings)
    {
        ValueTree tree ("PRSettings");
        values_.addChild (tree, 0, undo_manager_);
    }

    ValueTree& set (const Identifier& name, const var& newValue)
    {
        return values_.getChild (0).setProperty (name, newValue, undo_manager_);
    }

    var get (const Identifier& name)
    {
        if (values_.getChild (0).hasProperty (name) && fallback_settings_ != nullptr)
        {
            return fallback_settings_->get (name);
        }
        else
        {
            return values_.getChild (0).getProperty (name);
        }
    }

    std::unique_ptr<XmlElement> createXml() const
    {
        return values_.getChild (0).createXml();
    }

    void loadXml (XmlElement const& xmlProgram)
    {
        values_.removeChild (0, undo_manager_);
        auto tree = xmlProgram.getChildByName ("PRSettings");
        if (tree != nullptr)
        {
            values_.addChild (ValueTree::fromXml (*tree), 0, undo_manager_);
        }
    }

private:
    void loadDefaults()
    {
        set ("slot", 0);
        set ("snap", true);
        set ("dotted", false);
        set ("triplet", false);
        set ("stepsize", 0.5f);
        set ("width", 500);
        set ("height", 1200);
        set ("x", 0);
        set ("y", 500);
        set ("bars", 4);
    }

    ValueTree values_;
    UndoManager* undo_manager_;
    PianoRollSettings* fallback_settings_;
};

//==============================================================================
class JuceProgram
{
    friend class PizLooper;

public:
    JuceProgram();
    ~JuceProgram();

private:
    float param[numGlobalParams + numParamsPerSlot];
    Loop loop;
    double loopstart;
    double looplength;
    double measureFromHere;
    String name;
    int numerator, denominator;
    PianoRollSettings PRSettings;
    MidiDeviceInfo device;
};

//==============================================================================
class PizLooper : public PizAudioProcessor,
                  public ChangeBroadcaster,
                  public MidiKeyboardStateListener
{
public:
    //==============================================================================
    PizLooper();
    ~PizLooper() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor (void) const override { return true; }
    //==============================================================================
    const String getName() const override;

    int getNumParameters() override { return numParams; }
    inline float getParameterForSlot (int parameter, int slot)
    {
        if (parameter < numGlobalParams)
            return getParameter (parameter);
        return getParameter (parameter + slot * numParamsPerSlot);
    }
    inline float getParamForActiveSlot (int parameter)
    {
        if (parameter < numGlobalParams)
            return getParameter (parameter);
        return getParameter (parameter + curProgram * numParamsPerSlot);
    }
    inline bool isSlotPlaying (int slot) { return getParameterForSlot (kPlay, slot) >= 0.5f && programs[slot].looplength > 0; }
    void playSlot (int slot)
    {
        notifyHost (kPlay, slot, 1.f);
        processGroups (slot);
    }
    void stopSlot (int slot)
    {
        notifyHost (kPlay, slot, 0.f);
        processGroups (slot);
    }
    void toggleSlotPlaying (int slot)
    {
        notifyHost (kPlay, slot, 1.f - getParameterForSlot (kPlay, slot));
        processGroups (slot);
    }
    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;
    void setParameterNotifyingHost (int parameterIndex, float newValue)
    {
        if (getParameter (kParamsToHost) >= 0.5f)
            AudioProcessor::setParameterNotifyingHost (parameterIndex, newValue);
        else
            setParameter (parameterIndex, newValue);
    }

    void notifyHost (int parameter, int slot, float value)
    {
        if (parameter < numGlobalParams)
            setParameterNotifyingHost (parameter, value);
        else
            setParameterNotifyingHost (parameter + slot * numParamsPerSlot, value);
    }
    void setParameterForSlot (int parameter, int slot, float value)
    {
        if (parameter < numGlobalParams)
            setParameter (parameter, value);
        else
            setParameter (parameter + slot * numParamsPerSlot, value);
    }
    void notifyHostForActiveSlot (int parameter, float value)
    {
        if (parameter < numGlobalParams)
            setParameterNotifyingHost (parameter, value);
        else
            setParameterNotifyingHost (parameter + curProgram * numParamsPerSlot, value);
    }
    const String getParameterName (int index) override;
    const String getParameterText (int index) override;
    const String getCurrentSlotParameterText (int parameter)
    {
        if (parameter < numGlobalParams)
            return getParameterText (parameter);
        return getParameterText (parameter + curProgram * numParamsPerSlot);
    }

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    double getTailLengthSeconds() const override { return 0; }

    //==============================================================================
    int getNumPrograms() override { return numPrograms; }
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override
    {
        setActiveSlot (index);
    }
    void resetCurrentProgram (int index);
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    void setActiveSlot (int slot);

    void setSize (int w, int h)
    {
        lastUIWidth  = w;
        lastUIHeight = h;
    }

    //void setCCMap(int index, int cc)
    //{
    //	if (index==kRecordMain)
    //		setParameter(kRecCC,(float)(cc+2)/129.f);
    //	else if (index==kPlayMain)
    //		setParameter(kPlayCC,(float)(cc+2)/129.f);
    //}
    //int getCCMap(int index)
    //{
    //	switch(index)
    //	{
    //	case kRecordMain: return recCC;
    //	case kPlayMain: return playCC;
    //	default: return -1;
    //	}
    //}

    //==============================================================================
    //void getCurrentProgramStateInformation (MemoryBlock& destData);
    //void setCurrentProgramStateInformation (const void* data, int sizeInBytes);
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void handleNoteOn (MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override
    {
        setParameterForSlot (kNote0 + midiNoteNumber % 12, curProgram, 1.f);
    }
    void handleNoteOff (MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override
    {
        setParameterForSlot (kNote0 + midiNoteNumber % 12, curProgram, 0.f);
    }

    //==============================================================================
    AudioPlayHead::CurrentPositionInfo lastPosInfo;

    int lastUIWidth, lastUIHeight;
    double currentLength;
    int currentNumEvents;
    class Info : public ChangeBroadcaster
    {
    public:
        String s;
    };
    Info* info;
    void killNotes (int slot);
    bool newLoop;
    bool readKeyFile (File file = File());
    bool demo;

    void loadMidiFile (File file);
    Loop* getActiveLoop();
    void updateLoopInfo();
    void setLoopLength (int slot, double newLength)
    {
        programs[slot].looplength = newLength;
        if (slot == curProgram)
            currentLength = newLength;
    }
    double getLoopLength (int slot)
    {
        return programs[slot].looplength;
    }
    void setLoopStart (int slot, double newStart)
    {
        programs[slot].loopstart = newStart;
        for (int instance = 0; instance < polyphony; instance++)
            lastPlayedIndex[slot][instance] = -1;
    }
    double getLoopStart (int slot)
    {
        return programs[slot].loopstart;
    }
    double getLoopEnd (int slot)
    {
        return programs[slot].loopstart + programs[slot].looplength;
    }
    bool isLoopEmpty (int slot)
    {
        return programs[slot].loop.getNumEvents() == 0;
    }
    MidiKeyboardState keySelectorState;

    int getNumerator (int slot) { return programs[slot].numerator; }
    int getDenominator (int slot) { return programs[slot].denominator; }
    void setTimeSig (int slot, int n, int d)
    {
        programs[slot].numerator   = n;
        programs[slot].denominator = d;
    }

    //midi out stuff
    piz::JArray<MidiDeviceInfo> devices;
    void setActiveDevice (String name);
    void setActiveDevice (MidiDeviceInfo device);
    MidiDeviceInfo getActiveDevice() { return activeDevice; }
    MidiDeviceInfo getDeviceByName (String name) const;

    void setPRSetting (const Identifier& name, const var& value, bool updateEditor = true)
    {
        programs[curProgram].PRSettings.set (name, value);
        if (updateEditor)
            sendChangeMessage();
    }

    const var getPRSetting (const Identifier& name)
    {
        return programs[curProgram].PRSettings.get (name);
    }

    double getPlayPosition (bool& playing, bool& recording)
    {
        playing   = isSlotPlaying (curProgram);
        recording = getParameterForSlot (kRecord, curProgram) >= 0.5f;
        return loopPpq[curProgram];
    }
    MidiKeyboardState kbstate;

    String loopDir;
    bool writeMidiFile (int index, File file, bool IncrementFilename = false);
    bool readMidiFile (int index, String progname, File mid = File());

    //==============================================================================
    juce_UseDebuggingNewOperator

        private : float param[numParams];
    int recCC, playCC;

    JuceProgram* programs;
    int curProgram;

    PizMidiMessageSequence oldloop[numPrograms];
    PizMidiMessageSequence temploop[numPrograms];
    double loopPpq[numPrograms];
    double oldlength[numPrograms];
    double newlooplength[numPrograms];
    double measureDubFromHere[numPrograms];
    double measurePlayFromHere[numPrograms][polyphony];
    double recstart[numPrograms];
    double recstop[numPrograms];
    float lastrec[numPrograms];
    float lastplay[numPrograms];
    double lastshift[numPrograms];
    double lastend[numPrograms];
    double laststart[numPrograms];
    double laststretch[numPrograms];
    int lastchannel[numPrograms];
    struct h
    {
        int vel[16][128];   // velocity of currently playing input notes
        int count[16][128]; // number of instances of each note
    } hanging[numPrograms];
    int inputNoteTransposition[16][128];
    int recNoteOn[numPrograms][16][128];
    int polytrigger[numPrograms][polyphony]; // note numbers of polyphonic triggers
    int currentPoly[numPrograms];            // number of playing instances of current pattern

    void endHangingNotesInLoop (MidiBuffer& buffer, int samplePos, int slot, int voice = -1, bool kill = false);
    class TransposeRules
    {
    public:
        TransposeRules (PizLooper* _plugin, int _slot) : plugin (0), slot (_slot)
        {
            plugin = _plugin;
            for (int i = 0; i < polyphony; i++)
                trignote[i] = -1;
            update (true);
        }
        ~TransposeRules() {}

        int trignote[polyphony];
        float velscale[polyphony];
        struct Rules
        {
            int semitones;
            int octaves;
            bool forceToScale;
            bool noteswitch[12];
            bool transposedTrigger;
            int root;
            int masterTranspose;
            int mode;
            bool transpose10;
        } rules, oldrules;

        int getSlot() const { return slot; }

        void trigger (int note, float vel, int voice)
        {
            trignote[voice] = note;
            velscale[voice] = vel;
        }

        void release (int voice)
        {
            trignote[voice] = -1;
            velscale[voice] = 1.f;
        }

        bool update (bool initialize = false) //return true if changed
        {
            if (! initialize)
                memcpy (&oldrules, &rules, sizeof (Rules));
            else if (memcmp (&rules, &oldrules, sizeof (Rules)) == 0)
                return false;
            rules.semitones         = roundToInt (plugin->getParameterForSlot (kTranspose, slot) * 24.f) - 12;
            rules.octaves           = roundToInt (plugin->getParameterForSlot (kOctave, slot) * 8.f) - 4;
            rules.forceToScale      = plugin->getParameterForSlot (kForceToKey, slot) >= 0.5f;
            rules.noteswitch[0]     = plugin->getParameterForSlot (kNote0, slot) >= 0.5f;
            rules.noteswitch[1]     = plugin->getParameterForSlot (kNote1, slot) >= 0.5f;
            rules.noteswitch[2]     = plugin->getParameterForSlot (kNote2, slot) >= 0.5f;
            rules.noteswitch[3]     = plugin->getParameterForSlot (kNote3, slot) >= 0.5f;
            rules.noteswitch[4]     = plugin->getParameterForSlot (kNote4, slot) >= 0.5f;
            rules.noteswitch[5]     = plugin->getParameterForSlot (kNote5, slot) >= 0.5f;
            rules.noteswitch[6]     = plugin->getParameterForSlot (kNote6, slot) >= 0.5f;
            rules.noteswitch[7]     = plugin->getParameterForSlot (kNote7, slot) >= 0.5f;
            rules.noteswitch[8]     = plugin->getParameterForSlot (kNote8, slot) >= 0.5f;
            rules.noteswitch[9]     = plugin->getParameterForSlot (kNote9, slot) >= 0.5f;
            rules.noteswitch[10]    = plugin->getParameterForSlot (kNote10, slot) >= 0.5f;
            rules.noteswitch[11]    = plugin->getParameterForSlot (kNote11, slot) >= 0.5f;
            rules.root              = floatToMidi (plugin->getParameterForSlot (kRoot, slot));
            rules.transposedTrigger = plugin->getParameterForSlot (kNoteTrig, slot) > 0.0
                                   && plugin->getParameterForSlot (kNoteTrig, slot) < 0.2f
                                   && rules.root > -1;
            rules.masterTranspose = roundToInt (plugin->getParameter (kMasterTranspose) * 24.f) - 12;
            rules.mode            = roundToInt (plugin->getParameterForSlot (kForceToScaleMode, slot) * (float) (numForceToKeyModes - 1));
            rules.transpose10     = plugin->getParameterForSlot (kTranspose10, slot) >= 0.5f;
            if (initialize)
                memcpy (&oldrules, &rules, sizeof (Rules));
            return memcmp (&rules, &oldrules, sizeof (Rules)) != 0;
        }

        bool changed()
        {
            return memcmp (&rules, &oldrules, sizeof (Rules)) != 0;
        }

        int getTransposedNote (int note, int voice, bool& killNote, int channel, bool isInputNote = false)
        {
            if (channel == 10 && ! rules.transpose10)
                return note;
            int transposey = 0;
            int interval   = (! isInputNote && rules.transposedTrigger) ? (trignote[voice] - rules.root) : 0;
            if (interval != 0 && rules.forceToScale)
            {
                //first force trigger note to scale
                if (! rules.noteswitch[trignote[voice] % 12])
                {
                    int force = 0;
                    int j     = -1;
                    switch (rules.mode)
                    {
                        //nearest note, down when tied (same as ndc)
                        case nearest:
                            killNote = true;
                            while (j < 12)
                            {
                                if (rules.noteswitch[(trignote[voice] + j) % 12])
                                {
                                    force    = j;
                                    killNote = false;
                                    break;
                                }
                                if (j < 0)
                                    j = -j;
                                else
                                    j = -j - 1;
                            }
                            break;
                        //always up
                        case alwaysup:
                            j        = 1;
                            killNote = true;
                            while (j < 12)
                            {
                                if (rules.noteswitch[(trignote[voice] + j) % 12])
                                {
                                    force    = j;
                                    killNote = false;
                                    break;
                                }
                                j++;
                            }
                            break;
                        //always down
                        case alwaysdown:
                            killNote = true;
                            while (j < 12)
                            {
                                if (rules.noteswitch[(trignote[voice] + j) % 12])
                                {
                                    force    = j;
                                    killNote = false;
                                    break;
                                }
                                j--;
                            }
                            break;
                        //block wrong notes
                        case block:
                            killNote = true;
                            release (voice);
                            return note;
                        default:
                            break;
                    }
                    trignote[voice] += force;
                }
                int m       = 0;
                int counter = 0;
                if (trignote[voice] > rules.root)
                {
                    while (counter < (trignote[voice] - rules.root))
                    {
                        m++;
                        if (rules.noteswitch[(rules.root + m) % 12])
                            counter++;
                        if (trignote[voice] - m == rules.root)
                            break;
                    }
                }
                else if (trignote[voice] < rules.root)
                {
                    while (counter > (trignote[voice] - rules.root))
                    {
                        m++;
                        if (rules.noteswitch[(rules.root - m) % 12])
                            counter--;
                        if (trignote[voice] + m == rules.root)
                            break;
                    }
                }
                transposey = counter;
                interval   = 0;
            }
            int newNote = note + rules.semitones + rules.octaves * 12;
            if (rules.forceToScale)
            {
                if (! rules.noteswitch[newNote % 12])
                {
                    int force = 0;
                    int j     = -1;
                    switch (rules.mode)
                    {
                        //nearest note, down when tied (same as ndc)
                        case nearest:
                            killNote = true;
                            while (j < 12)
                            {
                                if (rules.noteswitch[(newNote + j) % 12])
                                {
                                    force    = j;
                                    killNote = false;
                                    break;
                                }
                                if (j < 0)
                                    j = -j;
                                else
                                    j = -j - 1;
                            }
                            break;
                        //always up
                        case alwaysup:
                            j        = 1;
                            killNote = true;
                            while (j < 12)
                            {
                                if (rules.noteswitch[(newNote + j) % 12])
                                {
                                    force    = j;
                                    killNote = false;
                                    break;
                                }
                                j++;
                            }
                            break;
                        //always down
                        case alwaysdown:
                            killNote = true;
                            while (j < 12)
                            {
                                if (rules.noteswitch[(newNote + j) % 12])
                                {
                                    force    = j;
                                    killNote = false;
                                    break;
                                }
                                j--;
                            }
                            break;
                        //block wrong notes
                        case block:
                            killNote = true;
                            break;
                        default:
                            break;
                    }
                    newNote += force;
                }
                if (transposey > 0)
                {
                    //move the note up to the right scale degree
                    int counter = 0;
                    int m       = 0;
                    while (counter < transposey)
                    {
                        m++;
                        if (rules.noteswitch[(newNote + m) % 12])
                            counter++;
                        if (newNote + m == 127)
                            break;
                    }
                    newNote += m;
                }
                else if (transposey < 0)
                {
                    //move the note down the scale
                    int counter = 0;
                    int m       = 0;
                    while (counter > transposey)
                    {
                        m++;
                        if (rules.noteswitch[(newNote - m) % 12])
                            counter--;
                        if (newNote - m == 0)
                            break;
                    }
                    newNote -= m;
                }
                jassert (! killNote);
            }
            newNote += interval + rules.masterTranspose;
            if (newNote > 127 || newNote < 0)
                killNote = true;
            return newNote;
        }

    private:
        PizLooper* plugin;
        int slot;
    };
    TransposeRules* tRules[numSlots];
    void transposePlayingNotes (MidiBuffer& buffer, int samplePos, int slot, int voice, int numSamples);

    void processGroups (int slot)
    {
        int pgroup = roundToInt (getParameterForSlot (kPlayGroup, slot) * 16.f) - 1;
        int mgroup = roundToInt (getParameterForSlot (kMuteGroup, slot) * 16.f) - 1;
        if (mgroup != -1 && getParameterForSlot (kPlay, slot) >= 0.5f)
        {
            for (int i = 0; i < numSlots; i++)
                if (i != slot && mgroup == (roundToInt (getParameterForSlot (kMuteGroup, i) * 16.f) - 1))
                {
                    for (int v = 0; v < polyphony; v++)
                        if (slot != i)
                            polytrigger[i][v] = -1;
                    notifyHost (kPlay, i, 0.f);
                }
        }
        if (pgroup != -1)
        {
            for (int i = 0; i < numSlots; i++)
                if (i != slot && pgroup == (roundToInt (getParameterForSlot (kPlayGroup, i) * 16.f) - 1))
                    notifyHost (kPlay, i, getParameterForSlot (kPlay, slot));
        }
    }

    int FilenameIndex;

    bool init;
    bool kill_all_notes[numSlots];
    int capturingScale[numSlots];
    int capturingTranspose[numSlots];
    int lowestScaleInputNote[numSlots];
    int playingNote[16][128]; //keep track of the actual notes that are output
    int64 barTriggerSample[numSlots][polyphony];
    int64 barStopSample[numSlots][polyphony];
    int lastPlayedIndex[numSlots][polyphony];
    int lastLoopCount[numSlots][polyphony];
    int startLoopCount[numSlots][polyphony];
    Array<LoopNote> noteOffBuffer[numSlots];
    int getIndexOfNoteOff (PizMidiMessageSequence::MidiEventHolder* n, int slot, int v)
    {
        for (int i = 0; i < noteOffBuffer[slot].size(); i++)
        {
            if ((noteOffBuffer[slot].getUnchecked (i).note->noteOffObject.get() == n
                 || noteOffBuffer[slot].getUnchecked (i).note.get() == n)
                && noteOffBuffer[slot].getUnchecked (i).voice == v)
                return i;
        }
        return -1;
    }
    bool recordAndPlay[numSlots];

    uint64 samples;
    int samplesInLastBuffer;

    MidiDeviceInfo activeDevice;
    std::unique_ptr<MidiOutput> midiOutput;

    PianoRollSettings defaultPRSettings;

    double getStretchMultiplier (int slot);
    bool isNoteTriggeringAnySlot (MidiMessage const& message);
    bool processTriggerNote (const int slot, MidiMessage& message, int root, MidiBuffer& midiout, const int sample, const double ppqOfNextBar, const double ppqPerSample, const double ppqOfLastStep, const double looplengthstep);
    void processNoteOffBuffer (int slot, MidiBuffer& midiout, int numSamples);
    void playLoopEvent (int slot, int instance, int eventindex, int channel, double stretch, int samplepos, double samplesPerPpq, MidiBuffer& midiout);
    double getLoopLengthStep();
    double getPpqOfLastLoopStart (double ppq, int slot);
    void recordMessage (const MidiMessage& midi_message, int slot, bool playing, double ppq, double eventoffset, double ppqPerSample, int sample_number);

    int slotLimit;
};

#endif
