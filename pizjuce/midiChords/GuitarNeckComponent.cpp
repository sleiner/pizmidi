#include "GuitarNeckComponent.h"

using juce::jlimit;
using juce::jmax;
using juce::jmin;
using juce::roundToInt;

//==============================================================================
GuitarNeckComponent::GuitarNeckComponent(juce::MidiKeyboardState& state_)
    : state(state_),
      xOffset(0),
      midiChannel(1),
      midiInChannelMask(0xffff),
      velocity(1.0f),
      numStrings(6),
      numFrets(24),
      dotSize(13.f),
      rangeStart(0),
      rangeEnd(24),
      firstFret(0),
      mouseDragging(false),
      octaveNumForMiddleC(3)
{
    setNumFrets(24);
    setNumStrings(6);

    for (int i = 0; i < maxStrings; i++)
    {
        stringNote[i]           = -1;
        currentlyFrettedFret[i] = -1;
    }
    stringNote[0] = 64;
    stringNote[1] = 59;
    stringNote[2] = 55;
    stringNote[3] = 50;
    stringNote[4] = 45;
    stringNote[5] = 40;

    setOpaque(true);
    setWantsKeyboardFocus(false);
    state.addListener(this);
}

GuitarNeckComponent::~GuitarNeckComponent()
{
    state.removeListener(this);
    jassert(! mouseDownNote.isValid() && keysPressed.countNumberOfSetBits() == 0); // leaving stuck notes!
}

static const float fretPosition[] = { 0.f, 0.056126f, 0.109101f, 0.159104f, 0.206299f, 0.250846f, 0.292893f, 0.332580f, 0.370039f, 0.405396f, 0.438769f, 0.470268f, 0.500000f, 0.528063f, 0.554551f, 0.579552f, 0.603150f, 0.625423f, 0.646447f, 0.666290f, 0.685020f, 0.702698f, 0.719384f, 0.735134f, 0.750000f, 0.764031f, 0.777275f, 0.789776f, 0.801575f, 0.812712f, 0.823223f, 0.833145f, 0.842510f };

void GuitarNeckComponent::setNumStrings(int n)
{
    numStrings = jmin(n, maxStrings);
    dotSize    = jmax(15.f, jmin(25.f, (float) getFretWidth(numFrets), (float) getHeight() / (float) numStrings));
    repaint();
}
void GuitarNeckComponent::setNumFrets(int n)
{
    numFrets = jmin(n, maxFrets);
    rangeEnd = n;
    dotSize  = jmax(15.f, jmin(25.f, (float) getFretWidth(numFrets), (float) getHeight() / (float) numStrings));
    repaint();
}

void GuitarNeckComponent::setLowestVisibleFret(int fretNumber)
{
    fretNumber = jlimit(rangeStart, rangeEnd, fretNumber);

    if (fretNumber != firstFret)
    {
        firstFret = fretNumber;
        sendChangeMessage();
        resized();
    }
}

void GuitarNeckComponent::colourChanged()
{
    repaint();
}

//==============================================================================
void GuitarNeckComponent::setMidiChannel(const int midiChannelNumber)
{
    jassert(midiChannelNumber > 0 && midiChannelNumber <= 16);

    if (midiChannel != midiChannelNumber)
    {
        resetAnyKeysInUse();
        midiChannel = jlimit(1, 16, midiChannelNumber);
    }
}

void GuitarNeckComponent::setMidiChannelsToDisplay(const int midiChannelMask)
{
    midiInChannelMask = midiChannelMask;
    triggerAsyncUpdate();
}

void GuitarNeckComponent::setVelocity(const float velocity_)
{
    velocity = jlimit(0.0f, 1.0f, velocity_);
}

int GuitarNeckComponent::getStringFret(int string)
{
    return currentlyFrettedFret[string];
}

FrettedNote GuitarNeckComponent::xyToNote(const juce::Point<int>& pos, float& mousePositionVelocity)
{
    FrettedNote fretString;

    if (! reallyContains(pos, false))
        return fretString;

    juce::Point<int> p(pos);

    for (int f = 0; f <= numFrets; f++)
    {
        int x, w;
        getFretPos(f, x, w);
        if (pos.getX() <= x)
        {
            fretString.fret     = f;
            float stringSpacing = (float) getHeight() / (float) numStrings;
            for (int s = 0; s < numStrings; s++)
            {
                if (pos.getY() < roundToInt(stringSpacing * (float) (s + 1)))
                {
                    fretString.string     = s;
                    mousePositionVelocity = 1.f;
                    return fretString;
                }
            }
        }
    }
    mousePositionVelocity = 0;
    return fretString;
}

