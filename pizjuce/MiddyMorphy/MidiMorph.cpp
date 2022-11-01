
#include "MidiMorph.h"

#include "ControllerValue.h"
#include "Cursor.h"

using juce::jmax;
using juce::roundToInt;

MidiMorph::MidiMorph()
{
    cursor        = new Cursor(this);
    valuesChanged = false;
    refreshRate   = 200;
    guiBounds.setSize(450, 300);
    guiState = 0;
    paneSize.setSize(300, 300);
    autoKey             = true;
    autoLearn           = true;
    controllerListWidth = 150;
}

MidiMorph::~MidiMorph()
{
    if (guiState != nullptr)
    {
        deleteAndZero(guiState);
    }
    deleteAndZero(cursor);
}

void MidiMorph::setCursorXRatio(float x)
{
    cursor->setXY((float) roundToInt(x * paneSize.getWidth()) - cursor->size / 2, cursor->getY());
}

void MidiMorph::setCursorYRatio(float y)
{
    cursor->setXY(cursor->getX(), (float) roundToInt(y * paneSize.getHeight()) - cursor->size / 2);
}

float MidiMorph::getCursorXRatio()
{
    return (cursor->getX() + cursor->size / 2) / paneSize.getWidth();
}

float MidiMorph::getCursorYRatio()
{
    return (cursor->getY() + cursor->size / 2) / paneSize.getHeight();
}

void MidiMorph::controllerChanged(const Controller* controllerThatHasChanged)
{
    this->valueChanged = true;
}

void MidiMorph::distancesChanged()
{
    this->sumDistancesChanged = true;
    if (! auditSelScene || selectedScenes.size() == 0)
    {
        this->valuesChanged = true;
    }
    this->sendChangeMessage(cursor);
}

void MidiMorph::cursorChanged()
{
    distancesChanged();
    for (int i = 0; i < scenes.size(); i++)
    {
        scenes[i]->distanceFromCursorChanged();
    }
}

void MidiMorph::addController(int ccNo, int Channel)
{
    auto* nc = new Controller(this);
    nc->setCcNo(ccNo);
    nc->setChannel(Channel);
    nc->setName(juce::MidiMessage::getControllerName(ccNo));

    controllers.add(nc);
    sendChangeMessage(&controllers);
}

void MidiMorph::addController()
{
    if (controllers.size() > 0)
    {
        addController(controllers.getLast()->getCcNo() + 1, controllers.getLast()->getChannel());
    }
    else
    {
        addController(0, 1);
    }
}

void MidiMorph::addScene(Scene* scene)
{
    scene->id = scenes.size();
    scenes.add(scene);
    sendChangeMessage(&scenes);

    juce::Array<Scene*> selScenes(&scene, 1);
    setSelectedScenes(selScenes);
}

int MidiMorph::getNumControllers()
{
    return controllers.size();
}

int MidiMorph::getControllerCCNo(int index)
{
    return controllers[index]->getCcNo();
}

int MidiMorph::getControllerValue(int index)
{
    return controllers[index]->getInterpolatedValue();
}

void MidiMorph::onMidiEvent(juce::MidiMessage& events)
{
    bool controllerIsInList = false;

    if (events.isController())
    {
        for (int i = 0; i < controllers.size(); i++)
        {
            if (controllers[i]->getCcNo() == events.getControllerNumber()
                && controllers[i]->getChannel() == events.getChannel())
            {
                controllers[i]->setValueInSelectedScenes(events.getControllerValue());
                controllerIsInList = true;
            }
        }

        if (! controllerIsInList && autoLearn)
        {
            this->addController(events.getControllerNumber(), events.getChannel());
            onMidiEvent(events);
        }
    }
}

void MidiMorph::getMidiMessages(int offset, juce::MidiBuffer& buffer)
{
    if (auditSelScene && selectedScenes.size() > 0 && hasNewMidi())
    {
        selectedScenes.getFirst()->getMidiMessages(buffer, offset);
        valueChanged  = false;
        valuesChanged = false;
    }
    else
    {
        for (int i = controllers.size(); --i >= 0;)
        {
            if (! valueChanged)
            {
                controllers[i]->getMidiMessage(buffer, offset);
            }
            else if (controllers[i]->hasNewMidi())
            {
                controllers[i]->getMidiMessage(buffer, offset);
            }
        }
        valueChanged  = false;
        valuesChanged = false;
    }
}

float MidiMorph::getSumDistances()
{
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
}

Controller* MidiMorph::getController(int index)
{
    return controllers[index];
}

int MidiMorph::getNumSelectedScenes()
{
    return selectedScenes.size();
}

