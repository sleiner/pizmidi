
#include "PianoRoll.h"

#include <utility>

using juce::jlimit;
using juce::jmax;
using juce::jmin;
using juce::roundToInt;

PianoRoll::PianoRoll(juce::AudioProcessor* _plugin, juce::AudioProcessorEditor* _owner, Timeline* _timeline)
    : sequence(nullptr),
      hoveringNoteIndex(-1),
      hoveringNote(nullptr),
      timebase(960.0),
      snapToGrid(true),
      defaultChannel(0),
      playTime(0.0),
      timeSigN(0),
      timeSigD(0),
      beatSize(0),
      barSize(0),
      gridSize(0),
      timeline(nullptr),
      draggingNoteTransposition(0),
      draggingNoteTimeDelta(0)

{
    plugin      = _plugin;
    owner       = _owner;
    timeline    = _timeline;
    blankLength = timebase * 16.0;

    setNoteLength(4.f);
    setTimeSig(4, 4);
    addAndMakeVisible(bg = new PianoRollBackground());
    bg->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(noteLayer = new PianoRollNotes());
    noteLayer->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(playBar = new PlayBar(this));
    playBar->setInterceptsMouseClicks(false, false);
    setSize(800, 800);
    lasso.setBounds(0, 0, 0, 0);
    wasResizing = false;
}

PianoRoll::~PianoRoll()
{
    dispatchPendingMessages();
    deleteAndZero(playBar);
    deleteAndZero(bg);
    deleteAndZero(noteLayer);
}

void PianoRoll::setSequence(Loop* sequence_)
{
    sequence = sequence_;
    sequence->getNumEvents();
    sequenceChanged();
}

void PianoRoll::mouseDown(const juce::MouseEvent& e)
{
    bool snap           = snapToGrid != e.mods.isShiftDown();
    int n               = (int) ((float) (getHeight() - e.y) * 128.f / (float) getHeight());
    double t            = pixelsToPpq((float) e.x, true);
    double accurateTime = pixelsToPpq((float) e.x, false);
    hoveringNoteIndex   = sequence->getIndexOfNote(n, accurateTime);
    if (hoveringNoteIndex != No_Note)
    {
        hoveringNote = sequence->getEventPointer(hoveringNoteIndex);
        //existing note
        draggingNoteTimeDelta     = 0.0;
        draggingNoteTransposition = 0;
        draggingNoteNumber        = n;
        originalNoteVelocity      = hoveringNote->message.getVelocity();
        draggingNoteVelocity      = originalNoteVelocity;
        draggingNoteLength        = sequence->getTimeOfMatchingKeyUp(hoveringNoteIndex) - sequence->getEventTime(hoveringNoteIndex);
        draggingNoteStartTime     = sequence->getEventTime(hoveringNoteIndex);
        draggingNoteChannel       = hoveringNote->message.getChannel() - 1;
        draggingNoteEndOffset     = (draggingNoteStartTime + draggingNoteLength) - accurateTime;
        lastDragTime              = snap ? t : accurateTime;
        if (! selectedNotes.contains(hoveringNote))
        {
            clearSelection();
            addToSelection(hoveringNote);
            jassert(hoveringNote->noteOffObject != nullptr);
            DBG("n=" + juce::String(n) + "v=" + juce::String(draggingNoteVelocity));
        }
        hoveringNoteIndex -= Dragging_A_Note;
    }
    else if (e.mods.isAltDown())
    {
        //make a new note
        wasResizing = true;
        clearSelection();
        hoveringNoteIndex    = New_Note;
        draggingNoteNumber   = n;
        draggingNoteVelocity = originalNoteVelocity = 127;
        draggingNoteLength                          = stepLengthInPpq - 1;
        draggingNoteStartTime                       = snap ? t : accurateTime;
        draggingNoteChannel                         = defaultChannel;
        draggingNoteTimeDelta                       = 0.0;
        draggingNoteTransposition                   = 0;
        lastDragTime                                = snap ? t : accurateTime;
        if (sequence->getNumEvents() == 0 && timeline->getLength() == 0)
        {
            timeline->setLoop(0, quarterNotesPerBar * ceil((lastDragTime + 1) / (timebase * quarterNotesPerBar)));
        }
        plugin->getCallbackLock().enter();
        sequence->addNote(juce::MidiMessage(MIDI_NOTEON | draggingNoteChannel, draggingNoteNumber, draggingNoteVelocity, draggingNoteStartTime),
                          juce::MidiMessage(MIDI_NOTEOFF | draggingNoteChannel, draggingNoteNumber, 0, draggingNoteStartTime + draggingNoteLength));
        plugin->getCallbackLock().exit();
        sequence->updateMatchedPairs();
        hoveringNote = sequence->getEventPointer(sequence->getIndexOfNote(draggingNoteNumber, draggingNoteStartTime, true));
        addToSelection(hoveringNote);
        sendChangeMessage();
    }
    else
    {
        clearSelection();
    }
    noteLayer->repaint();
}

