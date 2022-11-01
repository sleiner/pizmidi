#pragma once

#include "Module.h"

#include <juce_events/juce_events.h>

class MidiMorph;

class Cursor : public Module, public juce::ChangeBroadcaster
{
public:
    Cursor(MidiMorph* core);

    void moved() override;

    juce::Point<float> getCursorPosition();

private:
    MidiMorph* core;
};
