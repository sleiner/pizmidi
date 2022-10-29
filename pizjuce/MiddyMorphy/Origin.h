#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ZoomingShiftingComponent;

class Origin : public juce::Component
{
public:
    Origin();

    void paint(juce::Graphics& g) override;

    void resized() override;

    void moved() override;

private:
    ZoomingShiftingComponent* owner;
};
