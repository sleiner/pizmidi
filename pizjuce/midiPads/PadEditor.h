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
#include "juce_gui_extra/juce_gui_extra.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PadEditor  : public juce::Component,
                   public juce::Button::Listener,
                   public juce::Slider::Listener
{
public:
    //==============================================================================
    PadEditor ();
    ~PadEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextEditor> textEditor;
    std::unique_ptr<juce::TextButton> textButton;
    std::unique_ptr<juce::TextButton> textButton2;
    std::unique_ptr<juce::ColourSelector> component;
    std::unique_ptr<juce::ToggleButton> toggleButton;
    std::unique_ptr<juce::ToggleButton> toggleButton2;
    std::unique_ptr<juce::ToggleButton> toggleButton3;
    std::unique_ptr<juce::Slider> slider;
    std::unique_ptr<juce::Slider> slider2;
    std::unique_ptr<juce::ToggleButton> toggleButton4;
    std::unique_ptr<juce::ToggleButton> toggleButton5;
    std::unique_ptr<juce::ToggleButton> toggleButton6;
    std::unique_ptr<juce::ToggleButton> toggleButton7;
    std::unique_ptr<juce::ToggleButton> toggleButton8;
    std::unique_ptr<juce::ToggleButton> toggleButton9;
    std::unique_ptr<juce::ToggleButton> toggleButton10;
    std::unique_ptr<juce::Slider> slider3;
    std::unique_ptr<juce::Slider> slider4;
    std::unique_ptr<juce::Slider> slider5;
    std::unique_ptr<juce::Slider> slider6;
    std::unique_ptr<juce::Slider> slider7;
    std::unique_ptr<juce::Slider> slider8;
    std::unique_ptr<juce::Slider> slider9;
    std::unique_ptr<juce::TextButton> textButton3;
    std::unique_ptr<juce::TextButton> textButton4;
    std::unique_ptr<juce::TextButton> textButton5;
    std::unique_ptr<juce::TextButton> textButton6;
    std::unique_ptr<juce::TextButton> textButton7;
    std::unique_ptr<juce::TextButton> textButton8;
    std::unique_ptr<juce::TextButton> textButton9;
    std::unique_ptr<juce::Label> label;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<juce::Label> label5;
    std::unique_ptr<juce::Label> label6;
    std::unique_ptr<juce::Label> label7;
    std::unique_ptr<juce::Label> label8;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PadEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

