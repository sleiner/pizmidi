#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

#include "ModulePaneModel.h"
#include "ModuleGUI.h"
#include "Scene.h"
#include "TextBoxSlider.h"

class Cursor;
class MidiMorph;
class Module;

using namespace juce;

class MorphPaneModel : public ModulePaneModel,
                       public Label::Listener,
                       public Component
{
  private:
    Cursor* cursor;
    MidiMorph* core;
    TextBoxSlider* rateBox;

  public:
    MorphPaneModel(MidiMorph * core);

    int getNumModules() override;

    void mouseDown(const MouseEvent & e) override;
    void mouseUp(const MouseEvent & e) override;
    void mouseDrag(const MouseEvent & e) override;

    Module* getModule(int index) override;
    ModuleGUI* createGUI(int index) override;

    void selectionChanged(const Array<ModuleGUI*> & modules) override;

    void labelTextChanged(Label * labelThatHasChanged) override;
};
