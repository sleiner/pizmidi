#include "PianoRoll.h"

PianoRoll::PianoRoll(MidiStep *plugin_)
	: sequence(0),
	hoveringNote(-1),
	timebase(960.0),
	numEvents(0),
	snapToGrid(true)
{
	plugin = plugin_;
	blankLength=timebase*16.0;
	pixelsPerPpq = (float)(800.0/blankLength);
	setNoteLength(4);
	setSize(800,800);
}

PianoRoll::~PianoRoll()
{
	//deleteAndZero(keyboard);
}

void PianoRoll::setSequence(Loop *sequence_)
{
	sequence=sequence_;
	numEvents=sequence->getNumEvents();
	sequenceChanged();
}

int PianoRoll::getTimeInPixels ()
{
	return roundFloatToInt((float)sequence->getCurrentTime()*(float)getWidth()/seqLengthInPpq);
}

void PianoRoll::mouseDown (const MouseEvent& e)
{
	bool snap = snapToGrid != e.mods.isShiftDown();
	int n = (int)((float)(getHeight()-e.y)*128.f/(float)getHeight());
	double t = pixelsToPpq((float)e.x,true);
	double accurateTime = pixelsToPpq((float)e.x,false);
	hoveringNote = sequence->getIndexOfNote(n,accurateTime);
	if (hoveringNote!=-1) {
		draggingNoteNumber = n;
		draggingNoteVelocity = sequence->getEventPointer(hoveringNote)->message.getVelocity();
		draggingNoteLength = sequence->getTimeOfMatchingKeyUp(hoveringNote) - sequence->getEventTime(hoveringNote);
		draggingNoteStartTime = sequence->getEventTime(hoveringNote);
		draggingNoteChannel = sequence->getEventPointer(hoveringNote)->message.getChannel()-1;
		lastDragTime= snap ? t : accurateTime;
		hoveringNote-=9999;
	}
	else {
		//sequence->addEvent(MidiMessage(MIDI_NOTEON,n,0x7f,snap ? t : accurateTime));
		//sequence->addEvent(MidiMessage(MIDI_NOTEOFF,n,0,(snap?t:accurateTime) + stepLengthInPpq-1));
		//sequence->updateMatchedPairs();
		//hoveringNote = sequence->getIndexOfNote(n,accurateTime);
		hoveringNote = -2;
		draggingNoteNumber = n;
		draggingNoteVelocity = 127;
		draggingNoteLength = stepLengthInPpq-1;
		draggingNoteStartTime = snap ? t : accurateTime;
		draggingNoteChannel = jmax(0,floatToChannel(plugin->getParameter(kChannel+plugin->activeLoop))-1);
		lastDragTime = snap ? t : accurateTime;
	}
	repaint();
}

void PianoRoll::mouseDrag (const MouseEvent& e)
{
	bool snap = snapToGrid != e.mods.isShiftDown();
	double x = pixelsToPpq((float)e.x,snap);
	if (snap)
	{
		lastDragTime = snapPpqToGrid(lastDragTime);
	}
	int n = (int)((float)(getHeight()-e.y)*128.f/(float)getHeight());
	if(hoveringNote!=-1)
	{
		double startTime = draggingNoteStartTime;
		double length = draggingNoteLength;
		double offset = lastDragTime-startTime;
		if (!e.mods.isAltDown())
		{
			draggingNoteStartTime = x-offset;
			draggingNoteNumber = n;
		}
		else
		{
			draggingNoteLength = jmax(1.0, x-startTime+stepLengthInPpq);
		}
		lastDragTime=x;
		repaint();
	}
}

void PianoRoll::mouseUp (const MouseEvent& e)
{
	if (hoveringNote!=-1) {
		if (hoveringNote<-2) hoveringNote+=9999;
		if (e.mods.isPopupMenu() && hoveringNote!=-2)
		{
			sequence->deleteEvent(hoveringNote,true);
			sequence->updateMatchedPairs();
		}
		else
		{
			if (hoveringNote!=-2) {
				sequence->deleteEvent(hoveringNote,true);
				sequence->updateMatchedPairs();
			}
			sequence->addEvent(MidiMessage(MIDI_NOTEON | draggingNoteChannel, draggingNoteNumber, draggingNoteVelocity, draggingNoteStartTime));
			sequence->addEvent(MidiMessage(MIDI_NOTEOFF | draggingNoteChannel, draggingNoteNumber, draggingNoteVelocity, draggingNoteStartTime+draggingNoteLength));
			sequence->updateMatchedPairs();
		}
		hoveringNote=-1;
		repaint();
	}
}

void PianoRoll::mouseMove (const MouseEvent& e)
{

}

void PianoRoll::mouseDoubleClick (const MouseEvent& e)
{

}

