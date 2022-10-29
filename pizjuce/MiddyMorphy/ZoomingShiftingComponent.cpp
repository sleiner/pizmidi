#include "ZoomingShiftingComponent.h"
#include "ZoomableShiftableComponent.h"

using juce::roundToInt;

ZoomingShiftingComponent::ZoomingShiftingComponent()
{
    addAndMakeVisible (&origin);
    zoomFactorX = zoomFactorY = 1;
}

juce::XmlElement* ZoomingShiftingComponent::getXml (const juce::String tagName)
{
    auto* state = new juce::XmlElement (tagName);
    state->setAttribute ("xoff", this->origin.getX());
    state->setAttribute ("yoff", this->origin.getY());
    state->setAttribute ("xzoom", this->zoomFactorX);
    state->setAttribute ("yzoom", this->zoomFactorX);
    return state;
}

void ZoomingShiftingComponent::setFromXml (juce::XmlElement* data)
{
    if (data != nullptr)
    {
        int x = data->getIntAttribute ("xoff");
        int y = data->getIntAttribute ("yoff");

        this->zoomFactorX = (float) data->getDoubleAttribute ("xzoom");
        this->zoomFactorY = (float) data->getDoubleAttribute ("yzoom");
        origin.setTopLeftPosition (x, y);
    }
}

void ZoomingShiftingComponent::dragOrigin (const juce::MouseEvent& e)
{
    this->dragger.dragComponent (&origin, e, nullptr);
}

void ZoomingShiftingComponent::startDragOrigin (const juce::MouseEvent& e)
{
    dragger.startDraggingComponent (&origin, e);
}

void ZoomingShiftingComponent::zoom (float multX, float multY, float x, float y)
{
    float newZoomRatioX = multX * zoomFactorX;
    float newZoomRatioY = multY * zoomFactorY;

    double grownTop  = 0;
    double grownLeft = 0;

    if (newZoomRatioX >= 0.1 && newZoomRatioX <= 10)
    {
        x           = origin.getX() - x;
        grownLeft   = (double) x * (newZoomRatioX - zoomFactorX) / zoomFactorY;
        zoomFactorX = newZoomRatioX;
    }
    if (newZoomRatioY >= 0.1 && newZoomRatioY <= 10)
    {
        y           = origin.getY() - y;
        grownTop    = (double) y * (newZoomRatioY - zoomFactorY) / zoomFactorY;
        zoomFactorY = newZoomRatioY;
    }

    origin.setTopLeftPosition (roundToInt ((double) origin.getX() + grownLeft),
                               roundToInt ((double) origin.getY() + grownTop));
}

void ZoomingShiftingComponent::shift (const juce::Point<int> newPosition)
{
    origin.setTopLeftPosition (newPosition.getX(), newPosition.getY());
}

void ZoomingShiftingComponent::rePositionChildren()
{
    for (int i = 0; i < zoomedComponents.size(); i++)
    {
        zoomedComponents[i]->rePosition();
    }
}

float ZoomingShiftingComponent::getXOffset()
{
    return (float) origin.getX();
}

float ZoomingShiftingComponent::getYOffset()
{
    return (float) origin.getY();
}

float ZoomingShiftingComponent::getZoomFactorX()
{
    return this->zoomFactorX;
}

float ZoomingShiftingComponent::getZoomFactorY()
{
    return this->zoomFactorY;
}

void ZoomingShiftingComponent::childBoundsChanged (juce::Component* component)
{
    if (component == &origin)
    {
        this->rePositionChildren();
    }
}

void ZoomingShiftingComponent::deleteAllZoomedComps()
{
    for (int i = 0; i < zoomedComponents.size(); i++)
    {
        removeChildComponent (zoomedComponents[i]);
    }
    zoomedComponents.clear (true);
}

void ZoomingShiftingComponent::addZoomedComp (ZoomableShiftableComponent* zsComp, bool doZoom)
{
    addAndMakeVisible (zsComp);
    zsComp->setZoomer (this);
    this->zoomedComponents.add (zsComp);
    zsComp->rePosition();
}

void ZoomingShiftingComponent::removeZoomedComp (ZoomableShiftableComponent* comp)
{
    removeChildComponent (comp);
    zoomedComponents.remove (zoomedComponents.indexOf (comp), true);
}
