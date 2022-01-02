/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  3 Dec 2011 4:38:03pm

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

#include "curvegui.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
CurveEditor::CurveEditor (MidiCurve* const ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      curve (0),
      label (0),
      label2 (0),
      resizer (0),
      velocityButton (0),
      ccButton (0),
      slider (0),
      channelSlider (0),
      label3 (0),
      channelPressureButton (0),
      aftertouchButton (0),
      resetButton (0),
      helpButton (0),
      instructionsLabel (0),
      label4 (0),
      label5 (0)
{
    addAndMakeVisible (curve = new MidiEnvelope (0,this,this->getFilter()));
    curve->setName (L"curve");

    addAndMakeVisible (label = new Label (L"new label",
                                          L"In: --"));
    label->setFont (Font (15.0000f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label2 = new Label (L"new label",
                                           L"Out: --"));
    label2->setFont (Font (15.0000f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));

    addAndMakeVisible (velocityButton = new ToggleButton (L"new toggle button"));
    velocityButton->setButtonText (L"Velocity");
    velocityButton->addListener (this);

    addAndMakeVisible (ccButton = new ToggleButton (L"new toggle button"));
    ccButton->setButtonText (L"CC");
    ccButton->addListener (this);

    addAndMakeVisible (slider = new Slider (L"new slider"));
    slider->setRange (0, 127, 1);
    slider->setSliderStyle (Slider::IncDecButtons);
    slider->setTextBoxStyle (Slider::TextBoxLeft, false, 35, 20);
    slider->setColour (Slider::textBoxTextColourId, Colours::black);
    slider->addListener (this);

    addAndMakeVisible (channelSlider = new ChannelSlider (L"new slider"));
    channelSlider->setRange (0, 16, 1);
    channelSlider->setSliderStyle (Slider::IncDecButtons);
    channelSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 35, 20);
    channelSlider->setColour (Slider::textBoxTextColourId, Colours::black);
    channelSlider->addListener (this);

    addAndMakeVisible (label3 = new Label (L"new label",
                                           L"Channel"));
    label3->setFont (Font (15.0000f, Font::plain));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (channelPressureButton = new ToggleButton (L"new toggle button"));
    channelPressureButton->setButtonText (L"Ch. Press.");
    channelPressureButton->addListener (this);

    addAndMakeVisible (aftertouchButton = new ToggleButton (L"new toggle button"));
    aftertouchButton->setButtonText (L"Poly Aft.");
    aftertouchButton->addListener (this);

    addAndMakeVisible (resetButton = new TextButton (L"new button"));
    resetButton->setButtonText (L"Reset");
    resetButton->addListener (this);

    addAndMakeVisible (helpButton = new TextButton (L"new button"));
    helpButton->setButtonText (L"Help");
    helpButton->addListener (this);

    addAndMakeVisible (instructionsLabel = new Label (L"new label",
                                                      L"X is input, Y is output.\nMax 32 points.\n\nDefault settings load from \"midiCurve.fxb\"\n_______________\n\nDouble-click or Right-click: \nadd or delete a point\n\nMiddle-click or Alt-click: \nmake a curve control point\n\nShift-click/drag: \nset point to center line\n\nCtrl-drag: \nrestrict to horizontal/vertical movement"));
    instructionsLabel->setFont (Font (18.0000f, Font::plain));
    instructionsLabel->setJustificationType (Justification::centred);
    instructionsLabel->setEditable (false, false, false);
    instructionsLabel->setColour (Label::backgroundColourId, Colour (0xcbffffff));
    instructionsLabel->setColour (TextEditor::textColourId, Colours::black);
    instructionsLabel->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label4 = new Label (L"new label",
                                           L"Last Message"));
    label4->setFont (Font (15.0000f, Font::bold));
    label4->setJustificationType (Justification::centred);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label5 = new Label (L"new label",
                                           L"Inputs"));
    label5->setFont (Font (15.0000f, Font::bold));
    label5->setJustificationType (Justification::centred);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x0));


    //[UserPreSize]
	resizeLimits.setSizeLimits (50, 50, 1600, 1600);
	instructionsLabel->setVisible(false);
#if 0
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
#endif
	ownerFilter->addChangeListener (this);
	ownerFilter->lastMsg.addChangeListener (this);
	this->updateParameters();
    //[/Constructor]
}

