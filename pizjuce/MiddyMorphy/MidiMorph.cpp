
#include "MidiMorph.h"
#include "ControllerValue.h"
#include "Cursor.h"

MidiMorph::MidiMorph()
{
    // Bouml preserved body begin 0003540D
    cursor        = new Cursor (this);
    valuesChanged = false;
    refreshRate   = 200;
    guiBounds.setSize (450, 300);
    guiState = 0;
    paneSize.setSize (300, 300);
    autoKey             = true;
    autoLearn           = true;
    controllerListWidth = 150;
    // Bouml preserved body end 0003540D
}

MidiMorph::~MidiMorph()
{
    // Bouml preserved body begin 00041F8D
    if (guiState != 0)
    {
        deleteAndZero (guiState);
    }
    deleteAndZero (cursor);
    // Bouml preserved body end 00041F8D
}

void MidiMorph::setCursorXRatio (float x)
{
    // Bouml preserved body begin 00042D8D
    cursor->setXY ((float) roundToInt (x * paneSize.getWidth()) - cursor->size / 2, cursor->Point::getY());
    // Bouml preserved body end 00042D8D
}

void MidiMorph::setCursorYRatio (float y)
{
    // Bouml preserved body begin 00042E0D
    cursor->setXY (cursor->Point::getX(), (float) roundToInt (y * paneSize.getHeight()) - cursor->size / 2);
    // Bouml preserved body end 00042E0D
}

float MidiMorph::getCursorXRatio()
{
    // Bouml preserved body begin 00042E8D
    return (cursor->Point::getX() + cursor->size / 2) / paneSize.getWidth();
    // Bouml preserved body end 00042E8D
}

float MidiMorph::getCursorYRatio()
{
    // Bouml preserved body begin 00042F0D
    return (cursor->Point::getY() + cursor->size / 2) / paneSize.getHeight();
    // Bouml preserved body end 00042F0D
}

//-can happen by input or slider movement.
//
//-if a scene is selected, (or multiple scenes) the value for that controller gets set.
//-if no scene is selected and auto key is on, a scene gets added.
void MidiMorph::controllerChanged (const Controller* controllerThatHasChanged)
{
    // Bouml preserved body begin 0003538D
    this->valueChanged = true;
    // Bouml preserved body end 0003538D
}

//send interpolated controller vaules  that actually differ from the last output to midi out
void MidiMorph::distancesChanged()
{
    // Bouml preserved body begin 00034E8D
    this->sumDistancesChanged = true;
    if (! auditSelScene || selectedScenes.size() == 0)
    {
        this->valuesChanged = true;
    }
    this->sendChangeMessage (cursor);

    // Bouml preserved body end 00034E8D
}

//foreach scene : moved();
void MidiMorph::cursorChanged()
{
    // Bouml preserved body begin 00035A0D
    distancesChanged();
    for (int i = 0; i < scenes.size(); i++)
    {
        scenes[i]->distanceFromCursorChanged();
    }
    // Bouml preserved body end 00035A0D
}

void MidiMorph::addController (int ccNo, int Channel)
{
    // Bouml preserved body begin 00034F0D
    Controller* nc = new Controller (this);
    nc->setCcNo (ccNo);
    nc->setChannel (Channel);
    nc->setName (MidiMessage::getControllerName (ccNo));

    controllers.add (nc);
    sendChangeMessage (&controllers);

    // Bouml preserved body end 00034F0D
}

void MidiMorph::addController()
{
    // Bouml preserved body begin 0004800D
    if (controllers.size() > 0)
    {
        addController (controllers.getLast()->getCcNo() + 1, controllers.getLast()->getChannel());
    }
    else
    {
        addController (0, 1);
    }
    // Bouml preserved body end 0004800D
}

