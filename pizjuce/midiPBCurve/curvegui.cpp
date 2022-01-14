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

#include "curvegui.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
CurveEditor::CurveEditor (MidiCurve* const ownerFilter)
    : AudioProcessorEditor (ownerFilter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    curve.reset (new MidiEnvelope (0,this,this->getFilter()));
    addAndMakeVisible (curve.get());
    curve->setName ("curve");

    label.reset (new juce::Label ("new label",
                                  TRANS("In: --")));
    addAndMakeVisible (label.get());
    label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (juce::Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label2.reset (new juce::Label ("new label",
                                   TRANS("Out: --")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    resizer.reset (new ResizableCornerComponent (this, &resizeLimits));
    addAndMakeVisible (resizer.get());

    channelSlider.reset (new ChannelSlider ("new slider"));
    addAndMakeVisible (channelSlider.get());
    channelSlider->setRange (0, 16, 1);
    channelSlider->setSliderStyle (juce::Slider::IncDecButtons);
    channelSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 35, 20);
    channelSlider->setColour (juce::Slider::textBoxTextColourId, juce::Colours::black);
    channelSlider->addListener (this);

    label3.reset (new juce::Label ("new label",
                                   TRANS("Channel")));
    addAndMakeVisible (label3.get());
    label3->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (juce::Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    resetButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (resetButton.get());
    resetButton->setButtonText (TRANS("Reset"));
    resetButton->addListener (this);

    helpButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (helpButton.get());
    helpButton->setButtonText (TRANS("Help"));
    helpButton->addListener (this);

    instructionsLabel.reset (new juce::Label ("new label",
                                              TRANS("X is input, Y is output.\n"
                                              "Max 32 points.\n"
                                              "\n"
                                              "Default settings load from \"midiPBCurve.fxb\"\n"
                                              "_______________\n"
                                              "\n"
                                              "Double-click or Right-click: \n"
                                              "add or delete a point\n"
                                              "\n"
                                              "Middle-click or Alt-click: \n"
                                              "make a curve control point\n"
                                              "\n"
                                              "Shift-click/drag: \n"
                                              "set point to center line\n"
                                              "\n"
                                              "Ctrl-Shift-click/drag: \n"
                                              "set point to Y-center\n"
                                              "\n"
                                              "Ctrl-drag: \n"
                                              "restrict to horizontal/vertical movement")));
    addAndMakeVisible (instructionsLabel.get());
    instructionsLabel->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    instructionsLabel->setJustificationType (juce::Justification::centred);
    instructionsLabel->setEditable (false, false, false);
    instructionsLabel->setColour (juce::Label::backgroundColourId, juce::Colour (0xcbffffff));
    instructionsLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    instructionsLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label4.reset (new juce::Label ("new label",
                                   TRANS("Last Message")));
    addAndMakeVisible (label4.get());
    label4->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
    label4->setJustificationType (juce::Justification::centred);
    label4->setEditable (false, false, false);
    label4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    rangeSlider.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (rangeSlider.get());
    rangeSlider->setRange (0, 24, 1);
    rangeSlider->setSliderStyle (juce::Slider::IncDecButtons);
    rangeSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 35, 20);
    rangeSlider->setColour (juce::Slider::textBoxTextColourId, juce::Colours::black);
    rangeSlider->addListener (this);

    label5.reset (new juce::Label ("new label",
                                   TRANS("PB Range +")));
    addAndMakeVisible (label5.get());
    label5->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (juce::Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label5->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label6.reset (new juce::Label ("new label",
                                   juce::String()));
    addAndMakeVisible (label6.get());
    label6->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (juce::Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label6->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label7.reset (new juce::Label ("new label",
                                   juce::String()));
    addAndMakeVisible (label7.get());
    label7->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (juce::Justification::centredLeft);
    label7->setEditable (false, false, false);
    label7->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label7->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    rangeSlider2.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (rangeSlider2.get());
    rangeSlider2->setRange (0, 24, 1);
    rangeSlider2->setSliderStyle (juce::Slider::IncDecButtons);
    rangeSlider2->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 35, 20);
    rangeSlider2->setColour (juce::Slider::textBoxTextColourId, juce::Colours::black);
    rangeSlider2->addListener (this);

    label8.reset (new juce::Label ("new label",
                                   TRANS("PB Range -")));
    addAndMakeVisible (label8.get());
    label8->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label8->setJustificationType (juce::Justification::centredLeft);
    label8->setEditable (false, false, false);
    label8->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label8->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
	resizeLimits.setSizeLimits (50, 50, 1600, 1600);
	instructionsLabel->setVisible(false);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	ownerFilter->addChangeListener (this);
	this->updateParameters();
    //[/Constructor]
}

CurveEditor::~CurveEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	getFilter()->removeChangeListener (this);
    //[/Destructor_pre]

    curve = nullptr;
    label = nullptr;
    label2 = nullptr;
    resizer = nullptr;
    channelSlider = nullptr;
    label3 = nullptr;
    resetButton = nullptr;
    helpButton = nullptr;
    instructionsLabel = nullptr;
    label4 = nullptr;
    rangeSlider = nullptr;
    label5 = nullptr;
    label6 = nullptr;
    label7 = nullptr;
    rangeSlider2 = nullptr;
    label8 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void CurveEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffdddddd));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void CurveEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    curve->setBounds (8, 8, getWidth() - 93, getHeight() - 16);
    label->setBounds (getWidth() - 6 - 67, 61, 67, 16);
    label2->setBounds (getWidth() - 8 - 72, 98, 72, 16);
    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
    channelSlider->setBounds (getWidth() - 6 - 72, 148, 72, 16);
    label3->setBounds (getWidth() - 11 - 72, 132, 72, 16);
    resetButton->setBounds (getWidth() - 6 - 72, 261, 72, 24);
    helpButton->setBounds (getWidth() - 6 - 72, 293, 72, 24);
    instructionsLabel->setBounds (16, 16, getWidth() - 108, getHeight() - 32);
    label4->setBounds (getWidth() - 6 - 72, 16, 72, 45);
    rangeSlider->setBounds (getWidth() - 6 - 72, 197, 72, 16);
    label5->setBounds (getWidth() - -3 - 86, 181, 86, 16);
    label6->setBounds (getWidth() - 12 - 51, 77, 51, 16);
    label7->setBounds (getWidth() - 12 - 51, 114, 51, 16);
    rangeSlider2->setBounds (getWidth() - 6 - 72, 232, 72, 16);
    label8->setBounds (getWidth() - -3 - 86, 216, 86, 16);
    //[UserResized] Add your own custom resize handling here..
	getFilter()->lastUIHeight=getHeight();
	getFilter()->lastUIWidth=getWidth();
    //[/UserResized]
}

