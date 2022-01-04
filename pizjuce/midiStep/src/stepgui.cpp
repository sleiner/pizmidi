/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  14 Sep 2011 10:46:54am

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
//[/Headers]

#include "stepgui.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
StepEditor::StepEditor (MidiStep* const ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      activeLoopLabel (0),
      resizer (0),
      viewport (0),
      recordButton (0),
      outChannelSlider (0),
      recArmButton1 (0),
      recArmButton2 (0),
      recArmButton3 (0),
      recArmButton4 (0),
      recArmButton5 (0),
      recArmButton6 (0),
      recArmButton7 (0),
      recArmButton8 (0),
      keySlider (0),
      recArmButton9 (0),
      recArmButton10 (0),
      recArmButton11 (0),
      recArmButton12 (0),
      recArmButton13 (0),
      recArmButton14 (0),
      recArmButton15 (0),
      recArmButton16 (0),
      recChannelSlider (0),
      transposeSlider (0),
      saveButton (0),
      component (0),
      toggleButton (0),
      addBarButton (0),
      deleteBarButton (0),
      thruButton (0)
{
    addAndMakeVisible (activeLoopLabel = new GroupComponent (L"new group",
                                                             L"16"));
    activeLoopLabel->setColour (GroupComponent::outlineColourId, Colour (0x66000000));

    addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));

    addAndMakeVisible (viewport = new PianoPort (L"new viewport"));
    viewport->setViewedComponent (new PianoRoll (this->getFilter()));

    addAndMakeVisible (recordButton = new TextButton (L"Record Button"));
    recordButton->setButtonText (L"Record");
    recordButton->addListener (this);
    recordButton->setColour (TextButton::buttonColourId, Colours::brown);
    recordButton->setColour (TextButton::buttonOnColourId, Colours::red);
    recordButton->setColour (TextButton::textColourOnId, Colours::white);

    addAndMakeVisible (outChannelSlider = new ChannelSlider (L"new slider"));
    outChannelSlider->setRange (0, 16, 1);
    outChannelSlider->setSliderStyle (Slider::IncDecButtons);
    outChannelSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 35, 20);
    outChannelSlider->setColour (Slider::textBoxTextColourId, Colours::black);
    outChannelSlider->addListener (this);

    addAndMakeVisible (recArmButton1 = new TextButton (L"new button"));
    recArmButton1->setButtonText (L"1");
    recArmButton1->addListener (this);

    addAndMakeVisible (recArmButton2 = new TextButton (L"new button"));
    recArmButton2->setButtonText (L"2");
    recArmButton2->addListener (this);

    addAndMakeVisible (recArmButton3 = new TextButton (L"new button"));
    recArmButton3->setButtonText (L"3");
    recArmButton3->addListener (this);

    addAndMakeVisible (recArmButton4 = new TextButton (L"new button"));
    recArmButton4->setButtonText (L"4");
    recArmButton4->addListener (this);

    addAndMakeVisible (recArmButton5 = new TextButton (L"new button"));
    recArmButton5->setButtonText (L"5");
    recArmButton5->addListener (this);

    addAndMakeVisible (recArmButton6 = new TextButton (L"new button"));
    recArmButton6->setButtonText (L"6");
    recArmButton6->addListener (this);

    addAndMakeVisible (recArmButton7 = new TextButton (L"new button"));
    recArmButton7->setButtonText (L"7");
    recArmButton7->addListener (this);

    addAndMakeVisible (recArmButton8 = new TextButton (L"new button"));
    recArmButton8->setButtonText (L"8");
    recArmButton8->addListener (this);

    addAndMakeVisible (keySlider = new Slider (L"new slider"));
    keySlider->setRange (-1, 127, 1);
    keySlider->setSliderStyle (Slider::IncDecButtons);
    keySlider->setTextBoxStyle (Slider::TextBoxLeft, false, 35, 20);
    keySlider->setColour (Slider::textBoxTextColourId, Colours::black);
    keySlider->addListener (this);

    addAndMakeVisible (recArmButton9 = new TextButton (L"new button"));
    recArmButton9->setButtonText (L"9");
    recArmButton9->addListener (this);

    addAndMakeVisible (recArmButton10 = new TextButton (L"new button"));
    recArmButton10->setButtonText (L"10");
    recArmButton10->addListener (this);

    addAndMakeVisible (recArmButton11 = new TextButton (L"new button"));
    recArmButton11->setButtonText (L"11");
    recArmButton11->addListener (this);

    addAndMakeVisible (recArmButton12 = new TextButton (L"new button"));
    recArmButton12->setButtonText (L"12");
    recArmButton12->addListener (this);

    addAndMakeVisible (recArmButton13 = new TextButton (L"new button"));
    recArmButton13->setButtonText (L"13");
    recArmButton13->addListener (this);

    addAndMakeVisible (recArmButton14 = new TextButton (L"new button"));
    recArmButton14->setButtonText (L"14");
    recArmButton14->addListener (this);

    addAndMakeVisible (recArmButton15 = new TextButton (L"new button"));
    recArmButton15->setButtonText (L"15");
    recArmButton15->addListener (this);

    addAndMakeVisible (recArmButton16 = new TextButton (L"new button"));
    recArmButton16->setButtonText (L"16");
    recArmButton16->addListener (this);

    addAndMakeVisible (recChannelSlider = new ChannelSlider (L"new slider"));
    recChannelSlider->setRange (0, 16, 1);
    recChannelSlider->setSliderStyle (Slider::IncDecButtons);
    recChannelSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 35, 20);
    recChannelSlider->setColour (Slider::textBoxTextColourId, Colours::black);
    recChannelSlider->addListener (this);

    addAndMakeVisible (transposeSlider = new Slider (L"new slider"));
    transposeSlider->setRange (-48, 48, 1);
    transposeSlider->setSliderStyle (Slider::IncDecButtons);
    transposeSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 35, 20);
    transposeSlider->setColour (Slider::textBoxTextColourId, Colours::black);
    transposeSlider->addListener (this);

    addAndMakeVisible (saveButton = new TextButton (L"new button"));
    saveButton->setButtonText (L"Export...");
    saveButton->addListener (this);

    addAndMakeVisible (component = new PizButton());
    addAndMakeVisible (toggleButton = new ToggleButton (L"new toggle button"));
    toggleButton->setButtonText (L"active");
    toggleButton->addListener (this);

    addAndMakeVisible (addBarButton = new TextButton (L"new button"));
    addBarButton->setButtonText (L"Add Bar");
    addBarButton->addListener (this);

    addAndMakeVisible (deleteBarButton = new TextButton (L"new button"));
    deleteBarButton->setButtonText (L"Del Bar");
    deleteBarButton->addListener (this);

    addAndMakeVisible (thruButton = new ToggleButton (L"thru button"));
    thruButton->setButtonText (L"thru");
    thruButton->addListener (this);


    //[UserPreSize]
	resizeLimits.setSizeLimits (510, 360, 1600, 1600);
    //[/UserPreSize]

    setSize (610, 360);


    //[Constructor] You can add your own custom stuff here..
	ownerFilter->addChangeListener(this);
	pianoRoll = (PianoRoll*)viewport->getViewedComponent();
	//pianoRoll->addChangeListener(this);

	pianoRoll->setSize(500,1200);
	pianoRoll->setSequence(ownerFilter->getActiveLoop());
	lastActiveLoop = ownerFilter->activeLoop;
	viewport->setViewPosition(0,pianoRoll->getHeight()/2-viewport->getHeight()/2);

	this->startTimer(1000/15);
	this->updateParameters(true);
	resized();
    //[/Constructor]
}