void MidiMorph::setSelectedScenes(juce::Array<Scene*>& scenes)
{
    this->selectedScenes.swapWith(scenes);
    if (auditSelScene && selectedScenes.size() == 1)
    {
        valuesChanged = true;
    }
    sendChangeMessage(&selectedScenes);
}

juce::Array<Scene*>* MidiMorph::getSelectedScenes()
{
    return &this->selectedScenes;
}

Scene* MidiMorph::getSelectedScene(int index)
{
    return selectedScenes[index];
}

Cursor* MidiMorph::getCursor()
{
    return cursor;
}

float MidiMorph::getSumAffectionValues()
{
    float sum = 0;
    for (int i = 0; i < scenes.size(); i++)
    {
        sum += scenes[i]->getAffectionValue();
    }
    return sum;
}

void MidiMorph::removeScene(Scene* scene)
{
    scenes.getLock().enter();
    scenes.removeObject(scene);
    for (int i = scenes.size(); --i >= 0;)
    {
        scenes[i]->id = i;
    }
    scenes.getLock().exit();
    sendChangeMessage(&scenes);
}

juce::OwnedArray<Controller>* MidiMorph::getControllers()
{
    return &controllers;
}

juce::XmlElement* MidiMorph::getXml(const juce::String tagname)
{
    auto* xml = new juce::XmlElement(tagname);

    auto* controllersXml = new juce::XmlElement("controllers");
    juce::XmlElement* controllerXml;
    for (int i = 0; i < this->controllers.size(); i++)
    {
        Controller* controller = controllers[i];
        controllerXml          = new juce::XmlElement("controller");
        juce::XmlElement* valueXml;
        for (int i = 0; i < controller->getNumValues(); i++)
        {
            ControllerValue* value = controller->getValue(i);
            valueXml               = new juce::XmlElement("value");
            valueXml->setAttribute("value", value->getValue());
            valueXml->setAttribute("scene", value->getScene()->getId());
            controllerXml->addChildElement(valueXml);
        }
        controllerXml->setAttribute("ccno", controller->getCcNo());
        controllerXml->setAttribute("channel", controller->getChannel());
        controllerXml->setAttribute("name", controller->getChannel());
        controllersXml->addChildElement(controllerXml);
    }

    auto* scenesXml = new juce::XmlElement("scenes");
    juce::XmlElement* sceneXml;
    for (int i = 0; i < this->scenes.size(); i++)
    {
        Scene* scene = scenes[i];
        sceneXml     = new juce::XmlElement("scene");
        sceneXml->setAttribute("id", i);
        sceneXml->setAttribute("name", scene->getName());
        sceneXml->setAttribute("colour", scene->getColour().toString());
        sceneXml->setAttribute("x", scene->Point::getX());
        sceneXml->setAttribute("y", scene->Point::getY());
        sceneXml->setAttribute("size", scene->size);

        scenesXml->addChildElement(sceneXml);
    }

    auto* gui = new juce::XmlElement("gui");
    gui->setAttribute("width", this->guiBounds.getWidth());
    gui->setAttribute("height", this->guiBounds.getHeight());
    gui->setAttribute("listwidth", this->controllerListWidth);

    auto* cursorXml = new juce::XmlElement("cursor");
    cursorXml->setAttribute("x", cursor->Point::getX());
    cursorXml->setAttribute("y", cursor->Point::getY());

    auto* options = new juce::XmlElement("options");
    options->setAttribute("autokey", this->autoKey);
    options->setAttribute("autolearn", this->autoLearn);
    options->setAttribute("auditselscene", getAuditSelScene());
    options->setAttribute("refreshrate", this->refreshRate);

    xml->addChildElement(controllersXml);
    xml->addChildElement(scenesXml);
    xml->addChildElement(gui);
    xml->addChildElement(guiState);
    xml->addChildElement(cursorXml);
    xml->addChildElement(options);

    return xml;
}

