#pragma once

#include <juce_events/juce_events.h>

#include "ModuleGUI.h"

class Cursor;

class CursorGUI : public ModuleGUI, public juce::ChangeListener
{
public:
    CursorGUI (Cursor* cursor);
    ~CursorGUI() override;

    void paint (juce::Graphics& g) override;

    void mouseDown (const juce::MouseEvent& e) override;
    void mouseUp (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;

    void changeListenerCallback (juce::ChangeBroadcaster* source) override;

private:
    juce::ComponentDragger dragger;
    Cursor* cursor;
};
