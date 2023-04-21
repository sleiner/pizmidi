#ifndef PIZ_PIANO_ROLL_HEADER
#define PIZ_PIANO_ROLL_HEADER

#include "../data/MidiLoop.h"
#include "../data/PizNote.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "PianoRollBackground.h"
#include "PianoRollNotes.h"
#include "PlayBar.h"
#include "Timeline.h"

class PianoRoll : public juce::Component,
                  public juce::ChangeBroadcaster
{
public:
    PianoRoll(juce::AudioProcessor* _plugin, juce::AudioProcessorEditor* _owner, Timeline* _timeline);
    ~PianoRoll() override;

    void setSequence(Loop* sequence_);

    void sequenceChanged();
    double pixelsToPpq(float pixels, bool snap, bool round = false);
    float ppqToPixels(double ppq);
    int ppqToPixelsWithOffset(double ppq);
    double snapPpqToGrid(double ppq, bool round = false) const;
    void setNoteLength(float beatDiv);
    double blankLength;

    void setDisplayLength(double ppq);
    void setDisplayLength(int bars);

    void addBar();
    void removeBar();

    int getDisplayLength() const;

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

    void mouseDoubleClick(const juce::MouseEvent& e) override;

    void paintOverChildren(juce::Graphics& g) override;
    void resized() override;

    bool getSnap() const;
    void setSnap(bool snap);

    void setPlayTime(double timeInPpq);

    void setTimeSig(int n, int d);

    double getPpqPerBar() const;

    Component* getPlayBar();

    int defaultChannel;
    int timeSigN, timeSigD;
    bool playing;
    bool recording;

    void repaintBG();

    double seqLengthInPpq;
    double playTime;

    float gridSize; //quantization grid size in pixels
    float beatSize; //number of pixels per beat
    float barSize;  //number of pixels per measure

    Loop* sequence;

    float yinc;

    int draggingNoteTransposition;
    double draggingNoteTimeDelta;

    juce::Array<PizMidiMessageSequence::mehPtr> selectedNotes;

private:
    juce::Rectangle<int> lasso;
    juce::Array<PizNote> selectedNoteLengths;

    void addToSelection(PizMidiMessageSequence::mehPtr note);
    void clearSelection();

    Timeline* timeline;
    PlayBar* playBar;
    PianoRollBackground* bg;
    PianoRollNotes* noteLayer;
    juce::AudioProcessor* plugin;
    juce::AudioProcessorEditor* owner;
    PizMidiMessageSequence::mehPtr hoveringNote;
    int hoveringNoteIndex;
    double timebase;
    double stepLengthInPpq;

    float seqLength;

    double lastDragTime;
    juce::uint8 draggingNoteChannel;
    int draggingNoteNumber;
    int draggingNoteVelocity;
    int originalNoteVelocity;
    double draggingNoteLength;
    double draggingNoteStartTime;
    double draggingNoteEndOffset;

    bool wasResizing;
    bool snapToGrid;
    float noteLength;
    double quarterNotesPerBar;
};

#endif
