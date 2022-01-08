/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.4

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "juce_gui_basics/juce_gui_basics.h"

#include "curve.h"
#include "MidiEnvelope.h"
#include "../_common/ChannelSlider.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class CurveEditor  : public AudioProcessorEditor,
                     public ChangeListener,
                     public juce::Button::Listener,
                     public juce::Slider::Listener
{
public:
    //==============================================================================
    CurveEditor (MidiCurve* const ownerFilter);
    ~CurveEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    MidiCurve* getFilter() const throw() { return (MidiCurve*) getAudioProcessor(); }
	void changeListenerCallback (ChangeBroadcaster* source);
	void updateParameters();
    ComponentBoundsConstrainer resizeLimits;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<MidiEnvelope> curve;
    std::unique_ptr<juce::Label> label;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<ResizableCornerComponent> resizer;
    std::unique_ptr<juce::ToggleButton> velocityButton;
    std::unique_ptr<juce::ToggleButton> ccButton;
    std::unique_ptr<juce::Slider> slider;
    std::unique_ptr<ChannelSlider> channelSlider;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<juce::ToggleButton> channelPressureButton;
    std::unique_ptr<juce::ToggleButton> aftertouchButton;
    std::unique_ptr<juce::TextButton> resetButton;
    std::unique_ptr<juce::TextButton> helpButton;
    std::unique_ptr<juce::Label> instructionsLabel;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<juce::Label> label5;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CurveEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

