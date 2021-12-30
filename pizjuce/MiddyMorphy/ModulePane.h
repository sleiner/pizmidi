#pragma once


#include "juce_gui_basics/juce_gui_basics.h"
#include "ZoomingShiftingComponent.h"

class ModulePaneModel;
class ModuleGUI;
class Module;
//class D3CKStdCommands;

using namespace juce;

class ModulePane : public juce::LassoSource<ModuleGUI*>, public juce::ChangeListener, public ZoomingShiftingComponent, public juce::ApplicationCommandTarget {
  public:
    ModulePane(ModulePaneModel * modulePaneModel);

    ~ModulePane();


  private:
    LassoComponent<ModuleGUI*> lasso;

    Array<ModuleGUI*> modules;

    ModulePaneModel* model;


  public:
    SelectedItemSet<ModuleGUI*> selectedModules;

    //drag the origin around
    //call the reArrangeChildren
    void mouseDrag(const MouseEvent & e);

    void mouseDown(const MouseEvent & e);

    void mouseUp(const MouseEvent & e);

    void mouseWheelMove(const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);

    void updateContent();

    bool isModuleSelected(const Module * module);

    void paintOverChildren(Graphics & g);

juce_UseDebuggingNewOperator    void changeListenerCallback(ChangeBroadcaster* source);

    //  getLassoSelection
    SelectedItemSet<ModuleGUI*>& getLassoSelection();

    void findLassoItemsInArea(Array<ModuleGUI*>& itemsFound, const juce::Rectangle<int>& area) override;

    void selectModule(int index, bool deselectOthers);

    //perform ()=0
    bool perform(const InvocationInfo& info);

    //ApplicationCommandTarget *  getNextCommandTarget
    ApplicationCommandTarget* getNextCommandTarget();

    //void  getCommandInfo (const CommandID commandID, ApplicationCommandInfo &result)=
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result);

    //void  getAllCommands (Array< CommandID > &commands)=0
    void getAllCommands(Array<CommandID>& commands);

};