//==============================================================================
void GuitarNeckComponent::repaintNote(const int fret)
{
    if (fret >= rangeStart && fret <= rangeEnd)
    {
        int x = 0;
        int w = getFretWidth(fret);
        if (fret > 0)
            x = getFretPos(fret - 1);
        repaint(x - 10, 0, w + 20, getHeight());
    }
}

void GuitarNeckComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::brown.brighter(0.2f));

    const juce::Colour lineColour(findColour(keySeparatorLineColourId));
    const juce::Colour textColour(findColour(textLabelColourId));

    for (int fret = firstFret; fret <= (numFrets - firstFret); fret++)
    {
        if (fret == 0)
        {
            g.setColour(juce::Colours::ivory);
            g.drawLine(0.f, 0.f, 0.f, (float) getHeight(), (float) nutWidth);
        }
        else
        {
            g.setColour(juce::Colours::grey);
            g.drawLine(fretPosition[fret] * getWidth() / fretPosition[numFrets], 0.f, fretPosition[fret] * getWidth() / fretPosition[numFrets], (float) getHeight(), 3.f);
            if (fret == 3 || fret == 5 || fret == 7 || fret == 9 || fret == 15 || fret == 17 || fret == 19 || fret == 21)
            {
                g.setColour(juce::Colours::black.withAlpha(0.2f));
                g.fillEllipse((getFretPos(fret) - getFretWidth(fret) * 0.5f) - 4.f, (float) getHeight() * 0.5f - 4.f, 8.f, 8.f);
            }
            else if (fret == 12 || fret == 24)
            {
                g.setColour(juce::Colours::black.withAlpha(0.2f));
                g.fillEllipse((getFretPos(fret) - getFretWidth(fret) * 0.5f) - 4.f, (float) getHeight() * 0.25f - 4.f, 8.f, 8.f);
                g.fillEllipse((getFretPos(fret) - getFretWidth(fret) * 0.5f) - 4.f, (float) getHeight() * 0.75f - 4.f, 8.f, 8.f);
            }
        }
    }

    float stringSpacing = (float) getHeight() / (float) numStrings;
    for (int string = 0; string < numStrings; string++)
    {
        float yStringPos = stringSpacing * 0.5f + stringSpacing * (float) string;
        g.setColour(juce::Colours::goldenrod);
        g.drawLine(0.f, yStringPos, (float) getWidth(), yStringPos, 2.f);

        if (currentlyFrettedFret[string] >= 0)
        {
            int x, w;
            getFretPos(currentlyFrettedFret[string], x, w);
            g.setColour(juce::Colours::pink);
            g.fillEllipse((float) x - (float) w * 0.5f - dotSize * 0.5f, yStringPos - dotSize * 0.5f, dotSize, dotSize);
            g.setColour(juce::Colours::black);
            g.drawEllipse((float) x - (float) w * 0.5f - dotSize * 0.5f, yStringPos - dotSize * 0.5f, dotSize, dotSize, 1.f);
            g.setFont(juce::Font(12.f, juce::Font::bold));
            g.drawFittedText(
                getNoteNameWithoutOctave(currentlyFrettedFret[string] + stringNote[string], ! showFlats),
                roundToInt(x - w / 2 - dotSize / 2 + 3),
                roundToInt(yStringPos - dotSize / 2 + 3),
                roundToInt(dotSize) - 6,
                roundToInt(dotSize) - 6,
                juce::Justification::centred,
                1,
                0.4f);
        }
        else
        {
            g.setColour(juce::Colours::darkred);
            g.drawLine(0, yStringPos - dotSize * 0.5f, dotSize, yStringPos + dotSize * 0.5f, 2.f);
            g.drawLine(0, yStringPos + dotSize * 0.5f, dotSize, yStringPos - dotSize * 0.5f, 2.f);
        }
    }
    if (noteUnderMouse.isValid())
    {
        int x, w;
        float yStringPos = stringSpacing * 0.5f + stringSpacing * (float) noteUnderMouse.string;
        getFretPos(noteUnderMouse.fret, x, w);
        g.setColour(juce::Colours::green);
        g.fillEllipse((float) x - (float) w * 0.5f - dotSize * 0.5f, yStringPos - dotSize * 0.5f, dotSize, dotSize);
        g.setColour(juce::Colours::black);
        g.drawEllipse((float) x - (float) w * 0.5f - dotSize * 0.5f, yStringPos - dotSize * 0.5f, dotSize, dotSize, 1.f);

        g.setFont(juce::Font(12.f, juce::Font::bold));
        g.drawFittedText(
            getNoteNameWithoutOctave(noteUnderMouse.fret + stringNote[noteUnderMouse.string], ! showFlats),
            roundToInt(x - w / 2 - dotSize / 2) + 3,
            roundToInt(yStringPos - dotSize / 2) + 3,
            roundToInt(dotSize) - 6,
            roundToInt(dotSize) - 6,
            juce::Justification::centred,
            1,
            0.4f);
    }
}

