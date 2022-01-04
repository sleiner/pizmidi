#pragma once

#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"


class MidiMorph;
namespace juce { class Component; }
class ControllerGUI;

using namespace juce;
class ControllerList : public juce::ListBoxModel, public juce::ChangeBroadcaster {
  public:
    ControllerList(MidiMorph * core);

    ~ControllerList();


  private:
    MidiMorph* core;


  public:
    Component* refreshComponentForRow(int rowNumber, bool isRowSelected, Component* existingComponentToUpdate);

    void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);

    int getNumRows();

    void addController();

    void scenesSelected();

    void distancesChanged();

};