void PianoRoll::paint (Graphics& g)
{
	float x = gridSize;
	int n=0;
	int b=1;
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

	while (x<getWidth()) {
		if (b%bar==0) g.setColour(Colours::black);
		else if (b%beat==0) g.setColour(Colours::grey);
		else g.setColour(Colours::lightgrey);
		g.drawLine(x,0.f,x,(float)getHeight());
		x+=gridSize;
		b++;
	}
	if (sequence) {
		if (sequence->isRecording) {
			g.setColour(Colours::brown);
			g.drawVerticalLine((int)((float)sequence->recTime*(float)getWidth()/seqLengthInPpq),0.f,(float)getHeight());
		}
		for(int i=0;i<sequence->getNumEvents();i++)
		{
			if(sequence->getEventPointer(i)->message.isNoteOn() && (i-9999!=hoveringNote))
			{
				float noteLength = (float)jmax(1.0,(sequence->getEventTime(sequence->getIndexOfMatchingKeyUp(i))-sequence->getEventTime(i)))*(float)getWidth()/seqLengthInPpq;
				if (i==sequence->indexOfLastNoteOn
					|| abs(sequence->getEventTime(i)-sequence->getEventTime(sequence->indexOfLastNoteOn))<sequence->chordTolerance)
					g.setColour(Colours::blue);
				else g.setColour(Colours::darkgoldenrod.withAlpha(sequence->getEventPointer(i)->message.getFloatVelocity()));
				g.fillRect( (float)getWidth()*(float)sequence->getEventTime(i)/seqLengthInPpq,
							(float)getHeight() - (float)(sequence->getEventPointer(i)->message.getNoteNumber())*yinc-yinc,
							noteLength,
							yinc);
				if (i==hoveringNote) g.setColour(Colours::red);
				else g.setColour(Colours::black);
				g.drawRect( (float)getWidth()*(float)sequence->getEventTime(i)/seqLengthInPpq,
							(float)getHeight() - (float)(sequence->getEventPointer(i)->message.getNoteNumber())*yinc-yinc,
							noteLength,
							yinc);
			}
		}
		if (hoveringNote<-1) {
			float noteLength = ppqToPixels(draggingNoteLength);
			g.setColour(Colours::darkgoldenrod.withAlpha(draggingNoteVelocity*midiScaler));
			g.fillRect( (float)getWidth()*(float)draggingNoteStartTime/seqLengthInPpq,
						(float)getHeight() - (float)(draggingNoteNumber)*yinc-yinc,
						noteLength,
						yinc);
			g.setColour(Colours::red);
			g.drawRect( (float)getWidth()*(float)draggingNoteStartTime/seqLengthInPpq,
						(float)getHeight() - (float)draggingNoteNumber*yinc-yinc,
						noteLength,
						yinc);
		}
	}

}

void PianoRoll::resized ()
{
	xinc = (float)getWidth()/(seqLength-1);
	yinc = (float)getHeight()/128.f;
	gridSize = (float)getWidth()/seqLength;
	//keyboard->setBounds(0,0,20,getHeight());
}

void PianoRoll::sequenceChanged()
{
	//int extraLength = roundDoubleToInt(sequence->getEndTime() - pixelsToPpq((float)getWidth(),false));
	//if (extraLength) setSize(getWidth()+extraLength,getHeight());
	seqLengthInPpq = (float)jmax(blankLength,sequence->getEndTime());
	seqLength = seqLengthInPpq/(float)stepLengthInPpq;
	gridSize = (float)getWidth()/seqLength;
	xinc = (float)getWidth()/(seqLength-1);
	numEvents=sequence->getNumEvents();
	repaint();
}

void PianoRoll::setNoteLength(int beatDiv)
{
	beat = beatDiv;
	bar = beat * 4;//plugin->lastPosInfo.timeSigNumerator;
	noteLength = 1.f/(float)beatDiv;
	stepLengthInPpq = timebase*noteLength;
	seqLength = seqLengthInPpq/(float)stepLengthInPpq;
	gridSize = (float)getWidth()/seqLength;
	xinc = (float)getWidth()/(seqLength-1);
}

double PianoRoll::pixelsToPpq(float pixels, bool snap)
{
	if (snap)
		return stepLengthInPpq*floor((double)pixels*seqLength/(double)getWidth());
	else
		return stepLengthInPpq*(double)pixels*seqLength/(double)getWidth();
}

float PianoRoll::ppqToPixels(double ppq)
{
	return (float)getWidth() * ((float)ppq/seqLengthInPpq);
	//return ppq*pixelsPerPpq;
}

double PianoRoll::snapPpqToGrid(double ppq)
{
	return stepLengthInPpq*floor(ppq/stepLengthInPpq);
}

float PianoRoll::getNoteHeight()
{
	return (float)getHeight()/128.f;
}
