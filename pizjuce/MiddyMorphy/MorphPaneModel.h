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

    virtual int getNumModules();

    void mouseDown(const MouseEvent & e);
    void mouseUp(const MouseEvent & e);
    void mouseDrag(const MouseEvent & e);

    virtual Module* getModule(int index);
    virtual ModuleGUI* createGUI(int index);

    void selectionChanged(const Array<ModuleGUI*> & modules);

    void labelTextChanged(Label * labelThatHasChanged);
};
