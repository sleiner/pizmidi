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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "AudioToCCEditor.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AudioToCCEditor::AudioToCCEditor(AudioToCC* const ownerFilter)
    : AudioProcessorEditor(ownerFilter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    s_Thresh.reset(new DecibelSlider("slider"));
    addAndMakeVisible(s_Thresh.get());
    s_Thresh->setRange(0, 1, 0);
    s_Thresh->setSliderStyle(juce::Slider::LinearBar);
    s_Thresh->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
    s_Thresh->addListener(this);
    s_Thresh->setSkewFactor(0.6);

    s_Thresh->setBounds(32, 182, 152, 45);

    label20.reset(new juce::Label("new label",
                                  TRANS("Peak")));
    addAndMakeVisible(label20.get());
    label20->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label20->setJustificationType(juce::Justification::centred);
    label20->setEditable(false, false, false);
    label20->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label20->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label20->setBounds(112, 134, 72, 16);

    clipR.reset(new juce::TextButton("new button"));
    addAndMakeVisible(clipR.get());
    clipR->setButtonText(TRANS("gate"));
    clipR->setConnectedEdges(juce::Button::ConnectedOnLeft);
    clipR->addListener(this);
    clipR->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffc33333));
    clipR->setColour(juce::TextButton::textColourOffId, juce::Colours::white);

    clipR->setBounds(183, 211, 33, 16);

    clipL.reset(new juce::TextButton("new button"));
    addAndMakeVisible(clipL.get());
    clipL->setButtonText(TRANS("gate"));
    clipL->setConnectedEdges(juce::Button::ConnectedOnLeft);
    clipL->addListener(this);
    clipL->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffc33333));
    clipL->setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    clipL->setColour(juce::TextButton::textColourOnId, juce::Colours::black);

    clipL->setBounds(183, 182, 33, 16);

    comboBox.reset(new juce::ComboBox("new combo box"));
    addAndMakeVisible(comboBox.get());
    comboBox->setTooltip(TRANS("Output Device"));
    comboBox->setEditableText(false);
    comboBox->setJustificationType(juce::Justification::centredLeft);
    comboBox->setTextWhenNothingSelected(TRANS("--"));
    comboBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    comboBox->addListener(this);

    comboBox->setBounds(280, 41, 194, 16);

    s_LCC.reset(new VSTSlider("LCC slider"));
    addAndMakeVisible(s_LCC.get());
    s_LCC->setRange(-1, 127, 1);
    s_LCC->setSliderStyle(juce::Slider::LinearBar);
    s_LCC->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    s_LCC->addListener(this);

    s_LCC->setBounds(272, 104, 72, 16);

    s_RCC.reset(new VSTSlider("RCC slider"));
    addAndMakeVisible(s_RCC.get());
    s_RCC->setRange(-1, 127, 1);
    s_RCC->setSliderStyle(juce::Slider::LinearBar);
    s_RCC->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    s_RCC->addListener(this);

    s_RCC->setBounds(272, 128, 72, 16);

    s_Channel.reset(new VSTSlider("channel slider"));
    addAndMakeVisible(s_Channel.get());
    s_Channel->setRange(1, 16, 1);
    s_Channel->setSliderStyle(juce::Slider::LinearBar);
    s_Channel->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    s_Channel->addListener(this);

    s_Channel->setBounds(312, 61, 72, 16);

    b_Stereo.reset(new juce::TextButton("mono/stereo"));
    addAndMakeVisible(b_Stereo.get());
    b_Stereo->setButtonText(TRANS("Stereo"));
    b_Stereo->addListener(this);
    b_Stereo->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffbbbbff));

    b_Stereo->setBounds(400, 61, 72, 16);

    s_Rate.reset(new VSTSlider("rate slider"));
    addAndMakeVisible(s_Rate.get());
    s_Rate->setRange(0, 100, 1);
    s_Rate->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    s_Rate->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 16);
    s_Rate->addListener(this);

    s_Rate->setBounds(248, 240, 40, 48);

    s_Inertia.reset(new VSTSlider("inertia slider"));
    addAndMakeVisible(s_Inertia.get());
    s_Inertia->setRange(0, 100, 1);
    s_Inertia->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    s_Inertia->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 16);
    s_Inertia->addListener(this);

    s_Inertia->setBounds(296, 240, 40, 48);

    label.reset(new juce::Label("new label",
                                TRANS("MIDI Out Device:")));
    addAndMakeVisible(label.get());
    label->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label->setJustificationType(juce::Justification::centredLeft);
    label->setEditable(false, false, false);
    label->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label->setBounds(275, 29, 150, 8);

    label2.reset(new juce::Label("new label",
                                 TRANS("RMS")));
    addAndMakeVisible(label2.get());
    label2->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label2->setJustificationType(juce::Justification::centred);
    label2->setEditable(false, false, false);
    label2->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label2->setBounds(32, 132, 72, 16);

    label3.reset(new juce::Label("new label",
                                 TRANS("L CC:")));
    addAndMakeVisible(label3.get());
    label3->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label3->setJustificationType(juce::Justification::centredRight);
    label3->setEditable(false, false, false);
    label3->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label3->setBounds(232, 104, 40, 16);

    label4.reset(new juce::Label("new label",
                                 TRANS("R CC:")));
    addAndMakeVisible(label4.get());
    label4->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label4->setJustificationType(juce::Justification::centredRight);
    label4->setEditable(false, false, false);
    label4->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label4->setBounds(232, 128, 40, 16);

    label5.reset(new juce::Label("new label",
                                 TRANS("Ch:")));
    addAndMakeVisible(label5.get());
    label5->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label5->setJustificationType(juce::Justification::centredRight);
    label5->setEditable(false, false, false);
    label5->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label5->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label5->setBounds(272, 61, 40, 16);

    label6.reset(new juce::Label("new label",
                                 TRANS("Rate")));
    addAndMakeVisible(label6.get());
    label6->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label6->setJustificationType(juce::Justification::centred);
    label6->setEditable(false, false, false);
    label6->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label6->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label6->setBounds(248, 224, 40, 16);

    s_Gain.reset(new DecibelSlider("gain slider"));
    addAndMakeVisible(s_Gain.get());
    s_Gain->setRange(0, 32, 0);
    s_Gain->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    s_Gain->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 16);
    s_Gain->addListener(this);
    s_Gain->setSkewFactor(0.333);

    s_Gain->setBounds(32, 78, 72, 88);

    label7.reset(new juce::Label("new label",
                                 TRANS("Inertia")));
    addAndMakeVisible(label7.get());
    label7->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label7->setJustificationType(juce::Justification::centred);
    label7->setEditable(false, false, false);
    label7->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label7->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label7->setBounds(288, 224, 56, 16);

    s_IndicatorL.reset(new juce::Slider("new slider"));
    addAndMakeVisible(s_IndicatorL.get());
    s_IndicatorL->setRange(0, 127, 1);
    s_IndicatorL->setSliderStyle(juce::Slider::LinearBar);
    s_IndicatorL->setTextBoxStyle(juce::Slider::TextBoxLeft, true, 80, 20);
    s_IndicatorL->setColour(juce::Slider::backgroundColourId, juce::Colours::black);
    s_IndicatorL->setColour(juce::Slider::thumbColourId, juce::Colour(0xff7e7434));
    s_IndicatorL->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_IndicatorL->addListener(this);

    s_IndicatorL->setBounds(32, 236, 152, 24);

    s_IndicatorR.reset(new juce::Slider("new slider"));
    addAndMakeVisible(s_IndicatorR.get());
    s_IndicatorR->setRange(0, 127, 1);
    s_IndicatorR->setSliderStyle(juce::Slider::LinearBar);
    s_IndicatorR->setTextBoxStyle(juce::Slider::TextBoxLeft, true, 80, 20);
    s_IndicatorR->setColour(juce::Slider::backgroundColourId, juce::Colours::black);
    s_IndicatorR->setColour(juce::Slider::thumbColourId, juce::Colour(0xff7e7434));
    s_IndicatorR->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_IndicatorR->addListener(this);

    s_IndicatorR->setBounds(32, 262, 152, 24);

    toggleButton.reset(new juce::ToggleButton("new toggle button"));
    addAndMakeVisible(toggleButton.get());
    toggleButton->setButtonText(TRANS("Automation to host"));
    toggleButton->addListener(this);

    toggleButton->setBounds(48, 288, 120, 16);

    toggleButton2.reset(new juce::ToggleButton("new toggle button"));
    addAndMakeVisible(toggleButton2.get());
    toggleButton2->setButtonText(TRANS("MIDI to host"));
    toggleButton2->addListener(this);

    toggleButton2->setBounds(384, 25, 88, 16);

    s_Attack.reset(new VSTSlider("attack slider"));
    addAndMakeVisible(s_Attack.get());
    s_Attack->setRange(0, 100, 1);
    s_Attack->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    s_Attack->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 16);
    s_Attack->addListener(this);

    s_Attack->setBounds(248, 168, 40, 48);

    s_Release.reset(new VSTSlider("release slider"));
    addAndMakeVisible(s_Release.get());
    s_Release->setRange(0, 100, 1);
    s_Release->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    s_Release->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 16);
    s_Release->addListener(this);

    s_Release->setBounds(296, 168, 40, 48);

    label8.reset(new juce::Label("new label",
                                 TRANS("Attack")));
    addAndMakeVisible(label8.get());
    label8->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label8->setJustificationType(juce::Justification::centred);
    label8->setEditable(false, false, false);
    label8->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label8->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label8->setBounds(240, 152, 56, 16);

    label9.reset(new juce::Label("new label",
                                 TRANS("Release")));
    addAndMakeVisible(label9.get());
    label9->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label9->setJustificationType(juce::Justification::centred);
    label9->setEditable(false, false, false);
    label9->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label9->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label9->setBounds(288, 152, 56, 16);

    b_Mode.reset(new juce::TextButton("dB"));
    addAndMakeVisible(b_Mode.get());
    b_Mode->setButtonText(TRANS("Logarithmic"));
    b_Mode->addListener(this);
    b_Mode->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffbbbbff));

    b_Mode->setBounds(244, -21, 80, 16);

    label10.reset(new juce::Label("new label",
                                  TRANS("Scale:")));
    addAndMakeVisible(label10.get());
    label10->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label10->setJustificationType(juce::Justification::centredRight);
    label10->setEditable(false, false, false);
    label10->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label10->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label10->setBounds(196, -21, 46, 16);

    label11.reset(new juce::Label("new label",
                                  TRANS("L")));
    addAndMakeVisible(label11.get());
    label11->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label11->setJustificationType(juce::Justification::centredRight);
    label11->setEditable(false, false, false);
    label11->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label11->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label11->setBounds(16, 240, 16, 16);

    label12.reset(new juce::Label("new label",
                                  TRANS("R")));
    addAndMakeVisible(label12.get());
    label12->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label12->setJustificationType(juce::Justification::centredRight);
    label12->setEditable(false, false, false);
    label12->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label12->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label12->setBounds(16, 266, 16, 16);

    s_GateLCC.reset(new VSTSlider("LCC slider"));
    addAndMakeVisible(s_GateLCC.get());
    s_GateLCC->setRange(-1, 127, 1);
    s_GateLCC->setSliderStyle(juce::Slider::LinearBar);
    s_GateLCC->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    s_GateLCC->addListener(this);

    s_GateLCC->setBounds(432, 136, 72, 16);

    s_GateCCR.reset(new VSTSlider("RCC slider"));
    addAndMakeVisible(s_GateCCR.get());
    s_GateCCR->setRange(-1, 127, 1);
    s_GateCCR->setSliderStyle(juce::Slider::LinearBar);
    s_GateCCR->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    s_GateCCR->addListener(this);

    s_GateCCR->setBounds(432, 216, 72, 16);

    label13.reset(new juce::Label("new label",
                                  TRANS("Gate L CC:")));
    addAndMakeVisible(label13.get());
    label13->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label13->setJustificationType(juce::Justification::centredRight);
    label13->setEditable(false, false, false);
    label13->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label13->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label13->setBounds(368, 136, 64, 16);

    label14.reset(new juce::Label("new label",
                                  TRANS("Gate R CC:")));
    addAndMakeVisible(label14.get());
    label14->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label14->setJustificationType(juce::Justification::centredRight);
    label14->setEditable(false, false, false);
    label14->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label14->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label14->setBounds(368, 216, 64, 16);

    s_OnValueL.reset(new VSTSlider("LCC slider"));
    addAndMakeVisible(s_OnValueL.get());
    s_OnValueL->setRange(-1, 127, 1);
    s_OnValueL->setSliderStyle(juce::Slider::LinearBar);
    s_OnValueL->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    s_OnValueL->addListener(this);

    s_OnValueL->setBounds(432, 160, 72, 16);

    s_OnValueR.reset(new VSTSlider("RCC slider"));
    addAndMakeVisible(s_OnValueR.get());
    s_OnValueR->setRange(-1, 127, 1);
    s_OnValueR->setSliderStyle(juce::Slider::LinearBar);
    s_OnValueR->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    s_OnValueR->addListener(this);

    s_OnValueR->setBounds(432, 240, 72, 16);

    label15.reset(new juce::Label("new label",
                                  TRANS("On Value L:")));
    addAndMakeVisible(label15.get());
    label15->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label15->setJustificationType(juce::Justification::centredRight);
    label15->setEditable(false, false, false);
    label15->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label15->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label15->setBounds(360, 160, 72, 16);

    label16.reset(new juce::Label("new label",
                                  TRANS("On Value R:")));
    addAndMakeVisible(label16.get());
    label16->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label16->setJustificationType(juce::Justification::centredRight);
    label16->setEditable(false, false, false);
    label16->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label16->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label16->setBounds(360, 240, 72, 16);

    s_OffValueL.reset(new VSTSlider("LCC slider"));
    addAndMakeVisible(s_OffValueL.get());
    s_OffValueL->setRange(-1, 127, 1);
    s_OffValueL->setSliderStyle(juce::Slider::LinearBar);
    s_OffValueL->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    s_OffValueL->addListener(this);

    s_OffValueL->setBounds(432, 184, 72, 16);

    s_OffValueR.reset(new VSTSlider("RCC slider"));
    addAndMakeVisible(s_OffValueR.get());
    s_OffValueR->setRange(-1, 127, 1);
    s_OffValueR->setSliderStyle(juce::Slider::LinearBar);
    s_OffValueR->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    s_OffValueR->addListener(this);

    s_OffValueR->setBounds(432, 264, 72, 16);

    label17.reset(new juce::Label("new label",
                                  TRANS("Off Value L:")));
    addAndMakeVisible(label17.get());
    label17->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label17->setJustificationType(juce::Justification::centredRight);
    label17->setEditable(false, false, false);
    label17->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label17->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label17->setBounds(352, 184, 80, 16);

    label18.reset(new juce::Label("new label",
                                  TRANS("Off Value R:")));
    addAndMakeVisible(label18.get());
    label18->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label18->setJustificationType(juce::Justification::centredRight);
    label18->setEditable(false, false, false);
    label18->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label18->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label18->setBounds(352, 264, 80, 16);

    label19.reset(new juce::Label("new label",
                                  TRANS("Threshold:")));
    addAndMakeVisible(label19.get());
    label19->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label19->setJustificationType(juce::Justification::centredRight);
    label19->setEditable(false, false, false);
    label19->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label19->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label19->setBounds(-1, 196, 64, 16);

    s_PeakGain.reset(new DecibelSlider("gain slider"));
    addAndMakeVisible(s_PeakGain.get());
    s_PeakGain->setRange(0, 32, 0);
    s_PeakGain->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    s_PeakGain->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 16);
    s_PeakGain->addListener(this);
    s_PeakGain->setSkewFactor(0.333);

    s_PeakGain->setBounds(112, 78, 72, 88);

    s_IndicatorRIn.reset(new DecibelMeter("new slider"));
    addAndMakeVisible(s_IndicatorRIn.get());
    s_IndicatorRIn->setRange(0, 1, 0);
    s_IndicatorRIn->setSliderStyle(juce::Slider::LinearBar);
    s_IndicatorRIn->setTextBoxStyle(juce::Slider::TextBoxLeft, true, 80, 20);
    s_IndicatorRIn->setColour(juce::Slider::backgroundColourId, juce::Colours::black);
    s_IndicatorRIn->setColour(juce::Slider::thumbColourId, juce::Colour(0xff347e4c));
    s_IndicatorRIn->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_IndicatorRIn->addListener(this);
    s_IndicatorRIn->setSkewFactor(0.6);

    s_IndicatorRIn->setBounds(32, 211, 152, 16);

    s_IndicatorLIn.reset(new DecibelMeter("new slider"));
    addAndMakeVisible(s_IndicatorLIn.get());
    s_IndicatorLIn->setRange(0, 1, 0);
    s_IndicatorLIn->setSliderStyle(juce::Slider::LinearBar);
    s_IndicatorLIn->setTextBoxStyle(juce::Slider::TextBoxLeft, true, 80, 20);
    s_IndicatorLIn->setColour(juce::Slider::backgroundColourId, juce::Colours::black);
    s_IndicatorLIn->setColour(juce::Slider::thumbColourId, juce::Colour(0xff347e4c));
    s_IndicatorLIn->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_IndicatorLIn->addListener(this);
    s_IndicatorLIn->setSkewFactor(0.6);

    s_IndicatorLIn->setBounds(32, 182, 152, 16);

    //[UserPreSize]
    setMouseClickGrabsKeyboardFocus(false);

    s_LCC->setMouseClickGrabsKeyboardFocus(false);
    s_RCC->setMouseClickGrabsKeyboardFocus(false);
    s_Channel->setMouseClickGrabsKeyboardFocus(false);
    s_Rate->setMouseClickGrabsKeyboardFocus(false);
    s_Inertia->setMouseClickGrabsKeyboardFocus(false);
    s_Gain->setMouseClickGrabsKeyboardFocus(false);
    s_Attack->setMouseClickGrabsKeyboardFocus(false);
    s_Release->setMouseClickGrabsKeyboardFocus(false);
    label->setMouseClickGrabsKeyboardFocus(false);
    label2->setMouseClickGrabsKeyboardFocus(false);
    label3->setMouseClickGrabsKeyboardFocus(false);
    label4->setMouseClickGrabsKeyboardFocus(false);
    label5->setMouseClickGrabsKeyboardFocus(false);
    label6->setMouseClickGrabsKeyboardFocus(false);
    label7->setMouseClickGrabsKeyboardFocus(false);
    label8->setMouseClickGrabsKeyboardFocus(false);
    label9->setMouseClickGrabsKeyboardFocus(false);
    label10->setMouseClickGrabsKeyboardFocus(false);
    label11->setMouseClickGrabsKeyboardFocus(false);
    label12->setMouseClickGrabsKeyboardFocus(false);
    s_IndicatorL->setMouseClickGrabsKeyboardFocus(false);
    s_IndicatorR->setMouseClickGrabsKeyboardFocus(false);
    b_Stereo->setMouseClickGrabsKeyboardFocus(false);
    b_Mode->setMouseClickGrabsKeyboardFocus(false);
    toggleButton->setMouseClickGrabsKeyboardFocus(false);
    toggleButton2->setMouseClickGrabsKeyboardFocus(false);
    comboBox->setMouseClickGrabsKeyboardFocus(false);
    s_GateLCC->setMouseClickGrabsKeyboardFocus(false);
    s_GateCCR->setMouseClickGrabsKeyboardFocus(false);
    s_OnValueL->setMouseClickGrabsKeyboardFocus(false);
    s_OnValueR->setMouseClickGrabsKeyboardFocus(false);
    s_OffValueL->setMouseClickGrabsKeyboardFocus(false);
    s_OffValueR->setMouseClickGrabsKeyboardFocus(false);
    s_Thresh->setMouseClickGrabsKeyboardFocus(false);
    s_IndicatorLIn->setMouseClickGrabsKeyboardFocus(false);
    s_IndicatorRIn->setMouseClickGrabsKeyboardFocus(false);
    label13->setMouseClickGrabsKeyboardFocus(false);
    label14->setMouseClickGrabsKeyboardFocus(false);
    label15->setMouseClickGrabsKeyboardFocus(false);
    label16->setMouseClickGrabsKeyboardFocus(false);
    label17->setMouseClickGrabsKeyboardFocus(false);
    label18->setMouseClickGrabsKeyboardFocus(false);
    label19->setMouseClickGrabsKeyboardFocus(false);
    clipR->setMouseClickGrabsKeyboardFocus(false);
    clipL->setMouseClickGrabsKeyboardFocus(false);

    s_IndicatorL->setInterceptsMouseClicks(false, false);
    s_IndicatorR->setInterceptsMouseClicks(false, false);
    s_IndicatorLIn->setInterceptsMouseClicks(false, false);
    s_IndicatorRIn->setInterceptsMouseClicks(false, false);

    s_LCC->setSliderSnapsToMousePosition(false);
    s_RCC->setSliderSnapsToMousePosition(false);
    s_Channel->setSliderSnapsToMousePosition(false);
    s_GateLCC->setSliderSnapsToMousePosition(false);
    s_GateCCR->setSliderSnapsToMousePosition(false);
    s_OnValueL->setSliderSnapsToMousePosition(false);
    s_OnValueR->setSliderSnapsToMousePosition(false);
    s_OffValueL->setSliderSnapsToMousePosition(false);
    s_OffValueR->setSliderSnapsToMousePosition(false);
    s_Thresh->setSliderSnapsToMousePosition(true);

    s_LCC->setOwner(getAudioProcessor(), kCCL);
    s_RCC->setOwner(getAudioProcessor(), kCCR);
    s_Channel->setOwner(getAudioProcessor(), kChannel);
    s_Rate->setOwner(getAudioProcessor(), kRate);
    s_Inertia->setOwner(getAudioProcessor(), kSmooth);
    //s_Gain->setOwner(getAudioProcessor(),kGain);
    s_Attack->setOwner(getAudioProcessor(), kAttack);
    s_Release->setOwner(getAudioProcessor(), kRelease);
    s_GateLCC->setOwner(getAudioProcessor(), kGateCCL);
    s_GateCCR->setOwner(getAudioProcessor(), kGateCCR);
    s_OnValueL->setOwner(getAudioProcessor(), kGateOnValueCCL);
    s_OnValueR->setOwner(getAudioProcessor(), kGateOnValueCCR);
    s_OffValueL->setOwner(getAudioProcessor(), kGateOffValueCCL);
    s_OffValueR->setOwner(getAudioProcessor(), kGateOffValueCCR);
    //s_Thresh->setOwner(getAudioProcessor(),kGateThreshold);

    s_Gain->setDoubleClickReturnValue(true, 1.f);
    s_PeakGain->setDoubleClickReturnValue(true, 1.f);

    b_Stereo->setClickingTogglesState(true);
    b_Mode->setClickingTogglesState(true);

    comboBox->addItem(L"--", 1);
    for (int i = 0; i < ownerFilter->devices.size(); i++)
    {
        comboBox->addItem(ownerFilter->devices[i].name, i + 2);
    }
    comboBox->setSelectedId(1);

    //[/UserPreSize]

    setSize(528, 312);

    //[Constructor] You can add your own custom stuff here..
    ownerFilter->addChangeListener(this);
    updateParametersFromFilter();
    peakcounter = 0;
    startTimer(50);
    //[/Constructor]
}

