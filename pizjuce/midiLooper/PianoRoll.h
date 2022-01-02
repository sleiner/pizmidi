#ifndef PIZ_PIANO_ROLL_HEADER
#define PIZ_PIANO_ROLL_HEADER

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "MidiLoop.h"

using namespace juce;

class PianoRoll;

struct PizNote
{
	PizNote()
	{
		object=nullptr;
		length=-1;
	}
	PizNote(PizMidiMessageSequence::mehPtr _object)
	{
		object = _object;
		jassert(_object->noteOffObject!=nullptr);
		length = _object->noteOffObject!=nullptr ? _object->noteOffObject->message.getTimeStamp() - _object->message.getTimeStamp() : 0;
	}
	void updateLength() {
		if (object->message.isNoteOn()) {
			jassert(object->noteOffObject!=nullptr);
			length = object->noteOffObject!=nullptr ? object->noteOffObject->message.getTimeStamp() - object->message.getTimeStamp() : 0;
		}
	}

	double length;
	PizMidiMessageSequence::mehPtr object;
};

class Timeline : public Component, public ChangeBroadcaster
{
	friend class PianoRoll;
public:
	Timeline();
	~Timeline();

	void setPianoRoll(PianoRoll* pr);

	void paint(Graphics& g);
	float getStartPixel();
	float getEndPixel();
	double getLength();
	double getStart();

	void setLoop(double start, double length);

	void mouseDown(const MouseEvent &e);
	void mouseDrag(const MouseEvent &e);

	int scrollOffset;

private:
	double loopStart;
	double loopEnd;
	PianoRoll* roll;
};

class PianoPort : public Viewport, public ChangeBroadcaster
{
public:
	PianoPort(String name) : Viewport(name) {};
	~PianoPort() {dispatchPendingMessages();}
	void setTimeline(Timeline* t) {timeline=t;}
	void setPlayline(Component* p) {playline=p;}
	void setKeyboard(Viewport* kb) {keyboard=kb;}
	void mouseWheelMove (const MouseEvent &e, const MouseWheelDetails& wheel) override
	{
		this->getParentComponent()->mouseWheelMove(e,wheel);
	}

	void visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea)
	{
		timeline->scrollOffset=newVisibleArea.getX();
		timeline->repaint();
		playline->setBounds(newVisibleArea);
		keyboard->setViewPosition(0,newVisibleArea.getY());
		sendChangeMessage();
	}
private:
	Timeline* timeline;
	Component* playline;
	Viewport* keyboard;
};

