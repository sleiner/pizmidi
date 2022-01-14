#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;
class ZoomingShiftingComponent;

class ZoomableShiftableComponent : public juce::Component
{
public:
    virtual juce::Rectangle<int> getOriginalBounds() = 0;

    virtual void setOriginalBounds (const juce::Rectangle<int> bounds);

    //returns
    float getZoomFactorX();

    //returns
    float getZoomFactorY();

    //calcu
    void rePosition();

private:
    ZoomingShiftingComponent* zoomingComponent;

public:
    void setZoomer (ZoomingShiftingComponent* zoomer);

    void refreshOriginalBounds();
};
