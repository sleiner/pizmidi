/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  14 Sep 2011 10:45:23am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_MIDIOUTEDITOR_MIDIOUTEDITOR_3F8B31BF__
#define __JUCER_HEADER_MIDIOUTEDITOR_MIDIOUTEDITOR_3F8B31BF__

//[Headers]     -- You can add your own extra header files here --
#include "juce_gui_basics/juce_gui_basics.h"

#include "midiOut.h"
#include "MidiPad.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiOutEditor  : public AudioProcessorEditor,
                       public ChangeListener,
                       public Button::Listener,
                       public ComboBox::Listener
{
public:
    //==============================================================================
    MidiOutEditor (MidiOutFilter* const ownerFilter);
    ~MidiOutEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback (ChangeBroadcaster* source);
    void buttonStateChanged (Button* buttonThatWasClicked);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void buttonClicked (Button* buttonThatWasClicked);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void updateParametersFromFilter();
    MidiOutFilter* getFilter() const throw()       { return (MidiOutFilter*) getAudioProcessor(); }
    //[/UserVariables]

    //==============================================================================
    ComboBox* comboBox;
    ToggleButton* clockButton;
    ToggleButton* mtcButton;
    ToggleButton* hostButton;
    ComboBox* channelBox;
    MidiPad* imagepad;
    Label* label;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    MidiOutEditor (const MidiOutEditor&);
    const MidiOutEditor& operator= (const MidiOutEditor&);
};


#endif   // __JUCER_HEADER_MIDIOUTEDITOR_MIDIOUTEDITOR_3F8B31BF__