void MidiMorph::addScene (Scene* scene)
{
    // Bouml preserved body begin 0003F98D
    scene->id = scenes.size();
    scenes.add (scene);
    sendChangeMessage (&scenes);

    Array<Scene*> selScenes (&scene, 1);
    setSelectedScenes (selScenes);
    //OwnedArray<Scene>* scenes = getScenes();

    // Bouml preserved body end 0003F98D
}

int MidiMorph::getNumControllers()
{
    // Bouml preserved body begin 00035B8D
    return controllers.size();
    // Bouml preserved body end 00035B8D
}

int MidiMorph::getControllerCCNo (int index)
{
    // Bouml preserved body begin 00035C0D
    return controllers[index]->getCcNo();
    // Bouml preserved body end 00035C0D
}

int MidiMorph::getControllerValue (int index)
{
    // Bouml preserved body begin 00035C8D
    return controllers[index]->getInterpolatedValue();
    // Bouml preserved body end 00035C8D
}

//if controller != cursor controller
//if   controller is in list
//	if - scene/s is selected, .> set/add value for selected scenes/s
//	else if  "autokey" is on, ->add scene at cursor pos, select scene, set/add value for cursor,
//
//else if "autolearn" is on add controller.
//		if - scene/s is selected, .> set/add value for selected scenes/s
//		else if no scene is selected && "autokey" is on, ->add scene at cursor pos, select scene, set/add value for cursor
void MidiMorph::onMidiEvent (juce::MidiMessage& events)
{
    // Bouml preserved body begin 00035D8D
    bool controllerIsInList = false;
    if (events.isController())
    {
        for (int i = 0; i < controllers.size(); i++)
        {
            if (controllers[i]->getCcNo() == events.getControllerNumber()
                && controllers[i]->getChannel() == events.getChannel())
            {
                controllers[i]->setValue (events.getControllerValue());
                controllerIsInList = true;
            }
        }
        if (! controllerIsInList && autoLearn)
        {
            this->addController (events.getControllerNumber(), events.getChannel());
            onMidiEvent (events);
        }
    }
    // Bouml preserved body end 00035D8D
}

void MidiMorph::getMidiMessages (int offset, juce::MidiBuffer& buffer)
{
    // Bouml preserved body begin 0004100D
    if (auditSelScene && selectedScenes.size() > 0 && hasNewMidi())
    {
        selectedScenes.getFirst()->getMidiMessages (buffer, offset);
        valueChanged  = false;
        valuesChanged = false;
    }
    else
    {
        for (int i = controllers.size(); --i >= 0;)
        {
            if (! valueChanged)
            {
                controllers[i]->getMidiMessage (buffer, offset);
            }
            else if (controllers[i]->hasNewMidi())
            {
                controllers[i]->getMidiMessage (buffer, offset);
            }
        }
        valueChanged  = false;
        valuesChanged = false;
    }

    // Bouml preserved body end 0004100D
}

float MidiMorph::getSumDistances()
{
    // Bouml preserved body begin 0003C60D
    if (sumDistancesChanged)
    {
        sumDistancesChanged = false;
        sumDistances        = 0;
        for (int i = 0; i < scenes.size(); i++)
        {
            sumDistances += scenes[i]->getDistanceFromCursor();
        }
    }
    return sumDistances;
    // Bouml preserved body end 0003C60D
}

Controller* MidiMorph::getController (int index)
{
    // Bouml preserved body begin 0003E60D
    return controllers[index];
    // Bouml preserved body end 0003E60D
}

int MidiMorph::getNumSelectedScenes()
{
    // Bouml preserved body begin 0003E68D
    return selectedScenes.size();
    // Bouml preserved body end 0003E68D
}

void MidiMorph::setSelectedScenes (Array<Scene*>& scenes)
{
    // Bouml preserved body begin 0003E80D

    this->selectedScenes.swapWith (scenes);
    if (auditSelScene && selectedScenes.size() == 1)
    {
        valuesChanged = true;
    }
    sendChangeMessage (&selectedScenes);
    // Bouml preserved body end 0003E80D
}

