
#include "TextBoxSlider.h"
#include "NumberBox.h"

void TextBoxSlider::mouseDrag(const MouseEvent & e) {
  // Bouml preserved body begin 0004818D
	double delta = this->step * e.getDistanceFromDragStartY() / pixelStep;
	double newVal = oldValue - delta;
	setValue(newVal,sendNotification);

  // Bouml preserved body end 0004818D
}

void TextBoxSlider::labelTextChanged(Label * labelThatHasChanged) {
  // Bouml preserved body begin 0004820D

  // Bouml preserved body end 0004820D
}

TextBoxSlider::TextBoxSlider(double initval)
:  Label("","")
{
  // Bouml preserved body begin 0004830D
	setRange(0,100,1,10);
	setValue(initval,dontSendNotification);
	//addListener(this);
	setEditable(true,true);
	this->setJustificationType(Justification::centred);
  // Bouml preserved body end 0004830D
}

TextBoxSlider::~TextBoxSlider() {
  // Bouml preserved body begin 0004838D
	//this->removeListener(this);
  // Bouml preserved body end 0004838D
}

void TextBoxSlider::mouseDown(const MouseEvent & e) {
  // Bouml preserved body begin 0004840D
	this->oldValue = getText().getDoubleValue();
  // Bouml preserved body end 0004840D
}

void TextBoxSlider::mouseUp(const MouseEvent & e) {
  // Bouml preserved body begin 0004840D
    this->getParentComponent()->mouseUp(e);
  // Bouml preserved body end 0004840D
}

void TextBoxSlider::setRange(double min, double max, double stepsize, int pixelPerStep) {
  // Bouml preserved body begin 0004848D
	this->min = min;
	this->max = max;
	this->step = stepsize;
	this->pixelStep = pixelPerStep;
  // Bouml preserved body end 0004848D
}

juce::TextEditor* TextBoxSlider::createEditorComponent() {
  // Bouml preserved body begin 0004850D
	TextEditor* te = new NumberBox();
	te->setSelectAllWhenFocused(true);
	te->setFont(getFont());
	te->setInputRestrictions(256,"0123456789.,-+");
	te->setColour(te->outlineColourId,Colour(0.f,0,0,0.f));
	te->setColour(te->backgroundColourId,Colour(0.f,0,0,0.f));
	te->setColour(te->focusedOutlineColourId,Colour(0.f,0,0,0.f));
	te->setColour(te->highlightColourId,Colours::white);
	////te->setIndents(getWidth()/2,0);
	return te;
  // Bouml preserved body end 0004850D
}

//==============================================================================

void TextBoxSlider::paintOverChildren(Graphics & g) {
  // Bouml preserved body begin 0004858D
	g.setColour(findColour(textColourId));
	g.drawRoundedRectangle(1.f,1.f,(float)(getWidth()-2),(float)(getHeight()-2),(float)(getHeight()-2)/2.f,1.f);
  // Bouml preserved body end 0004858D
}

double TextBoxSlider::getValue() {
  // Bouml preserved body begin 0004870D
	return value;
  // Bouml preserved body end 0004870D
}

// (const String& newText,                       const bool broadcastChangeMessage)
void TextBoxSlider::setText(const String & newText, NotificationType notification) {
  // Bouml preserved body begin 00048B0D
	String t(newText.trimStart());

	while (t.startsWithChar ('+'))
	{
	t = t.substring (1).trimStart();
	}

	double newVal = t.initialSectionContainingOnly ("0123456789.,-").getDoubleValue();

	setValue(newVal,notification);
  // Bouml preserved body end 00048B0D
}

void TextBoxSlider::setValue(double newVal, NotificationType notification) {
  // Bouml preserved body begin 00048A8D
		long newValInt = roundDoubleToInt(newVal/step);
		newVal = newValInt*step;
		newVal = jmin(max,newVal);
		newVal = jmax(min,newVal);

		value = newVal;
		Label::setText(String(value),notification);

  // Bouml preserved body end 00048A8D
}

void TextBoxSlider::resized() {
  // Bouml preserved body begin 00049C0D
	setFont(Font((float)(getHeight()-2)));
	Label::resized();
  // Bouml preserved body end 00049C0D
}

void TextBoxSlider::textWasEdited() {
  // Bouml preserved body begin 0004A70D
	this->setText(getText(),sendNotification);
  // Bouml preserved body end 0004A70D
}
