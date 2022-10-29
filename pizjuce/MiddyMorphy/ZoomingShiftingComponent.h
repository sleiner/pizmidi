#pragma once

#include "Origin.h"

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

class ZoomableShiftableComponent;

class ZoomingShiftingComponent : public juce::Component
{
public:
    ZoomingShiftingComponent();

    juce::XmlElement* getXml(juce::String tagName);
    void setFromXml(juce::XmlElement* data);

    void dragOrigin(const juce::MouseEvent& e);
    void startDragOrigin(const juce::MouseEvent& e);

    void zoom(float multX, float multY, float x, float y);
    void shift(juce::Point<int> newPosition);

    void rePositionChildren();

    float getXOffset();
    float getYOffset();

    float getZoomFactorX();
    float getZoomFactorY();

    void childBoundsChanged(juce::Component* component) override;

    void addZoomedComp(ZoomableShiftableComponent* zsComp, bool doZoom = true);
    void removeZoomedComp(ZoomableShiftableComponent* comp);
    void deleteAllZoomedComps();

private:
    Origin origin;

    float zoomFactorX;
    float zoomFactorY;

    juce::OwnedArray<ZoomableShiftableComponent> zoomedComponents;

    juce::ComponentDragger dragger;
};
