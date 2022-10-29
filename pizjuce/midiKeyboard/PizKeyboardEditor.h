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
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>

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
class midiKeyboardEditor : public juce::AudioProcessorEditor,
                           public juce::ChangeListener,
                           public juce::KeyListener,
                           public juce::Slider::Listener,
                           public juce::Button::Listener
{
public:
    //==============================================================================
    midiKeyboardEditor (PizKeyboard* const ownerFilter);
    ~midiKeyboardEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    friend class PizKeyboardComponent;
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void mouseUp (const juce::MouseEvent& e) override;
    bool keyPressed (const juce::KeyPress& key, Component* originatingComponent) override
    {
        DBG (juce::String (key.getKeyCode()) + " " + key.getTextDescription());
        return false;
    }
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    juce::ComponentBoundsConstrainer resizeLimits;
    juce::TooltipWindow tooltipWindow;
    void updateParametersFromFilter();
    PizKeyboard* getFilter() const throw() { return (PizKeyboard*) getAudioProcessor(); }
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> keyWidthSlider;
    std::unique_ptr<juce::Slider> chSlider;
    std::unique_ptr<juce::TextButton> grabQwertyButton;
    std::unique_ptr<juce::Slider> velocitySlider;
    std::unique_ptr<juce::ToggleButton> yButton;
    std::unique_ptr<juce::ToggleButton> inputToggleButton;
    std::unique_ptr<juce::TextButton> aboutButton;
    std::unique_ptr<juce::TextButton> hideButton;
    std::unique_ptr<juce::ResizableCornerComponent> resizer;
    std::unique_ptr<PizKeyboardComponent> midiKeyboard;
    std::unique_ptr<juce::TextEditor> aboutBox;
    std::unique_ptr<juce::ToggleButton> useProgCh;
    std::unique_ptr<juce::ToggleButton> useCapsLock;
    std::unique_ptr<juce::TextButton> sendState;
    std::unique_ptr<juce::ToggleButton> showNumbersButton;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (midiKeyboardEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
