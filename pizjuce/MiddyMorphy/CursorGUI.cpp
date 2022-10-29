
#include "CursorGUI.h"
#include "Cursor.h"

CursorGUI::CursorGUI (Cursor* cursor)
    : ModuleGUI (cursor)
{
    this->setAlwaysOnTop (true);
    this->cursor = cursor;
    cursor->addChangeListener (this);
    this->setMouseClickGrabsKeyboardFocus (false);
}

CursorGUI::~CursorGUI()
{
    cursor->removeChangeListener (this);
}

void CursorGUI::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::blue);
    g.drawLine ((float) getWidth() * 0.5f, 0, (float) getWidth() * 0.5f, (float) getHeight(), 2);
    g.drawLine (0, (float) getHeight() * 0.5f, (float) getWidth(), (float) getHeight() * 0.5f, 2);
    g.drawEllipse (6, 6, (float) (getWidth() - 12), (float) (getHeight() - 12), 2);
    g.setColour (juce::Colours::orange);
    g.drawEllipse (5, 5, (float) (getWidth() - 10), (float) (getHeight() - 10), 4);
}

void CursorGUI::mouseDown (const juce::MouseEvent& e)
{
    this->setInterceptsMouseClicks (true, false);
    if (e.mods.isLeftButtonDown() && ! e.mods.isAltDown() && ! e.mods.isCtrlDown())
    {
        startDrag (e);
    }
}

void CursorGUI::mouseDrag (const juce::MouseEvent& e)
{
    this->setInterceptsMouseClicks (true, false);
    if (e.mods.isLeftButtonDown() && ! e.mods.isAltDown() && ! e.mods.isCtrlDown())
    {
        this->drag (e);
    }
}

void CursorGUI::mouseUp (const juce::MouseEvent& e)
{
    if (e.mods.isRightButtonDown())
    {
        this->setInterceptsMouseClicks (false, true);
    }
    else
    {
        this->setInterceptsMouseClicks (true, false);
    }
}

void CursorGUI::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (source == cursor)
    {
        rePosition();
    }
}
