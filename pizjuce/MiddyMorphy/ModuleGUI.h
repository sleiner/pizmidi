#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "ZoomableShiftableComponent.h"

class Module;
class ModulePane;

class ModuleGUI : public ZoomableShiftableComponent
{
public:
    ModuleGUI (Module* module);

    void mouseDrag (const juce::MouseEvent& e) override;

    void mouseUp (const juce::MouseEvent& e) override;

    void mouseDown (const juce::MouseEvent& e) override;

    void setOriginalBounds (juce::Rectangle<int> bounds) override;

    juce::Rectangle<int> getOriginalBounds() override;

    void paint (juce::Graphics& g) override;

    void setPane (ModulePane* pane);

    void startDrag (juce::MouseEvent const& e);

    void drag (const juce::MouseEvent& e);

    bool isSelected();

    Module* getModule();

private:
    bool selectionBool;

    Module* module;

    juce::ComponentDragger dragger;

    ModulePane* pane;
};
