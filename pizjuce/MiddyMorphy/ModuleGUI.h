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
    void mouseDrag(const MouseEvent & e) override;

    void mouseUp(const MouseEvent & e) override;

    void mouseDown(const MouseEvent & e) override;

    void setOriginalBounds(const juce::Rectangle<int> bounds) override;

    juce::Rectangle<int> getOriginalBounds() override;

    void paint(Graphics & g) override;

    void setPane(ModulePane * pane);

    void startDrag(juce::MouseEvent const& e);

    void drag(const MouseEvent & e);

    bool isSelected();

    Module* getModule();

};
