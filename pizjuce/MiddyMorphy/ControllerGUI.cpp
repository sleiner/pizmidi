
#include "ControllerGUI.h"

#include "Controller.h"
#include "ControllerValue.h"
#include "MidiMorph.h"

using juce::roundToInt;

ControllerGUI::ControllerGUI(Controller* controller, MidiMorph* core)
{
    this->controller = controller;
    this->core       = core;

    addAndMakeVisible(value = new TextBoxSlider(0));
    value->setRange(0, 127, 1, 2);
    value->addListener(this);

    addAndMakeVisible(channel = new TextBoxSlider(0));
    channel->setRange(1, 16, 1, 2);
    channel->addListener(this);

    addAndMakeVisible(ccNo = new TextBoxSlider(0));
    ccNo->setRange(0, 127, 1, 2);
    ccNo->addListener(this);

    addAndMakeVisible(this->name = new juce::TextEditor());
    name->setSelectAllWhenFocused(true);
    name->setColour(juce::TextEditor::outlineColourId, juce::Colour(0.f, 0, 0, 0.f));
    name->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0.f, 0, 0, 0.f));
    name->setColour(juce::TextEditor::focusedOutlineColourId, juce::Colour(0.f, 0, 0, 0.f));
    name->setColour(juce::TextEditor::highlightColourId, juce::Colours::white);
    name->setFont(juce::Font(10));
    name->setInputRestrictions(32);
    name->addListener(this);

    addAndMakeVisible(labCc = new juce::Label("", "cc:"));
    addAndMakeVisible(labCh = new juce::Label("", "ch:"));
    labCc->setFont(juce::Font(10));
    labCh->setFont(juce::Font(10));

    setOpaque(true);

    this->setSize(core->controllerListWidth, 24);
    this->isSelected = false;

    refreshControllerData();

    core->addChangeListener(this);
}

ControllerGUI::~ControllerGUI()
{
    core->removeChangeListener(this);
    deleteAndZero(value);
    deleteAndZero(channel);
    deleteAndZero(ccNo);
    deleteAndZero(name);
    deleteAndZero(labCc);
    deleteAndZero(labCh);
}

void ControllerGUI::resized()
{
    int left       = roundToInt((float) getWidth() * 0.30f);
    int right      = getWidth() - left;
    int h2         = getHeight() / 2;
    int rightdelta = roundToInt((float) right * 0.25f);
    int right1     = rightdelta + left;
    int right2     = right1 + rightdelta;
    int right3     = right2 + rightdelta;

    value->setBounds(0, 0, left, getHeight());

    name->setBounds(left, 0, right, h2);

    labCh->setBounds(left, h2, rightdelta, h2);
    channel->setBounds(right1, h2, rightdelta, h2);

    labCc->setBounds(right2, h2, rightdelta, h2);
    ccNo->setBounds(right3, h2, rightdelta, h2);
}

//==============================================================================

void ControllerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::lightgrey);
}

//==============================================================================

void ControllerGUI::paintOverChildren(juce::Graphics& g)
{
    g.drawRoundedRectangle(0, 0, (float) getWidth(), (float) getHeight(), 2, 2);
}

void ControllerGUI::mouseUp(const juce::MouseEvent& e)
{
    if (e.mods.isRightButtonDown())
    {
        juce::PopupMenu menu;
        menu.addItem(1, "remove controller");
        menu.addItem(2, "add controller");
        int result = menu.show();
        if (result == 1)
        {
            controller->remove();
        }
        else if (result == 2)
        {
            int ch = core->getController(core->getNumControllers() - 1)->getChannel();
            int cc = core->getController(core->getNumControllers() - 1)->getCcNo();
            core->addController(cc + 1, ch);
        }
    }
}

void ControllerGUI::sliderValueChanged(juce::Slider* slider)
{
}

void ControllerGUI::setSelected(bool shouldDrawSelected)
{
    this->isSelected = true;
}

void ControllerGUI::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    refreshControllerData();
}

void ControllerGUI::refreshControllerData()
{
    if (core->getNumSelectedScenes() > 0)
    {
        if (controller->getValue(core->getSelectedScene(0)) != 0)
        {
            this->value->setValue(controller->getValue(core->getSelectedScene(0))->getValue(), juce::dontSendNotification);
        }
        else
        {
            this->value->setValue(0, juce::dontSendNotification);
        }
    }
    else
    {
        this->value->setValue(controller->getInterpolatedValue(), juce::dontSendNotification);
    }
    this->channel->setValue(controller->getChannel(), juce::dontSendNotification);
    this->ccNo->setValue(controller->getCcNo(), juce::dontSendNotification);
    this->name->setText(controller->getName(), false);
    int state = controller->getState();
    switch (state)
    {
        case Controller::undefined:
            value->setColour(juce::Label::textColourId, juce::Colours::blue);
            break;
        case Controller::mutival:
            value->setColour(juce::Label::textColourId, juce::Colours::red);
            break;
        case Controller::defined:
            value->setColour(juce::Label::textColourId, juce::Colours::black);
            break;
    }

    this->repaint();
}

void ControllerGUI::textEditorTextChanged(juce::TextEditor& editor)
{
    if (&editor == name)
    {
        controller->setName(editor.getText());
    }
}

void ControllerGUI::textEditorReturnKeyPressed(juce::TextEditor& editor)
{
}

void ControllerGUI::textEditorEscapeKeyPressed(juce::TextEditor& editor)
{
}

void ControllerGUI::textEditorFocusLost(juce::TextEditor& editor)
{
}

void ControllerGUI::labelTextChanged(juce::Label* labelThatHasChanged)
{
    if (labelThatHasChanged == value)
    {
        controller->setValueInSelectedScenes(roundToInt(value->getValue()));
        repaint();
    }
    else if (labelThatHasChanged == ccNo)
    {
        controller->ccNo = roundToInt(ccNo->getValue());
    }
    else if (labelThatHasChanged == channel)
    {
        controller->channel = roundToInt(channel->getValue());
    }
}
