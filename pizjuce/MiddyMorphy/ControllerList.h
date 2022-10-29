#pragma once

#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>

class MidiMorph;
class ControllerGUI;

class ControllerList : public juce::ListBoxModel, public juce::ChangeBroadcaster
{
public:
    ControllerList (MidiMorph* core);

    ~ControllerList() override;

    juce::Component* refreshComponentForRow (int rowNumber, bool isRowSelected, juce::Component* existingComponentToUpdate) override;

    void paintListBoxItem (int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;

    int getNumRows() override;

    void scenesSelected();

    void distancesChanged();

private:
    MidiMorph* core;
};
