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

#include "PizLooperEditor.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
using juce::jmax;
using juce::jmin;
using juce::roundToInt;

//[/MiscUserDefs]

//==============================================================================
PizLooperEditor::PizLooperEditor(PizLooper* const ownerFilter)
    : AudioProcessorEditor(ownerFilter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    label.reset(new juce::Label("new label",
                                TRANS("Zoom")));
    addAndMakeVisible(label.get());
    label->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label->setJustificationType(juce::Justification::centredLeft);
    label->setEditable(false, false, false);
    label->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label->setBounds(613, 64, 46, 18);

    timeline.reset(new Timeline());
    addAndMakeVisible(timeline.get());
    timeline->setName("timeline");

    textButton1.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton1.get());
    textButton1->setButtonText(TRANS("1"));
    textButton1->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton1->setRadioGroupId(1);

    textButton1->setBounds(0, 61, 25, 24);

    textButton2.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton2.get());
    textButton2->setButtonText(TRANS("2"));
    textButton2->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton2->setRadioGroupId(1);
    textButton2->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton2->setBounds(25, 61, 23, 24);

    textButton3.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton3.get());
    textButton3->setButtonText(TRANS("3"));
    textButton3->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton3->setRadioGroupId(1);

    textButton3->setBounds(48, 61, 23, 24);

    textButton4.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton4.get());
    textButton4->setButtonText(TRANS("4"));
    textButton4->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton4->setRadioGroupId(1);

    textButton4->setBounds(71, 61, 23, 24);

    textButton5.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton5.get());
    textButton5->setButtonText(TRANS("5"));
    textButton5->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton5->setRadioGroupId(1);

    textButton5->setBounds(94, 61, 23, 24);

    textButton6.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton6.get());
    textButton6->setButtonText(TRANS("6"));
    textButton6->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton6->setRadioGroupId(1);

    textButton6->setBounds(117, 61, 23, 24);

    textButton7.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton7.get());
    textButton7->setButtonText(TRANS("7"));
    textButton7->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton7->setRadioGroupId(1);

    textButton7->setBounds(140, 61, 23, 24);

    textButton8.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton8.get());
    textButton8->setButtonText(TRANS("8"));
    textButton8->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton8->setRadioGroupId(1);

    textButton8->setBounds(163, 61, 23, 24);

    textButton9.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton9.get());
    textButton9->setButtonText(TRANS("9"));
    textButton9->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton9->setRadioGroupId(1);

    textButton9->setBounds(186, 61, 23, 24);

    textButton10.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton10.get());
    textButton10->setButtonText(TRANS("10"));
    textButton10->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton10->setRadioGroupId(1);

    textButton10->setBounds(209, 61, 25, 24);

    textButton11.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton11.get());
    textButton11->setButtonText(TRANS("11"));
    textButton11->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton11->setRadioGroupId(1);

    textButton11->setBounds(234, 61, 25, 24);

    textButton12.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton12.get());
    textButton12->setButtonText(TRANS("12"));
    textButton12->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton12->setRadioGroupId(1);

    textButton12->setBounds(259, 61, 25, 24);

    textButton13.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton13.get());
    textButton13->setButtonText(TRANS("13"));
    textButton13->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton13->setRadioGroupId(1);

    textButton13->setBounds(284, 61, 25, 24);

    textButton14.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton14.get());
    textButton14->setButtonText(TRANS("14"));
    textButton14->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton14->setRadioGroupId(1);

    textButton14->setBounds(309, 61, 25, 24);

    textButton15.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton15.get());
    textButton15->setButtonText(TRANS("15"));
    textButton15->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton15->setRadioGroupId(1);

    textButton15->setBounds(334, 61, 25, 24);

    textButton16.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton16.get());
    textButton16->setButtonText(TRANS("16"));
    textButton16->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton16->setRadioGroupId(1);

    textButton16->setBounds(359, 61, 28, 24);

    b_Play.reset(new juce::TextButton("Play"));
    addAndMakeVisible(b_Play.get());
    b_Play->setTooltip(TRANS("Toggle playback of current slot"));
    b_Play->setButtonText(TRANS("PLAY"));
    b_Play->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnTop);
    b_Play->setColour(juce::TextButton::buttonColourId, juce::Colour(0xd213540e));
    b_Play->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff00c400));
    b_Play->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    b_Play->setColour(juce::TextButton::textColourOnId, juce::Colours::white);

    b_Play->setBounds(78, 117, 72, 50);

    b_Record.reset(new juce::TextButton("Record"));
    addAndMakeVisible(b_Record.get());
    b_Record->setTooltip(TRANS("Toggle recording to current slot"));
    b_Record->setButtonText(TRANS("RECORD "));
    b_Record->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop);
    b_Record->setColour(juce::TextButton::buttonColourId, juce::Colour(0xd2a90000));
    b_Record->setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    b_Record->setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    b_Record->setColour(juce::TextButton::textColourOnId, juce::Colours::white);

    b_Record->setBounds(6, 117, 72, 50);

    b_Overdub.reset(new juce::TextButton("Overdub"));
    addAndMakeVisible(b_Overdub.get());
    b_Overdub->setTooltip(TRANS("Toggle overdub recording"));
    b_Overdub->addListener(this);
    b_Overdub->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));
    b_Overdub->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    b_Overdub->setColour(juce::TextButton::textColourOnId, juce::Colours::black);

    b_Overdub->setBounds(523, 12, 80, 20);

    b_Thru.reset(new juce::TextButton("MIDI Thru"));
    addAndMakeVisible(b_Thru.get());
    b_Thru->setTooltip(TRANS("Toggle MIDI Thru (Notes selected for Note Triggering and Scale Channel are always blocked)"));
    b_Thru->setButtonText(TRANS("Thru"));
    b_Thru->addListener(this);
    b_Thru->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));

    b_Thru->setBounds(634, 33, 40, 20);

    b_Clear.reset(new juce::TextButton("Clear"));
    addAndMakeVisible(b_Clear.get());
    b_Clear->setTooltip(TRANS("Erase MIDI data from the current slot"));
    b_Clear->setConnectedEdges(juce::Button::ConnectedOnRight);
    b_Clear->addListener(this);
    b_Clear->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    b_Clear->setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    b_Clear->setColour(juce::TextButton::textColourOnId, juce::Colours::azure);

    b_Clear->setBounds(264, 90, 39, 22);

    stepsizeBox.reset(new juce::ComboBox("Loop Step Size"));
    addAndMakeVisible(stepsizeBox.get());
    stepsizeBox->setTooltip(TRANS("Recording length will be quantized to this step size."));
    stepsizeBox->setEditableText(false);
    stepsizeBox->setJustificationType(juce::Justification::centredLeft);
    stepsizeBox->setTextWhenNothingSelected(TRANS("16th Note"));
    stepsizeBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    stepsizeBox->addItem(TRANS("1 Bar"), 1);
    stepsizeBox->addItem(TRANS("3 Beats"), 2);
    stepsizeBox->addItem(TRANS("2 Beats"), 3);
    stepsizeBox->addItem(TRANS("1 Beat"), 4);
    stepsizeBox->addItem(TRANS("16th Note"), 5);
    stepsizeBox->addItem(TRANS("1 Tick"), 6);
    stepsizeBox->addListener(this);

    stepsizeBox->setBounds(264, 15, 77, 16);

    s_Transpose.reset(new VSTSlider("Transpose"));
    addAndMakeVisible(s_Transpose.get());
    s_Transpose->setTooltip(TRANS("Transposition applied to the current slot"));
    s_Transpose->setRange(-12, 12, 1);
    s_Transpose->setSliderStyle(juce::Slider::LinearBar);
    s_Transpose->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_Transpose->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_Transpose->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_Transpose->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_Transpose->addListener(this);

    s_Transpose->setBounds(146, 283, 72, 20);

    s_Octave.reset(new VSTSlider("Octave Shift"));
    addAndMakeVisible(s_Octave.get());
    s_Octave->setTooltip(TRANS("Transposition by octave for the current slot"));
    s_Octave->setRange(-4, 4, 1);
    s_Octave->setSliderStyle(juce::Slider::LinearBar);
    s_Octave->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_Octave->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_Octave->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_Octave->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_Octave->addListener(this);

    s_Octave->setBounds(226, 283, 72, 20);

    s_Velocity.reset(new VSTSlider("Velocity Offset"));
    addAndMakeVisible(s_Velocity.get());
    s_Velocity->setTooltip(TRANS("Velocity adjustment for the current slot"));
    s_Velocity->setRange(0, 200, 1);
    s_Velocity->setSliderStyle(juce::Slider::LinearBar);
    s_Velocity->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_Velocity->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_Velocity->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_Velocity->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_Velocity->addListener(this);

    s_Velocity->setBounds(306, 283, 72, 20);

    label3.reset(new juce::Label("new label",
                                 TRANS("Semitones")));
    addAndMakeVisible(label3.get());
    label3->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label3->setJustificationType(juce::Justification::centred);
    label3->setEditable(false, false, false);
    label3->setColour(juce::Label::textColourId, juce::Colours::white);
    label3->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label3->setBounds(146, 267, 72, 16);

    label4.reset(new juce::Label("new label",
                                 TRANS("Octave")));
    addAndMakeVisible(label4.get());
    label4->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label4->setJustificationType(juce::Justification::centred);
    label4->setEditable(false, false, false);
    label4->setColour(juce::Label::textColourId, juce::Colours::white);
    label4->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label4->setBounds(226, 267, 72, 16);

    label5.reset(new juce::Label("new label",
                                 TRANS("Velocity")));
    addAndMakeVisible(label5.get());
    label5->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label5->setJustificationType(juce::Justification::centred);
    label5->setEditable(false, false, false);
    label5->setColour(juce::Label::textColourId, juce::Colours::white);
    label5->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label5->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label5->setBounds(301, 267, 80, 16);

    s_Start.reset(new VSTSlider("Loop Start"));
    addAndMakeVisible(s_Start.get());
    s_Start->setTooltip(TRANS("Offsets the loop start time by this number of beats"));
    s_Start->setRange(-8, 8, 1);
    s_Start->setSliderStyle(juce::Slider::LinearBar);
    s_Start->setTextBoxStyle(juce::Slider::TextBoxLeft, true, 80, 20);
    s_Start->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_Start->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_Start->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_Start->addListener(this);

    s_Start->setBounds(66, 367, 72, 20);

    label6.reset(new juce::Label("new label",
                                 TRANS("Start Offset")));
    addAndMakeVisible(label6.get());
    label6->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label6->setJustificationType(juce::Justification::centred);
    label6->setEditable(false, false, false);
    label6->setColour(juce::Label::textColourId, juce::Colours::white);
    label6->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label6->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label6->setBounds(66, 351, 72, 16);

    s_End.reset(new VSTSlider("Loop End"));
    addAndMakeVisible(s_End.get());
    s_End->setTooltip(TRANS("Offsets the loop end time by this number of beats"));
    s_End->setRange(-8, 8, 1);
    s_End->setSliderStyle(juce::Slider::LinearBar);
    s_End->setTextBoxStyle(juce::Slider::TextBoxLeft, true, 80, 20);
    s_End->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_End->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_End->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_End->addListener(this);

    s_End->setBounds(146, 367, 72, 20);

    label7.reset(new juce::Label("new label",
                                 TRANS("End Offset")));
    addAndMakeVisible(label7.get());
    label7->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label7->setJustificationType(juce::Justification::centred);
    label7->setEditable(false, false, false);
    label7->setColour(juce::Label::textColourId, juce::Colours::white);
    label7->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label7->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label7->setBounds(146, 351, 72, 16);

    s_Stretch.reset(new VSTSlider("Loop Stretch"));
    addAndMakeVisible(s_Stretch.get());
    s_Stretch->setTooltip(TRANS("Playback speed, relative to host tempo"));
    s_Stretch->setRange(-10, 10, 1);
    s_Stretch->setSliderStyle(juce::Slider::LinearBar);
    s_Stretch->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_Stretch->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_Stretch->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_Stretch->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_Stretch->addListener(this);

    s_Stretch->setBounds(306, 367, 72, 20);

    label8.reset(new juce::Label("new label",
                                 TRANS("Speed")));
    addAndMakeVisible(label8.get());
    label8->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label8->setJustificationType(juce::Justification::centred);
    label8->setEditable(false, false, false);
    label8->setColour(juce::Label::textColourId, juce::Colours::white);
    label8->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label8->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label8->setBounds(306, 351, 72, 16);

    loopmodeBox.reset(new juce::ComboBox("Loop Mode"));
    addAndMakeVisible(loopmodeBox.get());
    loopmodeBox->setTooltip(TRANS("Playback Mode: \"Sync Loop\" follows the host timeline. \"Loop after rec\" is the same but also plays automatically as soon as recording ends. \"Unsync\" modes play the pattern from the beginning as soon as playback is started."));
    loopmodeBox->setEditableText(false);
    loopmodeBox->setJustificationType(juce::Justification::centredLeft);
    loopmodeBox->setTextWhenNothingSelected(TRANS("Unsync 1-shot"));
    loopmodeBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    loopmodeBox->addItem(TRANS("Loop after rec"), 1);
    loopmodeBox->addItem(TRANS("Sync loop"), 2);
    loopmodeBox->addItem(TRANS("Unsync 1-shot"), 3);
    loopmodeBox->addItem(TRANS("Unsync loop"), 4);
    loopmodeBox->addListener(this);

    loopmodeBox->setBounds(175, 144, 110, 16);

    notetriggerBox.reset(new juce::ComboBox("Note Trigger"));
    addAndMakeVisible(notetriggerBox.get());
    notetriggerBox->setTooltip(TRANS("For \"Transpose\" modes, pattern will be transposed relative to \"Root Note\""));
    notetriggerBox->setEditableText(false);
    notetriggerBox->setJustificationType(juce::Justification::centredLeft);
    notetriggerBox->setTextWhenNothingSelected(TRANS("Mono (Transposed)"));
    notetriggerBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    notetriggerBox->addItem(TRANS("Off"), 1);
    notetriggerBox->addItem(TRANS("Mono (Transpose)"), 2);
    notetriggerBox->addItem(TRANS("Poly (Transpose)"), 3);
    notetriggerBox->addItem(TRANS("Mono (Orig. Key)"), 4);
    notetriggerBox->addSeparator();
    notetriggerBox->addListener(this);

    notetriggerBox->setBounds(146, 402, 106, 16);

    syncmodeBox.reset(new juce::ComboBox("Sync"));
    addAndMakeVisible(syncmodeBox.get());
    syncmodeBox->setTooltip(TRANS("\"PPQ\" modes always follow host timeline, which may not work in all hosts. \"Sample\" mode ignores the host\'s timeline, but the host\'s tempo is still followed."));
    syncmodeBox->setEditableText(false);
    syncmodeBox->setJustificationType(juce::Justification::centredLeft);
    syncmodeBox->setTextWhenNothingSelected(TRANS("PPQ (Recstart)"));
    syncmodeBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    syncmodeBox->addItem(TRANS("PPQ (Host 0)"), 1);
    syncmodeBox->addItem(TRANS("PPQ (Recstart)"), 2);
    syncmodeBox->addItem(TRANS("Sample"), 3);
    syncmodeBox->addListener(this);

    syncmodeBox->setBounds(159, 15, 99, 16);

    s_Root.reset(new VSTSlider("Root Note"));
    addAndMakeVisible(s_Root.get());
    s_Root->setTooltip(TRANS("Transposed note triggering and Scale Channel input will transpose the pattern relative to this note"));
    s_Root->setRange(-1, 127, 1);
    s_Root->setSliderStyle(juce::Slider::LinearBar);
    s_Root->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_Root->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_Root->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_Root->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_Root->addListener(this);

    s_Root->setBounds(76, 174, 64, 20);

    label9.reset(new juce::Label("new label",
                                 TRANS("Root Note:")));
    addAndMakeVisible(label9.get());
    label9->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label9->setJustificationType(juce::Justification::centred);
    label9->setEditable(false, false, false);
    label9->setColour(juce::Label::textColourId, juce::Colours::white);
    label9->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label9->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label9->setBounds(15, 176, 64, 16);

    s_Low.reset(new VSTSlider("Low Note"));
    addAndMakeVisible(s_Low.get());
    s_Low->setTooltip(TRANS("Lowest note to use for triggering"));
    s_Low->setRange(-1, 127, 1);
    s_Low->setSliderStyle(juce::Slider::LinearBar);
    s_Low->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_Low->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_Low->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_Low->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_Low->addListener(this);

    s_Low->setBounds(18, 440, 64, 20);

    label10.reset(new juce::Label("new label",
                                  TRANS("Low Note")));
    addAndMakeVisible(label10.get());
    label10->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label10->setJustificationType(juce::Justification::centred);
    label10->setEditable(false, false, false);
    label10->setColour(juce::Label::textColourId, juce::Colours::white);
    label10->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label10->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label10->setBounds(18, 424, 64, 16);

    s_High.reset(new VSTSlider("High Note"));
    addAndMakeVisible(s_High.get());
    s_High->setTooltip(TRANS("Highest note to use for triggering"));
    s_High->setRange(-1, 127, 1);
    s_High->setSliderStyle(juce::Slider::LinearBar);
    s_High->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_High->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_High->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_High->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_High->addListener(this);

    s_High->setBounds(90, 440, 64, 20);

    label11.reset(new juce::Label("new label",
                                  TRANS("High Note")));
    addAndMakeVisible(label11.get());
    label11->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label11->setJustificationType(juce::Justification::centred);
    label11->setEditable(false, false, false);
    label11->setColour(juce::Label::textColourId, juce::Colours::white);
    label11->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label11->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label11->setBounds(90, 424, 64, 16);

    s_TrigChan.reset(new VSTSlider("TriggerChannel"));
    addAndMakeVisible(s_TrigChan.get());
    s_TrigChan->setTooltip(TRANS("Channel to use for trigger notes"));
    s_TrigChan->setRange(1, 16, 1);
    s_TrigChan->setSliderStyle(juce::Slider::LinearBar);
    s_TrigChan->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_TrigChan->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_TrigChan->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_TrigChan->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_TrigChan->addListener(this);

    s_TrigChan->setBounds(162, 440, 64, 20);

    label12.reset(new juce::Label("Trigger Channel",
                                  TRANS("Channel")));
    addAndMakeVisible(label12.get());
    label12->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label12->setJustificationType(juce::Justification::centred);
    label12->setEditable(false, false, false);
    label12->setColour(juce::Label::textColourId, juce::Colours::white);
    label12->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label12->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label12->setBounds(161, 424, 64, 16);

    b_Reload.reset(new juce::TextButton("Load"));
    addAndMakeVisible(b_Reload.get());
    b_Reload->setTooltip(TRANS("Load MIDI file (Ctrl-click: load MIDI file with the current pattern name from the \"midiloops\" folder)"));
    b_Reload->setConnectedEdges(juce::Button::ConnectedOnLeft);
    b_Reload->addListener(this);
    b_Reload->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    b_Reload->setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    b_Reload->setColour(juce::TextButton::textColourOnId, juce::Colours::white);

    b_Reload->setBounds(344, 90, 39, 22);

    quantizeBox.reset(new juce::ComboBox("Input Quantize Step"));
    addAndMakeVisible(quantizeBox.get());
    quantizeBox->setTooltip(TRANS("Recorded events will be quantized to this step size"));
    quantizeBox->setEditableText(false);
    quantizeBox->setJustificationType(juce::Justification::centredLeft);
    quantizeBox->setTextWhenNothingSelected(TRANS("32nd"));
    quantizeBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    quantizeBox->addItem(TRANS("Off"), 1);
    quantizeBox->addItem(TRANS("8th"), 2);
    quantizeBox->addItem(TRANS("16th"), 3);
    quantizeBox->addItem(TRANS("32nd"), 4);
    quantizeBox->addItem(TRANS("64th"), 5);
    quantizeBox->addListener(this);

    quantizeBox->setBounds(439, 15, 77, 16);

    label21.reset(new juce::Label("LoopStepSize",
                                  TRANS("Loop Step Size")));
    addAndMakeVisible(label21.get());
    label21->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label21->setJustificationType(juce::Justification::centred);
    label21->setEditable(false, false, false);
    label21->setColour(juce::Label::textColourId, juce::Colours::white);
    label21->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label21->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label21->setBounds(261, 0, 84, 16);

    s_Shift.reset(new VSTSlider("Shift"));
    addAndMakeVisible(s_Shift.get());
    s_Shift->setTooltip(TRANS("Shifts the pattern by this number of beats, with wraparound"));
    s_Shift->setRange(-8, 8, 1);
    s_Shift->setSliderStyle(juce::Slider::LinearBar);
    s_Shift->setTextBoxStyle(juce::Slider::TextBoxLeft, true, 80, 20);
    s_Shift->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_Shift->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_Shift->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_Shift->addListener(this);

    s_Shift->setBounds(226, 367, 72, 20);

    label2.reset(new juce::Label("Shift",
                                 TRANS("Beat Shift")));
    addAndMakeVisible(label2.get());
    label2->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label2->setJustificationType(juce::Justification::centred);
    label2->setEditable(false, false, false);
    label2->setColour(juce::Label::textColourId, juce::Colours::white);
    label2->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label2->setBounds(226, 351, 72, 16);

    label23.reset(new juce::Label("QuantizeLabel",
                                  TRANS("Quantize Input")));
    addAndMakeVisible(label23.get());
    label23->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label23->setJustificationType(juce::Justification::centred);
    label23->setEditable(false, false, false);
    label23->setColour(juce::Label::textColourId, juce::Colours::white);
    label23->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label23->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label23->setBounds(433, 0, 87, 16);

    nameLabel.reset(new ClickableLabel("Name",
                                       TRANS("Bassline (4 bars)")));
    addAndMakeVisible(nameLabel.get());
    nameLabel->setTooltip(TRANS("Current pattern name (double-click to edit)"));
    nameLabel->setFont(juce::Font(26.30f, juce::Font::plain).withTypefaceStyle("Bold"));
    nameLabel->setJustificationType(juce::Justification::centredLeft);
    nameLabel->setEditable(false, true, false);
    nameLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    nameLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colours::white);
    nameLabel->addListener(this);

    nameLabel->setBounds(4, 87, 256, 27);

    b_Save.reset(new juce::TextButton("Save"));
    addAndMakeVisible(b_Save.get());
    b_Save->setTooltip(TRANS("Save a MIDI file of the current pattern (Ctrl-click: save to the \"midiloops\" folder with the current name)"));
    b_Save->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    b_Save->addListener(this);
    b_Save->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    b_Save->setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    b_Save->setColour(juce::TextButton::textColourOnId, juce::Colours::white);

    b_Save->setBounds(304, 90, 39, 22);

    label22.reset(new juce::Label("Sync",
                                  TRANS("Host Sync Mode")));
    addAndMakeVisible(label22.get());
    label22->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label22->setJustificationType(juce::Justification::centred);
    label22->setEditable(false, false, false);
    label22->setColour(juce::Label::textColourId, juce::Colours::white);
    label22->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label22->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label22->setBounds(162, 0, 95, 16);

    label18.reset(new juce::Label("Sync:",
                                  TRANS("Note Triggering")));
    addAndMakeVisible(label18.get());
    label18->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Bold"));
    label18->setJustificationType(juce::Justification::centred);
    label18->setEditable(false, false, false);
    label18->setColour(juce::Label::textColourId, juce::Colour(0xff9f9f9f));
    label18->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label18->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label18->setBounds(12, 469, 125, 16);

    loopinfoLabel.reset(new juce::Label("Loop Info",
                                        TRANS("label text")));
    addAndMakeVisible(loopinfoLabel.get());
    loopinfoLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    loopinfoLabel->setJustificationType(juce::Justification::centredLeft);
    loopinfoLabel->setEditable(false, false, false);
    loopinfoLabel->setColour(juce::Label::textColourId, juce::Colours::black);
    loopinfoLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    loopinfoLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    loopinfoLabel->setBounds(14, 205, 272, 16);

    loopinfoLabel2.reset(new juce::Label("Loop Info 2",
                                         TRANS("label text")));
    addAndMakeVisible(loopinfoLabel2.get());
    loopinfoLabel2->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    loopinfoLabel2->setJustificationType(juce::Justification::centredLeft);
    loopinfoLabel2->setEditable(false, false, false);
    loopinfoLabel2->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    loopinfoLabel2->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    loopinfoLabel2->setBounds(14, 225, 272, 16);

    label17.reset(new juce::Label("Sync:",
                                  TRANS("Loop Manipulation")));
    addAndMakeVisible(label17.get());
    label17->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Bold"));
    label17->setJustificationType(juce::Justification::centred);
    label17->setEditable(false, false, false);
    label17->setColour(juce::Label::textColourId, juce::Colour(0xff9f9f9f));
    label17->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label17->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label17->setBounds(14, 391, 125, 16);

    s_Channel.reset(new VSTSlider("Channel"));
    addAndMakeVisible(s_Channel.get());
    s_Channel->setTooltip(TRANS("Input and output channel for the current slot"));
    s_Channel->setRange(0, 16, 1);
    s_Channel->setSliderStyle(juce::Slider::LinearBar);
    s_Channel->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_Channel->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_Channel->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_Channel->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_Channel->addListener(this);

    s_Channel->setBounds(314, 414, 64, 20);

    label19.reset(new juce::Label("I/O Channel",
                                  TRANS("I/O Channel")));
    addAndMakeVisible(label19.get());
    label19->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label19->setJustificationType(juce::Justification::centred);
    label19->setEditable(false, false, false);
    label19->setColour(juce::Label::textColourId, juce::Colours::white);
    label19->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label19->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label19->setBounds(309, 398, 74, 16);

    label20.reset(new juce::Label("RecordLengthLabel",
                                  TRANS("Record Length")));
    addAndMakeVisible(label20.get());
    label20->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label20->setJustificationType(juce::Justification::centred);
    label20->setEditable(false, false, false);
    label20->setColour(juce::Label::textColourId, juce::Colours::white);
    label20->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label20->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label20->setBounds(347, 0, 83, 16);

    s_FixedLength.reset(new VSTSlider("Recording Length"));
    addAndMakeVisible(s_FixedLength.get());
    s_FixedLength->setTooltip(TRANS("If set to \"Manual\", recording will go on as long as the record button is on. Otherwise, length will be limited to this number of steps (based on \"Loop Step Size\" setting)."));
    s_FixedLength->setRange(0, 32, 1);
    s_FixedLength->setSliderStyle(juce::Slider::LinearBar);
    s_FixedLength->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_FixedLength->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e707070));
    s_FixedLength->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_FixedLength->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_FixedLength->addListener(this);

    s_FixedLength->setBounds(347, 15, 86, 16);

    b_Filt.reset(new juce::TextButton("Transform/Filter"));
    addAndMakeVisible(b_Filt.get());
    b_Filt->setTooltip(TRANS("Transform: all events in the pattern are channelized to the sected channel; Filter: only events with the selected channel will be output"));
    b_Filt->setButtonText(TRANS("Transform"));
    b_Filt->addListener(this);
    b_Filt->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));
    b_Filt->setColour(juce::TextButton::textColourOffId, juce::Colours::black);

    b_Filt->setBounds(314, 440, 64, 20);

    viewport.reset(new PianoPort("Piano Roll View"));
    addAndMakeVisible(viewport.get());
    viewport->setScrollBarThickness(16);
    viewport->setViewedComponent(new PianoRoll(this->getFilter(), this, timeline.get()));

    resizer.reset(new juce::ResizableCornerComponent(this, &resizeLimits));
    addAndMakeVisible(resizer.get());

    b_NoteToggle.reset(new juce::TextButton("new button"));
    addAndMakeVisible(b_NoteToggle.get());
    b_NoteToggle->setTooltip(TRANS("When enabled, Note On events will toggle playback, ignoring Note Off events; otherwise Note Off will stop playback"));
    b_NoteToggle->setButtonText(TRANS("Toggle"));
    b_NoteToggle->addListener(this);
    b_NoteToggle->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));

    b_NoteToggle->setBounds(257, 402, 40, 16);

    s_PlayGroup.reset(new VSTSlider("TriggerChannel"));
    addAndMakeVisible(s_PlayGroup.get());
    s_PlayGroup->setTooltip(TRANS("Slots with the same Play Group number will all start/stop at the same time"));
    s_PlayGroup->setRange(0, 16, 1);
    s_PlayGroup->setSliderStyle(juce::Slider::LinearBar);
    s_PlayGroup->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_PlayGroup->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_PlayGroup->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_PlayGroup->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_PlayGroup->addListener(this);

    s_PlayGroup->setBounds(312, 136, 64, 20);

    label13.reset(new juce::Label("Trigger Channel",
                                  TRANS("Play Group")));
    addAndMakeVisible(label13.get());
    label13->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label13->setJustificationType(juce::Justification::centred);
    label13->setEditable(false, false, false);
    label13->setColour(juce::Label::textColourId, juce::Colours::white);
    label13->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label13->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label13->setBounds(313, 119, 64, 16);

    s_MuteGroup.reset(new VSTSlider("TriggerChannel"));
    addAndMakeVisible(s_MuteGroup.get());
    s_MuteGroup->setTooltip(TRANS("Only one slot with the same Mute Group number can be played at the same time"));
    s_MuteGroup->setRange(0, 16, 1);
    s_MuteGroup->setSliderStyle(juce::Slider::LinearBar);
    s_MuteGroup->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_MuteGroup->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_MuteGroup->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_MuteGroup->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_MuteGroup->addListener(this);

    s_MuteGroup->setBounds(312, 174, 64, 20);

    label14.reset(new juce::Label("Trigger Channel",
                                  TRANS("Mute Group")));
    addAndMakeVisible(label14.get());
    label14->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label14->setJustificationType(juce::Justification::centred);
    label14->setEditable(false, false, false);
    label14->setColour(juce::Label::textColourId, juce::Colours::white);
    label14->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label14->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label14->setBounds(307, 157, 74, 16);

    b_Snap.reset(new juce::ToggleButton("new toggle button"));
    addAndMakeVisible(b_Snap.get());
    b_Snap->setTooltip(TRANS("Toggle Snap to Grid"));
    b_Snap->setButtonText(TRANS("Snap"));
    b_Snap->addListener(this);

    b_Snap->setBounds(392, 61, 59, 24);

    quantizeBox2.reset(new juce::ComboBox("PR Quantize Step"));
    addAndMakeVisible(quantizeBox2.get());
    quantizeBox2->setTooltip(TRANS("Grid Size"));
    quantizeBox2->setEditableText(false);
    quantizeBox2->setJustificationType(juce::Justification::centredLeft);
    quantizeBox2->setTextWhenNothingSelected(TRANS("32nd"));
    quantizeBox2->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    quantizeBox2->addItem(TRANS("4th"), 1);
    quantizeBox2->addItem(TRANS("8th"), 2);
    quantizeBox2->addItem(TRANS("16th"), 3);
    quantizeBox2->addItem(TRANS("32nd"), 4);
    quantizeBox2->addItem(TRANS("64th"), 5);
    quantizeBox2->addListener(this);

    quantizeBox2->setBounds(453, 64, 50, 18);

    b_ForceToKey.reset(new juce::ToggleButton("new toggle button"));
    addAndMakeVisible(b_ForceToKey.get());
    b_ForceToKey->setTooltip(TRANS("When checked, played notes will be fitted to the defined scale"));
    b_ForceToKey->setButtonText(TRANS("Force to Scale"));
    b_ForceToKey->addListener(this);
    b_ForceToKey->setColour(juce::ToggleButton::textColourId, juce::Colours::white);

    b_ForceToKey->setBounds(11, 312, 99, 17);

    keySelector.reset(new KeySelector(ownerFilter->keySelectorState));
    addAndMakeVisible(keySelector.get());
    keySelector->setName("new component");

    keySelector->setBounds(199, 315, 154, 28);

    b_ShiftUp.reset(new juce::TextButton("new button"));
    addAndMakeVisible(b_ShiftUp.get());
    b_ShiftUp->setTooltip(TRANS("Shift selected notes one semitone up"));
    b_ShiftUp->setButtonText(TRANS(">"));
    b_ShiftUp->setConnectedEdges(juce::Button::ConnectedOnLeft);
    b_ShiftUp->addListener(this);
    b_ShiftUp->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));

    b_ShiftUp->setBounds(353, 314, 21, 30);

    b_ShiftDown.reset(new juce::TextButton("new button"));
    addAndMakeVisible(b_ShiftDown.get());
    b_ShiftDown->setTooltip(TRANS("Shift selected notes one semitone down"));
    b_ShiftDown->setButtonText(TRANS("<"));
    b_ShiftDown->setConnectedEdges(juce::Button::ConnectedOnRight);
    b_ShiftDown->addListener(this);
    b_ShiftDown->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));

    b_ShiftDown->setBounds(178, 314, 21, 30);

    b_SingleLoop.reset(new juce::ToggleButton("Single Loop"));
    addAndMakeVisible(b_SingleLoop.get());
    b_SingleLoop->setTooltip(TRANS("When checked, switching from a playing slot to another slot will automatically play the new slot and stop the previous one"));
    b_SingleLoop->setButtonText(TRANS("Play active slot only"));
    b_SingleLoop->addListener(this);
    b_SingleLoop->setColour(juce::ToggleButton::textColourId, juce::Colours::white);

    b_SingleLoop->setBounds(155, 36, 122, 16);

    s_MasterVelocity.reset(new VSTSlider("VMasterVelocity"));
    addAndMakeVisible(s_MasterVelocity.get());
    s_MasterVelocity->setTooltip(TRANS("Global velocity adjustment applied to all played notes"));
    s_MasterVelocity->setRange(0, 200, 1);
    s_MasterVelocity->setSliderStyle(juce::Slider::LinearBar);
    s_MasterVelocity->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_MasterVelocity->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e707070));
    s_MasterVelocity->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_MasterVelocity->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_MasterVelocity->addListener(this);

    s_MasterVelocity->setBounds(371, 36, 72, 16);

    label15.reset(new juce::Label("new label",
                                  TRANS("Master Velocity:")));
    addAndMakeVisible(label15.get());
    label15->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label15->setJustificationType(juce::Justification::centredRight);
    label15->setEditable(false, false, false);
    label15->setColour(juce::Label::textColourId, juce::Colours::white);
    label15->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label15->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label15->setBounds(283, 36, 88, 16);

    aboutButton.reset(new juce::ImageButton("new button"));
    addAndMakeVisible(aboutButton.get());
    aboutButton->setTooltip(TRANS("Insert Piz Here-> midiLooper v1.3  http://thepiz.org/plugins/?p=midiLooper"));
    aboutButton->setButtonText(juce::String());
    aboutButton->addListener(this);

    aboutButton->setImages(false, true, true, juce::Image(), 1.000f, juce::Colour(0x00000000), juce::Image(), 1.000f, juce::Colour(0x00000000), juce::Image(), 1.000f, juce::Colour(0x00000000));
    aboutButton->setBounds(9, 1, 136, 47);

    b_Triplet.reset(new juce::TextButton("Triplet"));
    addAndMakeVisible(b_Triplet.get());
    b_Triplet->setTooltip(TRANS("Toggle Triplet Note Grid"));
    b_Triplet->setButtonText(TRANS("3"));
    b_Triplet->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    b_Triplet->setRadioGroupId(2);
    b_Triplet->addListener(this);
    b_Triplet->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff8d8d8d));

    b_Triplet->setBounds(506, 64, 18, 18);

    b_Dotted.reset(new juce::TextButton("Dotted"));
    addAndMakeVisible(b_Dotted.get());
    b_Dotted->setTooltip(TRANS("Toggle Dotted Note Grid"));
    b_Dotted->setButtonText(TRANS("."));
    b_Dotted->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    b_Dotted->setRadioGroupId(2);
    b_Dotted->addListener(this);
    b_Dotted->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff8d8d8d));

    b_Dotted->setBounds(528, 64, 18, 18);

    b_ZoomOut.reset(new juce::TextButton("ZoomOut"));
    addAndMakeVisible(b_ZoomOut.get());
    b_ZoomOut->setTooltip(TRANS("Zoom Out (Ctrl-click for vertical)"));
    b_ZoomOut->setButtonText(TRANS("-"));
    b_ZoomOut->setConnectedEdges(juce::Button::ConnectedOnRight);
    b_ZoomOut->addListener(this);
    b_ZoomOut->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    b_ZoomOut->setBounds(656, 64, 18, 18);

    b_ZoomIn.reset(new juce::TextButton("ZoomIn"));
    addAndMakeVisible(b_ZoomIn.get());
    b_ZoomIn->setTooltip(TRANS("Zoom In (Ctrl-click for vertical)"));
    b_ZoomIn->setButtonText(TRANS("+"));
    b_ZoomIn->setConnectedEdges(juce::Button::ConnectedOnLeft);
    b_ZoomIn->setRadioGroupId(2);
    b_ZoomIn->addListener(this);
    b_ZoomIn->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    b_ZoomIn->setBounds(674, 64, 18, 18);

    numerator.reset(new juce::Label("new label",
                                    TRANS("4")));
    addAndMakeVisible(numerator.get());
    numerator->setTooltip(TRANS("Time Sig Numerator"));
    numerator->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    numerator->setJustificationType(juce::Justification::centredRight);
    numerator->setEditable(true, true, false);
    numerator->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    numerator->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
    numerator->addListener(this);

    numerator->setBounds(555, 64, 27, 18);

    denominator.reset(new juce::Label("new label",
                                      TRANS("4")));
    addAndMakeVisible(denominator.get());
    denominator->setTooltip(TRANS("Time Sig Denominator"));
    denominator->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    denominator->setJustificationType(juce::Justification::centredLeft);
    denominator->setEditable(true, true, false);
    denominator->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    denominator->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
    denominator->addListener(this);

    denominator->setBounds(584, 64, 29, 18);

    b_UseScaleChannel.reset(new juce::ToggleButton("new toggle button"));
    addAndMakeVisible(b_UseScaleChannel.get());
    b_UseScaleChannel->setTooltip(TRANS("When checked, input notes on \"Scale Ch\" will be used to define the scale"));
    b_UseScaleChannel->setButtonText(TRANS("Use Scale Channel"));
    b_UseScaleChannel->addListener(this);
    b_UseScaleChannel->setColour(juce::ToggleButton::textColourId, juce::Colours::white);

    b_UseScaleChannel->setBounds(11, 329, 129, 17);

    s_ScaleChannel.reset(new VSTSlider("ScaleChannel"));
    addAndMakeVisible(s_ScaleChannel.get());
    s_ScaleChannel->setTooltip(TRANS("Input notes on this channel will affect Semitones, Octave, and/or Force to Scale settings where \"Use Scale Ch\" is enabled"));
    s_ScaleChannel->setRange(1, 16, 1);
    s_ScaleChannel->setSliderStyle(juce::Slider::LinearBar);
    s_ScaleChannel->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_ScaleChannel->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_ScaleChannel->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_ScaleChannel->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_ScaleChannel->addListener(this);

    s_ScaleChannel->setBounds(10, 283, 60, 20);

    label25.reset(new juce::Label("scale ch",
                                  TRANS("Scale Ch")));
    addAndMakeVisible(label25.get());
    label25->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label25->setJustificationType(juce::Justification::centred);
    label25->setEditable(false, false, false);
    label25->setColour(juce::Label::textColourId, juce::Colours::white);
    label25->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label25->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label25->setBounds(13, 267, 54, 16);

    s_MasterTranspose.reset(new VSTSlider("MasterTranspose"));
    addAndMakeVisible(s_MasterTranspose.get());
    s_MasterTranspose->setTooltip(TRANS("Global transposition applied to all played notes (after Force to Scale)"));
    s_MasterTranspose->setRange(-12, 12, 1);
    s_MasterTranspose->setSliderStyle(juce::Slider::LinearBar);
    s_MasterTranspose->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_MasterTranspose->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e707070));
    s_MasterTranspose->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_MasterTranspose->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_MasterTranspose->addListener(this);

    s_MasterTranspose->setBounds(554, 36, 72, 16);

    label26.reset(new juce::Label("new label",
                                  TRANS("Master Transpose:")));
    addAndMakeVisible(label26.get());
    label26->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label26->setJustificationType(juce::Justification::centredRight);
    label26->setEditable(false, false, false);
    label26->setColour(juce::Label::textColourId, juce::Colours::white);
    label26->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label26->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label26->setBounds(452, 36, 102, 16);

    b_WaitForBar.reset(new juce::ToggleButton("WaitForBar"));
    addAndMakeVisible(b_WaitForBar.get());
    b_WaitForBar->setTooltip(TRANS("When checked, play/stop of this slot will happen at the start of the bar after"));
    b_WaitForBar->setButtonText(TRANS("Wait for Next Bar"));
    b_WaitForBar->addListener(this);
    b_WaitForBar->setColour(juce::ToggleButton::textColourId, juce::Colours::white);

    b_WaitForBar->setBounds(175, 123, 107, 16);

    midiOutDeviceBox.reset(new juce::ComboBox("midiOutDevice"));
    addAndMakeVisible(midiOutDeviceBox.get());
    midiOutDeviceBox->setTooltip(TRANS("Send ouput to selected MIDI port in addition to VST host output"));
    midiOutDeviceBox->setEditableText(false);
    midiOutDeviceBox->setJustificationType(juce::Justification::centredLeft);
    midiOutDeviceBox->setTextWhenNothingSelected(TRANS("--"));
    midiOutDeviceBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    midiOutDeviceBox->addListener(this);

    midiOutDeviceBox->setBounds(633, 15, 158, 16);

    label27.reset(new juce::Label("QuantizeLabel",
                                  TRANS("MIDI Output Device")));
    addAndMakeVisible(label27.get());
    label27->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label27->setJustificationType(juce::Justification::centred);
    label27->setEditable(false, false, false);
    label27->setColour(juce::Label::textColourId, juce::Colours::white);
    label27->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label27->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label27->setBounds(627, 0, 117, 16);

    b_UseTrChannel.reset(new juce::ToggleButton("new toggle button"));
    addAndMakeVisible(b_UseTrChannel.get());
    b_UseTrChannel->setTooltip(TRANS("When checked, notes on selected \"Transpose Ch\" will apply to \"Semitones\" and \"Octave\" settings, relative to \"Root Note\""));
    b_UseTrChannel->setButtonText(TRANS("Use Transp Ch"));
    b_UseTrChannel->addListener(this);
    b_UseTrChannel->setColour(juce::ToggleButton::textColourId, juce::Colours::white);

    b_UseTrChannel->setBounds(148, 252, 130, 17);

    b_ImmediateTranspose.reset(new juce::ToggleButton("new toggle button"));
    addAndMakeVisible(b_ImmediateTranspose.get());
    b_ImmediateTranspose->setTooltip(TRANS("When checked, playing notes will be split and transposed immediately on changes to Semitones / Octave / Force to Scale / Master Transpose settings"));
    b_ImmediateTranspose->setButtonText(TRANS("Split"));
    b_ImmediateTranspose->addListener(this);
    b_ImmediateTranspose->setColour(juce::ToggleButton::textColourId, juce::Colours::white);

    b_ImmediateTranspose->setBounds(247, 252, 48, 17);

    s_NumLoops.reset(new VSTSlider("NumLoops"));
    addAndMakeVisible(s_NumLoops.get());
    s_NumLoops->setTooltip(TRANS("Number of times to loop playback"));
    s_NumLoops->setRange(0, 64, 1);
    s_NumLoops->setSliderStyle(juce::Slider::LinearBar);
    s_NumLoops->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_NumLoops->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_NumLoops->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_NumLoops->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_NumLoops->addListener(this);

    s_NumLoops->setBounds(155, 174, 64, 20);

    s_NextSlot.reset(new VSTSlider("NextSlot"));
    addAndMakeVisible(s_NextSlot.get());
    s_NextSlot->setTooltip(TRANS("What to do after the selected number of loops have played"));
    s_NextSlot->setRange(0, 16, 1);
    s_NextSlot->setSliderStyle(juce::Slider::LinearBar);
    s_NextSlot->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_NextSlot->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_NextSlot->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_NextSlot->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_NextSlot->addListener(this);

    s_NextSlot->setBounds(234, 174, 64, 20);

    label16.reset(new juce::Label("Sync:",
                                  TRANS("Loop Settings")));
    addAndMakeVisible(label16.get());
    label16->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Bold"));
    label16->setJustificationType(juce::Justification::centred);
    label16->setEditable(false, false, false);
    label16->setColour(juce::Label::textColourId, juce::Colour(0xff9f9f9f));
    label16->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label16->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label16->setBounds(12, 247, 125, 16);

    forceModeBox.reset(new juce::ComboBox("Force To Scale Mode"));
    addAndMakeVisible(forceModeBox.get());
    forceModeBox->setEditableText(false);
    forceModeBox->setJustificationType(juce::Justification::centredLeft);
    forceModeBox->setTextWhenNothingSelected(TRANS("Nearest"));
    forceModeBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    forceModeBox->addItem(TRANS("Nearest"), 1);
    forceModeBox->addItem(TRANS("Up"), 2);
    forceModeBox->addItem(TRANS("Down"), 3);
    forceModeBox->addItem(TRANS("Block"), 4);
    forceModeBox->addListener(this);

    forceModeBox->setBounds(110, 313, 61, 16);

    kbport.reset(new juce::Viewport("Keyboard View"));
    addAndMakeVisible(kbport.get());
    kbport->setScrollBarsShown(false, false);
    kbport->setScrollBarThickness(16);
    kbport->setViewedComponent(new juce::MidiKeyboardComponent(ownerFilter->kbstate, juce::MidiKeyboardComponent::verticalKeyboardFacingRight));

    b_RemoveBar.reset(new juce::TextButton("RemoveBar"));
    addAndMakeVisible(b_RemoveBar.get());
    b_RemoveBar->setTooltip(TRANS("Remove bar"));
    b_RemoveBar->setButtonText(TRANS("-"));
    b_RemoveBar->setConnectedEdges(juce::Button::ConnectedOnRight);
    b_RemoveBar->addListener(this);
    b_RemoveBar->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    b_RemoveBar->setBounds(706, 64, 18, 18);

    b_AddBar.reset(new juce::TextButton("AddBar"));
    addAndMakeVisible(b_AddBar.get());
    b_AddBar->setTooltip(TRANS("Add bar"));
    b_AddBar->setButtonText(TRANS("+"));
    b_AddBar->setConnectedEdges(juce::Button::ConnectedOnLeft);
    b_AddBar->setRadioGroupId(2);
    b_AddBar->addListener(this);
    b_AddBar->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    b_AddBar->setBounds(773, 64, 18, 18);

    LengthLabel.reset(new juce::Label("Length",
                                      TRANS("4")));
    addAndMakeVisible(LengthLabel.get());
    LengthLabel->setTooltip(TRANS("Pattern length in bars"));
    LengthLabel->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    LengthLabel->setJustificationType(juce::Justification::centred);
    LengthLabel->setEditable(true, true, false);
    LengthLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    LengthLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));
    LengthLabel->addListener(this);

    LengthLabel->setBounds(724, 65, 49, 16);

    textButton17.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton17.get());
    textButton17->setButtonText(TRANS("1"));
    textButton17->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton17->setRadioGroupId(1);

    textButton17->setBounds(4, -97, 25, 24);

    textButton18.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton18.get());
    textButton18->setButtonText(TRANS("2"));
    textButton18->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton18->setRadioGroupId(1);
    textButton18->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton18->setBounds(29, -97, 23, 24);

    textButton19.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton19.get());
    textButton19->setButtonText(TRANS("3"));
    textButton19->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton19->setRadioGroupId(1);

    textButton19->setBounds(52, -97, 23, 24);

    textButton20.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton20.get());
    textButton20->setButtonText(TRANS("4"));
    textButton20->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton20->setRadioGroupId(1);

    textButton20->setBounds(75, -97, 23, 24);

    textButton21.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton21.get());
    textButton21->setButtonText(TRANS("5"));
    textButton21->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton21->setRadioGroupId(1);

    textButton21->setBounds(98, -97, 23, 24);

    textButton22.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton22.get());
    textButton22->setButtonText(TRANS("6"));
    textButton22->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton22->setRadioGroupId(1);

    textButton22->setBounds(121, -97, 23, 24);

    textButton23.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton23.get());
    textButton23->setButtonText(TRANS("7"));
    textButton23->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton23->setRadioGroupId(1);

    textButton23->setBounds(144, -97, 23, 24);

    textButton24.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton24.get());
    textButton24->setButtonText(TRANS("8"));
    textButton24->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton24->setRadioGroupId(1);

    textButton24->setBounds(167, -97, 23, 24);

    textButton25.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton25.get());
    textButton25->setButtonText(TRANS("9"));
    textButton25->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton25->setRadioGroupId(1);

    textButton25->setBounds(190, -97, 23, 24);

    textButton26.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton26.get());
    textButton26->setButtonText(TRANS("10"));
    textButton26->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton26->setRadioGroupId(1);

    textButton26->setBounds(213, -97, 25, 24);

    textButton27.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton27.get());
    textButton27->setButtonText(TRANS("11"));
    textButton27->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton27->setRadioGroupId(1);

    textButton27->setBounds(238, -97, 25, 24);

    textButton28.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton28.get());
    textButton28->setButtonText(TRANS("12"));
    textButton28->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton28->setRadioGroupId(1);

    textButton28->setBounds(263, -97, 25, 24);

    textButton29.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton29.get());
    textButton29->setButtonText(TRANS("13"));
    textButton29->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton29->setRadioGroupId(1);

    textButton29->setBounds(288, -97, 25, 24);

    textButton30.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton30.get());
    textButton30->setButtonText(TRANS("14"));
    textButton30->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton30->setRadioGroupId(1);

    textButton30->setBounds(313, -97, 25, 24);

    textButton31.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton31.get());
    textButton31->setButtonText(TRANS("15"));
    textButton31->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton31->setRadioGroupId(1);

    textButton31->setBounds(338, -97, 25, 24);

    textButton32.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton32.get());
    textButton32->setButtonText(TRANS("16"));
    textButton32->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton32->setRadioGroupId(1);

    textButton32->setBounds(363, -97, 28, 24);

    textButton33.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton33.get());
    textButton33->setButtonText(TRANS("1"));
    textButton33->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton33->setRadioGroupId(1);

    textButton33->setBounds(1, -73, 25, 24);

    textButton34.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton34.get());
    textButton34->setButtonText(TRANS("2"));
    textButton34->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton34->setRadioGroupId(1);
    textButton34->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton34->setBounds(26, -73, 23, 24);

    textButton35.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton35.get());
    textButton35->setButtonText(TRANS("3"));
    textButton35->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton35->setRadioGroupId(1);

    textButton35->setBounds(49, -73, 23, 24);

    textButton36.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton36.get());
    textButton36->setButtonText(TRANS("4"));
    textButton36->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton36->setRadioGroupId(1);

    textButton36->setBounds(72, -73, 23, 24);

    textButton37.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton37.get());
    textButton37->setButtonText(TRANS("5"));
    textButton37->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton37->setRadioGroupId(1);

    textButton37->setBounds(95, -73, 23, 24);

    textButton38.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton38.get());
    textButton38->setButtonText(TRANS("6"));
    textButton38->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton38->setRadioGroupId(1);

    textButton38->setBounds(118, -73, 23, 24);

    textButton39.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton39.get());
    textButton39->setButtonText(TRANS("7"));
    textButton39->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton39->setRadioGroupId(1);

    textButton39->setBounds(141, -73, 23, 24);

    textButton40.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton40.get());
    textButton40->setButtonText(TRANS("8"));
    textButton40->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton40->setRadioGroupId(1);

    textButton40->setBounds(164, -73, 23, 24);

    textButton41.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton41.get());
    textButton41->setButtonText(TRANS("9"));
    textButton41->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton41->setRadioGroupId(1);

    textButton41->setBounds(187, -73, 23, 24);

    textButton42.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton42.get());
    textButton42->setButtonText(TRANS("10"));
    textButton42->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton42->setRadioGroupId(1);

    textButton42->setBounds(210, -73, 25, 24);

    textButton43.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton43.get());
    textButton43->setButtonText(TRANS("11"));
    textButton43->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton43->setRadioGroupId(1);

    textButton43->setBounds(235, -73, 25, 24);

    textButton44.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton44.get());
    textButton44->setButtonText(TRANS("12"));
    textButton44->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton44->setRadioGroupId(1);

    textButton44->setBounds(260, -73, 25, 24);

    textButton45.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton45.get());
    textButton45->setButtonText(TRANS("13"));
    textButton45->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton45->setRadioGroupId(1);

    textButton45->setBounds(285, -73, 25, 24);

    textButton46.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton46.get());
    textButton46->setButtonText(TRANS("14"));
    textButton46->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton46->setRadioGroupId(1);

    textButton46->setBounds(310, -73, 25, 24);

    textButton47.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton47.get());
    textButton47->setButtonText(TRANS("15"));
    textButton47->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton47->setRadioGroupId(1);

    textButton47->setBounds(335, -73, 25, 24);

    textButton48.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton48.get());
    textButton48->setButtonText(TRANS("16"));
    textButton48->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton48->setRadioGroupId(1);

    textButton48->setBounds(360, -73, 28, 24);

    textButton49.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton49.get());
    textButton49->setButtonText(TRANS("1"));
    textButton49->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton49->setRadioGroupId(1);

    textButton49->setBounds(-2, -40, 25, 24);

    textButton50.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton50.get());
    textButton50->setButtonText(TRANS("2"));
    textButton50->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton50->setRadioGroupId(1);
    textButton50->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton50->setBounds(23, -40, 23, 24);

    textButton51.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton51.get());
    textButton51->setButtonText(TRANS("3"));
    textButton51->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton51->setRadioGroupId(1);

    textButton51->setBounds(46, -40, 23, 24);

    textButton52.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton52.get());
    textButton52->setButtonText(TRANS("4"));
    textButton52->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton52->setRadioGroupId(1);

    textButton52->setBounds(69, -40, 23, 24);

    textButton53.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton53.get());
    textButton53->setButtonText(TRANS("5"));
    textButton53->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton53->setRadioGroupId(1);

    textButton53->setBounds(92, -40, 23, 24);

    textButton54.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton54.get());
    textButton54->setButtonText(TRANS("6"));
    textButton54->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton54->setRadioGroupId(1);

    textButton54->setBounds(115, -40, 23, 24);

    textButton55.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton55.get());
    textButton55->setButtonText(TRANS("7"));
    textButton55->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton55->setRadioGroupId(1);

    textButton55->setBounds(138, -40, 23, 24);

    textButton56.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton56.get());
    textButton56->setButtonText(TRANS("8"));
    textButton56->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton56->setRadioGroupId(1);

    textButton56->setBounds(161, -40, 23, 24);

    textButton57.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton57.get());
    textButton57->setButtonText(TRANS("9"));
    textButton57->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton57->setRadioGroupId(1);

    textButton57->setBounds(184, -40, 23, 24);

    textButton58.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton58.get());
    textButton58->setButtonText(TRANS("10"));
    textButton58->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton58->setRadioGroupId(1);

    textButton58->setBounds(207, -40, 25, 24);

    textButton59.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton59.get());
    textButton59->setButtonText(TRANS("11"));
    textButton59->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton59->setRadioGroupId(1);

    textButton59->setBounds(232, -40, 25, 24);

    textButton60.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton60.get());
    textButton60->setButtonText(TRANS("12"));
    textButton60->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton60->setRadioGroupId(1);

    textButton60->setBounds(257, -40, 25, 24);

    textButton61.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton61.get());
    textButton61->setButtonText(TRANS("13"));
    textButton61->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton61->setRadioGroupId(1);

    textButton61->setBounds(282, -40, 25, 24);

    textButton62.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton62.get());
    textButton62->setButtonText(TRANS("14"));
    textButton62->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton62->setRadioGroupId(1);

    textButton62->setBounds(307, -40, 25, 24);

    textButton63.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton63.get());
    textButton63->setButtonText(TRANS("15"));
    textButton63->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton63->setRadioGroupId(1);

    textButton63->setBounds(332, -40, 25, 24);

    textButton64.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton64.get());
    textButton64->setButtonText(TRANS("16"));
    textButton64->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton64->setRadioGroupId(1);

    textButton64->setBounds(357, -40, 28, 24);

    textButton65.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton65.get());
    textButton65->setButtonText(TRANS("1"));
    textButton65->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton65->setRadioGroupId(1);

    textButton65->setBounds(383, -93, 25, 24);

    textButton66.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton66.get());
    textButton66->setButtonText(TRANS("2"));
    textButton66->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton66->setRadioGroupId(1);
    textButton66->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton66->setBounds(408, -93, 23, 24);

    textButton67.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton67.get());
    textButton67->setButtonText(TRANS("3"));
    textButton67->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton67->setRadioGroupId(1);

    textButton67->setBounds(431, -93, 23, 24);

    textButton68.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton68.get());
    textButton68->setButtonText(TRANS("4"));
    textButton68->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton68->setRadioGroupId(1);

    textButton68->setBounds(454, -93, 23, 24);

    textButton69.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton69.get());
    textButton69->setButtonText(TRANS("5"));
    textButton69->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton69->setRadioGroupId(1);

    textButton69->setBounds(477, -93, 23, 24);

    textButton70.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton70.get());
    textButton70->setButtonText(TRANS("6"));
    textButton70->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton70->setRadioGroupId(1);

    textButton70->setBounds(500, -93, 23, 24);

    textButton71.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton71.get());
    textButton71->setButtonText(TRANS("7"));
    textButton71->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton71->setRadioGroupId(1);

    textButton71->setBounds(523, -93, 23, 24);

    textButton72.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton72.get());
    textButton72->setButtonText(TRANS("8"));
    textButton72->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton72->setRadioGroupId(1);

    textButton72->setBounds(546, -93, 23, 24);

    textButton73.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton73.get());
    textButton73->setButtonText(TRANS("9"));
    textButton73->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton73->setRadioGroupId(1);

    textButton73->setBounds(569, -93, 23, 24);

    textButton74.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton74.get());
    textButton74->setButtonText(TRANS("10"));
    textButton74->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton74->setRadioGroupId(1);

    textButton74->setBounds(592, -93, 25, 24);

    textButton75.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton75.get());
    textButton75->setButtonText(TRANS("11"));
    textButton75->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton75->setRadioGroupId(1);

    textButton75->setBounds(617, -93, 25, 24);

    textButton76.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton76.get());
    textButton76->setButtonText(TRANS("12"));
    textButton76->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton76->setRadioGroupId(1);

    textButton76->setBounds(642, -93, 25, 24);

    textButton77.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton77.get());
    textButton77->setButtonText(TRANS("13"));
    textButton77->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton77->setRadioGroupId(1);

    textButton77->setBounds(667, -93, 25, 24);

    textButton78.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton78.get());
    textButton78->setButtonText(TRANS("14"));
    textButton78->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton78->setRadioGroupId(1);

    textButton78->setBounds(692, -93, 25, 24);

    textButton79.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton79.get());
    textButton79->setButtonText(TRANS("15"));
    textButton79->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton79->setRadioGroupId(1);

    textButton79->setBounds(717, -93, 25, 24);

    textButton80.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton80.get());
    textButton80->setButtonText(TRANS("16"));
    textButton80->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton80->setRadioGroupId(1);

    textButton80->setBounds(742, -93, 28, 24);

    textButton81.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton81.get());
    textButton81->setButtonText(TRANS("1"));
    textButton81->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton81->setRadioGroupId(1);

    textButton81->setBounds(371, -76, 25, 24);

    textButton82.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton82.get());
    textButton82->setButtonText(TRANS("2"));
    textButton82->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton82->setRadioGroupId(1);
    textButton82->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton82->setBounds(396, -76, 23, 24);

    textButton83.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton83.get());
    textButton83->setButtonText(TRANS("3"));
    textButton83->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton83->setRadioGroupId(1);

    textButton83->setBounds(419, -76, 23, 24);

    textButton84.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton84.get());
    textButton84->setButtonText(TRANS("4"));
    textButton84->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton84->setRadioGroupId(1);

    textButton84->setBounds(442, -76, 23, 24);

    textButton85.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton85.get());
    textButton85->setButtonText(TRANS("5"));
    textButton85->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton85->setRadioGroupId(1);

    textButton85->setBounds(465, -76, 23, 24);

    textButton86.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton86.get());
    textButton86->setButtonText(TRANS("6"));
    textButton86->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton86->setRadioGroupId(1);

    textButton86->setBounds(488, -76, 23, 24);

    textButton87.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton87.get());
    textButton87->setButtonText(TRANS("7"));
    textButton87->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton87->setRadioGroupId(1);

    textButton87->setBounds(511, -76, 23, 24);

    textButton88.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton88.get());
    textButton88->setButtonText(TRANS("8"));
    textButton88->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton88->setRadioGroupId(1);

    textButton88->setBounds(534, -76, 23, 24);

    textButton89.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton89.get());
    textButton89->setButtonText(TRANS("9"));
    textButton89->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton89->setRadioGroupId(1);

    textButton89->setBounds(557, -76, 23, 24);

    textButton90.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton90.get());
    textButton90->setButtonText(TRANS("10"));
    textButton90->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton90->setRadioGroupId(1);

    textButton90->setBounds(580, -76, 25, 24);

    textButton91.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton91.get());
    textButton91->setButtonText(TRANS("11"));
    textButton91->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton91->setRadioGroupId(1);

    textButton91->setBounds(605, -76, 25, 24);

    textButton92.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton92.get());
    textButton92->setButtonText(TRANS("12"));
    textButton92->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton92->setRadioGroupId(1);

    textButton92->setBounds(630, -76, 25, 24);

    textButton93.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton93.get());
    textButton93->setButtonText(TRANS("13"));
    textButton93->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton93->setRadioGroupId(1);

    textButton93->setBounds(655, -76, 25, 24);

    textButton94.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton94.get());
    textButton94->setButtonText(TRANS("14"));
    textButton94->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton94->setRadioGroupId(1);

    textButton94->setBounds(680, -76, 25, 24);

    textButton95.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton95.get());
    textButton95->setButtonText(TRANS("15"));
    textButton95->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton95->setRadioGroupId(1);

    textButton95->setBounds(705, -76, 25, 24);

    textButton96.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton96.get());
    textButton96->setButtonText(TRANS("16"));
    textButton96->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton96->setRadioGroupId(1);

    textButton96->setBounds(730, -76, 28, 24);

    textButton97.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton97.get());
    textButton97->setButtonText(TRANS("1"));
    textButton97->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton97->setRadioGroupId(1);

    textButton97->setBounds(413, -59, 25, 24);

    textButton98.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton98.get());
    textButton98->setButtonText(TRANS("2"));
    textButton98->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton98->setRadioGroupId(1);
    textButton98->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton98->setBounds(438, -59, 23, 24);

    textButton99.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton99.get());
    textButton99->setButtonText(TRANS("3"));
    textButton99->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton99->setRadioGroupId(1);

    textButton99->setBounds(461, -59, 23, 24);

    textButton100.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton100.get());
    textButton100->setButtonText(TRANS("4"));
    textButton100->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton100->setRadioGroupId(1);

    textButton100->setBounds(484, -59, 23, 24);

    textButton101.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton101.get());
    textButton101->setButtonText(TRANS("5"));
    textButton101->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton101->setRadioGroupId(1);

    textButton101->setBounds(507, -59, 23, 24);

    textButton102.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton102.get());
    textButton102->setButtonText(TRANS("6"));
    textButton102->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton102->setRadioGroupId(1);

    textButton102->setBounds(530, -59, 23, 24);

    textButton103.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton103.get());
    textButton103->setButtonText(TRANS("7"));
    textButton103->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton103->setRadioGroupId(1);

    textButton103->setBounds(553, -59, 23, 24);

    textButton104.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton104.get());
    textButton104->setButtonText(TRANS("8"));
    textButton104->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton104->setRadioGroupId(1);

    textButton104->setBounds(576, -59, 23, 24);

    textButton105.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton105.get());
    textButton105->setButtonText(TRANS("9"));
    textButton105->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton105->setRadioGroupId(1);

    textButton105->setBounds(599, -59, 23, 24);

    textButton106.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton106.get());
    textButton106->setButtonText(TRANS("10"));
    textButton106->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton106->setRadioGroupId(1);

    textButton106->setBounds(622, -59, 25, 24);

    textButton107.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton107.get());
    textButton107->setButtonText(TRANS("11"));
    textButton107->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton107->setRadioGroupId(1);

    textButton107->setBounds(647, -59, 25, 24);

    textButton108.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton108.get());
    textButton108->setButtonText(TRANS("12"));
    textButton108->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton108->setRadioGroupId(1);

    textButton108->setBounds(672, -59, 25, 24);

    textButton109.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton109.get());
    textButton109->setButtonText(TRANS("13"));
    textButton109->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton109->setRadioGroupId(1);

    textButton109->setBounds(697, -59, 25, 24);

    textButton110.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton110.get());
    textButton110->setButtonText(TRANS("14"));
    textButton110->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton110->setRadioGroupId(1);

    textButton110->setBounds(722, -59, 25, 24);

    textButton111.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton111.get());
    textButton111->setButtonText(TRANS("15"));
    textButton111->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton111->setRadioGroupId(1);

    textButton111->setBounds(747, -59, 25, 24);

    textButton112.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton112.get());
    textButton112->setButtonText(TRANS("16"));
    textButton112->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton112->setRadioGroupId(1);

    textButton112->setBounds(772, -59, 28, 24);

    textButton113.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton113.get());
    textButton113->setButtonText(TRANS("1"));
    textButton113->setConnectedEdges(juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton113->setRadioGroupId(1);

    textButton113->setBounds(408, -29, 25, 24);

    textButton114.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton114.get());
    textButton114->setButtonText(TRANS("2"));
    textButton114->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton114->setRadioGroupId(1);
    textButton114->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffbbbbff));

    textButton114->setBounds(433, -29, 23, 24);

    textButton115.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton115.get());
    textButton115->setButtonText(TRANS("3"));
    textButton115->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton115->setRadioGroupId(1);

    textButton115->setBounds(456, -29, 23, 24);

    textButton116.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton116.get());
    textButton116->setButtonText(TRANS("4"));
    textButton116->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton116->setRadioGroupId(1);

    textButton116->setBounds(479, -29, 23, 24);

    textButton117.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton117.get());
    textButton117->setButtonText(TRANS("5"));
    textButton117->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton117->setRadioGroupId(1);

    textButton117->setBounds(502, -29, 23, 24);

    textButton118.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton118.get());
    textButton118->setButtonText(TRANS("6"));
    textButton118->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton118->setRadioGroupId(1);

    textButton118->setBounds(525, -29, 23, 24);

    textButton119.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton119.get());
    textButton119->setButtonText(TRANS("7"));
    textButton119->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton119->setRadioGroupId(1);

    textButton119->setBounds(548, -29, 23, 24);

    textButton120.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton120.get());
    textButton120->setButtonText(TRANS("8"));
    textButton120->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton120->setRadioGroupId(1);

    textButton120->setBounds(571, -29, 23, 24);

    textButton121.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton121.get());
    textButton121->setButtonText(TRANS("9"));
    textButton121->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton121->setRadioGroupId(1);

    textButton121->setBounds(594, -29, 23, 24);

    textButton122.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton122.get());
    textButton122->setButtonText(TRANS("10"));
    textButton122->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton122->setRadioGroupId(1);

    textButton122->setBounds(617, -29, 25, 24);

    textButton123.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton123.get());
    textButton123->setButtonText(TRANS("11"));
    textButton123->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton123->setRadioGroupId(1);

    textButton123->setBounds(642, -29, 25, 24);

    textButton124.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton124.get());
    textButton124->setButtonText(TRANS("12"));
    textButton124->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton124->setRadioGroupId(1);

    textButton124->setBounds(667, -29, 25, 24);

    textButton125.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton125.get());
    textButton125->setButtonText(TRANS("13"));
    textButton125->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton125->setRadioGroupId(1);

    textButton125->setBounds(692, -29, 25, 24);

    textButton126.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton126.get());
    textButton126->setButtonText(TRANS("14"));
    textButton126->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton126->setRadioGroupId(1);

    textButton126->setBounds(717, -29, 25, 24);

    textButton127.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton127.get());
    textButton127->setButtonText(TRANS("15"));
    textButton127->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    textButton127->setRadioGroupId(1);

    textButton127->setBounds(742, -29, 25, 24);

    textButton128.reset(new juce::TextButton("new button"));
    addAndMakeVisible(textButton128.get());
    textButton128->setButtonText(TRANS("16"));
    textButton128->setConnectedEdges(juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    textButton128->setRadioGroupId(1);

    textButton128->setBounds(767, -29, 28, 24);

    b_Transpose10.reset(new juce::TextButton("new button"));
    addAndMakeVisible(b_Transpose10.get());
    b_Transpose10->setButtonText(TRANS("transpose channel 10"));
    b_Transpose10->addListener(this);
    b_Transpose10->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));

    b_Transpose10->setBounds(306, 252, 72, 13);

    b_KeepLength.reset(new juce::ToggleButton("OverdubMode"));
    addAndMakeVisible(b_KeepLength.get());
    b_KeepLength->setTooltip(TRANS("When checked, overdubbing will loop record into existing loop length"));
    b_KeepLength->setButtonText(TRANS("Keep Length"));
    b_KeepLength->addListener(this);
    b_KeepLength->setColour(juce::ToggleButton::textColourId, juce::Colours::white);

    b_KeepLength->setBounds(520, -2, 83, 16);

    s_RecCC.reset(new VSTSlider("recCC"));
    addAndMakeVisible(s_RecCC.get());
    s_RecCC->setTooltip(TRANS("CC Number to toggle recoring to active slot"));
    s_RecCC->setRange(-2, 127, 1);
    s_RecCC->setSliderStyle(juce::Slider::LinearBar);
    s_RecCC->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_RecCC->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_RecCC->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_RecCC->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_RecCC->addListener(this);

    s_RecCC->setBounds(11, 149, 64, 20);

    s_PlayCC.reset(new VSTSlider("playCC"));
    addAndMakeVisible(s_PlayCC.get());
    s_PlayCC->setTooltip(TRANS("CC Number to toggle play for active slot"));
    s_PlayCC->setRange(-2, 127, 1);
    s_PlayCC->setSliderStyle(juce::Slider::LinearBar);
    s_PlayCC->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_PlayCC->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_PlayCC->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_PlayCC->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_PlayCC->addListener(this);

    s_PlayCC->setBounds(81, 149, 64, 20);

    s_VelocitySens.reset(new VSTSlider("Velocity Sensitivity"));
    addAndMakeVisible(s_VelocitySens.get());
    s_VelocitySens->setTooltip(TRANS("Velocity Sensitivity (Input Velocity -> Output Velocity)"));
    s_VelocitySens->setRange(0, 200, 1);
    s_VelocitySens->setSliderStyle(juce::Slider::LinearBar);
    s_VelocitySens->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_VelocitySens->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_VelocitySens->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_VelocitySens->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_VelocitySens->addListener(this);

    s_VelocitySens->setBounds(234, 440, 64, 20);

    label24.reset(new juce::Label("new label",
                                  TRANS("VeloSens")));
    addAndMakeVisible(label24.get());
    label24->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label24->setJustificationType(juce::Justification::centred);
    label24->setEditable(false, false, false);
    label24->setColour(juce::Label::textColourId, juce::Colours::white);
    label24->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label24->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label24->setBounds(225, 424, 80, 16);

    b_Monitor.reset(new juce::TextButton("MIDI_Monitor"));
    addAndMakeVisible(b_Monitor.get());
    b_Monitor->setTooltip(TRANS("Monitor input MIDI through active slot\'s settings (Transpose, Scale, I/O Channel)"));
    b_Monitor->setButtonText(TRANS("Monitor"));
    b_Monitor->addListener(this);
    b_Monitor->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff999999));

    b_Monitor->setBounds(678, 33, 55, 20);

    s_TransposeChannel.reset(new VSTSlider("TransposeChannel"));
    addAndMakeVisible(s_TransposeChannel.get());
    s_TransposeChannel->setTooltip(TRANS("Input notes on this channel will affect Semitones, Octave, and/or Force to Scale settings where \"Use Transp Ch\" is enabled"));
    s_TransposeChannel->setRange(1, 16, 1);
    s_TransposeChannel->setSliderStyle(juce::Slider::LinearBar);
    s_TransposeChannel->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    s_TransposeChannel->setColour(juce::Slider::backgroundColourId, juce::Colour(0x1e000000));
    s_TransposeChannel->setColour(juce::Slider::thumbColourId, juce::Colours::black);
    s_TransposeChannel->setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    s_TransposeChannel->addListener(this);

    s_TransposeChannel->setBounds(76, 283, 60, 20);

    label28.reset(new juce::Label("tr ch",
                                  TRANS("Transpose Ch")));
    addAndMakeVisible(label28.get());
    label28->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    label28->setJustificationType(juce::Justification::centred);
    label28->setEditable(false, false, false);
    label28->setColour(juce::Label::textColourId, juce::Colours::white);
    label28->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    label28->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    label28->setBounds(66, 267, 80, 16);

    cachedImage_piznew40_png_1 = juce::ImageCache::getFromMemory(piznew40_png, piznew40_pngSize);

    //[UserPreSize]
    DBG("PizLooperEditor()");
    aboutButton->setTooltip(L"Insert Piz Here-> midiLooper v" + juce::String(JucePlugin_VersionString) + " http://thepiz.org/plugins/?p=midiLooper");
    viewport->setTimeline(timeline.get());
    viewport->setKeyboard(kbport.get());
    this->setMouseClickGrabsKeyboardFocus(false);
    for (int slot = 0; slot < numSlots; slot++)
    {
        getButtonForSlot(slot)->addListener(this);
        getButtonForSlot(slot)->addMouseListener(this, false);
        getButtonForSlot(slot)->setMouseClickGrabsKeyboardFocus(false);
    }
    s_PlayCC->setVisible(false);
    s_RecCC->setVisible(false);
    b_Play->setMouseClickGrabsKeyboardFocus(false);
    b_Record->setMouseClickGrabsKeyboardFocus(false);
    b_Overdub->setMouseClickGrabsKeyboardFocus(false);
    b_Thru->setMouseClickGrabsKeyboardFocus(false);
    b_Clear->setMouseClickGrabsKeyboardFocus(false);
    s_FixedLength->setMouseClickGrabsKeyboardFocus(false);
    stepsizeBox->setMouseClickGrabsKeyboardFocus(false);
    s_Transpose->setMouseClickGrabsKeyboardFocus(false);
    s_Octave->setMouseClickGrabsKeyboardFocus(false);
    s_Velocity->setMouseClickGrabsKeyboardFocus(false);
    label3->setMouseClickGrabsKeyboardFocus(false);
    label4->setMouseClickGrabsKeyboardFocus(false);
    label5->setMouseClickGrabsKeyboardFocus(false);
    s_Start->setMouseClickGrabsKeyboardFocus(false);
    label6->setMouseClickGrabsKeyboardFocus(false);
    s_End->setMouseClickGrabsKeyboardFocus(false);
    label7->setMouseClickGrabsKeyboardFocus(false);
    s_Stretch->setMouseClickGrabsKeyboardFocus(false);
    label8->setMouseClickGrabsKeyboardFocus(false);
    loopmodeBox->setMouseClickGrabsKeyboardFocus(false);
    notetriggerBox->setMouseClickGrabsKeyboardFocus(false);
    syncmodeBox->setMouseClickGrabsKeyboardFocus(false);
    s_Root->setMouseClickGrabsKeyboardFocus(false);
    label9->setMouseClickGrabsKeyboardFocus(false);
    s_Low->setMouseClickGrabsKeyboardFocus(false);
    label10->setMouseClickGrabsKeyboardFocus(false);
    s_High->setMouseClickGrabsKeyboardFocus(false);
    label11->setMouseClickGrabsKeyboardFocus(false);
    s_TrigChan->setMouseClickGrabsKeyboardFocus(false);
    label12->setMouseClickGrabsKeyboardFocus(false);
    b_Reload->setMouseClickGrabsKeyboardFocus(false);
    quantizeBox->setMouseClickGrabsKeyboardFocus(false);
    label13->setMouseClickGrabsKeyboardFocus(false);
    s_Shift->setMouseClickGrabsKeyboardFocus(false);
    label2->setMouseClickGrabsKeyboardFocus(false);
    label14->setMouseClickGrabsKeyboardFocus(false);
    nameLabel->setMouseClickGrabsKeyboardFocus(false);
    b_Save->setMouseClickGrabsKeyboardFocus(false);
    label22->setMouseClickGrabsKeyboardFocus(false);
    label18->setMouseClickGrabsKeyboardFocus(false);
    loopinfoLabel->setMouseClickGrabsKeyboardFocus(false);
    loopinfoLabel2->setMouseClickGrabsKeyboardFocus(false);
    label17->setMouseClickGrabsKeyboardFocus(false);
    s_Channel->setMouseClickGrabsKeyboardFocus(false);
    label19->setMouseClickGrabsKeyboardFocus(false);
    b_Filt->setMouseClickGrabsKeyboardFocus(false);
    b_SingleLoop->setMouseClickGrabsKeyboardFocus(false);
    label15->setMouseClickGrabsKeyboardFocus(false);
    b_Triplet->setMouseClickGrabsKeyboardFocus(false);
    b_Dotted->setMouseClickGrabsKeyboardFocus(false);
    label->setMouseClickGrabsKeyboardFocus(false);
    b_ZoomIn->setMouseClickGrabsKeyboardFocus(false);
    b_ZoomOut->setMouseClickGrabsKeyboardFocus(false);
    b_UseScaleChannel->setMouseClickGrabsKeyboardFocus(false);
    b_UseTrChannel->setMouseClickGrabsKeyboardFocus(false);
    b_WaitForBar->setMouseClickGrabsKeyboardFocus(false);
    b_ImmediateTranspose->setMouseClickGrabsKeyboardFocus(false);
    b_ForceToKey->setMouseClickGrabsKeyboardFocus(false);
    b_Snap->setMouseClickGrabsKeyboardFocus(false);
    s_NextSlot->setMouseClickGrabsKeyboardFocus(false);
    s_NumLoops->setMouseClickGrabsKeyboardFocus(false);
    b_AddBar->setMouseClickGrabsKeyboardFocus(false);
    b_RemoveBar->setMouseClickGrabsKeyboardFocus(false);
    s_RecCC->setMouseClickGrabsKeyboardFocus(false);
    s_PlayCC->setMouseClickGrabsKeyboardFocus(false);
    s_VelocitySens->setMouseClickGrabsKeyboardFocus(false);
    b_Monitor->setMouseClickGrabsKeyboardFocus(false);
    //b_Monitor->setVisible(false);

    s_Stretch->setOwner(getAudioProcessor(), kStretch);
    s_High->setOwner(getAudioProcessor(), kNHigh);
    s_Low->setOwner(getAudioProcessor(), kNLow);
    s_Root->setOwner(getAudioProcessor(), kRoot);
    s_Transpose->setOwner(getAudioProcessor(), kTranspose);
    s_Octave->setOwner(getAudioProcessor(), kOctave);
    s_Velocity->setOwner(getAudioProcessor(), kVelocity);
    s_Start->setOwner(getAudioProcessor(), kLoopStart);
    s_End->setOwner(getAudioProcessor(), kLoopEnd);
    s_Shift->setOwner(getAudioProcessor(), kShift);
    s_Channel->setOwner(getAudioProcessor(), kChannel);
    s_TrigChan->setOwner(getAudioProcessor(), kTrigChan);
    s_FixedLength->setOwner(getAudioProcessor(), kFixedLength);
    s_PlayGroup->setOwner(getAudioProcessor(), kPlayGroup);
    s_MuteGroup->setOwner(getAudioProcessor(), kMuteGroup);
    s_MasterVelocity->setOwner(getAudioProcessor(), kMasterVelocity);
    s_MasterTranspose->setOwner(getAudioProcessor(), kMasterTranspose);
    s_ScaleChannel->setOwner(getAudioProcessor(), kScaleChannel);
    s_TransposeChannel->setOwner(getAudioProcessor(), kTransposeChannel);
    s_NumLoops->setOwner(getAudioProcessor(), kNumLoops);
    s_NextSlot->setOwner(getAudioProcessor(), kNextSlot);
    s_RecCC->setOwner(getAudioProcessor(), kRecCC);
    s_PlayCC->setOwner(getAudioProcessor(), kPlayCC);
    s_VelocitySens->setOwner(getAudioProcessor(), kVeloSens);

    s_NumLoops->addMouseListener(this, true);
    s_NextSlot->addMouseListener(this, true);
    s_Channel->addMouseListener(this, true);
    s_PlayGroup->addMouseListener(this, true);
    s_MuteGroup->addMouseListener(this, true);
    s_Stretch->addMouseListener(this, true);
    s_Transpose->addMouseListener(this, true);
    s_Octave->addMouseListener(this, true);
    s_Start->addMouseListener(this, true);
    s_End->addMouseListener(this, true);
    s_Shift->addMouseListener(this, true);
    s_Velocity->addMouseListener(this, true);
    s_VelocitySens->addMouseListener(this, true);
    s_MasterVelocity->addMouseListener(this, true);
    s_MasterTranspose->addMouseListener(this, true);
    s_NumLoops->setDoubleClickReturnValue(true, 0);
    s_NextSlot->setDoubleClickReturnValue(true, 0);
    s_Channel->setDoubleClickReturnValue(true, 0);
    s_PlayGroup->setDoubleClickReturnValue(true, 0);
    s_MuteGroup->setDoubleClickReturnValue(true, 0);
    s_Stretch->setDoubleClickReturnValue(true, 0);
    s_Transpose->setDoubleClickReturnValue(true, 0);
    s_Octave->setDoubleClickReturnValue(true, 0);
    s_Start->setDoubleClickReturnValue(true, 0);
    s_End->setDoubleClickReturnValue(true, 0);
    s_Shift->setDoubleClickReturnValue(true, 0);
    s_Velocity->setDoubleClickReturnValue(true, 100);
    s_VelocitySens->setDoubleClickReturnValue(true, 0);
    s_MasterVelocity->setDoubleClickReturnValue(true, 0);
    s_MasterTranspose->setDoubleClickReturnValue(true, 0);

    b_Record->addMouseListener(this, false);
    b_Play->addMouseListener(this, false);
    s_PlayCC->addMouseListener(this, true);
    s_PlayCC->setSliderSnapsToMousePosition(false);
    s_PlayCC->setDoubleClickReturnValue(true, -1);
    s_RecCC->addMouseListener(this, true);
    s_RecCC->setSliderSnapsToMousePosition(false);
    s_RecCC->setDoubleClickReturnValue(true, -1);
    s_Low->setDoubleClickReturnValue(true, 0);
    s_Low->setSliderSnapsToMousePosition(false);
    s_Low->addMouseListener(this, true);
    s_High->setDoubleClickReturnValue(true, 127);
    s_High->setSliderSnapsToMousePosition(false);
    s_High->addMouseListener(this, true);
    s_Root->setDoubleClickReturnValue(true, 60);
    s_Root->setSliderSnapsToMousePosition(false);
    s_Root->addMouseListener(this, true);
    resizer->setMouseClickGrabsKeyboardFocus(false);
    timeline->addChangeListener(this);
    viewport->addChangeListener(this);

    midiOutDeviceBox->setMouseClickGrabsKeyboardFocus(false);
    midiOutDeviceBox->addItem(juce::String("--"), 1);
    for (int i = 0; i < ownerFilter->devices.size(); i++)
    {
        midiOutDeviceBox->addItem(ownerFilter->devices[i].name, i + 2);
    }
    midiOutDeviceBox->setSelectedId(1);

    loopinfoLabel2->setText("Stopped", juce::dontSendNotification);

    resizeLimits.setSizeLimits(385, 248, 1600, 900);
    ownerFilter->addChangeListener(this);
    ownerFilter->info->addChangeListener(this);
    pianoRoll = (PianoRoll*) viewport->getViewedComponent();
    viewport->setPlayline(pianoRoll->getPlayline());
    pianoRoll->setSize(500, 1200);
    pianoRoll->setSequence(ownerFilter->getActiveLoop());
    pianoRoll->addChangeListener(this);
    keyboard = (juce::MidiKeyboardComponent*) kbport->getViewedComponent();
    keyboard->setScrollButtonsVisible(false);
    keyboard->setBounds(0, 0, 25, pianoRoll->getHeight());
    keyboard->setKeyWidth((float) pianoRoll->getHeight() / 74.75f);
    keyboard->setOctaveForMiddleC(ownerFilter->bottomOctave + 5);
    lastActiveLoop = ownerFilter->getCurrentProgram();
    //viewport->setViewPosition(0,pianoRoll->getHeight()/2-viewport->getHeight()/2);
    //ownerFilter->setPRSetting("x",viewport->getViewPositionX());
    //ownerFilter->setPRSetting("y",viewport->getViewPositionY());
    //ownerFilter->setPRSetting("width",pianoRoll->getWidth());
    //ownerFilter->setPRSetting("height",pianoRoll->getHeight());
    timeline->setPianoRoll(pianoRoll);
    noSnap = true;
    nameLabel->setListener(this);
    keySelector->setKeyWidth(22.f);
    keySelector->setAvailableRange(0, 11);
    keySelector->addChangeListener(this);
    ownerFilter->keySelectorState.addListener(this);
    startTimer(75);

