#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

class ZoomingShiftingComponent;

using namespace juce;

class Origin : public juce::Component {
  public:
    void paint(Graphics & g);

    Origin();

    void resized();


  private:
    ZoomingShiftingComponent* owner;


  public:
    void moved();


  private:
    float originalSize;

};
