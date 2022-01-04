/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  14 Sep 2011 10:46:40am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
inline int combineBytes(int lsb, int msb)
{
	return ((msb&0x7f)<<7) | (lsb&0x7f);
}
//[/Headers]

#include "midiPCGUIEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
midiPCGUIEditor::midiPCGUIEditor (midiPCGUI* const ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      groupComponent2 (0),
      groupComponent (0),
      s_Program (0),
      s_BankMSB (0),
      s_BankLSB (0),
      b_BankTrig (0),
      b_PCTrig (0),
      b_Inc (0),
      b_Dec (0),
      b_PCListen (0),
      s_Channel (0),
      b_Mode (0),
      PCDisplay (0),
      label (0),
      label2 (0),
      label3 (0),
      label4 (0),
      label5 (0),
      b_Thru (0),
      PCDisplay2 (0),
      PCDisplay3 (0),
      label6 (0),
      ProgramName (0),
      minimize (0)
{
    addAndMakeVisible (groupComponent2 = new GroupComponent ("new group",
                                                             "Program"));
    groupComponent2->setTextLabelPosition (Justification::centred);

    addAndMakeVisible (groupComponent = new GroupComponent ("new group",
                                                            "Bank"));
    groupComponent->setTextLabelPosition (Justification::centred);

    addAndMakeVisible (s_Program = new VSTSlider ("Program"));
    s_Program->setTooltip ("Next Program Change to send");
    s_Program->setRange (0, 128, 1);
    s_Program->setSliderStyle (Slider::LinearHorizontal);
    s_Program->setTextBoxStyle (Slider::TextBoxLeft, false, 28, 20);
    s_Program->addListener (this);

    addAndMakeVisible (s_BankMSB = new VSTSlider ("Bank MSB"));
    s_BankMSB->setTooltip ("Next Bank Select to send");
    s_BankMSB->setRange (0, 128, 1);
    s_BankMSB->setSliderStyle (Slider::LinearHorizontal);
    s_BankMSB->setTextBoxStyle (Slider::TextBoxLeft, false, 28, 20);
    s_BankMSB->addListener (this);

    addAndMakeVisible (s_BankLSB = new VSTSlider ("Bank LSB"));
    s_BankLSB->setTooltip ("Next Bank LSB to send");
    s_BankLSB->setRange (0, 128, 1);
    s_BankLSB->setSliderStyle (Slider::LinearHorizontal);
    s_BankLSB->setTextBoxStyle (Slider::TextBoxLeft, false, 28, 20);
    s_BankLSB->addListener (this);

    addAndMakeVisible (b_BankTrig = new TextButton ("Bank Trig"));
    b_BankTrig->setTooltip ("Send selected Bank Select message(s)");
    b_BankTrig->setButtonText ("Trigger Bank");
    b_BankTrig->addListener (this);
    b_BankTrig->setColour (TextButton::buttonColourId, Colour (0xffbbbbff));

    addAndMakeVisible (b_PCTrig = new TextButton ("PC Trig"));
    b_PCTrig->setTooltip ("Send selected Program Change message");
    b_PCTrig->setButtonText ("Trigger Program");
    b_PCTrig->addListener (this);
    b_PCTrig->setColour (TextButton::buttonColourId, Colour (0xffbbbbff));

    addAndMakeVisible (b_Inc = new TextButton ("Inc"));
    b_Inc->setButtonText ("+");
    b_Inc->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnTop);
    b_Inc->addListener (this);
    b_Inc->setColour (TextButton::buttonColourId, Colour (0xffbbbbff));
    b_Inc->setColour (TextButton::textColourOnId, Colours::black);

    addAndMakeVisible (b_Dec = new TextButton ("Dec"));
    b_Dec->setButtonText ("-");
    b_Dec->setConnectedEdges (Button::ConnectedOnRight | Button::ConnectedOnTop);
    b_Dec->addListener (this);
    b_Dec->setColour (TextButton::buttonColourId, Colour (0xffbbbbff));
    b_Dec->setColour (TextButton::textColourOnId, Colours::black);

    addAndMakeVisible (b_PCListen = new ToggleButton ("PC Listen"));
    b_PCListen->setTooltip ("Use incoming Program Changes to select VST programs");
    b_PCListen->addListener (this);

    addAndMakeVisible (s_Channel = new VSTSlider ("Channel"));
    s_Channel->setTooltip ("Input/Output MIDI Channel");
    s_Channel->setRange (1, 16, 1);
    s_Channel->setSliderStyle (Slider::LinearHorizontal);
    s_Channel->setTextBoxStyle (Slider::TextBoxLeft, false, 28, 20);
    s_Channel->addListener (this);

    addAndMakeVisible (b_Mode = new TextButton ("Mode"));
    b_Mode->setTooltip ("Triggered: Send messages when button is clicked; Direct: Send messages when sliders are moved");
    b_Mode->setButtonText ("Triggered");
    b_Mode->addListener (this);
    b_Mode->setColour (TextButton::buttonColourId, Colour (0xffbbbbff));
    b_Mode->setColour (TextButton::buttonOnColourId, Colours::coral);

    addAndMakeVisible (PCDisplay = new Label ("PCDisplay",
                                              "128"));
    PCDisplay->setTooltip ("Last Program Change sent");
    PCDisplay->setFont (Font (63.3000f, Font::bold));
    PCDisplay->setJustificationType (Justification::centred);
    PCDisplay->setEditable (false, true, false);
    PCDisplay->setColour (Label::backgroundColourId, Colour (0xffd0d0d0));
    PCDisplay->setColour (Label::outlineColourId, Colours::black);
    PCDisplay->setColour (TextEditor::textColourId, Colours::black);
    PCDisplay->setColour (TextEditor::backgroundColourId, Colour (0x0));
    PCDisplay->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          "Channel"));
    label->setFont (Font (15.0000f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label2 = new Label ("new label",
                                           "Mode"));
    label2->setFont (Font (15.0000f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label3 = new Label ("new label",
                                           "MSB"));
    label3->setFont (Font (12.0000f, Font::plain));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label4 = new Label ("new label",
                                           "LSB"));
    label4->setFont (Font (12.0000f, Font::plain));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label5 = new Label ("new label",
                                           "Current Program"));
    label5->setFont (Font (15.0000f, Font::plain));
    label5->setJustificationType (Justification::centred);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (b_Thru = new ToggleButton ("Thru"));
    b_Thru->addListener (this);

    addAndMakeVisible (PCDisplay2 = new Label ("PCDisplay",
                                               "128"));
    PCDisplay2->setFont (Font (18.0000f, Font::bold));
    PCDisplay2->setJustificationType (Justification::centred);
    PCDisplay2->setEditable (false, true, false);
    PCDisplay2->setColour (Label::backgroundColourId, Colour (0xffcecece));
    PCDisplay2->setColour (Label::outlineColourId, Colours::black);
    PCDisplay2->setColour (TextEditor::textColourId, Colours::black);
    PCDisplay2->setColour (TextEditor::backgroundColourId, Colour (0x0));
    PCDisplay2->addListener (this);

    addAndMakeVisible (PCDisplay3 = new Label ("PCDisplay",
                                               "128"));
    PCDisplay3->setFont (Font (18.0000f, Font::bold));
    PCDisplay3->setJustificationType (Justification::centred);
    PCDisplay3->setEditable (false, true, false);
    PCDisplay3->setColour (Label::backgroundColourId, Colour (0xffd0d0d0));
    PCDisplay3->setColour (Label::outlineColourId, Colours::black);
    PCDisplay3->setColour (TextEditor::textColourId, Colours::black);
    PCDisplay3->setColour (TextEditor::backgroundColourId, Colour (0x0));
    PCDisplay3->addListener (this);

    addAndMakeVisible (label6 = new Label ("new label",
                                           "Current Bank"));
    label6->setFont (Font (15.0000f, Font::plain));
    label6->setJustificationType (Justification::centred);
    label6->setEditable (false, false, false);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (ProgramName = new ClickableLabel ("Program Name",
                                                         "Awesome Thing"));
    ProgramName->setFont (Font (18.0000f, Font::bold));
    ProgramName->setJustificationType (Justification::centred);
    ProgramName->setEditable (false, true, false);
    ProgramName->setColour (Label::backgroundColourId, Colour (0xffcecece));
    ProgramName->setColour (Label::outlineColourId, Colours::black);
    ProgramName->setColour (TextEditor::textColourId, Colours::black);
    ProgramName->setColour (TextEditor::backgroundColourId, Colours::white);
    ProgramName->addListener (this);

    addAndMakeVisible (minimize = new TextButton ("Minimize"));
    minimize->setButtonText ("_");
    minimize->addListener (this);


    //[UserPreSize]
    ProgramName->setListener (this);
	s_Program->setOwner(getAudioProcessor(),kProgram);
	s_BankMSB->setOwner(getAudioProcessor(),kBankMSB);
	s_BankLSB->setOwner(getAudioProcessor(),kBankLSB);
	s_Channel->setOwner(getAudioProcessor(),kChannel);
	s_Program->setSliderSnapsToMousePosition(false);
	s_BankMSB->setSliderSnapsToMousePosition(false);
	s_BankLSB->setSliderSnapsToMousePosition(false);
	s_Channel->setSliderSnapsToMousePosition(true);

	this->setMouseClickGrabsKeyboardFocus(false);
	groupComponent->setMouseClickGrabsKeyboardFocus(false);
	groupComponent2->setMouseClickGrabsKeyboardFocus(false);
	b_BankTrig->setMouseClickGrabsKeyboardFocus(false);
	b_PCTrig->setMouseClickGrabsKeyboardFocus(false);
	b_Inc->setMouseClickGrabsKeyboardFocus(false);
	b_Dec->setMouseClickGrabsKeyboardFocus(false);
	b_PCListen->setMouseClickGrabsKeyboardFocus(false);
	b_Mode->setMouseClickGrabsKeyboardFocus(false);
	b_Thru->setMouseClickGrabsKeyboardFocus(false);
	PCDisplay->setMouseClickGrabsKeyboardFocus(false);
	label->setMouseClickGrabsKeyboardFocus(false);
	label2->setMouseClickGrabsKeyboardFocus(false);
	label3->setMouseClickGrabsKeyboardFocus(false);
	label4->setMouseClickGrabsKeyboardFocus(false);
	label5->setMouseClickGrabsKeyboardFocus(false);

	minimized = ownerFilter->getParameter(kMinimize)>=0.5f;
    //[/UserPreSize]

    setSize (310, 280);


    //[Constructor] You can add your own custom stuff here..
    ownerFilter->addChangeListener (this);
	updateParametersFromFilter();
    //[/Constructor]
}