#if 0
    //[/UserPreSize]

    setSize (800, 487);


    //[Constructor] You can add your own custom stuff here..
#endif
    setSize(ownerFilter->lastUIWidth, ownerFilter->lastUIHeight);
    updateParametersFromFilter();
    //[/Constructor]
}

PizLooperEditor::~PizLooperEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    DBG("start ~PizLooperEditor()");
    stopTimer();
    getFilter()->dispatchPendingMessages();
    getFilter()->info->dispatchPendingMessages();
    getFilter()->removeChangeListener(this);
    getFilter()->info->removeChangeListener(this);
    getFilter()->keySelectorState.removeListener(this);
    //[/Destructor_pre]

    label                = nullptr;
    timeline             = nullptr;
    textButton1          = nullptr;
    textButton2          = nullptr;
    textButton3          = nullptr;
    textButton4          = nullptr;
    textButton5          = nullptr;
    textButton6          = nullptr;
    textButton7          = nullptr;
    textButton8          = nullptr;
    textButton9          = nullptr;
    textButton10         = nullptr;
    textButton11         = nullptr;
    textButton12         = nullptr;
    textButton13         = nullptr;
    textButton14         = nullptr;
    textButton15         = nullptr;
    textButton16         = nullptr;
    b_Play               = nullptr;
    b_Record             = nullptr;
    b_Overdub            = nullptr;
    b_Thru               = nullptr;
    b_Clear              = nullptr;
    stepsizeBox          = nullptr;
    s_Transpose          = nullptr;
    s_Octave             = nullptr;
    s_Velocity           = nullptr;
    label3               = nullptr;
    label4               = nullptr;
    label5               = nullptr;
    s_Start              = nullptr;
    label6               = nullptr;
    s_End                = nullptr;
    label7               = nullptr;
    s_Stretch            = nullptr;
    label8               = nullptr;
    loopmodeBox          = nullptr;
    notetriggerBox       = nullptr;
    syncmodeBox          = nullptr;
    s_Root               = nullptr;
    label9               = nullptr;
    s_Low                = nullptr;
    label10              = nullptr;
    s_High               = nullptr;
    label11              = nullptr;
    s_TrigChan           = nullptr;
    label12              = nullptr;
    b_Reload             = nullptr;
    quantizeBox          = nullptr;
    label21              = nullptr;
    s_Shift              = nullptr;
    label2               = nullptr;
    label23              = nullptr;
    nameLabel            = nullptr;
    b_Save               = nullptr;
    label22              = nullptr;
    label18              = nullptr;
    loopinfoLabel        = nullptr;
    loopinfoLabel2       = nullptr;
    label17              = nullptr;
    s_Channel            = nullptr;
    label19              = nullptr;
    label20              = nullptr;
    s_FixedLength        = nullptr;
    b_Filt               = nullptr;
    viewport             = nullptr;
    resizer              = nullptr;
    b_NoteToggle         = nullptr;
    s_PlayGroup          = nullptr;
    label13              = nullptr;
    s_MuteGroup          = nullptr;
    label14              = nullptr;
    b_Snap               = nullptr;
    quantizeBox2         = nullptr;
    b_ForceToKey         = nullptr;
    keySelector          = nullptr;
    b_ShiftUp            = nullptr;
    b_ShiftDown          = nullptr;
    b_SingleLoop         = nullptr;
    s_MasterVelocity     = nullptr;
    label15              = nullptr;
    aboutButton          = nullptr;
    b_Triplet            = nullptr;
    b_Dotted             = nullptr;
    b_ZoomOut            = nullptr;
    b_ZoomIn             = nullptr;
    numerator            = nullptr;
    denominator          = nullptr;
    b_UseScaleChannel    = nullptr;
    s_ScaleChannel       = nullptr;
    label25              = nullptr;
    s_MasterTranspose    = nullptr;
    label26              = nullptr;
    b_WaitForBar         = nullptr;
    midiOutDeviceBox     = nullptr;
    label27              = nullptr;
    b_UseTrChannel       = nullptr;
    b_ImmediateTranspose = nullptr;
    s_NumLoops           = nullptr;
    s_NextSlot           = nullptr;
    label16              = nullptr;
    forceModeBox         = nullptr;
    kbport               = nullptr;
    b_RemoveBar          = nullptr;
    b_AddBar             = nullptr;
    LengthLabel          = nullptr;
    textButton17         = nullptr;
    textButton18         = nullptr;
    textButton19         = nullptr;
    textButton20         = nullptr;
    textButton21         = nullptr;
    textButton22         = nullptr;
    textButton23         = nullptr;
    textButton24         = nullptr;
    textButton25         = nullptr;
    textButton26         = nullptr;
    textButton27         = nullptr;
    textButton28         = nullptr;
    textButton29         = nullptr;
    textButton30         = nullptr;
    textButton31         = nullptr;
    textButton32         = nullptr;
    textButton33         = nullptr;
    textButton34         = nullptr;
    textButton35         = nullptr;
    textButton36         = nullptr;
    textButton37         = nullptr;
    textButton38         = nullptr;
    textButton39         = nullptr;
    textButton40         = nullptr;
    textButton41         = nullptr;
    textButton42         = nullptr;
    textButton43         = nullptr;
    textButton44         = nullptr;
    textButton45         = nullptr;
    textButton46         = nullptr;
    textButton47         = nullptr;
    textButton48         = nullptr;
    textButton49         = nullptr;
    textButton50         = nullptr;
    textButton51         = nullptr;
    textButton52         = nullptr;
    textButton53         = nullptr;
    textButton54         = nullptr;
    textButton55         = nullptr;
    textButton56         = nullptr;
    textButton57         = nullptr;
    textButton58         = nullptr;
    textButton59         = nullptr;
    textButton60         = nullptr;
    textButton61         = nullptr;
    textButton62         = nullptr;
    textButton63         = nullptr;
    textButton64         = nullptr;
    textButton65         = nullptr;
    textButton66         = nullptr;
    textButton67         = nullptr;
    textButton68         = nullptr;
    textButton69         = nullptr;
    textButton70         = nullptr;
    textButton71         = nullptr;
    textButton72         = nullptr;
    textButton73         = nullptr;
    textButton74         = nullptr;
    textButton75         = nullptr;
    textButton76         = nullptr;
    textButton77         = nullptr;
    textButton78         = nullptr;
    textButton79         = nullptr;
    textButton80         = nullptr;
    textButton81         = nullptr;
    textButton82         = nullptr;
    textButton83         = nullptr;
    textButton84         = nullptr;
    textButton85         = nullptr;
    textButton86         = nullptr;
    textButton87         = nullptr;
    textButton88         = nullptr;
    textButton89         = nullptr;
    textButton90         = nullptr;
    textButton91         = nullptr;
    textButton92         = nullptr;
    textButton93         = nullptr;
    textButton94         = nullptr;
    textButton95         = nullptr;
    textButton96         = nullptr;
    textButton97         = nullptr;
    textButton98         = nullptr;
    textButton99         = nullptr;
    textButton100        = nullptr;
    textButton101        = nullptr;
    textButton102        = nullptr;
    textButton103        = nullptr;
    textButton104        = nullptr;
    textButton105        = nullptr;
    textButton106        = nullptr;
    textButton107        = nullptr;
    textButton108        = nullptr;
    textButton109        = nullptr;
    textButton110        = nullptr;
    textButton111        = nullptr;
    textButton112        = nullptr;
    textButton113        = nullptr;
    textButton114        = nullptr;
    textButton115        = nullptr;
    textButton116        = nullptr;
    textButton117        = nullptr;
    textButton118        = nullptr;
    textButton119        = nullptr;
    textButton120        = nullptr;
    textButton121        = nullptr;
    textButton122        = nullptr;
    textButton123        = nullptr;
    textButton124        = nullptr;
    textButton125        = nullptr;
    textButton126        = nullptr;
    textButton127        = nullptr;
    textButton128        = nullptr;
    b_Transpose10        = nullptr;
    b_KeepLength         = nullptr;
    s_RecCC              = nullptr;
    s_PlayCC             = nullptr;
    s_VelocitySens       = nullptr;
    label24              = nullptr;
    b_Monitor            = nullptr;
    s_TransposeChannel   = nullptr;
    label28              = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    DBG("end ~PizLooperEditor()");
    //[/Destructor]
}