void GuitarNeckComponent::drawFretString(int fret, int string, juce::Graphics& g, int x, int y, int w, int h, bool isDown, bool isOver, const juce::Colour& lineColour, const juce::Colour& textColour)
{
    juce::Colour c(juce::Colours::transparentWhite);

    if (isDown)
        c = findColour(keyDownOverlayColourId);

    if (isOver)
        c = c.overlaidWith(findColour(mouseOverKeyOverlayColourId));

    g.setColour(c);
    g.fillRect(x, y, w, h);

    const juce::String text(getNoteText(fret, string));

    if (! text.isEmpty())
    {
        g.setColour(textColour);

        juce::Font f(12.0f);
        f.setHorizontalScale(0.8f);
        g.setFont(f);
        juce::Justification justification(juce::Justification::centredBottom);

        g.drawFittedText(text, x + 2, y + 2, w - 4, h - 4, justification, 1);
    }

    g.setColour(lineColour);

    g.fillRect(x, y, 1, h);

    if (fret == rangeEnd)
        g.fillRect(x + w, y, 1, h);
}

void GuitarNeckComponent::setOctaveForMiddleC(const int octaveNumForMiddleC_)
{
    octaveNumForMiddleC = octaveNumForMiddleC_;
    repaint();
}

const juce::String GuitarNeckComponent::getNoteText(const int fret, const int string)
{
    const int midiNoteNumber = stringNote[string] + fret;
    //if (keyWidth > 14.0f && midiNoteNumber % 12 == 0)
    return juce::MidiMessage::getMidiNoteName(midiNoteNumber, true, true, octaveNumForMiddleC);

    //return String();
}

void GuitarNeckComponent::getFretPos(int fret, int& x, int& w) const
{
    if (fret == 0)
    {
        w = nutWidth;
        x = nutWidth;
    }
    else
    {
        x = proportionOfWidth(fretPosition[fret] / fretPosition[numFrets]);
        w = proportionOfWidth((fretPosition[fret] - fretPosition[fret - 1]) / fretPosition[numFrets]);
    }
}

int GuitarNeckComponent::getFretPos(int fret) const
{
    if (fret == 0)
    {
        return nutWidth;
    }
    else
    {
        return proportionOfWidth(fretPosition[fret] / fretPosition[numFrets]);
    }
}

int GuitarNeckComponent::getFretWidth(int fret) const
{
    if (fret == 0)
    {
        return 40;
    }
    else
    {
        return proportionOfWidth((fretPosition[fret] - fretPosition[fret - 1]) / fretPosition[numFrets]);
    }
}

void GuitarNeckComponent::resized()
{
    int w = getWidth();
    int h = getHeight();

    if (w > 0 && h > 0)
    {
        int kx2, kw2;
        getFretPos(rangeEnd, kx2, kw2);

        kx2 += kw2;

        if (firstFret != rangeStart)
        {
            int kx1, kw1;
            getFretPos(rangeStart, kx1, kw1);

            if (kx2 - kx1 <= w)
            {
                firstFret = rangeStart;
                sendChangeMessage();
                repaint();
            }
        }

        xOffset   = 0;
        firstFret = rangeStart;

        timerCallback();
        repaint();
    }
}

//==============================================================================
void GuitarNeckComponent::handleNoteOn(juce::MidiKeyboardState*, int /*midiChannel*/, int /*midiNoteNumber*/, float /*velocity*/)
{
    triggerAsyncUpdate();
}

void GuitarNeckComponent::handleNoteOff(juce::MidiKeyboardState*, int /*midiChannel*/, int /*midiNoteNumber*/, float /*velocity*/)
{
    triggerAsyncUpdate();
}

