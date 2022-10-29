#include "MidiPad.h"

using juce::jmax;
using juce::jmin;

MidiPad::MidiPad()
    : Button ("MidiPad"),
      normalImage (nullptr),
      index (0),
      text (nullptr)

{
    bgColour   = juce::Colours::white;
    textColour = juce::Colours::black;

    centeredText = false;
    imageSize    = 1.f;
    setMouseClickGrabsKeyboardFocus (true);

    addAndMakeVisible (text = new juce::Label ("label", ""));
    text->setJustificationType (juce::Justification::centredTop);
    text->setEditable (false, false, false);
    text->setInterceptsMouseClicks (false, false);

    setSize (200, 200);
}

MidiPad::~MidiPad()
{
    if (text)
        delete text;
    deleteImages();
}

void MidiPad::deleteImages()
{
    normalImage.reset();
}

bool MidiPad::setImageFromFile (juce::File file)
{
    if (file.exists())
    {
        deleteImages();

        normalImage = juce::Drawable::createFromImageFile (file);
        if (normalImage != nullptr)
        {
            repaint();
        }
        else
            return false;
        return true;
    }
    return false;
}

void MidiPad::setImages (const juce::Drawable* normal)
{
    deleteImages();
    if (normal != nullptr)
    {
        normalImage = normal->createCopy();
    }
    repaint();
}

void MidiPad::setIconPath (juce::String name)
{
    iconPath = name;
}

juce::String MidiPad::getIconPath()
{
    return iconPath;
}

void MidiPad::setColour (const juce::Colour& colour)
{
    bgColour = colour;
    repaint();
}

void MidiPad::setTextColour (const juce::Colour& colour)
{
    textColour = colour;
    text->setColour (juce::Label::textColourId, colour);
}

const juce::Colour& MidiPad::getBackgroundColour() const throw()
{
    return bgColour;
}

void MidiPad::drawButtonBackground (juce::Graphics& g,
                                    Button& button,
                                    const juce::Colour& backgroundColour,
                                    bool isMouseOverButton,
                                    bool isButtonDown)
{
    g.fillAll (backgroundColour);
}

void MidiPad::paintButton (juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    juce::Rectangle<float> imageSpace;
    const float insetX = getWidth() * (1.f - imageSize) * 0.5f;
    const float insetY = getHeight() * (1.f - imageSize) * 0.5f;
    imageSpace.setBounds (insetX, insetY, getWidth() - insetX * 2, getHeight() - insetY * 2);
    MidiPad::drawButtonBackground (g, *this, getBackgroundColour(), isMouseOverButton, isButtonDown);

    g.setImageResamplingQuality (juce::Graphics::highResamplingQuality);
    g.setOpacity (1.0f);

    const juce::Drawable* imageToDraw = nullptr;
    imageToDraw                       = getCurrentImage();
    if (imageToDraw != nullptr)
    {
        imageToDraw->drawWithin (g,
                                 imageSpace,
                                 juce::RectanglePlacement::centred,
                                 1.f);
    }
}

void MidiPad::setCenteredText (bool centered)
{
    text->setJustificationType (centered ? juce::Justification::centred : juce::Justification::centredTop);
}

void MidiPad::resized()
{
    text->setBounds (4, 4, getWidth() - 8, getHeight() - 8);
    float fontsize = jmax (5.f, jmin ((float) (proportionOfWidth (0.2f)), (float) (proportionOfHeight (0.15f))));
    text->setFont (juce::Font (fontsize, juce::Font::bold));
}

const juce::Drawable* MidiPad::getCurrentImage() const throw()
{
    return getNormalImage();
}

const juce::Drawable* MidiPad::getNormalImage() const throw()
{
    return normalImage.get();
}

void MidiPad::setText (const juce::String& name)
{
    text->setText (name, juce::dontSendNotification);
}

void MidiPad::setButtonText (const juce::String& newText)
{
    text->setText (newText, juce::dontSendNotification);
}

juce::String MidiPad::getText()
{
    return text->getText();
}
