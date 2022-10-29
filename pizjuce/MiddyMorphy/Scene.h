#pragma once

#include "Module.h"

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

class MidiMorph;
class ControllerValue;
class Controller;
class Cursor;

class Scene : public Module,
              public juce::Component,
              public juce::ChangeListener,
              public juce::Slider::Listener,
              public juce::TextEditor::Listener
{
public:
    Scene(const Scene& scene);

    Scene(MidiMorph* core);

    ~Scene() override;

    void sliderValueChanged(juce::Slider* sliderThatWasMoved) override;
    void textEditorTextChanged(juce::TextEditor&) override;
    void textEditorReturnKeyPressed(juce::TextEditor&) override;
    void textEditorEscapeKeyPressed(juce::TextEditor&) override;
    void textEditorFocusLost(juce::TextEditor&) override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    juce::Array<ControllerValue*> controllerValues;
    float getAffectionRatio();

    float getDistanceFromCursor();

    int getValue(const Controller* controller);

    void moved() override;

    juce::Colour getColour();

    void setColour(const juce::Colour& colour);

    juce::String getName();

    void setName(juce::String newName);

    void addValue(ControllerValue* value);

    float getAffectionValue();

    void distanceFromCursorChanged();

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;

    void getMidiMessages(juce::MidiBuffer& buffer, int pos);

    int getId();

    friend class ControllerValue;
    int id;

private:
    float distanceFromCursor;

    float affectionRatio;
    float affectionValue;

    bool distanceFromCursorChanged_;

    bool affectionRatioChanged;
    bool affectionValueChanged;

    MidiMorph* core;

    juce::TextEditor* textEditor;
    juce::ColourSelector* colourSelector;
    juce::Slider* sizeSlider;
};
