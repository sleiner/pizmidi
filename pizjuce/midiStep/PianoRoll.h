#ifndef PIZ_PIANO_ROLL_HEADER
#define PIZ_PIANO_ROLL_HEADER

#include "step.h"

class PianoRoll : public Component
{
public:
	PianoRoll(MidiStep* plugin_);
	~PianoRoll();

	void setSequence(Loop* sequence_);
	void sequenceChanged();
	int getTimeInPixels();
	double pixelsToPpq(float pixels, bool snap);
	float ppqToPixels(double ppq);
	double snapPpqToGrid(double ppq);
	void setNoteLength(int beatDiv);
	double blankLength;
	float pixelsPerPpq;
	float getNoteHeight();

    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);
    void mouseUp (const MouseEvent& e);
    void mouseMove (const MouseEvent& e);
    void mouseDoubleClick (const MouseEvent& e);

    void paint (Graphics& g);
    void resized ();

private:
	MidiStep* plugin;
	Loop* sequence;
	int hoveringNote;
	double timebase;
	double stepLengthInPpq;
	float seqLengthInPpq;
	float seqLength;
	float gridSize;
	int numEvents;
	float xinc;
	float yinc;
	double lastDragTime;
	uint8 draggingNoteChannel;
	int draggingNoteNumber;
	int draggingNoteVelocity;
	double draggingNoteLength;
	double draggingNoteStartTime;
	bool snapToGrid;
	float noteLength;
	int beat;
	int bar;

	//MidiKeyboardComponent* keyboard;
};

#endif
