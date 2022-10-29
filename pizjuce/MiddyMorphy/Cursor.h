#pragma once

#include <juce_events/juce_events.h>

#include "Module.h"

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
