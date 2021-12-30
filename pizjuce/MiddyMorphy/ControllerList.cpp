
#include "MidiMorph.h"
#include "ControllerList.h"
#include "ControllerGUI.h"

ControllerList::ControllerList(MidiMorph * core) {
  // Bouml preserved body begin 0003170D
	this->core = core;
  // Bouml preserved body end 0003170D
}

ControllerList::~ControllerList() {
  // Bouml preserved body begin 0004210D
	//this->removeAllChangeListeners();
  // Bouml preserved body end 0004210D
}

Component* ControllerList::refreshComponentForRow(int rowNumber, bool isRowSelected, Component* existingComponentToUpdate) {
  // Bouml preserved body begin 00033A0D

			if(existingComponentToUpdate != 0)
			{
				//removeChangeListener((ControllerGUI*)existingComponentToUpdate);
			}
			Controller* controller = core->getController(rowNumber);
			if(controller != 0)
			{
				ControllerGUI* gui = new ControllerGUI(controller,core);
				gui->setSelected(isRowSelected);
				//addChangeListener(gui);
				return gui;
			}
			else
			return 0;
	
  // Bouml preserved body end 00033A0D
}

void ControllerList::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) {
  // Bouml preserved body begin 0003168D
  // Bouml preserved body end 0003168D
}

int ControllerList::getNumRows() {
  // Bouml preserved body begin 0003160D
	return core->getNumControllers();
  // Bouml preserved body end 0003160D
}

void ControllerList::addController() {
  // Bouml preserved body begin 00034D0D
  // Bouml preserved body end 00034D0D
}

void ControllerList::scenesSelected() {
  // Bouml preserved body begin 0003EF8D
	//this->sendChangeMessage(core->getSelectedScenes());
  // Bouml preserved body end 0003EF8D
}

void ControllerList::distancesChanged() {
  // Bouml preserved body begin 0003F40D
	//sendChangeMessage(core->getCursor());
  // Bouml preserved body end 0003F40D
}

