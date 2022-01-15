#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

namespace juce
{
    class TextEditor;
}
class NumberBox;
namespace juce
{
    class String;
}
using namespace juce;

class TextBoxSlider : public juce::Label::Listener, public juce::Label
{
public:
    void mouseDrag (const MouseEvent& e) override;

    void labelTextChanged (Label* labelThatHasChanged) override;

    TextBoxSlider (double initval);

    ~TextBoxSlider() override;

    void mouseDown (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;

private:
    int pixelStep;

    double oldValue;

    double value;

    double min;

    double max;

    double step;

public:
    void setRange (double min, double max, double stepsize, int pixelPerStep);

    juce::TextEditor* createEditorComponent() override;

    void paintOverChildren (Graphics& g) override;

    double getValue();

private:
    // (const String& newText,                       const bool broadcastChangeMessage)
    void setText (const String& newText, NotificationType notification);

public:
    void setValue (double newVal, NotificationType notification);

    void resized() override;

    void textWasEdited() override;
};
