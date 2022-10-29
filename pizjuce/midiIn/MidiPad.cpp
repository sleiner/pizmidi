#include "MidiPad.h"

MidiPad::MidiPad()
    : drawableButton(nullptr)
{
    addAndMakeVisible(drawableButton = new DrawablePad("MidiPad"));
    drawableButton->addListener(this);
    addAndMakeVisible(label = new juce::Label("Label", juce::String()));
    label->setFont(juce::Font(9.0000f, juce::Font::plain));
    label->setJustificationType(juce::Justification::centred);
    setMouseClickGrabsKeyboardFocus(false);
}

MidiPad::~MidiPad()
{
    deleteAndZero(drawableButton);
    deleteAndZero(label);
}

void MidiPad::setTooltip(juce::String text)
{
    drawableButton->setTooltip(text);
}

void MidiPad::paint(juce::Graphics& g)
{
}

void MidiPad::setColour(const juce::Colour& colour)
{
    drawableButton->setBackgroundColours(colour, colour);
}

void MidiPad::resized()
{
    drawableButton->setBounds(0, 0, proportionOfWidth(1.0000f), proportionOfHeight(1.0000f));
    label->setBounds(0, 0, proportionOfWidth(1.0000f), proportionOfHeight(1.0000f));
}

void MidiPad::buttonClicked(juce::Button* buttonThatWasClicked)
{
}

bool MidiPad::isInterestedInFileDrag(const juce::StringArray& files)
{
    juce::File file = juce::File(files.joinIntoString(juce::String(), 0, 1));
    if (file.hasFileExtension("png") || file.hasFileExtension("gif") || file.hasFileExtension("jpg") || file.hasFileExtension("svg"))
        return true;
    else
        return false;
}

void MidiPad::filesDropped(const juce::StringArray& filenames, int mouseX, int mouseY)
{
    if (isInterestedInFileDrag(filenames))
    {
        juce::String filename = filenames.joinIntoString(juce::String(), 0, 1);
        juce::File file       = juce::File(filename);
        auto image            = juce::Drawable::createFromImageFile(file);
        drawableButton->setImages(image.get());
        //save the relative path
        drawableButton->setName(file.getRelativePathFrom(juce::File::getSpecialLocation(juce::File::currentExecutableFile)));
        drawableButton->setState(juce::Button::buttonNormal);
        label->setText(juce::String(), juce::dontSendNotification);
    }
}

void MidiPad::setButtonText(const juce::String& newText)
{
    label->setText(newText, juce::dontSendNotification);
}

void MidiPad::setTriggeredOnMouseDown(const bool isTriggeredOnMouseDown)
{
    drawableButton->setTriggeredOnMouseDown(isTriggeredOnMouseDown);
}

void MidiPad::addButtonListener(juce::Button::Listener* const newListener)
{
    drawableButton->addListener(newListener);
}

void MidiPad::clearIcon()
{
    drawableButton->setImages(nullptr);
    drawableButton->setName("");
    label->setText("Drag\nIcon", juce::dontSendNotification);
}
