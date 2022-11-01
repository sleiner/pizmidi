#include "Controller.h"

#include "ControllerValue.h"
#include "MidiMorph.h"
#include "Scene.h"

using juce::roundToInt;

Controller::Controller(MidiMorph* core)
{
    this->core         = core;
    this->channel      = 1;
    this->ccNo         = 0;
    this->valueChanged = true;
    this->newMidi      = true;
}

Controller::~Controller()
{
    for (int i = 0; i < this->values.size(); i++)
    {
        delete values[i];
    }
    removeAllChangeListeners();
}

ControllerValue* Controller::getValue(int index)
{
    return values[index];
}

ControllerValue* Controller::getValue(const Scene* scene)
{
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i]->scene == scene)
        {
            return values[i];
        }
    }
    return nullptr;
}

void Controller::setValue(int newValue, Scene* scene)
{
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i]->scene == scene)
        {
            values[i]->setValue(newValue);
            core->controllerChanged(this);
            controllerChanged();
            return;
        }
    }
    auto* newControllerValue = new ControllerValue(this, scene);
    newControllerValue->setValue(newValue);
    values.add(newControllerValue);
    scene->addValue(newControllerValue);
    controllerChanged();
}

void Controller::controllerChanged()
{
    this->valueChanged = true;
    this->newMidi      = true;
    this->core->controllerChanged(this);
}

int Controller::getState()
{
    if (core->getNumSelectedScenes() == 0)
    {
        return Controller::undefined;
    }
    else if (core->getNumSelectedScenes() == 1)
    {
        if (core->getSelectedScene(0)->getValue(this) == -1)
        {
            return Controller::undefined;
        }
        else
        {
            return Controller::defined;
        }
    }
    else if (core->getNumSelectedScenes() > 1)
    {
        bool defined = false;
        int lastVal  = 0;

        for (int i = 0; i < this->core->getNumSelectedScenes(); i++)
        {
            if (core->getSelectedScene(i)->getValue(this) != lastVal && i > 0)
            {
                return Controller::mutival;
            }
            lastVal = core->getSelectedScenes()->operator[](i)->getValue(this);
            if (lastVal != -1)
            {
                defined = true;
            }
        }
        if (defined)
        {
            return Controller::defined;
        }
        else
        {
            return Controller::undefined;
        }
    }
    return Controller::undefined;
}

int Controller::getInterpolatedValue()
{
    if (valueChanged || core->hasNewMidi())
    {
        float tmp = 0;
        Scene* scene;
        for (int i = 0; i < values.size(); i++)
        {
            scene = values[i]->getScene();
            tmp += scene->getValue(this) * scene->getAffectionRatio();
        }
        valueChanged = false;
        value        = roundToInt(tmp);
    }
    return value;
}

void Controller::addValue(ControllerValue* valueToAdd)
{
    this->values.add(valueToAdd);
}

void Controller::learn()
{
}

void Controller::removeValue(ControllerValue* valueToRemove)
{
    this->values.removeAllInstancesOf(valueToRemove);
}

int Controller::getNumValues()
{
    return this->values.size();
}

void Controller::getMidiMessage(juce::MidiBuffer& buffer, int pos)
{
    {
        buffer.addEvent(juce::MidiMessage::controllerEvent(channel, ccNo, getInterpolatedValue()), pos);
    }
}

void Controller::remove()
{
    core->removeController(this);
}

bool Controller::hasNewMidi()
{
    bool result = newMidi || core->needsRefresh();
    newMidi     = false;
    return result;
}

int Controller::getCcNo() const
{
    return ccNo;
}

void Controller::setCcNo(int val)
{
    this->ccNo = val;
}

int Controller::getChannel() const
{
    return channel;
}

void Controller::setChannel(int channelToSet)
{
    this->channel = channelToSet;
}

juce::String Controller::getName()
{
    return this->name;
}

void Controller::setName(juce::String nameToSet)
{
    this->name = nameToSet;
}

void Controller::setValueInSelectedScenes(int newValue)
{
    for (int i = core->getNumSelectedScenes(); --i >= 0;)
    {
        setValue(newValue, core->getSelectedScene(i));
    }
    core->sendChangeMessage(this);

    if (core->getNumSelectedScenes() == 0 && core->autoKey)
    {
        core->addSceneAtCursor();
    }
}
