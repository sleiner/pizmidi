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
#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "AudioToCC.h"
#include "../_common/VSTSlider.h"

class DecibelMeter : public Slider
{
public:
	DecibelMeter(const String &name) : Slider(name) {
		this->setMouseClickGrabsKeyboardFocus(false);
		this->setInterceptsMouseClicks(false,false);
	}
	~DecibelMeter() override {}

	String getTextFromValue(double value) override
	{
		return value==0.0 ? " " : String (20.0 * log10(value),1) + " dB";
	}
};

class DecibelSlider : public Slider
{
public:
	DecibelSlider(const String &name) : Slider(name) {
		this->setMouseClickGrabsKeyboardFocus(false);
	}
	~DecibelSlider() override {}

	String getTextFromValue(double value) override
	{
		return value==0.0 ? "-inf" : String (20.0 * log10(value),1) + " dB";
	}
	double getValueFromText(const String &text) override
	{
		if (text.equalsIgnoreCase("-inf"))
			return 0.0;
		double dB = text.getDoubleValue();
		return pow(10,dB/20.0);
	}
};
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AudioToCCEditor  : public juce::AudioProcessorEditor,
                         public juce::ChangeListener,
                         public juce::Timer,
                         public juce::Slider::Listener,
                         public juce::Button::Listener,
                         public juce::ComboBox::Listener
{
public:
    //==============================================================================
    AudioToCCEditor (AudioToCC* const ownerFilter);
    ~AudioToCCEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback (ChangeBroadcaster* source) override;
	void timerCallback() override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	int peakcounter;
	void updateParametersFromFilter();
    AudioToCC* getFilter() const throw()       { return (AudioToCC*) getAudioProcessor(); }
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<DecibelSlider> s_Thresh;
    std::unique_ptr<juce::Label> label20;
    std::unique_ptr<juce::TextButton> clipR;
    std::unique_ptr<juce::TextButton> clipL;
    std::unique_ptr<juce::ComboBox> comboBox;
    std::unique_ptr<VSTSlider> s_LCC;
    std::unique_ptr<VSTSlider> s_RCC;
    std::unique_ptr<VSTSlider> s_Channel;
    std::unique_ptr<juce::TextButton> b_Stereo;
    std::unique_ptr<VSTSlider> s_Rate;
    std::unique_ptr<VSTSlider> s_Inertia;
    std::unique_ptr<juce::Label> label;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<juce::Label> label3;
    std::unique_ptr<juce::Label> label4;
    std::unique_ptr<juce::Label> label5;
    std::unique_ptr<juce::Label> label6;
    std::unique_ptr<DecibelSlider> s_Gain;
    std::unique_ptr<juce::Label> label7;
    std::unique_ptr<juce::Slider> s_IndicatorL;
    std::unique_ptr<juce::Slider> s_IndicatorR;
    std::unique_ptr<juce::ToggleButton> toggleButton;
    std::unique_ptr<juce::ToggleButton> toggleButton2;
    std::unique_ptr<VSTSlider> s_Attack;
    std::unique_ptr<VSTSlider> s_Release;
    std::unique_ptr<juce::Label> label8;
    std::unique_ptr<juce::Label> label9;
    std::unique_ptr<juce::TextButton> b_Mode;
    std::unique_ptr<juce::Label> label10;
    std::unique_ptr<juce::Label> label11;
    std::unique_ptr<juce::Label> label12;
    std::unique_ptr<VSTSlider> s_GateLCC;
    std::unique_ptr<VSTSlider> s_GateCCR;
    std::unique_ptr<juce::Label> label13;
    std::unique_ptr<juce::Label> label14;
    std::unique_ptr<VSTSlider> s_OnValueL;
    std::unique_ptr<VSTSlider> s_OnValueR;
    std::unique_ptr<juce::Label> label15;
    std::unique_ptr<juce::Label> label16;
    std::unique_ptr<VSTSlider> s_OffValueL;
    std::unique_ptr<VSTSlider> s_OffValueR;
    std::unique_ptr<juce::Label> label17;
    std::unique_ptr<juce::Label> label18;
    std::unique_ptr<juce::Label> label19;
    std::unique_ptr<DecibelSlider> s_PeakGain;
    std::unique_ptr<DecibelMeter> s_IndicatorRIn;
    std::unique_ptr<DecibelMeter> s_IndicatorLIn;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioToCCEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

