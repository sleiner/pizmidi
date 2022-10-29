#include "ControllerValue.h"

#include <juce_audio_basics/juce_audio_basics.h>

#include "Controller.h"
#include "Scene.h"

ControllerValue::ControllerValue (Controller* controller, Scene* scene)
{
    this->scene      = scene;
    this->controller = controller;
}

ControllerValue::~ControllerValue()
{
    scene->controllerValues.removeAllInstancesOf (this);
    controller->removeValue (this);
}

void ControllerValue::setValue (int newValue)
{
    this->value = newValue;
}

int ControllerValue::getValue()
{
    return value;
}

Scene* ControllerValue::getScene()
{
    return scene;
}

Controller* ControllerValue::getController()
{
    return controller;
}

void ControllerValue::getMidiMessage (juce::MidiBuffer& buffer, int pos)
{
    if (controller->hasNewMidi())
    {
        buffer.addEvent (juce::MidiMessage::controllerEvent (controller->getChannel(), controller->getCcNo(), value), pos);
    }
}
