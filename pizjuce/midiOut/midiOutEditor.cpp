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

#include "midiOutEditor.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MidiOutEditor::MidiOutEditor (MidiOutFilter* const ownerFilter)
    : AudioProcessorEditor (ownerFilter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    comboBox.reset (new juce::ComboBox ("new combo box"));
    addAndMakeVisible (comboBox.get());
    comboBox->setTooltip (TRANS ("Output Device"));
    comboBox->setEditableText (false);
    comboBox->setJustificationType (juce::Justification::centredLeft);
    comboBox->setTextWhenNothingSelected (juce::String());
    comboBox->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    comboBox->addListener (this);

    comboBox->setBounds (4, 4, 219, 22);

    clockButton.reset (new juce::ToggleButton ("Send MIDI Clock"));
    addAndMakeVisible (clockButton.get());
    clockButton->setTooltip (TRANS ("Send MIDI Clock"));
    clockButton->setButtonText (TRANS ("Clock"));
    clockButton->addListener (this);

    clockButton->setBounds (0, 27, 60, 22);

    mtcButton.reset (new juce::ToggleButton ("Send MTC"));
    addAndMakeVisible (mtcButton.get());
    mtcButton->setTooltip (TRANS ("Send MTC"));
    mtcButton->setButtonText (TRANS ("MTC"));
    mtcButton->addListener (this);

    mtcButton->setBounds (57, 27, 50, 22);

    hostButton.reset (new juce::ToggleButton ("Out to host"));
    addAndMakeVisible (hostButton.get());
    hostButton->setTooltip (TRANS ("Pass thru MIDI to host"));
    hostButton->setButtonText (TRANS ("Host"));
    hostButton->addListener (this);

    hostButton->setBounds (109, 27, 55, 22);

    channelBox.reset (new juce::ComboBox ("Input Channel"));
    addAndMakeVisible (channelBox.get());
    channelBox->setTooltip (TRANS ("Input Channel"));
    channelBox->setEditableText (false);
    channelBox->setJustificationType (juce::Justification::centredLeft);
    channelBox->setTextWhenNothingSelected (TRANS ("16"));
    channelBox->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    channelBox->addItem (TRANS ("All"), 1);
    channelBox->addItem (TRANS ("1"), 2);
    channelBox->addItem (TRANS ("2"), 3);
    channelBox->addItem (TRANS ("3"), 4);
    channelBox->addItem (TRANS ("4"), 5);
    channelBox->addItem (TRANS ("5"), 6);
    channelBox->addItem (TRANS ("6"), 7);
    channelBox->addItem (TRANS ("7"), 8);
    channelBox->addItem (TRANS ("8"), 9);
    channelBox->addItem (TRANS ("9"), 10);
    channelBox->addItem (TRANS ("10"), 11);
    channelBox->addItem (TRANS ("11"), 12);
    channelBox->addItem (TRANS ("12"), 13);
    channelBox->addItem (TRANS ("13"), 14);
    channelBox->addItem (TRANS ("14"), 15);
    channelBox->addItem (TRANS ("15"), 16);
    channelBox->addItem (TRANS ("16"), 17);
    channelBox->addListener (this);

    channelBox->setBounds (190, 31, 33, 16);

    imagepad.reset (new MidiPad());
    addAndMakeVisible (imagepad.get());
    imagepad->setName ("Icon");

    label.reset (new juce::Label ("new label",
                                  TRANS ("Ch:")));
    addAndMakeVisible (label.get());
    label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (juce::Justification::centredRight);
    label->setEditable (false, false, false);
    label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label->setBounds (156, 30, 38, 16);

    //[UserPreSize]
    setMouseClickGrabsKeyboardFocus (false);

    comboBox->setMouseClickGrabsKeyboardFocus (false);
    comboBox->addItem (String ("--"), 1);
    for (int i = 0; i < ownerFilter->devices.size(); i++)
    {
        comboBox->addItem (ownerFilter->devices[i].name, i + 2);
    }
    comboBox->setSelectedId (1);

    imagepad->setTriggeredOnMouseDown (true);
    imagepad->addButtonListener (this);
    imagepad->drawableButton->Label = String ("Drag\nIcon");

    clockButton->setMouseClickGrabsKeyboardFocus (false);
    mtcButton->setMouseClickGrabsKeyboardFocus (false);
    hostButton->setMouseClickGrabsKeyboardFocus (false);
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
    getFilter()->icon = imagepad->drawableButton->getName();
    //[/Destructor_pre]

    comboBox = nullptr;
    clockButton = nullptr;
    mtcButton = nullptr;
    hostButton = nullptr;
    channelBox = nullptr;
    imagepad = nullptr;
    label = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MidiOutEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffd8d8d8));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MidiOutEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    imagepad->setBounds (getWidth() - 45, 5, 40, 40);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MidiOutEditor::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox.get())
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
        if (comboBox->getSelectedItemIndex() == 0)
        {
            getFilter()->setParameter (0, 0.0f);
            getFilter()->setActiveDevice (comboBox->getText());
        }
        else
        {
            getFilter()->setActiveDevice (comboBox->getText());
            //getFilter()->setParameter(0,float(comboBox->getSelectedItemIndex()-1)/float(getFilter()->devices.size()-1)+0.00001f);
        }
        //[/UserComboBoxCode_comboBox]
    }
    else if (comboBoxThatHasChanged == channelBox.get())
    {
        //[UserComboBoxCode_channelBox] -- add your combo box handling code here..
        getFilter()->setParameter (kChannel, 0.0625f * (float) channelBox->getSelectedItemIndex());
        //[/UserComboBoxCode_channelBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void MidiOutEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == clockButton.get())
    {
        //[UserButtonCode_clockButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost (kClock, clockButton->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_clockButton]
    }
    else if (buttonThatWasClicked == mtcButton.get())
    {
        //[UserButtonCode_mtcButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost (kMTC, mtcButton->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_mtcButton]
    }
    else if (buttonThatWasClicked == hostButton.get())
    {
        //[UserButtonCode_hostButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost (kHostOut, hostButton->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_hostButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MidiOutEditor::buttonStateChanged (Button* buttonThatWasClicked)
{
    getFilter()->icon = imagepad->drawableButton->getName();
    if (imagepad->drawableButton->isDown())
    {
        ModifierKeys mousebutton = ModifierKeys::getCurrentModifiers();
        if (mousebutton.isPopupMenu())
        {
            PopupMenu m, sub1;
            //m.addItem(0,"Text:",false);
            //m.addCustomItem (1, textEditor, 200 , 24, false);
            //m.addSeparator();

            m.addItem (66, "Clear Image");
            //sub1.addCustomItem (1234, colourSelector, 300, 300, false);
            //m.addSubMenu ("Color", sub1);
            m.addSeparator();

            int result = m.show();
            if (result != 0)
            {
                if (result == 66)
                {
                    getFilter()->icon = String ("");
                    imagepad->clearIcon();
                }
            }
        }
    }
}

//==============================================================================
void MidiOutEditor::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == getFilter())
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
    const int newDevice = filter->devices.indexOf (filter->getActiveDevice());
    const float clock = filter->getParameter (kClock);
    const float mtc = filter->getParameter (kMTC);
    const float hostout = filter->getParameter (kHostOut);
    const int channel = roundToInt (filter->getParameter (kChannel) * 16.f);
    const String icon = filter->icon;

    // ..release the lock ASAP
    filter->getCallbackLock().exit();

    comboBox->setSelectedItemIndex (newDevice + 1, dontSendNotification);
    channelBox->setSelectedItemIndex (channel, dontSendNotification);

    clockButton->setToggleState (clock >= 0.5f, dontSendNotification);
    mtcButton->setToggleState (mtc >= 0.5f, dontSendNotification);
    hostButton->setToggleState (hostout >= 0.5f, dontSendNotification);

    String fullpath = icon;
    if (! File::getCurrentWorkingDirectory().getChildFile (fullpath).existsAsFile())
        fullpath = ((File::getSpecialLocation (File::currentExecutableFile)).getParentDirectory()).getFullPathName()
                   + File::getSeparatorString() + icon;
    auto image = Drawable::createFromImageFile (File (fullpath));
    if (image)
    {
        imagepad->drawableButton->setImages (image.get());
        imagepad->drawableButton->setName (icon);
        imagepad->setButtonText (String());
    }
    else
        imagepad->setButtonText ("IPH\nmidiOut\n1.3");
}
//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiOutEditor" componentName=""
                 parentClasses="public juce::AudioProcessorEditor, public juce::ChangeListener"
                 constructorParams="MidiOutFilter* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
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
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]
