/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  14 Sep 2011 10:43:42am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_AUDIOTOCCEDITOR_AUDIOTOCCEDITOR_14735CEB__
#define __JUCER_HEADER_AUDIOTOCCEDITOR_AUDIOTOCCEDITOR_14735CEB__

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
	~DecibelMeter() {}

	String getTextFromValue(double value)
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
	~DecibelSlider() {}

	String getTextFromValue(double value)
	{
		return value==0.0 ? "-inf" : String (20.0 * log10(value),1) + " dB";
	}
	double getValueFromText(const String &text)
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
class AudioToCCEditor  : public AudioProcessorEditor,
                         public ChangeListener,
                         public Button::Listener,
                         public ComboBox::Listener,
                         public Slider::Listener,
                         public Timer
{
public:
    //==============================================================================
    AudioToCCEditor (AudioToCC* const ownerFilter);
    ~AudioToCCEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback (ChangeBroadcaster* source);
	void timerCallback();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	int peakcounter;
	void updateParametersFromFilter();
    AudioToCC* getFilter() const throw()       { return (AudioToCC*) getAudioProcessor(); }
    //[/UserVariables]

    //==============================================================================
    DecibelSlider* s_Thresh;
    Label* label20;
    TextButton* clipR;
    TextButton* clipL;
    ComboBox* comboBox;
    VSTSlider* s_LCC;
    VSTSlider* s_RCC;
    VSTSlider* s_Channel;
    TextButton* b_Stereo;
    VSTSlider* s_Rate;
    VSTSlider* s_Inertia;
    Label* label;
    Label* label2;
    Label* label3;
    Label* label4;
    Label* label5;
    Label* label6;
    DecibelSlider* s_Gain;
    Label* label7;
    Slider* s_IndicatorL;
    Slider* s_IndicatorR;
    ToggleButton* toggleButton;
    ToggleButton* toggleButton2;
    VSTSlider* s_Attack;
    VSTSlider* s_Release;
    Label* label8;
    Label* label9;
    TextButton* b_Mode;
    Label* label10;
    Label* label11;
    Label* label12;
    VSTSlider* s_GateLCC;
    VSTSlider* s_GateCCR;
    Label* label13;
    Label* label14;
    VSTSlider* s_OnValueL;
    VSTSlider* s_OnValueR;
    Label* label15;
    Label* label16;
    VSTSlider* s_OffValueL;
    VSTSlider* s_OffValueR;
    Label* label17;
    Label* label18;
    Label* label19;
    DecibelSlider* s_PeakGain;
    DecibelMeter* s_IndicatorRIn;
    DecibelMeter* s_IndicatorLIn;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    AudioToCCEditor (const AudioToCCEditor&);
    const AudioToCCEditor& operator= (const AudioToCCEditor&);
};


#endif   // __JUCER_HEADER_AUDIOTOCCEDITOR_AUDIOTOCCEDITOR_14735CEB__