//==============================================================================
void PizLooperEditor::paint(juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(juce::Colour(0xff202029));

    {
        int x = 1, y = 393, width = 303, height = 78;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour(0xff404049);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               176.0f - 1.0f + x,
                                               352.0f - 393.0f + y,
                                               fillColour2,
                                               176.0f - 1.0f + x,
                                               443.0f - 393.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
    }

    {
        int x = 305, y = 393, width = 81, height = 78;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour(0xff162938);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               190.0f - 305.0f + x,
                                               369.0f - 393.0f + y,
                                               fillColour2,
                                               190.0f - 305.0f + x,
                                               419.0f - 393.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
    }

    {
        float x = 390.0f, y = 62.0f, width = static_cast<float>(getWidth() - 391), height = 40.0f;
        juce::Colour fillColour = juce::Colour(0xffa0a0a0);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
    }

    {
        int x = 152, y = -1, width = getWidth() - 151, height = 56;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour(0xff404049);
        juce::Colour strokeColour = juce::Colour(0xff46495f);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               160.0f - 152.0f + x,
                                               static_cast<float>(-30) - static_cast<float>(-1) + y,
                                               fillColour2,
                                               161.0f - 152.0f + x,
                                               43.0f - static_cast<float>(-1) + y,
                                               false));
        g.fillRect(x, y, width, height);
        g.setColour(strokeColour);
        g.drawRect(x, y, width, height, 1);
    }

    {
        int x = 0, y = 0, width = 152, height = 55;
        juce::Colour fillColour = juce::Colour(0xff202029);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRect(x, y, width, height);
    }

    {
        float x = 6.0f, y = 317.0f, width = 136.0f, height = 90.0f;
        juce::Colour fillColour = juce::Colour(0xff404049);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 14.500f);
    }

    {
        int x = 1, y = 246, width = 385, height = 147;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour(0xff404049);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               159.0f - 1.0f + x,
                                               215.0f - 246.0f + y,
                                               fillColour2,
                                               159.0f - 1.0f + x,
                                               307.0f - 246.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
    }

    {
        float x = 6.0f, y = 159.0f, width = 136.0f, height = 104.0f;
        juce::Colour fillColour = juce::Colour(0xff404049);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 14.500f);
    }

    {
        int x = 1, y = 109, width = 385, height = 140;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour(0xff404049);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               100.0f - 1.0f + x,
                                               89.0f - 109.0f + y,
                                               fillColour2,
                                               100.0f - 1.0f + x,
                                               165.0f - 109.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
    }

    {
        float x = 6.0f, y = static_cast<float>(-20), width = 139.0f, height = 67.0f;
        juce::Colour fillColour1 = juce::Colour(0xff002700), fillColour2 = juce::Colour(0xba267387);
        juce::Colour strokeColour = juce::Colour(0xffcbcbcb);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               95.0f - 6.0f + x,
                                               static_cast<float>(-25) - static_cast<float>(-20) + y,
                                               fillColour2,
                                               95.0f - 6.0f + x,
                                               107.0f - static_cast<float>(-20) + y,
                                               false));
        g.fillRoundedRectangle(x, y, width, height, 14.500f);
        g.setColour(strokeColour);
        g.drawRoundedRectangle(x, y, width, height, 14.500f, 1.000f);
    }

    {
        int x = 12, y = 12, width = 41, height = 25;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(juce::Colours::black);
        g.drawImage(cachedImage_piznew40_png_1,
                    x,
                    y,
                    width,
                    height,
                    0,
                    0,
                    cachedImage_piznew40_png_1.getWidth(),
                    cachedImage_piznew40_png_1.getHeight());
    }

    {
        int x = 44, y = 23, width = 108, height = 20;
        juce::String text(TRANS("midiLooper"));
        juce::Colour fillColour = juce::Colour(0xffcbcbcb);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Bold"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 46, y = 8, width = 108, height = 20;
        juce::String text(TRANS("Insert Piz Here->"));
        juce::Colour fillColour = juce::Colour(0xffbfbfbf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(10.00f, juce::Font::plain).withTypefaceStyle("Bold"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 9, y = 202, width = 369, height = 42;
        juce::Colour fillColour1 = juce::Colour(0xff433b22), fillColour2 = juce::Colour(0xff8b7a47);
        juce::Colour strokeColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               100.0f - 9.0f + x,
                                               194.0f - 202.0f + y,
                                               fillColour2,
                                               100.0f - 9.0f + x,
                                               214.0f - 202.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
        g.setColour(strokeColour);
        g.drawRect(x, y, width, height, 1);
    }

    {
        int x = 1, y = 85, width = 385, height = 32;
        juce::Colour fillColour1 = juce::Colour(0xff979797), fillColour2 = juce::Colour(0xff6f737a);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               100.0f - 1.0f + x,
                                               85.0f - 85.0f + y,
                                               fillColour2,
                                               100.0f - 1.0f + x,
                                               98.0f - 85.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
    }

    {
        int x = 390, y = 85, width = getWidth() - 391, height = 20;
        juce::Colour fillColour = juce::Colour(0xff8c8c8c);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRect(x, y, width, height);
    }

    {
        int x = 215, y = 177, width = 24, height = 15;
        juce::String text(TRANS("->"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 153, y = 162, width = 24, height = 15;
        juce::String text(TRANS("PLAY"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(10.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 230, y = 162, width = 29, height = 15;
        juce::String text(TRANS("THEN"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(10.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        float x = 6.0f, y = 458.0f, width = 136.0f, height = 27.0f;
        juce::Colour fillColour = juce::Colour(0xff404049);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 14.500f);
    }

    {
        int x = 574, y = 67, width = 20, height = 12;
        juce::String text(TRANS("/"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        float x = 141.0f, y = 254.0f, width = 161.0f, height = 72.0f;
        juce::Colour fillColour = juce::Colour(0x86404049);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
    }

    {
        float x = 8.0f, y = 311.0f, width = 371.0f, height = 36.0f;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour(0xff434450);
        juce::Colour strokeColour = juce::Colour(0xff111111);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               256.0f - 8.0f + x,
                                               265.0f - 311.0f + y,
                                               fillColour2,
                                               256.0f - 8.0f + x,
                                               355.0f - 311.0f + y,
                                               false));
        g.fillRoundedRectangle(x, y, width, height, 10.000f);
        g.setColour(strokeColour);
        g.drawRoundedRectangle(x, y, width, height, 10.000f, 0.700f);
    }

    {
        int x = 723, y = 64, width = 51, height = 18;
        juce::Colour fillColour   = juce::Colour(0xffbababa);
        juce::Colour strokeColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillRect(x, y, width, height);
        g.setColour(strokeColour);
        g.drawRect(x, y, width, height, 1);
    }

    //[UserPaint] Add your own custom painting code here..
    if (getFilter()->getParameterForSlot(kUseTrChannel, getFilter()->getCurrentProgram()) >= 0.5f)
    {
        g.setColour(juce::Colour(0xff5b5b5b));
        g.strokePath(internalPath1, juce::PathStrokeType(5.0000f));
    }
    if (getFilter()->getParameterForSlot(kUseScaleChannel, getFilter()->getCurrentProgram()) >= 0.5f)
    {
        g.setColour(juce::Colour(0xff5b5b5b));
        g.strokePath(internalPath2, juce::PathStrokeType(5.0000f));
    }
    //[/UserPaint]
}

void PizLooperEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    timeline->setBounds(415, 85, getWidth() - 430, 20);
    viewport->setBounds(415, 105, getWidth() - 415, getHeight() - 105);
    resizer->setBounds(getWidth() - 16, getHeight() - 16, 16, 16);
    kbport->setBounds(390, 105, 25, getHeight() - 121);
    //[UserResized] Add your own custom resize handling here..
    internalPath1.clear();
    internalPath1.startNewSubPath(141.0f, 294.0f);
    internalPath1.lineTo(136.0f, 294.0f);

    internalPath2.clear();
    internalPath2.startNewSubPath(29.0f, 310.0f);
    internalPath2.lineTo(29.0f, 301.0f);

    pianoRoll->setVisible(getWidth() > 390);
    getFilter()->setSize(getWidth(), getHeight());
    //[/UserResized]
}

void PizLooperEditor::buttonClicked(juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    int index = getButtonIndex(buttonThatWasClicked);
    if (index > -1)
    {
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            getFilter()->toggleSlotPlaying(index);
        }
        else
        {
            getFilter()->setActiveSlot(index);
            buttonThatWasClicked->setToggleState(true, juce::dontSendNotification);
        }
    }
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == b_Overdub.get())
    {
        //[UserButtonCode_b_Overdub] -- add your button handler code here..
        if (getFilter()->getParamForActiveSlot(kRecMode) >= 0.5f)
        {
            getFilter()->setParameter(kRecMode, 0.0f);
        }
        else
        {
            if (juce::ModifierKeys::getCurrentModifiers().isAltDown())
            {
                getFilter()->setParameter(kRecMode, 1.0f);
            }
            else
            {
                getFilter()->setParameter(kRecMode, 0.7f);
            }
        }
        //[/UserButtonCode_b_Overdub]
    }
    else if (buttonThatWasClicked == b_Thru.get())
    {
        //[UserButtonCode_b_Thru] -- add your button handler code here..
        if (getFilter()->getParamForActiveSlot(kThru) >= 0.5f)
        {
            getFilter()->setParameter(kThru, 0.0f);
        }
        else
        {
            getFilter()->setParameter(kThru, 1.0f);
        }
        //[/UserButtonCode_b_Thru]
    }
    else if (buttonThatWasClicked == b_Clear.get())
    {
        //[UserButtonCode_b_Clear] -- add your button handler code here..
        getFilter()->setParameter(kFile, 0.0f);
        //[/UserButtonCode_b_Clear]
    }
    else if (buttonThatWasClicked == b_Reload.get())
    {
        //[UserButtonCode_b_Reload] -- add your button handler code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            getFilter()->setParameter(kFile, 1.0f);
        }

        else
        {
            juce::FileChooser myChooser("Load MIDI File...",
                                        juce::File(getFilter()->loopDir),
                                        "*.mid");

            if (myChooser.browseForFileToOpen())
            {
                juce::File midiFile(myChooser.getResult());
                getFilter()->loadMidiFile(midiFile);
            }
        }
        //[/UserButtonCode_b_Reload]
    }
    else if (buttonThatWasClicked == b_Save.get())
    {
        //[UserButtonCode_b_Save] -- add your button handler code here..
        if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
        {
            //save with filename incremented
            getFilter()->setParameter(kSave, 1.0f);
        }
        else if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            getFilter()->setParameter(kSave, 0.9f);
        }
        else
        {
            juce::FileChooser myChooser("Save MIDI File...",
                                        juce::File(getFilter()->loopDir + juce::File::getSeparatorString() + nameLabel->getText()),
                                        "*.mid");

            if (myChooser.browseForFileToSave(true))
            {
                juce::File midiFile(myChooser.getResult());
                if (! midiFile.hasFileExtension("mid"))
                {
                    midiFile = midiFile.withFileExtension("mid");
                }

                getFilter()->writeMidiFile(lastActiveLoop, midiFile);
            }
        }
        //[/UserButtonCode_b_Save]
    }
    else if (buttonThatWasClicked == b_Filt.get())
    {
        //[UserButtonCode_b_Filt] -- add your button handler code here..
        if (getFilter()->getParamForActiveSlot(kFiltChan) >= 0.5f)
        {
            getFilter()->notifyHostForActiveSlot(kFiltChan, 0.0f);
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kFiltChan, 1.0f);
        }
        //[/UserButtonCode_b_Filt]
    }
    else if (buttonThatWasClicked == b_NoteToggle.get())
    {
        //[UserButtonCode_b_NoteToggle] -- add your button handler code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kNoteToggle, i, b_NoteToggle->getToggleState() ? 0.f : 1.f);
            }
        }
        else
        {
            if (getFilter()->getParamForActiveSlot(kNoteToggle) >= 0.5f)
            {
                getFilter()->notifyHostForActiveSlot(kNoteToggle, 0.0f);
            }
            else
            {
                getFilter()->notifyHostForActiveSlot(kNoteToggle, 1.0f);
            }
        }
        //[/UserButtonCode_b_NoteToggle]
    }
    else if (buttonThatWasClicked == b_Snap.get())
    {
        //[UserButtonCode_b_Snap] -- add your button handler code here..
        b_Snap->setToggleState(! getFilter()->getPRSetting("snap"), juce::dontSendNotification);
        getFilter()->setPRSetting("snap", b_Snap->getToggleState());
        pianoRoll->repaintBG();
        //[/UserButtonCode_b_Snap]
    }
    else if (buttonThatWasClicked == b_ForceToKey.get())
    {
        //[UserButtonCode_b_ForceToKey] -- add your button handler code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kForceToKey, i, buttonThatWasClicked->getToggleState() ? 1.f : 0.f);
            }
        }
        else
        {
            if (getFilter()->getParamForActiveSlot(kForceToKey) >= 0.5f)
            {
                getFilter()->notifyHostForActiveSlot(kForceToKey, 0.0f);
            }
            else
            {
                getFilter()->notifyHostForActiveSlot(kForceToKey, 1.0f);
            }
        }
        //[/UserButtonCode_b_ForceToKey]
    }
    else if (buttonThatWasClicked == b_ShiftUp.get())
    {
        //[UserButtonCode_b_ShiftUp] -- add your button handler code here..
        bool lastNotes[12];
        for (int i = 0; i < 12; i++)
        {
            lastNotes[i] = getFilter()->keySelectorState.isNoteOn(1, i);
        }
        getFilter()->keySelectorState.reset();
        if (lastNotes[11])
        {
            getFilter()->keySelectorState.noteOn(1, 0, 1.f);
        }
        for (int n = 0; n < 11; n++)
        {
            if (lastNotes[n])
            {
                getFilter()->keySelectorState.noteOn(1, n + 1, 1.f);
            }
        }
        //if (ModifierKeys::getCurrentModifiers().isCommandDown())
        //{
        //	for (int i=0;i<numSlots;i++) {
        //		getFilter()->notifyHost(kNote0,i,lastNotes[11] ? 1.f : 0.f);
        //		for (int n=0;n<11;n++) {
        //			getFilter()->notifyHost(kNote0+n+1,i,lastNotes[i]?1.f:0.f);
        //		}
        //	}
        //}
        //else {
        getFilter()->notifyHostForActiveSlot(kNote0, lastNotes[11] ? 1.f : 0.f);
        for (int n = 0; n < 11; n++)
        {
            getFilter()->notifyHostForActiveSlot(kNote0 + n + 1, lastNotes[n] ? 1.f : 0.f);
        }
        //}
        //[/UserButtonCode_b_ShiftUp]
    }
    else if (buttonThatWasClicked == b_ShiftDown.get())
    {
        //[UserButtonCode_b_ShiftDown] -- add your button handler code here..
        bool lastNotes[12];
        for (int i = 0; i < 12; i++)
        {
            lastNotes[i] = getFilter()->keySelectorState.isNoteOn(1, i);
        }
        getFilter()->keySelectorState.reset();
        if (lastNotes[0])
        {
            getFilter()->keySelectorState.noteOn(1, 11, 1.f);
        }
        for (int n = 1; n < 12; n++)
        {
            if (lastNotes[n])
            {
                getFilter()->keySelectorState.noteOn(1, n - 1, 1.f);
            }
        }
        //if (ModifierKeys::getCurrentModifiers().isCommandDown())
        //{
        //	for (int i=0;i<numSlots;i++) {
        //		getFilter()->notifyHost(kNote11,i,lastNotes[0]?1.f:0.f);
        //		for (int n=0;n<11;n++)
        //			getFilter()->notifyHost(kNote0+n-1,i,lastNotes[n]?1.f:0.f);
        //	}
        //}
        //else {
        getFilter()->notifyHostForActiveSlot(kNote11, lastNotes[0] ? 1.f : 0.f);
        for (int n = 0; n < 11; n++)
        {
            getFilter()->notifyHostForActiveSlot(kNote0 + n - 1, lastNotes[n] ? 1.f : 0.f);
        }
        //}
        //[/UserButtonCode_b_ShiftDown]
    }
    else if (buttonThatWasClicked == b_SingleLoop.get())
    {
        //[UserButtonCode_b_SingleLoop] -- add your button handler code here..
        if (getFilter()->getParamForActiveSlot(kSingleLoop) >= 0.5f)
        {
            getFilter()->notifyHostForActiveSlot(kSingleLoop, 0.0f);
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kSingleLoop, 1.0f);
        }
        //[/UserButtonCode_b_SingleLoop]
    }
    else if (buttonThatWasClicked == aboutButton.get())
    {
        //[UserButtonCode_aboutButton] -- add your button handler code here..
        juce::URL("http://thepiz.org/plugins/?p=midiLooper").launchInDefaultBrowser();
        //[/UserButtonCode_aboutButton]
    }
    else if (buttonThatWasClicked == b_Triplet.get())
    {
        //[UserButtonCode_b_Triplet] -- add your button handler code here..
        buttonThatWasClicked->setToggleState(! getFilter()->getPRSetting("triplet"), juce::dontSendNotification);
        getFilter()->setPRSetting("triplet", buttonThatWasClicked->getToggleState());
        pianoRoll->repaintBG();
        //[/UserButtonCode_b_Triplet]
    }
    else if (buttonThatWasClicked == b_Dotted.get())
    {
        //[UserButtonCode_b_Dotted] -- add your button handler code here..
        buttonThatWasClicked->setToggleState(! getFilter()->getPRSetting("dotted"), juce::dontSendNotification);
        getFilter()->setPRSetting("dotted", buttonThatWasClicked->getToggleState());
        pianoRoll->repaintBG();
        //[/UserButtonCode_b_Dotted]
    }
    else if (buttonThatWasClicked == b_ZoomOut.get())
    {
        //[UserButtonCode_b_ZoomOut] -- add your button handler code here..
        double y = (double) viewport->getViewPositionY() / ((double) pianoRoll->getHeight() - viewport->getHeight());
        double x = (double) viewport->getViewPositionX() / ((double) pianoRoll->getWidth() - viewport->getWidth());
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            pianoRoll->setSize(pianoRoll->getWidth(), jmax(366, (int) (pianoRoll->getHeight() * 0.75)));
            keyboard->setSize(25, pianoRoll->getHeight());
            keyboard->setKeyWidth((float) pianoRoll->getHeight() / 74.75f);
            viewport->setViewPositionProportionately(x, y);
            getFilter()->setPRSetting("height", pianoRoll->getHeight(), false);
            getFilter()->setPRSetting("y", viewport->getViewPositionY(), false);
        }
        else
        {
            pianoRoll->setSize(jmax(100, (int) (pianoRoll->getWidth() * 0.75)), pianoRoll->getHeight());
            viewport->setViewPositionProportionately(x, y);
            getFilter()->setPRSetting("width", pianoRoll->getWidth(), false);
            getFilter()->setPRSetting("x", viewport->getViewPositionX(), false);
        }
        //[/UserButtonCode_b_ZoomOut]
    }
    else if (buttonThatWasClicked == b_ZoomIn.get())
    {
        //[UserButtonCode_b_ZoomIn] -- add your button handler code here..
        double y = (double) viewport->getViewPositionY() / ((double) pianoRoll->getHeight() - viewport->getHeight());
        double x = (double) viewport->getViewPositionX() / ((double) pianoRoll->getWidth() - viewport->getWidth());
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            pianoRoll->setSize(pianoRoll->getWidth(), jmin(25600, (int) (pianoRoll->getHeight() * 1.33333333333)));
            keyboard->setSize(25, pianoRoll->getHeight());
            keyboard->setKeyWidth((float) pianoRoll->getHeight() / 74.75f);
            viewport->setViewPositionProportionately(x, y);
            getFilter()->setPRSetting("height", pianoRoll->getHeight(), false);
            getFilter()->setPRSetting("y", viewport->getViewPositionY(), false);
        }
        else
        {
            pianoRoll->setSize((int) (pianoRoll->getWidth() * 1.33333333333), pianoRoll->getHeight());
            viewport->setViewPositionProportionately(x, y);
            getFilter()->setPRSetting("width", pianoRoll->getWidth(), false);
            getFilter()->setPRSetting("x", viewport->getViewPositionX(), false);
        }
        //[/UserButtonCode_b_ZoomIn]
    }
    else if (buttonThatWasClicked == b_UseScaleChannel.get())
    {
        //[UserButtonCode_b_UseScaleChannel] -- add your button handler code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kUseScaleChannel, i, buttonThatWasClicked->getToggleState() ? 1.f : 0.f);
            }
        }
        else
        {
            if (getFilter()->getParamForActiveSlot(kUseScaleChannel) >= 0.5f)
            {
                getFilter()->notifyHostForActiveSlot(kUseScaleChannel, 0.0f);
            }
            else
            {
                getFilter()->notifyHostForActiveSlot(kUseScaleChannel, 1.0f);
            }
        }
        repaint(24, 290, 22, 23);
        //[/UserButtonCode_b_UseScaleChannel]
    }
    else if (buttonThatWasClicked == b_WaitForBar.get())
    {
        //[UserButtonCode_b_WaitForBar] -- add your button handler code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kWaitForBar, i, buttonThatWasClicked->getToggleState() ? 1.f : 0.f);
            }
        }
        else
        {
            if (getFilter()->getParamForActiveSlot(kWaitForBar) >= 0.5f)
            {
                getFilter()->notifyHostForActiveSlot(kWaitForBar, 0.0f);
            }
            else
            {
                getFilter()->notifyHostForActiveSlot(kWaitForBar, 1.0f);
            }
        }
        //[/UserButtonCode_b_WaitForBar]
    }
    else if (buttonThatWasClicked == b_UseTrChannel.get())
    {
        //[UserButtonCode_b_UseTrChannel] -- add your button handler code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kUseTrChannel, i, buttonThatWasClicked->getToggleState() ? 1.f : 0.f);
            }
        }
        else
        {
            if (getFilter()->getParamForActiveSlot(kUseTrChannel) >= 0.5f)
            {
                getFilter()->notifyHostForActiveSlot(kUseTrChannel, 0.0f);
            }
            else
            {
                getFilter()->notifyHostForActiveSlot(kUseTrChannel, 1.0f);
            }
        }
        repaint(112, 284, 34, 20);
        //[/UserButtonCode_b_UseTrChannel]
    }
    else if (buttonThatWasClicked == b_ImmediateTranspose.get())
    {
        //[UserButtonCode_b_ImmediateTranspose] -- add your button handler code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kImmediateTranspose, i, buttonThatWasClicked->getToggleState() ? 1.f : 0.f);
            }
        }
        else
        {
            if (getFilter()->getParamForActiveSlot(kImmediateTranspose) >= 0.5f)
            {
                getFilter()->notifyHostForActiveSlot(kImmediateTranspose, 0.0f);
            }
            else
            {
                getFilter()->notifyHostForActiveSlot(kImmediateTranspose, 1.0f);
            }
        }
        //[/UserButtonCode_b_ImmediateTranspose]
    }
    else if (buttonThatWasClicked == b_RemoveBar.get())
    {
        //[UserButtonCode_b_RemoveBar] -- add your button handler code here..
        pianoRoll->removeBar();
        LengthLabel->setText(juce::String(pianoRoll->getDisplayLength()), juce::dontSendNotification);
        getFilter()->setPRSetting("bars", pianoRoll->getDisplayLength(), false);
        getFilter()->setPRSetting("width", pianoRoll->getWidth(), false);
        //[/UserButtonCode_b_RemoveBar]
    }
    else if (buttonThatWasClicked == b_AddBar.get())
    {
        //[UserButtonCode_b_AddBar] -- add your button handler code here..
        pianoRoll->addBar();
        LengthLabel->setText(juce::String(pianoRoll->getDisplayLength()), juce::dontSendNotification);
        getFilter()->setPRSetting("bars", pianoRoll->getDisplayLength(), false);
        getFilter()->setPRSetting("width", pianoRoll->getWidth(), false);
        //[/UserButtonCode_b_AddBar]
    }
    else if (buttonThatWasClicked == b_Transpose10.get())
    {
        //[UserButtonCode_b_Transpose10] -- add your button handler code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kTranspose10, i, b_Transpose10->getToggleState() ? 0.f : 1.f);
            }
        }
        else
        {
            if (getFilter()->getParamForActiveSlot(kTranspose10) >= 0.5f)
            {
                getFilter()->notifyHostForActiveSlot(kTranspose10, 0.0f);
            }
            else
            {
                getFilter()->notifyHostForActiveSlot(kTranspose10, 1.0f);
            }
        }
        //[/UserButtonCode_b_Transpose10]
    }
    else if (buttonThatWasClicked == b_KeepLength.get())
    {
        //[UserButtonCode_b_KeepLength] -- add your button handler code here..
        if (getFilter()->getParamForActiveSlot(kRecMode) >= 0.8f)
        {
            getFilter()->notifyHostForActiveSlot(kRecMode, 0.7f);
        }
        else if (getFilter()->getParamForActiveSlot(kRecMode) >= 0.5f)
        {
            getFilter()->notifyHostForActiveSlot(kRecMode, 1.0f);
        }
        //[/UserButtonCode_b_KeepLength]
    }
    else if (buttonThatWasClicked == b_Monitor.get())
    {
        //[UserButtonCode_b_Monitor] -- add your button handler code here..
        if (getFilter()->getParamForActiveSlot(kMonitor) >= 0.5f)
        {
            getFilter()->setParameter(kMonitor, 0.0f);
        }
        else
        {
            getFilter()->setParameter(kMonitor, 1.0f);
        }
        //[/UserButtonCode_b_Monitor]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void PizLooperEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    const juce::String selection = comboBoxThatHasChanged->getText();
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == stepsizeBox.get())
    {
        //[UserComboBoxCode_stepsizeBox] -- add your combo box handling code here..
        if (selection == "1 Bar")
        {
            getFilter()->notifyHostForActiveSlot(kRecStep, 0.f);
        }
        else if (selection == "3 Beats")
        {
            getFilter()->notifyHostForActiveSlot(kRecStep, 0.15f);
        }
        else if (selection == "2 Beats")
        {
            getFilter()->notifyHostForActiveSlot(kRecStep, 0.25f);
        }
        else if (selection == "1 Beat")
        {
            getFilter()->notifyHostForActiveSlot(kRecStep, 0.35f);
        }
        else if (selection == "8th Note")
        {
            getFilter()->notifyHostForActiveSlot(kRecStep, 0.45f);
        }
        else if (selection == "16th Note")
        {
            getFilter()->notifyHostForActiveSlot(kRecStep, 0.55f);
        }
        else if (selection == "1 Tick")
        {
            getFilter()->notifyHostForActiveSlot(kRecStep, 1.0f);
        }
        //[/UserComboBoxCode_stepsizeBox]
    }
    else if (comboBoxThatHasChanged == loopmodeBox.get())
    {
        //[UserComboBoxCode_loopmodeBox] -- add your combo box handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                if (selection == "Loop after rec")
                {
                    getFilter()->notifyHost(kTrigger, i, 0.f);
                }
                else if (selection == "Sync loop")
                {
                    getFilter()->notifyHost(kTrigger, i, 0.05f);
                }
                else if (selection == "Unsync 1-shot")
                {
                    getFilter()->notifyHost(kTrigger, i, 0.15f);
                }
                else if (selection == "Unsync loop")
                {
                    getFilter()->notifyHost(kTrigger, i, 0.25f);
                }
            }
        }
        else
        {
            if (selection == "Loop after rec")
            {
                getFilter()->notifyHostForActiveSlot(kTrigger, 0.f);
            }
            else if (selection == "Sync loop")
            {
                getFilter()->notifyHostForActiveSlot(kTrigger, 0.05f);
            }
            else if (selection == "Unsync 1-shot")
            {
                getFilter()->notifyHostForActiveSlot(kTrigger, 0.15f);
            }
            else if (selection == "Unsync loop")
            {
                getFilter()->notifyHostForActiveSlot(kTrigger, 0.25f);
            }
        }
        //[/UserComboBoxCode_loopmodeBox]
    }
    else if (comboBoxThatHasChanged == notetriggerBox.get())
    {
        //[UserComboBoxCode_notetriggerBox] -- add your combo box handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                if (selection == "Off")
                {
                    getFilter()->notifyHost(kNoteTrig, i, 0.f);
                }
                else if (selection == "Mono (Transpose)")
                {
                    getFilter()->notifyHost(kNoteTrig, i, 0.05f);
                }
                else if (selection == "Poly (Transpose)")
                {
                    getFilter()->notifyHost(kNoteTrig, i, 0.15f);
                }
                else if (selection == "Mono (Orig. Key)")
                {
                    getFilter()->notifyHost(kNoteTrig, i, 0.25f);
                }
            }
        }
        else
        {
            if (selection == "Off")
            {
                getFilter()->notifyHostForActiveSlot(kNoteTrig, 0.f);
            }
            else if (selection == "Mono (Transpose)")
            {
                getFilter()->notifyHostForActiveSlot(kNoteTrig, 0.05f);
            }
            else if (selection == "Poly (Transpose)")
            {
                getFilter()->notifyHostForActiveSlot(kNoteTrig, 0.15f);
            }
            else if (selection == "Mono (Orig. Key)")
            {
                getFilter()->notifyHostForActiveSlot(kNoteTrig, 0.25f);
            }
        }
        //[/UserComboBoxCode_notetriggerBox]
    }
    else if (comboBoxThatHasChanged == syncmodeBox.get())
    {
        //[UserComboBoxCode_syncmodeBox] -- add your combo box handling code here..
        if (selection == "PPQ (Host 0)")
        {
            getFilter()->notifyHostForActiveSlot(kSync, 0.f);
        }
        else if (selection == "PPQ (Recstart)")
        {
            getFilter()->notifyHostForActiveSlot(kSync, 0.25f);
        }
        else if (selection == "Sample")
        {
            getFilter()->notifyHostForActiveSlot(kSync, 1.f);
        }
        //[/UserComboBoxCode_syncmodeBox]
    }
    else if (comboBoxThatHasChanged == quantizeBox.get())
    {
        //[UserComboBoxCode_quantizeBox] -- add your combo box handling code here..
        if (selection == "Off")
        {
            getFilter()->notifyHostForActiveSlot(kQuantize, 0.f);
        }
        else if (selection == "8th")
        {
            getFilter()->notifyHostForActiveSlot(kQuantize, 0.25f);
        }
        else if (selection == "16th")
        {
            getFilter()->notifyHostForActiveSlot(kQuantize, 0.55f);
        }
        else if (selection == "32nd")
        {
            getFilter()->notifyHostForActiveSlot(kQuantize, 0.75f);
        }
        else if (selection == "64th")
        {
            getFilter()->notifyHostForActiveSlot(kQuantize, 1.f);
        }
        //[/UserComboBoxCode_quantizeBox]
    }
    else if (comboBoxThatHasChanged == quantizeBox2.get())
    {
        //[UserComboBoxCode_quantizeBox2] -- add your combo box handling code here..
        if (selection == "4th")
        {
            getFilter()->setPRSetting("stepsize", 0.f);
        }
        else if (selection == "8th")
        {
            getFilter()->setPRSetting("stepsize", 0.25f);
        }
        else if (selection == "16th")
        {
            getFilter()->setPRSetting("stepsize", 0.55f);
        }
        else if (selection == "32nd")
        {
            getFilter()->setPRSetting("stepsize", 0.75f);
        }
        else if (selection == "64th")
        {
            getFilter()->setPRSetting("stepsize", 1.f);
        }
        pianoRoll->repaintBG();
        //[/UserComboBoxCode_quantizeBox2]
    }
    else if (comboBoxThatHasChanged == midiOutDeviceBox.get())
    {
        //[UserComboBoxCode_midiOutDeviceBox] -- add your combo box handling code here..
        if (midiOutDeviceBox->getSelectedItemIndex() == 0)
        {
            getFilter()->setParameter(kMidiOutDevice, 0.0f);
            getFilter()->setActiveDevice(midiOutDeviceBox->getText());
        }
        else
        {
            getFilter()->setActiveDevice(midiOutDeviceBox->getText());
            getFilter()->setParameter(kMidiOutDevice, 1.0f);
            //getFilter()->setParameter(0,float(comboBox->getSelectedItemIndex()-1)/float(getFilter()->devices.size()-1)+0.00001f);
        }
        //[/UserComboBoxCode_midiOutDeviceBox]
    }
    else if (comboBoxThatHasChanged == forceModeBox.get())
    {
        //[UserComboBoxCode_forceModeBox] -- add your combo box handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                if (selection == "Nearest")
                {
                    getFilter()->notifyHost(kForceToScaleMode, i, 0.f);
                }
                else if (selection == "Up")
                {
                    getFilter()->notifyHost(kForceToScaleMode, i, 0.33f);
                }
                else if (selection == "Down")
                {
                    getFilter()->notifyHost(kForceToScaleMode, i, 0.67f);
                }
                else if (selection == "Block")
                {
                    getFilter()->notifyHost(kForceToScaleMode, i, 1.f);
                }
            }
        }
        else
        {
            if (selection == "Nearest")
            {
                getFilter()->notifyHostForActiveSlot(kForceToScaleMode, 0.f);
            }
            else if (selection == "Up")
            {
                getFilter()->notifyHostForActiveSlot(kForceToScaleMode, 0.33f);
            }
            else if (selection == "Down")
            {
                getFilter()->notifyHostForActiveSlot(kForceToScaleMode, 0.67f);
            }
            else if (selection == "Block")
            {
                getFilter()->notifyHostForActiveSlot(kForceToScaleMode, 1.f);
            }
        }
        //[/UserComboBoxCode_forceModeBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void PizLooperEditor::sliderValueChanged(juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    VSTSlider* slider = (VSTSlider*) sliderThatWasMoved;
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == s_Transpose.get())
    {
        //[UserSliderCode_s_Transpose] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kTranspose, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kTranspose, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_Transpose]
    }
    else if (sliderThatWasMoved == s_Octave.get())
    {
        //[UserSliderCode_s_Octave] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kOctave, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kOctave, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_Octave]
    }
    else if (sliderThatWasMoved == s_Velocity.get())
    {
        //[UserSliderCode_s_Velocity] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kVelocity, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kVelocity, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_Velocity]
    }
    else if (sliderThatWasMoved == s_Start.get())
    {
        //[UserSliderCode_s_Start] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kLoopStart, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kLoopStart, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_Start]
    }
    else if (sliderThatWasMoved == s_End.get())
    {
        //[UserSliderCode_s_End] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kLoopEnd, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kLoopEnd, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_End]
    }
    else if (sliderThatWasMoved == s_Stretch.get())
    {
        //[UserSliderCode_s_Stretch] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kStretch, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kStretch, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_Stretch]
    }
    else if (sliderThatWasMoved == s_Root.get())
    {
        //[UserSliderCode_s_Root] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kRoot, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kRoot, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_Root]
    }
    else if (sliderThatWasMoved == s_Low.get())
    {
        //[UserSliderCode_s_Low] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kNLow, i, slider->mapToVSTRange());
                if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
                {
                    getFilter()->notifyHost(kNHigh, i, slider->mapToVSTRange());
                }
            }
        }
        else if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
        {
            getFilter()->notifyHostForActiveSlot(kNLow, slider->mapToVSTRange());
            getFilter()->notifyHostForActiveSlot(kNHigh, slider->mapToVSTRange());
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kNLow, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_Low]
    }
    else if (sliderThatWasMoved == s_High.get())
    {
        //[UserSliderCode_s_High] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kNHigh, i, slider->mapToVSTRange());
                if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
                {
                    getFilter()->notifyHost(kNLow, i, slider->mapToVSTRange());
                }
            }
        }
        else if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
        {
            getFilter()->notifyHostForActiveSlot(kNHigh, slider->mapToVSTRange());
            getFilter()->notifyHostForActiveSlot(kNLow, slider->mapToVSTRange());
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kNHigh, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_High]
    }
    else if (sliderThatWasMoved == s_TrigChan.get())
    {
        //[UserSliderCode_s_TrigChan] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kTrigChan, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kTrigChan, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_TrigChan]
    }
    else if (sliderThatWasMoved == s_Shift.get())
    {
        //[UserSliderCode_s_Shift] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kShift, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kShift, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_Shift]
    }
    else if (sliderThatWasMoved == s_Channel.get())
    {
        //[UserSliderCode_s_Channel] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kChannel, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kChannel, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_Channel]
    }
    else if (sliderThatWasMoved == s_FixedLength.get())
    {
        //[UserSliderCode_s_FixedLength] -- add your slider handling code here..
        getFilter()->notifyHostForActiveSlot(kFixedLength, slider->mapToVSTRange());
        //[/UserSliderCode_s_FixedLength]
    }
    else if (sliderThatWasMoved == s_PlayGroup.get())
    {
        //[UserSliderCode_s_PlayGroup] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kPlayGroup, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kPlayGroup, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_PlayGroup]
    }
    else if (sliderThatWasMoved == s_MuteGroup.get())
    {
        //[UserSliderCode_s_MuteGroup] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kMuteGroup, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kMuteGroup, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_MuteGroup]
    }
    else if (sliderThatWasMoved == s_MasterVelocity.get())
    {
        //[UserSliderCode_s_MasterVelocity] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kMasterVelocity, slider->mapToVSTRange());
        //[/UserSliderCode_s_MasterVelocity]
    }
    else if (sliderThatWasMoved == s_ScaleChannel.get())
    {
        //[UserSliderCode_s_ScaleChannel] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
            {
                for (int i = 0; i < numSlots; i++)
                {
                    getFilter()->notifyHost(kScaleChannel, i, slider->mapToVSTRange());
                    getFilter()->notifyHost(kTransposeChannel, i, slider->mapToVSTRange());
                }
            }
            else
            {
                for (int i = 0; i < numSlots; i++)
                {
                    getFilter()->notifyHost(kScaleChannel, i, slider->mapToVSTRange());
                }
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kScaleChannel, slider->mapToVSTRange());
            if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
            {
                getFilter()->notifyHostForActiveSlot(kTransposeChannel, slider->mapToVSTRange());
            }
        }
        //[/UserSliderCode_s_ScaleChannel]
    }
    else if (sliderThatWasMoved == s_MasterTranspose.get())
    {
        //[UserSliderCode_s_MasterTranspose] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kMasterTranspose, slider->mapToVSTRange());
        //[/UserSliderCode_s_MasterTranspose]
    }
    else if (sliderThatWasMoved == s_NumLoops.get())
    {
        //[UserSliderCode_s_NumLoops] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kNumLoops, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kNumLoops, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_NumLoops]
    }
    else if (sliderThatWasMoved == s_NextSlot.get())
    {
        //[UserSliderCode_s_NextSlot] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kNextSlot, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kNextSlot, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_NextSlot]
    }
    else if (sliderThatWasMoved == s_RecCC.get())
    {
        //[UserSliderCode_s_RecCC] -- add your slider handling code here..
        getFilter()->notifyHostForActiveSlot(kRecCC, slider->mapToVSTRange());
        //[/UserSliderCode_s_RecCC]
    }
    else if (sliderThatWasMoved == s_PlayCC.get())
    {
        //[UserSliderCode_s_PlayCC] -- add your slider handling code here..
        getFilter()->notifyHostForActiveSlot(kPlayCC, slider->mapToVSTRange());
        //[/UserSliderCode_s_PlayCC]
    }
    else if (sliderThatWasMoved == s_VelocitySens.get())
    {
        //[UserSliderCode_s_VelocitySens] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kVeloSens, i, slider->mapToVSTRange());
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kVeloSens, slider->mapToVSTRange());
        }
        //[/UserSliderCode_s_VelocitySens]
    }
    else if (sliderThatWasMoved == s_TransposeChannel.get())
    {
        //[UserSliderCode_s_TransposeChannel] -- add your slider handling code here..
        if (juce::ModifierKeys::getCurrentModifiers().isCommandDown())
        {
            if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
            {
                for (int i = 0; i < numSlots; i++)
                {
                    getFilter()->notifyHost(kTransposeChannel, i, slider->mapToVSTRange());
                    getFilter()->notifyHost(kScaleChannel, i, slider->mapToVSTRange());
                }
            }
            else
            {
                for (int i = 0; i < numSlots; i++)
                {
                    getFilter()->notifyHost(kTransposeChannel, i, slider->mapToVSTRange());
                }
            }
        }
        else
        {
            getFilter()->notifyHostForActiveSlot(kTransposeChannel, slider->mapToVSTRange());
            if (juce::ModifierKeys::getCurrentModifiers().isShiftDown())
            {
                getFilter()->notifyHostForActiveSlot(kScaleChannel, slider->mapToVSTRange());
            }
        }
        //[/UserSliderCode_s_TransposeChannel]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void PizLooperEditor::labelTextChanged(juce::Label* labelThatHasChanged)
{
    //[UserlabelTextChanged_Pre]
    //[/UserlabelTextChanged_Pre]

    if (labelThatHasChanged == nameLabel.get())
    {
        //[UserLabelCode_nameLabel] -- add your label text handling code here..
        getFilter()->changeProgramName(getFilter()->getCurrentProgram(), nameLabel->getText());
        //[/UserLabelCode_nameLabel]
    }
    else if (labelThatHasChanged == numerator.get())
    {
        //[UserLabelCode_numerator] -- add your label text handling code here..
        int x = numerator->getText().getIntValue();
        if (x > 0)
        {
            pianoRoll->setTimeSig(x, pianoRoll->timeSigD);
            getFilter()->setTimeSig(lastActiveLoop, x, pianoRoll->timeSigD);
            pianoRoll->repaintBG();
        }
        else
        {
            numerator->setText(juce::String(getFilter()->getNumerator(lastActiveLoop)), juce::dontSendNotification);
        }
        //[/UserLabelCode_numerator]
    }
    else if (labelThatHasChanged == denominator.get())
    {
        //[UserLabelCode_denominator] -- add your label text handling code here..
        int x = denominator->getText().getIntValue();
        if (x > 0 && (x & (x - 1)) == 0)
        {
            pianoRoll->setTimeSig(pianoRoll->timeSigN, x);
            getFilter()->setTimeSig(lastActiveLoop, pianoRoll->timeSigN, x);
            pianoRoll->repaintBG();
        }
        else
        {
            denominator->setText(juce::String(getFilter()->getDenominator(lastActiveLoop)), juce::dontSendNotification);
        }
        //[/UserLabelCode_denominator]
    }
    else if (labelThatHasChanged == LengthLabel.get())
    {
        //[UserLabelCode_LengthLabel] -- add your label text handling code here..
        pianoRoll->setDisplayLength(LengthLabel->getText().getIntValue());
        getFilter()->setPRSetting("bars", pianoRoll->getDisplayLength(), false);
        getFilter()->setPRSetting("width", pianoRoll->getWidth(), false);
        //[/UserLabelCode_LengthLabel]
    }

    //[UserlabelTextChanged_Post]
    //[/UserlabelTextChanged_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//==============================================================================
void PizLooperEditor::buttonStateChanged(juce::Button* button)
{
}

void PizLooperEditor::mouseDown(const juce::MouseEvent& e)
{
    int index = getButtonIndex(e.eventComponent);
    if (e.mods.isPopupMenu() && index != -1)
    {
        for (int i = 0; i < numSlots; i++)
        {
            getFilter()->notifyHost(kPlay, i, index == i ? 1.f : 0.f);
        }
        ((juce::Button*) e.eventComponent)->setToggleState(true, juce::dontSendNotification);
    }
}

void PizLooperEditor::mouseDrag(const juce::MouseEvent& e)
{
}

void PizLooperEditor::mouseUp(const juce::MouseEvent& e)
{
    Component* p = e.eventComponent->getParentComponent();
    if (e.eventComponent == b_Play.get())
    {
        if (e.mods.isPopupMenu())
        {
            s_PlayCC->setVisible(! s_PlayCC->isVisible());
        }
        else if (e.mods.isMiddleButtonDown())
        {
            getFilter()->setParameter(kPlayCC, 0.f);
        }
        else if (e.mods.isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kPlay, i, b_Play->getToggleState() ? 0.f : 1.f);
            }
        }
        else
        {
            getFilter()->toggleSlotPlaying(getFilter()->getCurrentProgram());
        }
    }
    else if (e.eventComponent == b_Record.get())
    {
        if (e.mods.isPopupMenu())
        {
            s_RecCC->setVisible(! s_RecCC->isVisible());
        }
        else if (e.mods.isMiddleButtonDown())
        {
            getFilter()->setParameter(kRecCC, 0.f);
        }
        else if (e.mods.isCommandDown())
        {
            for (int i = 0; i < numSlots; i++)
            {
                getFilter()->notifyHost(kRecord, i, b_Record->getToggleState() ? 0.f : 1.f);
            }
        }
        else
        {
            if (getFilter()->getParamForActiveSlot(kRecord) >= 0.5)
            {
                getFilter()->notifyHostForActiveSlot(kRecord, 0.0);
            }
            else
            {
                getFilter()->notifyHostForActiveSlot(kRecord, 1.0);
            }
        }
    }
    else if (p == s_Root.get() || p == s_High.get() || p == s_Low.get())
    {
        auto* slider = (juce::Slider*) p;
        if (e.mods.isMiddleButtonDown())
        {
            //middle-click midi learn
            slider->setValue(-1, juce::sendNotification);
        }
        else if (e.mods.isPopupMenu())
        {
            if (slider->isDoubleClickReturnEnabled())
            {
                double value = slider->getDoubleClickReturnValue();
                slider->setValue(value, juce::sendNotification);
            }
        }
    }
    else if (p == s_PlayCC.get() || p == s_RecCC.get())
    {
        auto* slider = (juce::Slider*) p;
        if (e.mods.isMiddleButtonDown())
        {
            //middle-click midi learn
            slider->setValue(-2, juce::sendNotification);
        }
        else if (e.mods.isPopupMenu())
        {
            if (slider->isDoubleClickReturnEnabled())
            {
                double value = slider->getDoubleClickReturnValue();
                slider->setValue(value, juce::sendNotification);
            }
        }
    }
    else if (p == s_Stretch.get() || p == s_Transpose.get() || p == s_Octave.get() || p == s_Start.get() || p == s_End.get()
             || p == s_Shift.get() || p == s_Velocity.get() || p == s_VelocitySens.get() || p == s_MasterVelocity.get() || p == s_MasterTranspose.get()
             || p == s_PlayGroup.get() || p == s_MuteGroup.get() || p == s_Channel.get() || p == s_NumLoops.get() || p == s_NextSlot.get())
    {
        if (e.mods.isPopupMenu())
        {
            auto* slider = (juce::Slider*) p;
            if (slider->isDoubleClickReturnEnabled())
            {
                double value = slider->getDoubleClickReturnValue();
                slider->setValue(value, juce::sendNotification);
            }
        }
    }
}

