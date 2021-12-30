#include "ControllerValue.h"

#include "juce_audio_basics/juce_audio_basics.h"

#include "Controller.h"
#include "Scene.h"

ControllerValue::ControllerValue(Controller * controller, Scene * scene) {
  // Bouml preserved body begin 00035A8D
	this->scene = scene;
	this->controller = controller;
  // Bouml preserved body end 00035A8D
}

//at destruction make shure to deletete reference in controller AND scene!!
ControllerValue::~ControllerValue() {
  // Bouml preserved body begin 00035B0D
	scene->controllerValues.removeAllInstancesOf(this);
	controller->removeValue(this);
  // Bouml preserved body end 00035B0D
}

void ControllerValue::setValue(int newValue) {
  // Bouml preserved body begin 0003C50D
	this->value = newValue;
  // Bouml preserved body end 0003C50D
}

int ControllerValue::getValue() {
  // Bouml preserved body begin 0003C58D
	return value;
  // Bouml preserved body end 0003C58D
}

float ControllerValue::getValueRation() {
  // Bouml preserved body begin 0003C70D
	return   scene->getAffectionRatio() * (float)value;
  // Bouml preserved body end 0003C70D
}

Scene* ControllerValue::getScene() {
  // Bouml preserved body begin 0004068D
	return scene;
  // Bouml preserved body end 0004068D
}

Controller* ControllerValue::getController() {
  // Bouml preserved body begin 0004090D
	return controller;
  // Bouml preserved body end 0004090D
}

void ControllerValue::getMidiMessage(juce::MidiBuffer & buffer, int pos) {
  // Bouml preserved body begin 00048E0D
	if(controller->hasNewMidi())
	{
		buffer.addEvent(MidiMessage::controllerEvent(controller->getChannel(),controller->getCcNo(),value),pos);
	}
  // Bouml preserved body end 00048E0D
}