void CurveEditor::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == channelSlider.get())
    {
        //[UserSliderCode_channelSlider] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kChannel,(float)(channelSlider->getValue()/channelSlider->getMaximum()));
        //[/UserSliderCode_channelSlider]
    }
    else if (sliderThatWasMoved == rangeSlider.get())
    {
        //[UserSliderCode_rangeSlider] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kPBRange,(float)(rangeSlider->getValue()/rangeSlider->getMaximum()));
        //[/UserSliderCode_rangeSlider]
    }
    else if (sliderThatWasMoved == rangeSlider2.get())
    {
        //[UserSliderCode_rangeSlider2] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kPBRange2,(float)(rangeSlider2->getValue()/rangeSlider2->getMaximum()));
        //[/UserSliderCode_rangeSlider2]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void CurveEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == resetButton.get())
    {
        //[UserButtonCode_resetButton] -- add your button handler code here..
		getFilter()->resetPoints();
        //[/UserButtonCode_resetButton]
    }
    else if (buttonThatWasClicked == helpButton.get())
    {
        //[UserButtonCode_helpButton] -- add your button handler code here..
		instructionsLabel->setVisible(!instructionsLabel->isVisible());
        //[/UserButtonCode_helpButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void CurveEditor::changeListenerCallback (ChangeBroadcaster* source)
{
	if(source==getFilter())
		updateParameters();
}

void CurveEditor::updateParameters ()
{
	const int lastin = getFilter()->lastCCIn;
	const int lastout = getFilter()->lastCCOut;
	const float pbrange = getFilter()->getParameter(kPBRange)*48.f;
	const float pbrange2 = getFilter()->getParameter(kPBRange2)*48.f;
	if (lastin!=-1) {
		label->setText("In: " + String(lastin),sendNotification);
		if (lastin==8192) label6->setText("(center)",sendNotification);
		else if (lastin>8192) label6->setText("(+"+String(pbrange*((float)lastin/16383.f-0.5f),2) + ")",sendNotification);
		else if (lastin<8192) label6->setText("("+String(pbrange2*((float)lastin/16383.f-0.5f),2) + ")",sendNotification);
	}
	if (lastout!=-1) {
		label2->setText("Out: " + String(lastout),sendNotification);
		if (lastout==8192) label7->setText("(center)",sendNotification);
		else if (lastout>8192) label7->setText("(+"+String(pbrange*((float)lastout/16383.f-0.5f),2) + ")",sendNotification);
		else if (lastout<8192) label7->setText("("+String(pbrange2*((float)lastout/16383.f-0.5f),2) + ")",sendNotification);
	}

	channelSlider->setValue(getFilter()->getParameter(kChannel)*channelSlider->getMaximum(),dontSendNotification);
	rangeSlider->setValue(getFilter()->getParameter(kPBRange)*rangeSlider->getMaximum(),dontSendNotification);
	rangeSlider2->setValue(getFilter()->getParameter(kPBRange2)*rangeSlider2->getMaximum(),dontSendNotification);

	curve->updateParameters(true);

	setSize(getFilter()->lastUIWidth,getFilter()->lastUIHeight);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="CurveEditor" componentName=""
                 parentClasses="public juce::AudioProcessorEditor, public juce::ChangeListener"
                 constructorParams="MidiCurve* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffdddddd"/>
  <GENERICCOMPONENT name="curve" id="a93377748ddb46bb" memberName="curve" virtualName=""
                    explicitFocusOrder="0" pos="8 8 93M 16M" class="MidiEnvelope"
                    params="0,this,this-&gt;getFilter()"/>
  <LABEL name="new label" id="2453dab56ac8437b" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="6Rr 61 67 16" edTextCol="ff000000"
         edBkgCol="0" labelText="In: --" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="f7832c70777477b2" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8Rr 98 72 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Out: --" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="" id="2a32bbdca363149e" memberName="resizer" virtualName=""
                    explicitFocusOrder="0" pos="0Rr 0Rr 16 16" class="ResizableCornerComponent"
                    params="this, &amp;resizeLimits"/>
  <SLIDER name="new slider" id="a4e096bdcf717868" memberName="channelSlider"
          virtualName="ChannelSlider" explicitFocusOrder="0" pos="6Rr 148 72 16"
          textboxtext="ff000000" min="0.0" max="16.0" int="1.0" style="IncDecButtons"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="35"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="59be378b1bb0a393" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="11Rr 132 72 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Channel" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="d5b7ebe4f17e4a55" memberName="resetButton"
              virtualName="" explicitFocusOrder="0" pos="6Rr 261 72 24" buttonText="Reset"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="619bb49c64605970" memberName="helpButton"
              virtualName="" explicitFocusOrder="0" pos="6Rr 293 72 24" buttonText="Help"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="8dd803ff153c9de5" memberName="instructionsLabel"
         virtualName="" explicitFocusOrder="0" pos="16 16 108M 32M" bkgCol="cbffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="X is input, Y is output.&#10;Max 32 points.&#10;&#10;Default settings load from &quot;midiPBCurve.fxb&quot;&#10;_______________&#10;&#10;Double-click or Right-click: &#10;add or delete a point&#10;&#10;Middle-click or Alt-click: &#10;make a curve control point&#10;&#10;Shift-click/drag: &#10;set point to center line&#10;&#10;Ctrl-Shift-click/drag: &#10;set point to Y-center&#10;&#10;Ctrl-drag: &#10;restrict to horizontal/vertical movement"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="18.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="76fe7ddb2ab9df1" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="6Rr 16 72 45" edTextCol="ff000000"
         edBkgCol="0" labelText="Last Message" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="1" italic="0" justification="36"
         typefaceStyle="Bold"/>
  <SLIDER name="new slider" id="eda942c14ab3c70e" memberName="rangeSlider"
          virtualName="" explicitFocusOrder="0" pos="6Rr 197 72 16" textboxtext="ff000000"
          min="0.0" max="24.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="35" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="4790fe757cf3918a" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="-3Rr 181 86 16" edTextCol="ff000000"
         edBkgCol="0" labelText="PB Range +" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="68ffc50c40b39786" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="12Rr 77 51 16" edTextCol="ff000000"
         edBkgCol="0" labelText="" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="5a28bf0087037467" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="12Rr 114 51 16" edTextCol="ff000000"
         edBkgCol="0" labelText="" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="b296a671e0d8ba04" memberName="rangeSlider2"
          virtualName="" explicitFocusOrder="0" pos="6Rr 232 72 16" textboxtext="ff000000"
          min="0.0" max="24.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="35" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="5f6c75f935fce83" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="-3Rr 216 86 16" edTextCol="ff000000"
         edBkgCol="0" labelText="PB Range -" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

