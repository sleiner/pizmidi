#pragma once

#include "TextBoxSlider.h"

#include <juce_gui_basics/juce_gui_basics.h>

class Controller;
class MidiMorph;

class ControllerGUI : public juce::TextEditor::Listener, public juce::ChangeListener, public juce::Component, public juce::Slider::Listener, public juce::Label::Listener
{
public:
    ControllerGUI(Controller* controller, MidiMorph* core);

    ~ControllerGUI() override;

    void resized() override;

    void paint(juce::Graphics& g) override;

    void paintOverChildren(juce::Graphics& g) override;

    void mouseUp(const juce::MouseEvent& e) override;

    void sliderValueChanged(juce::Slider* slider) override;

    void setSelected(bool shouldDrawSelected);

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void refreshControllerData();

    void textEditorTextChanged(juce::TextEditor& editor) override;

    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;

    void textEditorEscapeKeyPressed(juce::TextEditor& editor) override;

    void textEditorFocusLost(juce::TextEditor& editor) override;

    void labelTextChanged(juce::Label* labelThatHasChanged) override;

private:
    bool isSelected;

    Controller* controller;

    MidiMorph* core;

    juce::TextEditor* name;

    TextBoxSlider* test;
    TextBoxSlider* ccNo;
    TextBoxSlider* channel;
    TextBoxSlider* value;

    juce::Label* labCc;
    juce::Label* labCh;
};
