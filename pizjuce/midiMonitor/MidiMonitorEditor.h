/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  28 Sep 2011 11:07:39am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_MIDIMONITOREDITOR_MIDIMONITOREDITOR_B4642E5D__
#define __JUCER_HEADER_MIDIMONITOREDITOR_MIDIMONITOREDITOR_B4642E5D__

//[Headers]     -- You can add your own extra header files here --
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "MidiMonitor.h"
//[/Headers]

using namespace juce;

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiMonitorEditor  : public AudioProcessorEditor,
                           public ChangeListener,
                           public Timer,
                           public Button::Listener,
                           public Slider::Listener
{
public:
    //==============================================================================
    MidiMonitorEditor (MidiMonitorPlugin* const ownerFilter);
    ~MidiMonitorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback (ChangeBroadcaster* source);
	void mouseDown(const MouseEvent &e);
    void timerCallback ();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	Slider* maxLinesEditor;
    ComponentBoundsConstrainer resizeLimits;
    TooltipWindow tooltipWindow;
    Colour bgcolor, fgcolor;
    bool showbytes;
	bool showtime;
	int timemode;
	int numLines;
	int maxLines;

    void updateParametersFromFilter();
    MidiMonitorPlugin* getFilter() const throw()
	{
		return (MidiMonitorPlugin*) getAudioProcessor();
	}

	const String ppqToString (const double sppq, const int numerator, const int denominator, const double bpm);
    //[/UserVariables]

    //==============================================================================
    TextButton* clearButton;
    ToggleButton* powerButton;
    ToggleButton* thruButton;
    ToggleButton* bytesButton;
    Slider* hueSlider;
    Slider* saturationSlider;
    Slider* lightnessSlider;
    ResizableCornerComponent* resizer;
    TextButton* saveButton;
    TextButton* menuButton;
    TextEditor* midiOutputEditor;
    ToggleButton* wrapButton;
    ToggleButton* timeButton;
    TextButton* colorButton;
    TextEditor* aboutBox;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    MidiMonitorEditor (const MidiMonitorEditor&);
    const MidiMonitorEditor& operator= (const MidiMonitorEditor&);
};


#endif   // __JUCER_HEADER_MIDIMONITOREDITOR_MIDIMONITOREDITOR_B4642E5D__
