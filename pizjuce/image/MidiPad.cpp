#include "MidiPad.h"

#include "MidiPad.h"

MidiPad::MidiPad ()
    : Button ("MidiPad"),
      normalImage (nullptr),
      index(0),
	  text (0)

{
	bgColour = Colours::white;
	textColour = Colours::black;

	centeredText=false;
	imageSize=1.f;
    setMouseClickGrabsKeyboardFocus (true);

	addAndMakeVisible(text = new Label("label",""));
    text->setJustificationType (Justification::centredTop);
    text->setEditable (false, false, false);
	text->setInterceptsMouseClicks(false,false);

    setSize (200, 200);
}

MidiPad::~MidiPad()
{
	if (text) delete text;
    deleteImages();
}

void MidiPad::deleteImages()
{
    normalImage.reset();
}

bool MidiPad::setImageFromFile(File file)
{
	if (file.exists())
	{
		deleteImages();

		normalImage = Drawable::createFromImageFile(file);
		if (normalImage!=nullptr) {
			repaint();
		}
		else return false;
		return true;
	}
	return false;
}

void MidiPad::setImages (const Drawable* normal)
{
    deleteImages();
	if (normal != nullptr) {
		normalImage = normal->createCopy();
	}
    repaint();
}

void MidiPad::setIconPath(String name)
{
	iconPath = name;
}

String MidiPad::getIconPath()
{
	return iconPath;
}

void MidiPad::setColour(const juce::Colour &colour) {
    bgColour = colour;
	repaint();
}

void MidiPad::setTextColour(const juce::Colour &colour) {
	textColour = colour;
	text->setColour (Label::textColourId, colour);
}

const Colour& MidiPad::getBackgroundColour() const throw()
{
    return bgColour;
}

void MidiPad::drawButtonBackground (Graphics& g,
                                Button& button,
                                const Colour& backgroundColour,
                                bool isMouseOverButton,
                                bool isButtonDown)
{
	g.fillAll(backgroundColour);
}

void MidiPad::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    juce::Rectangle<float> imageSpace;
    const float insetX = getWidth()*(1.f-imageSize)*0.5f;
    const float insetY = getHeight()*(1.f-imageSize)*0.5f;
    imageSpace.setBounds (insetX, insetY, getWidth() - insetX * 2, getHeight() - insetY * 2);
    MidiPad::drawButtonBackground (g, *this,
                                       getBackgroundColour(),
                                       isMouseOverButton,
                                       isButtonDown);

    g.setImageResamplingQuality (Graphics::highResamplingQuality);
    g.setOpacity (1.0f);

    const Drawable* imageToDraw = 0;
    imageToDraw = getCurrentImage();
    if (imageToDraw != 0)
    {
        imageToDraw->drawWithin (g,
								 imageSpace,
                                 RectanglePlacement::centred,
								 1.f);
    }
}

void MidiPad::setCenteredText(bool centered)
{
	text->setJustificationType (centered ? Justification::centred : Justification::centredTop);
}

void MidiPad::resized()
{
	text->setBounds(4,4,getWidth()-8,getHeight()-8);
    float fontsize = jmax(5.f,jmin((float)(proportionOfWidth(0.2f)),(float)(proportionOfHeight(0.15f))));
    text->setFont (Font (fontsize, Font::bold));
}

const Drawable* MidiPad::getCurrentImage() const throw()
{
    return getNormalImage();
}

const Drawable* MidiPad::getNormalImage() const throw()
{
    return normalImage.get();
}

void MidiPad::setText(const String& name)
{
	text->setText(name,dontSendNotification);
}

void MidiPad::setButtonText(const String& newText)
{
	text->setText(newText,dontSendNotification);
}

String MidiPad::getText()
{
	return text->getText();
}
