#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "ModuleGUI.h"
#include "ModulePaneModel.h"
#include "Scene.h"
#include "TextBoxSlider.h"

class Cursor;
class MidiMorph;
class Module;

class MorphPaneModel : public ModulePaneModel,
                       public juce::Label::Listener,
                       public juce::Component
{
public:
    MorphPaneModel (MidiMorph* core);

    int getNumModules() override;

    void mouseDown (const juce::MouseEvent& e) override;
    void mouseUp (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;

    Module* getModule (int index) override;
    ModuleGUI* createGUI (int index) override;

    void selectionChanged (const juce::Array<ModuleGUI*>& modules) override;

    void labelTextChanged (juce::Label* labelThatHasChanged) override;

private:
    Cursor* cursor;
    MidiMorph* core;
    TextBoxSlider* rateBox;
};
