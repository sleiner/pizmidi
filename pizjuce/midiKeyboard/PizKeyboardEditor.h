/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  31 Oct 2011 7:13:02am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_MIDIKEYBOARDEDITOR_PIZKEYBOARDEDITOR_B0E33023__
#define __JUCER_HEADER_MIDIKEYBOARDEDITOR_PIZKEYBOARDEDITOR_B0E33023__

//[Headers]     -- You can add your own extra header files here --
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "PizKeyboard.h"
#include "PizKeyboardComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class midiKeyboardEditor  : public AudioProcessorEditor,
                            public ChangeListener,
                            public KeyListener,
                            public Slider::Listener,
                            public Button::Listener
{
public:
    //==============================================================================
    midiKeyboardEditor (PizKeyboard* const ownerFilter);
    ~midiKeyboardEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	friend class PizKeyboardComponent;
	void changeListenerCallback (ChangeBroadcaster* source);
	void mouseUp(const MouseEvent &e);
	bool keyPressed(const KeyPress &key, Component* originatingComponent)
	{
		DBG(String(key.getKeyCode()) + " " + key.getTextDescription());
		return false;
	}
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ComponentBoundsConstrainer resizeLimits;
    TooltipWindow tooltipWindow;
    void updateParametersFromFilter();
    PizKeyboard* getFilter() const throw()       { return (PizKeyboard*) getAudioProcessor(); }
    //[/UserVariables]

    //==============================================================================
    Slider* keyWidthSlider;
    Slider* chSlider;
    TextButton* grabQwertyButton;
    Slider* velocitySlider;
    ToggleButton* yButton;
    ToggleButton* inputToggleButton;
    TextButton* aboutButton;
    TextButton* hideButton;
    ResizableCornerComponent* resizer;
    PizKeyboardComponent* midiKeyboard;
    TextEditor* aboutBox;
    ToggleButton* useProgCh;
    ToggleButton* useCapsLock;
    TextButton* sendState;
    ToggleButton* showNumbersButton;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    midiKeyboardEditor (const midiKeyboardEditor&);
    const midiKeyboardEditor& operator= (const midiKeyboardEditor&);
};


#endif   // __JUCER_HEADER_MIDIKEYBOARDEDITOR_PIZKEYBOARDEDITOR_B0E33023__
