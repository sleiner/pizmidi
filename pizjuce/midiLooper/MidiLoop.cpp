#include "MidiLoop.h"

Loop::Loop() 
: PizMidiMessageSequence(), 
currentIndex(0), 
triggerNote(60), 
transpose(0), 
octave(0),
playMode(0),
isRecording(false),
isRecArmed(false),
recChannel(0),
outChannel(0),
recTime(0),
indexOfLastNoteOn(-1),
velocitySensitivity(1.f),
chordTolerance(120.0),
length(0)
{
	resetNotes();
}

Loop::Loop(PizMidiMessageSequence sequence, int triggerNote_, int transpose_, bool playMode_) 
: PizMidiMessageSequence(sequence), 
currentIndex(0), 
triggerNote(triggerNote_), 
transpose(transpose_),
playMode(playMode_),
isRecording(false),
isRecArmed(false),
recChannel(0),
outChannel(0),
recTime(0),
indexOfLastNoteOn(-1),
velocitySensitivity(1.f),
chordTolerance(120.0)
{
	resetNotes();
}

Loop::~Loop() 
{
	;
}

void Loop::startRecording() 
{
	clear();
	recTime = 0;
	currentIndex=0;
	isRecording = true;
}

bool Loop::findNextNote()
{
	if (currentIndex>=getNumEvents()) 
		currentIndex=0;
	if (this->getEventPointer(currentIndex)->message.isNoteOn()) 
	{
		return true;
	}		
	++currentIndex;
	while (currentIndex<getNumEvents())
	{
		if (this->getEventPointer(currentIndex)->message.isNoteOn()) 
		{
			return true;
		}
		++currentIndex;
	}
	currentIndex=0;
	while (currentIndex<getNumEvents())
	{
		if (this->getEventPointer(currentIndex)->message.isNoteOn()) 
		{
			return true;
		}
		++currentIndex;
	}
	return false;
}

void Loop::playAllNotesAtCurrentTime(MidiBuffer& buffer, int sample_number, int velocity)
{
	if (findNextNote())
	{
		sendCurrentNoteToBuffer(buffer,sample_number, velocity);

		const double time = getCurrentTime();
		++currentIndex;
		while (abs(getCurrentTime()-time)<chordTolerance)
		{
			if (this->getEventPointer(currentIndex)->message.isNoteOn())
				sendCurrentNoteToBuffer(buffer,sample_number, velocity);
			++currentIndex;
		}
	}
}

void Loop::sendCurrentNoteToBuffer(MidiBuffer& buffer, int sample_number, int velocity)
{
	MidiMessage m = this->getEventPointer(currentIndex)->message;
	//playingNote[m.getNoteNumber()][m.getChannel()-1][0] = jlimit(0,127,m.getNoteNumber()+getTransposition());
	m.setNoteNumber(jlimit(0,127,m.getNoteNumber()+getTransposition()));
	m.setVelocity((((float)velocity*midiScaler * velocitySensitivity) + (1.f-velocitySensitivity)));
	if (outChannel>0) m.setChannel(outChannel);
	buffer.addEvent(m,sample_number);
	indexOfLastNoteOn=currentIndex;
}

bool Loop::isNotePlaying(PizMidiMessageSequence::mehPtr note, int p)
{
	for (int i=0;i<playingNotes[p].size();i++) {
		if (playingNotes[p].getReference(i).note==note
			|| playingNotes[p].getReference(i).note->noteOffObject==note) 
			return true;
	}
	return false;
}

bool Loop::isNotePlaying(PizMidiMessageSequence::mehPtr note)
{
	for (int p=0;p<polyphony;p++) {
		for (int i=0;i<playingNotes[p].size();i++) {
			if (playingNotes[p].getReference(i).voice==p && 
				(playingNotes[p].getReference(i).note==note			
				|| playingNotes[p].getReference(i).note->noteOffObject==note))
				return true;
		}
	}
	return false;
}

bool Loop::isTriggerNote(int note)
{
	return note==triggerNote;
}

void Loop::setTriggerNote(int note)
{
	triggerNote=note;
}

void Loop::resetNotes()
{
	for (int v=0;v<4;v++) {
		playingNotes[v].clear();
	}
	for (int n=0;n<128;n++) {
		for (int ch=0;ch<16;ch++) {
			//for (int v=0;v<4;v++) {
			//	playingNote[n][ch][v]=NOT_PLAYING;
			//	channelOfNote[n][ch][v]=ch+1;
			//}
			Qdelta[n][ch]=0;
		}
	}	
}

void Loop::sendNoteOffMessagesToBuffer(MidiBuffer& buffer, int sample_number)
{
	for (int ch=0;ch<16;ch++)
	{
		for (int i=0;i<128;i++)
		{
			//if (isNotePlaying(i,ch,0))
			//{
			//	int channel = outChannel>0 ? outChannel-1 : ch;
			//	buffer.addEvent(MidiMessage(MIDI_NOTEOFF+channel,playingNote[i][ch][0],0),sample_number);
			//	playingNote[i][ch][0]=NOT_PLAYING;
			//}
		}
	}
}

MidiMessage Loop::getCurrentMessage() 
{
	if (currentIndex>=this->getNumEvents()) currentIndex=0;
	return this->getEventPointer(currentIndex)->message;
}

