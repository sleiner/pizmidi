
#include "ModulePaneModel.h"
#include "Module.h"
#include "ModuleGUI.h"
#include "ModulePane.h"

void ModulePaneModel::mouseDown(const juce::MouseEvent& e)
{
}

void ModulePaneModel::mouseUp(const juce::MouseEvent& e)
{
}

void ModulePaneModel::mouseDoubleClick(const juce::MouseEvent& e)
{
}

void ModulePaneModel::mouseDrag(const juce::MouseEvent& e)
{
}

ModulePane* ModulePaneModel::getOwnerPane()
{
    return ownerPane;
}

void ModulePaneModel::refreshModulePane()
{
    ownerPane->updateContent();
}

void ModulePaneModel::setOwnerPane(ModulePane* ownerPane)
{
    this->ownerPane = ownerPane;
}

bool ModulePaneModel::isModuleSelected(const Module* module)
{
    return false;
}

bool ModulePaneModel::performCommand(const juce::ApplicationCommandTarget::InvocationInfo& info)
{
    return this->ownerPane->perform(info);
}

bool ModulePaneModel::deleteModule(const Module* moduleToDelete, int indexOfModule)
{
    return false;
}
