#ifndef PIZ_MIDI_LOOP_HEADER
#define PIZ_MIDI_LOOP_HEADER

#include "midistuff.h"
#include "piz_MidiMessageSequence.h"
#define polyphony 4

enum playmodes
{
    playStep,
    playOneShot,
    playLooped,
    playWithHost,
    playLoopedWithHost,
    numPlayModes
};

enum hoveringNoteStates
{
    //if not one of these, it's the index of an existing note
    No_Note         = -1,
    New_Note        = -2,
    Dragging_A_Note = 9999
};

struct LoopNote
{
    LoopNote()
    {
        note                 = nullptr;
        lastOutputNoteNumber = NOT_PLAYING;
        lastOutputChannel    = NOT_PLAYING;
        noteOffSample        = -1;
        voice                = -1;
    }

    LoopNote(PizMidiMessageSequence::mehPtr object, int n, int ch, int offSample, int v)
    {
        note                 = object;
        lastOutputNoteNumber = n;
        lastOutputChannel    = ch;
        noteOffSample        = offSample;
        voice                = v;
    }

    void subtractFromTime(int samples)
    {
        noteOffSample -= samples;
    }

    PizMidiMessageSequence::mehPtr note;
    int lastOutputNoteNumber;
    int lastOutputChannel;
    int noteOffSample;
    int voice;
};

class Loop : public PizMidiMessageSequence
{
public:
    Loop();
    Loop(PizMidiMessageSequence sequence, int triggerNote_, int transpose_ = 0, bool playMode_ = 0);
    ~Loop();

    void startRecording();
    bool findNextNote();
    void playAllNotesAtCurrentTime(juce::MidiBuffer& buffer, int sample_number, int velocity);
    void sendCurrentNoteToBuffer(juce::MidiBuffer& buffer, int sample_number, int velocity);
    bool isNotePlaying(PizMidiMessageSequence::mehPtr note, int p);
    bool isNotePlaying(PizMidiMessageSequence::mehPtr note);
    bool isTriggerNote(int note);
    void setTriggerNote(int note);
    void resetNotes();
    void sendNoteOffMessagesToBuffer(juce::MidiBuffer& buffer, int sample_number);
    juce::MidiMessage getCurrentMessage();
    double getCurrentTime();
    int getIndexOfNote(int noteNumber, double time, bool exact = false);
    void convertTimeBase(short timeBase);
    int getTransposition();
    int getTranspositionOfNote(PizMidiMessageSequence::mehPtr note, int p);
    int getChannelOfNote(PizMidiMessageSequence::mehPtr note, int p);
    void setSemitones(int semitones);
    void setOctaves(int octaves);
    void setNoteOff(PizMidiMessageSequence::mehPtr note, int p);
    void setNoteOff(PizMidiMessageSequence::mehPtr note);
    void sentNoteAs(PizMidiMessageSequence::mehPtr note, int p, int sentNoteNumber, int sentChannel, int offSample);
    //void setNoteChannel(MidiEventHolder* note, int p);
    void cleanZeroLengthNotes();

    void setLength(double newLength)
    {
        length = 960.0 * newLength;
    }

    double getLength()
    {
        return length;
    }

    int currentIndex;
    int playMode;
    bool isRecording;
    bool isRecArmed;
    int recChannel;
    int outChannel;
    double recTime;
    int indexOfLastNoteOn;
    float velocitySensitivity;
    double chordTolerance;
    double Qdelta[128][16]; // quantization shift for each note (to shift noteoff the same amount)
    juce::Array<LoopNote> playingNotes[polyphony];

private:
    double length;
    int transpose;
    int octave;
    int triggerNote;
    //int playingNote[128][16][4]; // notes playing from the loop, value is how much they're transposed
    //int channelOfNote[128][16][4];
};

#endif