double Loop::getCurrentTime()
{
	return this->getEventTime(currentIndex);
}

void Loop::cleanZeroLengthNotes()
{
	Array<int> matchedNoteOffs;
	for (int i=0;i<getNumEvents();i++)
	{
		if (getEventPointer(i)->message.isNoteOn())
		{
			if (getEventTime(i) == getTimeOfMatchingKeyUp(i) || getTimeOfMatchingKeyUp(i)==0.0)
			{
				//zero length note
				deleteEvent(i,(getTimeOfMatchingKeyUp(i)!=0.0));
				updateMatchedPairs();
			}
		}
	}
	for (int i=0;i<getNumEvents();i++)
	{
		if (getEventPointer(i)->message.isNoteOn() && getIndexOfMatchingKeyUp(i)!=-1)
			matchedNoteOffs.add(getIndexOfMatchingKeyUp(i));
	}
	for (int i=0;i<getNumEvents();i++)
	{
		if (getEventPointer(i)->message.isNoteOff() && !matchedNoteOffs.contains(i)) {
			//unmatched note-off
			deleteEvent(i,false);
			updateMatchedPairs();
			break;
		}	
	}
	//updateMatchedPairs();
}

int Loop::getIndexOfNote(int noteNumber, double time, bool exact) 
{
	if (!exact) {
		for (int i=0;i<getNumEvents();i++)
		{
			if (getEventPointer(i)->message.isNoteOn()
				&& getEventPointer(i)->message.getNoteNumber()==noteNumber
				&& (getEventTime(i)<=time && getTimeOfMatchingKeyUp(i)>time))
			{
				return i;
			}
		}
	}
	else {
		for (int i=0;i<getNumEvents();i++)
		{
			if (getEventPointer(i)->message.isNoteOn()
				&& getEventPointer(i)->message.getNoteNumber()==noteNumber
				&& fabs(getEventTime(i)-time)<1.0)
			{
				return i;
			}
		}
	}
	return No_Note;
}

void Loop::convertTimeBase(short timeBase)
{
	if (timeBase>0 && timeBase!=960)
	{
		double factor = 960.0/(double)timeBase;
		for (int i=0;i<getNumEvents();i++)
		{
			getEventPointer(i)->message.setTimeStamp(getEventPointer(i)->message.getTimeStamp()*factor);
		}
	}
	else {
		/* SMTPE
		If it's negative, the upper byte indicates the frames-per-second (but negative), and
		the lower byte is the number of ticks per frame - see setSmpteTimeFormat().	

		@param framesPerSecond      must be 24, 25, 29 or 30
		@param subframeResolution   the sub-second resolution, e.g. 4 (midi time code),
		8, 10, 80 (SMPTE bit resolution), or 100. For millisecond
		timing, setSmpteTimeFormat (25, 40) 
		*/
		int framesPerSecond = -(timeBase / 0x100);
		int subframeResolution = (timeBase & 0x00ff);
	}
}

int Loop::getTransposition() { return transpose + octave*12; }

void Loop::setSemitones(int semitones) {transpose = semitones; }

void Loop::setOctaves(int octaves) { octave = octaves; }

int Loop::getTranspositionOfNote(PizMidiMessageSequence::mehPtr note, int p) { 
	for (int i=0;i<playingNotes[p].size();i++)
		if (playingNotes[p].getReference(i).note==note			
			|| playingNotes[p].getReference(i).note->noteOffObject==note) 
			return playingNotes[p].getReference(i).lastOutputNoteNumber;
	return NOT_PLAYING;
}

int Loop::getChannelOfNote(PizMidiMessageSequence::mehPtr note, int p) { 
	for (int i=0;i<playingNotes[p].size();i++)
		if (playingNotes[p].getReference(i).note==note			
			|| playingNotes[p].getReference(i).note->noteOffObject==note) 
			return playingNotes[p].getReference(i).lastOutputChannel;
	return NOT_PLAYING;
}

void Loop::setNoteOff(PizMidiMessageSequence::mehPtr note, int p) { 
	for (int i=0;i<playingNotes[p].size();i++)
		if (playingNotes[p].getReference(i).note==note			
			|| playingNotes[p].getReference(i).note->noteOffObject==note) 
			playingNotes[p].remove(i);
}

void Loop::setNoteOff(PizMidiMessageSequence::mehPtr note) { 
	for (int p=0;p<polyphony;p++) {
		for (int i=0;i<playingNotes[p].size();i++)
			if (playingNotes[p].getReference(i).voice==p && 
				(playingNotes[p].getReference(i).note==note			
				|| playingNotes[p].getReference(i).note->noteOffObject==note))
				playingNotes[p].remove(i);
	}
}

void Loop::sentNoteAs(PizMidiMessageSequence::mehPtr note, int p, int sentNoteNumber, int sentNoteChannel, int offSample) { 
	jassert(sentNoteChannel>0);
	for (int i=0;i<playingNotes[p].size();i++) {
		if (playingNotes[p].getReference(i).note==note
			|| playingNotes[p].getReference(i).note->noteOffObject==note) 
			playingNotes[p].remove(i);
	}
	playingNotes[p].add(LoopNote(note,sentNoteNumber,sentNoteChannel,offSample,p));
}

//void Loop::setNoteChannel(int n, int ch, int p, int channel) { 
//	channelOfNote[n][ch][p] = channel; 
//}