void PianoRoll::mouseDrag(const juce::MouseEvent& e)
{
    bool snap = snapToGrid != e.mods.isShiftDown();
    double x  = pixelsToPpq((float) e.x, false);
    if (snap)
    {
        lastDragTime = snapPpqToGrid(lastDragTime);
    }
    int n = (int) ((float) (getHeight() - e.y) * 128.f / (float) getHeight());
    if (hoveringNoteIndex != No_Note)
    {
        double startTime = draggingNoteStartTime;
        double length    = draggingNoteLength;
        double offset    = lastDragTime - startTime;
        if (! e.mods.isAltDown())
        {
            int noteDelta    = n - draggingNoteNumber;
            double timeDelta = jmax(0.0, snap ? snapPpqToGrid(x - offset) : x - offset) - draggingNoteStartTime;
            draggingNoteNumber += noteDelta;
            draggingNoteStartTime += timeDelta;
            draggingNoteTransposition = n - hoveringNote->message.getNoteNumber();
            draggingNoteTimeDelta     = jmax(0.0, snap ? snapPpqToGrid(x - offset) : x - offset) - hoveringNote->message.getTimeStamp();
        }
        else
        {
            wasResizing        = true;
            length             = x - startTime + draggingNoteEndOffset;
            double lengthDelta = snap ? snapPpqToGrid(length - draggingNoteLength, true) : length - draggingNoteLength;
            int velocityDelta  = jlimit(1, 127, originalNoteVelocity + (getHeight() - e.y) - draggingNoteNumber * getHeight() / 128) - draggingNoteVelocity;
            for (int i = 0; i < selectedNotes.size(); i++)
            {
                if (selectedNotes.getUnchecked(i)->noteOffObject)
                {
                    //set length and velocity when Alt is down
                    double minWidth = snap ? stepLengthInPpq : 1.0;
                    //if (lengthDelta!=0)
                    selectedNotes.getUnchecked(i)->noteOffObject->message.setTimeStamp(jmax(selectedNotes.getUnchecked(i)->message.getTimeStamp() + minWidth, selectedNotes.getUnchecked(i)->message.getTimeStamp() + selectedNoteLengths.getUnchecked(i).length + lengthDelta));
                    selectedNotes.getUnchecked(i)->message.setVelocity((jlimit(1, 127, selectedNotes.getUnchecked(i)->message.getVelocity() + velocityDelta)) * midiScaler);
                }
            }
            draggingNoteVelocity += velocityDelta;
        }
        lastDragTime = x;
        noteLayer->repaint();
    }
    else if (e.mods.isLeftButtonDown())
    {
        lasso.setBounds(jmin(e.x, e.getMouseDownX()),
                        jmin(e.y, e.getMouseDownY()),
                        abs(e.getDistanceFromDragStartX()),
                        abs(e.getDistanceFromDragStartY()));
        repaint();
    }
}

