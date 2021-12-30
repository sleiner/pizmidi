
#include "ZoomableShiftableComponent.h"
#include "ZoomingShiftingComponent.h"

void ZoomableShiftableComponent::setOriginalBounds(const Rectangle<int> bounds) {
  // Bouml preserved body begin 0003DF0D
  // Bouml preserved body end 0003DF0D
}

//returns
float ZoomableShiftableComponent::getZoomFactorX() {
  // Bouml preserved body begin 0003AE8D
	return this->zoomingComponent->getZoomFactorX();
  // Bouml preserved body end 0003AE8D
}

//returns
float ZoomableShiftableComponent::getZoomFactorY() {
  // Bouml preserved body begin 0003E10D
	return this->zoomingComponent->getZoomFactorY();
  // Bouml preserved body end 0003E10D
}

//calcu
void ZoomableShiftableComponent::rePosition() {
  // Bouml preserved body begin 0003AF0D
	Rectangle<int> rect = getOriginalBounds();
	int w = roundFloatToInt((float)rect.getWidth()*getZoomFactorX());
	int h = roundFloatToInt((float)rect.getHeight()*getZoomFactorY());
	int x = roundFloatToInt((float)rect.getX()*getZoomFactorX());
	int y = roundFloatToInt((float)rect.getY()*getZoomFactorY());
	setBounds(roundToInt(zoomingComponent->getXOffset())+x,roundToInt(zoomingComponent->getYOffset())+y,w,h);
  // Bouml preserved body end 0003AF0D
}

void ZoomableShiftableComponent::setZoomer(ZoomingShiftingComponent * zoomer) {
  // Bouml preserved body begin 0003DC8D
	zoomingComponent = zoomer;
  // Bouml preserved body end 0003DC8D
}

void ZoomableShiftableComponent::refreshOriginalBounds() {
  // Bouml preserved body begin 0003E00D
	float x = getX()-zoomingComponent->getXOffset();
	float y = getY()-zoomingComponent->getYOffset();

	float w = getWidth()/getZoomFactorX();
	float h = getHeight()/getZoomFactorY();
	x = x/getZoomFactorX();
	y = y/getZoomFactorY();

	Rectangle<int> rect(roundFloatToInt(x),roundFloatToInt(y),roundFloatToInt(w),roundFloatToInt(h));
	setOriginalBounds(rect);
  // Bouml preserved body end 0003E00D
}

