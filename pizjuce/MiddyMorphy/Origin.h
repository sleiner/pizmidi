#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

class ZoomingShiftingComponent;

using namespace juce;

class Origin : public juce::Component {
  public:
    void paint(Graphics & g) override;

    Origin();

    void resized() override;


  private:
    ZoomingShiftingComponent* owner;


  public:
    void moved() override;


  private:
    float originalSize;

};
