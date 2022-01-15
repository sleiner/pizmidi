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

#include "MidiMonitor.h"
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiMonitorEditor : public juce::AudioProcessorEditor,
                          public juce::ChangeListener,
                          public juce::Timer,
                          public juce::Button::Listener,
                          public juce::Slider::Listener
{
public:
    //==============================================================================
    MidiMonitorEditor (MidiMonitorPlugin* const ownerFilter);
    ~MidiMonitorEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void mouseDown (const MouseEvent& e) override;
    void timerCallback() override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;

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
    std::unique_ptr<juce::TextButton> clearButton;
    std::unique_ptr<juce::ToggleButton> powerButton;
    std::unique_ptr<juce::ToggleButton> thruButton;
    std::unique_ptr<juce::ToggleButton> bytesButton;
    std::unique_ptr<juce::Slider> hueSlider;
    std::unique_ptr<juce::Slider> saturationSlider;
    std::unique_ptr<juce::Slider> lightnessSlider;
    std::unique_ptr<ResizableCornerComponent> resizer;
    std::unique_ptr<juce::TextButton> saveButton;
    std::unique_ptr<juce::TextButton> menuButton;
    std::unique_ptr<juce::TextEditor> midiOutputEditor;
    std::unique_ptr<juce::ToggleButton> wrapButton;
    std::unique_ptr<juce::ToggleButton> timeButton;
    std::unique_ptr<juce::TextButton> colorButton;
    std::unique_ptr<juce::TextEditor> aboutBox;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiMonitorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
