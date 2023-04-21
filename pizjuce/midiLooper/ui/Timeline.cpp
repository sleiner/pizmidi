#include "Timeline.h"

#include "PianoRoll.h"

using juce::jlimit;
using juce::jmax;

Timeline::Timeline()
    : roll(nullptr),
      scrollOffset(0),
      loopStart(0),
      loopEnd(0)
{
    setLoop(0, 0);
    setMouseClickGrabsKeyboardFocus(false);
}

Timeline::~Timeline()
{
    dispatchPendingMessages();
}

void Timeline::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0x0));
    if (roll && getLength() != 0)
    {
        g.setColour(juce::Colour(0x79006b00));
        g.fillRect(getStartPixel(), 0.f, getEndPixel() - getStartPixel(), (float) getHeight());
        g.setColour(juce::Colours::white);
        g.drawFittedText("LOOP AREA", (int) getStartPixel(), 0, (int) (getEndPixel() - getStartPixel()), getHeight(), juce::Justification::centred, 2);
    }
}

float Timeline::getStartPixel()
{
    return roll->ppqToPixels(loopStart) - scrollOffset;
}

float Timeline::getEndPixel()
{
    return roll->ppqToPixels(loopEnd) - scrollOffset;
}

double Timeline::getLength()
{
    return (loopEnd - loopStart) / 960.0;
}

double Timeline::getStart()
{
    return loopStart / 960.0;
}

void Timeline::setLoop(double start, double length)
{
    loopStart = start * 960.0;
    loopEnd   = (start + length) * 960.0;
    repaint();
}

void Timeline::setPianoRoll(PianoRoll* pr)
{
    roll = pr;
    setSize(pr->getWidth(), getHeight());
}

void Timeline::mouseDown(const juce::MouseEvent& e)
{
    bool snap = roll->getSnap() != e.mods.isShiftDown();
    if (e.mods.isPopupMenu())
    {
        loopEnd = jmax(loopStart, roll->pixelsToPpq((float) e.x + scrollOffset, snap, true));
    }
    else
    {
        loopStart = jlimit(0.0, loopEnd, roll->pixelsToPpq((float) e.x + scrollOffset, snap, true));
    }
    repaint();
    sendChangeMessage();
}

void Timeline::mouseDrag(const juce::MouseEvent& e)
{
    bool snap = roll->getSnap() != e.mods.isShiftDown();
    if (e.mods.isPopupMenu())
    {
        loopEnd = jlimit(loopStart, (double) roll->seqLengthInPpq, (roll->pixelsToPpq((float) e.x + scrollOffset, snap, true)));
    }
    else
    {
        loopStart = jlimit(0.0, loopEnd, roll->pixelsToPpq((float) e.x + scrollOffset, snap, true));
    }
    repaint();
    sendChangeMessage();
}