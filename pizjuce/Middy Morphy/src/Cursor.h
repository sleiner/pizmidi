#pragma once

#include "juce_events/juce_events.h"

#include "Module.h"

class MidiMorph;
using namespace juce;

class Cursor : public Module, public juce::ChangeBroadcaster {
  public:
    Cursor(MidiMorph * core);

    //gets called when the XYItem gets new coordinates
    virtual void moved();


  private:
    MidiMorph* core;


  public:
    juce::Point<float> getCursorPosition();

};
