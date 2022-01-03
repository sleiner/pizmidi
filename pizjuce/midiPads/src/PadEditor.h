/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  27 Nov 2010 3:48:47pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_PADEDITOR_PADEDITOR_44E60834__
#define __JUCER_HEADER_PADEDITOR_PADEDITOR_44E60834__

//[Headers]     -- You can add your own extra header files here --
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_gui_extra/juce_gui_extra.h"
//[/Headers]

using namespace juce;

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PadEditor  : public Component,
                   public Button::Listener,
                   public Slider::Listener
{
public:
    //==============================================================================
    PadEditor ();
    ~PadEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    TextEditor* textEditor;
    TextButton* textButton;
    TextButton* textButton2;
    ColourSelector* component;
    ToggleButton* toggleButton;
    ToggleButton* toggleButton2;
    ToggleButton* toggleButton3;
    Slider* slider;
    Slider* slider2;
    ToggleButton* toggleButton4;
    ToggleButton* toggleButton5;
    ToggleButton* toggleButton6;
    ToggleButton* toggleButton7;
    ToggleButton* toggleButton8;
    ToggleButton* toggleButton9;
    ToggleButton* toggleButton10;
    Slider* slider3;
    Slider* slider4;
    Slider* slider5;
    Slider* slider6;
    Slider* slider7;
    Slider* slider8;
    Slider* slider9;
    TextButton* textButton3;
    TextButton* textButton4;
    TextButton* textButton5;
    TextButton* textButton6;
    TextButton* textButton7;
    TextButton* textButton8;
    TextButton* textButton9;
    Label* label;
    Label* label2;
    Label* label3;
    Label* label4;
    Label* label5;
    Label* label6;
    Label* label7;
    Label* label8;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    PadEditor (const PadEditor&);
    const PadEditor& operator= (const PadEditor&);
};


#endif   // __JUCER_HEADER_PADEDITOR_PADEDITOR_44E60834__
