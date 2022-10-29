#include "MidiChsEditor.h"

//==============================================================================
MidiChsEditor::MidiChsEditor(MidiChsProcessor* const ownerFilter)
    : AudioProcessorEditor(ownerFilter)
{
    setMouseClickGrabsKeyboardFocus(false);

    bgcolor = juce::Colour(getFilter()->getParameter(18), getFilter()->getParameter(19), getFilter()->getParameter(20), 1.0f);
    fgcolor = juce::Colour(bgcolor.contrasting(getFilter()->getParameter(21)));

    addAndMakeVisible(resetButton = new juce::TextButton("reset"));
    resetButton->setButtonText("Reset");
    resetButton->addListener(this);
    resetButton->setColour(juce::TextButton::buttonColourId, bgcolor);
    resetButton->setColour(juce::TextButton::textColourOnId, fgcolor);
    resetButton->setColour(juce::TextButton::textColourOffId, fgcolor);
    resetButton->setMouseClickGrabsKeyboardFocus(false);

    addAndMakeVisible(clearButton = new juce::TextButton("clear"));
    clearButton->setButtonText("Clear");
    clearButton->addListener(this);
    clearButton->setColour(juce::TextButton::buttonColourId, bgcolor);
    clearButton->setColour(juce::TextButton::textColourOnId, fgcolor);
    clearButton->setColour(juce::TextButton::textColourOffId, fgcolor);
    clearButton->setMouseClickGrabsKeyboardFocus(false);

    for (int i = 0; i < 16; i++)
    {
        addAndMakeVisible(slider[i] = new juce::Slider("new slider"));
        slider[i]->setRange(0, 16, 1);
        slider[i]->setSliderStyle(juce::Slider::LinearBar);
        slider[i]->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
        slider[i]->setColour(juce::Slider::thumbColourId, juce::Colour(0x5effff));
        slider[i]->setColour(juce::Slider::trackColourId, juce::Colour(0x5e2b2b));
        slider[i]->setColour(juce::Slider::textBoxTextColourId, fgcolor);
        slider[i]->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff808080));
        slider[i]->setValue((getFilter()->getParameter(i)) * 16.0);
        slider[i]->addListener(this);
        slider[i]->setMouseClickGrabsKeyboardFocus(false);

        addAndMakeVisible(label[i] = new juce::Label("new label", juce::String(i + 1)));
        label[i]->setFont(juce::Font(15.0000f, juce::Font::plain));
        label[i]->setJustificationType(juce::Justification::centred);
        label[i]->setEditable(false, false, false);
        label[i]->setColour(juce::Label::textColourId, fgcolor);
        label[i]->setMouseClickGrabsKeyboardFocus(false);
    }

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize(ownerFilter->lastUIWidth,
            ownerFilter->lastUIHeight);

    // register ourselves with the filter - it will use its ChangeBroadcaster base
    // class to tell us when something has changed, and this will call our changeListenerCallback()
    // method.
    ownerFilter->addChangeListener(this);
}

MidiChsEditor::~MidiChsEditor()
{
    getFilter()->removeChangeListener(this);

    deleteAllChildren();
}

//==============================================================================
void MidiChsEditor::paint(juce::Graphics& g)
{
    g.fillAll(bgcolor);
}

void MidiChsEditor::resized()
{
    resetButton->setBounds(8, 8, 48, 16);
    clearButton->setBounds(64, 8, 48, 16);
    for (int i = 0; i < 8; i++)
    {
        label[i]->setBounds(i * 40 + 8, 32, 32, 16);
        slider[i]->setBounds(i * 40 + 8, 52, 32, 16);
        label[i + 8]->setBounds(i * 40 + 8, 80, 32, 16);
        slider[i + 8]->setBounds(i * 40 + 8, 100, 32, 16);
    }

    // if we've been resized, tell the filter so that it can store the new size
    // in its settings
    getFilter()->lastUIWidth  = getWidth();
    getFilter()->lastUIHeight = getHeight();
}

void MidiChsEditor::buttonClicked(juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == resetButton)
    {
        for (int i = 0; i < 16; i++)
        {
            slider[i]->setValue((float) (i + 1));
        }
    }
    else if (buttonThatWasClicked == clearButton)
    {
        for (int i = 0; i < 16; i++)
        {
            slider[i]->setValue(0.0f);
        }
    }
}
void MidiChsEditor::sliderValueChanged(juce::Slider* sliderThatWasMoved)
{
    for (int i = 0; i < 16; i++)
    {
        if (sliderThatWasMoved == slider[i])
        {
            getFilter()->setParameterNotifyingHost(i, ((float) slider[i]->getValue()) * 0.0625f);
        }
    }
}
//==============================================================================
void MidiChsEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    // this is the filter telling us that it's changed, so we'll update our
    // display of the time, midi message, etc.
    updateParametersFromFilter();
}

//==============================================================================
void MidiChsEditor::updateParametersFromFilter()
{
    MidiChsProcessor* const filter = getFilter();

    // we use this lock to make sure the processBlock() method isn't writing to the
    // lastMidiMessage variable while we're trying to read it, but be extra-careful to
    // only hold the lock for a minimum amount of time..
    filter->getCallbackLock().enter();

    // take a local copy of the info we need while we've got the lock..
    float ch[16];
    for (int i = 0; i < 16; i++)
        ch[i] = filter->getParameter(i);
    const float reset    = filter->getParameter(16);
    const float clear    = filter->getParameter(17);
    const float hue      = filter->getParameter(18);
    const float sat      = filter->getParameter(19);
    const float bri      = filter->getParameter(20);
    const float contrast = filter->getParameter(21);

    // ..release the lock ASAP
    filter->getCallbackLock().exit();

    // ..and after releasing the lock, we're free to do the time-consuming UI stuff..

    bgcolor = juce::Colour(hue, sat, bri, 1.0f);
    fgcolor = juce::Colour(bgcolor.contrasting(contrast));

    for (int i = 0; i < 16; i++)
    {
        slider[i]->setValue((float) ((int) (ch[i] * 16.0)), juce::dontSendNotification);
        slider[i]->setColour(juce::Slider::textBoxTextColourId, fgcolor);
        label[i]->setColour(juce::Label::textColourId, fgcolor);
    }
    if (reset == 1.0f)
    {
        getFilter()->setParameterNotifyingHost(17, 0.0f);
        getFilter()->setParameterNotifyingHost(16, 0.0f);
        resetButton->triggerClick();
    }
    if (clear == 1.0f)
    {
        getFilter()->setParameterNotifyingHost(16, 0.0f);
        getFilter()->setParameterNotifyingHost(17, 0.0f);
        clearButton->triggerClick();
    }
    resetButton->setColour(juce::TextButton::textColourOnId, fgcolor);
    resetButton->setColour(juce::TextButton::buttonColourId, bgcolor);
    clearButton->setColour(juce::TextButton::textColourOnId, fgcolor);
    clearButton->setColour(juce::TextButton::buttonColourId, bgcolor);
    repaint();
    setSize(filter->lastUIWidth, filter->lastUIHeight);
}