midiPCGUIEditor::~midiPCGUIEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    getFilter()->removeChangeListener (this);
    //[/Destructor_pre]

    deleteAndZero (groupComponent2);
    deleteAndZero (groupComponent);
    deleteAndZero (s_Program);
    deleteAndZero (s_BankMSB);
    deleteAndZero (s_BankLSB);
    deleteAndZero (b_BankTrig);
    deleteAndZero (b_PCTrig);
    deleteAndZero (b_Inc);
    deleteAndZero (b_Dec);
    deleteAndZero (b_PCListen);
    deleteAndZero (s_Channel);
    deleteAndZero (b_Mode);
    deleteAndZero (PCDisplay);
    deleteAndZero (label);
    deleteAndZero (label2);
    deleteAndZero (label3);
    deleteAndZero (label4);
    deleteAndZero (label5);
    deleteAndZero (b_Thru);
    deleteAndZero (PCDisplay2);
    deleteAndZero (PCDisplay3);
    deleteAndZero (label6);
    deleteAndZero (ProgramName);
    deleteAndZero (minimize);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void midiPCGUIEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
	if (minimized)
	{
		g.fillAll (Colour (0xff464646));

		g.setColour (Colour (0xffe6e6e6));
		g.fillRoundedRectangle (4.0f, 4.0f, 302.0f, 272.0f, 10.0000f);

		g.setColour (Colour (0xffbababa));
		g.fillRoundedRectangle (14.0f, 13.0f, 282.0f, 255.0f, 10.0000f);
	}
	else {
    //[/UserPrePaint]

    g.fillAll (Colour (0xff464646));

    g.setColour (Colour (0xffe6e6e6));
    g.fillRoundedRectangle (4.0f, 4.0f, 302.0f, 272.0f, 10.0000f);

    g.setColour (Colour (0xffbababa));
    g.fillRoundedRectangle (14.0f, 26.0f, 140.0f, 138.0f, 10.0000f);

    g.setColour (Colour (0xffbababa));
    g.fillRoundedRectangle (187.0f, 66.0f, 97.0f, 44.0f, 10.0000f);

    g.setColour (Colour (0xff464646));
    g.fillRoundedRectangle (165.0f, 10.0f, 137.0f, 30.0f, 4.0000f);

    g.setColour (Colour (0xfffaffa0));
    g.fillPath (internalPath1);

    g.setColour (Colour (0xfffaffa0));
    g.fillPath (internalPath2);

    //[UserPaint] Add your own custom painting code here..
	}
    //[/UserPaint]
}