StepEditor::~StepEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	this->stopTimer();
	getFilter()->removeChangeListener (this);
    //[/Destructor_pre]

    deleteAndZero (activeLoopLabel);
    deleteAndZero (resizer);
    deleteAndZero (viewport);
    deleteAndZero (recordButton);
    deleteAndZero (outChannelSlider);
    deleteAndZero (recArmButton1);
    deleteAndZero (recArmButton2);
    deleteAndZero (recArmButton3);
    deleteAndZero (recArmButton4);
    deleteAndZero (recArmButton5);
    deleteAndZero (recArmButton6);
    deleteAndZero (recArmButton7);
    deleteAndZero (recArmButton8);
    deleteAndZero (keySlider);
    deleteAndZero (recArmButton9);
    deleteAndZero (recArmButton10);
    deleteAndZero (recArmButton11);
    deleteAndZero (recArmButton12);
    deleteAndZero (recArmButton13);
    deleteAndZero (recArmButton14);
    deleteAndZero (recArmButton15);
    deleteAndZero (recArmButton16);
    deleteAndZero (recChannelSlider);
    deleteAndZero (transposeSlider);
    deleteAndZero (saveButton);
    deleteAndZero (component);
    deleteAndZero (toggleButton);
    deleteAndZero (addBarButton);
    deleteAndZero (deleteBarButton);
    deleteAndZero (thruButton);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void StepEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffdddddd));

    g.setGradientFill (ColourGradient (Colour (0xa65f5f5f),
                                       (float) (getWidth() - 78), 0.0f,
                                       Colour (0xffa7a7a7),
                                       (float) (getWidth() - 62), 0.0f,
                                       false));
    g.fillRect (getWidth() - 80, 0, 80, proportionOfHeight (1.0000f));

    g.setColour (Colours::black);
    g.setFont (Font (L"hooge 05_55", 15.0000f, Font::plain));
    g.drawText (L"Transp.",
                getWidth() - 9 - 63, 246, 63, 16,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (L"hooge 05_55", 15.0000f, Font::plain));
    g.drawText (L"Rec Ch.",
                getWidth() - 8 - 63, 166, 63, 16,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (L"hooge 05_55", 15.0000f, Font::plain));
    g.drawText (L"Trig. Key",
                getWidth() - 8 - 63, 206, 63, 16,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.setFont (Font (L"hooge 05_55", 15.0000f, Font::plain));
    g.drawText (L"Out Ch.",
                getWidth() - 9 - 63, 281, 63, 16,
                Justification::centred, true);

    g.setColour (Colours::black);
    g.fillPath (internalPath1);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void StepEditor::resized()
{
    activeLoopLabel->setBounds (getWidth() - 2 - 73, 154, 73, 163);
    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
    viewport->setBounds (8, 32, getWidth() - 89, getHeight() - 34);
    recordButton->setBounds (getWidth() - 6 - 64, 8, 64, 43);
    outChannelSlider->setBounds (getWidth() - 10 - 57, 295, 57, 16);
    recArmButton1->setBounds (getWidth() - 55 - 20, 71, 20, 18);
    recArmButton2->setBounds (getWidth() - 37 - 20, 71, 20, 18);
    recArmButton3->setBounds (getWidth() - 19 - 20, 71, 20, 18);
    recArmButton4->setBounds (getWidth() - 21, 71, 20, 18);
    recArmButton5->setBounds (getWidth() - 55 - 20, 92, 20, 18);
    recArmButton6->setBounds (getWidth() - 37 - 20, 92, 20, 18);
    recArmButton7->setBounds (getWidth() - 19 - 20, 92, 20, 18);
    recArmButton8->setBounds (getWidth() - 21, 92, 20, 18);
    keySlider->setBounds (getWidth() - 10 - 57, 220, 57, 16);
    recArmButton9->setBounds (getWidth() - 55 - 20, 113, 20, 18);
    recArmButton10->setBounds (getWidth() - 37 - 20, 113, 20, 18);
    recArmButton11->setBounds (getWidth() - 19 - 20, 113, 20, 18);
    recArmButton12->setBounds (getWidth() - 21, 113, 20, 18);
    recArmButton13->setBounds (getWidth() - 55 - 20, 134, 20, 18);
    recArmButton14->setBounds (getWidth() - 37 - 20, 134, 20, 18);
    recArmButton15->setBounds (getWidth() - 19 - 20, 134, 20, 18);
    recArmButton16->setBounds (getWidth() - 21, 134, 20, 18);
    recChannelSlider->setBounds (getWidth() - 10 - 57, 180, 57, 16);
    transposeSlider->setBounds (getWidth() - 10 - 57, 260, 57, 16);
    saveButton->setBounds (getWidth() - 14 - 54, 333, 54, 18);
    component->setBounds (8, 5, 48, 24);
    toggleButton->setBounds (getWidth() - 6 - 61, 49, 61, 21);
    addBarButton->setBounds (445, -30, 29, 24);
    deleteBarButton->setBounds (413, -30, 29, 24);
    thruButton->setBounds (getWidth() - 67, 313, 61, 21);
    internalPath1.clear();
    internalPath1.startNewSubPath (60.0f, 10.0f);
    internalPath1.lineTo (65.0f, 10.0f);
    internalPath1.lineTo (65.0f, 23.0f);
    internalPath1.lineTo (60.0f, 23.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (68.0f, 12.0f);
    internalPath1.lineTo (81.0f, 12.0f);
    internalPath1.lineTo (81.0f, 23.0f);
    internalPath1.lineTo (76.0f, 23.0f);
    internalPath1.lineTo (76.0f, 15.0f);
    internalPath1.lineTo (73.0f, 15.0f);
    internalPath1.lineTo (73.0f, 23.0f);
    internalPath1.lineTo (68.0f, 23.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (97.0f, 15.0f);
    internalPath1.lineTo (91.0f, 15.0f);
    internalPath1.lineTo (91.0f, 18.0f);
    internalPath1.lineTo (97.0f, 18.0f);
    internalPath1.lineTo (97.0f, 20.0f);
    internalPath1.lineTo (94.0f, 20.0f);
    internalPath1.lineTo (94.0f, 23.0f);
    internalPath1.lineTo (84.0f, 23.0f);
    internalPath1.lineTo (84.0f, 20.0f);
    internalPath1.lineTo (89.0f, 20.0f);
    internalPath1.lineTo (89.0f, 18.0f);
    internalPath1.lineTo (84.0f, 18.0f);
    internalPath1.lineTo (84.0f, 15.0f);
    internalPath1.lineTo (86.0f, 15.0f);
    internalPath1.lineTo (86.0f, 12.0f);
    internalPath1.lineTo (97.0f, 12.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (99.0f, 23.0f);
    internalPath1.lineTo (99.0f, 12.0f);
    internalPath1.lineTo (112.0f, 12.0f);
    internalPath1.lineTo (112.0f, 18.0f);
    internalPath1.lineTo (110.0f, 18.0f);
    internalPath1.lineTo (110.0f, 20.0f);
    internalPath1.lineTo (112.0f, 20.0f);
    internalPath1.lineTo (112.0f, 23.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (105.0f, 18.0f);
    internalPath1.lineTo (107.0f, 18.0f);
    internalPath1.lineTo (107.0f, 15.0f);
    internalPath1.lineTo (105.0f, 15.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (126.0f, 12.0f);
    internalPath1.lineTo (126.0f, 15.0f);
    internalPath1.lineTo (120.0f, 15.0f);
    internalPath1.lineTo (120.0f, 23.0f);
    internalPath1.lineTo (115.0f, 23.0f);
    internalPath1.lineTo (115.0f, 12.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (139.0f, 12.0f);
    internalPath1.lineTo (139.0f, 15.0f);
    internalPath1.lineTo (136.0f, 15.0f);
    internalPath1.lineTo (136.0f, 23.0f);
    internalPath1.lineTo (131.0f, 23.0f);
    internalPath1.lineTo (131.0f, 15.0f);
    internalPath1.lineTo (128.0f, 15.0f);
    internalPath1.lineTo (128.0f, 12.0f);
    internalPath1.lineTo (131.0f, 12.0f);
    internalPath1.lineTo (131.0f, 10.0f);
    internalPath1.lineTo (136.0f, 10.0f);
    internalPath1.lineTo (136.0f, 12.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (162.0f, 10.0f);
    internalPath1.lineTo (162.0f, 18.0f);
    internalPath1.lineTo (154.0f, 18.0f);
    internalPath1.lineTo (154.0f, 23.0f);
    internalPath1.lineTo (149.0f, 23.0f);
    internalPath1.lineTo (149.0f, 10.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (154.0f, 15.0f);
    internalPath1.lineTo (157.0f, 15.0f);
    internalPath1.lineTo (157.0f, 12.0f);
    internalPath1.lineTo (154.0f, 12.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (165.0f, 23.0f);
    internalPath1.lineTo (165.0f, 15.0f);
    internalPath1.lineTo (170.0f, 15.0f);
    internalPath1.lineTo (170.0f, 23.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (165.0f, 10.0f);
    internalPath1.lineTo (170.0f, 10.0f);
    internalPath1.lineTo (170.0f, 12.0f);
    internalPath1.lineTo (165.0f, 12.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (178.0f, 18.0f);
    internalPath1.lineTo (178.0f, 15.0f);
    internalPath1.lineTo (173.0f, 15.0f);
    internalPath1.lineTo (173.0f, 12.0f);
    internalPath1.lineTo (186.0f, 12.0f);
    internalPath1.lineTo (186.0f, 15.0f);
    internalPath1.lineTo (183.0f, 15.0f);
    internalPath1.lineTo (183.0f, 18.0f);
    internalPath1.lineTo (181.0f, 18.0f);
    internalPath1.lineTo (181.0f, 20.0f);
    internalPath1.lineTo (186.0f, 20.0f);
    internalPath1.lineTo (186.0f, 23.0f);
    internalPath1.lineTo (173.0f, 23.0f);
    internalPath1.lineTo (173.0f, 20.0f);
    internalPath1.lineTo (175.0f, 20.0f);
    internalPath1.lineTo (175.0f, 18.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (204.0f, 10.0f);
    internalPath1.lineTo (209.0f, 10.0f);
    internalPath1.lineTo (209.0f, 23.0f);
    internalPath1.lineTo (204.0f, 23.0f);
    internalPath1.lineTo (204.0f, 18.0f);
    internalPath1.lineTo (202.0f, 18.0f);
    internalPath1.lineTo (202.0f, 23.0f);
    internalPath1.lineTo (196.0f, 23.0f);
    internalPath1.lineTo (196.0f, 10.0f);
    internalPath1.lineTo (202.0f, 10.0f);
    internalPath1.lineTo (202.0f, 15.0f);
    internalPath1.lineTo (204.0f, 15.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (212.0f, 23.0f);
    internalPath1.lineTo (212.0f, 12.0f);
    internalPath1.lineTo (225.0f, 12.0f);
    internalPath1.lineTo (225.0f, 18.0f);
    internalPath1.lineTo (222.0f, 18.0f);
    internalPath1.lineTo (222.0f, 20.0f);
    internalPath1.lineTo (225.0f, 20.0f);
    internalPath1.lineTo (225.0f, 23.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (217.0f, 18.0f);
    internalPath1.lineTo (220.0f, 18.0f);
    internalPath1.lineTo (220.0f, 15.0f);
    internalPath1.lineTo (217.0f, 15.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (238.0f, 12.0f);
    internalPath1.lineTo (238.0f, 15.0f);
    internalPath1.lineTo (233.0f, 15.0f);
    internalPath1.lineTo (233.0f, 23.0f);
    internalPath1.lineTo (228.0f, 23.0f);
    internalPath1.lineTo (228.0f, 12.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (241.0f, 23.0f);
    internalPath1.lineTo (241.0f, 12.0f);
    internalPath1.lineTo (254.0f, 12.0f);
    internalPath1.lineTo (254.0f, 18.0f);
    internalPath1.lineTo (251.0f, 18.0f);
    internalPath1.lineTo (251.0f, 20.0f);
    internalPath1.lineTo (254.0f, 20.0f);
    internalPath1.lineTo (254.0f, 23.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (246.0f, 18.0f);
    internalPath1.lineTo (249.0f, 18.0f);
    internalPath1.lineTo (249.0f, 15.0f);
    internalPath1.lineTo (246.0f, 15.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (257.0f, 18.0f);
    internalPath1.lineTo (257.0f, 15.0f);
    internalPath1.lineTo (270.0f, 15.0f);
    internalPath1.lineTo (270.0f, 18.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (272.0f, 10.0f);
    internalPath1.lineTo (277.0f, 10.0f);
    internalPath1.lineTo (277.0f, 12.0f);
    internalPath1.lineTo (280.0f, 12.0f);
    internalPath1.lineTo (280.0f, 15.0f);
    internalPath1.lineTo (283.0f, 15.0f);
    internalPath1.lineTo (283.0f, 18.0f);
    internalPath1.lineTo (280.0f, 18.0f);
    internalPath1.lineTo (280.0f, 20.0f);
    internalPath1.lineTo (277.0f, 20.0f);
    internalPath1.lineTo (277.0f, 23.0f);
    internalPath1.lineTo (272.0f, 23.0f);
    internalPath1.lineTo (272.0f, 20.0f);
    internalPath1.lineTo (275.0f, 20.0f);
    internalPath1.lineTo (275.0f, 18.0f);
    internalPath1.lineTo (277.0f, 18.0f);
    internalPath1.lineTo (277.0f, 15.0f);
    internalPath1.lineTo (275.0f, 15.0f);
    internalPath1.lineTo (275.0f, 12.0f);
    internalPath1.lineTo (272.0f, 12.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (314.0f, 12.0f);
    internalPath1.lineTo (314.0f, 23.0f);
    internalPath1.lineTo (309.0f, 23.0f);
    internalPath1.lineTo (309.0f, 15.0f);
    internalPath1.lineTo (306.0f, 15.0f);
    internalPath1.lineTo (306.0f, 23.0f);
    internalPath1.lineTo (301.0f, 23.0f);
    internalPath1.lineTo (301.0f, 15.0f);
    internalPath1.lineTo (298.0f, 15.0f);
    internalPath1.lineTo (298.0f, 23.0f);
    internalPath1.lineTo (293.0f, 23.0f);
    internalPath1.lineTo (293.0f, 12.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (317.0f, 23.0f);
    internalPath1.lineTo (317.0f, 15.0f);
    internalPath1.lineTo (322.0f, 15.0f);
    internalPath1.lineTo (322.0f, 23.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (317.0f, 10.0f);
    internalPath1.lineTo (322.0f, 10.0f);
    internalPath1.lineTo (322.0f, 12.0f);
    internalPath1.lineTo (317.0f, 12.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (325.0f, 23.0f);
    internalPath1.lineTo (325.0f, 12.0f);
    internalPath1.lineTo (332.0f, 12.0f);
    internalPath1.lineTo (332.0f, 10.0f);
    internalPath1.lineTo (338.0f, 10.0f);
    internalPath1.lineTo (338.0f, 23.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (330.0f, 15.0f);
    internalPath1.lineTo (330.0f, 20.0f);
    internalPath1.lineTo (332.0f, 20.0f);
    internalPath1.lineTo (332.0f, 15.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (340.0f, 23.0f);
    internalPath1.lineTo (340.0f, 15.0f);
    internalPath1.lineTo (346.0f, 15.0f);
    internalPath1.lineTo (346.0f, 23.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (340.0f, 10.0f);
    internalPath1.lineTo (346.0f, 10.0f);
    internalPath1.lineTo (346.0f, 12.0f);
    internalPath1.lineTo (340.0f, 12.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (361.0f, 12.0f);
    internalPath1.lineTo (353.0f, 12.0f);
    internalPath1.lineTo (353.0f, 15.0f);
    internalPath1.lineTo (361.0f, 15.0f);
    internalPath1.lineTo (361.0f, 23.0f);
    internalPath1.lineTo (348.0f, 23.0f);
    internalPath1.lineTo (348.0f, 20.0f);
    internalPath1.lineTo (356.0f, 20.0f);
    internalPath1.lineTo (356.0f, 18.0f);
    internalPath1.lineTo (348.0f, 18.0f);
    internalPath1.lineTo (348.0f, 10.0f);
    internalPath1.lineTo (361.0f, 10.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (374.0f, 12.0f);
    internalPath1.lineTo (374.0f, 15.0f);
    internalPath1.lineTo (372.0f, 15.0f);
    internalPath1.lineTo (372.0f, 23.0f);
    internalPath1.lineTo (367.0f, 23.0f);
    internalPath1.lineTo (367.0f, 15.0f);
    internalPath1.lineTo (364.0f, 15.0f);
    internalPath1.lineTo (364.0f, 12.0f);
    internalPath1.lineTo (367.0f, 12.0f);
    internalPath1.lineTo (367.0f, 10.0f);
    internalPath1.lineTo (372.0f, 10.0f);
    internalPath1.lineTo (372.0f, 12.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (377.0f, 23.0f);
    internalPath1.lineTo (377.0f, 12.0f);
    internalPath1.lineTo (390.0f, 12.0f);
    internalPath1.lineTo (390.0f, 18.0f);
    internalPath1.lineTo (388.0f, 18.0f);
    internalPath1.lineTo (388.0f, 20.0f);
    internalPath1.lineTo (390.0f, 20.0f);
    internalPath1.lineTo (390.0f, 23.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (382.0f, 18.0f);
    internalPath1.lineTo (385.0f, 18.0f);
    internalPath1.lineTo (385.0f, 15.0f);
    internalPath1.lineTo (382.0f, 15.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (406.0f, 12.0f);
    internalPath1.lineTo (406.0f, 23.0f);
    internalPath1.lineTo (398.0f, 23.0f);
    internalPath1.lineTo (398.0f, 28.0f);
    internalPath1.lineTo (393.0f, 28.0f);
    internalPath1.lineTo (393.0f, 12.0f);
    internalPath1.closeSubPath();
    internalPath1.startNewSubPath (401.0f, 20.0f);
    internalPath1.lineTo (401.0f, 15.0f);
    internalPath1.lineTo (398.0f, 15.0f);
    internalPath1.lineTo (398.0f, 20.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
	getFilter()->lastUIHeight=getHeight();
	getFilter()->lastUIWidth=getWidth();
    //[/UserResized]
}

void StepEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == recordButton)
    {
        //[UserButtonCode_recordButton] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kRecord,recordButton->getToggleState()?0.f:1.f);
		recordButton->setToggleState(!recordButton->getToggleState(),false);
        //[/UserButtonCode_recordButton]
    }
    else if (buttonThatWasClicked == recArmButton1)
    {
        //[UserButtonCode_recArmButton1] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton1]
    }
    else if (buttonThatWasClicked == recArmButton2)
    {
        //[UserButtonCode_recArmButton2] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton2]
    }
    else if (buttonThatWasClicked == recArmButton3)
    {
        //[UserButtonCode_recArmButton3] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton3]
    }
    else if (buttonThatWasClicked == recArmButton4)
    {
        //[UserButtonCode_recArmButton4] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton4]
    }
    else if (buttonThatWasClicked == recArmButton5)
    {
        //[UserButtonCode_recArmButton5] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton5]
    }
    else if (buttonThatWasClicked == recArmButton6)
    {
        //[UserButtonCode_recArmButton6] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton6]
    }
    else if (buttonThatWasClicked == recArmButton7)
    {
        //[UserButtonCode_recArmButton7] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton7]
    }
    else if (buttonThatWasClicked == recArmButton8)
    {
        //[UserButtonCode_recArmButton8] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton8]
    }
    else if (buttonThatWasClicked == recArmButton9)
    {
        //[UserButtonCode_recArmButton9] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton9]
    }
    else if (buttonThatWasClicked == recArmButton10)
    {
        //[UserButtonCode_recArmButton10] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton10]
    }
    else if (buttonThatWasClicked == recArmButton11)
    {
        //[UserButtonCode_recArmButton11] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton11]
    }
    else if (buttonThatWasClicked == recArmButton12)
    {
        //[UserButtonCode_recArmButton12] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton12]
    }
    else if (buttonThatWasClicked == recArmButton13)
    {
        //[UserButtonCode_recArmButton13] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton13]
    }
    else if (buttonThatWasClicked == recArmButton14)
    {
        //[UserButtonCode_recArmButton14] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton14]
    }
    else if (buttonThatWasClicked == recArmButton15)
    {
        //[UserButtonCode_recArmButton15] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton15]
    }
    else if (buttonThatWasClicked == recArmButton16)
    {
        //[UserButtonCode_recArmButton16] -- add your button handler code here..
		recArmButtonClicked(buttonThatWasClicked);
        //[/UserButtonCode_recArmButton16]
    }
    else if (buttonThatWasClicked == saveButton)
    {
        //[UserButtonCode_saveButton] -- add your button handler code here..
        FileChooser myChooser ("Export MIDI...",
            File(getFilter()->loopDir + File::getSeparatorString() + "Untitled.mid"),"*.mid");

        if (myChooser.browseForFileToSave(true))
        {
            File midiFile (myChooser.getResult());
            if (!midiFile.hasFileExtension("mid")) midiFile = midiFile.withFileExtension("mid");

			getFilter()->writeMidiFile(getFilter()->activeLoop,midiFile);
        }
        //[/UserButtonCode_saveButton]
    }
    else if (buttonThatWasClicked == toggleButton)
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kRecActive,toggleButton->getToggleState()?1.f:0.f);
        //[/UserButtonCode_toggleButton]
    }
    else if (buttonThatWasClicked == addBarButton)
    {
        //[UserButtonCode_addBarButton] -- add your button handler code here..
		pianoRoll->blankLength+=960.0*4.0;
		pianoRoll->setSize(pianoRoll->getWidth()+roundFloatToInt(pianoRoll->ppqToPixels(960.0*4.0)),pianoRoll->getHeight());
		pianoRoll->sequenceChanged();
        //[/UserButtonCode_addBarButton]
    }
    else if (buttonThatWasClicked == deleteBarButton)
    {
        //[UserButtonCode_deleteBarButton] -- add your button handler code here..
		pianoRoll->blankLength-=960.0*4.0;
		if (pianoRoll->blankLength<960.0*4.0) pianoRoll->blankLength = 960.0*4.0;
		pianoRoll->setSize(roundFloatToInt(pianoRoll->ppqToPixels(pianoRoll->blankLength)),pianoRoll->getHeight());
		pianoRoll->sequenceChanged();
        //[/UserButtonCode_deleteBarButton]
    }
    else if (buttonThatWasClicked == thruButton)
    {
        //[UserButtonCode_thruButton] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kThru,thruButton->getToggleState()?1.f:0.f);
        //[/UserButtonCode_thruButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void StepEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == outChannelSlider)
    {
        //[UserSliderCode_outChannelSlider] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kOutChannel+getFilter()->activeLoop,channelToFloat((int)outChannelSlider->getValue()));
        //[/UserSliderCode_outChannelSlider]
    }
    else if (sliderThatWasMoved == keySlider)
    {
        //[UserSliderCode_keySlider] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kTriggerKey+getFilter()->activeLoop,midiToFloat((int)keySlider->getValue(),true));
        //[/UserSliderCode_keySlider]
    }
    else if (sliderThatWasMoved == recChannelSlider)
    {
        //[UserSliderCode_recChannelSlider] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kChannel+getFilter()->activeLoop,channelToFloat((int)recChannelSlider->getValue()));
        //[/UserSliderCode_recChannelSlider]
    }
    else if (sliderThatWasMoved == transposeSlider)
    {
        //[UserSliderCode_transposeSlider] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kTranspose+getFilter()->activeLoop,(float)((transposeSlider->getValue()+48.0)/96.0));
        //[/UserSliderCode_transposeSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
TextButton* StepEditor::getButtonByIndex(int i)
{
	switch(i)
	{
	case 0: return recArmButton1;
	case 1: return recArmButton2;
	case 2: return recArmButton3;
	case 3: return recArmButton4;
	case 4: return recArmButton5;
	case 5: return recArmButton6;
	case 6: return recArmButton7;
	case 7: return recArmButton8;
	case 8: return recArmButton9;
	case 9: return recArmButton10;
	case 10: return recArmButton11;
	case 11: return recArmButton12;
	case 12: return recArmButton13;
	case 13: return recArmButton14;
	case 14: return recArmButton15;
	case 15: return recArmButton16;
	default: return 0;
	}
}

void StepEditor::recArmButtonClicked(Button* buttonThatWasClicked)
{
	int index = buttonThatWasClicked->getButtonText().getIntValue()-1;
	if (ModifierKeys::getCurrentModifiers().isCommandDown()) {
		buttonThatWasClicked->setToggleState(!buttonThatWasClicked->getToggleState(),false);
		getFilter()->setParameterNotifyingHost(kRecArm+index, buttonThatWasClicked->getToggleState() ? 1.f : 0.f);
	} else {
		getFilter()->setParameterNotifyingHost(kActiveLoop,(float)index/(float)(numLoops-1));
		pianoRoll->setSequence(getFilter()->getActiveLoop());
		activeLoopLabel->setText(String(getFilter()->activeLoop+1));
		for (int i=0;i<numLoops;i++)
		{
			getButtonByIndex(i)->setToggleState(i==index ? true : false,false);
			if (toggleButton->getToggleState()) getFilter()->setParameterNotifyingHost(kRecArm+i, i==index ? 1.f : 0.f);
		}
	}
}

bool StepEditor::isInterestedInFileDrag (const StringArray& files){
    File file = File(files[0]);
    if ( file.hasFileExtension("mid") ) return true;
    return false;
}

void StepEditor::filesDropped (const StringArray& filenames, int mouseX, int mouseY)
{
    File file(filenames[0]);
	if(getFilter()->readMidiFile(getFilter()->activeLoop,file))
		updateParameters(true);
}

void StepEditor::mouseWheelMove (const MouseEvent &e, float wheelIncrementX, float wheelIncrementY)
{
	if (e.eventComponent==viewport)
	{
		if (wheelIncrementY>0.f) zoomIn(e.getEventRelativeTo(pianoRoll).x);
		else if (wheelIncrementY<0.f) zoomOut(e.getEventRelativeTo(pianoRoll).x);
	}
}

void StepEditor::timerCallback ()
{
	if (getFilter()->getActiveLoop()->isRecording)
	{
		int x = roundFloatToInt(pianoRoll->ppqToPixels(getFilter()->getActiveLoop()->recTime));
		//if (x < viewport->getWidth()-30) x=0;
		if (x>pianoRoll->getWidth()) {
			getFilter()->setParameterNotifyingHost(kRecord,0.f);
			//pianoRoll->blankLength+=960.0*4.0;
			//pianoRoll->setSize(pianoRoll->getWidth()+roundFloatToInt(pianoRoll->ppqToPixels(960.0*4.0)),pianoRoll->getHeight());
			//pianoRoll->sequenceChanged();
			////pianoRoll->setSize(x,pianoRoll->getHeight());
			//viewport->setViewPosition(x-pianoRoll->getWidth(),viewport->getViewPositionY());
		}
		pianoRoll->repaint();
	}
}

void StepEditor::changeListenerCallback (ChangeBroadcaster* source)
{
	if(source==getFilter())
		updateParameters();
}

void StepEditor::updateParameters (bool updateLoop)
{
	MidiStep* const filter = getFilter();
    float param[kNumParams];

    filter->getCallbackLock().enter();
		for (int i=0;i<kNumParams;i++) {
			param[i] = filter->getParameter(i);
		}
		const int activeLoop = filter->activeLoop;
		const int w = filter->lastUIWidth;
		const int h = filter->lastUIHeight;
	filter->getCallbackLock().exit();

	for (int i=0;i<numLoops;i++) {
		getButtonByIndex(i)->setColour(TextButton::textColourOnId,i==activeLoop ? Colours::red : Colours::black);
		getButtonByIndex(i)->setToggleState(param[kRecArm+i]>=0.5f,dontSendNotification);
	}
	thruButton->setToggleState(param[kThru]>=0.5f,dontSendNotification);
	toggleButton->setToggleState(param[kRecActive]>=0.5f,dontSendNotification);
	recordButton->setToggleState(param[kRecord]>=0.5f,dontSendNotification);
	keySlider->setValue(floatToMidi(param[kTriggerKey+activeLoop],true),dontSendNotification);
	transposeSlider->setValue(param[kTranspose+activeLoop]*96.f - 48.f,dontSendNotification);
	recChannelSlider->setValue(floatToChannel(param[kChannel+activeLoop]),dontSendNotification);
	outChannelSlider->setValue(floatToChannel(param[kOutChannel+activeLoop]),dontSendNotification);
	if (updateLoop || lastActiveLoop!=activeLoop)
	{
		lastActiveLoop=activeLoop;
		pianoRoll->setSequence(getFilter()->getActiveLoop());
		activeLoopLabel->setText(String(activeLoop+1));
	}
	else repaint();
	setSize(w,h);
}

void StepEditor::zoomIn(int centerPixel)
{
	pianoRoll->setSize(roundFloatToInt((float)pianoRoll->getWidth()*1.1f),pianoRoll->getHeight());
	viewport->setViewPosition(jlimit(0,pianoRoll->getWidth()-viewport->getViewWidth(),centerPixel-viewport->getViewWidth()/2),viewport->getViewPositionY());
}

void StepEditor::zoomOut(int centerPixel)
{
	pianoRoll->setSize(roundFloatToInt((float)pianoRoll->getWidth()*0.9f),pianoRoll->getHeight());
	viewport->setViewPosition(jlimit(0,pianoRoll->getWidth()-viewport->getViewWidth(),centerPixel-viewport->getViewWidth()/2),viewport->getViewPositionY());
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="StepEditor" componentName=""
                 parentClasses="public AudioProcessorEditor, public ChangeListener, public FileDragAndDropTarget, public Timer"
                 constructorParams="MidiStep* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="0" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="610" initialHeight="360">
  <BACKGROUND backgroundColour="ffdddddd">
    <RECT pos="80R 0 80 100%" fill="linear: 78R 0, 62R 0, 0=a65f5f5f, 1=ffa7a7a7"
          hasStroke="0"/>
    <TEXT pos="9Rr 246 63 16" fill="solid: ff000000" hasStroke="0" text="Transp."
          fontname="hooge 05_55" fontsize="15" bold="0" italic="0" justification="36"/>
    <TEXT pos="8Rr 166 63 16" fill="solid: ff000000" hasStroke="0" text="Rec Ch."
          fontname="hooge 05_55" fontsize="15" bold="0" italic="0" justification="36"/>
    <TEXT pos="8Rr 206 63 16" fill="solid: ff000000" hasStroke="0" text="Trig. Key"
          fontname="hooge 05_55" fontsize="15" bold="0" italic="0" justification="36"/>
    <TEXT pos="9Rr 281 63 16" fill="solid: ff000000" hasStroke="0" text="Out Ch."
          fontname="hooge 05_55" fontsize="15" bold="0" italic="0" justification="36"/>
    <PATH pos="0 0 100 100" fill="solid: ff000000" hasStroke="0" nonZeroWinding="1">s 60.02 9.76 l 65.26 9.76 l 65.26 22.86 l 60.02 22.86 x s 67.88 12.38 l 80.98 12.38 l 80.98 22.86 l 75.74 22.86 l 75.74 15 l 73.12 15 l 73.12 22.86 l 67.88 22.86 x s 96.7 15 l 91.46 15 l 91.46 17.62 l 96.7 17.62 l 96.7 20.24 l 94.08 20.24 l 94.08 22.86 l 83.6 22.86 l 83.6 20.24 l 88.84 20.24 l 88.84 17.62 l 83.6 17.62 l 83.6 15 l 86.22 15 l 86.22 12.38 l 96.7 12.38 x s 99.32 22.86 l 99.32 12.38 l 112.42 12.38 l 112.42 17.62 l 109.8 17.62 l 109.8 20.24 l 112.42 20.24 l 112.42 22.86 x s 104.56 17.52 l 107.18 17.52 l 107.18 15.1 l 104.56 15.1 x s 125.52 12.38 l 125.52 15 l 120.28 15 l 120.28 22.86 l 115.04 22.86 l 115.04 12.38 x s 138.62 12.38 l 138.62 15 l 136 15 l 136 22.86 l 130.76 22.86 l 130.76 15 l 128.14 15 l 128.14 12.38 l 130.76 12.38 l 130.76 9.76 l 136 9.76 l 136 12.38 x s 162.2 9.76 l 162.2 17.62 l 154.34 17.62 l 154.34 22.86 l 149.1 22.86 l 149.1 9.76 x s 154.34 15 l 156.96 15 l 156.96 12.38 l 154.34 12.38 x s 164.82 22.86 l 164.82 15 l 170.06 15 l 170.06 22.86 x s 164.82 9.76 l 170.06 9.76 l 170.06 12.38 l 164.82 12.38 x s 177.92 17.62 l 177.92 15 l 172.68 15 l 172.68 12.38 l 185.78 12.38 l 185.78 15 l 183.16 15 l 183.16 17.62 l 180.54 17.62 l 180.54 20.24 l 185.78 20.24 l 185.78 22.86 l 172.68 22.86 l 172.68 20.24 l 175.3 20.24 l 175.3 17.62 x s 204.12 9.76 l 209.36 9.76 l 209.36 22.86 l 204.12 22.86 l 204.12 17.62 l 201.5 17.62 l 201.5 22.86 l 196.26 22.86 l 196.26 9.76 l 201.5 9.76 l 201.5 15 l 204.12 15 x s 211.98 22.86 l 211.98 12.38 l 225.08 12.38 l 225.08 17.62 l 222.46 17.62 l 222.46 20.24 l 225.08 20.24 l 225.08 22.86 x s 217.22 17.52 l 219.84 17.52 l 219.84 15.1 l 217.22 15.1 x s 238.18 12.38 l 238.18 15 l 232.94 15 l 232.94 22.86 l 227.7 22.86 l 227.7 12.38 x s 240.8 22.86 l 240.8 12.38 l 253.9 12.38 l 253.9 17.62 l 251.28 17.62 l 251.28 20.24 l 253.9 20.24 l 253.9 22.86 x s 246.04 17.52 l 248.66 17.52 l 248.66 15.1 l 246.04 15.1 x s 256.52 17.62 l 256.52 15 l 269.62 15 l 269.62 17.62 x s 272.24 9.76 l 277.48 9.76 l 277.48 12.38 l 280.1 12.38 l 280.1 15 l 282.72 15 l 282.72 17.62 l 280.1 17.62 l 280.1 20.24 l 277.48 20.24 l 277.48 22.86 l 272.24 22.86 l 272.24 20.24 l 274.86 20.24 l 274.86 17.62 l 277.48 17.62 l 277.48 15 l 274.86 15 l 274.86 12.38 l 272.24 12.38 x s 314.16 12.38 l 314.16 22.86 l 308.92 22.86 l 308.92 15 l 306.3 15 l 306.3 22.86 l 301.06 22.86 l 301.06 15 l 298.44 15 l 298.44 22.86 l 293.2 22.86 l 293.2 12.38 x s 316.78 22.86 l 316.78 15 l 322.02 15 l 322.02 22.86 x s 316.78 9.76 l 322.02 9.76 l 322.02 12.38 l 316.78 12.38 x s 324.64 22.86 l 324.64 12.38 l 332.5 12.38 l 332.5 9.76 l 337.74 9.76 l 337.74 22.86 x s 329.99 15 l 329.99 20.24 l 332.5 20.24 l 332.5 15 x s 340.36 22.86 l 340.36 15 l 345.6 15 l 345.6 22.86 x s 340.36 9.76 l 345.6 9.76 l 345.6 12.38 l 340.36 12.38 x s 361.32 12.38 l 353.46 12.38 l 353.46 15 l 361.32 15 l 361.32 22.86 l 348.22 22.86 l 348.22 20.24 l 356.08 20.24 l 356.08 17.62 l 348.22 17.62 l 348.22 9.76 l 361.32 9.76 x s 374.42 12.38 l 374.42 15 l 371.8 15 l 371.8 22.86 l 366.56 22.86 l 366.56 15 l 363.94 15 l 363.94 12.38 l 366.56 12.38 l 366.56 9.76 l 371.8 9.76 l 371.8 12.38 x s 377.04 22.86 l 377.04 12.38 l 390.14 12.38 l 390.14 17.62 l 387.52 17.62 l 387.52 20.24 l 390.14 20.24 l 390.14 22.86 x s 382.28 17.52 l 384.9 17.52 l 384.9 15.1 l 382.28 15.1 x s 405.86 12.38 l 405.86 22.86 l 398 22.86 l 398 28.1 l 392.76 28.1 l 392.76 12.38 x s 400.52 20.24 l 400.52 15 l 398 15 l 398 20.24 x</PATH>
  </BACKGROUND>
  <GROUPCOMPONENT name="new group" id="fac932833c3845e9" memberName="activeLoopLabel"
                  virtualName="" explicitFocusOrder="0" pos="2Rr 154 73 163" outlinecol="66000000"
                  title="16"/>
  <GENERICCOMPONENT name="" id="2a32bbdca363149e" memberName="resizer" virtualName=""
                    explicitFocusOrder="0" pos="0Rr 0Rr 16 16" class="ResizableCornerComponent"
                    params="this, &amp;resizeLimits"/>
  <VIEWPORT name="new viewport" id="3deb5df0f81d6dcd" memberName="viewport"
            virtualName="PianoPort" explicitFocusOrder="0" pos="8 32 89M 34M"
            vscroll="1" hscroll="1" scrollbarThickness="18" contentType="2"
            jucerFile="" contentClass="PianoRoll" constructorParams="this-&gt;getFilter()"/>
  <TEXTBUTTON name="Record Button" id="d9abb0e6696ae5e1" memberName="recordButton"
              virtualName="" explicitFocusOrder="0" pos="6Rr 8 64 43" bgColOff="ffa52a2a"
              bgColOn="ffff0000" textCol="ffffffff" buttonText="Record" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="16866e2a5109ea3f" memberName="outChannelSlider"
          virtualName="ChannelSlider" explicitFocusOrder="0" pos="10Rr 295 57 16"
          textboxtext="ff000000" min="0" max="16" int="1" style="IncDecButtons"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="35"
          textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="new button" id="7fc3421a14f7fb9c" memberName="recArmButton1"
              virtualName="" explicitFocusOrder="0" pos="55Rr 71 20 18" buttonText="1"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="76beef56b2f4b9e6" memberName="recArmButton2"
              virtualName="" explicitFocusOrder="0" pos="37Rr 71 20 18" buttonText="2"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="61fe653f655ef38b" memberName="recArmButton3"
              virtualName="" explicitFocusOrder="0" pos="19Rr 71 20 18" buttonText="3"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="30a28388507cffcb" memberName="recArmButton4"
              virtualName="" explicitFocusOrder="0" pos="21R 71 20 18" buttonText="4"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="86518304581e549e" memberName="recArmButton5"
              virtualName="" explicitFocusOrder="0" pos="55Rr 92 20 18" buttonText="5"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="61f7fa7a3d092ce6" memberName="recArmButton6"
              virtualName="" explicitFocusOrder="0" pos="37Rr 92 20 18" buttonText="6"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3c99f17b273aa68d" memberName="recArmButton7"
              virtualName="" explicitFocusOrder="0" pos="19Rr 92 20 18" buttonText="7"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="42a3d8b8b17aa75f" memberName="recArmButton8"
              virtualName="" explicitFocusOrder="0" pos="21R 92 20 18" buttonText="8"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="c3a867e3f63a2713" memberName="keySlider"
          virtualName="" explicitFocusOrder="0" pos="10Rr 220 57 16" textboxtext="ff000000"
          min="-1" max="127" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="35" textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="new button" id="d77e7bf45fffc1d2" memberName="recArmButton9"
              virtualName="" explicitFocusOrder="0" pos="55Rr 113 20 18" buttonText="9"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="1c7f4fa4f0748684" memberName="recArmButton10"
              virtualName="" explicitFocusOrder="0" pos="37Rr 113 20 18" buttonText="10"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="70d692c3d12474ff" memberName="recArmButton11"
              virtualName="" explicitFocusOrder="0" pos="19Rr 113 20 18" buttonText="11"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="c7c15956e004fa45" memberName="recArmButton12"
              virtualName="" explicitFocusOrder="0" pos="21R 113 20 18" buttonText="12"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="e73c4abbf5d1682d" memberName="recArmButton13"
              virtualName="" explicitFocusOrder="0" pos="55Rr 134 20 18" buttonText="13"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="b1645817b0059321" memberName="recArmButton14"
              virtualName="" explicitFocusOrder="0" pos="37Rr 134 20 18" buttonText="14"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="61ee10d17d0e3606" memberName="recArmButton15"
              virtualName="" explicitFocusOrder="0" pos="19Rr 134 20 18" buttonText="15"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="90eff9156aa4e3ad" memberName="recArmButton16"
              virtualName="" explicitFocusOrder="0" pos="21R 134 20 18" buttonText="16"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="7ae245cb5a9b32e6" memberName="recChannelSlider"
          virtualName="ChannelSlider" explicitFocusOrder="0" pos="10Rr 180 57 16"
          textboxtext="ff000000" min="0" max="16" int="1" style="IncDecButtons"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="35"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="a25d139588b1f62d" memberName="transposeSlider"
          virtualName="" explicitFocusOrder="0" pos="10Rr 260 57 16" textboxtext="ff000000"
          min="-48" max="48" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="35" textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="new button" id="e2f159b8cf225153" memberName="saveButton"
              virtualName="" explicitFocusOrder="0" pos="14Rr 333 54 18" buttonText="Export..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <JUCERCOMP name="" id="2753f22783a7a3b6" memberName="component" virtualName=""
             explicitFocusOrder="0" pos="8 5 48 24" sourceFile="../../common/PizButton.cpp"
             constructorParams=""/>
  <TOGGLEBUTTON name="new toggle button" id="e334487eca358d4f" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="6Rr 49 61 21" buttonText="active"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="new button" id="2df0660a89c55019" memberName="addBarButton"
              virtualName="" explicitFocusOrder="0" pos="445 -30 29 24" buttonText="Add Bar"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="edc9437fb123b307" memberName="deleteBarButton"
              virtualName="" explicitFocusOrder="0" pos="413 -30 29 24" buttonText="Del Bar"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="thru button" id="2122599a9ad8f91e" memberName="thruButton"
                virtualName="" explicitFocusOrder="0" pos="67R 313 61 21" buttonText="thru"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
