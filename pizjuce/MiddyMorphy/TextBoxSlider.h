#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class NumberBox;

class TextBoxSlider : public juce::Label::Listener, public juce::Label
{
public:
    TextBoxSlider(double initVal);

    ~TextBoxSlider() override;

    void mouseDrag(const juce::MouseEvent& e) override;

    void labelTextChanged(Label* labelThatHasChanged) override;

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

    void setRange(double newMin, double newMax, double stepSize, int pixelPerStep);

    juce::TextEditor* createEditorComponent() override;

    void paintOverChildren(juce::Graphics& g) override;

    double getValue();

    void setValue(double newVal, juce::NotificationType notification);

    void resized() override;

    void textWasEdited() override;

private:
    int pixelStep;

    double oldValue;
    double value;

    double min;
    double max;

    double step;

    void setText(const juce::String& newText, juce::NotificationType notification);
};
