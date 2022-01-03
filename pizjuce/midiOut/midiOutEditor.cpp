/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  14 Sep 2011 10:45:23am

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

#include "midiOutEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MidiOutEditor::MidiOutEditor (MidiOutFilter* const ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      comboBox (0),
      clockButton (0),
      mtcButton (0),
      hostButton (0),
      channelBox (0),
      imagepad (0),
      label (0)
{
    addAndMakeVisible (comboBox = new ComboBox (L"new combo box"));
    comboBox->setTooltip (L"Output Device");
    comboBox->setEditableText (false);
    comboBox->setJustificationType (Justification::centredLeft);
    comboBox->setTextWhenNothingSelected (String());
    comboBox->setTextWhenNoChoicesAvailable (L"(no choices)");
    comboBox->addListener (this);

    addAndMakeVisible (clockButton = new ToggleButton (L"Send MIDI Clock"));
    clockButton->setTooltip (L"Send MIDI Clock");
    clockButton->setButtonText (L"Clock");
    clockButton->addListener (this);

    addAndMakeVisible (mtcButton = new ToggleButton (L"Send MTC"));
    mtcButton->setTooltip (L"Send MTC");
    mtcButton->setButtonText (L"MTC");
    mtcButton->addListener (this);

    addAndMakeVisible (hostButton = new ToggleButton (L"Out to host"));
    hostButton->setTooltip (L"Pass thru MIDI to host");
    hostButton->setButtonText (L"Host");
    hostButton->addListener (this);

    addAndMakeVisible (channelBox = new ComboBox (L"Input Channel"));
    channelBox->setTooltip (L"Input Channel");
    channelBox->setEditableText (false);
    channelBox->setJustificationType (Justification::centredLeft);
    channelBox->setTextWhenNothingSelected (L"16");
    channelBox->setTextWhenNoChoicesAvailable (L"(no choices)");
    channelBox->addItem (L"All", 1);
    channelBox->addItem (L"1", 2);
    channelBox->addItem (L"2", 3);
    channelBox->addItem (L"3", 4);
    channelBox->addItem (L"4", 5);
    channelBox->addItem (L"5", 6);
    channelBox->addItem (L"6", 7);
    channelBox->addItem (L"7", 8);
    channelBox->addItem (L"8", 9);
    channelBox->addItem (L"9", 10);
    channelBox->addItem (L"10", 11);
    channelBox->addItem (L"11", 12);
    channelBox->addItem (L"12", 13);
    channelBox->addItem (L"13", 14);
    channelBox->addItem (L"14", 15);
    channelBox->addItem (L"15", 16);
    channelBox->addItem (L"16", 17);
    channelBox->addListener (this);

    addAndMakeVisible (imagepad = new MidiPad());
    imagepad->setName (L"Icon");

    addAndMakeVisible (label = new Label (L"new label",
                                          L"Ch:"));
    label->setFont (Font (15.0000f, Font::plain));
    label->setJustificationType (Justification::centredRight);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x0));


    //[UserPreSize]
	setMouseClickGrabsKeyboardFocus(false);
    StringArray devices = ownerFilter->devices;

    comboBox->setMouseClickGrabsKeyboardFocus(false);
    comboBox->addItem(String("--"),1);
    for (int i=0;i<devices.size();i++) {
        comboBox->addItem(devices.joinIntoString("",i,1),i+2);
    }
    comboBox->setSelectedId(1);

    imagepad->setTriggeredOnMouseDown(true);
    imagepad->addButtonListener(this);
    imagepad->drawableButton->Label = String("Drag\nIcon");

	clockButton->setMouseClickGrabsKeyboardFocus(false);
    mtcButton->setMouseClickGrabsKeyboardFocus(false);
    hostButton->setMouseClickGrabsKeyboardFocus(false);
    //[/UserPreSize]

    setSize (275, 50);


    //[Constructor] You can add your own custom stuff here..
    ownerFilter->addChangeListener (this);
    updateParametersFromFilter();
    //[/Constructor]
}