void PizLooperEditor::filesDropped(const juce::StringArray& filenames, int mouseX, int mouseY)
{
    if (juce::File(filenames[0]).hasFileExtension("mid"))
    {
        getFilter()->loadMidiFile(juce::File(filenames[0]));
    }
}

bool PizLooperEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    juce::File file = juce::File(files[0]);
    if (file.hasFileExtension("mid"))
    {
        return true;
    }
    if (file.getFileName() == "midiLooperKey.txt")
    {
        return true;
    }
    return false;
}

void PizLooperEditor::timerCallback()
{
    pianoRoll->setPlayTime(960.0 * getFilter()->getPlayPosition(pianoRoll->playing, pianoRoll->recording));
}

void PizLooperEditor::handleNoteOn(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    if (source == &(getFilter()->keySelectorState))
    {
        getFilter()->notifyHostForActiveSlot(kNote0 + midiNoteNumber, 1.f);
        keySelector->repaint();
    }
}

void PizLooperEditor::handleNoteOff(juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    if (source == &(getFilter()->keySelectorState))
    {
        getFilter()->notifyHostForActiveSlot(kNote0 + midiNoteNumber, 0.f);
        keySelector->repaint();
    }
}

//==============================================================================
void PizLooperEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == getFilter() || getFilter()->newLoop)
    {
        updateParametersFromFilter();
    }
    else if (source == getFilter()->info)
    {
        loopinfoLabel2->setText(getFilter()->info->s, juce::sendNotification);
    }
    else if (source == keySelector.get())
    {
        for (int note = 0; note < 12; note++)
        {
            if (getFilter()->keySelectorState.isNoteOn(1, note) != getFilter()->getParamForActiveSlot(kNote0 + note) >= 0.5f)
            {
                getFilter()->notifyHostForActiveSlot(kNote0 + note, getFilter()->keySelectorState.isNoteOn(keySelector->getMidiChannel(), note) ? 1.f : 0.f);
            }
        }
    }
    else if (source == timeline.get() || source == pianoRoll)
    {
        getFilter()->setLoopLength(lastActiveLoop, timeline->getLength());
        getFilter()->setLoopStart(lastActiveLoop, timeline->getStart());
        getFilter()->updateLoopInfo();
        if (getFilter()->currentNumEvents == 0)
        {
            loopinfoLabel->setText("No Loop", juce::dontSendNotification);
            getButtonForSlot(lastActiveLoop)->setColour(juce::TextButton::textColourOffId, juce::Colour(0xff979797));
            getButtonForSlot(lastActiveLoop)->setColour(juce::TextButton::textColourOnId, juce::Colour(0xff555555));
        }
        else
        {
            juce::String loopinfo = "Loop length: ";
            if (getFilter()->currentLength == 1.0)
            {
                loopinfo << "1 Beat (";
            }
            else
            {
                loopinfo << getFilter()->currentLength << " Beats (";
            }
            loopinfo << getFilter()->currentNumEvents << " Events)";
            loopinfoLabel->setText(loopinfo, juce::dontSendNotification);
            getButtonForSlot(lastActiveLoop)->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
            getButtonForSlot(lastActiveLoop)->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
        }
    }
    else if (source == viewport.get())
    {
        getFilter()->setPRSetting("x", viewport->getViewPositionX(), false);
        getFilter()->setPRSetting("y", viewport->getViewPositionY(), false);
    }
}

