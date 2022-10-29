
#include "ControllerList.h"

#include "ControllerGUI.h"
#include "MidiMorph.h"

ControllerList::ControllerList(MidiMorph* core)
{
    this->core = core;
}

ControllerList::~ControllerList()
{
    //this->removeAllChangeListeners();
}

juce::Component* ControllerList::refreshComponentForRow(int rowNumber, bool isRowSelected, juce::Component* existingComponentToUpdate)
{
    if (existingComponentToUpdate != 0)
    {
        //removeChangeListener((ControllerGUI*)existingComponentToUpdate);
    }
    Controller* controller = core->getController(rowNumber);
    if (controller != 0)
    {
        ControllerGUI* gui = new ControllerGUI(controller, core);
        gui->setSelected(isRowSelected);
        //addChangeListener(gui);
        return gui;
    }
    else
        return 0;
}

void ControllerList::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
}

int ControllerList::getNumRows()
{
    return core->getNumControllers();
}

void ControllerList::scenesSelected()
{
    //this->sendChangeMessage(core->getSelectedScenes());
}

void ControllerList::distancesChanged()
{
    //sendChangeMessage(core->getCursor());
}
