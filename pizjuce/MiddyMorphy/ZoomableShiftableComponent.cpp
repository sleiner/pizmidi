
#include "ZoomableShiftableComponent.h"
#include "ZoomingShiftingComponent.h"

using juce::roundToInt;

void ZoomableShiftableComponent::setOriginalBounds (const juce::Rectangle<int> bounds)
{
}

float ZoomableShiftableComponent::getZoomFactorX()
{
    return this->zoomingComponent->getZoomFactorX();
}

float ZoomableShiftableComponent::getZoomFactorY()
{
    return this->zoomingComponent->getZoomFactorY();
}

void ZoomableShiftableComponent::rePosition()
{
    juce::Rectangle<int> rect = getOriginalBounds();

    int w = roundToInt ((float) rect.getWidth() * getZoomFactorX());
    int h = roundToInt ((float) rect.getHeight() * getZoomFactorY());
    int x = roundToInt ((float) rect.getX() * getZoomFactorX());
    int y = roundToInt ((float) rect.getY() * getZoomFactorY());

    setBounds (roundToInt (zoomingComponent->getXOffset()) + x, roundToInt (zoomingComponent->getYOffset()) + y, w, h);
}

void ZoomableShiftableComponent::setZoomer (ZoomingShiftingComponent* zoomer)
{
    zoomingComponent = zoomer;
}

void ZoomableShiftableComponent::refreshOriginalBounds()
{
    float x = getX() - zoomingComponent->getXOffset();
    float y = getY() - zoomingComponent->getYOffset();

    float w = getWidth() / getZoomFactorX();
    float h = getHeight() / getZoomFactorY();
    x       = x / getZoomFactorX();
    y       = y / getZoomFactorY();

    juce::Rectangle<int> rect (roundToInt (x), roundToInt (y), roundToInt (w), roundToInt (h));
    setOriginalBounds (rect);
}