void GuitarNeckComponent::handleAsyncUpdate()
{
    for (int s = 0; s < numStrings; s++)
    {
        for (int i = rangeStart + stringNote[s]; i <= rangeEnd + stringNote[s]; ++i)
        {
            if (state.isNoteOn(s + 1, i))
            {
                repaintNote(currentlyFrettedFret[s]);
                currentlyFrettedFret[s] = i - stringNote[s];
                repaintNote(currentlyFrettedFret[s]);
            }
            else if (currentlyFrettedFret[s] == i - stringNote[s])
            {
                repaintNote(currentlyFrettedFret[s]);
                currentlyFrettedFret[s] = -1;
                repaintNote(currentlyFrettedFret[s]);
            }
        }
    }
}

//==============================================================================
void GuitarNeckComponent::resetAnyKeysInUse()
{
    if (keysPressed.countNumberOfSetBits() > 0 || mouseDownNote.isValid())
    {
        state.allNotesOff(midiChannel);
        keysPressed.clear();
        mouseDownNote.invalidate();
    }
}

void GuitarNeckComponent::updateNoteUnderMouse(const juce::Point<int>& pos)
{
    float mousePositionVelocity = 0.0f;
    const FrettedNote newNote   = (mouseDragging || isMouseOver())
                                    ? xyToNote(pos, mousePositionVelocity)
                                    : FrettedNote();

    if (noteUnderMouse != newNote)
    {
        if (mouseDownNote.isValid())
        {
            //state.noteOff (mouseDownNote.string+1, getNote(mouseDownNote));
            currentlyFrettedFret[newNote.string] = -1;
            mouseDownNote.invalidate();
        }

        if (mouseDragging && newNote.isValid())
        {
            //if (! useMousePositionForVelocity)
            mousePositionVelocity = 1.0f;

            //state.noteOn (newNote.string+1, getNote(newNote), mousePositionVelocity * velocity);
            int oldFret                          = currentlyFrettedFret[newNote.string];
            currentlyFrettedFret[newNote.string] = newNote.fret;
            repaintNote(oldFret);
            mouseDownNote = newNote;
        }

        repaintNote(noteUnderMouse.fret);
        noteUnderMouse = newNote;
        repaintNote(noteUnderMouse.fret);
    }
    else if (mouseDownNote.isValid() && ! mouseDragging)
    {
        //state.noteOff (mouseDownNote.string+1, getNote(mouseDownNote));
        //currentlyFrettedFret[newNote.string]=-1;
        mouseDownNote.invalidate();
    }
}

void GuitarNeckComponent::mouseMove(const juce::MouseEvent& e)
{
    updateNoteUnderMouse(e.getPosition());
    stopTimer();
}

void GuitarNeckComponent::mouseDrag(const juce::MouseEvent& e)
{
    float mousePositionVelocity;
    const FrettedNote newNote = xyToNote(e.getPosition(), mousePositionVelocity);

    if (newNote.isValid())
        mouseDraggedToKey(newNote.fret, newNote.string, e);

    updateNoteUnderMouse(e.getPosition());
    repaint();
}

bool GuitarNeckComponent::mouseDownOnKey(int /*fret*/, int /*string*/, const juce::MouseEvent&)
{
    return true;
}

void GuitarNeckComponent::mouseDraggedToKey(int /*fret*/, int /*string*/, const juce::MouseEvent&)
{
}

void GuitarNeckComponent::mouseDown(const juce::MouseEvent& e)
{
    float mousePositionVelocity;
    const FrettedNote newNote = xyToNote(e.getPosition(), mousePositionVelocity);
    mouseDragging             = false;

    if (newNote.isValid() && mouseDownOnKey(newNote.fret, newNote.string, e))
    {
        //repaintNote (noteUnderMouse.fret);
        noteUnderMouse.invalidate();
        mouseDragging = true;

        currentlyFrettedFret[newNote.string] = newNote.fret;
        updateNoteUnderMouse(e.getPosition());
        startTimer(500);
        repaint();
    }
}

void GuitarNeckComponent::mouseUp(const juce::MouseEvent& e)
{
    mouseDragging = false;
    updateNoteUnderMouse(e.getPosition());

    stopTimer();
}

void GuitarNeckComponent::mouseEnter(const juce::MouseEvent& e)
{
    updateNoteUnderMouse(e.getPosition());
}

void GuitarNeckComponent::mouseExit(const juce::MouseEvent& e)
{
    updateNoteUnderMouse(e.getPosition());
}

void GuitarNeckComponent::timerCallback()
{
    updateNoteUnderMouse(getMouseXYRelative());
}

void GuitarNeckComponent::focusLost(FocusChangeType)
{
    resetAnyKeysInUse();
}