Array<Scene*>* MidiMorph::getSelectedScenes()
{
    // Bouml preserved body begin 0003E90D
    return &this->selectedScenes;
    // Bouml preserved body end 0003E90D
}

Scene* MidiMorph::getSelectedScene (int index)
{
    // Bouml preserved body begin 0003EA0D
    return selectedScenes[index];
    // Bouml preserved body end 0003EA0D
}

Cursor* MidiMorph::getCursor()
{
    // Bouml preserved body begin 0003F28D
    return cursor;
    // Bouml preserved body end 0003F28D
}

float MidiMorph::getSumAffectionValues()
{
    // Bouml preserved body begin 0003F50D
    float sum = 0;
    for (int i = 0; i < scenes.size(); i++)
    {
        sum += scenes[i]->getAffectionValue();
    }
    return sum;
    // Bouml preserved body end 0003F50D
}

void MidiMorph::removeScene (Scene* scene)
{
    // Bouml preserved body begin 0003F90D
    //Array<Scene*> noSelection;
    //setSelectedScenes(noSelection);
    scenes.getLock().enter();
    scenes.removeObject (scene);
    for (int i = scenes.size(); --i >= 0;)
    {
        scenes[i]->id = i;
    }
    scenes.getLock().exit();
    sendChangeMessage (&scenes);
    // Bouml preserved body end 0003F90D
}

OwnedArray<Controller>* MidiMorph::getControllers()
{
    // Bouml preserved body begin 0003FA0D
    return &controllers;
    // Bouml preserved body end 0003FA0D
}

juce::XmlElement* MidiMorph::getXml (const juce::String tagname)
{
    // Bouml preserved body begin 0004040D
    XmlElement* xml = new XmlElement (tagname);

    XmlElement* controllersXml = new XmlElement ("controllers");
    XmlElement* controllerXml;
    for (int i = 0; i < this->controllers.size(); i++)
    {
        Controller* controller = controllers[i];
        controllerXml          = new XmlElement ("controller");
        XmlElement* valueXml;
        for (int i = 0; i < controller->getNumValues(); i++)
        {
            ControllerValue* value = controller->getValue (i);
            valueXml               = new XmlElement ("value");
            valueXml->setAttribute ("value", value->getValue());
            valueXml->setAttribute ("scene", value->getScene()->getId());
            controllerXml->addChildElement (valueXml);
        }
        controllerXml->setAttribute ("ccno", controller->getCcNo());
        controllerXml->setAttribute ("channel", controller->getChannel());
        controllerXml->setAttribute ("name", controller->getChannel());
        controllersXml->addChildElement (controllerXml);
    }

    XmlElement* scenesXml = new XmlElement ("scenes");
    XmlElement* sceneXml;
    for (int i = 0; i < this->scenes.size(); i++)
    {
        Scene* scene = scenes[i];
        sceneXml     = new XmlElement ("scene");
        sceneXml->setAttribute ("id", i);
        sceneXml->setAttribute ("name", scene->getName());
        sceneXml->setAttribute ("colour", scene->getColour().toString());
        sceneXml->setAttribute ("x", scene->Point::getX());
        sceneXml->setAttribute ("y", scene->Point::getY());
        sceneXml->setAttribute ("size", scene->size);

        scenesXml->addChildElement (sceneXml);
    }

    XmlElement* gui = new XmlElement ("gui");
    gui->setAttribute ("width", this->guiBounds.getWidth());
    gui->setAttribute ("height", this->guiBounds.getHeight());
    gui->setAttribute ("listwidth", this->controllerListWidth);

    XmlElement* cursorXml = new XmlElement ("cursor");
    cursorXml->setAttribute ("x", cursor->Point::getX());
    cursorXml->setAttribute ("y", cursor->Point::getY());

    XmlElement* options = new XmlElement ("options");
    options->setAttribute ("autokey", this->autoKey);
    options->setAttribute ("autolearn", this->autoLearn);
    options->setAttribute ("auditselscene", getAuditSelScene());
    options->setAttribute ("refreshrate", this->refreshRate);

    xml->addChildElement (controllersXml);
    xml->addChildElement (scenesXml);
    xml->addChildElement (gui);
    xml->addChildElement (guiState);
    xml->addChildElement (cursorXml);
    xml->addChildElement (options);

    return xml;
    // Bouml preserved body end 0004040D
}