AudioToCCEditor::~AudioToCCEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    getFilter()->removeChangeListener(this);
    //[/Destructor_pre]

    s_Thresh       = nullptr;
    label20        = nullptr;
    clipR          = nullptr;
    clipL          = nullptr;
    comboBox       = nullptr;
    s_LCC          = nullptr;
    s_RCC          = nullptr;
    s_Channel      = nullptr;
    b_Stereo       = nullptr;
    s_Rate         = nullptr;
    s_Inertia      = nullptr;
    label          = nullptr;
    label2         = nullptr;
    label3         = nullptr;
    label4         = nullptr;
    label5         = nullptr;
    label6         = nullptr;
    s_Gain         = nullptr;
    label7         = nullptr;
    s_IndicatorL   = nullptr;
    s_IndicatorR   = nullptr;
    toggleButton   = nullptr;
    toggleButton2  = nullptr;
    s_Attack       = nullptr;
    s_Release      = nullptr;
    label8         = nullptr;
    label9         = nullptr;
    b_Mode         = nullptr;
    label10        = nullptr;
    label11        = nullptr;
    label12        = nullptr;
    s_GateLCC      = nullptr;
    s_GateCCR      = nullptr;
    label13        = nullptr;
    label14        = nullptr;
    s_OnValueL     = nullptr;
    s_OnValueR     = nullptr;
    label15        = nullptr;
    label16        = nullptr;
    s_OffValueL    = nullptr;
    s_OffValueR    = nullptr;
    label17        = nullptr;
    label18        = nullptr;
    label19        = nullptr;
    s_PeakGain     = nullptr;
    s_IndicatorRIn = nullptr;
    s_IndicatorLIn = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AudioToCCEditor::paint(juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(juce::Colour(0xffd8d8d8));

    {
        float x = 24.0f, y = 8.0f, width = 168.0f, height = 48.0f;
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
    }

    {
        float x = 32.0f, y = 16.0f, width = 152.0f, height = 32.0f;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
    }

    {
        float x = 224.0f, y = 8.0f, width = 296.0f, height = 296.0f;
        juce::Colour fillColour = juce::Colour(0xffb6b6b6);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
    }

    {
        float x = 363.0f, y = 82.0f, width = 149.0f, height = 214.0f;
        juce::Colour fillColour = juce::Colour(0xff9a9a9a);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
    }

    {
        float x = 235.0f, y = 82.0f, width = 120.0f, height = 214.0f;
        juce::Colour fillColour = juce::Colour(0xff9a9a9a);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
    }

    {
        int x = 235, y = 76, width = 117, height = 30;
        juce::String text(TRANS("Continuous"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Bold"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 363, y = 76, width = 149, height = 30;
        juce::String text(TRANS("Gate"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Bold"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 40, y = 16, width = 136, height = 16;
        juce::String text(TRANS("Insert Piz Here->"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(11.20f, juce::Font::plain).withTypefaceStyle("Bold"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 235, y = 2, width = 277, height = 30;
        juce::String text(TRANS("MIDI"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Bold"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 40, y = 26, width = 136, height = 24;
        juce::String text(TRANS("AudioToCC"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Bold"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        float x = 24.0f, y = 66.0f, width = 168.0f, height = 110.0f;
        juce::Colour fillColour = juce::Colour(0xff9a9a9a);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
    }

    {
        int x = 27, y = 60, width = 165, height = 30;
        juce::String text(TRANS("Gain"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Bold"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AudioToCCEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void AudioToCCEditor::sliderValueChanged(juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == s_Thresh.get())
    {
        //[UserSliderCode_s_Thresh] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kGateThreshold, (float) (s_Thresh->getValue()));
        //[/UserSliderCode_s_Thresh]
    }
    else if (sliderThatWasMoved == s_LCC.get())
    {
        //[UserSliderCode_s_LCC] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kCCL, s_LCC->mapToVSTRange());
        //[/UserSliderCode_s_LCC]
    }
    else if (sliderThatWasMoved == s_RCC.get())
    {
        //[UserSliderCode_s_RCC] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kCCR, s_RCC->mapToVSTRange());
        //[/UserSliderCode_s_RCC]
    }
    else if (sliderThatWasMoved == s_Channel.get())
    {
        //[UserSliderCode_s_Channel] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kChannel, s_Channel->mapToVSTRange());
        //[/UserSliderCode_s_Channel]
    }
    else if (sliderThatWasMoved == s_Rate.get())
    {
        //[UserSliderCode_s_Rate] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kRate, s_Rate->mapToVSTRange());
        //[/UserSliderCode_s_Rate]
    }
    else if (sliderThatWasMoved == s_Inertia.get())
    {
        //[UserSliderCode_s_Inertia] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kSmooth, s_Inertia->mapToVSTRange());
        //[/UserSliderCode_s_Inertia]
    }
    else if (sliderThatWasMoved == s_Gain.get())
    {
        //[UserSliderCode_s_Gain] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kGain, (float) (s_Gain->getValue()) / maxGain);
        //[/UserSliderCode_s_Gain]
    }
    else if (sliderThatWasMoved == s_IndicatorL.get())
    {
        //[UserSliderCode_s_IndicatorL] -- add your slider handling code here..
        //[/UserSliderCode_s_IndicatorL]
    }
    else if (sliderThatWasMoved == s_IndicatorR.get())
    {
        //[UserSliderCode_s_IndicatorR] -- add your slider handling code here..
        //[/UserSliderCode_s_IndicatorR]
    }
    else if (sliderThatWasMoved == s_Attack.get())
    {
        //[UserSliderCode_s_Attack] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kAttack, s_Attack->mapToVSTRange());
        //[/UserSliderCode_s_Attack]
    }
    else if (sliderThatWasMoved == s_Release.get())
    {
        //[UserSliderCode_s_Release] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kRelease, s_Release->mapToVSTRange());
        //[/UserSliderCode_s_Release]
    }
    else if (sliderThatWasMoved == s_GateLCC.get())
    {
        //[UserSliderCode_s_GateLCC] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kGateCCL, s_GateLCC->mapToVSTRange());
        //[/UserSliderCode_s_GateLCC]
    }
    else if (sliderThatWasMoved == s_GateCCR.get())
    {
        //[UserSliderCode_s_GateCCR] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kGateCCR, s_GateCCR->mapToVSTRange());
        //[/UserSliderCode_s_GateCCR]
    }
    else if (sliderThatWasMoved == s_OnValueL.get())
    {
        //[UserSliderCode_s_OnValueL] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kGateOnValueCCL, s_OnValueL->mapToVSTRange());
        //[/UserSliderCode_s_OnValueL]
    }
    else if (sliderThatWasMoved == s_OnValueR.get())
    {
        //[UserSliderCode_s_OnValueR] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kGateOnValueCCR, s_OnValueR->mapToVSTRange());
        //[/UserSliderCode_s_OnValueR]
    }
    else if (sliderThatWasMoved == s_OffValueL.get())
    {
        //[UserSliderCode_s_OffValueL] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kGateOffValueCCL, s_OffValueL->mapToVSTRange());
        //[/UserSliderCode_s_OffValueL]
    }
    else if (sliderThatWasMoved == s_OffValueR.get())
    {
        //[UserSliderCode_s_OffValueR] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kGateOffValueCCR, s_OffValueR->mapToVSTRange());
        //[/UserSliderCode_s_OffValueR]
    }
    else if (sliderThatWasMoved == s_PeakGain.get())
    {
        //[UserSliderCode_s_PeakGain] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kPeakGain, (float) (s_PeakGain->getValue()) / maxGain);
        //[/UserSliderCode_s_PeakGain]
    }
    else if (sliderThatWasMoved == s_IndicatorRIn.get())
    {
        //[UserSliderCode_s_IndicatorRIn] -- add your slider handling code here..
        //[/UserSliderCode_s_IndicatorRIn]
    }
    else if (sliderThatWasMoved == s_IndicatorLIn.get())
    {
        //[UserSliderCode_s_IndicatorLIn] -- add your slider handling code here..
        //[/UserSliderCode_s_IndicatorLIn]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void AudioToCCEditor::buttonClicked(juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == clipR.get())
    {
        //[UserButtonCode_clipR] -- add your button handler code here..
        getFilter()->setParameter(kGateResetR, 1.f);
        //[/UserButtonCode_clipR]
    }
    else if (buttonThatWasClicked == clipL.get())
    {
        //[UserButtonCode_clipL] -- add your button handler code here..
        getFilter()->setParameter(kGateResetL, 1.f);
        //[/UserButtonCode_clipL]
    }
    else if (buttonThatWasClicked == b_Stereo.get())
    {
        //[UserButtonCode_b_Stereo] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost(kStereo, b_Stereo->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_b_Stereo]
    }
    else if (buttonThatWasClicked == toggleButton.get())
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost(kAutomateHost, toggleButton->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_toggleButton]
    }
    else if (buttonThatWasClicked == toggleButton2.get())
    {
        //[UserButtonCode_toggleButton2] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost(kMidiToHost, toggleButton2->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_toggleButton2]
    }
    else if (buttonThatWasClicked == b_Mode.get())
    {
        //[UserButtonCode_b_Mode] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost(kMode, b_Mode->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_b_Mode]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void AudioToCCEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox.get())
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
        if (comboBox->getSelectedItemIndex() == 0)
        {
            getFilter()->setParameter(kDevice, 0.0f);
            getFilter()->setActiveDevice(comboBox->getText());
        }
        else
        {
            getFilter()->setActiveDevice(comboBox->getText());
            //getFilter()->setParameter(0,float(comboBox->getSelectedItemIndex()-1)/float(getFilter()->devices.size()-1)+0.00001f);
        }
        //[/UserComboBoxCode_comboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void AudioToCCEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == getFilter())
        updateParametersFromFilter();
}

void AudioToCCEditor::updateParametersFromFilter()
{
    AudioToCC* const filter = getFilter();

    float p[numParams];
    filter->getCallbackLock().enter();
    const int newDevice = filter->devices.indexOf(filter->getActiveDevice());
    for (int i = 0; i < numParams; i++)
        p[i] = filter->getParameter(i);
    const int ccL = filter->lastCCL;
    const int ccR = filter->lastCCR;
    filter->getCallbackLock().exit();

    s_Gain->setValue(p[kGain] * maxGain);
    s_PeakGain->setValue(p[kPeakGain] * maxGain);
    s_LCC->setVSTSlider(p[kCCL]);
    s_RCC->setVSTSlider(p[kCCR]);
    s_Channel->setVSTSlider(p[kChannel]);
    s_Rate->setVSTSlider(p[kRate]);
    s_Inertia->setVSTSlider(p[kSmooth]);
    s_Attack->setVSTSlider(p[kAttack]);
    s_Release->setVSTSlider(p[kRelease]);
    s_GateLCC->setVSTSlider(p[kGateCCL]);
    s_GateCCR->setVSTSlider(p[kGateCCR]);
    s_OnValueL->setVSTSlider(p[kGateOnValueCCL]);
    s_OnValueR->setVSTSlider(p[kGateOnValueCCR]);
    s_OffValueL->setVSTSlider(p[kGateOffValueCCL]);
    s_OffValueR->setVSTSlider(p[kGateOffValueCCR]);
    s_Thresh->setValue(p[kGateThreshold]);

    comboBox->setSelectedItemIndex(newDevice + 1, juce::dontSendNotification);
    toggleButton->setToggleState(p[kAutomateHost] >= 0.5f, juce::dontSendNotification);
    toggleButton2->setToggleState(p[kMidiToHost] >= 0.5f, juce::dontSendNotification);
    b_Stereo->setToggleState(p[kStereo] >= 0.5f, juce::dontSendNotification);
    b_Stereo->setButtonText(p[kStereo] >= 0.5f ? "Stereo" : "Mono (L+R)");
    b_Mode->setToggleState(p[kMode] >= 0.5f, juce::dontSendNotification);
    b_Mode->setButtonText(p[kMode] >= 0.5f ? "Logarithmic" : "Linear");
}

void AudioToCCEditor::timerCallback()
{
    getFilter()->getCallbackLock().enter();
    const int ccL    = getFilter()->lastCCL;
    const int ccR    = getFilter()->lastCCR;
    const float inL  = getFilter()->lastInL;
    const float inR  = getFilter()->lastInR;
    const bool gateL = getFilter()->lastGateCCL;
    const bool gateR = getFilter()->lastGateCCR;
    getFilter()->getCallbackLock().exit();

    s_IndicatorL->setValue(ccL, juce::dontSendNotification);
    s_IndicatorR->setValue(ccR, juce::dontSendNotification);
    s_IndicatorLIn->setValue(inL, juce::dontSendNotification);
    s_IndicatorRIn->setValue(inR, juce::dontSendNotification);
    ++peakcounter;
    if (peakcounter == 2)
    {
        peakcounter = 0;
        clipL->setToggleState(gateL, juce::dontSendNotification);
        clipR->setToggleState(gateR, juce::dontSendNotification);
    }
    else
    {
        if (gateL)
            clipL->setToggleState(true, juce::dontSendNotification);
        if (gateR)
            clipR->setToggleState(true, juce::dontSendNotification);
    }
}
//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AudioToCCEditor" componentName=""
                 parentClasses="public juce::AudioProcessorEditor, public juce::ChangeListener, public juce::Timer"
                 constructorParams="AudioToCC* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="528" initialHeight="312">
  <BACKGROUND backgroundColour="ffd8d8d8">
    <ROUNDRECT pos="24 8 168 48" cornerSize="10.0" fill="solid: ffffffff" hasStroke="0"/>
    <ROUNDRECT pos="32 16 152 32" cornerSize="10.0" fill="solid: ff000000" hasStroke="0"/>
    <ROUNDRECT pos="224 8 296 296" cornerSize="10.0" fill="solid: ffb6b6b6"
               hasStroke="0"/>
    <ROUNDRECT pos="363 82 149 214" cornerSize="10.0" fill="solid: ff9a9a9a"
               hasStroke="0"/>
    <ROUNDRECT pos="235 82 120 214" cornerSize="10.0" fill="solid: ff9a9a9a"
               hasStroke="0"/>
    <TEXT pos="235 76 117 30" fill="solid: ff000000" hasStroke="0" text="Continuous"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="36" typefaceStyle="Bold"/>
    <TEXT pos="363 76 149 30" fill="solid: ff000000" hasStroke="0" text="Gate"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="36" typefaceStyle="Bold"/>
    <TEXT pos="40 16 136 16" fill="solid: ffffffff" hasStroke="0" text="Insert Piz Here-&gt;"
          fontname="Default font" fontsize="11.2" kerning="0.0" bold="1"
          italic="0" justification="36" typefaceStyle="Bold"/>
    <TEXT pos="235 2 277 30" fill="solid: ff000000" hasStroke="0" text="MIDI"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="36" typefaceStyle="Bold"/>
    <TEXT pos="40 26 136 24" fill="solid: ffffffff" hasStroke="0" text="AudioToCC"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="36" typefaceStyle="Bold"/>
    <ROUNDRECT pos="24 66 168 110" cornerSize="10.0" fill="solid: ff9a9a9a"
               hasStroke="0"/>
    <TEXT pos="27 60 165 30" fill="solid: ff000000" hasStroke="0" text="Gain"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="36" typefaceStyle="Bold"/>
  </BACKGROUND>
  <SLIDER name="slider" id="823450fac3f64450" memberName="s_Thresh" virtualName="DecibelSlider"
          explicitFocusOrder="0" pos="32 182 152 45" min="0.0" max="1.0"
          int="0.0" style="LinearBar" textBoxPos="TextBoxBelow" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="0.6" needsCallback="1"/>
  <LABEL name="new label" id="408faf055a8f09b" memberName="label20" virtualName=""
         explicitFocusOrder="0" pos="112 134 72 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Peak" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="new button" id="18e6c8407abe4977" memberName="clipR" virtualName=""
              explicitFocusOrder="0" pos="183 211 33 16" bgColOn="ffc33333"
              textCol="ffffffff" buttonText="gate" connectedEdges="1" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="1676a9267ec47ccf" memberName="clipL" virtualName=""
              explicitFocusOrder="0" pos="183 182 33 16" bgColOn="ffc33333"
              textCol="ffffffff" textColOn="ff000000" buttonText="gate" connectedEdges="1"
              needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="new combo box" id="a04840c19e04d837" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="280 41 194 16" tooltip="Output Device"
            editable="0" layout="33" items="" textWhenNonSelected="--" textWhenNoItems="(no choices)"/>
  <SLIDER name="LCC slider" id="71fcaacdd19b819a" memberName="s_LCC" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="272 104 72 16" min="-1.0" max="127.0"
          int="1.0" style="LinearBar" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="RCC slider" id="1ebfa8346656c622" memberName="s_RCC" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="272 128 72 16" min="-1.0" max="127.0"
          int="1.0" style="LinearBar" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="channel slider" id="f2742b92b455391" memberName="s_Channel"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="312 61 72 16"
          min="1.0" max="16.0" int="1.0" style="LinearBar" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TEXTBUTTON name="mono/stereo" id="cc33b9af97a255fa" memberName="b_Stereo"
              virtualName="" explicitFocusOrder="0" pos="400 61 72 16" bgColOn="ffbbbbff"
              buttonText="Stereo" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="rate slider" id="d404e81338e76f73" memberName="s_Rate"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="248 240 40 48"
          min="0.0" max="100.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="16" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="inertia slider" id="738b8b242a96bb71" memberName="s_Inertia"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="296 240 40 48"
          min="0.0" max="100.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="16" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="6c723bc2831142a7" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="275 29 150 8" edTextCol="ff000000"
         edBkgCol="0" labelText="MIDI Out Device:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="6ef5efe574b69b48" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="32 132 72 16" edTextCol="ff000000"
         edBkgCol="0" labelText="RMS" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="340f799e1ecceeec" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="232 104 40 16" edTextCol="ff000000"
         edBkgCol="0" labelText="L CC:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="e023036ab79cf561" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="232 128 40 16" edTextCol="ff000000"
         edBkgCol="0" labelText="R CC:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="3d56114808131b06" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="272 61 40 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Ch:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="b12b0b77f888ceac" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="248 224 40 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Rate" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="gain slider" id="60f4dbdc28325f06" memberName="s_Gain"
          virtualName="DecibelSlider" explicitFocusOrder="0" pos="32 78 72 88"
          min="0.0" max="32.0" int="0.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="16" skewFactor="0.333"
          needsCallback="1"/>
  <LABEL name="new label" id="cca1fb27662e9234" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="288 224 56 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Inertia" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="14cd913f745933b0" memberName="s_IndicatorL"
          virtualName="" explicitFocusOrder="0" pos="32 236 152 24" bkgcol="ff000000"
          thumbcol="ff7e7434" textboxtext="ffffffff" min="0.0" max="127.0"
          int="1.0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="330ac78fbebd8364" memberName="s_IndicatorR"
          virtualName="" explicitFocusOrder="0" pos="32 262 152 24" bkgcol="ff000000"
          thumbcol="ff7e7434" textboxtext="ffffffff" min="0.0" max="127.0"
          int="1.0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TOGGLEBUTTON name="new toggle button" id="c7244283a62de40d" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="48 288 120 16" buttonText="Automation to host"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="d7b9270f0071e0c2" memberName="toggleButton2"
                virtualName="" explicitFocusOrder="0" pos="384 25 88 16" buttonText="MIDI to host"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="attack slider" id="a52f94145d8acc90" memberName="s_Attack"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="248 168 40 48"
          min="0.0" max="100.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="16" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="release slider" id="2f8489f087333f83" memberName="s_Release"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="296 168 40 48"
          min="0.0" max="100.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="16" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="b9436352365500ec" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="240 152 56 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Attack" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="405ea789fca9617b" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="288 152 56 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Release" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="dB" id="bb20e2ed360643cf" memberName="b_Mode" virtualName=""
              explicitFocusOrder="0" pos="244 -21 80 16" bgColOn="ffbbbbff"
              buttonText="Logarithmic" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="new label" id="dd3011facb5b4850" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="196 -21 46 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Scale:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="1ee6592a17755252" memberName="label11" virtualName=""
         explicitFocusOrder="0" pos="16 240 16 16" edTextCol="ff000000"
         edBkgCol="0" labelText="L" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="dd6c3c6feae41d30" memberName="label12" virtualName=""
         explicitFocusOrder="0" pos="16 266 16 16" edTextCol="ff000000"
         edBkgCol="0" labelText="R" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="LCC slider" id="d5e457f417b75de0" memberName="s_GateLCC"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="432 136 72 16"
          min="-1.0" max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="RCC slider" id="e0e3717fd4efa33d" memberName="s_GateCCR"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="432 216 72 16"
          min="-1.0" max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="ac48f93040782198" memberName="label13" virtualName=""
         explicitFocusOrder="0" pos="368 136 64 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Gate L CC:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="4fe98004d25fec5b" memberName="label14" virtualName=""
         explicitFocusOrder="0" pos="368 216 64 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Gate R CC:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="LCC slider" id="bbf1f7a3818fe30" memberName="s_OnValueL"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="432 160 72 16"
          min="-1.0" max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="RCC slider" id="bf7c945199e1932b" memberName="s_OnValueR"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="432 240 72 16"
          min="-1.0" max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="f48f3d6c3af0675e" memberName="label15" virtualName=""
         explicitFocusOrder="0" pos="360 160 72 16" edTextCol="ff000000"
         edBkgCol="0" labelText="On Value L:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="b22f446661cc8441" memberName="label16" virtualName=""
         explicitFocusOrder="0" pos="360 240 72 16" edTextCol="ff000000"
         edBkgCol="0" labelText="On Value R:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="LCC slider" id="6d98a3dac1c63181" memberName="s_OffValueL"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="432 184 72 16"
          min="-1.0" max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="RCC slider" id="2e76cd038f25b4cd" memberName="s_OffValueR"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="432 264 72 16"
          min="-1.0" max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="b218aad886b11a5f" memberName="label17" virtualName=""
         explicitFocusOrder="0" pos="352 184 80 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Off Value L:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="bce9fa74698943f7" memberName="label18" virtualName=""
         explicitFocusOrder="0" pos="352 264 80 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Off Value R:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="1a296eabc3802958" memberName="label19" virtualName=""
         explicitFocusOrder="0" pos="-1 196 64 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Threshold:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <SLIDER name="gain slider" id="e893dbc7f028806a" memberName="s_PeakGain"
          virtualName="DecibelSlider" explicitFocusOrder="0" pos="112 78 72 88"
          min="0.0" max="32.0" int="0.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="16" skewFactor="0.333"
          needsCallback="1"/>
  <SLIDER name="new slider" id="6781186ed706f895" memberName="s_IndicatorRIn"
          virtualName="DecibelMeter" explicitFocusOrder="0" pos="32 211 152 16"
          bkgcol="ff000000" thumbcol="ff347e4c" textboxtext="ffffffff"
          min="0.0" max="1.0" int="0.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.6"
          needsCallback="1"/>
  <SLIDER name="new slider" id="ea85cd5620a96fd3" memberName="s_IndicatorLIn"
          virtualName="DecibelMeter" explicitFocusOrder="0" pos="32 182 152 16"
          bkgcol="ff000000" thumbcol="ff347e4c" textboxtext="ffffffff"
          min="0.0" max="1.0" int="0.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="0.6"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]
