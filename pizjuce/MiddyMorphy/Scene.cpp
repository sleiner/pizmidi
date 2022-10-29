
#include "Scene.h"
#include "Controller.h"
#include "ControllerValue.h"
#include "Cursor.h"
#include "MidiMorph.h"
#include "ModuleGUI.h"

using juce::jmax;

Scene::Scene(const Scene& scene)
{
}

Scene::Scene(MidiMorph* core)
{
    this->core   = core;
    this->colour = juce::Colours::green;
    this->size   = 50;

    addAndMakeVisible(textEditor = new juce::TextEditor("new text editor"));
    textEditor->setMultiLine(false);
    textEditor->setReturnKeyStartsNewLine(false);
    textEditor->setReadOnly(false);
    textEditor->setScrollbarsShown(false);
    textEditor->setCaretVisible(true);
    textEditor->setPopupMenuEnabled(false);
    textEditor->setSelectAllWhenFocused(true);
    textEditor->addListener(this);
    textEditor->setText(this->getName());

    colourSelector = new juce::ColourSelector(juce::ColourSelector::showColourAtTop | juce::ColourSelector::showSliders | juce::ColourSelector::showColourspace);
    colourSelector->setName("color");
    colourSelector->setCurrentColour(this->colour);
    colourSelector->addChangeListener(this);

    addAndMakeVisible(sizeSlider = new juce::Slider("size"));
    sizeSlider->setRange(10, 100, 1);
    sizeSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    sizeSlider->addListener(this);

    this->setMouseClickGrabsKeyboardFocus(false);
}

Scene::~Scene()
{
    for (int i = 0; i < this->controllerValues.size(); i++)
    {
        delete controllerValues[i];
    }
}

float Scene::getAffectionRatio()
{
    if (affectionRatioChanged || core->needsRefresh())
    {
        affectionRatio        = getAffectionValue() / core->getSumAffectionValues();
        affectionRatioChanged = false;
    }
    return affectionRatio;
}

float Scene::getDistanceFromCursor()
{
    if (distanceFromCursorChanged_)
    {
        this->distanceFromCursor   = jmax(0.f, getDistance(*core->cursor) - this->size * 0.5f);
        distanceFromCursorChanged_ = false;
    }
    return this->distanceFromCursor;
}

int Scene::getValue(const Controller* controller)
{
    for (int i = 0; i < controllerValues.size(); i++)
    {
        if (controllerValues[i]->getController() == controller)
        {
            return controllerValues[i]->getValue();
        }
    }
    return -1;
}

void Scene::moved()
{
    distanceFromCursorChanged();
    core->sceneMoved();
}

juce::Colour Scene::getColour()
{
    return this->colour;
}

void Scene::setColour(const juce::Colour& colour)
{
    this->colour = colour;
    core->sendChangeMessage(this);
}

void Scene::addValue(ControllerValue* value)
{
    this->controllerValues.add(value);
}

float Scene::getAffectionValue()
{
    if (affectionValueChanged || core->needsRefresh())
    {
        float sumOtherDis = 0;
        float facOtherDis = 1;
        for (int i = 0; i < core->scenes.size(); i++)
        {
            Scene* scene = core->scenes[i];
            if (scene != this)
            {
                sumOtherDis += scene->getDistanceFromCursor();
                facOtherDis *= scene->getDistanceFromCursor();
            }
        }
        this->affectionValue        = sumOtherDis / core->getSumDistances() * facOtherDis;
        this->affectionValueChanged = false;
    }
    return this->affectionValue;
}

void Scene::distanceFromCursorChanged()
{
    distanceFromCursorChanged_ = true;
    affectionRatioChanged      = true;
    affectionValueChanged      = true;
}

void Scene::mouseDown(const juce::MouseEvent& e)
{
}

void Scene::mouseUp(const juce::MouseEvent& e)
{
    if (e.mods.isRightButtonDown())
    {
        juce::PopupMenu menu, sub1, sub2, sub3;
        menu.addItem(1, "delete");
        textEditor->setText(this->getName());
        sub1.addCustomItem(2, *textEditor, 64, 20, false, nullptr);
        menu.addSubMenu("name", sub1);
        sub2.addCustomItem(3, *colourSelector, 300, 300, false, nullptr);
        menu.addSubMenu("color", sub2);
        sub3.addCustomItem(4, *sizeSlider, 300, 20, false, nullptr);

        int result = menu.show();
        if (result == 1)
        {
            this->core->removeScene(this);
        }
    }
}

void Scene::mouseDrag(const juce::MouseEvent& e)
{
}

void Scene::getMidiMessages(juce::MidiBuffer& buffer, int pos)
{
    for (int i = controllerValues.size(); --i >= 0;)
    {
        controllerValues[i]->getMidiMessage(buffer, pos);
    }
}

int Scene::getId()
{
    return core->getSceneIndex(this);
}

juce::String Scene::getName()
{
    return this->name;
}

void Scene::setName(juce::String newName)
{
    this->name = newName;
    core->sendChangeMessage(this);
}

void Scene::textEditorTextChanged(juce::TextEditor& editor)
{
    if (&editor == textEditor)
    {
        this->setName(editor.getText());
    }
}

void Scene::textEditorReturnKeyPressed(juce::TextEditor& editor)
{
    if (&editor == textEditor)
    {
        this->setName(editor.getText());
    }
}

void Scene::textEditorEscapeKeyPressed(juce::TextEditor& editor)
{
}

void Scene::textEditorFocusLost(juce::TextEditor& editor)
{
}

void Scene::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    auto* cs = (juce::ColourSelector*) source;
    this->setColour(cs->getCurrentColour());
}

void Scene::sliderValueChanged(juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sizeSlider)
    {
        this->size = juce::roundToInt(sizeSlider->getValue());
        this->setSize(juce::roundToInt(sizeSlider->getValue()), juce::roundToInt(sizeSlider->getValue()));
        this->resized();
        core->sendChangeMessage(this);
    }
}
