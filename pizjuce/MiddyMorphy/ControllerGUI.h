#pragma once
#include "TextBoxSlider.h"

#include "juce_gui_basics/juce_gui_basics.h"

class Controller;
class MidiMorph;
namespace juce
{
    class TextEditor;
}
namespace juce
{
    class Slider;
}
class ControllerValue;
class TextBoxSlider;
namespace juce
{
    class Label;
}

class ControllerGUI : public juce::TextEditor::Listener, public juce::ChangeListener, public juce::Component, public juce::Slider::Listener, public juce::Label::Listener
{
public:
    ControllerGUI (Controller* controller, MidiMorph* core);

    ~ControllerGUI() override;

private:
    bool isSelected;

    Controller* controller;

    MidiMorph* core;

    juce::TextEditor* name;

public:
    void resized() override;

    void paint (Graphics& g) override;

    void paintOverChildren (Graphics& g) override;

    void mouseUp (const MouseEvent& e) override;

    //()=
    void sliderValueChanged (Slider* slider) override;

    void setSelected (bool shouldDrawSelected);

    void changeListenerCallback (ChangeBroadcaster* source) override;

    void refreshControllerData();

    void textEditorTextChanged (TextEditor& editor) override;

    //  (       )
    void textEditorReturnKeyPressed (TextEditor& editor) override;

    void textEditorEscapeKeyPressed (TextEditor& editor) override;

    void textEditorFocusLost (TextEditor& editor) override;

private:
    TextBoxSlider* test;

    TextBoxSlider* ccNo;

public:
    void labelTextChanged (Label* labelThatHasChanged) override;

private:
    TextBoxSlider* channel;

    TextBoxSlider* value;

    juce::Label* labCc;

    juce::Label* labCh;

    juce::Colour lineColour;
};