void PianoRoll::mouseUp(const juce::MouseEvent& e)
{
    int highnote, lownote;
    if (lasso.getWidth() > 0)
    {
        lownote  = (int) ((float) (getHeight() - lasso.getY()) * 128.f / (float) getHeight());
        highnote = (int) ((float) (getHeight() - (lasso.getY() + lasso.getHeight())) * 128.f / (float) getHeight());
        if (lownote > highnote)
        {
            std::swap(lownote, highnote);
        }
        for (int index = 0; index < (sequence->getNumEvents()); index++)
        {
            juce::MidiMessage m = sequence->getEventPointer(index)->message;
            DBG("eventtime=" + juce::String(sequence->getEventTime(index)));
            if (m.isNoteOn())
            {
                DBG("note=" + juce::String(m.getNoteNumber()));
            }
            DBG("lassostart=" + juce::String(pixelsToPpq((float) lasso.getX(), false)));
            DBG("lassoend=" + juce::String(pixelsToPpq((float) (lasso.getX() + lasso.getWidth()), false)));
            if (m.isNoteOn()
                && sequence->getEventTime(index) >= pixelsToPpq((float) lasso.getX(), false)
                && sequence->getEventTime(index) <= pixelsToPpq((float) (lasso.getX() + lasso.getWidth()), false)
                && m.getNoteNumber() >= lownote
                && m.getNoteNumber() <= highnote)
            {
                addToSelection(sequence->getEventPointer(index));
            }
        }
        lasso.setSize(0, 0);
        repaint();
        noteLayer->repaint();
    }
    if (hoveringNoteIndex != No_Note)
    {
        if (hoveringNoteIndex < -2)
        {
            hoveringNoteIndex += 9999;
        }
        if (e.mods.isPopupMenu() && hoveringNoteIndex != -2)
        {
            //right click, delete notes
            plugin->getCallbackLock().enter();
            for (int i = selectedNotes.size(); --i >= 0;)
            {
                sequence->deleteEvent(sequence->getIndexOf(selectedNotes.getUnchecked(i)), true);
            }
            sequence->updateMatchedPairs();
            plugin->getCallbackLock().exit();
            clearSelection();
        }
        else
        {
            if (draggingNoteTimeDelta != 0 || draggingNoteTransposition != 0)
            {
                plugin->getCallbackLock().enter();
                if (draggingNoteTimeDelta != 0.0)
                {
                    for (int i = 0; i < selectedNotes.size(); i++)
                    {
                        selectedNotes.getUnchecked(i)->message.addToTimeStamp(draggingNoteTimeDelta);
                        selectedNotes.getUnchecked(i)->noteOffObject->message.addToTimeStamp(draggingNoteTimeDelta);
                        //sequence->moveEvent(sequence->getIndexOf(selectedNotes.getUnchecked(i)),draggingNoteTimeDelta,true);
                    }
                    draggingNoteTimeDelta = 0.0;
                }
                if (draggingNoteTransposition != 0)
                {
                    for (int i = 0; i < selectedNotes.size(); i++)
                    {
                        sequence->transposeEvent(sequence->getIndexOf(selectedNotes.getUnchecked(i)), draggingNoteTransposition);
                    }
                    draggingNoteTransposition = 0;
                }
                sequence->updateMatchedPairs(true);
                plugin->getCallbackLock().exit();
            }
            else if (wasResizing || e.mods.isAltDown())
            {
                //resize notes
                wasResizing = false;
                for (int i = selectedNoteLengths.size(); --i >= 0;)
                {
                    if (selectedNotes.getUnchecked(i) != nullptr && selectedNotes.getUnchecked(i)->noteOffObject != nullptr)
                    {
                        selectedNoteLengths.getReference(i).updateLength();
                    }
                    else
                    {
                        selectedNotes.remove(i);
                        selectedNoteLengths.remove(i);
                    }
                }
                plugin->getCallbackLock().enter();
                sequence->updateMatchedPairs(true);
                plugin->getCallbackLock().exit();
            }
        }
        hoveringNoteIndex = No_Note;
        hoveringNote      = nullptr;
        sendChangeMessage();
        noteLayer->repaint();
    }
}

