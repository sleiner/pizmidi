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

#include "PadEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PadEditor::PadEditor ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    textEditor.reset (new juce::TextEditor ("new text editor"));
    addAndMakeVisible (textEditor.get());
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setColour (juce::TextEditor::outlineColourId, juce::Colours::black);
    textEditor->setText (juce::String());

    textEditor->setBounds (8, 16, 224, 88);

    textButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (textButton.get());
    textButton->setButtonText (TRANS("Clear Icon"));
    textButton->addListener (this);

    textButton->setBounds (8, 128, 56, 16);

    textButton2.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (textButton2.get());
    textButton2->setButtonText (TRANS("Choose Icon"));
    textButton2->addListener (this);

    textButton2->setBounds (72, 128, 64, 16);

    component.reset (new juce::ColourSelector());
    addAndMakeVisible (component.get());
    component->setName ("new component");

    component->setBounds (8, 152, 224, 192);

    toggleButton.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton.get());
    toggleButton->setButtonText (TRANS("Show Dot"));
    toggleButton->addListener (this);

    toggleButton->setBounds (8, 352, 72, 16);

    toggleButton2.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton2.get());
    toggleButton2->setButtonText (TRANS("Show Values"));
    toggleButton2->addListener (this);

    toggleButton2->setBounds (80, 352, 88, 16);

    toggleButton3.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton3.get());
    toggleButton3->setButtonText (TRANS("Centered Text"));
    toggleButton3->addListener (this);

    toggleButton3->setBounds (8, 104, 150, 16);

    slider.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (slider.get());
    slider->setRange (0, 10, 0);
    slider->setSliderStyle (juce::Slider::LinearBar);
    slider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    slider->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0x00000000));
    slider->addListener (this);

    slider->setBounds (72, 376, 144, 16);

    slider2.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (slider2.get());
    slider2->setRange (0, 10, 0);
    slider2->setSliderStyle (juce::Slider::LinearBar);
    slider2->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    slider2->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0x00000000));
    slider2->addListener (this);

    slider2->setBounds (144, 128, 88, 16);

    toggleButton4.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton4.get());
    toggleButton4->setButtonText (TRANS("Note"));
    toggleButton4->setRadioGroupId (1);
    toggleButton4->addListener (this);

    toggleButton4->setBounds (248, 8, 64, 24);

    toggleButton5.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton5.get());
    toggleButton5->setButtonText (TRANS("CC"));
    toggleButton5->setRadioGroupId (1);
    toggleButton5->addListener (this);

    toggleButton5->setBounds (320, 8, 56, 24);

    toggleButton6.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton6.get());
    toggleButton6->setButtonText (TRANS("Use Y-Position"));
    toggleButton6->addListener (this);

    toggleButton6->setBounds (248, 24, 150, 24);

    toggleButton7.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton7.get());
    toggleButton7->setButtonText (TRANS("Use X-Position"));
    toggleButton7->addListener (this);

    toggleButton7->setBounds (248, 40, 150, 24);

    toggleButton8.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton8.get());
    toggleButton8->setButtonText (TRANS("X is Pitch Bend"));
    toggleButton8->addListener (this);

    toggleButton8->setBounds (248, 56, 150, 24);

    toggleButton9.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton9.get());
    toggleButton9->setButtonText (TRANS("Send Off Values"));
    toggleButton9->addListener (this);

    toggleButton9->setBounds (248, 72, 150, 24);

    toggleButton10.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton10.get());
    toggleButton10->setButtonText (TRANS("Toggle Mode"));
    toggleButton10->addListener (this);

    toggleButton10->setBounds (248, 88, 150, 24);

    slider3.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (slider3.get());
    slider3->setRange (0, 127, 1);
    slider3->setSliderStyle (juce::Slider::LinearBar);
    slider3->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    slider3->addListener (this);

    slider3->setBounds (248, 136, 96, 16);

    slider4.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (slider4.get());
    slider4->setRange (0, 127, 1);
    slider4->setSliderStyle (juce::Slider::LinearBar);
    slider4->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    slider4->addListener (this);

    slider4->setBounds (248, 176, 96, 16);

    slider5.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (slider5.get());
    slider5->setRange (0, 127, 1);
    slider5->setSliderStyle (juce::Slider::LinearBar);
    slider5->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    slider5->addListener (this);

    slider5->setBounds (248, 216, 96, 16);

    slider6.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (slider6.get());
    slider6->setRange (0, 127, 1);
    slider6->setSliderStyle (juce::Slider::LinearBar);
    slider6->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    slider6->addListener (this);

    slider6->setBounds (248, 256, 96, 16);

    slider7.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (slider7.get());
    slider7->setRange (0, 127, 1);
    slider7->setSliderStyle (juce::Slider::LinearBar);
    slider7->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    slider7->addListener (this);

    slider7->setBounds (248, 296, 96, 16);

    slider8.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (slider8.get());
    slider8->setRange (0, 127, 1);
    slider8->setSliderStyle (juce::Slider::LinearBar);
    slider8->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    slider8->addListener (this);

    slider8->setBounds (248, 336, 96, 16);

    slider9.reset (new juce::Slider ("new slider"));
    addAndMakeVisible (slider9.get());
    slider9->setRange (0, 127, 1);
    slider9->setSliderStyle (juce::Slider::LinearBar);
    slider9->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    slider9->addListener (this);

    slider9->setBounds (248, 376, 96, 16);

    textButton3.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (textButton3.get());
    textButton3->setButtonText (TRANS("Learn"));
    textButton3->addListener (this);

    textButton3->setBounds (352, 136, 40, 16);

    textButton4.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (textButton4.get());
    textButton4->setButtonText (TRANS("Learn"));
    textButton4->addListener (this);

    textButton4->setBounds (352, 176, 40, 16);

    textButton5.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (textButton5.get());
    textButton5->setButtonText (TRANS("Learn"));
    textButton5->addListener (this);

    textButton5->setBounds (352, 216, 40, 16);

    textButton6.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (textButton6.get());
    textButton6->setButtonText (TRANS("Learn"));
    textButton6->addListener (this);

    textButton6->setBounds (352, 256, 40, 16);

    textButton7.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (textButton7.get());
    textButton7->setButtonText (TRANS("Learn"));
    textButton7->addListener (this);

    textButton7->setBounds (352, 296, 40, 16);

    textButton8.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (textButton8.get());
    textButton8->setButtonText (TRANS("Learn"));
    textButton8->addListener (this);

    textButton8->setBounds (352, 336, 40, 16);

    textButton9.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (textButton9.get());
    textButton9->setButtonText (TRANS("Learn"));
    textButton9->addListener (this);

    textButton9->setBounds (352, 376, 40, 16);

    label.reset (new juce::Label ("new label",
                                  TRANS("Trigger Note")));
    addAndMakeVisible (label.get());
    label->setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (juce::Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label->setBounds (248, 360, 150, 16);

    label2.reset (new juce::Label ("new label",
                                   TRANS("X-CC Off Value")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label2->setBounds (248, 320, 150, 16);

    label3.reset (new juce::Label ("new label",
                                   TRANS("X-CC #")));
    addAndMakeVisible (label3.get());
    label3->setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (juce::Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label3->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label3->setBounds (248, 280, 150, 16);

    label4.reset (new juce::Label ("new label",
                                   TRANS("Velocity/Y-CC Off Value")));
    addAndMakeVisible (label4.get());
    label4->setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (juce::Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label4->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label4->setBounds (248, 240, 150, 16);

    label5.reset (new juce::Label ("new label",
                                   TRANS("Velocity/Y-CC On Value")));
    addAndMakeVisible (label5.get());
    label5->setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (juce::Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label5->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label5->setBounds (248, 200, 150, 16);

    label6.reset (new juce::Label ("new label",
                                   TRANS("Y-CC #")));
    addAndMakeVisible (label6.get());
    label6->setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (juce::Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label6->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label6->setBounds (248, 160, 150, 16);

    label7.reset (new juce::Label ("new label",
                                   TRANS("Note")));
    addAndMakeVisible (label7.get());
    label7->setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (juce::Justification::centredLeft);
    label7->setEditable (false, false, false);
    label7->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label7->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label7->setBounds (248, 120, 150, 16);

    label8.reset (new juce::Label ("new label",
                                   TRANS("Roundness")));
    addAndMakeVisible (label8.get());
    label8->setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label8->setJustificationType (juce::Justification::centredLeft);
    label8->setEditable (false, false, false);
    label8->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label8->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label8->setBounds (8, 376, 64, 16);


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

    textEditor = nullptr;
    textButton = nullptr;
    textButton2 = nullptr;
    component = nullptr;
    toggleButton = nullptr;
    toggleButton2 = nullptr;
    toggleButton3 = nullptr;
    slider = nullptr;
    slider2 = nullptr;
    toggleButton4 = nullptr;
    toggleButton5 = nullptr;
    toggleButton6 = nullptr;
    toggleButton7 = nullptr;
    toggleButton8 = nullptr;
    toggleButton9 = nullptr;
    toggleButton10 = nullptr;
    slider3 = nullptr;
    slider4 = nullptr;
    slider5 = nullptr;
    slider6 = nullptr;
    slider7 = nullptr;
    slider8 = nullptr;
    slider9 = nullptr;
    textButton3 = nullptr;
    textButton4 = nullptr;
    textButton5 = nullptr;
    textButton6 = nullptr;
    textButton7 = nullptr;
    textButton8 = nullptr;
    textButton9 = nullptr;
    label = nullptr;
    label2 = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    label5 = nullptr;
    label6 = nullptr;
    label7 = nullptr;
    label8 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PadEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colours::white);

    {
        int x = 147, y = 131, width = 85, height = 13;
        juce::String text (TRANS("Icon Size"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PadEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PadEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButton.get())
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }
    else if (buttonThatWasClicked == textButton2.get())
    {
        //[UserButtonCode_textButton2] -- add your button handler code here..
        //[/UserButtonCode_textButton2]
    }
    else if (buttonThatWasClicked == toggleButton.get())
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
        //[/UserButtonCode_toggleButton]
    }
    else if (buttonThatWasClicked == toggleButton2.get())
    {
        //[UserButtonCode_toggleButton2] -- add your button handler code here..
        //[/UserButtonCode_toggleButton2]
    }
    else if (buttonThatWasClicked == toggleButton3.get())
    {
        //[UserButtonCode_toggleButton3] -- add your button handler code here..
        //[/UserButtonCode_toggleButton3]
    }
    else if (buttonThatWasClicked == toggleButton4.get())
    {
        //[UserButtonCode_toggleButton4] -- add your button handler code here..
        //[/UserButtonCode_toggleButton4]
    }
    else if (buttonThatWasClicked == toggleButton5.get())
    {
        //[UserButtonCode_toggleButton5] -- add your button handler code here..
        //[/UserButtonCode_toggleButton5]
    }
    else if (buttonThatWasClicked == toggleButton6.get())
    {
        //[UserButtonCode_toggleButton6] -- add your button handler code here..
        //[/UserButtonCode_toggleButton6]
    }
    else if (buttonThatWasClicked == toggleButton7.get())
    {
        //[UserButtonCode_toggleButton7] -- add your button handler code here..
        //[/UserButtonCode_toggleButton7]
    }
    else if (buttonThatWasClicked == toggleButton8.get())
    {
        //[UserButtonCode_toggleButton8] -- add your button handler code here..
        //[/UserButtonCode_toggleButton8]
    }
    else if (buttonThatWasClicked == toggleButton9.get())
    {
        //[UserButtonCode_toggleButton9] -- add your button handler code here..
        //[/UserButtonCode_toggleButton9]
    }
    else if (buttonThatWasClicked == toggleButton10.get())
    {
        //[UserButtonCode_toggleButton10] -- add your button handler code here..
        //[/UserButtonCode_toggleButton10]
    }
    else if (buttonThatWasClicked == textButton3.get())
    {
        //[UserButtonCode_textButton3] -- add your button handler code here..
        //[/UserButtonCode_textButton3]
    }
    else if (buttonThatWasClicked == textButton4.get())
    {
        //[UserButtonCode_textButton4] -- add your button handler code here..
        //[/UserButtonCode_textButton4]
    }
    else if (buttonThatWasClicked == textButton5.get())
    {
        //[UserButtonCode_textButton5] -- add your button handler code here..
        //[/UserButtonCode_textButton5]
    }
    else if (buttonThatWasClicked == textButton6.get())
    {
        //[UserButtonCode_textButton6] -- add your button handler code here..
        //[/UserButtonCode_textButton6]
    }
    else if (buttonThatWasClicked == textButton7.get())
    {
        //[UserButtonCode_textButton7] -- add your button handler code here..
        //[/UserButtonCode_textButton7]
    }
    else if (buttonThatWasClicked == textButton8.get())
    {
        //[UserButtonCode_textButton8] -- add your button handler code here..
        //[/UserButtonCode_textButton8]
    }
    else if (buttonThatWasClicked == textButton9.get())
    {
        //[UserButtonCode_textButton9] -- add your button handler code here..
        //[/UserButtonCode_textButton9]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void PadEditor::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider.get())
    {
        //[UserSliderCode_slider] -- add your slider handling code here..
        //[/UserSliderCode_slider]
    }
    else if (sliderThatWasMoved == slider2.get())
    {
        //[UserSliderCode_slider2] -- add your slider handling code here..
        //[/UserSliderCode_slider2]
    }
    else if (sliderThatWasMoved == slider3.get())
    {
        //[UserSliderCode_slider3] -- add your slider handling code here..
        //[/UserSliderCode_slider3]
    }
    else if (sliderThatWasMoved == slider4.get())
    {
        //[UserSliderCode_slider4] -- add your slider handling code here..
        //[/UserSliderCode_slider4]
    }
    else if (sliderThatWasMoved == slider5.get())
    {
        //[UserSliderCode_slider5] -- add your slider handling code here..
        //[/UserSliderCode_slider5]
    }
    else if (sliderThatWasMoved == slider6.get())
    {
        //[UserSliderCode_slider6] -- add your slider handling code here..
        //[/UserSliderCode_slider6]
    }
    else if (sliderThatWasMoved == slider7.get())
    {
        //[UserSliderCode_slider7] -- add your slider handling code here..
        //[/UserSliderCode_slider7]
    }
    else if (sliderThatWasMoved == slider8.get())
    {
        //[UserSliderCode_slider8] -- add your slider handling code here..
        //[/UserSliderCode_slider8]
    }
    else if (sliderThatWasMoved == slider9.get())
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
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PadEditor" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="400" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff">
    <TEXT pos="147 131 85 13" fill="solid: ff000000" hasStroke="0" text="Icon Size"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
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
                    virtualName="" explicitFocusOrder="0" pos="8 152 224 192" class="juce::ColourSelector"
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
          explicitFocusOrder="0" pos="72 376 144 16" textboxtext="0" min="0.0"
          max="10.0" int="0.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="c79c8104b3f863e6" memberName="slider2"
          virtualName="" explicitFocusOrder="0" pos="144 128 88 16" textboxtext="0"
          min="0.0" max="10.0" int="0.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
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
          virtualName="" explicitFocusOrder="0" pos="248 136 96 16" min="0.0"
          max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="11e9ecb9bf8d288c" memberName="slider4"
          virtualName="" explicitFocusOrder="0" pos="248 176 96 16" min="0.0"
          max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="61e95351565d6a9" memberName="slider5" virtualName=""
          explicitFocusOrder="0" pos="248 216 96 16" min="0.0" max="127.0"
          int="1.0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="300e1c3880617ac4" memberName="slider6"
          virtualName="" explicitFocusOrder="0" pos="248 256 96 16" min="0.0"
          max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="a768eb776352aa27" memberName="slider7"
          virtualName="" explicitFocusOrder="0" pos="248 296 96 16" min="0.0"
          max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="e521d9ed41d7b815" memberName="slider8"
          virtualName="" explicitFocusOrder="0" pos="248 336 96 16" min="0.0"
          max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="36c8b2ef11b23153" memberName="slider9"
          virtualName="" explicitFocusOrder="0" pos="248 376 96 16" min="0.0"
          max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
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
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="f7d967812447fa98" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="248 320 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="X-CC Off Value" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="ca99b511b68ecf92" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="248 280 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="X-CC #" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="3d8643a52c316" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="248 240 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Velocity/Y-CC Off Value" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="c4280414863f2525" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="248 200 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Velocity/Y-CC On Value" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="12.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="2bd490db20aac7b1" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="248 160 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Y-CC #" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="36643a217f44c1a6" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="248 120 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Note" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="aa5b0fd5be9bde2d" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="8 376 64 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Roundness" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