MidiOutEditor::~MidiOutEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    getFilter()->removeChangeListener (this);
    getFilter()->icon=imagepad->drawableButton->getName();
    //[/Destructor_pre]

    deleteAndZero (comboBox);
    deleteAndZero (clockButton);
    deleteAndZero (mtcButton);
    deleteAndZero (hostButton);
    deleteAndZero (channelBox);
    deleteAndZero (imagepad);
    deleteAndZero (label);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MidiOutEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffd8d8d8));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MidiOutEditor::resized()
{
    comboBox->setBounds (4, 4, 219, 22);
    clockButton->setBounds (0, 27, 60, 22);
    mtcButton->setBounds (57, 27, 50, 22);
    hostButton->setBounds (109, 27, 55, 22);
    channelBox->setBounds (190, 31, 33, 16);
    imagepad->setBounds (getWidth() - 45, 5, 40, 40);
    label->setBounds (156, 30, 38, 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MidiOutEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox)
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
		if (comboBox->getSelectedItemIndex()==0) {
			getFilter()->setParameter(0,0.0f);
			getFilter()->setActiveDevice(comboBox->getText());
		}
		else {
			getFilter()->setActiveDevice(comboBox->getText());
			//getFilter()->setParameter(0,float(comboBox->getSelectedItemIndex()-1)/float(getFilter()->devices.size()-1)+0.00001f);
		}
        //[/UserComboBoxCode_comboBox]
    }
    else if (comboBoxThatHasChanged == channelBox)
    {
        //[UserComboBoxCode_channelBox] -- add your combo box handling code here..
		getFilter()->setParameter(kChannel,0.0625f * (float)channelBox->getSelectedItemIndex());
        //[/UserComboBoxCode_channelBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void MidiOutEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == clockButton)
    {
        //[UserButtonCode_clockButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost(kClock,clockButton->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_clockButton]
    }
    else if (buttonThatWasClicked == mtcButton)
    {
        //[UserButtonCode_mtcButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost(kMTC,mtcButton->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_mtcButton]
    }
    else if (buttonThatWasClicked == hostButton)
    {
        //[UserButtonCode_hostButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost(kHostOut,hostButton->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_hostButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MidiOutEditor::buttonStateChanged (Button* buttonThatWasClicked) {
    getFilter()->icon=imagepad->drawableButton->getName();
    if (imagepad->drawableButton->isDown()) {
        ModifierKeys mousebutton = ModifierKeys::getCurrentModifiers();
        if (mousebutton.isPopupMenu()) {
            PopupMenu m, sub1;
            //m.addItem(0,"Text:",false);
            //m.addCustomItem (1, textEditor, 200 , 24, false);
            //m.addSeparator();

            m.addItem(66,"Clear Image");
            //sub1.addCustomItem (1234, colourSelector, 300, 300, false);
            //m.addSubMenu ("Color", sub1);
            m.addSeparator();

            int result = m.show();
            if (result != 0)
            {
                if (result==66) {
                    getFilter()->icon = String("");
					imagepad->clearIcon();
                }
            }
        }
    }
}

//==============================================================================
void MidiOutEditor::changeListenerCallback (ChangeBroadcaster* source)
{
	if (source==getFilter())
		updateParametersFromFilter();
}

void MidiOutEditor::updateParametersFromFilter()
{
    MidiOutFilter* const filter = getFilter();

    // we use this lock to make sure the processBlock() method isn't writing to the
    // lastMidiMessage variable while we're trying to read it, but be extra-careful to
    // only hold the lock for a minimum amount of time..
    filter->getCallbackLock().enter();

    // take a local copy of the info we need while we've got the lock..
    const int newDevice = filter->devices.indexOf(filter->getActiveDevice());
    const float clock = filter->getParameter (kClock);
    const float mtc = filter->getParameter (kMTC);
    const float hostout = filter->getParameter (kHostOut);
	const int channel = roundToInt(filter->getParameter (kChannel) * 16.f);
    const String icon=filter->icon;

    // ..release the lock ASAP
    filter->getCallbackLock().exit();

    comboBox->setSelectedItemIndex(newDevice+1,true);
	channelBox->setSelectedItemIndex(channel,true);

    clockButton->setToggleState(clock>=0.5f,false);
    mtcButton->setToggleState(mtc>=0.5f,false);
    hostButton->setToggleState(hostout>=0.5f,false);

    String fullpath = icon;
    if (!File::getCurrentWorkingDirectory().getChildFile(fullpath).existsAsFile())
        fullpath = ((File::getSpecialLocation(File::currentExecutableFile)).getParentDirectory()).getFullPathName()
                   + File::getSeparatorString() + icon;
    auto image = Drawable::createFromImageFile(File(fullpath));
	if (image)
	{
		imagepad->drawableButton->setImages(image.get());
		imagepad->drawableButton->setName(icon);
		imagepad->setButtonText(String());
	}
	else imagepad->setButtonText("IPH\nmidiOut\n1.3");
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiOutEditor" componentName=""
                 parentClasses="public AudioProcessorEditor, public ChangeListener, public Button::Listener, public ComboBoxListener"
                 constructorParams="MidiOutFilter* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="275" initialHeight="50">
  <BACKGROUND backgroundColour="ffd8d8d8"/>
  <COMBOBOX name="new combo box" id="a04840c19e04d837" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="4 4 219 22" tooltip="Output Device"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="Send MIDI Clock" id="5129573a41f41b72" memberName="clockButton"
                virtualName="" explicitFocusOrder="0" pos="0 27 60 22" tooltip="Send MIDI Clock"
                buttonText="Clock" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="Send MTC" id="164b10b4db55df9" memberName="mtcButton" virtualName=""
                explicitFocusOrder="0" pos="57 27 50 22" tooltip="Send MTC" buttonText="MTC"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="Out to host" id="4bbb1d459a4f0283" memberName="hostButton"
                virtualName="" explicitFocusOrder="0" pos="109 27 55 22" tooltip="Pass thru MIDI to host"
                buttonText="Host" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <COMBOBOX name="Input Channel" id="3cf43dcd024135be" memberName="channelBox"
            virtualName="" explicitFocusOrder="0" pos="190 31 33 16" tooltip="Input Channel"
            editable="0" layout="33" items="All&#10;1&#10;2&#10;3&#10;4&#10;5&#10;6&#10;7&#10;8&#10;9&#10;10&#10;11&#10;12&#10;13&#10;14&#10;15&#10;16"
            textWhenNonSelected="16" textWhenNoItems="(no choices)"/>
  <GENERICCOMPONENT name="Icon" id="79b548c630fe4c11" memberName="imagepad" virtualName=""
                    explicitFocusOrder="0" pos="45R 5 40 40" class="MidiPad" params=""/>
  <LABEL name="new label" id="8f39c0ad00104ec3" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="156 30 38 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Ch:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="34"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
