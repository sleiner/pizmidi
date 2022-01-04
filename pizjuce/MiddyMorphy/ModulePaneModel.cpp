
#include "ModulePaneModel.h"
#include "ModuleGUI.h"
#include "ModulePane.h"
#include "Module.h"

void ModulePaneModel::mouseDown(const MouseEvent & e) {
  // Bouml preserved body begin 0003D30D

  // Bouml preserved body end 0003D30D
}

void ModulePaneModel::mouseUp(const MouseEvent & e) {
  // Bouml preserved body begin 0003D38D

  // Bouml preserved body end 0003D38D
}

void ModulePaneModel::mouseDoubleClick(const MouseEvent & e) {
  // Bouml preserved body begin 0003D40D
  // Bouml preserved body end 0003D40D
}

void ModulePaneModel::mouseDrag(const MouseEvent & e) {
}

ModulePane* ModulePaneModel::getOwnerPane() {
  // Bouml preserved body begin 0003DA0D
	return ownerPane;
  // Bouml preserved body end 0003DA0D
}

void ModulePaneModel::refreshModulePane() {
  // Bouml preserved body begin 0003DB0D
	ownerPane->updateContent();
  // Bouml preserved body end 0003DB0D
}

void ModulePaneModel::setOwnerPane(ModulePane * ownerPane) {
  // Bouml preserved body begin 0003DB8D
	this->ownerPane = ownerPane;
  // Bouml preserved body end 0003DB8D
}

bool ModulePaneModel::isModuleSelected(const Module * module) {
  // Bouml preserved body begin 0003DD8D
	return false;
  // Bouml preserved body end 0003DD8D
}

//SelectedItemSet<Module*> ModulePaneModel::getSelectedModules() {
//  return 0;
//}

//const InvocationInfo &    
bool ModulePaneModel::performCommand(const juce::ApplicationCommandTarget::InvocationInfo & info) {
  // Bouml preserved body begin 0004930D
	return this->ownerPane->perform(info);
  // Bouml preserved body end 0004930D
}

bool ModulePaneModel::deleteModule(const Module * moduleToDelete, int indexOfModule) {
  // Bouml preserved body begin 0004978D
	return false;
  // Bouml preserved body end 0004978D
}

