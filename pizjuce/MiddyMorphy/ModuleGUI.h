#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

#include "ZoomableShiftableComponent.h"

class Module;
class ModulePane;

using namespace juce;

class ModuleGUI : public ZoomableShiftableComponent {
  public:
    ModuleGUI(Module * module);


  private:
    bool selectionBool;

    Module* module;

    juce::ComponentDragger dragger;

    ModulePane* pane;


  public:
    void mouseDrag(const MouseEvent & e);

    void mouseUp(const MouseEvent & e);

    void mouseDown(const MouseEvent & e);

    virtual void setOriginalBounds(const juce::Rectangle<int> bounds);

    virtual juce::Rectangle<int> getOriginalBounds();

    void paint(Graphics & g);

    void setPane(ModulePane * pane);

    void startDrag(juce::MouseEvent const& e);

    void drag(const MouseEvent & e);

    bool isSelected();

    Module* getModule();

};