void MidiMorph::setFromXml (juce::XmlElement* xmlData)
{
    // Bouml preserved body begin 0004048D
    scenes.clear();
    controllers.clear();

    XmlElement* scenesXml = xmlData->getChildByName ("scenes");
    for (int i = 0; i < scenesXml->getNumChildElements(); i++)
    {
        XmlElement* sceneXml = scenesXml->getChildElement (i);
        Scene* scene         = new Scene (this);
        scene->setXY ((float) sceneXml->getIntAttribute ("x"), (float) sceneXml->getIntAttribute ("y"));
        scene->setColour (Colour::fromString (sceneXml->getStringAttribute ("colour")));
        scene->setName (sceneXml->getStringAttribute ("name"));
        scene->size = sceneXml->getIntAttribute ("size", scene->size);
        scenes.add (scene);
    }

    XmlElement* controllersXml = xmlData->getChildByName ("controllers");
    for (int i = 0; i < controllersXml->getNumChildElements(); i++)
    {
        Controller* controller;
        XmlElement* controllerXml = controllersXml->getChildElement (i);
        controller                = new Controller (this);
        controller->setCcNo (controllerXml->getIntAttribute ("ccno"));
        controller->setChannel (controllerXml->getIntAttribute ("channel"));

        for (int j = 0; j < controllerXml->getNumChildElements(); j++)
        {
            XmlElement* valueXml   = controllerXml->getChildElement (j);
            int cValue             = valueXml->getIntAttribute ("value");
            int sceneId            = valueXml->getIntAttribute ("scene");
            Scene* scene           = scenes[sceneId];
            ControllerValue* value = new ControllerValue (controller, scene);
            value->setValue (cValue);
            controller->addValue (value);
            scene->addValue (value);
        }

        controllers.add (controller);
    }

    XmlElement* cursorXml = xmlData->getChildByName ("cursor");
    if (xmlData->containsChildElement (cursorXml))
    {
        cursor->setXY ((float) cursorXml->getDoubleAttribute ("x"), (float) cursorXml->getDoubleAttribute ("y"));
    }

    this->guiState = xmlData->getChildByName ("morphpanestate");

    XmlElement* gui = xmlData->getChildByName ("gui");
    this->guiBounds.setSize (gui->getIntAttribute ("width"), gui->getIntAttribute ("height"));
    this->controllerListWidth = gui->getIntAttribute ("listwidth", this->controllerListWidth);

    XmlElement* options = xmlData->getChildByName ("options");
    if (xmlData->containsChildElement (options))
    {
        this->autoKey   = options->getBoolAttribute ("autokey", this->autoKey);
        this->autoLearn = options->getBoolAttribute ("autolearn", this->autoLearn);
        setAuditSelScene (options->getBoolAttribute ("auditselscene", this->auditSelScene));
        this->refreshRate = options->getIntAttribute ("refreshrate", this->refreshRate);
    }

    sendChangeMessage (&controllers);
    sendChangeMessage (&scenes);

    // Bouml preserved body end 0004048D
}

int MidiMorph::getSceneIndex (Scene* scene)
{
    // Bouml preserved body begin 0004088D
    return scenes.indexOf (scene);
    // Bouml preserved body end 0004088D
}

int MidiMorph::getUpdateRateSmpls (int rate)
{
    // Bouml preserved body begin 00040A8D
    return jmax (1, (rate / refreshRate));
    // Bouml preserved body end 00040A8D
}

