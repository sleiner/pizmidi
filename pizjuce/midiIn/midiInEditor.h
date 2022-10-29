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

#include "midiIn.h"
#include "MidiPad.h"

//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiInEditor : public juce::AudioProcessorEditor,
                     public juce::ChangeListener,
                     public juce::ComboBox::Listener,
                     public juce::Button::Listener
{
public:
    //==============================================================================
    MidiInEditor(MidiInFilter* const ownerFilter);
    ~MidiInEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void buttonStateChanged(juce::Button* buttonThatWasClicked) override;
    //[/UserMethods]

    void paint(juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked(juce::Button* buttonThatWasClicked) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void updateParametersFromFilter();

    MidiInFilter* getFilter() const throw()
    {
        return (MidiInFilter*) getAudioProcessor();
    }

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ComboBox> comboBox;
    std::unique_ptr<juce::ToggleButton> hostButton;
    std::unique_ptr<juce::ComboBox> channelBox;
    std::unique_ptr<MidiPad> imagepad;
    std::unique_ptr<juce::Label> label;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiInEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