//==============================================================================
void PizLooperEditor::updateControls(int param, float value, bool forCurProgram)
{
    if (! forCurProgram)
    {
        if (param == kPlay)
        {
            updateSlotButtons();
        }
        return;
    }
    switch (param)
    {
        case kThru:
            b_Thru->setToggleState(value >= 0.5f, juce::dontSendNotification);
            b_Monitor->setEnabled(b_Thru->getToggleState());
            break;
        case kMonitor:
            b_Monitor->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kSync:
            syncmodeBox->setText(getFilter()->getCurrentSlotParameterText(kSync), juce::dontSendNotification);
            break;
        case kRecStep:
            stepsizeBox->setText(getFilter()->getCurrentSlotParameterText(kRecStep), juce::dontSendNotification);
            break;
        case kQuantize:
            quantizeBox->setText(getFilter()->getCurrentSlotParameterText(kQuantize), juce::dontSendNotification);
            break;
        case kFixedLength:
            s_FixedLength->setVSTSlider(value);
            break;
        case kRecMode:
            if (value >= 0.8f)
            {
                b_Overdub->setColour(juce::TextButton::buttonOnColourId, getLookAndFeel().findColour(juce::TextButton::buttonOnColourId));
                b_KeepLength->setToggleState(false, juce::dontSendNotification);
            }
            else
            {
                b_Overdub->setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
                b_KeepLength->setToggleState(true, juce::dontSendNotification);
            }
            b_Overdub->setToggleState(value >= 0.5f, juce::dontSendNotification);
            b_KeepLength->setVisible(value >= 0.5f);
            break;
        case kSingleLoop:
            b_SingleLoop->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kMasterVelocity:
            s_MasterVelocity->setVSTSlider(value);
            break;
        case kMasterTranspose:
            s_MasterTranspose->setVSTSlider(value);
            break;
        case kImmediateTranspose:
            b_ImmediateTranspose->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kRecCC:
            s_RecCC->setVSTSlider(value);
            break;
        case kPlayCC:
            s_PlayCC->setVSTSlider(value);
            break;

        case kRecord:
            b_Record->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kPlay:
            b_Play->setToggleState(value >= 0.5f, juce::dontSendNotification);
            b_Play->setButtonText(value >= 0.5f ? "STOP" : "PLAY");
            updateSlotButtons();
            break;
        case kTranspose:
            s_Transpose->setIndex(lastActiveLoop * numParamsPerSlot + kTranspose);
            s_Transpose->setVSTSlider(value);
            break;
        case kOctave:
            s_Octave->setIndex(lastActiveLoop * numParamsPerSlot + kOctave);
            s_Octave->setVSTSlider(value);
            break;
        case kVelocity:
            s_Velocity->setIndex(lastActiveLoop * numParamsPerSlot + kVelocity);
            s_Velocity->setVSTSlider(value);
            break;
        case kVeloSens:
            s_VelocitySens->setIndex(lastActiveLoop * numParamsPerSlot + kVeloSens);
            s_VelocitySens->setVSTSlider(value);
            break;
        case kShift:
            s_Shift->setIndex(lastActiveLoop * numParamsPerSlot + kShift);
            s_Shift->setVSTSlider(value);
            break;
        case kLoopStart:
            s_Start->setIndex(lastActiveLoop * numParamsPerSlot + kLoopStart);
            s_Start->setVSTSlider(value);
            break;
        case kLoopEnd:
            s_End->setIndex(lastActiveLoop * numParamsPerSlot + kLoopEnd);
            s_End->setVSTSlider(value);
            break;
        case kStretch:
            s_Stretch->setIndex(lastActiveLoop * numParamsPerSlot + kStretch);
            s_Stretch->setVSTSlider(value);
            break;
        case kTrigger:
            loopmodeBox->setText(getFilter()->getCurrentSlotParameterText(kTrigger), juce::dontSendNotification);
            break;
        case kNoteTrig:
            notetriggerBox->setText(getFilter()->getCurrentSlotParameterText(kNoteTrig), juce::dontSendNotification);
            break;
        case kRoot:
            s_Root->setIndex(lastActiveLoop * numParamsPerSlot + kRoot);
            s_Root->setVSTSlider(value);
            break;
        case kNLow:
            s_Low->setIndex(lastActiveLoop * numParamsPerSlot + kNLow);
            s_Low->setVSTSlider(value);
            break;
        case kNHigh:
            s_High->setIndex(lastActiveLoop * numParamsPerSlot + kNHigh);
            s_High->setVSTSlider(value);
            break;
        case kChannel:
            s_Channel->setIndex(lastActiveLoop * numParamsPerSlot + kChannel);
            s_Channel->setVSTSlider(value);
            pianoRoll->defaultChannel = jmax(0, roundToInt(value * 16.f) - 1);
            keyboard->setMidiChannel(jmax(1, roundToInt(value * 16.f)));
            break;
        case kTrigChan:
            s_TrigChan->setIndex(lastActiveLoop * numParamsPerSlot + kTrigChan);
            s_TrigChan->setVSTSlider(value);
            break;
        case kFiltChan:
            if (value < 0.5f)
            {
                b_Filt->setButtonText("Transform");
            }
            else
            {
                b_Filt->setButtonText("Filter");
            }
            break;
        case kWaitForBar:
            b_WaitForBar->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kNumLoops:
            s_NumLoops->setIndex(lastActiveLoop * numParamsPerSlot + kNumLoops);
            s_NumLoops->setVSTSlider(value);
            break;
        case kNextSlot:
            s_NextSlot->setIndex(lastActiveLoop * numParamsPerSlot + kNextSlot);
            s_NextSlot->setVSTSlider(value);
            break;
        case kPlayGroup:
            s_PlayGroup->setIndex(lastActiveLoop * numParamsPerSlot + kPlayGroup);
            s_PlayGroup->setVSTSlider(value);
            break;
        case kMuteGroup:
            s_MuteGroup->setIndex(lastActiveLoop * numParamsPerSlot + kMuteGroup);
            s_MuteGroup->setVSTSlider(value);
            break;
        case kForceToKey:
            b_ForceToKey->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kScaleChannel:
            s_ScaleChannel->setIndex(lastActiveLoop * numParamsPerSlot + kScaleChannel);
            s_ScaleChannel->setVSTSlider(value);
            break;
        case kTransposeChannel:
            s_TransposeChannel->setIndex(lastActiveLoop * numParamsPerSlot + kTransposeChannel);
            s_TransposeChannel->setVSTSlider(value);
            break;
        case kUseScaleChannel:
            b_UseScaleChannel->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kUseTrChannel:
            b_UseTrChannel->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kNoteToggle:
            b_NoteToggle->setToggleState(value >= 0.5f, juce::dontSendNotification);
            break;
        case kForceToScaleMode:
            forceModeBox->setText(getFilter()->getCurrentSlotParameterText(kForceToScaleMode), juce::dontSendNotification);
            break;
        case kTranspose10:
            b_Transpose10->setToggleState(value >= 0.5f, juce::dontSendNotification);
        default:
            break;
    }
}

