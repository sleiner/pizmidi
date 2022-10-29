
#include "Origin.h"
#include "ZoomingShiftingComponent.h"

Origin::Origin()
{
    this->setSize (10, 10);
}

void Origin::paint (juce::Graphics& g)
{
    g.drawLine (0, 0, (float) getWidth(), 0);
    g.drawLine (0, 0, 0, (float) getHeight());
}

void Origin::resized()
{
}

void Origin::moved()
{
}