class PianoRoll : public Component,
				  public ChangeBroadcaster
{
	friend class Timeline;
	friend class PianoPort;
public:
	PianoRoll(AudioProcessor* _plugin, AudioProcessorEditor* _owner, Timeline* _timeline);
	~PianoRoll();

	void setSequence(Loop* sequence_);
	Loop* getSequence() {return sequence;}
	void sequenceChanged();
	int getTimeInPixels();
	double pixelsToPpq(float pixels, bool snap, bool round=false);
	float ppqToPixels(double ppq);
	int ppqToPixelsWithOffset(double ppq);
	double snapPpqToGrid(double ppq, bool round=false);
	void setNoteLength(float beatDiv);
	double blankLength;
	float pixelsPerPpq;
	float getNoteHeight();
	void setDisplayLength(double ppq) {blankLength = ppq; sequenceChanged();}
	void setDisplayLength(int bars) {
		int pixelBarLength = (int)ppqToPixels(getPpqPerBar());
		blankLength = getPpqPerBar()*bars; sequenceChanged();
		setSize((int)ppqToPixels(jmax(blankLength,seqLengthInPpq)),getHeight());
	}
	void addBar() {
		int pixelBarLength = (int)ppqToPixels(getPpqPerBar());
		setDisplayLength(seqLengthInPpq + getPpqPerBar());
		setSize(getWidth()+pixelBarLength,getHeight());
	}
	void removeBar() {
		int pixelBarLength = (int)ppqToPixels(getPpqPerBar());
		setDisplayLength(jmax(getPpqPerBar(),seqLengthInPpq - getPpqPerBar()));
		setSize(jmax(pixelBarLength,getWidth()-pixelBarLength),getHeight());
	}
	int getDisplayLength() {return (int)(jmax(blankLength,seqLengthInPpq) / getPpqPerBar());}

	void mouseDown (const MouseEvent& e);
	void mouseDrag (const MouseEvent& e);
	void mouseUp (const MouseEvent& e);
	//void mouseMove (const MouseEvent& e);
	void mouseDoubleClick (const MouseEvent& e);

	void paintOverChildren (Graphics& g);
	void resized ();

	bool getSnap() {return snapToGrid;}
	float getBeatDiv() {return 1.f/noteLength;}
	void setSnap(bool snap) {snapToGrid=snap;}
	void setPlayTime(double timeInPpq) {
		const int lastpixels=ppqToPixelsWithOffset(playTime);
		const int pixels=ppqToPixelsWithOffset(timeInPpq);
		if (pixels!=lastpixels) {
			playTime=timeInPpq;
			playline->repaint(lastpixels,0,1,getHeight());
			playline->repaint(pixels,0,1,getHeight());
		}
	}
	void setTimeSig(int n, int d);
	void setPlaying(bool isPlaying) {
		playing = isPlaying;
		playline->repaint();
	}
	void setRecording(bool isRecording) {
		recording = isRecording;
		playline->repaint();
	}
	double getPpqPerBar() {
		return timebase * quarterNotesPerBar;
	}

	Component* getPlayline() {return (Component*)playline;}

	int defaultChannel;
	int timeSigN, timeSigD;
	bool playing;
	bool recording;

	void repaintBG() {bg->repaint();}

private:
	juce::Rectangle<int> lasso;
	Array<PizMidiMessageSequence::mehPtr> selectedNotes;
	Array<PizNote> selectedNoteLengths;
	void addToSelection(PizMidiMessageSequence::mehPtr note)
	{
		if (note->message.isNoteOn()) {
			if (note->noteOffObject==nullptr)
				sequence->updateMatchedPairs();
			selectedNotes.addIfNotAlreadyThere(note);
			selectedNoteLengths.add(PizNote(note));
		}
	}
	void clearSelection()
	{
		selectedNotes.clear();
		selectedNoteLengths.clear();
	}

	class Playbar : public Component
	{
	public:
		Playbar(PianoRoll *pianoroll) : Component(), roll(pianoroll) {}
		~Playbar() {}
		void paint(Graphics& g)
		{
			g.fillAll(Colour(0x0));
			//if (sequence->isRecording) {
			//	g.setColour(Colours::red);
			//	g.drawVerticalLine((int)((float)sequence->recTime*(float)getWidth()/seqLengthInPpq),0.f,(float)getHeight());
			//}
			//else {
				g.setColour(Colours::green);
				g.drawVerticalLine((int)roll->ppqToPixelsWithOffset(roll->playTime),0.f,(float)getHeight());
			//}
		}
	private:
		PianoRoll* roll;
	};

	class PianoRollBackground : public Component
	{
	public:
		PianoRollBackground()
		{
			setBufferedToImage(true);
		}
		~PianoRollBackground() {}
		void paint(Graphics &g)
		{
			const PianoRoll* roll = (PianoRoll*)getParentComponent();
			int n=0;
			float y = (float)getHeight();
			float yinc = (float)getHeight()/128.f;

			while (y>0) {
				if (getNoteNameWithoutOctave(n).contains("#"))
					g.setColour(Colours::lightgrey);
				else if (n==60) g.setColour(Colours::yellow);
				else g.setColour(Colours::white);
				g.fillRect(0.f,y-yinc,(float)getWidth(),yinc);
				if (getNoteNameWithoutOctave(n).contains("F") && !getNoteNameWithoutOctave(n).contains("#"))
				{
					g.setColour(Colours::grey);
					g.drawLine(0.f,y,(float)getWidth(),y,1);
				}
				if (getNoteNameWithoutOctave(n).contains("C") && !getNoteNameWithoutOctave(n).contains("#"))
				{
					g.setColour(Colours::black);
					g.drawLine(0.f,y,(float)getWidth(),y,1);
				}
				n++;
				y-=yinc;
			}
			float x = roll->gridSize;
			while (x<getWidth()) {
				//draw grid
				if (!(fmod(x,roll->barSize)<0.0001) && !(fmod(x,roll->beatSize)<0.0001))
				{
					g.setColour(Colours::lightgrey);
					g.drawLine(x,0.f,x,(float)getHeight());
				}
				x+=roll->gridSize;
			}
			x = roll->beatSize;
			while (x<getWidth()) {
				//draw beats
				if (!(fmod(x,roll->barSize)<0.0001))
				{
					g.setColour(Colours::grey);
					g.drawLine(x,0.f,x,(float)getHeight());
				}
				x+=roll->beatSize;
			}
			x = roll->barSize;
			while (x<getWidth())
			{
				//draw bars
				g.setColour(Colours::black);
				g.drawLine(x,0.f,x,(float)getHeight());
				x+=roll->barSize;
			}
		}
	};

	class PianoRollNotes : public Component
	{
	public:
		PianoRollNotes()
		{
			setBufferedToImage(true);
		}
		~PianoRollNotes() {}
		void paint(Graphics &g)
		{
			const PianoRoll* roll = (PianoRoll*)getParentComponent();
			if (roll->barSize>0)
			{
				if (roll->sequence) {

					for(int i=0;i<roll->sequence->getNumEvents();i++)
					{
						if(roll->sequence->getEventPointer(i)->message.isNoteOn()/* && (i-9999!=hoveringNote)*/)
						{
							float noteLength = (float)(jmax(1.0,(roll->sequence->getEventTime(roll->sequence->getIndexOfMatchingKeyUp(i))-roll->sequence->getEventTime(i)))*(double)getWidth()/roll->seqLengthInPpq);
							//if (i==sequence->indexOfLastNoteOn
							//	|| abs(sequence->getEventTime(i)-sequence->getEventTime(sequence->indexOfLastNoteOn))<sequence->chordTolerance)
							//	g.setColour(Colours::blue);
							//else
							if (roll->selectedNotes.contains(roll->sequence->getEventPointer(i))) {
								//outline of original note position
								g.setColour(Colours::blue);
								g.drawRect( (float)getWidth()*(float)(roll->sequence->getEventTime(i)/roll->seqLengthInPpq),
									(float)getHeight() - (float)(roll->sequence->getEventPointer(i)->message.getNoteNumber())*roll->yinc-roll->yinc,
									noteLength,
									roll->yinc);

								//dragging note position
								const double newTime = (float)((roll->sequence->getEventTime(i)+roll->draggingNoteTimeDelta));
								const float newNote = (float)(roll->sequence->getEventPointer(i)->message.getNoteNumber()+roll->draggingNoteTransposition);
								g.setColour(Colours::darkgoldenrod.withAlpha(roll->sequence->getEventPointer(i)->message.getFloatVelocity()));
								g.fillRect( (float)getWidth()*(float)(newTime/roll->seqLengthInPpq),
											(float)getHeight() - newNote*roll->yinc-roll->yinc,
											noteLength,
											roll->yinc);
								g.setColour(Colours::red);
								g.drawRect( (float)getWidth()*(float)(newTime/roll->seqLengthInPpq),
									(float)getHeight() - newNote*roll->yinc - roll->yinc,
									noteLength,
									roll->yinc);
							}
							else {
								g.setColour(Colours::darkgoldenrod.withAlpha(roll->sequence->getEventPointer(i)->message.getFloatVelocity()));
								g.fillRect( (float)getWidth()*(float)(roll->sequence->getEventTime(i)/roll->seqLengthInPpq),
											(float)getHeight() - (float)(roll->sequence->getEventPointer(i)->message.getNoteNumber())*roll->yinc-roll->yinc,
											noteLength,
											roll->yinc);
								g.setColour(Colours::black);
								g.drawRect( (float)getWidth()*(float)(roll->sequence->getEventTime(i)/roll->seqLengthInPpq),
									(float)getHeight() - (float)(roll->sequence->getEventPointer(i)->message.getNoteNumber())*roll->yinc-roll->yinc,
									noteLength,
									roll->yinc);
							}
						}
					}
				}
			}
		}
	};

	Timeline* timeline;
	Playbar* playline;
	PianoRollBackground* bg;
	PianoRollNotes* noteLayer;
	AudioProcessor* plugin;
	AudioProcessorEditor* owner;
	Loop* sequence;
	PizMidiMessageSequence::mehPtr hoveringNote;
	int hoveringNoteIndex;
	double timebase;
	double stepLengthInPpq;
	double seqLengthInPpq;
	float seqLength;
	float gridSize; //quantization grid size in pixels
	float beatSize; //number of pixels per beat
	float barSize; //number of pixels per measure
	int numEvents;
	float xinc;
	float yinc;
	double lastDragTime;
	uint8 draggingNoteChannel;
	int draggingNoteNumber;
	int draggingNoteVelocity;
	int originalNoteVelocity;
	double draggingNoteLength;
	double draggingNoteStartTime;
	double draggingNoteEndOffset;

	int draggingNoteTransposition;
	double draggingNoteTimeDelta;

	bool wasResizing;
	bool snapToGrid;
	float noteLength;
	double playTime;
	double quarterNotesPerBar;
};

#endif
