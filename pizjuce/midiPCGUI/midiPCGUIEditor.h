/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  14 Sep 2011 10:46:40am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_MIDIPCGUIEDITOR_MIDIPCGUIEDITOR_A4034B54__
#define __JUCER_HEADER_MIDIPCGUIEDITOR_MIDIPCGUIEDITOR_A4034B54__

//[Headers]     -- You can add your own extra header files here --
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "midiPCGUI.h"
#include "../_common/VSTSlider.h"
#include "../_common/ClickableLabel.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class midiPCGUIEditor  : public AudioProcessorEditor,
                         public ChangeListener,
                         public ClickableLabelListener,
                         public juce::Slider::Listener,
                         public juce::Button::Listener,
                         public juce::Label::Listener
{
public:
    //==============================================================================
    midiPCGUIEditor (midiPCGUI* const ownerFilter);
    ~midiPCGUIEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void changeListenerCallback (ChangeBroadcaster* source);
	void clickableLabelMouseDown(ClickableLabel *label, const MouseEvent &e) {}
	void clickableLabelMouseDoubleClick(ClickableLabel *label, const MouseEvent &e) {label->edit();}
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);
    void labelTextChanged (Label* labelThatHasChanged);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
//    ResizableCornerComponent* resizer;
//    ComponentBoundsConstrainer resizeLimits;
    TooltipWindow tooltipWindow;
    void updateParametersFromFilter();

	bool minimized;


    // handy wrapper method to avoid having to cast the filter to a PizLooper
    // every time we need it..
    midiPCGUI* getFilter() const throw() { return (midiPCGUI*) getAudioProcessor(); }
    //[/UserVariables]

    //==============================================================================
    GroupComponent* groupComponent2;
    GroupComponent* groupComponent;
    VSTSlider* s_Program;
    VSTSlider* s_BankMSB;
    VSTSlider* s_BankLSB;
    TextButton* b_BankTrig;
    TextButton* b_PCTrig;
    TextButton* b_Inc;
    TextButton* b_Dec;
    ToggleButton* b_PCListen;
    VSTSlider* s_Channel;
    TextButton* b_Mode;
    Label* PCDisplay;
    Label* label;
    Label* label2;
    Label* label3;
    Label* label4;
    Label* label5;
    ToggleButton* b_Thru;
    Label* PCDisplay2;
    Label* PCDisplay3;
    Label* label6;
    ClickableLabel* ProgramName;
    TextButton* minimize;
    Path internalPath1;
    Path internalPath2;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    midiPCGUIEditor (const midiPCGUIEditor&);
    const midiPCGUIEditor& operator= (const midiPCGUIEditor&);
};


#endif   // __JUCER_HEADER_MIDIPCGUIEDITOR_MIDIPCGUIEDITOR_A4034B54__
