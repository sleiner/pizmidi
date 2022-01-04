
#include "XYItem.h"

float XYItem::getDistance(juce::Point<float> & otherPoint) {
  // Bouml preserved body begin 0003500D
	float dx = otherPoint.getX() - getX();
	float dy = otherPoint.getY() - getY();
	return pow( pow( dx, 2.f) + pow( dy, 2.f), 0.5f);
  // Bouml preserved body end 0003500D
}

//gets called when the XYItem gets new coordinates
void XYItem::moved() {
  // Bouml preserved body begin 0003570D
  // Bouml preserved body end 0003570D
}

void XYItem::setXY(float x, float y) {
  // Bouml preserved body begin 0003578D
	Point::setXY(x,y);
	moved();
  // Bouml preserved body end 0003578D
}