void MidiMorph::setFromXml(juce::XmlElement* xmlData)
{
    scenes.clear();
    controllers.clear();

    juce::XmlElement* scenesXml = xmlData->getChildByName("scenes");
    for (int i = 0; i < scenesXml->getNumChildElements(); i++)
    {
        juce::XmlElement* sceneXml = scenesXml->getChildElement(i);
        auto* scene                = new Scene(this);
        scene->setXY((float) sceneXml->getIntAttribute("x"), (float) sceneXml->getIntAttribute("y"));
        scene->setColour(juce::Colour::fromString(sceneXml->getStringAttribute("colour")));
        scene->setName(sceneXml->getStringAttribute("name"));
        scene->size = sceneXml->getIntAttribute("size", scene->size);
        scenes.add(scene);
    }

    juce::XmlElement* controllersXml = xmlData->getChildByName("controllers");
    for (int i = 0; i < controllersXml->getNumChildElements(); i++)
    {
        Controller* controller;
        juce::XmlElement* controllerXml = controllersXml->getChildElement(i);
        controller                      = new Controller(this);
        controller->setCcNo(controllerXml->getIntAttribute("ccno"));
        controller->setChannel(controllerXml->getIntAttribute("channel"));

        for (int j = 0; j < controllerXml->getNumChildElements(); j++)
        {
            juce::XmlElement* valueXml = controllerXml->getChildElement(j);
            int cValue                 = valueXml->getIntAttribute("value");
            int sceneId                = valueXml->getIntAttribute("scene");
            Scene* scene               = scenes[sceneId];
            auto* value                = new ControllerValue(controller, scene);
            value->setValue(cValue);
            controller->addValue(value);
            scene->addValue(value);
        }

        controllers.add(controller);
    }

    juce::XmlElement* cursorXml = xmlData->getChildByName("cursor");
    if (xmlData->containsChildElement(cursorXml))
    {
        cursor->setXY((float) cursorXml->getDoubleAttribute("x"), (float) cursorXml->getDoubleAttribute("y"));
    }

    this->guiState = xmlData->getChildByName("morphpanestate");

    juce::XmlElement* gui = xmlData->getChildByName("gui");
    this->guiBounds.setSize(gui->getIntAttribute("width"), gui->getIntAttribute("height"));
    this->controllerListWidth = gui->getIntAttribute("listwidth", this->controllerListWidth);

    juce::XmlElement* options = xmlData->getChildByName("options");
    if (xmlData->containsChildElement(options))
    {
        this->autoKey   = options->getBoolAttribute("autokey", this->autoKey);
        this->autoLearn = options->getBoolAttribute("autolearn", this->autoLearn);
        setAuditSelScene(options->getBoolAttribute("auditselscene", this->auditSelScene));
        this->refreshRate = options->getIntAttribute("refreshrate", this->refreshRate);
    }

    sendChangeMessage(&controllers);
    sendChangeMessage(&scenes);
}

int MidiMorph::getSceneIndex(Scene* scene)
{
    return scenes.indexOf(scene);
}

int MidiMorph::getUpdateRateSmpls(int rate)
{
    return jmax(1, (rate / refreshRate));
}

void MidiMorph::removeController(Controller* controllerToRemove)
{
    controllers.getLock().enter();
    for (int i = scenes.size(); --i >= 0;)
    {
        scenes[i]->controllerValues.removeAllInstancesOf(controllerToRemove->getValue(scenes[i]));
    }
    this->controllers.removeObject(controllerToRemove);
    controllers.getLock().exit();

    sendChangeMessage(&controllers);
}

void MidiMorph::saveGUIState(juce::XmlElement* state)
{
    if (guiState != nullptr)
    {
        deleteAndZero(guiState);
    }
    this->guiState = state;
}

juce::XmlElement* MidiMorph::getSavedGUIState()
{
    return guiState;
}

void MidiMorph::setPaneSize(juce::Rectangle<int> size)
{
    paneSize = size;
}

juce::Rectangle<int> MidiMorph::getPaneSize()
{
    return paneSize;
}

void MidiMorph::addSceneAtCursor()
{
    auto* scene = new Scene(this);
    scene->setXY(cursor->getCursorPosition().getX(), cursor->getCursorPosition().getY());
    addScene(scene);
}

int MidiMorph::getRefreshRate()
{
    return this->refreshRate;
}

bool MidiMorph::hasNewMidi()
{
    return (valuesChanged || valueChanged);
}

juce::OwnedArray<Scene>* MidiMorph::getScenes()
{
    return &scenes;
}

void MidiMorph::sendChangeMessage(void* ptr)
{
    if (ptr != lastRecipant)
    {
        juce::MessageManagerLock lock;
        dispatchPendingMessages();
    }
    currentChangeBroadcastRecipient = ptr;
    ChangeBroadcaster::sendChangeMessage();
    this->lastRecipant = ptr;
}

void MidiMorph::sceneMoved()
{
    if (! auditSelScene)
    {
        this->valuesChanged = true;
    }
    sumDistancesChanged = true;
}

void MidiMorph::setAuditSelScene(bool shouldAudit)
{
    auditSelScene = shouldAudit;
    valuesChanged = true;
}

bool MidiMorph::getAuditSelScene()
{
    return auditSelScene;
}

bool MidiMorph::needsRefresh()
{
    return valuesChanged;
}

bool MidiMorph::needsOneRefresh()
{
    return valueChanged;
}

void MidiMorph::showControllers(bool show)
{
    if (show)
    {
        controllerListWidth = 120;
    }
    else
    {
        controllerListWidth = 0;
    }

    sendChangeMessage(this);
}