void PizLooperEditor::updateSlotButtons()
{
    PizLooper* const filter = getFilter();
    for (int i = 0; i < numSlots; i++)
    {
        juce::TextButton* b = getButtonForSlot(i);
        if (filter->isSlotPlaying(i))
        {
            b->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff17e617));
            b->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff17e617));
        }
        else
        {
            b->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffd3d3d3));
            b->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff979797));
        }
        if (filter->isLoopEmpty(i))
        {
            b->setColour(juce::TextButton::textColourOffId, juce::Colour(0xff979797));
            b->setColour(juce::TextButton::textColourOnId, juce::Colour(0xff555555));
        }
        else
        {
            b->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
            b->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
        }
        b->setButtonText(juce::String(i + 1));
    }
}

void PizLooperEditor::updateParametersFromFilter()
{
    PizLooper* const filter = getFilter();

    float param[numParamsPerSlot + numGlobalParams];
    bool slotPlayState[numSlots];

    filter->getCallbackLock().enter();

    for (int i = 0; i < numParamsPerSlot + numGlobalParams; i++)
    {
        param[i] = filter->getParamForActiveSlot(i);
    }
    lastActiveLoop = filter->getCurrentProgram();
    for (int i = 0; i < numSlots; i++)
    {
        slotPlayState[i] = filter->isSlotPlaying(i);
    }

    const int w = filter->lastUIWidth;
    const int h = filter->lastUIHeight;

    const int n = filter->getNumerator(lastActiveLoop);
    const int d = filter->getDenominator(lastActiveLoop);

    const int newDevice = filter->devices.indexOf(filter->getActiveDevice());
    bool newloop        = filter->newLoop;

    filter->getCallbackLock().exit();

    // ..and after releasing the lock, we're free to do the time-consuming UI stuff..
    {
        b_Snap->setToggleState(filter->getPRSetting("snap"), juce::dontSendNotification);
        pianoRoll->setSnap(b_Snap->getToggleState());
        float q = filter->getPRSetting("stepsize");
        if (q == 0.0)
        {
            quantizeBox2->setText("4th", juce::dontSendNotification);
        }
        else if (q < 0.3)
        {
            quantizeBox2->setText("8th", juce::dontSendNotification);
        }
        else if (q < 0.6)
        {
            quantizeBox2->setText("16th", juce::dontSendNotification);
        }
        else if (q < 0.9)
        {
            quantizeBox2->setText("32nd", juce::dontSendNotification);
        }
        else
        {
            quantizeBox2->setText("64th", juce::dontSendNotification);
        }
        b_Dotted->setToggleState(filter->getPRSetting("dotted"), juce::dontSendNotification);
        b_Triplet->setToggleState(filter->getPRSetting("triplet"), juce::dontSendNotification);
        float tord = (filter->getPRSetting("triplet")) ? 1.5f : 1.f;
        if (filter->getPRSetting("dotted"))
        {
            tord = 0.666666667f;
        }
        if (q == 0.0)
        {
            pianoRoll->setNoteLength(1 * tord);
        }
        else if (q < 0.3)
        {
            pianoRoll->setNoteLength(2 * tord);
        }
        else if (q < 0.6)
        {
            pianoRoll->setNoteLength(4 * tord);
        }
        else if (q < 0.9)
        {
            pianoRoll->setNoteLength(8 * tord);
        }
        else
        {
            pianoRoll->setNoteLength(16 * tord);
        }
        pianoRoll->setDisplayLength((int) filter->getPRSetting("bars"));
        pianoRoll->setSize(filter->getPRSetting("width"), filter->getPRSetting("height"));
        keyboard->setSize(25, pianoRoll->getHeight());
        keyboard->setKeyWidth((float) pianoRoll->getHeight() / 74.75f);
        viewport->setViewPosition(filter->getPRSetting("x"),
                                  filter->getPRSetting("y"));
    }
    pianoRoll->setTimeSig(n, d);
    numerator->setText(juce::String(n), juce::dontSendNotification);
    denominator->setText(juce::String(d), juce::dontSendNotification);

    midiOutDeviceBox->setSelectedItemIndex(newDevice + 1, juce::dontSendNotification);

    for (int i = 0; i < numParamsPerSlot + numGlobalParams; i++)
    {
        updateControls(i, param[i], true);
    }

    //loop selector buttons
    for (int i = 0; i < numSlots; i++)
    {
        juce::TextButton* b = getButtonForSlot(i);
        if (slotPlayState[i])
        {
            b->setColour(juce::TextButton::buttonColourId, juce::Colour(0xff17e617));
            b->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff17e617));
        }
        else
        {
            b->setColour(juce::TextButton::buttonColourId, juce::Colour(0xffd3d3d3));
            b->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff979797));
        }
        if (filter->isLoopEmpty(i))
        {
            b->setColour(juce::TextButton::textColourOffId, juce::Colour(0xff979797));
            b->setColour(juce::TextButton::textColourOnId, juce::Colour(0xff555555));
        }
        else
        {
            b->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
            b->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
        }
        b->setButtonText(juce::String(i + 1));
    }
    getButtonForSlot(lastActiveLoop)->setToggleState(true, juce::dontSendNotification);

    nameLabel->setText(filter->getProgramName(lastActiveLoop), juce::dontSendNotification);

    if (filter->currentNumEvents == 0)
    {
        loopinfoLabel->setText("No Loop", juce::dontSendNotification);
    }
    else
    {
        juce::String loopinfo = "Loop length: ";
        if (filter->currentLength == 1.0)
        {
            loopinfo << "1 Beat (";
        }
        else
        {
            loopinfo << filter->currentLength << " Beats (";
        }
        loopinfo << filter->currentNumEvents << " Events)";
        loopinfoLabel->setText(loopinfo, juce::dontSendNotification);
    }
    noSnap = true;
    timeline->setLoop(filter->getLoopStart(lastActiveLoop), filter->getLoopLength(lastActiveLoop));
    noSnap = false;

    loopinfoLabel2->setText(filter->info->s, juce::dontSendNotification);

    if (newloop)
    {
        pianoRoll->setSequence(filter->getActiveLoop());
        filter->newLoop = false;
    }
    LengthLabel->setText(juce::String(pianoRoll->getDisplayLength()), juce::dontSendNotification);
    setSize(w, h);
}

juce::TextButton* PizLooperEditor::getButtonForSlot(int slot)
{
    switch (slot)
    {
        case 0:
            return textButton1.get();
        case 1:
            return textButton2.get();
        case 2:
            return textButton3.get();
        case 3:
            return textButton4.get();
        case 4:
            return textButton5.get();
        case 5:
            return textButton6.get();
        case 6:
            return textButton7.get();
        case 7:
            return textButton8.get();
        case 8:
            return textButton9.get();
        case 9:
            return textButton10.get();
        case 10:
            return textButton11.get();
        case 11:
            return textButton12.get();
        case 12:
            return textButton13.get();
        case 13:
            return textButton14.get();
        case 14:
            return textButton15.get();
        case 15:
            return textButton16.get();
        case 16:
            return textButton17.get();
        case 17:
            return textButton18.get();
        case 18:
            return textButton19.get();
        case 19:
            return textButton20.get();
        case 20:
            return textButton21.get();
        case 21:
            return textButton22.get();
        case 22:
            return textButton23.get();
        case 23:
            return textButton24.get();
        case 24:
            return textButton25.get();
        case 25:
            return textButton26.get();
        case 26:
            return textButton27.get();
        case 27:
            return textButton28.get();
        case 28:
            return textButton29.get();
        case 29:
            return textButton30.get();
        case 30:
            return textButton31.get();
        case 31:
            return textButton32.get();
        case 32:
            return textButton33.get();
        case 33:
            return textButton34.get();
        case 34:
            return textButton35.get();
        case 35:
            return textButton36.get();
        case 36:
            return textButton37.get();
        case 37:
            return textButton38.get();
        case 38:
            return textButton39.get();
        case 39:
            return textButton40.get();
        case 40:
            return textButton41.get();
        case 41:
            return textButton42.get();
        case 42:
            return textButton43.get();
        case 43:
            return textButton44.get();
        case 44:
            return textButton45.get();
        case 45:
            return textButton46.get();
        case 46:
            return textButton47.get();
        case 47:
            return textButton48.get();
        case 48:
            return textButton49.get();
        case 49:
            return textButton50.get();
        case 50:
            return textButton51.get();
        case 51:
            return textButton52.get();
        case 52:
            return textButton53.get();
        case 53:
            return textButton54.get();
        case 54:
            return textButton55.get();
        case 55:
            return textButton56.get();
        case 56:
            return textButton57.get();
        case 57:
            return textButton58.get();
        case 58:
            return textButton59.get();
        case 59:
            return textButton60.get();
        case 60:
            return textButton61.get();
        case 61:
            return textButton62.get();
        case 62:
            return textButton63.get();
        case 63:
            return textButton64.get();
        case 64:
            return textButton65.get();
        case 65:
            return textButton66.get();
        case 66:
            return textButton67.get();
        case 67:
            return textButton68.get();
        case 68:
            return textButton69.get();
        case 69:
            return textButton70.get();
        case 70:
            return textButton71.get();
        case 71:
            return textButton72.get();
        case 72:
            return textButton73.get();
        case 73:
            return textButton74.get();
        case 74:
            return textButton75.get();
        case 75:
            return textButton76.get();
        case 76:
            return textButton77.get();
        case 77:
            return textButton78.get();
        case 78:
            return textButton79.get();
        case 79:
            return textButton80.get();
        case 80:
            return textButton81.get();
        case 81:
            return textButton82.get();
        case 82:
            return textButton83.get();
        case 83:
            return textButton84.get();
        case 84:
            return textButton85.get();
        case 85:
            return textButton86.get();
        case 86:
            return textButton87.get();
        case 87:
            return textButton88.get();
        case 88:
            return textButton89.get();
        case 89:
            return textButton90.get();
        case 90:
            return textButton91.get();
        case 91:
            return textButton92.get();
        case 92:
            return textButton93.get();
        case 93:
            return textButton94.get();
        case 94:
            return textButton95.get();
        case 95:
            return textButton96.get();
        case 96:
            return textButton97.get();
        case 97:
            return textButton98.get();
        case 98:
            return textButton99.get();
        case 99:
            return textButton100.get();
        case 100:
            return textButton101.get();
        case 101:
            return textButton102.get();
        case 102:
            return textButton103.get();
        case 103:
            return textButton104.get();
        case 104:
            return textButton105.get();
        case 105:
            return textButton106.get();
        case 106:
            return textButton107.get();
        case 107:
            return textButton108.get();
        case 108:
            return textButton109.get();
        case 109:
            return textButton110.get();
        case 110:
            return textButton111.get();
        case 111:
            return textButton112.get();
        case 112:
            return textButton113.get();
        case 113:
            return textButton114.get();
        case 114:
            return textButton115.get();
        case 115:
            return textButton116.get();
        case 116:
            return textButton117.get();
        case 117:
            return textButton118.get();
        case 118:
            return textButton119.get();
        case 119:
            return textButton120.get();
        case 120:
            return textButton121.get();
        case 121:
            return textButton122.get();
        case 122:
            return textButton123.get();
        case 123:
            return textButton124.get();
        case 124:
            return textButton125.get();
        case 125:
            return textButton126.get();
        case 126:
            return textButton127.get();
        case 127:
            return textButton128.get();
        default:
            return 0;
    }
}

int PizLooperEditor::getButtonIndex(Component* button)
{
    if (button == textButton1.get())
    {
        return 0;
    }
    if (button == textButton2.get())
    {
        return 1;
    }
    if (button == textButton3.get())
    {
        return 2;
    }
    if (button == textButton4.get())
    {
        return 3;
    }
    if (button == textButton5.get())
    {
        return 4;
    }
    if (button == textButton6.get())
    {
        return 5;
    }
    if (button == textButton7.get())
    {
        return 6;
    }
    if (button == textButton8.get())
    {
        return 7;
    }
    if (button == textButton9.get())
    {
        return 8;
    }
    if (button == textButton10.get())
    {
        return 9;
    }
    if (button == textButton11.get())
    {
        return 10;
    }
    if (button == textButton12.get())
    {
        return 11;
    }
    if (button == textButton13.get())
    {
        return 12;
    }
    if (button == textButton14.get())
    {
        return 13;
    }
    if (button == textButton15.get())
    {
        return 14;
    }
    if (button == textButton16.get())
    {
        return 15;
    }
    if (button == textButton17.get())
    {
        return 16;
    }
    if (button == textButton18.get())
    {
        return 17;
    }
    if (button == textButton19.get())
    {
        return 18;
    }
    if (button == textButton20.get())
    {
        return 19;
    }
    if (button == textButton21.get())
    {
        return 20;
    }
    if (button == textButton22.get())
    {
        return 21;
    }
    if (button == textButton23.get())
    {
        return 22;
    }
    if (button == textButton24.get())
    {
        return 23;
    }
    if (button == textButton25.get())
    {
        return 24;
    }
    if (button == textButton26.get())
    {
        return 25;
    }
    if (button == textButton27.get())
    {
        return 26;
    }
    if (button == textButton28.get())
    {
        return 27;
    }
    if (button == textButton29.get())
    {
        return 28;
    }
    if (button == textButton30.get())
    {
        return 29;
    }
    if (button == textButton31.get())
    {
        return 30;
    }
    if (button == textButton32.get())
    {
        return 31;
    }
    if (button == textButton33.get())
    {
        return 32;
    }
    if (button == textButton34.get())
    {
        return 33;
    }
    if (button == textButton35.get())
    {
        return 34;
    }
    if (button == textButton36.get())
    {
        return 35;
    }
    if (button == textButton37.get())
    {
        return 36;
    }
    if (button == textButton38.get())
    {
        return 37;
    }
    if (button == textButton39.get())
    {
        return 38;
    }
    if (button == textButton40.get())
    {
        return 39;
    }
    if (button == textButton41.get())
    {
        return 40;
    }
    if (button == textButton42.get())
    {
        return 41;
    }
    if (button == textButton43.get())
    {
        return 42;
    }
    if (button == textButton44.get())
    {
        return 43;
    }
    if (button == textButton45.get())
    {
        return 44;
    }
    if (button == textButton46.get())
    {
        return 45;
    }
    if (button == textButton47.get())
    {
        return 46;
    }
    if (button == textButton48.get())
    {
        return 47;
    }
    if (button == textButton49.get())
    {
        return 48;
    }
    if (button == textButton50.get())
    {
        return 49;
    }
    if (button == textButton51.get())
    {
        return 50;
    }
    if (button == textButton52.get())
    {
        return 51;
    }
    if (button == textButton53.get())
    {
        return 52;
    }
    if (button == textButton54.get())
    {
        return 53;
    }
    if (button == textButton55.get())
    {
        return 54;
    }
    if (button == textButton56.get())
    {
        return 55;
    }
    if (button == textButton57.get())
    {
        return 56;
    }
    if (button == textButton58.get())
    {
        return 57;
    }
    if (button == textButton59.get())
    {
        return 58;
    }
    if (button == textButton60.get())
    {
        return 59;
    }
    if (button == textButton61.get())
    {
        return 60;
    }
    if (button == textButton62.get())
    {
        return 61;
    }
    if (button == textButton63.get())
    {
        return 62;
    }
    if (button == textButton64.get())
    {
        return 63;
    }
    if (button == textButton65.get())
    {
        return 64;
    }
    if (button == textButton66.get())
    {
        return 65;
    }
    if (button == textButton67.get())
    {
        return 66;
    }
    if (button == textButton68.get())
    {
        return 67;
    }
    if (button == textButton69.get())
    {
        return 68;
    }
    if (button == textButton70.get())
    {
        return 69;
    }
    if (button == textButton71.get())
    {
        return 70;
    }
    if (button == textButton72.get())
    {
        return 71;
    }
    if (button == textButton73.get())
    {
        return 72;
    }
    if (button == textButton74.get())
    {
        return 73;
    }
    if (button == textButton75.get())
    {
        return 74;
    }
    if (button == textButton76.get())
    {
        return 75;
    }
    if (button == textButton77.get())
    {
        return 76;
    }
    if (button == textButton78.get())
    {
        return 77;
    }
    if (button == textButton79.get())
    {
        return 78;
    }
    if (button == textButton80.get())
    {
        return 79;
    }
    if (button == textButton81.get())
    {
        return 80;
    }
    if (button == textButton82.get())
    {
        return 81;
    }
    if (button == textButton83.get())
    {
        return 82;
    }
    if (button == textButton84.get())
    {
        return 83;
    }
    if (button == textButton85.get())
    {
        return 84;
    }
    if (button == textButton86.get())
    {
        return 85;
    }
    if (button == textButton87.get())
    {
        return 86;
    }
    if (button == textButton88.get())
    {
        return 87;
    }
    if (button == textButton89.get())
    {
        return 88;
    }
    if (button == textButton90.get())
    {
        return 89;
    }
    if (button == textButton91.get())
    {
        return 90;
    }
    if (button == textButton92.get())
    {
        return 91;
    }
    if (button == textButton93.get())
    {
        return 92;
    }
    if (button == textButton94.get())
    {
        return 93;
    }
    if (button == textButton95.get())
    {
        return 94;
    }
    if (button == textButton96.get())
    {
        return 95;
    }
    if (button == textButton97.get())
    {
        return 96;
    }
    if (button == textButton98.get())
    {
        return 97;
    }
    if (button == textButton99.get())
    {
        return 98;
    }
    if (button == textButton100.get())
    {
        return 99;
    }
    if (button == textButton101.get())
    {
        return 100;
    }
    if (button == textButton102.get())
    {
        return 101;
    }
    if (button == textButton103.get())
    {
        return 102;
    }
    if (button == textButton104.get())
    {
        return 103;
    }
    if (button == textButton105.get())
    {
        return 104;
    }
    if (button == textButton106.get())
    {
        return 105;
    }
    if (button == textButton107.get())
    {
        return 106;
    }
    if (button == textButton108.get())
    {
        return 107;
    }
    if (button == textButton109.get())
    {
        return 108;
    }
    if (button == textButton110.get())
    {
        return 109;
    }
    if (button == textButton111.get())
    {
        return 110;
    }
    if (button == textButton112.get())
    {
        return 111;
    }
    if (button == textButton113.get())
    {
        return 112;
    }
    if (button == textButton114.get())
    {
        return 113;
    }
    if (button == textButton115.get())
    {
        return 114;
    }
    if (button == textButton116.get())
    {
        return 115;
    }
    if (button == textButton117.get())
    {
        return 116;
    }
    if (button == textButton118.get())
    {
        return 117;
    }
    if (button == textButton119.get())
    {
        return 118;
    }
    if (button == textButton120.get())
    {
        return 119;
    }
    if (button == textButton121.get())
    {
        return 120;
    }
    if (button == textButton122.get())
    {
        return 121;
    }
    if (button == textButton123.get())
    {
        return 122;
    }
    if (button == textButton124.get())
    {
        return 123;
    }
    if (button == textButton125.get())
    {
        return 124;
    }
    if (button == textButton126.get())
    {
        return 125;
    }
    if (button == textButton127.get())
    {
        return 126;
    }
    if (button == textButton128.get())
    {
        return 127;
    }
    return -1;
}

