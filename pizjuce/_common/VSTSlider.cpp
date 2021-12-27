#include "VSTSlider.h"

VSTSlider::VSTSlider(String name) : Slider(name)
{
    this->ownerPlugin = 0;
    this->vstIndex = -1;
    this->setMouseClickGrabsKeyboardFocus(false);
};

VSTSlider::~VSTSlider() {};

String VSTSlider::getTextFromValue(double value)
{
    if (ownerPlugin && vstIndex>-1) return ownerPlugin->getParameterText(vstIndex);
    else return String(value)+"?";
};

void VSTSlider::setOwner(AudioProcessor* owner, int index)
{
    this->ownerPlugin = owner;
    this->vstIndex = index;
};

float VSTSlider::mapToVSTRange() {
    //maps a slider of arbitrary range to [0..1]
    double min = this->getMinimum();
    double max = this->getMaximum();
    double x = this->getValue();
    return (float)((x-min)/(max-min));
};

void VSTSlider::setVSTParam()
{
    double min = this->getMinimum();
    double max = this->getMaximum();
    double x = this->getValue();
	ownerPlugin->setParameterNotifyingHost(vstIndex,(float)((x-min)/(max-min)));
}

void VSTSlider::setVSTSlider(float x) {
    //maps VST parameter range [0..1] to slider's range
    if (x!=this->mapToVSTRange()) {
        double min = this->getMinimum();
        double max = this->getMaximum();
        double value = min + (double)x*(max-min);
        this->setValue(value,false,false);
    }
    this->updateText();
};

void VSTSlider::setVSTSlider() {
    //maps VST parameter range [0..1] to slider's range
	const float x = ownerPlugin->getParameter(vstIndex);
    if (x!=this->mapToVSTRange()) {
        double min = this->getMinimum();
        double max = this->getMaximum();
        double value = min + (double)x*(max-min);
        this->setValue(value,false,false);
    }
    this->updateText();
};