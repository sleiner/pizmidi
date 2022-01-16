#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_gui_extra/juce_gui_extra.h"

#include "Module.h"

class MidiMorph;
class ControllerValue;
class Controller;
namespace juce
{
class MidiBuffer;
}
class Cursor;

using namespace juce;
class Scene : public Module,
              public Component,
              public ChangeListener,
              public Slider::Listener,
              public TextEditor::Listener
{
public:
    Scene (const Scene& scene);

    Scene (MidiMorph* core);

    ~Scene() override;

    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void textEditorTextChanged (TextEditor&) override;
    void textEditorReturnKeyPressed (TextEditor&) override;
    void textEditorEscapeKeyPressed (TextEditor&) override;
    void textEditorFocusLost (TextEditor&) override;
    void changeListenerCallback (ChangeBroadcaster* source) override;

private:
    //stored for performance reason
    float distanceFromCursor;

    float affectionRatio;

    float affectionValue;

    bool distanceFromCursorChanged_;

    bool affectionRatioChanged;

    bool affectionValueChanged;

    MidiMorph* core;

    TextEditor* textEditor;
    ColourSelector* colourSelector;
    Slider* sizeSlider;

public:
    Array<ControllerValue*> controllerValues;
    float getAffectionRatio();

    //get Precalculated Value
    float getDistanceFromCursor();

    int getValue (const Controller* controller);

    //gets called when the XYItem gets new coordinates
    //in this implementation refresh the distance value
    void moved() override;

    juce::Colour getColour();

    void setColour (const juce::Colour& colour);

    String getName();

    void setName (String newName);

    void addValue (ControllerValue* value);

    float getAffectionValue();

    void distanceFromCursorChanged();

    void mouseDown (const MouseEvent& e) override;

    void mouseUp (const MouseEvent& e) override;

    void getMidiMessages (juce::MidiBuffer& buffer, int pos);

    void mouseDrag (const MouseEvent& e) override;

    int getId();

    friend class ControllerValue;
    int id;
};