//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PizLooperEditor" componentName=""
                 parentClasses="public AudioProcessorEditor, public ChangeListener, public FileDragAndDropTarget, public ClickableLabelListener, public Timer, public MidiKeyboardStateListener"
                 constructorParams="PizLooper* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="0" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="487">
  <BACKGROUND backgroundColour="ff202029">
    <RECT pos="1 393 303 78" fill="linear: 176 352, 176 443, 0=ff000000, 1=ff404049"
          hasStroke="0"/>
    <RECT pos="305 393 81 78" fill="linear: 190 369, 190 419, 0=ff000000, 1=ff162938"
          hasStroke="0"/>
    <ROUNDRECT pos="390 62 391M 40" cornerSize="10.0" fill="solid: ffa0a0a0"
               hasStroke="0"/>
    <RECT pos="152 -1 151M 56" fill="linear: 160 -30, 161 43, 0=ff000000, 1=ff404049"
          hasStroke="1" stroke="1, mitered, butt" strokeColour="solid: ff46495f"/>
    <RECT pos="0 0 152 55" fill="solid: ff202029" hasStroke="0"/>
    <ROUNDRECT pos="6 317 136 90" cornerSize="14.5" fill="solid: ff404049" hasStroke="0"/>
    <RECT pos="1 246 385 147" fill="linear: 159 215, 159 307, 0=ff000000, 1=ff404049"
          hasStroke="0"/>
    <ROUNDRECT pos="6 159 136 104" cornerSize="14.5" fill="solid: ff404049"
               hasStroke="0"/>
    <RECT pos="1 109 385 140" fill="linear: 100 89, 100 165, 0=ff000000, 1=ff404049"
          hasStroke="0"/>
    <ROUNDRECT pos="6 -20 139 67" cornerSize="14.5" fill="linear: 95 -25, 95 107, 0=ff002700, 1=ba267387"
               hasStroke="1" stroke="1, mitered, butt" strokeColour="solid: ffcbcbcb"/>
    <IMAGE pos="12 12 41 25" resource="piznew40_png" opacity="1.0" mode="0"/>
    <TEXT pos="44 23 108 20" fill="solid: ffcbcbcb" hasStroke="0" text="midiLooper"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="36" typefaceStyle="Bold"/>
    <TEXT pos="46 8 108 20" fill="solid: ffbfbfbf" hasStroke="0" text="Insert Piz Here-&gt;"
          fontname="Default font" fontsize="10.0" kerning="0.0" bold="1"
          italic="0" justification="36" typefaceStyle="Bold"/>
    <RECT pos="9 202 369 42" fill="linear: 100 194, 100 214, 0=ff433b22, 1=ff8b7a47"
          hasStroke="1" stroke="1, mitered, butt" strokeColour="solid: ff000000"/>
    <RECT pos="1 85 385 32" fill="linear: 100 85, 100 98, 0=ff979797, 1=ff6f737a"
          hasStroke="0"/>
    <RECT pos="390 85 391M 20" fill="solid: ff8c8c8c" hasStroke="0"/>
    <TEXT pos="215 177 24 15" fill="solid: ffffffff" hasStroke="0" text="-&gt;"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="153 162 24 15" fill="solid: ffffffff" hasStroke="0" text="PLAY"
          fontname="Default font" fontsize="10.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="230 162 29 15" fill="solid: ffffffff" hasStroke="0" text="THEN"
          fontname="Default font" fontsize="10.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <ROUNDRECT pos="6 458 136 27" cornerSize="14.5" fill="solid: ff404049" hasStroke="0"/>
    <TEXT pos="574 67 20 12" fill="solid: ff000000" hasStroke="0" text="/"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <PATH pos="0 0 100 100" fill="solid: 822aa5" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: 5b5b5b" nonZeroWinding="1">s 139 294 l 139 310</PATH>
    <PATH pos="0 0 100 100" fill="solid: 822aa5" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: 5b5b5b" nonZeroWinding="1">s 141 294 l 136 294</PATH>
    <ROUNDRECT pos="141 254 161 72" cornerSize="10.0" fill="solid: 86404049"
               hasStroke="0"/>
    <ROUNDRECT pos="8 311 371 36" cornerSize="10.0" fill="linear: 256 265, 256 355, 0=ff000000, 1=ff434450"
               hasStroke="1" stroke="0.7, mitered, butt" strokeColour="solid: ff111111"/>
    <PATH pos="0 0 100 100" fill="solid: 822aa5" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: 5b5b5b" nonZeroWinding="1">s 29 310 l 29 301</PATH>
    <RECT pos="723 64 51 18" fill="solid: ffbababa" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff000000"/>
  </BACKGROUND>
  <LABEL name="new label" id="43a4ad473c82373c" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="613 64 46 18" edTextCol="ff000000"
         edBkgCol="0" labelText="Zoom" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="timeline" id="a7625d2eb81703f2" memberName="timeline" virtualName="Timeline"
                    explicitFocusOrder="0" pos="415 85 430M 20" class="Component"
                    params=""/>
  <TEXTBUTTON name="new button" id="cc9f83dd3d0468de" memberName="textButton1"
              virtualName="" explicitFocusOrder="0" pos="0 61 25 24" buttonText="1"
              connectedEdges="10" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="bbcf37dc6f0b1324" memberName="textButton2"
              virtualName="" explicitFocusOrder="0" pos="25 61 23 24" bgColOff="ffbbbbff"
              buttonText="2" connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="90432b9c6e7e857a" memberName="textButton3"
              virtualName="" explicitFocusOrder="0" pos="48 61 23 24" buttonText="3"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="88345655396ef38e" memberName="textButton4"
              virtualName="" explicitFocusOrder="0" pos="71 61 23 24" buttonText="4"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="3e29aef78f94dd2e" memberName="textButton5"
              virtualName="" explicitFocusOrder="0" pos="94 61 23 24" buttonText="5"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="4f173aa53ebf0c9d" memberName="textButton6"
              virtualName="" explicitFocusOrder="0" pos="117 61 23 24" buttonText="6"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="eaa3684bc44dddb" memberName="textButton7"
              virtualName="" explicitFocusOrder="0" pos="140 61 23 24" buttonText="7"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="804d8ad699bf3628" memberName="textButton8"
              virtualName="" explicitFocusOrder="0" pos="163 61 23 24" buttonText="8"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="9a99db46cfdaefcb" memberName="textButton9"
              virtualName="" explicitFocusOrder="0" pos="186 61 23 24" buttonText="9"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="3245c8856dffbe0f" memberName="textButton10"
              virtualName="" explicitFocusOrder="0" pos="209 61 25 24" buttonText="10"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="cde037153fb2cd8e" memberName="textButton11"
              virtualName="" explicitFocusOrder="0" pos="234 61 25 24" buttonText="11"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="8de7af63894ca4b1" memberName="textButton12"
              virtualName="" explicitFocusOrder="0" pos="259 61 25 24" buttonText="12"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="126f2b1b010e0876" memberName="textButton13"
              virtualName="" explicitFocusOrder="0" pos="284 61 25 24" buttonText="13"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="39b1e3b78817e974" memberName="textButton14"
              virtualName="" explicitFocusOrder="0" pos="309 61 25 24" buttonText="14"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="eefcf6590c869d1c" memberName="textButton15"
              virtualName="" explicitFocusOrder="0" pos="334 61 25 24" buttonText="15"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="506ef0db1b30e740" memberName="textButton16"
              virtualName="" explicitFocusOrder="0" pos="359 61 28 24" buttonText="16"
              connectedEdges="9" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="Play" id="40d6a4b1059081e9" memberName="b_Play" virtualName=""
              explicitFocusOrder="0" pos="78 117 72 50" tooltip="Toggle playback of current slot"
              bgColOff="d213540e" bgColOn="ff00c400" textCol="ff000000" textColOn="ffffffff"
              buttonText="PLAY" connectedEdges="5" needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="Record" id="6c2bf14bf3928adc" memberName="b_Record" virtualName=""
              explicitFocusOrder="0" pos="6 117 72 50" tooltip="Toggle recording to current slot"
              bgColOff="d2a90000" bgColOn="ffff0000" textCol="ffffffff" textColOn="ffffffff"
              buttonText="RECORD " connectedEdges="6" needsCallback="0" radioGroupId="0"/>
  <TEXTBUTTON name="Overdub" id="3cc1043be8aa3219" memberName="b_Overdub" virtualName=""
              explicitFocusOrder="0" pos="523 12 80 20" tooltip="Toggle overdub recording"
              bgColOff="ff999999" textCol="ff000000" textColOn="ff000000" buttonText="Overdub"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="MIDI Thru" id="a5cd82c0cb72daa9" memberName="b_Thru" virtualName=""
              explicitFocusOrder="0" pos="634 33 40 20" tooltip="Toggle MIDI Thru (Notes selected for Note Triggering and Scale Channel are always blocked)"
              bgColOff="ff999999" buttonText="Thru" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Clear" id="cec5dda5d9d59001" memberName="b_Clear" virtualName=""
              explicitFocusOrder="0" pos="264 90 39 22" tooltip="Erase MIDI data from the current slot"
              bgColOff="ff000000" textCol="ffffffff" textColOn="fff0ffff" buttonText="Clear"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="Loop Step Size" id="654c333e00a3602f" memberName="stepsizeBox"
            virtualName="" explicitFocusOrder="0" pos="264 15 77 16" tooltip="Recording length will be quantized to this step size."
            editable="0" layout="33" items="1 Bar&#10;3 Beats&#10;2 Beats&#10;1 Beat&#10;16th Note&#10;1 Tick"
            textWhenNonSelected="16th Note" textWhenNoItems="(no choices)"/>
  <SLIDER name="Transpose" id="bebd4900b26b7ea8" memberName="s_Transpose"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="146 283 72 20"
          tooltip="Transposition applied to the current slot" bkgcol="1e000000"
          thumbcol="ff000000" textboxtext="ffffffff" min="-12.0" max="12.0"
          int="1.0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="Octave Shift" id="d17041cc05fad561" memberName="s_Octave"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="226 283 72 20"
          tooltip="Transposition by octave for the current slot" bkgcol="1e000000"
          thumbcol="ff000000" textboxtext="ffffffff" min="-4.0" max="4.0"
          int="1.0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="Velocity Offset" id="2c207442dda4efb5" memberName="s_Velocity"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="306 283 72 20"
          tooltip="Velocity adjustment for the current slot" bkgcol="1e000000"
          thumbcol="ff000000" textboxtext="ffffffff" min="0.0" max="200.0"
          int="1.0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="46c2229e3e67f2f1" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="146 267 72 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Semitones" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="6f6ca47640c250a9" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="226 267 72 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Octave" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="93b1b361c9cc0086" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="301 267 80 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Velocity" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="Loop Start" id="f1ca2a13980568a8" memberName="s_Start"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="66 367 72 20"
          tooltip="Offsets the loop start time by this number of beats"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="-8.0" max="8.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="2c7022ccbc071339" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="66 351 72 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Start Offset" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="Loop End" id="dfaa63118a778aff" memberName="s_End" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="146 367 72 20" tooltip="Offsets the loop end time by this number of beats"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="-8.0" max="8.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="5015260b61e3b2f3" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="146 351 72 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="End Offset" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="Loop Stretch" id="a9f425f1801b9523" memberName="s_Stretch"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="306 367 72 20"
          tooltip="Playback speed, relative to host tempo" bkgcol="1e000000"
          thumbcol="ff000000" textboxtext="ffffffff" min="-10.0" max="10.0"
          int="1.0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="1b0b033f88b7706c" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="306 351 72 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Speed" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <COMBOBOX name="Loop Mode" id="c2baafe98c323fba" memberName="loopmodeBox"
            virtualName="" explicitFocusOrder="0" pos="175 144 110 16" tooltip="Playback Mode: &quot;Sync Loop&quot; follows the host timeline. &quot;Loop after rec&quot; is the same but also plays automatically as soon as recording ends. &quot;Unsync&quot; modes play the pattern from the beginning as soon as playback is started."
            editable="0" layout="33" items="Loop after rec&#10;Sync loop&#10;Unsync 1-shot&#10;Unsync loop"
            textWhenNonSelected="Unsync 1-shot" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="Note Trigger" id="3db6b8a47a703b85" memberName="notetriggerBox"
            virtualName="" explicitFocusOrder="0" pos="146 402 106 16" tooltip="For &quot;Transpose&quot; modes, pattern will be transposed relative to &quot;Root Note&quot;"
            editable="0" layout="33" items="Off&#10;Mono (Transpose)&#10;Poly (Transpose)&#10;Mono (Orig. Key)&#10;"
            textWhenNonSelected="Mono (Transposed)" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="Sync" id="32c39e356a406b40" memberName="syncmodeBox" virtualName=""
            explicitFocusOrder="0" pos="159 15 99 16" tooltip="&quot;PPQ&quot; modes always follow host timeline, which may not work in all hosts. &quot;Sample&quot; mode ignores the host's timeline, but the host's tempo is still followed."
            editable="0" layout="33" items="PPQ (Host 0)&#10;PPQ (Recstart)&#10;Sample"
            textWhenNonSelected="PPQ (Recstart)" textWhenNoItems="(no choices)"/>
  <SLIDER name="Root Note" id="2c76ae48d2c4565c" memberName="s_Root" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="76 174 64 20" tooltip="Transposed note triggering and Scale Channel input will transpose the pattern relative to this note"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="-1.0" max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="98b122ff20c1e0ed" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="15 176 64 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Root Note:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="Low Note" id="9fc4ae89d34ed5e4" memberName="s_Low" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="18 440 64 20" tooltip="Lowest note to use for triggering"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="-1.0" max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="fe46ab559c888729" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="18 424 64 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Low Note" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="High Note" id="7d4c7469f530caf5" memberName="s_High" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="90 440 64 20" tooltip="Highest note to use for triggering"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="-1.0" max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="a243134fcde26e0c" memberName="label11" virtualName=""
         explicitFocusOrder="0" pos="90 424 64 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="High Note" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="TriggerChannel" id="dfa469788a1385f7" memberName="s_TrigChan"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="162 440 64 20"
          tooltip="Channel to use for trigger notes" bkgcol="1e000000"
          thumbcol="ff000000" textboxtext="ffffffff" min="1.0" max="16.0"
          int="1.0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="Trigger Channel" id="fbd851f9f6e023e1" memberName="label12"
         virtualName="" explicitFocusOrder="0" pos="161 424 64 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Channel" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="Load" id="af7a2e7eac7e3bc3" memberName="b_Reload" virtualName=""
              explicitFocusOrder="0" pos="344 90 39 22" tooltip="Load MIDI file (Ctrl-click: load MIDI file with the current pattern name from the &quot;midiloops&quot; folder)"
              bgColOff="ff000000" textCol="ffffffff" textColOn="ffffffff" buttonText="Load"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="Input Quantize Step" id="3542a5d892a57b8" memberName="quantizeBox"
            virtualName="" explicitFocusOrder="0" pos="439 15 77 16" tooltip="Recorded events will be quantized to this step size"
            editable="0" layout="33" items="Off&#10;8th&#10;16th&#10;32nd&#10;64th"
            textWhenNonSelected="32nd" textWhenNoItems="(no choices)"/>
  <LABEL name="LoopStepSize" id="b46f312ba1698624" memberName="label21"
         virtualName="" explicitFocusOrder="0" pos="261 0 84 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Loop Step Size"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <SLIDER name="Shift" id="128b285d21c35465" memberName="s_Shift" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="226 367 72 20" tooltip="Shifts the pattern by this number of beats, with wraparound"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="-8.0" max="8.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Shift" id="493778df7df64035" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="226 351 72 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Beat Shift" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="QuantizeLabel" id="9539aa66eb92d49f" memberName="label23"
         virtualName="" explicitFocusOrder="0" pos="433 0 87 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Quantize Input"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="Name" id="c61a01db7e29e02e" memberName="nameLabel" virtualName="ClickableLabel"
         explicitFocusOrder="0" pos="4 87 256 27" tooltip="Current pattern name (double-click to edit)"
         edTextCol="ff000000" edBkgCol="ffffffff" labelText="Bassline (4 bars)"
         editableSingleClick="0" editableDoubleClick="1" focusDiscardsChanges="0"
         fontname="Default font" fontsize="26.3" kerning="0.0" bold="1"
         italic="0" justification="33" typefaceStyle="Bold"/>
  <TEXTBUTTON name="Save" id="2cda0dd565e6a8a4" memberName="b_Save" virtualName=""
              explicitFocusOrder="0" pos="304 90 39 22" tooltip="Save a MIDI file of the current pattern (Ctrl-click: save to the &quot;midiloops&quot; folder with the current name)"
              bgColOff="ff000000" textCol="ffffffff" textColOn="ffffffff" buttonText="Save"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <LABEL name="Sync" id="531c1b1950299dd1" memberName="label22" virtualName=""
         explicitFocusOrder="0" pos="162 0 95 16" textCol="ffffffff" edTextCol="ff000000"
         edBkgCol="0" labelText="Host Sync Mode" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="Sync:" id="8b2914befe90511" memberName="label18" virtualName=""
         explicitFocusOrder="0" pos="12 469 125 16" textCol="ff9f9f9f"
         edTextCol="ff000000" edBkgCol="0" labelText="Note Triggering"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="12.0" kerning="0.0" bold="1"
         italic="0" justification="36" typefaceStyle="Bold"/>
  <LABEL name="Loop Info" id="9e460abf305befb3" memberName="loopinfoLabel"
         virtualName="" explicitFocusOrder="0" pos="14 205 272 16" textCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="label text" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Loop Info 2" id="afa6b5a7c7af7805" memberName="loopinfoLabel2"
         virtualName="" explicitFocusOrder="0" pos="14 225 272 16" edTextCol="ff000000"
         edBkgCol="0" labelText="label text" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="Sync:" id="317b90b2dfd6f517" memberName="label17" virtualName=""
         explicitFocusOrder="0" pos="14 391 125 16" textCol="ff9f9f9f"
         edTextCol="ff000000" edBkgCol="0" labelText="Loop Manipulation"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="12.0" kerning="0.0" bold="1"
         italic="0" justification="36" typefaceStyle="Bold"/>
  <SLIDER name="Channel" id="94315107a91d9b27" memberName="s_Channel" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="314 414 64 20" tooltip="Input and output channel for the current slot"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="0.0" max="16.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="I/O Channel" id="842d9c7cde349df9" memberName="label19"
         virtualName="" explicitFocusOrder="0" pos="309 398 74 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="I/O Channel" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="RecordLengthLabel" id="63744c8a8acc6491" memberName="label20"
         virtualName="" explicitFocusOrder="0" pos="347 0 83 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Record Length" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="Recording Length" id="e936697207b7e6e8" memberName="s_FixedLength"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="347 15 86 16"
          tooltip="If set to &quot;Manual&quot;, recording will go on as long as the record button is on. Otherwise, length will be limited to this number of steps (based on &quot;Loop Step Size&quot; setting)."
          bkgcol="1e707070" thumbcol="ff000000" textboxtext="ffffffff"
          min="0.0" max="32.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TEXTBUTTON name="Transform/Filter" id="f37c696b28323e7f" memberName="b_Filt"
              virtualName="" explicitFocusOrder="0" pos="314 440 64 20" tooltip="Transform: all events in the pattern are channelized to the sected channel; Filter: only events with the selected channel will be output"
              bgColOff="ff999999" textCol="ff000000" buttonText="Transform"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <VIEWPORT name="Piano Roll View" id="5a31f2945a04ad37" memberName="viewport"
            virtualName="PianoPort" explicitFocusOrder="0" pos="415 105 415M 105M"
            vscroll="1" hscroll="1" scrollbarThickness="16" contentType="2"
            jucerFile="" contentClass="PianoRoll" constructorParams="this-&gt;getFilter(), this, timeline.get()"/>
  <GENERICCOMPONENT name="" id="12ca6202713b19aa" memberName="resizer" virtualName=""
                    explicitFocusOrder="0" pos="16R 16R 16 16" class="ResizableCornerComponent"
                    params="this, &amp;resizeLimits"/>
  <TEXTBUTTON name="new button" id="d48ae418cc5b8c13" memberName="b_NoteToggle"
              virtualName="" explicitFocusOrder="0" pos="257 402 40 16" tooltip="When enabled, Note On events will toggle playback, ignoring Note Off events; otherwise Note Off will stop playback"
              bgColOff="ff999999" buttonText="Toggle" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <SLIDER name="TriggerChannel" id="19f62893eab0e18d" memberName="s_PlayGroup"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="312 136 64 20"
          tooltip="Slots with the same Play Group number will all start/stop at the same time"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="0.0" max="16.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Trigger Channel" id="57f66cb40cfc50a9" memberName="label13"
         virtualName="" explicitFocusOrder="0" pos="313 119 64 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Play Group" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="TriggerChannel" id="9796e75abea5bd57" memberName="s_MuteGroup"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="312 174 64 20"
          tooltip="Only one slot with the same Mute Group number can be played at the same time"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="0.0" max="16.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Trigger Channel" id="9a98f4a58163bef3" memberName="label14"
         virtualName="" explicitFocusOrder="0" pos="307 157 74 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Mute Group" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <TOGGLEBUTTON name="new toggle button" id="561e4cda4f522f3c" memberName="b_Snap"
                virtualName="" explicitFocusOrder="0" pos="392 61 59 24" tooltip="Toggle Snap to Grid"
                buttonText="Snap" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <COMBOBOX name="PR Quantize Step" id="85626cd2316c90f7" memberName="quantizeBox2"
            virtualName="" explicitFocusOrder="0" pos="453 64 50 18" tooltip="Grid Size"
            editable="0" layout="33" items="4th&#10;8th&#10;16th&#10;32nd&#10;64th"
            textWhenNonSelected="32nd" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="new toggle button" id="621ee9f53408a8a" memberName="b_ForceToKey"
                virtualName="" explicitFocusOrder="0" pos="11 312 99 17" tooltip="When checked, played notes will be fitted to the defined scale"
                txtcol="ffffffff" buttonText="Force to Scale" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="new component" id="3112d9375a206148" memberName="keySelector"
                    virtualName="KeySelector" explicitFocusOrder="0" pos="199 315 154 28"
                    class="MidiKeyboardComponent" params="ownerFilter-&gt;keySelectorState"/>
  <TEXTBUTTON name="new button" id="995ef86d3cb49c2f" memberName="b_ShiftUp"
              virtualName="" explicitFocusOrder="0" pos="353 314 21 30" tooltip="Shift selected notes one semitone up"
              bgColOff="ff999999" buttonText="&gt;" connectedEdges="1" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="11c526a253e1d0a2" memberName="b_ShiftDown"
              virtualName="" explicitFocusOrder="0" pos="178 314 21 30" tooltip="Shift selected notes one semitone down"
              bgColOff="ff999999" buttonText="&lt;" connectedEdges="2" needsCallback="1"
              radioGroupId="0"/>
  <TOGGLEBUTTON name="Single Loop" id="4a29216d10fb9de8" memberName="b_SingleLoop"
                virtualName="" explicitFocusOrder="0" pos="155 36 122 16" tooltip="When checked, switching from a playing slot to another slot will automatically play the new slot and stop the previous one"
                txtcol="ffffffff" buttonText="Play active slot only" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="VMasterVelocity" id="9d52f6a9f07e4534" memberName="s_MasterVelocity"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="371 36 72 16"
          tooltip="Global velocity adjustment applied to all played notes"
          bkgcol="1e707070" thumbcol="ff000000" textboxtext="ffffffff"
          min="0.0" max="200.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="2a2210fac0e009dd" memberName="label15" virtualName=""
         explicitFocusOrder="0" pos="283 36 88 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Master Velocity:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
         italic="0" justification="34"/>
  <IMAGEBUTTON name="new button" id="5007a7df8a92f35d" memberName="aboutButton"
               virtualName="" explicitFocusOrder="0" pos="9 1 136 47" tooltip="Insert Piz Here-&gt; midiLooper v1.3  http://thepiz.org/plugins/?p=midiLooper"
               buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
               keepProportions="1" resourceNormal="" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="1.0" colourOver="0" resourceDown=""
               opacityDown="1.0" colourDown="0"/>
  <TEXTBUTTON name="Triplet" id="7c0cd0b629c71353" memberName="b_Triplet" virtualName=""
              explicitFocusOrder="0" pos="506 64 18 18" tooltip="Toggle Triplet Note Grid"
              bgColOff="ff8d8d8d" buttonText="3" connectedEdges="3" needsCallback="1"
              radioGroupId="2"/>
  <TEXTBUTTON name="Dotted" id="d877509ff32890af" memberName="b_Dotted" virtualName=""
              explicitFocusOrder="0" pos="528 64 18 18" tooltip="Toggle Dotted Note Grid"
              bgColOff="ff8d8d8d" buttonText="." connectedEdges="3" needsCallback="1"
              radioGroupId="2"/>
  <TEXTBUTTON name="ZoomOut" id="a5a56a67b71805c8" memberName="b_ZoomOut" virtualName=""
              explicitFocusOrder="0" pos="656 64 18 18" tooltip="Zoom Out (Ctrl-click for vertical)"
              bgColOff="ffbbbbff" buttonText="-" connectedEdges="2" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="ZoomIn" id="457e768c161c96fb" memberName="b_ZoomIn" virtualName=""
              explicitFocusOrder="0" pos="674 64 18 18" tooltip="Zoom In (Ctrl-click for vertical)"
              bgColOff="ffbbbbff" buttonText="+" connectedEdges="1" needsCallback="1"
              radioGroupId="2"/>
  <LABEL name="new label" id="6d9c9863d2d35748" memberName="numerator"
         virtualName="" explicitFocusOrder="0" pos="555 64 27 18" tooltip="Time Sig Numerator"
         edTextCol="ff000000" edBkgCol="0" labelText="4" editableSingleClick="1"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="57902df5cc918e78" memberName="denominator"
         virtualName="" explicitFocusOrder="0" pos="584 64 29 18" tooltip="Time Sig Denominator"
         edTextCol="ff000000" edBkgCol="0" labelText="4" editableSingleClick="1"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="new toggle button" id="5543bd5e8c49a007" memberName="b_UseScaleChannel"
                virtualName="" explicitFocusOrder="0" pos="11 329 129 17" tooltip="When checked, input notes on &quot;Scale Ch&quot; will be used to define the scale"
                txtcol="ffffffff" buttonText="Use Scale Channel" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="ScaleChannel" id="ea379f0c4e3dd4ec" memberName="s_ScaleChannel"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="10 283 60 20"
          tooltip="Input notes on this channel will affect Semitones, Octave, and/or Force to Scale settings where &quot;Use Scale Ch&quot; is enabled"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="1.0" max="16.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="scale ch" id="93cfb6ed66e0a17c" memberName="label25" virtualName=""
         explicitFocusOrder="0" pos="13 267 54 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Scale Ch" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="MasterTranspose" id="d57f221f805d0cab" memberName="s_MasterTranspose"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="554 36 72 16"
          tooltip="Global transposition applied to all played notes (after Force to Scale)"
          bkgcol="1e707070" thumbcol="ff000000" textboxtext="ffffffff"
          min="-12.0" max="12.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="d40dfc76a0032690" memberName="label26" virtualName=""
         explicitFocusOrder="0" pos="452 36 102 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Master Transpose:"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
         italic="0" justification="34"/>
  <TOGGLEBUTTON name="WaitForBar" id="9ab8ef37e97d43e0" memberName="b_WaitForBar"
                virtualName="" explicitFocusOrder="0" pos="175 123 107 16" tooltip="When checked, play/stop of this slot will happen at the start of the bar after"
                txtcol="ffffffff" buttonText="Wait for Next Bar" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="midiOutDevice" id="6ab1b2012b5d6135" memberName="midiOutDeviceBox"
            virtualName="" explicitFocusOrder="0" pos="633 15 158 16" tooltip="Send ouput to selected MIDI port in addition to VST host output"
            editable="0" layout="33" items="" textWhenNonSelected="--" textWhenNoItems="(no choices)"/>
  <LABEL name="QuantizeLabel" id="3339c761cb236777" memberName="label27"
         virtualName="" explicitFocusOrder="0" pos="627 0 117 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="MIDI Output Device"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <TOGGLEBUTTON name="new toggle button" id="4860c217b9607fce" memberName="b_UseTrChannel"
                virtualName="" explicitFocusOrder="0" pos="148 252 130 17" tooltip="When checked, notes on selected &quot;Transpose Ch&quot; will apply to &quot;Semitones&quot; and &quot;Octave&quot; settings, relative to &quot;Root Note&quot;"
                txtcol="ffffffff" buttonText="Use Transp Ch" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="85d95ffb3ae3028a" memberName="b_ImmediateTranspose"
                virtualName="" explicitFocusOrder="0" pos="247 252 48 17" tooltip="When checked, playing notes will be split and transposed immediately on changes to Semitones / Octave / Force to Scale / Master Transpose settings"
                txtcol="ffffffff" buttonText="Split" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <SLIDER name="NumLoops" id="6ce62dba23a224a8" memberName="s_NumLoops"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="155 174 64 20"
          tooltip="Number of times to loop playback" bkgcol="1e000000"
          thumbcol="ff000000" textboxtext="ffffffff" min="0.0" max="64.0"
          int="1.0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="NextSlot" id="89b8c16e6410c4dc" memberName="s_NextSlot"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="234 174 64 20"
          tooltip="What to do after the selected number of loops have played"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="0.0" max="16.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="Sync:" id="3505240ec530568f" memberName="label16" virtualName=""
         explicitFocusOrder="0" pos="12 247 125 16" textCol="ff9f9f9f"
         edTextCol="ff000000" edBkgCol="0" labelText="Loop Settings" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="1" italic="0" justification="36"
         typefaceStyle="Bold"/>
  <COMBOBOX name="Force To Scale Mode" id="43ff2ca0052c54b0" memberName="forceModeBox"
            virtualName="" explicitFocusOrder="0" pos="110 313 61 16" editable="0"
            layout="33" items="Nearest&#10;Up&#10;Down&#10;Block" textWhenNonSelected="Nearest"
            textWhenNoItems="(no choices)"/>
  <VIEWPORT name="Keyboard View" id="cd5cf3f9d86959b2" memberName="kbport"
            virtualName="" explicitFocusOrder="0" pos="390 105 25 121M" vscroll="0"
            hscroll="0" scrollbarThickness="16" contentType="2" jucerFile=""
            contentClass="MidiKeyboardComponent" constructorParams="ownerFilter-&gt;kbstate,MidiKeyboardComponent::verticalKeyboardFacingRight"/>
  <TEXTBUTTON name="RemoveBar" id="d6d493284d7125c9" memberName="b_RemoveBar"
              virtualName="" explicitFocusOrder="0" pos="706 64 18 18" tooltip="Remove bar"
              bgColOff="ffbbbbff" buttonText="-" connectedEdges="2" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="AddBar" id="d6e74e96a2942be5" memberName="b_AddBar" virtualName=""
              explicitFocusOrder="0" pos="773 64 18 18" tooltip="Add bar" bgColOff="ffbbbbff"
              buttonText="+" connectedEdges="1" needsCallback="1" radioGroupId="2"/>
  <LABEL name="Length" id="1b0d10758ceeb2c6" memberName="LengthLabel"
         virtualName="" explicitFocusOrder="0" pos="724 65 49 16" tooltip="Pattern length in bars"
         edTextCol="ff000000" edBkgCol="0" labelText="4" editableSingleClick="1"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="new button" id="f82b62c14f5e0654" memberName="textButton17"
              virtualName="" explicitFocusOrder="0" pos="4 -97 25 24" buttonText="1"
              connectedEdges="10" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="f3d9e6aa2250a016" memberName="textButton18"
              virtualName="" explicitFocusOrder="0" pos="29 -97 23 24" bgColOff="ffbbbbff"
              buttonText="2" connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="fd1b907d561151ac" memberName="textButton19"
              virtualName="" explicitFocusOrder="0" pos="52 -97 23 24" buttonText="3"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="bebf9067f8e7ead5" memberName="textButton20"
              virtualName="" explicitFocusOrder="0" pos="75 -97 23 24" buttonText="4"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="2e9b4196bc044ad4" memberName="textButton21"
              virtualName="" explicitFocusOrder="0" pos="98 -97 23 24" buttonText="5"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="143b513b7883582f" memberName="textButton22"
              virtualName="" explicitFocusOrder="0" pos="121 -97 23 24" buttonText="6"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="26aea4d0d192007d" memberName="textButton23"
              virtualName="" explicitFocusOrder="0" pos="144 -97 23 24" buttonText="7"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="34b98f50c0125adb" memberName="textButton24"
              virtualName="" explicitFocusOrder="0" pos="167 -97 23 24" buttonText="8"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="b346d14758db1eb8" memberName="textButton25"
              virtualName="" explicitFocusOrder="0" pos="190 -97 23 24" buttonText="9"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="4a7d45e14d6faa20" memberName="textButton26"
              virtualName="" explicitFocusOrder="0" pos="213 -97 25 24" buttonText="10"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="d7828f6b0aa51da9" memberName="textButton27"
              virtualName="" explicitFocusOrder="0" pos="238 -97 25 24" buttonText="11"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="a68c9fd9483e71ab" memberName="textButton28"
              virtualName="" explicitFocusOrder="0" pos="263 -97 25 24" buttonText="12"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="54442e4cee712779" memberName="textButton29"
              virtualName="" explicitFocusOrder="0" pos="288 -97 25 24" buttonText="13"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="69fa18b953ca2aa3" memberName="textButton30"
              virtualName="" explicitFocusOrder="0" pos="313 -97 25 24" buttonText="14"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="97f8e917a546e991" memberName="textButton31"
              virtualName="" explicitFocusOrder="0" pos="338 -97 25 24" buttonText="15"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="8a2447be2be271fa" memberName="textButton32"
              virtualName="" explicitFocusOrder="0" pos="363 -97 28 24" buttonText="16"
              connectedEdges="9" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="6b84b942866d7006" memberName="textButton33"
              virtualName="" explicitFocusOrder="0" pos="1 -73 25 24" buttonText="1"
              connectedEdges="10" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="9a7ceea1009a124c" memberName="textButton34"
              virtualName="" explicitFocusOrder="0" pos="26 -73 23 24" bgColOff="ffbbbbff"
              buttonText="2" connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="c5487a62fc00a54f" memberName="textButton35"
              virtualName="" explicitFocusOrder="0" pos="49 -73 23 24" buttonText="3"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="70e2d1815eef6c38" memberName="textButton36"
              virtualName="" explicitFocusOrder="0" pos="72 -73 23 24" buttonText="4"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="96733ae0ee7930aa" memberName="textButton37"
              virtualName="" explicitFocusOrder="0" pos="95 -73 23 24" buttonText="5"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="2a2a7d5a20dc3625" memberName="textButton38"
              virtualName="" explicitFocusOrder="0" pos="118 -73 23 24" buttonText="6"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="45f8294bb289e286" memberName="textButton39"
              virtualName="" explicitFocusOrder="0" pos="141 -73 23 24" buttonText="7"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="b2210d75a50bd16b" memberName="textButton40"
              virtualName="" explicitFocusOrder="0" pos="164 -73 23 24" buttonText="8"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="9041ed5d69c6b185" memberName="textButton41"
              virtualName="" explicitFocusOrder="0" pos="187 -73 23 24" buttonText="9"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="a3a81f971178361a" memberName="textButton42"
              virtualName="" explicitFocusOrder="0" pos="210 -73 25 24" buttonText="10"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="485564a3931a644e" memberName="textButton43"
              virtualName="" explicitFocusOrder="0" pos="235 -73 25 24" buttonText="11"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="e8d39b40ebcee2e6" memberName="textButton44"
              virtualName="" explicitFocusOrder="0" pos="260 -73 25 24" buttonText="12"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="b7510563090c84a1" memberName="textButton45"
              virtualName="" explicitFocusOrder="0" pos="285 -73 25 24" buttonText="13"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="458629325720585f" memberName="textButton46"
              virtualName="" explicitFocusOrder="0" pos="310 -73 25 24" buttonText="14"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="34c50bb9ed0aefab" memberName="textButton47"
              virtualName="" explicitFocusOrder="0" pos="335 -73 25 24" buttonText="15"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="82df1f30a42f9364" memberName="textButton48"
              virtualName="" explicitFocusOrder="0" pos="360 -73 28 24" buttonText="16"
              connectedEdges="9" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="bdb4e4075d1a3ad9" memberName="textButton49"
              virtualName="" explicitFocusOrder="0" pos="-2 -40 25 24" buttonText="1"
              connectedEdges="10" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="f3240d7ed7104a74" memberName="textButton50"
              virtualName="" explicitFocusOrder="0" pos="23 -40 23 24" bgColOff="ffbbbbff"
              buttonText="2" connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="14830fd1fb697497" memberName="textButton51"
              virtualName="" explicitFocusOrder="0" pos="46 -40 23 24" buttonText="3"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="3da094304d5f9224" memberName="textButton52"
              virtualName="" explicitFocusOrder="0" pos="69 -40 23 24" buttonText="4"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="b9e3cc78442695a4" memberName="textButton53"
              virtualName="" explicitFocusOrder="0" pos="92 -40 23 24" buttonText="5"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="8a34390909e301d6" memberName="textButton54"
              virtualName="" explicitFocusOrder="0" pos="115 -40 23 24" buttonText="6"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="2c203a79878e21a3" memberName="textButton55"
              virtualName="" explicitFocusOrder="0" pos="138 -40 23 24" buttonText="7"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="9d1bb945d1cc8484" memberName="textButton56"
              virtualName="" explicitFocusOrder="0" pos="161 -40 23 24" buttonText="8"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="e8dbddea1caecd8e" memberName="textButton57"
              virtualName="" explicitFocusOrder="0" pos="184 -40 23 24" buttonText="9"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="e4a29e1374a980cf" memberName="textButton58"
              virtualName="" explicitFocusOrder="0" pos="207 -40 25 24" buttonText="10"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="919b57bdbbe979a1" memberName="textButton59"
              virtualName="" explicitFocusOrder="0" pos="232 -40 25 24" buttonText="11"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="121e17b3623e612" memberName="textButton60"
              virtualName="" explicitFocusOrder="0" pos="257 -40 25 24" buttonText="12"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="cb26ef4661225699" memberName="textButton61"
              virtualName="" explicitFocusOrder="0" pos="282 -40 25 24" buttonText="13"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="9f497d7c031e76b4" memberName="textButton62"
              virtualName="" explicitFocusOrder="0" pos="307 -40 25 24" buttonText="14"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="a2cdaffa1ec2aa1e" memberName="textButton63"
              virtualName="" explicitFocusOrder="0" pos="332 -40 25 24" buttonText="15"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="995149744a9ae3bd" memberName="textButton64"
              virtualName="" explicitFocusOrder="0" pos="357 -40 28 24" buttonText="16"
              connectedEdges="9" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="d68fc9c452ad8dab" memberName="textButton65"
              virtualName="" explicitFocusOrder="0" pos="383 -93 25 24" buttonText="1"
              connectedEdges="10" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="7ab5cfcd21440999" memberName="textButton66"
              virtualName="" explicitFocusOrder="0" pos="408 -93 23 24" bgColOff="ffbbbbff"
              buttonText="2" connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="7bbd5b301abfe86" memberName="textButton67"
              virtualName="" explicitFocusOrder="0" pos="431 -93 23 24" buttonText="3"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="887951f325b67723" memberName="textButton68"
              virtualName="" explicitFocusOrder="0" pos="454 -93 23 24" buttonText="4"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="3083792c1007132" memberName="textButton69"
              virtualName="" explicitFocusOrder="0" pos="477 -93 23 24" buttonText="5"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="d55a4c9b0993c208" memberName="textButton70"
              virtualName="" explicitFocusOrder="0" pos="500 -93 23 24" buttonText="6"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="6496ab215addfa03" memberName="textButton71"
              virtualName="" explicitFocusOrder="0" pos="523 -93 23 24" buttonText="7"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="3699bb2c06c2192b" memberName="textButton72"
              virtualName="" explicitFocusOrder="0" pos="546 -93 23 24" buttonText="8"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="17b1de3153b62496" memberName="textButton73"
              virtualName="" explicitFocusOrder="0" pos="569 -93 23 24" buttonText="9"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="2e90990a96674b54" memberName="textButton74"
              virtualName="" explicitFocusOrder="0" pos="592 -93 25 24" buttonText="10"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="5214578a2d0a1ade" memberName="textButton75"
              virtualName="" explicitFocusOrder="0" pos="617 -93 25 24" buttonText="11"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="a7497d1bbbb89667" memberName="textButton76"
              virtualName="" explicitFocusOrder="0" pos="642 -93 25 24" buttonText="12"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="3e7c680e763ea97d" memberName="textButton77"
              virtualName="" explicitFocusOrder="0" pos="667 -93 25 24" buttonText="13"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="6e19e986e29bb5d8" memberName="textButton78"
              virtualName="" explicitFocusOrder="0" pos="692 -93 25 24" buttonText="14"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="652b8a1e62bbb554" memberName="textButton79"
              virtualName="" explicitFocusOrder="0" pos="717 -93 25 24" buttonText="15"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="1f1a53bdd4a5e45e" memberName="textButton80"
              virtualName="" explicitFocusOrder="0" pos="742 -93 28 24" buttonText="16"
              connectedEdges="9" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="8fc828bd969cc4e6" memberName="textButton81"
              virtualName="" explicitFocusOrder="0" pos="371 -76 25 24" buttonText="1"
              connectedEdges="10" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="3e4c5bf034830bb7" memberName="textButton82"
              virtualName="" explicitFocusOrder="0" pos="396 -76 23 24" bgColOff="ffbbbbff"
              buttonText="2" connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="5b0d51946a6866ca" memberName="textButton83"
              virtualName="" explicitFocusOrder="0" pos="419 -76 23 24" buttonText="3"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="ed27c1f0c2d13d02" memberName="textButton84"
              virtualName="" explicitFocusOrder="0" pos="442 -76 23 24" buttonText="4"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="1a9cfc23a4a4d942" memberName="textButton85"
              virtualName="" explicitFocusOrder="0" pos="465 -76 23 24" buttonText="5"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="4b46287ace557cec" memberName="textButton86"
              virtualName="" explicitFocusOrder="0" pos="488 -76 23 24" buttonText="6"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="424dac2524f5021a" memberName="textButton87"
              virtualName="" explicitFocusOrder="0" pos="511 -76 23 24" buttonText="7"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="ed5a5b0454cd810c" memberName="textButton88"
              virtualName="" explicitFocusOrder="0" pos="534 -76 23 24" buttonText="8"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="e614d86fa41faba5" memberName="textButton89"
              virtualName="" explicitFocusOrder="0" pos="557 -76 23 24" buttonText="9"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="51dff617965502ee" memberName="textButton90"
              virtualName="" explicitFocusOrder="0" pos="580 -76 25 24" buttonText="10"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="e2c898797e1afdf1" memberName="textButton91"
              virtualName="" explicitFocusOrder="0" pos="605 -76 25 24" buttonText="11"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="ad2ba786a567e6e" memberName="textButton92"
              virtualName="" explicitFocusOrder="0" pos="630 -76 25 24" buttonText="12"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="a110261c5fb9d73b" memberName="textButton93"
              virtualName="" explicitFocusOrder="0" pos="655 -76 25 24" buttonText="13"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="90e56e88fc990149" memberName="textButton94"
              virtualName="" explicitFocusOrder="0" pos="680 -76 25 24" buttonText="14"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="b134379d42f1b7a5" memberName="textButton95"
              virtualName="" explicitFocusOrder="0" pos="705 -76 25 24" buttonText="15"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="e0faf1edf4a2dff0" memberName="textButton96"
              virtualName="" explicitFocusOrder="0" pos="730 -76 28 24" buttonText="16"
              connectedEdges="9" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="7baa677054d263a0" memberName="textButton97"
              virtualName="" explicitFocusOrder="0" pos="413 -59 25 24" buttonText="1"
              connectedEdges="10" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="c5579322b15a24c0" memberName="textButton98"
              virtualName="" explicitFocusOrder="0" pos="438 -59 23 24" bgColOff="ffbbbbff"
              buttonText="2" connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="97c19928ab0ac5ae" memberName="textButton99"
              virtualName="" explicitFocusOrder="0" pos="461 -59 23 24" buttonText="3"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="4c9fc55a92ea01f8" memberName="textButton100"
              virtualName="" explicitFocusOrder="0" pos="484 -59 23 24" buttonText="4"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="d246fad8bbe50c27" memberName="textButton101"
              virtualName="" explicitFocusOrder="0" pos="507 -59 23 24" buttonText="5"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="53a5a99d123e2583" memberName="textButton102"
              virtualName="" explicitFocusOrder="0" pos="530 -59 23 24" buttonText="6"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="82aeee95b8399b98" memberName="textButton103"
              virtualName="" explicitFocusOrder="0" pos="553 -59 23 24" buttonText="7"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="c467d3344ea4e23d" memberName="textButton104"
              virtualName="" explicitFocusOrder="0" pos="576 -59 23 24" buttonText="8"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="1f0e2fac74d09229" memberName="textButton105"
              virtualName="" explicitFocusOrder="0" pos="599 -59 23 24" buttonText="9"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="f57d8b285c77a75e" memberName="textButton106"
              virtualName="" explicitFocusOrder="0" pos="622 -59 25 24" buttonText="10"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="f44d87c182543ff0" memberName="textButton107"
              virtualName="" explicitFocusOrder="0" pos="647 -59 25 24" buttonText="11"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="79df8405bfc1a2ef" memberName="textButton108"
              virtualName="" explicitFocusOrder="0" pos="672 -59 25 24" buttonText="12"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="7001323b79825083" memberName="textButton109"
              virtualName="" explicitFocusOrder="0" pos="697 -59 25 24" buttonText="13"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="69a185df3f899779" memberName="textButton110"
              virtualName="" explicitFocusOrder="0" pos="722 -59 25 24" buttonText="14"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="86c891df3a4d79cf" memberName="textButton111"
              virtualName="" explicitFocusOrder="0" pos="747 -59 25 24" buttonText="15"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="89cc5699baf6d900" memberName="textButton112"
              virtualName="" explicitFocusOrder="0" pos="772 -59 28 24" buttonText="16"
              connectedEdges="9" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="56b76264f094a52f" memberName="textButton113"
              virtualName="" explicitFocusOrder="0" pos="408 -29 25 24" buttonText="1"
              connectedEdges="10" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="94e1a2446be2bf9f" memberName="textButton114"
              virtualName="" explicitFocusOrder="0" pos="433 -29 23 24" bgColOff="ffbbbbff"
              buttonText="2" connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="4c35e8860c764ad4" memberName="textButton115"
              virtualName="" explicitFocusOrder="0" pos="456 -29 23 24" buttonText="3"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="2bc14fc280f03a9b" memberName="textButton116"
              virtualName="" explicitFocusOrder="0" pos="479 -29 23 24" buttonText="4"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="41e3b05c3731d48d" memberName="textButton117"
              virtualName="" explicitFocusOrder="0" pos="502 -29 23 24" buttonText="5"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="20a016ecbcfb4da2" memberName="textButton118"
              virtualName="" explicitFocusOrder="0" pos="525 -29 23 24" buttonText="6"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="fc198aa11cce2ac3" memberName="textButton119"
              virtualName="" explicitFocusOrder="0" pos="548 -29 23 24" buttonText="7"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="31bc5acefe18d077" memberName="textButton120"
              virtualName="" explicitFocusOrder="0" pos="571 -29 23 24" buttonText="8"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="5bd222f0831ce0d8" memberName="textButton121"
              virtualName="" explicitFocusOrder="0" pos="594 -29 23 24" buttonText="9"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="ef5c06f268638c6a" memberName="textButton122"
              virtualName="" explicitFocusOrder="0" pos="617 -29 25 24" buttonText="10"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="d54be58709c7219c" memberName="textButton123"
              virtualName="" explicitFocusOrder="0" pos="642 -29 25 24" buttonText="11"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="b0ad74bfe805bfeb" memberName="textButton124"
              virtualName="" explicitFocusOrder="0" pos="667 -29 25 24" buttonText="12"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="febcfd4d46d86f04" memberName="textButton125"
              virtualName="" explicitFocusOrder="0" pos="692 -29 25 24" buttonText="13"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="3506c31bc51ec75f" memberName="textButton126"
              virtualName="" explicitFocusOrder="0" pos="717 -29 25 24" buttonText="14"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="5a3aae16c809291d" memberName="textButton127"
              virtualName="" explicitFocusOrder="0" pos="742 -29 25 24" buttonText="15"
              connectedEdges="11" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="26d084584019ff36" memberName="textButton128"
              virtualName="" explicitFocusOrder="0" pos="767 -29 28 24" buttonText="16"
              connectedEdges="9" needsCallback="0" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="882925b8a07e236f" memberName="b_Transpose10"
              virtualName="" explicitFocusOrder="0" pos="306 252 72 13" bgColOff="ff999999"
              buttonText="transpose channel 10" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TOGGLEBUTTON name="OverdubMode" id="deb54e2d772c8ee4" memberName="b_KeepLength"
                virtualName="" explicitFocusOrder="0" pos="520 -2 83 16" tooltip="When checked, overdubbing will loop record into existing loop length"
                txtcol="ffffffff" buttonText="Keep Length" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="recCC" id="a135262705472738" memberName="s_RecCC" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="11 149 64 20" tooltip="CC Number to toggle recoring to active slot"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="-2.0" max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="playCC" id="742efa24bd3cf708" memberName="s_PlayCC" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="81 149 64 20" tooltip="CC Number to toggle play for active slot"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="-2.0" max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="Velocity Sensitivity" id="56077b1f3eddabf4" memberName="s_VelocitySens"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="234 440 64 20"
          tooltip="Velocity Sensitivity (Input Velocity -&gt; Output Velocity)"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="0.0" max="200.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="56dd185855bc67b0" memberName="label24" virtualName=""
         explicitFocusOrder="0" pos="225 424 80 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="VeloSens" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="MIDI_Monitor" id="9b82f84d650b5d48" memberName="b_Monitor"
              virtualName="" explicitFocusOrder="0" pos="678 33 55 20" tooltip="Monitor input MIDI through active slot's settings (Transpose, Scale, I/O Channel)"
              bgColOff="ff999999" buttonText="Monitor" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <SLIDER name="TransposeChannel" id="b0b2857ba8bcb082" memberName="s_TransposeChannel"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="76 283 60 20"
          tooltip="Input notes on this channel will affect Semitones, Octave, and/or Force to Scale settings where &quot;Use Transp Ch&quot; is enabled"
          bkgcol="1e000000" thumbcol="ff000000" textboxtext="ffffffff"
          min="1.0" max="16.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="tr ch" id="63fe8f3eb653b053" memberName="label28" virtualName=""
         explicitFocusOrder="0" pos="66 267 80 16" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Transpose Ch" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: piznew40_png, 1803, "../../../My Pictures/piznew40.png"
