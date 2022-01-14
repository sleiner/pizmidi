
#include "Scene.h"
#include "Controller.h"
#include "ControllerValue.h"
#include "Cursor.h"
#include "MidiMorph.h"
#include "ModuleGUI.h"

Scene::Scene (const Scene& scene)
{
    // Bouml preserved body begin 0003D10D
    // Bouml preserved body end 0003D10D
}

Scene::Scene (MidiMorph* core)
{
    // Bouml preserved body begin 00034B0D
    this->core = core;
    this->colour = Colours::green;
    this->size = 50;

    addAndMakeVisible (textEditor = new TextEditor ("new text editor"));
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (false);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (false);
    textEditor->setSelectAllWhenFocused (true);
    textEditor->addListener (this);
    textEditor->setText (this->getName());

    colourSelector = new ColourSelector (ColourSelector::showColourAtTop | ColourSelector::showSliders | ColourSelector::showColourspace);
    colourSelector->setName ("color");
    colourSelector->setCurrentColour (this->colour);
    colourSelector->addChangeListener (this);

    addAndMakeVisible (sizeSlider = new Slider ("size"));
    sizeSlider->setRange (10, 100, 1);
    sizeSlider->setSliderStyle (Slider::LinearHorizontal);
    sizeSlider->addListener (this);

    this->setMouseClickGrabsKeyboardFocus (false);

    // Bouml preserved body end 00034B0D
}

Scene::~Scene()
{
    // Bouml preserved body begin 00040B0D
    for (int i = 0; i < this->controllerValues.size(); i++)
    {
        delete controllerValues[i];
    }
    // Bouml preserved body end 00040B0D
}

float Scene::getAffectionRatio()
{
    // Bouml preserved body begin 0003C68D
    if (affectionRatioChanged || core->needsRefresh())
    {
        affectionRatio = getAffectionValue() / core->getSumAffectionValues();
        affectionRatioChanged = false;
    }
    return affectionRatio;
    //return 1. - getDistanceFromCursor() / core->getSumDistances() * ;
    // Bouml preserved body end 0003C68D
}

//get Precalculated Value
float Scene::getDistanceFromCursor()
{
    // Bouml preserved body begin 00034F8D
    if (distanceFromCursorChanged_)
    {
        this->distanceFromCursor = jmax (0.f, getDistance (*core->cursor) - this->size * 0.5f);
        //this->setName(String(this->distanceFromCursor));
        distanceFromCursorChanged_ = false;
    }
    return this->distanceFromCursor;
    // Bouml preserved body end 00034F8D
}

int Scene::getValue (const Controller* controller)
{
    // Bouml preserved body begin 0003520D
    for (int i = 0; i < controllerValues.size(); i++)
    {
        if (controllerValues[i]->getController() == controller)
        {
            return controllerValues[i]->getValue();
        }
    }
    return -1;
    // Bouml preserved body end 0003520D
}

//gets called when the XYItem gets new coordinates
//in this implementation refresh the distance value
void Scene::moved()
{
    // Bouml preserved body begin 0003590D
    distanceFromCursorChanged();
    core->sceneMoved();
    // Bouml preserved body end 0003590D
}

juce::Colour Scene::getColour()
{
    // Bouml preserved body begin 0003D00D
    return this->colour;
    // Bouml preserved body end 0003D00D
}

void Scene::setColour (const juce::Colour& colour)
{
    // Bouml preserved body begin 0003D08D
    this->colour = colour;
    core->sendChangeMessage (this);
    // Bouml preserved body end 0003D08D
}

void Scene::addValue (ControllerValue* value)
{
    // Bouml preserved body begin 0003F30D
    this->controllerValues.add (value);
    // Bouml preserved body end 0003F30D
}

float Scene::getAffectionValue()
{
    // Bouml preserved body begin 0003F48D
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
        this->affectionValue = sumOtherDis / core->getSumDistances() * facOtherDis;
        this->affectionValueChanged = false;
    }
    return this->affectionValue;

    // Bouml preserved body end 0003F48D
}

void Scene::distanceFromCursorChanged()
{
    // Bouml preserved body begin 0003F58D
    distanceFromCursorChanged_ = true;
    affectionRatioChanged = true;
    affectionValueChanged = true;
    // Bouml preserved body end 0003F58D
}

void Scene::mouseDown (const MouseEvent& e)
{
    // Bouml preserved body begin 0003F78D

    // Bouml preserved body end 0003F78D
}

void Scene::mouseUp (const MouseEvent& e)
{
    // Bouml preserved body begin 0003F80D
    if (e.mods.isRightButtonDown())
    {
        PopupMenu menu, sub1, sub2, sub3;
        menu.addItem (1, "delete");
        textEditor->setText (this->getName());
        sub1.addCustomItem (2, *textEditor, 64, 20, false, nullptr);
        menu.addSubMenu ("name", sub1);
        sub2.addCustomItem (3, *colourSelector, 300, 300, false, nullptr);
        menu.addSubMenu ("color", sub2);
        sub3.addCustomItem (4, *sizeSlider, 300, 20, false, nullptr);
        //menu.addSubMenu("size",sub3);

        int result = menu.show();
        if (result == 1)
        {
            this->core->removeScene (this);
        }
    }
    // Bouml preserved body end 0003F80D
}

void Scene::mouseDrag (const MouseEvent& e)
{
    // Bouml preserved body begin 0003F88D
    // Bouml preserved body end 0003F88D
}

void Scene::getMidiMessages (juce::MidiBuffer& buffer, int pos)
{
    // Bouml preserved body begin 00048D8D
    for (int i = controllerValues.size(); --i >= 0;)
    {
        controllerValues[i]->getMidiMessage (buffer, pos);
    }
    // Bouml preserved body end 00048D8D
}

int Scene::getId()
{
    // Bouml preserved body begin 0004070D
    return core->getSceneIndex (this);
    // Bouml preserved body end 0004070D
}

String Scene::getName()
{
    return this->name;
}

void Scene::setName (String newName)
{
    this->name = newName;
    core->sendChangeMessage (this);
}

void Scene::textEditorTextChanged (TextEditor& editor)
{
    if (&editor == textEditor)
    {
        this->setName (editor.getText());
    }
}

void Scene::textEditorReturnKeyPressed (TextEditor& editor)
{
    if (&editor == textEditor)
    {
        this->setName (editor.getText());
    }
}

void Scene::textEditorEscapeKeyPressed (TextEditor& editor)
{
}

void Scene::textEditorFocusLost (TextEditor& editor)
{
}

void Scene::changeListenerCallback (ChangeBroadcaster* source)
{
    ColourSelector* cs = (ColourSelector*) source;
    this->setColour (cs->getCurrentColour());
}

void Scene::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == sizeSlider)
    {
        this->size = ::roundToInt (sizeSlider->getValue());
        this->setSize (::roundToInt (sizeSlider->getValue()), ::roundToInt (sizeSlider->getValue()));
        this->resized();
        core->sendChangeMessage (this);
    }
}
