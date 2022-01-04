/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  14 Sep 2011 10:45:59am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_CURVEEDITOR_CURVEGUI_2037A6EC__
#define __JUCER_HEADER_CURVEEDITOR_CURVEGUI_2037A6EC__

//[Headers]     -- You can add your own extra header files here --
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "curve.h"
#include "../_common/ChannelSlider.h"
#include "MidiEnvelope.h"
//[/Headers]

using namespace juce;

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class CurveEditor  : public AudioProcessorEditor,
                     public ChangeListener,
                     public Slider::Listener,
                     public Button::Listener
{
public:
    //==============================================================================
    CurveEditor (MidiCurve* const ownerFilter);
    ~CurveEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    MidiCurve* getFilter() const throw() { return (MidiCurve*) getAudioProcessor(); }
	void changeListenerCallback (ChangeBroadcaster* source);
	void updateParameters();
    ComponentBoundsConstrainer resizeLimits;
    //[/UserVariables]

    //==============================================================================
    MidiEnvelope* curve;
    Label* label;
    Label* label2;
    ResizableCornerComponent* resizer;
    ChannelSlider* channelSlider;
    Label* label3;
    TextButton* resetButton;
    TextButton* helpButton;
    Label* instructionsLabel;
    Label* label4;
    Slider* rangeSlider;
    Label* label5;
    Label* label6;
    Label* label7;
    Slider* rangeSlider2;
    Label* label8;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    CurveEditor (const CurveEditor&);
    const CurveEditor& operator= (const CurveEditor&);
};


#endif   // __JUCER_HEADER_CURVEEDITOR_CURVEGUI_2037A6EC__
