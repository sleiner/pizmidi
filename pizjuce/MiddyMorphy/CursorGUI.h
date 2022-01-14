#pragma once

#include "juce_events/juce_events.h"

#include "ModuleGUI.h"

class Cursor;

using namespace juce;

class CursorGUI : public ModuleGUI, public juce::ChangeListener {
  private:
    juce::ComponentDragger dragger;


  public:
    void paint(Graphics & g) override;

    CursorGUI(Cursor * cursor);

    void mouseDown(const MouseEvent & e) override;

    void mouseDrag(const MouseEvent & e) override;

    void mouseUp(const MouseEvent & e) override;

    void changeListenerCallback(ChangeBroadcaster* source) override;


  private:
    Cursor* cursor;


  public:
    ~CursorGUI() override;

};