CurveEditor::~CurveEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	getFilter()->removeChangeListener (this);
	getFilter()->lastMsg.removeChangeListener (this);
    //[/Destructor_pre]

    deleteAndZero (curve);
    deleteAndZero (label);
    deleteAndZero (label2);
    deleteAndZero (resizer);
    deleteAndZero (velocityButton);
    deleteAndZero (ccButton);
    deleteAndZero (slider);
    deleteAndZero (channelSlider);
    deleteAndZero (label3);
    deleteAndZero (channelPressureButton);
    deleteAndZero (aftertouchButton);
    deleteAndZero (resetButton);
    deleteAndZero (helpButton);
    deleteAndZero (instructionsLabel);
    deleteAndZero (label4);
    deleteAndZero (label5);


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void CurveEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffdddddd));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void CurveEditor::resized()
{
    curve->setBounds (8, 8, getWidth() - 93, getHeight() - 16);
    label->setBounds (getWidth() - 6 - 72, 41, 72, 16);
    label2->setBounds (getWidth() - 6 - 72, 57, 72, 16);
    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
    velocityButton->setBounds (getWidth() - 6 - 72, 160, 72, 24);
    ccButton->setBounds (getWidth() - 6 - 72, 182, 72, 24);
    slider->setBounds (getWidth() - 6 - 72, 205, 72, 16);
    channelSlider->setBounds (getWidth() - 6 - 72, 128, 72, 16);
    label3->setBounds (getWidth() - 6 - 72, 112, 72, 16);
    channelPressureButton->setBounds (getWidth() - 6 - 72, 223, 72, 24);
    aftertouchButton->setBounds (getWidth() - 6 - 72, 244, 72, 24);
    resetButton->setBounds (getWidth() - 6 - 72, 304, 72, 24);
    helpButton->setBounds (getWidth() - 6 - 72, 336, 72, 24);
    instructionsLabel->setBounds (16, 16, getWidth() - 108, getHeight() - 32);
    label4->setBounds (getWidth() - 6 - 72, 6, 72, 32);
    label5->setBounds (getWidth() - 6 - 72, 91, 72, 16);
    //[UserResized] Add your own custom resize handling here..
	getFilter()->lastUIHeight=getHeight();
	getFilter()->lastUIWidth=getWidth();
    //[/UserResized]
}

void CurveEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == velocityButton)
    {
        //[UserButtonCode_velocityButton] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kVelocity,velocityButton->getToggleState()?1.f:0.f);
        //[/UserButtonCode_velocityButton]
    }
    else if (buttonThatWasClicked == ccButton)
    {
        //[UserButtonCode_ccButton] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kCC,ccButton->getToggleState()?1.f:0.f);
        //[/UserButtonCode_ccButton]
    }
    else if (buttonThatWasClicked == channelPressureButton)
    {
        //[UserButtonCode_channelPressureButton] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kChannelPressure,channelPressureButton->getToggleState()?1.f:0.f);
        //[/UserButtonCode_channelPressureButton]
    }
    else if (buttonThatWasClicked == aftertouchButton)
    {
        //[UserButtonCode_aftertouchButton] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kAftertouch,aftertouchButton->getToggleState()?1.f:0.f);
        //[/UserButtonCode_aftertouchButton]
    }
    else if (buttonThatWasClicked == resetButton)
    {
        //[UserButtonCode_resetButton] -- add your button handler code here..
		getFilter()->resetPoints();
        //[/UserButtonCode_resetButton]
    }
    else if (buttonThatWasClicked == helpButton)
    {
        //[UserButtonCode_helpButton] -- add your button handler code here..
		instructionsLabel->setVisible(!instructionsLabel->isVisible());
        //[/UserButtonCode_helpButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void CurveEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider)
    {
        //[UserSliderCode_slider] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kCCNumber,(float)(slider->getValue()/slider->getMaximum()));
        //[/UserSliderCode_slider]
    }
    else if (sliderThatWasMoved == channelSlider)
    {
        //[UserSliderCode_channelSlider] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kChannel,(float)(channelSlider->getValue()/channelSlider->getMaximum()));
        //[/UserSliderCode_channelSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void CurveEditor::changeListenerCallback (ChangeBroadcaster* source)
{
	if(source==getFilter())
		updateParameters();
	else if (source==&getFilter()->lastMsg)
	{
		getFilter()->getCallbackLock().enter();
		const int lastin = getFilter()->lastMsg.lastCCIn;
		const int lastout = getFilter()->lastMsg.lastCCOut;
		getFilter()->getCallbackLock().exit();
		if (lastin!=-1) label->setText("In: " + String(lastin),dontSendNotification);
		if (lastout!=-1) label2->setText("Out: " + String(lastout),dontSendNotification);
		curve->repaintIndicator(lastin,lastout);
	}
}

void CurveEditor::updateParameters ()
{
	MidiCurve* const filter = getFilter();

	filter->getCallbackLock().enter();
	const int lastin = filter->lastMsg.lastCCIn;
	const int lastout = filter->lastMsg.lastCCOut;
	const int h = filter->lastUIHeight;
	const int w = filter->lastUIWidth;
	const float p_ccnumber = filter->getParameter(kCCNumber);
	const float p_channel = filter->getParameter(kChannel);
	const float p_velocity = filter->getParameter(kVelocity);
	const float p_cc = filter->getParameter(kCC);
	const float p_aftertouch = filter->getParameter(kAftertouch);
	const float p_channelpressure = filter->getParameter(kChannelPressure);
	const float p_pitchbend = filter->getParameter(kPitchBend);
	filter->getCallbackLock().exit();


	slider->setValue(p_ccnumber*slider->getMaximum(),dontSendNotification);
	channelSlider->setValue(p_channel*channelSlider->getMaximum(),dontSendNotification);

	velocityButton->setToggleState(p_velocity>=0.5f,dontSendNotification);
	ccButton->setToggleState(p_cc>=0.5f,dontSendNotification);
	aftertouchButton->setToggleState(p_aftertouch>=0.5f,dontSendNotification);
	channelPressureButton->setToggleState(p_channelpressure>=0.5f,dontSendNotification);

	curve->updateParameters(true);

	setSize(w,h);
	resized();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="CurveEditor" componentName=""
                 parentClasses="public AudioProcessorEditor, public ChangeListener"
                 constructorParams="MidiCurve* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffdddddd"/>
  <GENERICCOMPONENT name="curve" id="a93377748ddb46bb" memberName="curve" virtualName=""
                    explicitFocusOrder="0" pos="8 8 93M 16M" class="MidiEnvelope"
                    params="0,this,this-&gt;getFilter()"/>
  <LABEL name="new label" id="2453dab56ac8437b" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="6Rr 41 72 16" edTextCol="ff000000"
         edBkgCol="0" labelText="In: --" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="f7832c70777477b2" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="6Rr 57 72 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Out: --" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="" id="2a32bbdca363149e" memberName="resizer" virtualName=""
                    explicitFocusOrder="0" pos="0Rr 0Rr 16 16" class="ResizableCornerComponent"
                    params="this, &amp;resizeLimits"/>
  <TOGGLEBUTTON name="new toggle button" id="9df1a16bdee47d6c" memberName="velocityButton"
                virtualName="" explicitFocusOrder="0" pos="6Rr 160 72 24" buttonText="Velocity"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="45892631fb439b39" memberName="ccButton"
                virtualName="" explicitFocusOrder="0" pos="6Rr 182 72 24" buttonText="CC"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="new slider" id="2627973e6f13fd2f" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="6Rr 205 72 16" textboxtext="ff000000"
          min="0" max="127" int="1" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="35" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="a4e096bdcf717868" memberName="channelSlider"
          virtualName="ChannelSlider" explicitFocusOrder="0" pos="6Rr 128 72 16"
          textboxtext="ff000000" min="0" max="16" int="1" style="IncDecButtons"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="35"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="new label" id="59be378b1bb0a393" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="6Rr 112 72 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Channel" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="new toggle button" id="87d9dede60c981e1" memberName="channelPressureButton"
                virtualName="" explicitFocusOrder="0" pos="6Rr 223 72 24" buttonText="Ch. Press."
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="1e8f33b1ceb3ece6" memberName="aftertouchButton"
                virtualName="" explicitFocusOrder="0" pos="6Rr 244 72 24" buttonText="Poly Aft."
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="new button" id="d5b7ebe4f17e4a55" memberName="resetButton"
              virtualName="" explicitFocusOrder="0" pos="6Rr 304 72 24" buttonText="Reset"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="619bb49c64605970" memberName="helpButton"
              virtualName="" explicitFocusOrder="0" pos="6Rr 336 72 24" buttonText="Help"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="8dd803ff153c9de5" memberName="instructionsLabel"
         virtualName="" explicitFocusOrder="0" pos="16 16 108M 32M" bkgCol="cbffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="X is input, Y is output.&#10;Max 32 points.&#10;&#10;Default settings load from &quot;midiCurve.fxb&quot;&#10;_______________&#10;&#10;Double-click or Right-click: &#10;add or delete a point&#10;&#10;Middle-click or Alt-click: &#10;make a curve control point&#10;&#10;Shift-click/drag: &#10;set point to center line&#10;&#10;Ctrl-drag: &#10;restrict to horizontal/vertical movement"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="18" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="76fe7ddb2ab9df1" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="6Rr 6 72 32" edTextCol="ff000000"
         edBkgCol="0" labelText="Last Message" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="1" italic="0" justification="36"/>
  <LABEL name="new label" id="b04715bd509e8494" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="6Rr 91 72 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Inputs" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="1" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
