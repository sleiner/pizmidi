/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  14 Sep 2011 10:44:57am

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

#include "midiInEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MidiInEditor::MidiInEditor (MidiInFilter* const ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      comboBox (0),
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

    addAndMakeVisible (hostButton = new ToggleButton (L"Out to host"));
    hostButton->setTooltip (L"Pass thru MIDI to host");
    hostButton->setButtonText (L"In from Host");
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
    imagepad->drawableButton->Label = "";
	imagepad->setButtonText(String());

    hostButton->setMouseClickGrabsKeyboardFocus(false);
    //[/UserPreSize]

    setSize (275, 50);


    //[Constructor] You can add your own custom stuff here..
    ownerFilter->addChangeListener (this);
    updateParametersFromFilter();
    //[/Constructor]
}

MidiInEditor::~MidiInEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    getFilter()->removeChangeListener (this);
    getFilter()->icon=imagepad->drawableButton->getName();
    //[/Destructor_pre]

    deleteAndZero (comboBox);
    deleteAndZero (hostButton);
    deleteAndZero (channelBox);
    deleteAndZero (imagepad);
    deleteAndZero (label);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MidiInEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffd8d8d8));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MidiInEditor::resized()
{
    comboBox->setBounds (4, 4, 219, 22);
    hostButton->setBounds (1, 27, 108, 22);
    channelBox->setBounds (190, 31, 33, 16);
    imagepad->setBounds (getWidth() - 45, 5, 40, 40);
    label->setBounds (156, 30, 38, 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MidiInEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
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
			//getFilter()->setParameter(0,float(cb->getSelectedItemIndex()-1)/float(getFilter()->devices.size()-1)+0.00001f);
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

void MidiInEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == hostButton)
    {
        //[UserButtonCode_hostButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost(kHostIn,hostButton->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_hostButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MidiInEditor::buttonStateChanged (Button* buttonThatWasClicked) {
    getFilter()->icon=imagepad->drawableButton->getName();
    if (imagepad->drawableButton->isDown()) {
        ModifierKeys mousebutton = ModifierKeys::getCurrentModifiers();
        if (mousebutton.isPopupMenu()) {
            PopupMenu m, sub1;
            m.addItem(66,"Clear Image");
            m.addSeparator();

            int result = m.show();
            if (result != 0)
            {
                if (result==66) {
                    getFilter()->icon = String("");
                    imagepad->drawableButton->setImages(0);
                    imagepad->drawableButton->setName("");
                }
            }
        }
    }
}

//==============================================================================
void MidiInEditor::changeListenerCallback (ChangeBroadcaster* source)
{
    updateParametersFromFilter();
}

void MidiInEditor::updateParametersFromFilter()
{
    MidiInFilter* const filter = getFilter();

    // we use this lock to make sure the processBlock() method isn't writing to the
    // lastMidiMessage variable while we're trying to read it, but be extra-careful to
    // only hold the lock for a minimum amount of time..
    filter->getCallbackLock().enter();

    // take a local copy of the info we need while we've got the lock..
    const int newDevice = filter->devices.indexOf(filter->getActiveDevice());
    const float hostin = filter->getParameter (kHostIn);
    const String icon=filter->icon;
	const int channel = roundToInt(filter->getParameter (kChannel) * 16.f);

    // ..release the lock ASAP
    filter->getCallbackLock().exit();

    comboBox->setSelectedItemIndex(newDevice+1,true);
	channelBox->setSelectedItemIndex(channel,true);

    hostButton->setToggleState(hostin>=0.5f,false);

    String fullpath = icon;
    if (!File(fullpath).existsAsFile())
        fullpath = ((File::getSpecialLocation(File::currentExecutableFile)).getParentDirectory()).getFullPathName()
                   + File::getSeparatorString() + icon;
    auto image = Drawable::createFromImageFile(File(fullpath));
	if (image != nullptr)
	{
		imagepad->drawableButton->setImages(image.get());
		imagepad->drawableButton->setName(icon);
		imagepad->setButtonText(String());
	}
	else imagepad->setButtonText("IPH\nmidiIn\n1.2");
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiInEditor" componentName=""
                 parentClasses="public AudioProcessorEditor, public ChangeListener, public Button::Listener, public ComboBoxListener"
                 constructorParams="MidiInFilter* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="275" initialHeight="50">
  <BACKGROUND backgroundColour="ffd8d8d8"/>
  <COMBOBOX name="new combo box" id="a04840c19e04d837" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="4 4 219 22" tooltip="Output Device"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="Out to host" id="4bbb1d459a4f0283" memberName="hostButton"
                virtualName="" explicitFocusOrder="0" pos="1 27 108 22" tooltip="Pass thru MIDI to host"
                buttonText="In from Host" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
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