static const unsigned char resource_PizLooperEditor_piznew40_png[] = { 137, 80, 78, 71, 13, 10, 26, 10, 0, 0, 0, 13, 73, 72, 68, 82, 0, 0, 0, 40, 0, 0, 0, 24, 8, 6, 0, 0, 1, 255, 131, 143, 86, 0, 0, 0, 9, 112, 72, 89, 115, 0, 0, 46, 35, 0, 0, 46, 35, 1, 120, 165, 63, 118, 0, 0, 0, 13, 116, 69, 88, 116, 67, 111, 109, 109, 101, 110, 116, 0, 104, 195, 123, 3, 0, 168, 254, 86, 169, 0, 0, 6, 164, 73, 68, 65, 84, 120, 156, 181, 87, 9, 84, 205, 105, 20, 255, 215, 123, 175, 69, 74, 90, 212, 107, 85, 74, 76, 26, 137, 142, 112, 44, 169, 168, 145, 37, 154, 49, 24, 145, 61, 164, 178, 150, 38, 106, 210, 12, 141, 148, 16, 102, 70, 165, 33, 45, 104, 179, 205, 88, 143, 194, 88, 178, 85, 42, 107, 171, 147, 97, 178, 68, 141, 234, 185, 115, 239, 247, 122, 233, 165, 158, 154, 49, 223, 57, 247, 252, 255, 255, 239, 187, 223, 125, 247, 254, 238, 250, 56, 78, 198, 50, 46, 187, 186, 7, 158, 220, 78, 0, 124, 183, 102, 59, 187, 54, 121, 137, 254, 126, 116, 16, 158, 23, 38, 130, 185, 137, 94, 14, 103, 209, 75, 63, 181, 229, 21, 171, 62, 198, 39, 217, 203, 153, 212, 13, 34, 226, 34, 110, 201, 161, 160, 232, 124, 12, 187, 138, 239, 158, 146, 205, 37, 65, 190, 83, 33, 35, 46, 144, 54, 5, 180, 145, 182, 39, 128, 49, 109, 94, 231, 9, 163, 135, 89, 85, 69, 133, 204, 107, 150, 192, 225, 71, 253, 211, 252, 95, 25, 131, 132, 94, 223, 75, 126, 207, 32, 39, 39, 183, 6, 42, 51, 222, 111, 112, 156, 214, 189, 11, 187, 0, 247, 67, 154, 190, 93, 107, 31, 164, 178, 115, 99, 89, 198, 211, 10, 243, 159, 41, 226, 174, 30, 143, 0, 168, 202, 2, 117, 53, 149, 159, 36, 7, 175, 239, 167, 52, 91, 38, 42, 79, 135, 105, 147, 134, 23, 52, 223, 10, 240, 118, 175, 190, 122, 44, 2, 136, 233, 69, 209, 1, 98, 130, 33, 3, 45, 178, 62, 16, 255, 234, 110, 146, 148, 33, 77, 208, 104, 75, 49, 233, 233, 104, 164, 244, 54, 213, 99, 82, 36, 248, 229, 159, 221, 14, 229, 185, 177, 239, 141, 12, 93, 61, 163, 14, 31, 250, 45, 238, 221, 122, 116, 249, 231, 102, 134, 132, 173, 190, 240, 174, 9, 21, 7, 130, 163, 246, 33, 131, 96, 169, 12, 227, 205, 23, 205, 116, 174, 28, 239, 100, 91, 136, 239, 30, 173, 206, 150, 92, 202, 10, 151, 168, 43, 181, 44, 209, 208, 27, 215, 127, 139, 132, 210, 43, 191, 208, 161, 29, 109, 234, 106, 171, 103, 19, 90, 254, 75, 167, 64, 13, 58, 139, 16, 173, 198, 203, 10, 10, 252, 92, 248, 243, 40, 12, 232, 103, 122, 183, 93, 53, 188, 60, 92, 158, 183, 6, 169, 37, 85, 223, 73, 132, 191, 10, 246, 193, 171, 226, 36, 48, 212, 211, 138, 147, 97, 81, 243, 154, 129, 102, 193, 53, 212, 34, 255, 204, 54, 120, 89, 252, 161, 23, 200, 109, 67, 7, 245, 41, 105, 243, 182, 130, 128, 159, 17, 187, 101, 25, 16, 142, 244, 171, 104, 86, 157, 128, 207, 91, 142, 71, 93, 145, 158, 52, 101, 12, 212, 61, 20, 39, 202, 219, 146, 67, 244, 61, 75, 173, 171, 114, 88, 90, 236, 90, 104, 40, 75, 131, 83, 201, 223, 209, 158, 191, 102, 119, 213, 13, 205, 130, 117, 180, 213, 55, 119, 68, 253, 118, 150, 254, 245, 223, 35, 49, 206, 185, 0, 177, 48, 45, 117, 194, 98, 194, 127, 16, 200, 21, 98, 188, 225, 99, 0, 103, 109, 105, 114, 167, 14, 61, 136, 33, 81, 131, 27, 42, 237, 93, 208, 237, 209, 61, 115, 205, 146, 201, 85, 6, 122, 90, 137, 156, 116, 36, 11, 28, 134, 247, 175, 120, 131, 41, 177, 59, 124, 49, 112, 211, 221, 70, 214, 44, 242, 112, 110, 164, 48, 161, 144, 112, 119, 29, 86, 133, 76, 163, 90, 10, 227, 201, 203, 7, 195, 227, 76, 134, 33, 57, 137, 242, 73, 32, 224, 71, 224, 209, 196, 188, 211, 209, 12, 223, 207, 63, 235, 121, 19, 195, 231, 15, 41, 45, 208, 25, 235, 191, 15, 152, 217, 72, 133, 97, 222, 116, 39, 73, 144, 26, 220, 60, 25, 5, 83, 198, 13, 5, 178, 228, 64, 204, 74, 76, 163, 109, 144, 135, 17, 16, 23, 185, 172, 254, 100, 18, 115, 200, 152, 118, 177, 192, 60, 204, 74, 217, 189, 186, 225, 98, 102, 248, 187, 188, 51, 209, 172, 234, 72, 66, 133, 200, 115, 170, 3, 172, 243, 155, 202, 52, 69, 246, 209, 31, 195, 86, 183, 17, 195, 64, 86, 96, 83, 117, 200, 78, 251, 1, 42, 175, 199, 125, 144, 102, 109, 173, 190, 245, 165, 135, 160, 224, 220, 118, 153, 66, 95, 20, 37, 130, 211, 8, 235, 199, 29, 17, 200, 161, 57, 111, 57, 10, 100, 196, 235, 214, 233, 173, 237, 10, 229, 196, 129, 47, 115, 77, 152, 49, 121, 100, 101, 66, 180, 95, 131, 190, 174, 38, 236, 141, 242, 5, 143, 47, 237, 153, 137, 207, 48, 119, 155, 43, 57, 122, 24, 147, 32, 43, 33, 218, 23, 126, 12, 242, 36, 193, 46, 82, 82, 130, 87, 76, 3, 239, 57, 227, 68, 84, 225, 207, 29, 12, 131, 254, 150, 38, 23, 56, 113, 156, 133, 142, 115, 28, 68, 149, 132, 121, 86, 226, 24, 140, 69, 134, 29, 254, 216, 83, 74, 203, 134, 178, 195, 48, 106, 168, 85, 137, 178, 146, 194, 106, 220, 118, 224, 236, 108, 44, 242, 41, 6, 23, 124, 51, 246, 73, 59, 154, 47, 38, 33, 164, 169, 247, 28, 215, 70, 252, 150, 107, 113, 198, 51, 51, 17, 94, 162, 51, 10, 236, 240, 111, 103, 139, 216, 174, 188, 188, 220, 2, 124, 76, 234, 16, 200, 109, 172, 73, 206, 118, 207, 176, 245, 212, 183, 220, 179, 178, 177, 234, 245, 32, 247, 196, 22, 102, 26, 150, 166, 130, 246, 46, 119, 98, 169, 35, 105, 34, 41, 118, 226, 142, 145, 155, 139, 221, 67, 106, 135, 199, 247, 175, 7, 52, 116, 19, 165, 233, 33, 250, 32, 159, 212, 62, 72, 97, 209, 66, 216, 199, 71, 249, 128, 173, 181, 121, 30, 94, 242, 70, 18, 118, 84, 41, 115, 19, 225, 237, 91, 167, 182, 50, 95, 81, 233, 164, 110, 93, 118, 109, 15, 96, 242, 81, 232, 150, 33, 143, 15, 146, 124, 139, 59, 125, 133, 61, 186, 159, 194, 217, 132, 185, 169, 161, 244, 48, 208, 136, 208, 88, 158, 198, 178, 91, 194, 52, 140, 207, 147, 95, 136, 207, 193, 72, 99, 12, 132, 154, 153, 62, 243, 198, 215, 230, 100, 108, 100, 141, 156, 234, 76, 113, 206, 78, 140, 186, 217, 48, 120, 64, 239, 92, 129, 128, 183, 12, 249, 186, 180, 82, 206, 108, 226, 216, 193, 213, 245, 248, 3, 235, 151, 127, 13, 145, 193, 115, 225, 13, 26, 76, 30, 161, 39, 213, 49, 138, 96, 250, 97, 191, 249, 19, 26, 144, 127, 149, 135, 187, 125, 45, 117, 39, 81, 69, 58, 60, 190, 17, 15, 115, 167, 57, 82, 74, 81, 137, 86, 224, 196, 3, 143, 60, 215, 129, 69, 76, 212, 75, 147, 76, 141, 117, 43, 13, 132, 90, 21, 170, 42, 202, 231, 21, 21, 248, 43, 187, 170, 40, 173, 212, 23, 106, 198, 186, 58, 218, 86, 230, 164, 111, 100, 104, 137, 219, 226, 126, 70, 146, 6, 132, 19, 2, 24, 234, 107, 1, 14, 108, 80, 148, 29, 3, 47, 209, 67, 230, 166, 122, 231, 93, 157, 108, 171, 16, 136, 58, 37, 69, 193, 14, 148, 175, 211, 65, 47, 73, 43, 71, 57, 75, 19, 15, 89, 94, 135, 147, 5, 101, 3, 33, 65, 53, 146, 80, 105, 171, 197, 74, 136, 13, 4, 120, 215, 197, 222, 6, 209, 113, 130, 17, 131, 45, 161, 6, 221, 72, 149, 202, 198, 202, 52, 253, 223, 40, 212, 122, 245, 221, 17, 182, 240, 173, 100, 110, 173, 185, 155, 204, 132, 203, 42, 147, 237, 149, 78, 138, 235, 215, 247, 147, 89, 235, 161, 57, 21, 99, 127, 241, 167, 80, 176, 219, 200, 33, 253, 74, 73, 65, 106, 67, 136, 38, 67, 132, 144, 33, 101, 137, 58, 163, 40, 41, 151, 25, 31, 8, 60, 158, 124, 208, 167, 80, 142, 150, 6, 146, 13, 210, 81, 36, 140, 35, 109, 246, 183, 160, 252, 90, 44, 24, 225, 251, 228, 47, 134, 48, 183, 139, 209, 77, 146, 137, 46, 197, 98, 116, 232, 124, 202, 74, 63, 45, 13, 181, 221, 152, 185, 123, 241, 221, 25, 137, 255, 49, 37, 148, 56, 113, 131, 118, 66, 178, 55, 49, 210, 201, 142, 12, 153, 11, 37, 88, 189, 223, 97, 118, 81, 77, 34, 4, 119, 110, 244, 162, 41, 225, 44, 206, 130, 151, 41, 216, 73, 225, 64, 159, 175, 240, 252, 8, 36, 198, 172, 0, 109, 77, 53, 56, 157, 18, 202, 98, 180, 165, 98, 47, 139, 15, 48, 194, 56, 172, 198, 59, 102, 38, 134, 58, 169, 148, 64, 20, 211, 84, 138, 42, 112, 222, 94, 229, 229, 38, 82, 82, 84, 216, 197, 181, 85, 206, 122, 26, 244, 0, 26, 152, 224, 233, 81, 160, 18, 65, 136, 156, 197, 30, 101, 55, 208, 162, 16, 93, 225, 143, 44, 150, 156, 116, 251, 144, 44, 67, 164, 16, 179, 158, 194, 18, 62, 143, 199, 20, 62, 145, 24, 204, 100, 16, 146, 228, 126, 66, 109, 83, 224, 44, 250, 23, 179, 161, 213, 93, 154, 249, 23, 244, 179, 48, 186, 72, 231, 84, 39, 169, 14, 147, 65, 71, 18, 130, 224, 202, 177, 205, 12, 32, 231, 81, 54, 229, 146, 11, 202, 72, 110, 254, 75, 221, 177, 72, 238, 3, 103, 123, 155, 251, 29, 129, 190, 213, 34, 25, 174, 26, 234, 170, 59, 177, 192, 103, 161, 11, 227, 241, 219, 177, 131, 119, 201, 139, 17, 164, 28, 33, 75, 255, 67, 176, 247, 190, 194, 57, 120, 71, 39, 117, 248, 223, 215, 218, 110, 170, 93, 104, 176, 220, 194, 53, 53, 130, 127, 0, 217, 46, 21, 116, 181, 241, 235, 130, 0, 0, 0, 0, 73, 69, 78, 68, 174, 66, 96, 130, 0, 0 };

const char* PizLooperEditor::piznew40_png   = (const char*) resource_PizLooperEditor_piznew40_png;
const int PizLooperEditor::piznew40_pngSize = 1803;

//[EndFile] You can add extra defines here...
//[/EndFile]
