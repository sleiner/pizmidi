#pragma once

#include "juce_events/juce_events.h"

#include "ModuleGUI.h"

class Cursor;

using namespace juce;

class CursorGUI : public ModuleGUI, public juce::ChangeListener {
  private:
    juce::ComponentDragger dragger;


  public:
    void paint(Graphics & g);

    CursorGUI(Cursor * cursor);

    void mouseDown(const MouseEvent & e);

    void mouseDrag(const MouseEvent & e);

    void mouseUp(const MouseEvent & e);

    virtual void changeListenerCallback(ChangeBroadcaster* source) override;


  private:
    Cursor* cursor;


  public:
    ~CursorGUI();

};
