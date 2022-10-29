
#include "TextBoxSlider.h"
#include "NumberBox.h"

using juce::jmax;
using juce::jmin;
using juce::roundToInt;

TextBoxSlider::TextBoxSlider (double initVal)
    : Label ("", "")
{
    setRange (0, 100, 1, 10);
    setValue (initVal, juce::dontSendNotification);
    setEditable (true, true);
    this->setJustificationType (juce::Justification::centred);
}

TextBoxSlider::~TextBoxSlider()
{
}

void TextBoxSlider::mouseDrag (const juce::MouseEvent& e)
{
    double delta  = this->step * e.getDistanceFromDragStartY() / pixelStep;
    double newVal = oldValue - delta;
    setValue (newVal, juce::sendNotification);
}

void TextBoxSlider::labelTextChanged (Label* labelThatHasChanged)
{
}

void TextBoxSlider::mouseDown (const juce::MouseEvent& e)
{
    this->oldValue = getText().getDoubleValue();
}

void TextBoxSlider::mouseUp (const juce::MouseEvent& e)
{
    this->getParentComponent()->mouseUp (e);
}

void TextBoxSlider::setRange (double newMin, double newMax, double stepSize, int pixelPerStep)
{
    this->min       = newMin;
    this->max       = newMax;
    this->step      = stepSize;
    this->pixelStep = pixelPerStep;
}

juce::TextEditor* TextBoxSlider::createEditorComponent()
{
    juce::TextEditor* te = new NumberBox();
    te->setSelectAllWhenFocused (true);
    te->setFont (getFont());
    te->setInputRestrictions (256, "0123456789.,-+");
    te->setColour (te->outlineColourId, juce::Colour (0.f, 0, 0, 0.f));
    te->setColour (te->backgroundColourId, juce::Colour (0.f, 0, 0, 0.f));
    te->setColour (te->focusedOutlineColourId, juce::Colour (0.f, 0, 0, 0.f));
    te->setColour (te->highlightColourId, juce::Colours::white);

    return te;
}

void TextBoxSlider::paintOverChildren (juce::Graphics& g)
{
    g.setColour (findColour (textColourId));
    g.drawRoundedRectangle (1.f, 1.f, (float) (getWidth() - 2), (float) (getHeight() - 2), (float) (getHeight() - 2) / 2.f, 1.f);
}

double TextBoxSlider::getValue()
{
    return value;
}

void TextBoxSlider::setText (const juce::String& newText, juce::NotificationType notification)
{
    juce::String t (newText.trimStart());

    while (t.startsWithChar ('+'))
    {
        t = t.substring (1).trimStart();
    }

    double newVal = t.initialSectionContainingOnly ("0123456789.,-").getDoubleValue();

    setValue (newVal, notification);
}

void TextBoxSlider::setValue (double newVal, juce::NotificationType notification)
{
    long newValInt = roundToInt (newVal / step);
    newVal         = newValInt * step;
    newVal         = jmin (max, newVal);
    newVal         = jmax (min, newVal);

    value = newVal;
    Label::setText (juce::String (value), notification);
}

void TextBoxSlider::resized()
{
    setFont (juce::Font ((float) (getHeight() - 2)));
    Label::resized();
}

void TextBoxSlider::textWasEdited()
{
    this->setText (getText(), juce::sendNotification);
}
