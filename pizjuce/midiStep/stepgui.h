/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  14 Sep 2011 10:46:54am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_STEPEDITOR_STEPGUI_2085C00C__
#define __JUCER_HEADER_STEPEDITOR_STEPGUI_2085C00C__

//[Headers]     -- You can add your own extra header files here --
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "step.h"
#include "PianoRoll.h"
#include "../_common/ChannelSlider.h"
class PianoPort : public Viewport
{
public:
	PianoPort(String name) : Viewport(name) {};
    void mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& d) override
	{
		this->getParentComponent()->mouseWheelMove(e,d);
	}
};
//[/Headers]

#include "../_common/PizButton.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class StepEditor  : public AudioProcessorEditor,
                    public ChangeListener,
                    public FileDragAndDropTarget,
                    public Timer,
                    public Button::Listener,
                    public Slider::Listener
{
public:
    //==============================================================================
    StepEditor (MidiStep* const ownerFilter);
    ~StepEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    bool isInterestedInFileDrag (const StringArray& files);
	void filesDropped (const StringArray& filenames, int mouseX, int mouseY);
	void timerCallback ();
	void zoomIn(int centerPixel);
	void zoomOut(int centerPixel);
	void mouseWheelMove (const MouseEvent &e, float wheelIncrementX, float wheelIncrementY);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    MidiStep* getFilter() const throw() { return (MidiStep*) getAudioProcessor(); }
	void changeListenerCallback (ChangeBroadcaster* source);
	void updateParameters(bool updateLoop=false);
    ComponentBoundsConstrainer resizeLimits;
    PianoRoll* pianoRoll;
	TextButton* getButtonByIndex(int i);
	void recArmButtonClicked(Button* buttonThatWasClicked);
	int lastActiveLoop;
    //[/UserVariables]

    //==============================================================================
    GroupComponent* activeLoopLabel;
    ResizableCornerComponent* resizer;
    PianoPort* viewport;
    TextButton* recordButton;
    ChannelSlider* outChannelSlider;
    TextButton* recArmButton1;
    TextButton* recArmButton2;
    TextButton* recArmButton3;
    TextButton* recArmButton4;
    TextButton* recArmButton5;
    TextButton* recArmButton6;
    TextButton* recArmButton7;
    TextButton* recArmButton8;
    Slider* keySlider;
    TextButton* recArmButton9;
    TextButton* recArmButton10;
    TextButton* recArmButton11;
    TextButton* recArmButton12;
    TextButton* recArmButton13;
    TextButton* recArmButton14;
    TextButton* recArmButton15;
    TextButton* recArmButton16;
    ChannelSlider* recChannelSlider;
    Slider* transposeSlider;
    TextButton* saveButton;
    PizButton* component;
    ToggleButton* toggleButton;
    TextButton* addBarButton;
    TextButton* deleteBarButton;
    ToggleButton* thruButton;
    Path internalPath1;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    StepEditor (const StepEditor&);
    const StepEditor& operator= (const StepEditor&);
};


#endif   // __JUCER_HEADER_STEPEDITOR_STEPGUI_2085C00C__
