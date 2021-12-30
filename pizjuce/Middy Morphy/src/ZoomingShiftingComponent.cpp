#include "ZoomingShiftingComponent.h"
#include "ZoomableShiftableComponent.h"

juce::XmlElement* ZoomingShiftingComponent::getXml(const String tagName) {
  // Bouml preserved body begin 0004198D
	XmlElement* state = new XmlElement(tagName);
	state->setAttribute("xoff",this->origin.getX());
	state->setAttribute("yoff",this->origin.getY());
	state->setAttribute("xzoom",this->zoomFactorX);
	state->setAttribute("yzoom",this->zoomFactorX);
	return state;
  // Bouml preserved body end 0004198D
}

void ZoomingShiftingComponent::setFromXml(juce::XmlElement * data) {
  // Bouml preserved body begin 00041A0D
	if(data != 0)
	{
	int x = data->getIntAttribute("xoff");
	int y = data->getIntAttribute("yoff");
	this->zoomFactorX = (float)data->getDoubleAttribute("xzoom");
	this->zoomFactorY = (float)data->getDoubleAttribute("yzoom");
	origin.setTopLeftPosition(x,y);
	}
  // Bouml preserved body end 00041A0D
}

void ZoomingShiftingComponent::dragOrigin(const MouseEvent & e) {
  // Bouml preserved body begin 0003D58D
	this->dragger.dragComponent(&origin,e,nullptr);
  // Bouml preserved body end 0003D58D
}

void ZoomingShiftingComponent::startDragOrigin(const MouseEvent& e) {
  // Bouml preserved body begin 0003D60D
	dragger.startDraggingComponent(&origin,e);
  // Bouml preserved body end 0003D60D
}

void ZoomingShiftingComponent::zoom(float multX, float multY, float x, float y) {
  // Bouml preserved body begin 0003AF8D

	float newZoomRatioX = multX*zoomFactorX;
	float newZoomRatioY = multY*zoomFactorY;

	double grownTop = 0;
	double grownLeft = 0;



	if(newZoomRatioX >= 0.1 && newZoomRatioX <= 10)
	{
		x = origin.getX() - x;
		grownLeft = (double)x * (newZoomRatioX-zoomFactorX)/zoomFactorY  ;
		zoomFactorX = newZoomRatioX;
	}
	if(newZoomRatioY >= 0.1 && newZoomRatioY <= 10)
	{
		y = origin.getY() - y;
		grownTop = (double)y * (newZoomRatioY-zoomFactorY)/zoomFactorY ;
		zoomFactorY = newZoomRatioY;
	}

	origin.setTopLeftPosition(roundToInt((double)origin.getX() + grownLeft),
		                      roundToInt((double)origin.getY() + grownTop));

  // Bouml preserved body end 0003AF8D
}

//drag the origin around
//call the reArrangeChildren
void ZoomingShiftingComponent::shift(const juce::Point<int> newPosition) {
  // Bouml preserved body begin 0003B10D
	origin.setTopLeftPosition(newPosition.getX(),newPosition.getY());
  // Bouml preserved body end 0003B10D
}

//call all childrens reposition method
void ZoomingShiftingComponent::rePositionChildren() {
  // Bouml preserved body begin 0003B00D
	for(int i = 0 ; i < zoomedComponents.size() ; i++)
	{
	zoomedComponents[i]->rePosition();
	}
  // Bouml preserved body end 0003B00D
}

float ZoomingShiftingComponent::getXOffset() {
  // Bouml preserved body begin 0003B18D
	return (float)origin.getX();
  // Bouml preserved body end 0003B18D
}

float ZoomingShiftingComponent::getYOffset() {
  // Bouml preserved body begin 0003B20D
	return (float)origin.getY();
  // Bouml preserved body end 0003B20D
}

float ZoomingShiftingComponent::getZoomFactorX() {
  // Bouml preserved body begin 0003CD0D
	return this->zoomFactorX;
  // Bouml preserved body end 0003CD0D
}

float ZoomingShiftingComponent::getZoomFactorY() {
  // Bouml preserved body begin 0003E08D
	return this->zoomFactorY;
  // Bouml preserved body end 0003E08D
}

void ZoomingShiftingComponent::childBoundsChanged(juce::Component * component) {
  // Bouml preserved body begin 0003D50D
	if(component == &origin)
	{
		this->rePositionChildren();
	}
  // Bouml preserved body end 0003D50D
}

ZoomingShiftingComponent::ZoomingShiftingComponent() {
  // Bouml preserved body begin 0003D68D
	addAndMakeVisible(&origin);
	zoomFactorX =zoomFactorY= 1;
  // Bouml preserved body end 0003D68D
}

void ZoomingShiftingComponent::deleteAllZoomedComps() {
  // Bouml preserved body begin 0003D88D
	for (int i = 0 ; i < zoomedComponents.size() ; i++)
	{
		removeChildComponent(zoomedComponents[i]);
	}
	zoomedComponents.clear(true);
  // Bouml preserved body end 0003D88D
}

void ZoomingShiftingComponent::addZoomedComp(ZoomableShiftableComponent * zsComp, bool doZoom) {
  // Bouml preserved body begin 0003D90D
	addAndMakeVisible(zsComp);
	zsComp->setZoomer(this);
	this->zoomedComponents.add(zsComp);
	zsComp->rePosition();
  // Bouml preserved body end 0003D90D
}

void ZoomingShiftingComponent::removeZoomedComp(ZoomableShiftableComponent * comp) {
  // Bouml preserved body begin 0003F08D
	removeChildComponent(comp);
	zoomedComponents.remove(zoomedComponents.indexOf(comp),true);
  // Bouml preserved body end 0003F08D
}
