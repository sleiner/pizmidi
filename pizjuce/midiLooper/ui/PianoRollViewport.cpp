#include "PianoRollViewport.h"

PianoRollViewport::PianoRollViewport(const juce::String& name)
    : Viewport(name),
      timeline(nullptr),
      playBar(nullptr),
      keyboard(nullptr)
{
}

PianoRollViewport::~PianoRollViewport()
{
    dispatchPendingMessages();
}

void PianoRollViewport::setTimeline(Timeline* t)
{
    timeline = t;
}

void PianoRollViewport::setPlayBar(Component* p)
{
    playBar = p;
}

void PianoRollViewport::setKeyboard(juce::Viewport* kb)
{
    keyboard = kb;
}

void PianoRollViewport::mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel)
{
    if (onMouseWheelMove != nullptr)
    {
        onMouseWheelMove(e, wheel);
    }
}

void PianoRollViewport::visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea)
{
    timeline->scrollOffset = newVisibleArea.getX();
    timeline->repaint();
    playBar->setBounds(newVisibleArea);
    keyboard->setViewPosition(0, newVisibleArea.getY());
    sendChangeMessage();
}