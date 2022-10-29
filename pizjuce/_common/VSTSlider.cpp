#include "VSTSlider.h"

VSTSlider::VSTSlider (juce::String name)
    : Slider (name)
{
    this->ownerPlugin = nullptr;
    this->vstIndex    = -1;
    this->setMouseClickGrabsKeyboardFocus (false);
};

VSTSlider::~VSTSlider(){};

juce::String VSTSlider::getTextFromValue (double value)
{
    if (ownerPlugin && vstIndex > -1)
        return ownerPlugin->getParameterText (vstIndex);
    else
        return juce::String (value) + "?";
};

void VSTSlider::setOwner (juce::AudioProcessor* owner, int index)
{
    this->ownerPlugin = owner;
    this->vstIndex    = index;
};

float VSTSlider::mapToVSTRange()
{
    //maps a slider of arbitrary range to [0..1]
    double min = this->getMinimum();
    double max = this->getMaximum();
    double x   = this->getValue();
    return (float) ((x - min) / (max - min));
};

void VSTSlider::setVSTParam()
{
    double min = this->getMinimum();
    double max = this->getMaximum();
    double x   = this->getValue();
    ownerPlugin->setParameterNotifyingHost (vstIndex, (float) ((x - min) / (max - min)));
}

void VSTSlider::setVSTSlider (float x)
{
    //maps VST parameter range [0..1] to slider's range
    if (x != this->mapToVSTRange())
    {
        double min   = this->getMinimum();
        double max   = this->getMaximum();
        double value = min + (double) x * (max - min);
        this->setValue (value, juce::dontSendNotification);
    }
    this->updateText();
};

void VSTSlider::setVSTSlider()
{
    //maps VST parameter range [0..1] to slider's range
    const float x = ownerPlugin->getParameter (vstIndex);
    if (x != this->mapToVSTRange())
    {
        double min   = this->getMinimum();
        double max   = this->getMaximum();
        double value = min + (double) x * (max - min);
        this->setValue (value, juce::dontSendNotification);
    }
    this->updateText();
};

void VSTSlider::setIndex (int index)
{
    this->vstIndex = index;
}
