#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ZoomingShiftingComponent;

class ZoomableShiftableComponent : public juce::Component
{
public:
    virtual juce::Rectangle<int> getOriginalBounds() = 0;

    virtual void setOriginalBounds(juce::Rectangle<int> bounds);

    float getZoomFactorX();
    float getZoomFactorY();

    void rePosition();

    void setZoomer(ZoomingShiftingComponent* zoomer);

    void refreshOriginalBounds();

private:
    ZoomingShiftingComponent* zoomingComponent;
};
