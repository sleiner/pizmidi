#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

class ModuleGUI;
class ModulePane;
class Module;

using namespace juce;

class ModulePaneModel {
  public:
    virtual ModuleGUI* createGUI(int index) = 0;

    virtual int getNumModules() = 0;

    virtual void mouseDown(const MouseEvent & e);

    virtual void mouseUp(const MouseEvent & e);
    virtual void mouseDrag(const MouseEvent & e);

    virtual void mouseDoubleClick(const MouseEvent & e);


  private:
    ModulePane* ownerPane;


  public:
    virtual Module* getModule(int index) = 0;

    ModulePane* getOwnerPane();

    void refreshModulePane();

    void setOwnerPane(ModulePane * ownerPane);

    bool isModuleSelected(const Module * module);

//    virtual SelectedItemSet<Module*> getSelectedModules();

    virtual void selectionChanged(const Array<ModuleGUI*> & modules) = 0;

    //const InvocationInfo &
    bool performCommand(const juce::ApplicationCommandTarget::InvocationInfo & info);

    bool deleteModule(const Module * moduleToDelete, int indexOfModule);

};