void PianoRoll::mouseDoubleClick(const juce::MouseEvent& e)
{
    if (e.mods.isLeftButtonDown())
    {
        bool snap                 = snapToGrid != e.mods.isShiftDown();
        int n                     = (int) ((float) (getHeight() - e.y) * 128.f / (float) getHeight());
        double t                  = pixelsToPpq((float) e.x, true);
        double accurateTime       = pixelsToPpq((float) e.x, false);
        hoveringNoteIndex         = sequence->getIndexOfNote(n, accurateTime);
        draggingNoteTimeDelta     = 0.0;
        draggingNoteTransposition = 0;
        draggingNoteNumber        = n;
        draggingNoteVelocity      = 127;
        originalNoteVelocity      = 127;
        draggingNoteLength        = stepLengthInPpq - 1;
        draggingNoteStartTime     = snap ? t : accurateTime;
        draggingNoteChannel       = defaultChannel;
        lastDragTime              = snap ? t : accurateTime;
        if (hoveringNoteIndex == No_Note)
        {
            if (sequence->getNumEvents() == 0 && timeline->getLength() == 0)
            {
                timeline->setLoop(0, quarterNotesPerBar * ceil((lastDragTime + 1) / (timebase * quarterNotesPerBar)));
            }
            sequence->addNote(juce::MidiMessage(MIDI_NOTEON | draggingNoteChannel, draggingNoteNumber, draggingNoteVelocity, draggingNoteStartTime),
                              juce::MidiMessage(MIDI_NOTEOFF | draggingNoteChannel, draggingNoteNumber, 0, draggingNoteStartTime + draggingNoteLength));
            //sequence->updateMatchedPairs();
            addToSelection(sequence->getEventPointer(sequence->getIndexOfNote(n, draggingNoteStartTime, true)));
            sendChangeMessage();
            noteLayer->repaint();
        }
    }
}

void PianoRoll::paintOverChildren(juce::Graphics& g)
{
    g.fillAll(juce::Colours::transparentBlack);
    if (lasso.getWidth() > 0)
    {
        g.setColour(juce::Colours::green.withAlpha(0.3f));
        g.fillRect(lasso);
        g.setColour(juce::Colours::green.withAlpha(0.5f));
        g.drawRect(lasso.reduced(1.0f));
    }
}

void PianoRoll::resized()
{
    timeline->setSize(getWidth(), timeline->getHeight());
    bg->setBounds(0, 0, getWidth(), getHeight());
    noteLayer->setBounds(0, 0, getWidth(), getHeight());
    gridSize = (float) noteLayer->getWidth() / seqLength;
    beatSize = ppqToPixels(timebase * 4.0 / (double) timeSigD);
    barSize  = ppqToPixels(timebase * quarterNotesPerBar);
    yinc     = (float) getHeight() / 128.f;
}

void PianoRoll::sequenceChanged()
{
    //int extraLength = roundToInt(sequence->getEndTime() - pixelsToPpq((float)getWidth(),false));
    //if (extraLength) setSize(getWidth()+extraLength,getHeight());
    seqLengthInPpq     = jmax(blankLength, sequence->getLength());
    seqLength          = (float) (seqLengthInPpq / stepLengthInPpq);
    gridSize           = (float) noteLayer->getWidth() / seqLength;
    beatSize           = ppqToPixels(timebase * 4.0 / (double) timeSigD);
    quarterNotesPerBar = (double) (timeSigN * 4) / (double) timeSigD;
    barSize            = ppqToPixels(timebase * quarterNotesPerBar);

    bg->repaint();
    noteLayer->repaint();
    timeline->repaint();
}

void PianoRoll::setTimeSig(int n, int d)
{
    timeSigN           = n;
    timeSigD           = d;
    quarterNotesPerBar = (double) (n * 4) / (double) d;
    beatSize           = ppqToPixels(timebase * 4.0 / (double) d);
    barSize            = ppqToPixels(timebase * quarterNotesPerBar);
}

