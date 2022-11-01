#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ModuleGUI;
class ModulePane;
class Module;

class ModulePaneModel
{
public:
    virtual ModuleGUI* createGUI(int index) = 0;

    virtual int getNumModules() = 0;

    virtual void mouseDown(const juce::MouseEvent& e);

    virtual void mouseUp(const juce::MouseEvent& e);
    virtual void mouseDrag(const juce::MouseEvent& e);

    virtual void mouseDoubleClick(const juce::MouseEvent& e);

    virtual Module* getModule(int index) = 0;

    ModulePane* getOwnerPane();

    void refreshModulePane();

    void setOwnerPane(ModulePane* ownerPane);

    bool isModuleSelected(const Module* module);

    virtual void selectionChanged(const juce::Array<ModuleGUI*>& modules) = 0;

    bool performCommand(const juce::ApplicationCommandTarget::InvocationInfo& info);

    bool deleteModule(const Module* moduleToDelete, int indexOfModule);

private:
    ModulePane* ownerPane;
};
