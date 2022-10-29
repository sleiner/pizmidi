#pragma once

#include "ZoomingShiftingComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>

class ModulePaneModel;
class ModuleGUI;
class Module;

class ModulePane : public juce::LassoSource<ModuleGUI*>, public juce::ChangeListener, public ZoomingShiftingComponent, public juce::ApplicationCommandTarget
{
public:
    ModulePane(ModulePaneModel* modulePaneModel);

    ~ModulePane() override;

    juce::SelectedItemSet<ModuleGUI*> selectedModules;

    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseWheelMove(const juce::MouseEvent& e, float wheelIncrementX, float wheelIncrementY);

    void updateContent();

    bool isModuleSelected(const Module* module);

    void paintOverChildren(juce::Graphics& g) override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    juce::SelectedItemSet<ModuleGUI*>& getLassoSelection() override;

    void findLassoItemsInArea(juce::Array<ModuleGUI*>& itemsFound, const juce::Rectangle<int>& area) override;

    void selectModule(int index, bool deselectOthers);

    bool perform(const InvocationInfo& info) override;

    ApplicationCommandTarget* getNextCommandTarget() override;

    void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;

    void getAllCommands(juce::Array<juce::CommandID>& commands) override;

private:
    juce::LassoComponent<ModuleGUI*> lasso;
    juce::Array<ModuleGUI*> modules;

    ModulePaneModel* model;

    JUCE_LEAK_DETECTOR(ModulePane)
};
