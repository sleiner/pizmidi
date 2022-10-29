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
#include "../_common/ChannelSlider.h"
#include "PianoRoll.h"
#include "step.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>

class PianoPort : public juce::Viewport
{
public:
    PianoPort(juce::String name)
        : Viewport(name){};

    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& d) override
    {
        this->getParentComponent()->mouseWheelMove(e, d);
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
class StepEditor : public juce::AudioProcessorEditor,
                   public juce::ChangeListener,
                   public juce::FileDragAndDropTarget,
                   public juce::Timer,
                   public juce::Button::Listener,
                   public juce::Slider::Listener
{
public:
    //==============================================================================
    StepEditor(MidiStep* const ownerFilter);
    ~StepEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& filenames, int mouseX, int mouseY) override;
    void timerCallback() override;
    void zoomIn(int centerPixel);
    void zoomOut(int centerPixel);
    void mouseWheelMove(const juce::MouseEvent& e, float wheelIncrementX, float wheelIncrementY);
    //[/UserMethods]

    void paint(juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged(juce::Slider* sliderThatWasMoved) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    MidiStep* getFilter() const throw()
    {
        return (MidiStep*) getAudioProcessor();
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void updateParameters(bool updateLoop = false);
    juce::ComponentBoundsConstrainer resizeLimits;
    PianoRoll* pianoRoll;
    juce::TextButton* getButtonByIndex(int i);
    void recArmButtonClicked(juce::Button* buttonThatWasClicked);
    int lastActiveLoop;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::GroupComponent> activeLoopLabel;
    std::unique_ptr<juce::ResizableCornerComponent> resizer;
    std::unique_ptr<PianoPort> viewport;
    std::unique_ptr<juce::TextButton> recordButton;
    std::unique_ptr<ChannelSlider> outChannelSlider;
    std::unique_ptr<juce::TextButton> recArmButton1;
    std::unique_ptr<juce::TextButton> recArmButton2;
    std::unique_ptr<juce::TextButton> recArmButton3;
    std::unique_ptr<juce::TextButton> recArmButton4;
    std::unique_ptr<juce::TextButton> recArmButton5;
    std::unique_ptr<juce::TextButton> recArmButton6;
    std::unique_ptr<juce::TextButton> recArmButton7;
    std::unique_ptr<juce::TextButton> recArmButton8;
    std::unique_ptr<juce::Slider> keySlider;
    std::unique_ptr<juce::TextButton> recArmButton9;
    std::unique_ptr<juce::TextButton> recArmButton10;
    std::unique_ptr<juce::TextButton> recArmButton11;
    std::unique_ptr<juce::TextButton> recArmButton12;
    std::unique_ptr<juce::TextButton> recArmButton13;
    std::unique_ptr<juce::TextButton> recArmButton14;
    std::unique_ptr<juce::TextButton> recArmButton15;
    std::unique_ptr<juce::TextButton> recArmButton16;
    std::unique_ptr<ChannelSlider> recChannelSlider;
    std::unique_ptr<juce::Slider> transposeSlider;
    std::unique_ptr<juce::TextButton> saveButton;
    std::unique_ptr<PizButton> component;
    std::unique_ptr<juce::ToggleButton> toggleButton;
    std::unique_ptr<juce::TextButton> addBarButton;
    std::unique_ptr<juce::TextButton> deleteBarButton;
    std::unique_ptr<juce::ToggleButton> thruButton;
    juce::Path internalPath1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StepEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