void MidiMorph::removeController (Controller* controllerToRemove)
{
    // Bouml preserved body begin 0004158D

    controllers.getLock().enter();
    for (int i = scenes.size(); --i >= 0;)
    {
        scenes[i]->controllerValues.removeAllInstancesOf (controllerToRemove->getValue (scenes[i]));
    }
    this->controllers.removeObject (controllerToRemove);
    controllers.getLock().exit();

    sendChangeMessage (&controllers);
    // Bouml preserved body end 0004158D
}

void MidiMorph::saveGUIState (juce::XmlElement* state)
{
    // Bouml preserved body begin 00041A8D
    if (guiState != 0)
    {
        deleteAndZero (guiState);
    }
    this->guiState = state;
    // Bouml preserved body end 00041A8D
}

juce::XmlElement* MidiMorph::getSavedGUIState()
{
    // Bouml preserved body begin 00041B0D
    return guiState;
    // Bouml preserved body end 00041B0D
}

void MidiMorph::setPaneSize (Rectangle<int> size)
{
    // Bouml preserved body begin 0004798D
    paneSize = size;
    // Bouml preserved body end 0004798D
}

Rectangle<int> MidiMorph::getPaneSize()
{
    // Bouml preserved body begin 00047A0D
    return paneSize;
    // Bouml preserved body end 00047A0D
}

void MidiMorph::addSceneAtCursor()
{
    // Bouml preserved body begin 00047D0D
    Scene* scene = new Scene (this);
    scene->setXY (cursor->getCursorPosition().getX(), cursor->getCursorPosition().getY());
    addScene (scene);

    // Bouml preserved body end 00047D0D
}

int MidiMorph::getRefreshRate()
{
    // Bouml preserved body begin 00047F8D
    return this->refreshRate;
    // Bouml preserved body end 00047F8D
}

bool MidiMorph::hasNewMidi()
{
    // Bouml preserved body begin 00048B8D
    return (valuesChanged || valueChanged);
    // Bouml preserved body end 00048B8D
}

OwnedArray<Scene>* MidiMorph::getScenes()
{
    // Bouml preserved body begin 00048C0D
    return &scenes;
    // Bouml preserved body end 00048C0D
}

void MidiMorph::sendChangeMessage (void* ptr)
{
    // Bouml preserved body begin 00048D0D
    if (ptr != lastRecipant)
    {
        MessageManagerLock lock;
        dispatchPendingMessages();
    }
    currentChangeBroadcastRecipient = ptr;
    ChangeBroadcaster::sendChangeMessage();
    this->lastRecipant = ptr;
    // Bouml preserved body end 00048D0D
}

void MidiMorph::sceneMoved()
{
    // Bouml preserved body begin 00048E8D
    if (! auditSelScene)
    {
        this->valuesChanged = true;
    }
    sumDistancesChanged = true;
    // Bouml preserved body end 00048E8D
}

void MidiMorph::setAuditSelScene (bool shouldAudit)
{
    // Bouml preserved body begin 00048F0D
    auditSelScene = shouldAudit;
    valuesChanged = true;
    // Bouml preserved body end 00048F0D
}

bool MidiMorph::getAuditSelScene()
{
    // Bouml preserved body begin 00048F8D
    return auditSelScene;

    // Bouml preserved body end 00048F8D
}

bool MidiMorph::needsRefresh()
{
    // Bouml preserved body begin 0004900D
    return valuesChanged;
    // Bouml preserved body end 0004900D
}

bool MidiMorph::needsOneRefresh()
{
    // Bouml preserved body begin 0004908D
    return valueChanged;
    // Bouml preserved body end 0004908D
}

void MidiMorph::showControllers (bool show)
{
    if (show)
        controllerListWidth = 120;
    else
        controllerListWidth = 0;
    sendChangeMessage (this);
}
