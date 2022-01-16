#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "ZoomingShiftingComponent.h"

class ModulePaneModel;
class ModuleGUI;
class Module;
//class D3CKStdCommands;

using namespace juce;

class ModulePane : public juce::LassoSource<ModuleGUI*>, public juce::ChangeListener, public ZoomingShiftingComponent, public juce::ApplicationCommandTarget
{
public:
    ModulePane (ModulePaneModel* modulePaneModel);

    ~ModulePane() override;

private:
    LassoComponent<ModuleGUI*> lasso;

    Array<ModuleGUI*> modules;

    ModulePaneModel* model;

public:
    SelectedItemSet<ModuleGUI*> selectedModules;

    //drag the origin around
    //call the reArrangeChildren
    void mouseDrag (const MouseEvent& e) override;

    void mouseDown (const MouseEvent& e) override;

    void mouseUp (const MouseEvent& e) override;

    void mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);

    void updateContent();

    bool isModuleSelected (const Module* module);

    void paintOverChildren (Graphics& g) override;

    juce_UseDebuggingNewOperator void changeListenerCallback (ChangeBroadcaster* source) override;

    //  getLassoSelection
    SelectedItemSet<ModuleGUI*>& getLassoSelection() override;

    void findLassoItemsInArea (Array<ModuleGUI*>& itemsFound, const juce::Rectangle<int>& area) override;

    void selectModule (int index, bool deselectOthers);

    //perform ()=0
    bool perform (const InvocationInfo& info) override;

    //ApplicationCommandTarget *  getNextCommandTarget
    ApplicationCommandTarget* getNextCommandTarget() override;

    //void  getCommandInfo (const CommandID commandID, ApplicationCommandInfo &result)=
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override;

    //void  getAllCommands (Array< CommandID > &commands)=0
    void getAllCommands (Array<CommandID>& commands) override;
};
