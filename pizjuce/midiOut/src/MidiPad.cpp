#include "MidiPad.h"

MidiPad::MidiPad ()
: drawableButton (0)
{
    addAndMakeVisible (drawableButton = new DrawablePad("MidiPad"));
    drawableButton->addListener (this);
	addAndMakeVisible (label = new Label("Label",String()));
    label->setFont (Font (9.0000f, Font::plain));
	label->setJustificationType(Justification::centred);
	label->setInterceptsMouseClicks(false,false);
    setMouseClickGrabsKeyboardFocus (false);
}

MidiPad::~MidiPad() {
	deleteAndZero(drawableButton);
	deleteAndZero(label);
}

void MidiPad::setTooltip(String text)
{
	drawableButton->setTooltip(text);
}

void MidiPad::paint (Graphics& g) {
}

void MidiPad::setColour(const juce::Colour &colour) {
    drawableButton->setBackgroundColours(colour,colour);
}

void MidiPad::resized()
{
    drawableButton->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
    label->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
}

void MidiPad::buttonClicked(Button* buttonThatWasClicked) {
}

bool MidiPad::isInterestedInFileDrag (const StringArray& files) {
    File file = File(files.joinIntoString(String(),0,1));
    if (file.hasFileExtension("png") ||
        file.hasFileExtension("gif") ||
        file.hasFileExtension("jpg") ||
        file.hasFileExtension("svg") )
        return true;
    else return false;
}

void MidiPad::filesDropped(const juce::StringArray &filenames, int mouseX, int mouseY)
{
    if (isInterestedInFileDrag(filenames)) {
        String filename = filenames.joinIntoString(String(),0,1);
        File file = File(filename);
        auto image = Drawable::createFromImageFile(file);
        drawableButton->setImages(image.get());
        //save the relative path
        drawableButton->setName(file.getRelativePathFrom(File::getSpecialLocation(File::currentExecutableFile)));
        drawableButton->setState(Button::buttonNormal);
		label->setText(String(),dontSendNotification);
    }
}

void MidiPad::setButtonText (const String &newText) {
    label->setText(newText,dontSendNotification);
}

void MidiPad::setTriggeredOnMouseDown(const bool isTriggeredOnMouseDown) {
    drawableButton->setTriggeredOnMouseDown(isTriggeredOnMouseDown);
}

void MidiPad::addButtonListener (Button::Listener *const newListener) {
    drawableButton->addListener(newListener);
}

void MidiPad::clearIcon()
{
    drawableButton->setImages(0);
    drawableButton->setName("");
	label->setText("Drag\nIcon",dontSendNotification);
}
