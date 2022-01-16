
#include "Controller.h"
#include "ControllerValue.h"
#include "MidiMorph.h"
#include "Scene.h"

Controller::Controller (MidiMorph* core)
{
    // Bouml preserved body begin 00034E0D
    this->core         = core;
    this->channel      = 1;
    this->ccNo         = 0;
    this->valueChanged = true;
    this->newMidi      = true;
    //this->midiMessage =  & MidiMessage::controllerEvent(channel,ccNo,64);
    // Bouml preserved body end 00034E0D
}

Controller::~Controller()
{
    // Bouml preserved body begin 00040B8D
    for (int i = 0; i < this->values.size(); i++)
    {
        delete values[i];
    }
    removeAllChangeListeners();
    // Bouml preserved body end 00040B8D
}

ControllerValue* Controller::getValue (int index)
{
    // Bouml preserved body begin 0004080D
    return values[index];
    // Bouml preserved body end 0004080D
}

ControllerValue* Controller::getValue (const Scene* scene)
{
    // Bouml preserved body begin 0002FB0D
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i]->scene == scene)
        {
            return values[i];
        }
    }
    return 0;
    // Bouml preserved body end 0002FB0D
}

//set value for particular scene
void Controller::setValue (int newValue, Scene* scene)
{
    // Bouml preserved body begin 00034C8D
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i]->scene == scene)
        {
            values[i]->setValue (newValue);
            core->controllerChanged (this);
            controllerChanged();
            return;
        }
    }
    ControllerValue* value = new ControllerValue (this, scene);
    value->setValue (newValue);
    values.add (value);
    scene->addValue (value);
    controllerChanged();

    // Bouml preserved body end 00034C8D
}

//refresh value of selected scenes
void Controller::controllerChanged()
{
    // Bouml preserved body begin 0003598D
    this->valueChanged = true;
    this->newMidi      = true;
    this->core->controllerChanged (this);
    // Bouml preserved body end 0003598D
}

int Controller::getState()
{
    // Bouml preserved body begin 0003E98D

    if (core->getNumSelectedScenes() == 0)
    {
        return Controller::undefined;
    }
    else if (core->getNumSelectedScenes() == 1)
    {
        if (core->getSelectedScene (0)->getValue (this) == -1)
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
        //bool multiple = false;
        int lastVal = 0;
        for (int i = 0; i < this->core->getNumSelectedScenes(); i++)
        {
            if (core->getSelectedScene (i)->getValue (this) != lastVal && i > 0)
            {
                return Controller::mutival;
            }
            lastVal = core->getSelectedScenes()->operator[] (i)->getValue (this);
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
    // Bouml preserved body end 0003E98D
}

int Controller::getInterpolatedValue()
{
    // Bouml preserved body begin 0003518D
    if (valueChanged || core->hasNewMidi())
    {
        float tmp = 0;
        Scene* scene;
        for (int i = 0; i < values.size(); i++)
        {
            scene = values[i]->getScene();
            tmp += scene->getValue (this) * scene->getAffectionRatio();
        }
        valueChanged = false;
        value        = roundToInt (tmp);
    }
    return value;
    // Bouml preserved body end 0003518D
}

void Controller::addValue (ControllerValue* value)
{
    // Bouml preserved body begin 0004098D
    this->values.add (value);
    // Bouml preserved body end 0004098D
}

void Controller::learn()
{
    // Bouml preserved body begin 0003100D
    // Bouml preserved body end 0003100D
}

void Controller::removeValue (ControllerValue* value)
{
    // Bouml preserved body begin 0003FA8D
    this->values.removeAllInstancesOf (value);
    // Bouml preserved body end 0003FA8D
}

int Controller::getNumValues()
{
    // Bouml preserved body begin 0004050D
    return this->values.size();
    // Bouml preserved body end 0004050D
}

void Controller::getMidiMessage (juce::MidiBuffer& buffer, int pos)
{
    // Bouml preserved body begin 0004108D
    /*if( newMidi )
	{*/
    //if( has)
    {
        buffer.addEvent (MidiMessage::controllerEvent (channel, ccNo, getInterpolatedValue()), pos);

        //lastSentValue =
        //this->lastSentChannel = channel
    }
    /*}*/
    //MidiMessage* m = new MidiMessage();
    //this->newMidi = false;
    //return *m;
    // Bouml preserved body end 0004108D
}

void Controller::remove()
{
    // Bouml preserved body begin 0004150D
    core->removeController (this);
    // Bouml preserved body end 0004150D
}

bool Controller::hasNewMidi()
{
    // Bouml preserved body begin 0004218D

    bool result = newMidi || core->needsRefresh();
    newMidi     = false;
    return result;

    // Bouml preserved body end 0004218D
}

int Controller::getCcNo()
{
    // Bouml preserved body begin 0004220D
    return ccNo;
    // Bouml preserved body end 0004220D
}

void Controller::setCcNo (int val)
{
    // Bouml preserved body begin 0004228D
    this->ccNo = val;
    // Bouml preserved body end 0004228D
}

int Controller::getChannel()
{
    // Bouml preserved body begin 0004230D
    return channel;
    // Bouml preserved body end 0004230D
}

void Controller::setChannel (int channel)
{
    // Bouml preserved body begin 0004238D
    this->channel = channel;
    // Bouml preserved body end 0004238D
}

String Controller::getName()
{
    // Bouml preserved body begin 0004288D
    return this->name;
    // Bouml preserved body end 0004288D
}

void Controller::setName (String name)
{
    // Bouml preserved body begin 0004290D
    this->name = name;
    // Bouml preserved body end 0004290D
}

//set value for selected Scenes
void Controller::setValue (int newValue)
{
    // Bouml preserved body begin 00047C8D

    for (int i = core->getNumSelectedScenes(); --i >= 0;)
    {
        setValue (newValue, core->getSelectedScene (i));
    }
    core->sendChangeMessage (this);
    if (core->getNumSelectedScenes() == 0 && core->autoKey)
    {
        core->addSceneAtCursor();
    }

    // Bouml preserved body end 00047C8D
}