void midiPCGUIEditor::resized()
{
    groupComponent2->setBounds (5, 7, 158, 225);
    groupComponent->setBounds (167, 45, 133, 166);
    s_Program->setBounds (18, 171, 133, 24);
    s_BankMSB->setBounds (202, 120, 94, 24);
    s_BankLSB->setBounds (202, 143, 94, 24);
    b_BankTrig->setBounds (179, 177, 109, 24);
    b_PCTrig->setBounds (20, 198, 129, 24);
    b_Inc->setBounds (84, 135, 40, 24);
    b_Dec->setBounds (44, 135, 40, 24);
    b_PCListen->setBounds (117, 253, 81, 24);
    s_Channel->setBounds (190, 227, 94, 24);
    b_Mode->setBounds (23, 249, 78, 24);
    PCDisplay->setBounds (18, 45, 132, 62);
    label->setBounds (204, 209, 58, 24);
    label2->setBounds (41, 231, 41, 23);
    label3->setBounds (174, 120, 33, 24);
    label4->setBounds (176, 143, 33, 24);
    label5->setBounds (21, 25, 126, 24);
    b_Thru->setBounds (214, 253, 54, 24);
    PCDisplay2->setBounds (193, 83, 42, 22);
    PCDisplay3->setBounds (236, 83, 42, 22);
    label6->setBounds (173, 64, 126, 24);
    ProgramName->setBounds (18, 106, 132, 30);
    minimize->setBounds (284, 258, 20, 16);
    internalPath1.clear();
    internalPath1.startNewSubPath (170.0f, 22.0f);
    internalPath1.lineTo (170.0f, 18.0f);
    internalPath1.lineTo (173.0f, 18.0f);
    internalPath1.lineTo (173.0f, 22.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (170.0f, 14.0f);
    internalPath1.lineTo (173.0f, 14.0f);
    internalPath1.lineTo (173.0f, 16.0f);
    internalPath1.lineTo (170.0f, 16.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (175.0f, 16.0f);
    internalPath1.lineTo (182.0f, 16.0f);
    internalPath1.lineTo (182.0f, 22.0f);
    internalPath1.lineTo (179.0f, 22.0f);
    internalPath1.lineTo (179.0f, 18.0f);
    internalPath1.lineTo (178.0f, 18.0f);
    internalPath1.lineTo (178.0f, 22.0f);
    internalPath1.lineTo (175.0f, 22.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (191.0f, 18.0f);
    internalPath1.lineTo (188.0f, 18.0f);
    internalPath1.lineTo (188.0f, 19.0f);
    internalPath1.lineTo (191.0f, 19.0f);
    internalPath1.lineTo (191.0f, 20.0f);
    internalPath1.lineTo (190.0f, 20.0f);
    internalPath1.lineTo (190.0f, 22.0f);
    internalPath1.lineTo (184.0f, 22.0f);
    internalPath1.lineTo (184.0f, 20.0f);
    internalPath1.lineTo (187.0f, 20.0f);
    internalPath1.lineTo (187.0f, 19.0f);
    internalPath1.lineTo (184.0f, 19.0f);
    internalPath1.lineTo (184.0f, 18.0f);
    internalPath1.lineTo (185.0f, 18.0f);
    internalPath1.lineTo (185.0f, 16.0f);
    internalPath1.lineTo (191.0f, 16.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (193.0f, 22.0f);
    internalPath1.lineTo (193.0f, 16.0f);
    internalPath1.lineTo (200.0f, 16.0f);
    internalPath1.lineTo (200.0f, 19.0f);
    internalPath1.lineTo (199.0f, 19.0f);
    internalPath1.lineTo (199.0f, 20.0f);
    internalPath1.lineTo (200.0f, 20.0f);
    internalPath1.lineTo (200.0f, 22.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (196.0f, 19.0f);
    internalPath1.lineTo (197.0f, 19.0f);
    internalPath1.lineTo (197.0f, 18.0f);
    internalPath1.lineTo (196.0f, 18.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (208.0f, 16.0f);
    internalPath1.lineTo (208.0f, 18.0f);
    internalPath1.lineTo (205.0f, 18.0f);
    internalPath1.lineTo (205.0f, 22.0f);
    internalPath1.lineTo (202.0f, 22.0f);
    internalPath1.lineTo (202.0f, 16.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (215.0f, 16.0f);
    internalPath1.lineTo (215.0f, 18.0f);
    internalPath1.lineTo (214.0f, 18.0f);
    internalPath1.lineTo (214.0f, 22.0f);
    internalPath1.lineTo (211.0f, 22.0f);
    internalPath1.lineTo (211.0f, 18.0f);
    internalPath1.lineTo (209.0f, 18.0f);
    internalPath1.lineTo (209.0f, 16.0f);
    internalPath1.lineTo (211.0f, 16.0f);
    internalPath1.lineTo (211.0f, 14.0f);
    internalPath1.lineTo (214.0f, 14.0f);
    internalPath1.lineTo (214.0f, 16.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (229.0f, 16.0f);
    internalPath1.lineTo (229.0f, 22.0f);
    internalPath1.lineTo (224.0f, 22.0f);
    internalPath1.lineTo (224.0f, 25.0f);
    internalPath1.lineTo (221.0f, 25.0f);
    internalPath1.lineTo (221.0f, 16.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (225.0f, 20.0f);
    internalPath1.lineTo (225.0f, 18.0f);
    internalPath1.lineTo (224.0f, 18.0f);
    internalPath1.lineTo (224.0f, 20.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (230.0f, 22.0f);
    internalPath1.lineTo (230.0f, 18.0f);
    internalPath1.lineTo (233.0f, 18.0f);
    internalPath1.lineTo (233.0f, 22.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (230.0f, 14.0f);
    internalPath1.lineTo (233.0f, 14.0f);
    internalPath1.lineTo (233.0f, 16.0f);
    internalPath1.lineTo (230.0f, 16.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (238.0f, 19.0f);
    internalPath1.lineTo (238.0f, 18.0f);
    internalPath1.lineTo (235.0f, 18.0f);
    internalPath1.lineTo (235.0f, 16.0f);
    internalPath1.lineTo (242.0f, 16.0f);
    internalPath1.lineTo (242.0f, 18.0f);
    internalPath1.lineTo (241.0f, 18.0f);
    internalPath1.lineTo (241.0f, 19.0f);
    internalPath1.lineTo (239.0f, 19.0f);
    internalPath1.lineTo (239.0f, 20.0f);
    internalPath1.lineTo (242.0f, 20.0f);
    internalPath1.lineTo (242.0f, 22.0f);
    internalPath1.lineTo (235.0f, 22.0f);
    internalPath1.lineTo (235.0f, 20.0f);
    internalPath1.lineTo (236.0f, 20.0f);
    internalPath1.lineTo (236.0f, 19.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (253.0f, 18.0f);
    internalPath1.lineTo (251.0f, 18.0f);
    internalPath1.lineTo (251.0f, 22.0f);
    internalPath1.lineTo (248.0f, 22.0f);
    internalPath1.lineTo (248.0f, 14.0f);
    internalPath1.lineTo (251.0f, 14.0f);
    internalPath1.lineTo (251.0f, 16.0f);
    internalPath1.lineTo (256.0f, 16.0f);
    internalPath1.lineTo (256.0f, 22.0f);
    internalPath1.lineTo (253.0f, 22.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (257.0f, 22.0f);
    internalPath1.lineTo (257.0f, 16.0f);
    internalPath1.lineTo (265.0f, 16.0f);
    internalPath1.lineTo (265.0f, 19.0f);
    internalPath1.lineTo (263.0f, 19.0f);
    internalPath1.lineTo (263.0f, 20.0f);
    internalPath1.lineTo (265.0f, 20.0f);
    internalPath1.lineTo (265.0f, 22.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (260.0f, 19.0f);
    internalPath1.lineTo (262.0f, 19.0f);
    internalPath1.lineTo (262.0f, 18.0f);
    internalPath1.lineTo (260.0f, 18.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (272.0f, 16.0f);
    internalPath1.lineTo (272.0f, 18.0f);
    internalPath1.lineTo (269.0f, 18.0f);
    internalPath1.lineTo (269.0f, 22.0f);
    internalPath1.lineTo (266.0f, 22.0f);
    internalPath1.lineTo (266.0f, 16.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (274.0f, 22.0f);
    internalPath1.lineTo (274.0f, 16.0f);
    internalPath1.lineTo (281.0f, 16.0f);
    internalPath1.lineTo (281.0f, 19.0f);
    internalPath1.lineTo (280.0f, 19.0f);
    internalPath1.lineTo (280.0f, 20.0f);
    internalPath1.lineTo (281.0f, 20.0f);
    internalPath1.lineTo (281.0f, 22.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (277.0f, 19.0f);
    internalPath1.lineTo (278.0f, 19.0f);
    internalPath1.lineTo (278.0f, 18.0f);
    internalPath1.lineTo (277.0f, 18.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (283.0f, 19.0f);
    internalPath1.lineTo (283.0f, 18.0f);
    internalPath1.lineTo (290.0f, 18.0f);
    internalPath1.lineTo (290.0f, 19.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (292.0f, 14.0f);
    internalPath1.lineTo (295.0f, 14.0f);
    internalPath1.lineTo (295.0f, 16.0f);
    internalPath1.lineTo (296.0f, 16.0f);
    internalPath1.lineTo (296.0f, 18.0f);
    internalPath1.lineTo (298.0f, 18.0f);
    internalPath1.lineTo (298.0f, 19.0f);
    internalPath1.lineTo (296.0f, 19.0f);
    internalPath1.lineTo (296.0f, 20.0f);
    internalPath1.lineTo (295.0f, 20.0f);
    internalPath1.lineTo (295.0f, 22.0f);
    internalPath1.lineTo (292.0f, 22.0f);
    internalPath1.lineTo (292.0f, 20.0f);
    internalPath1.lineTo (293.0f, 20.0f);
    internalPath1.lineTo (293.0f, 19.0f);
    internalPath1.lineTo (295.0f, 19.0f);
    internalPath1.lineTo (295.0f, 18.0f);
    internalPath1.lineTo (293.0f, 18.0f);
    internalPath1.lineTo (293.0f, 16.0f);
    internalPath1.lineTo (292.0f, 16.0f);
    internalPath1.closeSubPath();

    internalPath2.clear();
    internalPath2.startNewSubPath (179.0f, 29.0f);
    internalPath2.lineTo (179.0f, 34.0f);
    internalPath2.lineTo (177.0f, 34.0f);
    internalPath2.lineTo (177.0f, 30.0f);
    internalPath2.lineTo (176.0f, 30.0f);
    internalPath2.lineTo (176.0f, 34.0f);
    internalPath2.lineTo (173.0f, 34.0f);
    internalPath2.lineTo (173.0f, 30.0f);
    internalPath2.lineTo (172.0f, 30.0f);
    internalPath2.lineTo (172.0f, 34.0f);
    internalPath2.lineTo (169.0f, 34.0f);
    internalPath2.lineTo (169.0f, 29.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (181.0f, 34.0f);
    internalPath2.lineTo (181.0f, 30.0f);
    internalPath2.lineTo (183.0f, 30.0f);
    internalPath2.lineTo (183.0f, 34.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (181.0f, 28.0f);
    internalPath2.lineTo (183.0f, 28.0f);
    internalPath2.lineTo (183.0f, 29.0f);
    internalPath2.lineTo (181.0f, 29.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (185.0f, 34.0f);
    internalPath2.lineTo (185.0f, 29.0f);
    internalPath2.lineTo (189.0f, 29.0f);
    internalPath2.lineTo (189.0f, 28.0f);
    internalPath2.lineTo (191.0f, 28.0f);
    internalPath2.lineTo (191.0f, 34.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (187.0f, 30.0f);
    internalPath2.lineTo (187.0f, 33.0f);
    internalPath2.lineTo (189.0f, 33.0f);
    internalPath2.lineTo (189.0f, 30.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (192.0f, 34.0f);
    internalPath2.lineTo (192.0f, 30.0f);
    internalPath2.lineTo (195.0f, 30.0f);
    internalPath2.lineTo (195.0f, 34.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (192.0f, 28.0f);
    internalPath2.lineTo (195.0f, 28.0f);
    internalPath2.lineTo (195.0f, 29.0f);
    internalPath2.lineTo (192.0f, 29.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (203.0f, 28.0f);
    internalPath2.lineTo (203.0f, 32.0f);
    internalPath2.lineTo (199.0f, 32.0f);
    internalPath2.lineTo (199.0f, 34.0f);
    internalPath2.lineTo (196.0f, 34.0f);
    internalPath2.lineTo (196.0f, 28.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (199.0f, 30.0f);
    internalPath2.lineTo (200.0f, 30.0f);
    internalPath2.lineTo (200.0f, 29.0f);
    internalPath2.lineTo (199.0f, 29.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (209.0f, 29.0f);
    internalPath2.lineTo (209.0f, 30.0f);
    internalPath2.lineTo (207.0f, 30.0f);
    internalPath2.lineTo (207.0f, 34.0f);
    internalPath2.lineTo (204.0f, 34.0f);
    internalPath2.lineTo (204.0f, 29.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (217.0f, 34.0f);
    internalPath2.lineTo (211.0f, 34.0f);
    internalPath2.lineTo (211.0f, 29.0f);
    internalPath2.lineTo (217.0f, 29.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (215.0f, 30.0f);
    internalPath2.lineTo (213.0f, 30.0f);
    internalPath2.lineTo (213.0f, 33.0f);
    internalPath2.lineTo (215.0f, 33.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (218.0f, 34.0f);
    internalPath2.lineTo (218.0f, 29.0f);
    internalPath2.lineTo (225.0f, 29.0f);
    internalPath2.lineTo (225.0f, 37.0f);
    internalPath2.lineTo (218.0f, 37.0f);
    internalPath2.lineTo (218.0f, 36.0f);
    internalPath2.lineTo (222.0f, 36.0f);
    internalPath2.quadraticTo (222.0f, 36.0f, 222.0f, 34.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (222.0f, 30.0f);
    internalPath2.lineTo (221.0f, 30.0f);
    internalPath2.lineTo (221.0f, 33.0f);
    internalPath2.lineTo (222.0f, 33.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (231.0f, 29.0f);
    internalPath2.lineTo (231.0f, 30.0f);
    internalPath2.lineTo (229.0f, 30.0f);
    internalPath2.lineTo (229.0f, 34.0f);
    internalPath2.lineTo (226.0f, 34.0f);
    internalPath2.lineTo (226.0f, 29.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (233.0f, 30.0f);
    internalPath2.lineTo (234.0f, 30.0f);
    internalPath2.lineTo (234.0f, 29.0f);
    internalPath2.lineTo (239.0f, 29.0f);
    internalPath2.lineTo (239.0f, 34.0f);
    internalPath2.lineTo (233.0f, 34.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (237.0f, 30.0f);
    internalPath2.lineTo (235.0f, 30.0f);
    internalPath2.lineTo (235.0f, 33.0f);
    internalPath2.lineTo (237.0f, 33.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (251.0f, 29.0f);
    internalPath2.lineTo (251.0f, 34.0f);
    internalPath2.lineTo (248.0f, 34.0f);
    internalPath2.lineTo (248.0f, 30.0f);
    internalPath2.lineTo (247.0f, 30.0f);
    internalPath2.lineTo (247.0f, 34.0f);
    internalPath2.lineTo (244.0f, 34.0f);
    internalPath2.lineTo (244.0f, 30.0f);
    internalPath2.lineTo (243.0f, 30.0f);
    internalPath2.lineTo (243.0f, 34.0f);
    internalPath2.lineTo (241.0f, 34.0f);
    internalPath2.lineTo (241.0f, 29.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (259.0f, 33.0f);
    internalPath2.lineTo (259.0f, 34.0f);
    internalPath2.lineTo (252.0f, 34.0f);
    internalPath2.lineTo (252.0f, 28.0f);
    internalPath2.lineTo (259.0f, 28.0f);
    internalPath2.lineTo (259.0f, 29.0f);
    internalPath2.lineTo (255.0f, 29.0f);
    internalPath2.lineTo (255.0f, 33.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (264.0f, 30.0f);
    internalPath2.lineTo (263.0f, 30.0f);
    internalPath2.lineTo (263.0f, 34.0f);
    internalPath2.lineTo (260.0f, 34.0f);
    internalPath2.lineTo (260.0f, 28.0f);
    internalPath2.lineTo (263.0f, 28.0f);
    internalPath2.lineTo (263.0f, 29.0f);
    internalPath2.lineTo (267.0f, 29.0f);
    internalPath2.lineTo (267.0f, 34.0f);
    internalPath2.lineTo (264.0f, 34.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (268.0f, 30.0f);
    internalPath2.lineTo (269.0f, 30.0f);
    internalPath2.lineTo (269.0f, 29.0f);
    internalPath2.lineTo (274.0f, 29.0f);
    internalPath2.lineTo (274.0f, 34.0f);
    internalPath2.lineTo (268.0f, 34.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (272.0f, 30.0f);
    internalPath2.lineTo (270.0f, 30.0f);
    internalPath2.lineTo (270.0f, 33.0f);
    internalPath2.lineTo (272.0f, 33.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (276.0f, 29.0f);
    internalPath2.lineTo (282.0f, 29.0f);
    internalPath2.lineTo (282.0f, 34.0f);
    internalPath2.lineTo (280.0f, 34.0f);
    internalPath2.lineTo (280.0f, 30.0f);
    internalPath2.lineTo (278.0f, 30.0f);
    internalPath2.lineTo (278.0f, 34.0f);
    internalPath2.lineTo (276.0f, 34.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (283.0f, 34.0f);
    internalPath2.lineTo (283.0f, 29.0f);
    internalPath2.lineTo (290.0f, 29.0f);
    internalPath2.lineTo (290.0f, 37.0f);
    internalPath2.lineTo (283.0f, 37.0f);
    internalPath2.lineTo (283.0f, 36.0f);
    internalPath2.lineTo (287.0f, 36.0f);
    internalPath2.quadraticTo (287.0f, 36.0f, 287.0f, 34.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (287.0f, 30.0f);
    internalPath2.lineTo (286.0f, 30.0f);
    internalPath2.lineTo (286.0f, 33.0f);
    internalPath2.lineTo (287.0f, 33.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (291.0f, 34.0f);
    internalPath2.lineTo (291.0f, 29.0f);
    internalPath2.lineTo (298.0f, 29.0f);
    internalPath2.lineTo (298.0f, 32.0f);
    internalPath2.lineTo (296.0f, 32.0f);
    internalPath2.lineTo (296.0f, 33.0f);
    internalPath2.lineTo (298.0f, 33.0f);
    internalPath2.lineTo (298.0f, 34.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath (294.0f, 32.0f);
    internalPath2.lineTo (295.0f, 32.0f);
    internalPath2.lineTo (295.0f, 31.0f);
    internalPath2.lineTo (294.0f, 31.0f);
    internalPath2.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
	groupComponent->setVisible(!minimized);
	groupComponent2->setVisible(!minimized);
	s_Program->setVisible(!minimized);
	s_BankMSB->setVisible(!minimized);
	s_BankLSB->setVisible(!minimized);
	b_BankTrig->setVisible(!minimized);
	b_PCTrig->setVisible(!minimized);
	b_PCListen->setVisible(!minimized);
	s_Channel->setVisible(!minimized);
	b_Mode->setVisible(!minimized);
    label->setVisible(!minimized);
    label2->setVisible(!minimized);
    label3->setVisible(!minimized);
    label4->setVisible(!minimized);
    b_Thru->setVisible(!minimized);
    PCDisplay2->setVisible(!minimized);
    PCDisplay3->setVisible(!minimized);
    label6->setVisible(!minimized);
	if (minimized)
	{
		ProgramName->setFont(Font(32.f,Font::bold));
		PCDisplay->setFont(Font(128.f,Font::bold));

		b_Inc->setBounds (159, 228, 40, 24);
		b_Dec->setBounds (119, 228, 40, 24);
		label5->setBounds (92, 11, 126, 24);
		ProgramName->setBounds (18, 173, 274, 55);
		minimize->setBounds (284, 258, 20, 16);
		PCDisplay->setBounds (18, 51, 274, 123);
	}
	else {
		ProgramName->setFont(Font(18.f,Font::bold));
		PCDisplay->setFont(Font(63.3f,Font::bold));
	}
	repaint();
    //[/UserResized]
}

void midiPCGUIEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    VSTSlider* slider = (VSTSlider*)sliderThatWasMoved;
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == s_Program)
    {
        //[UserSliderCode_s_Program] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kProgram,slider->mapToVSTRange());
        //[/UserSliderCode_s_Program]
    }
    else if (sliderThatWasMoved == s_BankMSB)
    {
        //[UserSliderCode_s_BankMSB] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kBankMSB,slider->mapToVSTRange());
        //[/UserSliderCode_s_BankMSB]
    }
    else if (sliderThatWasMoved == s_BankLSB)
    {
        //[UserSliderCode_s_BankLSB] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kBankLSB,slider->mapToVSTRange());
        //[/UserSliderCode_s_BankLSB]
    }
    else if (sliderThatWasMoved == s_Channel)
    {
        //[UserSliderCode_s_Channel] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kChannel,slider->mapToVSTRange());
        //[/UserSliderCode_s_Channel]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void midiPCGUIEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == b_BankTrig)
    {
        //[UserButtonCode_b_BankTrig] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kBankTrigger,1.f);
        //[/UserButtonCode_b_BankTrig]
    }
    else if (buttonThatWasClicked == b_PCTrig)
    {
        //[UserButtonCode_b_PCTrig] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kTrigger,1.f);
        //[/UserButtonCode_b_PCTrig]
    }
    else if (buttonThatWasClicked == b_Inc)
    {
        //[UserButtonCode_b_Inc] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kInc,1.f);
        //[/UserButtonCode_b_Inc]
    }
    else if (buttonThatWasClicked == b_Dec)
    {
        //[UserButtonCode_b_Dec] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kDec,1.f);
        //[/UserButtonCode_b_Dec]
    }
    else if (buttonThatWasClicked == b_PCListen)
    {
        //[UserButtonCode_b_PCListen] -- add your button handler code here..
        if (getFilter()->getParameter(kPCListen)>=0.5f) {
            getFilter()->setParameterNotifyingHost(kPCListen,0.0f);
        }
        else {
            getFilter()->setParameterNotifyingHost(kPCListen,1.0f);
        }
        //[/UserButtonCode_b_PCListen]
    }
    else if (buttonThatWasClicked == b_Mode)
    {
        //[UserButtonCode_b_Mode] -- add your button handler code here..
        if (getFilter()->getParameter(kMode)>=0.5f) {
            getFilter()->setParameterNotifyingHost(kMode,0.0f);
        }
        else {
            getFilter()->setParameterNotifyingHost(kMode,1.0f);
        }
        //[/UserButtonCode_b_Mode]
    }
    else if (buttonThatWasClicked == b_Thru)
    {
        //[UserButtonCode_b_Thru] -- add your button handler code here..
        if (getFilter()->getParameter(kThru)>=0.5f) {
            getFilter()->setParameterNotifyingHost(kThru,0.0f);
        }
        else {
            getFilter()->setParameterNotifyingHost(kThru,1.0f);
        }
        //[/UserButtonCode_b_Thru]
    }
    else if (buttonThatWasClicked == minimize)
    {
        //[UserButtonCode_minimize] -- add your button handler code here..
		minimized = !minimized;
		getFilter()->setParameterNotifyingHost(kMinimize, minimized ? 1.f : 0.f);
		resized();
        //[/UserButtonCode_minimize]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void midiPCGUIEditor::labelTextChanged (Label* labelThatHasChanged)
{
    //[UserlabelTextChanged_Pre]
    //[/UserlabelTextChanged_Pre]

    if (labelThatHasChanged == PCDisplay)
    {
        //[UserLabelCode_PCDisplay] -- add your label text handling code here..
		int v = PCDisplay->getText().getIntValue();
		if (PCDisplay->getText().equalsIgnoreCase("off"))
			getFilter()->setParameterNotifyingHost(kProgram,0.f);
		else if (v>=0 && v<=128)
			getFilter()->setParameterNotifyingHost(kProgram,(float)v/128.f);
		getFilter()->setParameter(kTrigger,1.f);
        //[/UserLabelCode_PCDisplay]
    }
    else if (labelThatHasChanged == PCDisplay2)
    {
        //[UserLabelCode_PCDisplay2] -- add your label text handling code here..
		int v = PCDisplay2->getText().getIntValue();
		if (PCDisplay2->getText().equalsIgnoreCase("off"))
			getFilter()->setParameterNotifyingHost(kBankMSB,0.f);
		else if (v>=0 && v<=128)
			getFilter()->setParameterNotifyingHost(kBankMSB,(float)v/128.f);
		getFilter()->setParameter(kBankTrigger,1.f);
        //[/UserLabelCode_PCDisplay2]
    }
    else if (labelThatHasChanged == PCDisplay3)
    {
        //[UserLabelCode_PCDisplay3] -- add your label text handling code here..
		int v = PCDisplay3->getText().getIntValue();
		if (PCDisplay3->getText().equalsIgnoreCase("off"))
			getFilter()->setParameterNotifyingHost(kBankLSB,0.f);
		else if (v>=0 && v<=128)
			getFilter()->setParameterNotifyingHost(kBankLSB,(float)v/128.f);
		getFilter()->setParameter(kBankTrigger,1.f);
        //[/UserLabelCode_PCDisplay3]
    }
    else if (labelThatHasChanged == ProgramName)
    {
        //[UserLabelCode_ProgramName] -- add your label text handling code here..
		getFilter()->setMidiProgName((int)s_Channel->getValue()-1,
									 combineBytes(PCDisplay3->getText().getIntValue()-1,PCDisplay2->getText().getIntValue()-1),
									 PCDisplay->getText().getIntValue()-1,
									 ProgramName->getText());
        //[/UserLabelCode_ProgramName]
    }

    //[UserlabelTextChanged_Post]
    //[/UserlabelTextChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//==============================================================================
void midiPCGUIEditor::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source==getFilter()) {
        updateParametersFromFilter();
    }
}

//==============================================================================
void midiPCGUIEditor::updateParametersFromFilter()
{
    midiPCGUI* const filter = getFilter();

    float param[numParams];

    // we use this lock to make sure the processBlock() method isn't writing to the
    // lastMidiMessage variable while we're trying to read it, but be extra-careful to
    // only hold the lock for a minimum amount of time..
    filter->getCallbackLock().enter();

    // take a local copy of the info we need while we've got the lock..
    for (int i=0;i<numParams;i++) param[i] = filter->getParameter(i);
	const int channel = roundFloatToInt(param[kChannel] * 15.0f);
	const int p = filter->actualProgram[channel];
	const int msb = filter->actualBankMSB[channel];
	const int lsb = filter->actualBankLSB[channel];
	const String name = filter->getMidiProgName(channel, combineBytes(lsb-1,msb-1), p-1);

    // ..release the lock ASAP
    filter->getCallbackLock().exit();

    // ..and after releasing the lock, we're free to do the time-consuming UI stuff..

	//prog name
	ProgramName->setText(name,dontSendNotification);
    //toggle buttons
    b_PCListen->setToggleState(param[kPCListen]>=0.5f,false);
    b_Thru->setToggleState(param[kThru]>=0.5f,false);

    if (param[kMode]<0.5f) {
		b_Mode->setButtonText("Direct");
		b_Mode->setToggleState(true,false);
		s_Program->setColour (Slider::thumbColourId, Colours::coral);
		s_BankMSB->setColour (Slider::thumbColourId, Colours::coral);
		s_BankLSB->setColour (Slider::thumbColourId, Colours::coral);
	}
    else {
		b_Mode->setButtonText("Triggered");
		b_Mode->setToggleState(false,false);
		s_Program->setColour (Slider::thumbColourId, Colour(getLookAndFeel().findColour(Slider::thumbColourId)));
		s_BankMSB->setColour (Slider::thumbColourId, Colour(getLookAndFeel().findColour(Slider::thumbColourId)));
		s_BankLSB->setColour (Slider::thumbColourId, Colour(getLookAndFeel().findColour(Slider::thumbColourId)));
	}


    //sliders
    s_Program->setVSTSlider(param[kProgram]);
    s_BankMSB->setVSTSlider(param[kBankMSB]);
    s_BankLSB->setVSTSlider(param[kBankLSB]);
    s_Channel->setVSTSlider(param[kChannel]);
	PCDisplay->setText(String(p),dontSendNotification);
    PCDisplay2->setText(String(msb),dontSendNotification);
	PCDisplay3->setText(String(lsb),dontSendNotification);
	minimized = param[kMinimize]>=0.5f;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="midiPCGUIEditor" componentName=""
                 parentClasses="public AudioProcessorEditor, public ChangeListener, public ClickableLabelListener"
                 constructorParams="midiPCGUI* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="0" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="310" initialHeight="280">
  <BACKGROUND backgroundColour="ff464646">
    <ROUNDRECT pos="4 4 302 272" cornerSize="10" fill="solid: ffe6e6e6" hasStroke="0"/>
    <ROUNDRECT pos="14 26 140 138" cornerSize="10" fill="solid: ffbababa" hasStroke="0"/>
    <ROUNDRECT pos="187 66 97 44" cornerSize="10" fill="solid: ffbababa" hasStroke="0"/>
    <ROUNDRECT pos="165 10 137 30" cornerSize="4" fill="solid: ff464646" hasStroke="0"/>
    <PATH pos="0 0 100 100" fill="solid: fffaffa0" hasStroke="0" nonZeroWinding="1">s 170.01 22 l 170.01 17.5 l 173.01 17.5 l 173.01 22 x s 170.01 14.5 l 173.01 14.5 l 173.01 16 l 170.01 16 x s 174.51 16 l 182.01 16 l 182.01 22 l 179.01 22 l 179.01 17.5 l 177.51 17.5 l 177.51 22 l 174.51 22 x s 191.01 17.5 l 188.01 17.5 l 188.01 19 l 191.01 19 l 191.01 20.5 l 189.51 20.5 l 189.51 22 l 183.51 22 l 183.51 20.5 l 186.51 20.5 l 186.51 19 l 183.51 19 l 183.51 17.5 l 185.01 17.5 l 185.01 16 l 191.01 16 x s 192.51 22 l 192.51 16 l 200.01 16 l 200.01 19 l 198.51 19 l 198.51 20.5 l 200.01 20.5 l 200.01 22 x s 195.51 18.94 l 197.01 18.94 l 197.01 17.56 l 195.51 17.56 x s 207.51 16 l 207.51 17.5 l 204.51 17.5 l 204.51 22 l 201.51 22 l 201.51 16 x s 215.01 16 l 215.01 17.5 l 213.51 17.5 l 213.51 22 l 210.51 22 l 210.51 17.5 l 209.01 17.5 l 209.01 16 l 210.51 16 l 210.51 14.5 l 213.51 14.5 l 213.51 16 x s 228.51 16 l 228.51 22 l 224.01 22 l 224.01 25 l 221.01 25 l 221.01 16 x s 225.45 20.5 l 225.45 17.5 l 224.01 17.5 l 224.01 20.5 x s 230.01 22 l 230.01 17.5 l 233.01 17.5 l 233.01 22 x s 230.01 14.5 l 233.01 14.5 l 233.01 16 l 230.01 16 x s 237.51 19 l 237.51 17.5 l 234.51 17.5 l 234.51 16 l 242.01 16 l 242.01 17.5 l 240.51 17.5 l 240.51 19 l 239.01 19 l 239.01 20.5 l 242.01 20.5 l 242.01 22 l 234.51 22 l 234.51 20.5 l 236.01 20.5 l 236.01 19 x s 252.51 17.5 l 251.01 17.5 l 251.01 22 l 248.01 22 l 248.01 14.5 l 251.01 14.5 l 251.01 16 l 255.51 16 l 255.51 22 l 252.51 22 x s 257.01 22 l 257.01 16 l 264.51 16 l 264.51 19 l 263.01 19 l 263.01 20.5 l 264.51 20.5 l 264.51 22 x s 260.01 18.94 l 261.51 18.94 l 261.51 17.56 l 260.01 17.56 x s 272.01 16 l 272.01 17.5 l 269.01 17.5 l 269.01 22 l 266.01 22 l 266.01 16 x s 273.51 22 l 273.51 16 l 281.01 16 l 281.01 19 l 279.51 19 l 279.51 20.5 l 281.01 20.5 l 281.01 22 x s 276.51 18.94 l 278.01 18.94 l 278.01 17.56 l 276.51 17.56 x s 282.51 19 l 282.51 17.5 l 290.01 17.5 l 290.01 19 x s 291.51 14.5 l 294.51 14.5 l 294.51 16 l 296.01 16 l 296.01 17.5 l 297.51 17.5 l 297.51 19 l 296.01 19 l 296.01 20.5 l 294.51 20.5 l 294.51 22 l 291.51 22 l 291.51 20.5 l 293.01 20.5 l 293.01 19 l 294.51 19 l 294.51 17.5 l 293.01 17.5 l 293.01 16 l 291.51 16 x</PATH>
    <PATH pos="0 0 100 100" fill="solid: fffaffa0" hasStroke="0" nonZeroWinding="1">s 179.41 29.2 l 179.41 34.4 l 176.81 34.4 l 176.81 30.5 l 175.51 30.5 l 175.51 34.4 l 172.91 34.4 l 172.91 30.5 l 171.61 30.5 l 171.61 34.4 l 169.01 34.4 l 169.01 29.2 x s 180.71 34.4 l 180.71 30.5 l 183.31 30.5 l 183.31 34.4 x s 180.71 27.9 l 183.31 27.9 l 183.31 29.2 l 180.71 29.2 x s 184.61 34.4 l 184.61 29.2 l 188.51 29.2 l 188.51 27.9 l 191.11 27.9 l 191.11 34.4 x s 187.26 30.5 l 187.26 33.1 l 188.51 33.1 l 188.51 30.5 x s 192.41 34.4 l 192.41 30.5 l 195.01 30.5 l 195.01 34.4 x s 192.41 27.9 l 195.01 27.9 l 195.01 29.2 l 192.41 29.2 x s 202.81 27.9 l 202.81 31.8 l 198.91 31.8 l 198.91 34.4 l 196.31 34.4 l 196.31 27.9 x s 198.91 30.5 l 200.21 30.5 l 200.21 29.2 l 198.91 29.2 x s 209.31 29.2 l 209.31 30.5 l 206.71 30.5 l 206.71 34.4 l 204.11 34.4 l 204.11 29.2 x s 217.11 34.4 l 210.61 34.4 l 210.61 29.2 l 217.11 29.2 x s 214.51 30.5 l 213.21 30.5 l 213.21 33.1 l 214.51 33.1 x s 218.41 34.4 l 218.41 29.2 l 224.91 29.2 l 224.91 37 l 218.41 37 l 218.41 35.7 l 222.31 35.7 q 222.31 35.68 222.31 34.4 x s 222.31 30.5 l 221.01 30.5 l 221.01 33.1 l 222.31 33.1 x s 231.41 29.2 l 231.41 30.5 l 228.81 30.5 l 228.81 34.4 l 226.21 34.4 l 226.21 29.2 x s 232.71 30.5 l 234.01 30.5 l 234.01 29.2 l 239.21 29.2 l 239.21 34.4 l 232.71 34.4 x s 236.61 30.5 l 235.31 30.5 l 235.31 33.1 l 236.61 33.1 x s 250.91 29.2 l 250.91 34.4 l 248.31 34.4 l 248.31 30.5 l 247.01 30.5 l 247.01 34.4 l 244.41 34.4 l 244.41 30.5 l 243.11 30.5 l 243.11 34.4 l 240.51 34.4 l 240.51 29.2 x s 258.71 33.1 l 258.71 34.4 l 252.21 34.4 l 252.21 27.9 l 258.71 27.9 l 258.71 29.2 l 254.81 29.2 l 254.81 33.1 x s 263.91 30.5 l 262.61 30.5 l 262.61 34.4 l 260.01 34.4 l 260.01 27.9 l 262.61 27.9 l 262.61 29.2 l 266.51 29.2 l 266.51 34.4 l 263.91 34.4 x s 267.81 30.5 l 269.11 30.5 l 269.11 29.2 l 274.31 29.2 l 274.31 34.4 l 267.81 34.4 x s 271.71 30.5 l 270.41 30.5 l 270.41 33.1 l 271.71 33.1 x s 275.61 29.2 l 282.11 29.2 l 282.11 34.4 l 279.51 34.4 l 279.51 30.5 l 278.21 30.5 l 278.21 34.4 l 275.61 34.4 x s 283.41 34.4 l 283.41 29.2 l 289.91 29.2 l 289.91 37 l 283.41 37 l 283.41 35.7 l 287.31 35.7 q 287.31 35.68 287.31 34.4 x s 287.31 30.5 l 286.01 30.5 l 286.01 33.1 l 287.31 33.1 x s 291.21 34.4 l 291.21 29.2 l 297.71 29.2 l 297.71 31.8 l 296.41 31.8 l 296.41 33.1 l 297.71 33.1 l 297.71 34.4 x s 293.81 31.75 l 295.11 31.75 l 295.11 30.55 l 293.81 30.55 x</PATH>
  </BACKGROUND>
  <GROUPCOMPONENT name="new group" id="5cca9934cfc770aa" memberName="groupComponent2"
                  virtualName="" explicitFocusOrder="0" pos="5 7 158 225" title="Program"
                  textpos="36"/>
  <GROUPCOMPONENT name="new group" id="424c89975a274702" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="167 45 133 166" title="Bank"
                  textpos="36"/>
  <SLIDER name="Program" id="2288cc5e58f6e2e5" memberName="s_Program" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="18 171 133 24" tooltip="Next Program Change to send"
          min="0" max="128" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="28" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="Bank MSB" id="ef866cf4f66cd415" memberName="s_BankMSB"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="202 120 94 24"
          tooltip="Next Bank Select to send" min="0" max="128" int="1"
          style="LinearHorizontal" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="28" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="Bank LSB" id="c2ebdd7ada591c7c" memberName="s_BankLSB"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="202 143 94 24"
          tooltip="Next Bank LSB to send" min="0" max="128" int="1" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="28"
          textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="Bank Trig" id="c6731afd8b59ae43" memberName="b_BankTrig"
              virtualName="" explicitFocusOrder="0" pos="179 177 109 24" tooltip="Send selected Bank Select message(s)"
              bgColOff="ffbbbbff" buttonText="Trigger Bank" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="PC Trig" id="caa57639f5af1a67" memberName="b_PCTrig" virtualName=""
              explicitFocusOrder="0" pos="20 198 129 24" tooltip="Send selected Program Change message"
              bgColOff="ffbbbbff" buttonText="Trigger Program" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Inc" id="b0e2f8f46818c8b3" memberName="b_Inc" virtualName=""
              explicitFocusOrder="0" pos="84 135 40 24" bgColOff="ffbbbbff"
              textCol="ff000000" buttonText="+" connectedEdges="5" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Dec" id="fcaebc89c1ef6df5" memberName="b_Dec" virtualName=""
              explicitFocusOrder="0" pos="44 135 40 24" bgColOff="ffbbbbff"
              textCol="ff000000" buttonText="-" connectedEdges="6" needsCallback="1"
              radioGroupId="0"/>
  <TOGGLEBUTTON name="PC Listen" id="96c2985d7e3ce35d" memberName="b_PCListen"
                virtualName="" explicitFocusOrder="0" pos="117 253 81 24" tooltip="Use incoming Program Changes to select VST programs"
                buttonText="PC Listen" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <SLIDER name="Channel" id="618548eef3ed7f00" memberName="s_Channel" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="190 227 94 24" tooltip="Input/Output MIDI Channel"
          min="1" max="16" int="1" style="LinearHorizontal" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="28" textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="Mode" id="1db4f2da47c7baba" memberName="b_Mode" virtualName=""
              explicitFocusOrder="0" pos="23 249 78 24" tooltip="Triggered: Send messages when button is clicked; Direct: Send messages when sliders are moved"
              bgColOff="ffbbbbff" bgColOn="ffff7f50" buttonText="Triggered"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="PCDisplay" id="72c51462f1de614b" memberName="PCDisplay"
         virtualName="" explicitFocusOrder="0" pos="18 45 132 62" tooltip="Last Program Change sent"
         bkgCol="ffd0d0d0" outlineCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" labelText="128" editableSingleClick="0" editableDoubleClick="1"
         focusDiscardsChanges="0" fontname="Default font" fontsize="63.3"
         bold="1" italic="0" justification="36"/>
  <LABEL name="new label" id="c2ce2e439241d851" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="204 209 58 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Channel" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="353aeda7d51bcccb" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="41 231 41 23" edTextCol="ff000000"
         edBkgCol="0" labelText="Mode" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="bd317944291c6a08" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="174 120 33 24" edTextCol="ff000000"
         edBkgCol="0" labelText="MSB" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="28b3ac05e6748964" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="176 143 33 24" edTextCol="ff000000"
         edBkgCol="0" labelText="LSB" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="41ad2639bb80eb10" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="21 25 126 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Current Program" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <TOGGLEBUTTON name="Thru" id="804da0df783d655e" memberName="b_Thru" virtualName=""
                explicitFocusOrder="0" pos="214 253 54 24" buttonText="Thru"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="PCDisplay" id="38a2a5429ec87420" memberName="PCDisplay2"
         virtualName="" explicitFocusOrder="0" pos="193 83 42 22" bkgCol="ffcecece"
         outlineCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="128"
         editableSingleClick="0" editableDoubleClick="1" focusDiscardsChanges="0"
         fontname="Default font" fontsize="18" bold="1" italic="0" justification="36"/>
  <LABEL name="PCDisplay" id="c626351d5ce09a8b" memberName="PCDisplay3"
         virtualName="" explicitFocusOrder="0" pos="236 83 42 22" bkgCol="ffd0d0d0"
         outlineCol="ff000000" edTextCol="ff000000" edBkgCol="0" labelText="128"
         editableSingleClick="0" editableDoubleClick="1" focusDiscardsChanges="0"
         fontname="Default font" fontsize="18" bold="1" italic="0" justification="36"/>
  <LABEL name="new label" id="b228c4e04dd56cc" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="173 64 126 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Current Bank" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="36"/>
  <LABEL name="Program Name" id="3b96f9e82627ba71" memberName="ProgramName"
         virtualName="ClickableLabel" explicitFocusOrder="0" pos="18 106 132 30"
         bkgCol="ffcecece" outlineCol="ff000000" edTextCol="ff000000"
         edBkgCol="ffffffff" labelText="Awesome Thing" editableSingleClick="0"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="18" bold="1" italic="0" justification="36"/>
  <TEXTBUTTON name="Minimize" id="4bafc2904b05b5b4" memberName="minimize" virtualName=""
              explicitFocusOrder="0" pos="284 258 20 16" buttonText="_" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
