
#include "XYItem.h"

float XYItem::getDistance (juce::Point<float>& otherPoint)
{
    float dx = otherPoint.getX() - getX();
    float dy = otherPoint.getY() - getY();
    return pow (pow (dx, 2.f) + pow (dy, 2.f), 0.5f);
}

void XYItem::moved()
{
}

void XYItem::setXY (float x, float y)
{
    Point::setXY (x, y);
    moved();
}