void PianoRoll::setNoteLength(float newBeatDiv)
{
    if (noteLength != 1.f / newBeatDiv)
    {
        noteLength      = 1.f / newBeatDiv;
        stepLengthInPpq = timebase * noteLength;
        seqLength       = (float) (seqLengthInPpq / stepLengthInPpq);
        gridSize        = (float) getWidth() / seqLength;
    }
}

double PianoRoll::pixelsToPpq(float pixels, bool snap, bool round)
{
    if (snap)
    {
        if (round)
        {
            return stepLengthInPpq * (double) roundToInt(((double) pixels * seqLength / (double) getWidth()));
        }
        return stepLengthInPpq * floor((double) pixels * seqLength / (double) getWidth());
    }
    //snap to last tick (1/timebase of a quarter note)
    return floor((double) pixels * seqLengthInPpq / (double) getWidth());
}

float PianoRoll::ppqToPixels(double ppq)
{
    return (float) ((double) getWidth() * (ppq / seqLengthInPpq));
}

int PianoRoll::ppqToPixelsWithOffset(double ppq)
{
    return (int) ((double) getWidth() * (ppq / seqLengthInPpq)) - timeline->scrollOffset;
}

double PianoRoll::snapPpqToGrid(double ppq, bool round) const
{
    if (round)
    {
        return stepLengthInPpq * (double) roundToInt((ppq / stepLengthInPpq));
    }
    return stepLengthInPpq * floor(ppq / stepLengthInPpq);
}

void PianoRoll::setDisplayLength(double ppq)
{
    blankLength = ppq;
    sequenceChanged();
}

void PianoRoll::setDisplayLength(int bars)
{
    int pixelBarLength = (int) ppqToPixels(getPpqPerBar());
    blankLength        = getPpqPerBar() * bars;
    sequenceChanged();
    setSize((int) ppqToPixels(juce::jmax(blankLength, seqLengthInPpq)), getHeight());
}

void PianoRoll::addBar()
{
    int pixelBarLength = (int) ppqToPixels(getPpqPerBar());
    setDisplayLength(seqLengthInPpq + getPpqPerBar());
    setSize(getWidth() + pixelBarLength, getHeight());
}

void PianoRoll::removeBar()
{
    int pixelBarLength = (int) ppqToPixels(getPpqPerBar());
    setDisplayLength(juce::jmax(getPpqPerBar(), seqLengthInPpq - getPpqPerBar()));
    setSize(juce::jmax(pixelBarLength, getWidth() - pixelBarLength), getHeight());
}

int PianoRoll::getDisplayLength() const
{
    return (int) (juce::jmax(blankLength, seqLengthInPpq) / getPpqPerBar());
}

bool PianoRoll::getSnap() const
{
    return snapToGrid;
}

void PianoRoll::setSnap(bool snap)
{
    snapToGrid = snap;
}

void PianoRoll::setPlayTime(double timeInPpq)
{
    const int lastPixels = ppqToPixelsWithOffset(playTime);
    const int pixels     = ppqToPixelsWithOffset(timeInPpq);
    if (pixels != lastPixels)
    {
        playTime = timeInPpq;
        playBar->repaint(lastPixels, 0, 1, getHeight());
        playBar->repaint(pixels, 0, 1, getHeight());
    }
}

double PianoRoll::getPpqPerBar() const
{
    return timebase * quarterNotesPerBar;
}

juce::Component* PianoRoll::getPlayBar()
{
    return (Component*) playBar;
}

void PianoRoll::repaintBG()
{
    bg->repaint();
}

void PianoRoll::addToSelection(PizMidiMessageSequence::mehPtr note)
{
    if (note->message.isNoteOn())
    {
        if (note->noteOffObject == nullptr)
        {
            sequence->updateMatchedPairs();
        }
        selectedNotes.addIfNotAlreadyThere(note);
        selectedNoteLengths.add(PizNote(note));
    }
}

void PianoRoll::clearSelection()
{
    selectedNotes.clear();
    selectedNoteLengths.clear();
}
