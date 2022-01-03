/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  27 Nov 2010 3:48:47pm

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

#include "PadEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PadEditor::PadEditor ()
    : textEditor (0),
      textButton (0),
      textButton2 (0),
      component (0),
      toggleButton (0),
      toggleButton2 (0),
      toggleButton3 (0),
      slider (0),
      slider2 (0),
      toggleButton4 (0),
      toggleButton5 (0),
      toggleButton6 (0),
      toggleButton7 (0),
      toggleButton8 (0),
      toggleButton9 (0),
      toggleButton10 (0),
      slider3 (0),
      slider4 (0),
      slider5 (0),
      slider6 (0),
      slider7 (0),
      slider8 (0),
      slider9 (0),
      textButton3 (0),
      textButton4 (0),
      textButton5 (0),
      textButton6 (0),
      textButton7 (0),
      textButton8 (0),
      textButton9 (0),
      label (0),
      label2 (0),
      label3 (0),
      label4 (0),
      label5 (0),
      label6 (0),
      label7 (0),
      label8 (0)
{
    addAndMakeVisible (textEditor = new TextEditor ("new text editor"));
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setColour (TextEditor::outlineColourId, Colours::black);
    textEditor->setText (String());

    addAndMakeVisible (textButton = new TextButton ("new button"));
    textButton->setButtonText ("Clear Icon");
    textButton->addListener (this);

    addAndMakeVisible (textButton2 = new TextButton ("new button"));
    textButton2->setButtonText ("Choose Icon");
    textButton2->addListener (this);

    addAndMakeVisible (component = new ColourSelector());
    component->setName ("new component");

    addAndMakeVisible (toggleButton = new ToggleButton ("new toggle button"));
    toggleButton->setButtonText ("Show Dot");
    toggleButton->addListener (this);

    addAndMakeVisible (toggleButton2 = new ToggleButton ("new toggle button"));
    toggleButton2->setButtonText ("Show Values");
    toggleButton2->addListener (this);

    addAndMakeVisible (toggleButton3 = new ToggleButton ("new toggle button"));
    toggleButton3->setButtonText ("Centered Text");
    toggleButton3->addListener (this);

    addAndMakeVisible (slider = new Slider ("new slider"));
    slider->setRange (0, 10, 0);
    slider->setSliderStyle (Slider::LinearBar);
    slider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider->setColour (Slider::textBoxTextColourId, Colour (0x0));
    slider->addListener (this);

    addAndMakeVisible (slider2 = new Slider ("new slider"));
    slider2->setRange (0, 10, 0);
    slider2->setSliderStyle (Slider::LinearBar);
    slider2->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider2->setColour (Slider::textBoxTextColourId, Colour (0x0));
    slider2->addListener (this);

    addAndMakeVisible (toggleButton4 = new ToggleButton ("new toggle button"));
    toggleButton4->setButtonText ("Note");
    toggleButton4->setRadioGroupId (1);
    toggleButton4->addListener (this);

    addAndMakeVisible (toggleButton5 = new ToggleButton ("new toggle button"));
    toggleButton5->setButtonText ("CC");
    toggleButton5->setRadioGroupId (1);
    toggleButton5->addListener (this);

    addAndMakeVisible (toggleButton6 = new ToggleButton ("new toggle button"));
    toggleButton6->setButtonText ("Use Y-Position");
    toggleButton6->addListener (this);

    addAndMakeVisible (toggleButton7 = new ToggleButton ("new toggle button"));
    toggleButton7->setButtonText ("Use X-Position");
    toggleButton7->addListener (this);

    addAndMakeVisible (toggleButton8 = new ToggleButton ("new toggle button"));
    toggleButton8->setButtonText ("X is Pitch Bend");
    toggleButton8->addListener (this);

    addAndMakeVisible (toggleButton9 = new ToggleButton ("new toggle button"));
    toggleButton9->setButtonText ("Send Off Values");
    toggleButton9->addListener (this);

    addAndMakeVisible (toggleButton10 = new ToggleButton ("new toggle button"));
    toggleButton10->setButtonText ("Toggle Mode");
    toggleButton10->addListener (this);

    addAndMakeVisible (slider3 = new Slider ("new slider"));
    slider3->setRange (0, 127, 1);
    slider3->setSliderStyle (Slider::LinearBar);
    slider3->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider3->addListener (this);

    addAndMakeVisible (slider4 = new Slider ("new slider"));
    slider4->setRange (0, 127, 1);
    slider4->setSliderStyle (Slider::LinearBar);
    slider4->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider4->addListener (this);

    addAndMakeVisible (slider5 = new Slider ("new slider"));
    slider5->setRange (0, 127, 1);
    slider5->setSliderStyle (Slider::LinearBar);
    slider5->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider5->addListener (this);

    addAndMakeVisible (slider6 = new Slider ("new slider"));
    slider6->setRange (0, 127, 1);
    slider6->setSliderStyle (Slider::LinearBar);
    slider6->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider6->addListener (this);

    addAndMakeVisible (slider7 = new Slider ("new slider"));
    slider7->setRange (0, 127, 1);
    slider7->setSliderStyle (Slider::LinearBar);
    slider7->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider7->addListener (this);

    addAndMakeVisible (slider8 = new Slider ("new slider"));
    slider8->setRange (0, 127, 1);
    slider8->setSliderStyle (Slider::LinearBar);
    slider8->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider8->addListener (this);

    addAndMakeVisible (slider9 = new Slider ("new slider"));
    slider9->setRange (0, 127, 1);
    slider9->setSliderStyle (Slider::LinearBar);
    slider9->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    slider9->addListener (this);

    addAndMakeVisible (textButton3 = new TextButton ("new button"));
    textButton3->setButtonText ("Learn");
    textButton3->addListener (this);

    addAndMakeVisible (textButton4 = new TextButton ("new button"));
    textButton4->setButtonText ("Learn");
    textButton4->addListener (this);

    addAndMakeVisible (textButton5 = new TextButton ("new button"));
    textButton5->setButtonText ("Learn");
    textButton5->addListener (this);

    addAndMakeVisible (textButton6 = new TextButton ("new button"));
    textButton6->setButtonText ("Learn");
    textButton6->addListener (this);

    addAndMakeVisible (textButton7 = new TextButton ("new button"));
    textButton7->setButtonText ("Learn");
    textButton7->addListener (this);

    addAndMakeVisible (textButton8 = new TextButton ("new button"));
    textButton8->setButtonText ("Learn");
    textButton8->addListener (this);

    addAndMakeVisible (textButton9 = new TextButton ("new button"));
    textButton9->setButtonText ("Learn");
    textButton9->addListener (this);

    addAndMakeVisible (label = new Label ("new label",
                                          "Trigger Note"));
    label->setFont (Font (12.0000f, Font::plain));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label2 = new Label ("new label",
                                           "X-CC Off Value"));
    label2->setFont (Font (12.0000f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label3 = new Label ("new label",
                                           "X-CC #"));
    label3->setFont (Font (12.0000f, Font::plain));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label4 = new Label ("new label",
                                           "Velocity/Y-CC Off Value"));
    label4->setFont (Font (12.0000f, Font::plain));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label5 = new Label ("new label",
                                           "Velocity/Y-CC On Value"));
    label5->setFont (Font (12.0000f, Font::plain));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label6 = new Label ("new label",
                                           "Y-CC #"));
    label6->setFont (Font (12.0000f, Font::plain));
    label6->setJustificationType (Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label7 = new Label ("new label",
                                           "Note"));
    label7->setFont (Font (12.0000f, Font::plain));
    label7->setJustificationType (Justification::centredLeft);
    label7->setEditable (false, false, false);
    label7->setColour (TextEditor::textColourId, Colours::black);
    label7->setColour (TextEditor::backgroundColourId, Colour (0x0));

    addAndMakeVisible (label8 = new Label ("new label",
                                           "Roundness"));
    label8->setFont (Font (12.0000f, Font::plain));
    label8->setJustificationType (Justification::centredLeft);
    label8->setEditable (false, false, false);
    label8->setColour (TextEditor::textColourId, Colours::black);
    label8->setColour (TextEditor::backgroundColourId, Colour (0x0));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (400, 400);

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PadEditor::~PadEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    deleteAndZero (textEditor);
    deleteAndZero (textButton);
    deleteAndZero (textButton2);
    deleteAndZero (component);
    deleteAndZero (toggleButton);
    deleteAndZero (toggleButton2);
    deleteAndZero (toggleButton3);
    deleteAndZero (slider);
    deleteAndZero (slider2);
    deleteAndZero (toggleButton4);
    deleteAndZero (toggleButton5);
    deleteAndZero (toggleButton6);
    deleteAndZero (toggleButton7);
    deleteAndZero (toggleButton8);
    deleteAndZero (toggleButton9);
    deleteAndZero (toggleButton10);
    deleteAndZero (slider3);
    deleteAndZero (slider4);
    deleteAndZero (slider5);
    deleteAndZero (slider6);
    deleteAndZero (slider7);
    deleteAndZero (slider8);
    deleteAndZero (slider9);
    deleteAndZero (textButton3);
    deleteAndZero (textButton4);
    deleteAndZero (textButton5);
    deleteAndZero (textButton6);
    deleteAndZero (textButton7);
    deleteAndZero (textButton8);
    deleteAndZero (textButton9);
    deleteAndZero (label);
    deleteAndZero (label2);
    deleteAndZero (label3);
    deleteAndZero (label4);
    deleteAndZero (label5);
    deleteAndZero (label6);
    deleteAndZero (label7);
    deleteAndZero (label8);

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PadEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (Font (12.0000f, Font::plain));
    g.drawText ("Icon Size",
                147, 131, 85, 13,
                Justification::centredLeft, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PadEditor::resized()
{
    textEditor->setBounds (8, 16, 224, 88);
    textButton->setBounds (8, 128, 56, 16);
    textButton2->setBounds (72, 128, 64, 16);
    component->setBounds (8, 152, 224, 192);
    toggleButton->setBounds (8, 352, 72, 16);
    toggleButton2->setBounds (80, 352, 88, 16);
    toggleButton3->setBounds (8, 104, 150, 16);
    slider->setBounds (72, 376, 144, 16);
    slider2->setBounds (144, 128, 88, 16);
    toggleButton4->setBounds (248, 8, 64, 24);
    toggleButton5->setBounds (320, 8, 56, 24);
    toggleButton6->setBounds (248, 24, 150, 24);
    toggleButton7->setBounds (248, 40, 150, 24);
    toggleButton8->setBounds (248, 56, 150, 24);
    toggleButton9->setBounds (248, 72, 150, 24);
    toggleButton10->setBounds (248, 88, 150, 24);
    slider3->setBounds (248, 136, 96, 16);
    slider4->setBounds (248, 176, 96, 16);
    slider5->setBounds (248, 216, 96, 16);
    slider6->setBounds (248, 256, 96, 16);
    slider7->setBounds (248, 296, 96, 16);
    slider8->setBounds (248, 336, 96, 16);
    slider9->setBounds (248, 376, 96, 16);
    textButton3->setBounds (352, 136, 40, 16);
    textButton4->setBounds (352, 176, 40, 16);
    textButton5->setBounds (352, 216, 40, 16);
    textButton6->setBounds (352, 256, 40, 16);
    textButton7->setBounds (352, 296, 40, 16);
    textButton8->setBounds (352, 336, 40, 16);
    textButton9->setBounds (352, 376, 40, 16);
    label->setBounds (248, 360, 150, 16);
    label2->setBounds (248, 320, 150, 16);
    label3->setBounds (248, 280, 150, 16);
    label4->setBounds (248, 240, 150, 16);
    label5->setBounds (248, 200, 150, 16);
    label6->setBounds (248, 160, 150, 16);
    label7->setBounds (248, 120, 150, 16);
    label8->setBounds (8, 376, 64, 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PadEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }
    else if (buttonThatWasClicked == textButton2)
    {
        //[UserButtonCode_textButton2] -- add your button handler code here..
        //[/UserButtonCode_textButton2]
    }
    else if (buttonThatWasClicked == toggleButton)
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
        //[/UserButtonCode_toggleButton]
    }
    else if (buttonThatWasClicked == toggleButton2)
    {
        //[UserButtonCode_toggleButton2] -- add your button handler code here..
        //[/UserButtonCode_toggleButton2]
    }
    else if (buttonThatWasClicked == toggleButton3)
    {
        //[UserButtonCode_toggleButton3] -- add your button handler code here..
        //[/UserButtonCode_toggleButton3]
    }
    else if (buttonThatWasClicked == toggleButton4)
    {
        //[UserButtonCode_toggleButton4] -- add your button handler code here..
        //[/UserButtonCode_toggleButton4]
    }
    else if (buttonThatWasClicked == toggleButton5)
    {
        //[UserButtonCode_toggleButton5] -- add your button handler code here..
        //[/UserButtonCode_toggleButton5]
    }
    else if (buttonThatWasClicked == toggleButton6)
    {
        //[UserButtonCode_toggleButton6] -- add your button handler code here..
        //[/UserButtonCode_toggleButton6]
    }
    else if (buttonThatWasClicked == toggleButton7)
    {
        //[UserButtonCode_toggleButton7] -- add your button handler code here..
        //[/UserButtonCode_toggleButton7]
    }
    else if (buttonThatWasClicked == toggleButton8)
    {
        //[UserButtonCode_toggleButton8] -- add your button handler code here..
        //[/UserButtonCode_toggleButton8]
    }
    else if (buttonThatWasClicked == toggleButton9)
    {
        //[UserButtonCode_toggleButton9] -- add your button handler code here..
        //[/UserButtonCode_toggleButton9]
    }
    else if (buttonThatWasClicked == toggleButton10)
    {
        //[UserButtonCode_toggleButton10] -- add your button handler code here..
        //[/UserButtonCode_toggleButton10]
    }
    else if (buttonThatWasClicked == textButton3)
    {
        //[UserButtonCode_textButton3] -- add your button handler code here..
        //[/UserButtonCode_textButton3]
    }
    else if (buttonThatWasClicked == textButton4)
    {
        //[UserButtonCode_textButton4] -- add your button handler code here..
        //[/UserButtonCode_textButton4]
    }
    else if (buttonThatWasClicked == textButton5)
    {
        //[UserButtonCode_textButton5] -- add your button handler code here..
        //[/UserButtonCode_textButton5]
    }
    else if (buttonThatWasClicked == textButton6)
    {
        //[UserButtonCode_textButton6] -- add your button handler code here..
        //[/UserButtonCode_textButton6]
    }
    else if (buttonThatWasClicked == textButton7)
    {
        //[UserButtonCode_textButton7] -- add your button handler code here..
        //[/UserButtonCode_textButton7]
    }
    else if (buttonThatWasClicked == textButton8)
    {
        //[UserButtonCode_textButton8] -- add your button handler code here..
        //[/UserButtonCode_textButton8]
    }
    else if (buttonThatWasClicked == textButton9)
    {
        //[UserButtonCode_textButton9] -- add your button handler code here..
        //[/UserButtonCode_textButton9]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void PadEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider)
    {
        //[UserSliderCode_slider] -- add your slider handling code here..
        //[/UserSliderCode_slider]
    }
    else if (sliderThatWasMoved == slider2)
    {
        //[UserSliderCode_slider2] -- add your slider handling code here..
        //[/UserSliderCode_slider2]
    }
    else if (sliderThatWasMoved == slider3)
    {
        //[UserSliderCode_slider3] -- add your slider handling code here..
        //[/UserSliderCode_slider3]
    }
    else if (sliderThatWasMoved == slider4)
    {
        //[UserSliderCode_slider4] -- add your slider handling code here..
        //[/UserSliderCode_slider4]
    }
    else if (sliderThatWasMoved == slider5)
    {
        //[UserSliderCode_slider5] -- add your slider handling code here..
        //[/UserSliderCode_slider5]
    }
    else if (sliderThatWasMoved == slider6)
    {
        //[UserSliderCode_slider6] -- add your slider handling code here..
        //[/UserSliderCode_slider6]
    }
    else if (sliderThatWasMoved == slider7)
    {
        //[UserSliderCode_slider7] -- add your slider handling code here..
        //[/UserSliderCode_slider7]
    }
    else if (sliderThatWasMoved == slider8)
    {
        //[UserSliderCode_slider8] -- add your slider handling code here..
        //[/UserSliderCode_slider8]
    }
    else if (sliderThatWasMoved == slider9)
    {
        //[UserSliderCode_slider9] -- add your slider handling code here..
        //[/UserSliderCode_slider9]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PadEditor" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="400" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff">
    <TEXT pos="147 131 85 13" fill="solid: ff000000" hasStroke="0" text="Icon Size"
          fontname="Default font" fontsize="12" bold="0" italic="0" justification="33"/>
  </BACKGROUND>
  <TEXTEDITOR name="new text editor" id="81f86aa01d380218" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="8 16 224 88" outlinecol="ff000000"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="8ef9bb4d23ffe335" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="8 128 56 16" buttonText="Clear Icon"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="f67140f7436253d3" memberName="textButton2"
              virtualName="" explicitFocusOrder="0" pos="72 128 64 16" buttonText="Choose Icon"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="a5088001e17d97c2" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="8 152 224 192" class="ColourSelector"
                    params=""/>
  <TOGGLEBUTTON name="new toggle button" id="78354f7e73dff13b" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 352 72 16" buttonText="Show Dot"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="6e3e5c739747a384" memberName="toggleButton2"
                virtualName="" explicitFocusOrder="0" pos="80 352 88 16" buttonText="Show Values"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="cb0ab0bfda486c5d" memberName="toggleButton3"
                virtualName="" explicitFocusOrder="0" pos="8 104 150 16" buttonText="Centered Text"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="new slider" id="2cc888b8a3d25ea0" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="72 376 144 16" textboxtext="0" min="0"
          max="10" int="0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="c79c8104b3f863e6" memberName="slider2"
          virtualName="" explicitFocusOrder="0" pos="144 128 88 16" textboxtext="0"
          min="0" max="10" int="0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="new toggle button" id="b7ebd11aaed1d4e7" memberName="toggleButton4"
                virtualName="" explicitFocusOrder="0" pos="248 8 64 24" buttonText="Note"
                connectedEdges="0" needsCallback="1" radioGroupId="1" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="571c0d0fd114799a" memberName="toggleButton5"
                virtualName="" explicitFocusOrder="0" pos="320 8 56 24" buttonText="CC"
                connectedEdges="0" needsCallback="1" radioGroupId="1" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="bcc518306309c9c6" memberName="toggleButton6"
                virtualName="" explicitFocusOrder="0" pos="248 24 150 24" buttonText="Use Y-Position"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="25c413f4e0015f9e" memberName="toggleButton7"
                virtualName="" explicitFocusOrder="0" pos="248 40 150 24" buttonText="Use X-Position"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="956896f39756dc11" memberName="toggleButton8"
                virtualName="" explicitFocusOrder="0" pos="248 56 150 24" buttonText="X is Pitch Bend"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="a393febfd86c7457" memberName="toggleButton9"
                virtualName="" explicitFocusOrder="0" pos="248 72 150 24" buttonText="Send Off Values"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="a37d35206a4637c7" memberName="toggleButton10"
                virtualName="" explicitFocusOrder="0" pos="248 88 150 24" buttonText="Toggle Mode"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="new slider" id="6ed9287fe9e00de0" memberName="slider3"
          virtualName="" explicitFocusOrder="0" pos="248 136 96 16" min="0"
          max="127" int="1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="11e9ecb9bf8d288c" memberName="slider4"
          virtualName="" explicitFocusOrder="0" pos="248 176 96 16" min="0"
          max="127" int="1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="61e95351565d6a9" memberName="slider5" virtualName=""
          explicitFocusOrder="0" pos="248 216 96 16" min="0" max="127"
          int="1" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="300e1c3880617ac4" memberName="slider6"
          virtualName="" explicitFocusOrder="0" pos="248 256 96 16" min="0"
          max="127" int="1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="a768eb776352aa27" memberName="slider7"
          virtualName="" explicitFocusOrder="0" pos="248 296 96 16" min="0"
          max="127" int="1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="e521d9ed41d7b815" memberName="slider8"
          virtualName="" explicitFocusOrder="0" pos="248 336 96 16" min="0"
          max="127" int="1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="36c8b2ef11b23153" memberName="slider9"
          virtualName="" explicitFocusOrder="0" pos="248 376 96 16" min="0"
          max="127" int="1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TEXTBUTTON name="new button" id="831d9a7caf1c0bea" memberName="textButton3"
              virtualName="" explicitFocusOrder="0" pos="352 136 40 16" buttonText="Learn"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="5485e2d984d98f2b" memberName="textButton4"
              virtualName="" explicitFocusOrder="0" pos="352 176 40 16" buttonText="Learn"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="6780275d173337f3" memberName="textButton5"
              virtualName="" explicitFocusOrder="0" pos="352 216 40 16" buttonText="Learn"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="5ebec089d1f31d30" memberName="textButton6"
              virtualName="" explicitFocusOrder="0" pos="352 256 40 16" buttonText="Learn"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="c96ce55b613598a3" memberName="textButton7"
              virtualName="" explicitFocusOrder="0" pos="352 296 40 16" buttonText="Learn"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="fb940de3239537e7" memberName="textButton8"
              virtualName="" explicitFocusOrder="0" pos="352 336 40 16" buttonText="Learn"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="c9dc6b16563639b1" memberName="textButton9"
              virtualName="" explicitFocusOrder="0" pos="352 376 40 16" buttonText="Learn"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="8efa471b4f4d645c" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="248 360 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Trigger Note" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="f7d967812447fa98" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="248 320 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="X-CC Off Value" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="ca99b511b68ecf92" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="248 280 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="X-CC #" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="3d8643a52c316" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="248 240 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Velocity/Y-CC Off Value" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="c4280414863f2525" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="248 200 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Velocity/Y-CC On Value" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="2bd490db20aac7b1" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="248 160 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Y-CC #" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="36643a217f44c1a6" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="248 120 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Note" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="aa5b0fd5be9bde2d" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="8 376 64 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Roundness" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12"
         bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
