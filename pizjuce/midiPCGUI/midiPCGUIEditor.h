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
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_events/juce_events.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "../_common/ClickableLabel.h"
#include "../_common/VSTSlider.h"
#include "midiPCGUI.h"
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class midiPCGUIEditor : public juce::AudioProcessorEditor,
                        public juce::ChangeListener,
                        public ClickableLabelListener,
                        public juce::Slider::Listener,
                        public juce::Button::Listener,
                        public juce::Label::Listener
{
public:
    //==============================================================================
    midiPCGUIEditor (midiPCGUI* const ownerFilter);
    ~midiPCGUIEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void clickableLabelMouseDown (ClickableLabel* label, const MouseEvent& e) override {}
    void clickableLabelMouseDoubleClick (ClickableLabel* label, const MouseEvent& e) override { label->edit(); }
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void labelTextChanged (juce::Label* labelThatHasChanged) override;

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
    std::unique_ptr<juce::GroupComponent> groupComponent2;
    std::unique_ptr<juce::GroupComponent> groupComponent;
    std::unique_ptr<VSTSlider> s_Program;
    std::unique_ptr<VSTSlider> s_BankMSB;
    std::unique_ptr<VSTSlider> s_BankLSB;
    std::unique_ptr<juce::TextButton> b_BankTrig;
    std::unique_ptr<juce::TextButton> b_PCTrig;
    std::unique_ptr<juce::TextButton> b_Inc;
    std::unique_ptr<juce::TextButton> b_Dec;
    std::unique_ptr<juce::ToggleButton> b_PCListen;
    std::unique_ptr<VSTSlider> s_Channel;
    std::unique_ptr<juce::TextButton> b_Mode;
    std::unique_ptr<juce::Label> PCDisplay;
    std::unique_ptr<juce::Label> label;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<juce::Label> label5;
    std::unique_ptr<juce::ToggleButton> b_Thru;
    std::unique_ptr<juce::Label> PCDisplay2;
    std::unique_ptr<juce::Label> PCDisplay3;
    std::unique_ptr<juce::Label> label6;
    std::unique_ptr<ClickableLabel> ProgramName;
    std::unique_ptr<juce::TextButton> minimize;
    juce::Path internalPath1;
    juce::Path internalPath2;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (midiPCGUIEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
