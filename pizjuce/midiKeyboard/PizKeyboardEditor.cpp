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

#include "PizKeyboardEditor.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
using juce::jmax;
using juce::roundToInt;
//[/MiscUserDefs]

//==============================================================================
midiKeyboardEditor::midiKeyboardEditor (PizKeyboard* const ownerFilter)
    : AudioProcessorEditor (ownerFilter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    keyWidthSlider.reset (new juce::Slider ("keyWidthSlider"));
    addAndMakeVisible (keyWidthSlider.get());
    keyWidthSlider->setTooltip (TRANS ("Key Width"));
    keyWidthSlider->setRange (0, 100, 1);
    keyWidthSlider->setSliderStyle (juce::Slider::LinearBar);
    keyWidthSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    keyWidthSlider->setColour (juce::Slider::backgroundColourId, juce::Colour (0x5dffffff));
    keyWidthSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0x5d000000));
    keyWidthSlider->setColour (juce::Slider::textBoxTextColourId, juce::Colour (0x00000000));
    keyWidthSlider->addListener (this);

    keyWidthSlider->setBounds (4, 1, 100, 14);

    chSlider.reset (new juce::Slider ("Channel"));
    addAndMakeVisible (chSlider.get());
    chSlider->setTooltip (TRANS ("MIDI Channel"));
    chSlider->setRange (1, 16, 1);
    chSlider->setSliderStyle (juce::Slider::IncDecButtons);
    chSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 30, 20);
    chSlider->setColour (juce::Slider::backgroundColourId, juce::Colour (0x5dffffff));
    chSlider->setColour (juce::Slider::thumbColourId, juce::Colour (0x5dbbbbff));
    chSlider->addListener (this);

    chSlider->setBounds (119, 1, 60, 14);

    grabQwertyButton.reset (new juce::TextButton ("qwerty"));
    addAndMakeVisible (grabQwertyButton.get());
    grabQwertyButton->setTooltip (TRANS ("Attempt to grab keyboard focus; right-click to allow QWERTY input without keyboard focus"));
    grabQwertyButton->setButtonText (TRANS ("Grab QWERTY"));

    grabQwertyButton->setBounds (189, 1, 87, 14);

    velocitySlider.reset (new juce::Slider ("velocity"));
    addAndMakeVisible (velocitySlider.get());
    velocitySlider->setTooltip (TRANS ("Velocity"));
    velocitySlider->setRange (0, 127, 1);
    velocitySlider->setSliderStyle (juce::Slider::LinearBar);
    velocitySlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    velocitySlider->setColour (juce::Slider::backgroundColourId, juce::Colour (0x5dffffff));
    velocitySlider->setColour (juce::Slider::thumbColourId, juce::Colour (0x5d000000));
    velocitySlider->setColour (juce::Slider::textBoxTextColourId, juce::Colours::black);
    velocitySlider->addListener (this);

    velocitySlider->setBounds (286, 1, 110, 14);

    yButton.reset (new juce::ToggleButton ("Use Y Button"));
    addAndMakeVisible (yButton.get());
    yButton->setTooltip (TRANS ("Use mouse Y-position for velocity"));
    yButton->setButtonText (TRANS ("Y"));
    yButton->addListener (this);

    yButton->setBounds (401, -1, 32, 18);

    inputToggleButton.reset (new juce::ToggleButton ("Input Toggle Button"));
    addAndMakeVisible (inputToggleButton.get());
    inputToggleButton->setTooltip (TRANS ("Input notes as toggles"));
    inputToggleButton->setButtonText (TRANS ("Toggle"));
    inputToggleButton->addListener (this);

    inputToggleButton->setBounds (435, -1, 63, 18);

    aboutButton.reset (new juce::TextButton ("About"));
    addAndMakeVisible (aboutButton.get());
    aboutButton->setTooltip (TRANS ("midiKeyboard instructions"));
    aboutButton->addListener (this);

    aboutButton->setBounds (516, 1, 40, 14);

    hideButton.reset (new juce::TextButton ("Hide"));
    addAndMakeVisible (hideButton.get());
    hideButton->setTooltip (TRANS ("Hide editing panel (Alt-click keyboard to show)"));
    hideButton->setButtonText (TRANS ("_"));
    hideButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    hideButton->addListener (this);

    hideButton->setBounds (498, 1, 14, 14);

    resizer.reset (new juce::ResizableCornerComponent (this, &resizeLimits));
    addAndMakeVisible (resizer.get());
    resizer->setName ("new component");

    midiKeyboard.reset (new PizKeyboardComponent (ownerFilter->editorKbState, juce::MidiKeyboardComponent::horizontalKeyboard));
    addAndMakeVisible (midiKeyboard.get());

    aboutBox.reset (new juce::TextEditor ("Instructions"));
    addAndMakeVisible (aboutBox.get());
    aboutBox->setMultiLine (true);
    aboutBox->setReturnKeyStartsNewLine (false);
    aboutBox->setReadOnly (true);
    aboutBox->setScrollbarsShown (false);
    aboutBox->setCaretVisible (false);
    aboutBox->setPopupMenuEnabled (false);
    aboutBox->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0xdce9e79b));
    aboutBox->setText (TRANS ("Insert Piz Here-> midiKeyboard 1.4\n"
                              "- Right-click to hold note\n"
                              "- Ctrl-click or Middle-click to end all notes\n"
                              "- Shift-click to set QWERTY octave\n"
                              "- Alt-click to show/hide editing panel\n"
                              "- Default settings load from midiKeyboard.fxb"));

    useProgCh.reset (new juce::ToggleButton ("Use ProgCh Button"));
    addAndMakeVisible (useProgCh.get());
    useProgCh->setTooltip (TRANS ("Use Program Change"));
    useProgCh->setButtonText (TRANS ("Use Program Change"));
    useProgCh->addListener (this);

    useCapsLock.reset (new juce::ToggleButton ("UseCapsLock Button"));
    addAndMakeVisible (useCapsLock.get());
    useCapsLock->setTooltip (TRANS ("Respond to QWERTY only when Caps Lock is on"));
    useCapsLock->setButtonText (TRANS ("Use Caps Lock"));
    useCapsLock->addListener (this);

    sendState.reset (new juce::TextButton ("send state"));
    addAndMakeVisible (sendState.get());
    sendState->setTooltip (TRANS ("Send current keyboard state"));
    sendState->setButtonText (TRANS ("Send Current State"));
    sendState->addListener (this);

    showNumbersButton.reset (new juce::ToggleButton ("ShowNumbers Button"));
    addAndMakeVisible (showNumbersButton.get());
    showNumbersButton->setTooltip (TRANS ("Show MIDI note numbers on keys"));
    showNumbersButton->setButtonText (TRANS ("Show Note Numbers"));
    showNumbersButton->addListener (this);

    //[UserPreSize]
    DBG ("PizKeyboardEditor()");
    setMouseClickGrabsKeyboardFocus (true);
    aboutButton->setMouseClickGrabsKeyboardFocus (false);
    keyWidthSlider->setMouseClickGrabsKeyboardFocus (false);
    velocitySlider->setMouseClickGrabsKeyboardFocus (false);
    yButton->setMouseClickGrabsKeyboardFocus (false);
    inputToggleButton->setMouseClickGrabsKeyboardFocus (false);
    hideButton->setMouseClickGrabsKeyboardFocus (false);
    grabQwertyButton->setMouseClickGrabsKeyboardFocus (false);
    grabQwertyButton->addMouseListener (this, false);
    aboutBox->setVisible (false);
    aboutBox->setMouseClickGrabsKeyboardFocus (false);
    aboutBox->addMouseListener (this, false);
    sendState->setVisible (false);
    sendState->setMouseClickGrabsKeyboardFocus (false);
    useProgCh->setVisible (false);
    useProgCh->setMouseClickGrabsKeyboardFocus (false);
    useCapsLock->setVisible (false);
    useCapsLock->setMouseClickGrabsKeyboardFocus (false);
    showNumbersButton->setVisible (false);
    showNumbersButton->setMouseClickGrabsKeyboardFocus (false);
    resizer->setMouseClickGrabsKeyboardFocus (false);
    midiKeyboard->setMouseClickGrabsKeyboardFocus (false);
    const int ch = roundToInt (ownerFilter->getParameter (kChannel) * 15.f);
    midiKeyboard->setMidiChannel (ch + 1);
    midiKeyboard->setMidiChannelsToDisplay (1 << ch);
    midiKeyboard->setLowestVisibleKey (ownerFilter->keyPosition);
    midiKeyboard->setKeyPressBaseOctave (ownerFilter->octave);
    midiKeyboard->setOctaveForMiddleC (ownerFilter->bottomOctave + 5);

    resizeLimits.setSizeLimits (50, 20, 1600, 500);
#if 0
    //[/UserPreSize]

    setSize (560, 140);


    //[Constructor] You can add your own custom stuff here..
#endif
    ownerFilter->addChangeListener (this);
    updateParametersFromFilter();
    midiKeyboard->addChangeListener (this);
    midiKeyboard->addKeyListener (this);
    //[/Constructor]
}

midiKeyboardEditor::~midiKeyboardEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    DBG ("~PizKeyboardEditor()");
    midiKeyboard->removeChangeListener (this);
    getFilter()->removeChangeListener (this);
    //[/Destructor_pre]

    keyWidthSlider    = nullptr;
    chSlider          = nullptr;
    grabQwertyButton  = nullptr;
    velocitySlider    = nullptr;
    yButton           = nullptr;
    inputToggleButton = nullptr;
    aboutButton       = nullptr;
    hideButton        = nullptr;
    resizer           = nullptr;
    midiKeyboard      = nullptr;
    aboutBox          = nullptr;
    useProgCh         = nullptr;
    useCapsLock       = nullptr;
    sendState         = nullptr;
    showNumbersButton = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void midiKeyboardEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    if (getFilter()->getParameter (kHidePanel))
        g.fillAll (juce::Colour (0xff8c8c8c));
    else
    {
        //[/UserPrePaint]

        g.fillAll (juce::Colour (0xff8c8c8c));

        {
            int x = 6, y = 1, width = 100, height = 14;
            juce::String text (TRANS ("Key Width"));
            juce::Colour fillColour = juce::Colours::black;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (fillColour);
            g.setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
            g.drawText (text, x, y, width, height, juce::Justification::centredLeft, true);
        }

        {
            int x = 290, y = 1, width = 100, height = 14;
            juce::String text (TRANS ("Velocity"));
            juce::Colour fillColour = juce::Colours::black;
            //[UserPaintCustomArguments] Customize the painting arguments here..
            //[/UserPaintCustomArguments]
            g.setColour (fillColour);
            g.setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
            g.drawText (text, x, y, width, height, juce::Justification::centredLeft, true);
        }

        //[UserPaint] Add your own custom painting code here..
    }
    //[/UserPaint]
}

void midiKeyboardEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
    midiKeyboard->setBounds (4, 16, getWidth() - 8, getHeight() - 20);
    aboutBox->setBounds (proportionOfWidth (0.5000f) - (450 / 2), proportionOfHeight (0.5000f) - (92 / 2), 450, 92);
    useProgCh->setBounds ((proportionOfWidth (0.5000f) - (450 / 2)) + 288, (proportionOfHeight (0.5000f) - (92 / 2)) + 7, 147, 18);
    useCapsLock->setBounds ((proportionOfWidth (0.5000f) - (450 / 2)) + 288, (proportionOfHeight (0.5000f) - (92 / 2)) + 26, 147, 18);
    sendState->setBounds ((proportionOfWidth (0.5000f) - (450 / 2)) + 291, (proportionOfHeight (0.5000f) - (92 / 2)) + 65, 140, 21);
    showNumbersButton->setBounds ((proportionOfWidth (0.5000f) - (450 / 2)) + 288, (proportionOfHeight (0.5000f) - (92 / 2)) + 45, 147, 18);
    //[UserResized] Add your own custom resize handling here..
    //int topbar = jmax(14,getHeight()/20);
    if (getFilter()->getParameter (kHidePanel) >= 0.5f)
        midiKeyboard->setBounds (4, 4, getWidth() - 8, getHeight() - 8);
    midiKeyboard->setKeyWidth (jmax ((float) getWidth() / 76.f, getFilter()->getParameter (kWidth) * 150.0f + 5.0f));
    getFilter()->lastUIWidth  = getWidth();
    getFilter()->lastUIHeight = getHeight();
    //[/UserResized]
}

void midiKeyboardEditor::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == keyWidthSlider.get())
    {
        //[UserSliderCode_keyWidthSlider] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost (kWidth, (float) (keyWidthSlider->getValue() * 0.01));
        //[/UserSliderCode_keyWidthSlider]
    }
    else if (sliderThatWasMoved == chSlider.get())
    {
        //[UserSliderCode_chSlider] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost (kChannel, (float) (chSlider->getValue() - 1) / 15.f);
        //[/UserSliderCode_chSlider]
    }
    else if (sliderThatWasMoved == velocitySlider.get())
    {
        //[UserSliderCode_velocitySlider] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost (kVelocity, (float) (velocitySlider->getValue()) / 127.f);
        midiKeyboard->setVelocity ((float) (velocitySlider->getValue()) / 127.f, false);
        //[/UserSliderCode_velocitySlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void midiKeyboardEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == yButton.get())
    {
        //[UserButtonCode_yButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost (kUseY, buttonThatWasClicked->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_yButton]
    }
    else if (buttonThatWasClicked == inputToggleButton.get())
    {
        //[UserButtonCode_inputToggleButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost (kToggleInput, buttonThatWasClicked->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_inputToggleButton]
    }
    else if (buttonThatWasClicked == aboutButton.get())
    {
        //[UserButtonCode_aboutButton] -- add your button handler code here..
        aboutBox->setVisible (! aboutBox->isVisible());
        useProgCh->setVisible (aboutBox->isVisible());
        useCapsLock->setVisible (aboutBox->isVisible());
        sendState->setVisible (aboutBox->isVisible());
        showNumbersButton->setVisible (aboutBox->isVisible());
        //[/UserButtonCode_aboutButton]
    }
    else if (buttonThatWasClicked == hideButton.get())
    {
        //[UserButtonCode_hideButton] -- add your button handler code here..
        getFilter()->setParameter (kHidePanel, 1.f);
        //[/UserButtonCode_hideButton]
    }
    else if (buttonThatWasClicked == useProgCh.get())
    {
        //[UserButtonCode_useProgCh] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost (kUseProgCh, buttonThatWasClicked->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_useProgCh]
    }
    else if (buttonThatWasClicked == useCapsLock.get())
    {
        //[UserButtonCode_useCapsLock] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost (kCapsLock, buttonThatWasClicked->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_useCapsLock]
    }
    else if (buttonThatWasClicked == sendState.get())
    {
        //[UserButtonCode_sendState] -- add your button handler code here..
        getFilter()->setParameter (kSendHeldNotes, 1.f);
        //[/UserButtonCode_sendState]
    }
    else if (buttonThatWasClicked == showNumbersButton.get())
    {
        //[UserButtonCode_showNumbersButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost (kShowNumbers, buttonThatWasClicked->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_showNumbersButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void midiKeyboardEditor::mouseUp (const juce::MouseEvent& e)
{
    if (e.eventComponent == this)
    {
        if (e.mods.isPopupMenu())
            getFilter()->setParameter (kHidePanel, 0.f);
    }
    else if (e.eventComponent == aboutBox.get())
    {
        aboutBox->setVisible (false);
        useProgCh->setVisible (false);
        useCapsLock->setVisible (false);
        sendState->setVisible (false);
        showNumbersButton->setVisible (false);
    }
    else if (e.eventComponent == grabQwertyButton.get())
    {
        if (grabQwertyButton->getToggleState())
            getFilter()->setParameter (kQwertyAnywhere, 0.f);
        else if (e.mods.isRightButtonDown())
        {
            getFilter()->setParameter (kQwertyAnywhere, 1.f - getFilter()->getParameter (kQwertyAnywhere));
        }
        else
        {
            midiKeyboard->grabKeyboardFocus();
            midiKeyboard->repaint();
        }
    }
}

void midiKeyboardEditor::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (source == midiKeyboard.get())
    {
        getFilter()->keyPosition = midiKeyboard->getLowestVisibleKey();
        getFilter()->octave      = midiKeyboard->getKeyPressBaseOctave();
    }
    else if (source == getFilter())
        updateParametersFromFilter();
}

//==============================================================================
void midiKeyboardEditor::updateParametersFromFilter()
{
    PizKeyboard* const filter = getFilter();

    filter->getCallbackLock().enter();
    const int ch           = roundToInt (filter->getParameter (kChannel) * 15.f);
    const float keywidth   = filter->getParameter (kWidth);
    const float velocity   = filter->getParameter (kVelocity) * 127.f;
    const bool useY        = filter->getParameter (kUseY) >= 0.5f;
    const bool toggle      = filter->getParameter (kToggleInput) >= 0.5f;
    const bool hide        = filter->getParameter (kHidePanel) >= 0.5f;
    const bool qwerty      = filter->getParameter (kQwertyAnywhere) >= 0.5f;
    const bool capslock    = filter->getParameter (kCapsLock) >= 0.5f;
    const bool progch      = filter->getParameter (kUseProgCh) >= 0.5f;
    const bool showNumbers = filter->getParameter (kShowNumbers) >= 0.5f;
    const int w            = filter->lastUIWidth;
    const int h            = filter->lastUIHeight;
    const int keyPos       = filter->keyPosition;
    const int octave       = filter->octave;

    filter->getCallbackLock().exit();

    useProgCh->setToggleState (progch, juce::dontSendNotification);
    useCapsLock->setToggleState (capslock, juce::dontSendNotification);
    grabQwertyButton->setToggleState (qwerty, juce::dontSendNotification);
    showNumbersButton->setToggleState (showNumbers, juce::dontSendNotification);
    midiKeyboard->setDrawNoteNumber (showNumbers);
    midiKeyboard->setDrawQwerty (qwerty);
    if (qwerty)
        midiKeyboard->grabKeyboardFocus();
    midiKeyboard->setMidiChannelsToDisplay (1 << ch);
    midiKeyboard->setMidiChannel (ch + 1);
    chSlider->setValue (ch + 1, juce::dontSendNotification);
    yButton->setToggleState (useY, juce::dontSendNotification);
    velocitySlider->setValue (velocity);
    inputToggleButton->setToggleState (toggle, juce::dontSendNotification);
    setMouseClickGrabsKeyboardFocus (qwerty);
    midiKeyboard->setMouseClickGrabsKeyboardFocus (qwerty);
    midiKeyboard->toggle = toggle;
    midiKeyboard->setKeyPressBaseOctave (octave);
    midiKeyboard->setLowestVisibleKey (keyPos);
    midiKeyboard->setKeyWidth (jmax ((float) getWidth() / 76.f, keywidth * 150.0f + 5.0f));
    midiKeyboard->setScrollButtonsVisible (keywidth > 0.f);
    keyWidthSlider->setValue (keywidth * 100.f, juce::dontSendNotification);

    keyWidthSlider->setVisible (! hide);
    chSlider->setVisible (! hide);
    grabQwertyButton->setVisible (! hide);
    velocitySlider->setVisible (! hide);
    yButton->setVisible (! hide);
    inputToggleButton->setVisible (! hide);
    aboutButton->setVisible (! hide);
    hideButton->setVisible (! hide);

    if (hide)
    {
        midiKeyboard->setBounds (4, 4, w - 8, h - 8);
        resizer->toBehind (midiKeyboard.get());
    }
    else
    {
        midiKeyboard->setBounds (4, 16, w - 8, h - 20);
        resizer->toFront (false);
    }

    setSize (w, h);
}
//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="midiKeyboardEditor" componentName=""
                 parentClasses="public juce::AudioProcessorEditor, public juce::ChangeListener, public juce::KeyListener"
                 constructorParams="PizKeyboard* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="0" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="560" initialHeight="140">
  <BACKGROUND backgroundColour="ff8c8c8c">
    <TEXT pos="6 1 100 14" fill="solid: ff000000" hasStroke="0" text="Key Width"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="290 1 100 14" fill="solid: ff000000" hasStroke="0" text="Velocity"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
  </BACKGROUND>
  <SLIDER name="keyWidthSlider" id="2f97df245f379a32" memberName="keyWidthSlider"
          virtualName="" explicitFocusOrder="0" pos="4 1 100 14" tooltip="Key Width"
          bkgcol="5dffffff" thumbcol="5d000000" textboxtext="0" min="0.0"
          max="100.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="Channel" id="e888f858fbe02f3" memberName="chSlider" virtualName=""
          explicitFocusOrder="0" pos="119 1 60 14" tooltip="MIDI Channel"
          bkgcol="5dffffff" thumbcol="5dbbbbff" min="1.0" max="16.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="30" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TEXTBUTTON name="qwerty" id="4dfa64ca66fa41dc" memberName="grabQwertyButton"
              virtualName="" explicitFocusOrder="0" pos="189 1 87 14" tooltip="Attempt to grab keyboard focus; right-click to allow QWERTY input without keyboard focus"
              buttonText="Grab QWERTY" connectedEdges="0" needsCallback="0"
              radioGroupId="0"/>
  <SLIDER name="velocity" id="30f1aa1ebc94da9a" memberName="velocitySlider"
          virtualName="" explicitFocusOrder="0" pos="286 1 110 14" tooltip="Velocity"
          bkgcol="5dffffff" thumbcol="5d000000" textboxtext="ff000000"
          min="0.0" max="127.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="Use Y Button" id="c356057fe0e10768" memberName="yButton"
                virtualName="" explicitFocusOrder="0" pos="401 -1 32 18" tooltip="Use mouse Y-position for velocity"
                buttonText="Y" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="Input Toggle Button" id="81a3d3424ec5ae55" memberName="inputToggleButton"
                virtualName="" explicitFocusOrder="0" pos="435 -1 63 18" tooltip="Input notes as toggles"
                buttonText="Toggle" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TEXTBUTTON name="About" id="88a1761356ce446c" memberName="aboutButton" virtualName=""
              explicitFocusOrder="0" pos="516 1 40 14" tooltip="midiKeyboard instructions"
              buttonText="About" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Hide" id="84dad4e080d807d7" memberName="hideButton" virtualName=""
              explicitFocusOrder="0" pos="498 1 14 14" tooltip="Hide editing panel (Alt-click keyboard to show)"
              buttonText="_" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="57ecd2c813b0dcc1" memberName="resizer"
                    virtualName="" explicitFocusOrder="0" pos="16R 16R 16 16" class="ResizableCornerComponent"
                    params="this,&amp;resizeLimits"/>
  <GENERICCOMPONENT name="" id="8e79ee27297830eb" memberName="midiKeyboard" virtualName=""
                    explicitFocusOrder="0" pos="4 16 8M 20M" class="PizKeyboardComponent"
                    params="ownerFilter-&gt;editorKbState, MidiKeyboardComponent::horizontalKeyboard"/>
  <TEXTEDITOR name="Instructions" id="736835cb60c73832" memberName="aboutBox"
              virtualName="" explicitFocusOrder="0" pos="50%c 50%c 450 92"
              bkgcol="dce9e79b" initialText="Insert Piz Here-&gt; midiKeyboard 1.4&#10;- Right-click to hold note&#10;- Ctrl-click or Middle-click to end all notes&#10;- Shift-click to set QWERTY octave&#10;- Alt-click to show/hide editing panel&#10;- Default settings load from midiKeyboard.fxb"
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="0"
              caret="0" popupmenu="0"/>
  <TOGGLEBUTTON name="Use ProgCh Button" id="1365fbab3aebb877" memberName="useProgCh"
                virtualName="" explicitFocusOrder="0" pos="288 7 147 18" posRelativeX="736835cb60c73832"
                posRelativeY="736835cb60c73832" tooltip="Use Program Change"
                buttonText="Use Program Change" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="UseCapsLock Button" id="6c30f5345f3575b" memberName="useCapsLock"
                virtualName="" explicitFocusOrder="0" pos="288 26 147 18" posRelativeX="736835cb60c73832"
                posRelativeY="736835cb60c73832" tooltip="Respond to QWERTY only when Caps Lock is on"
                buttonText="Use Caps Lock" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <TEXTBUTTON name="send state" id="a9dbe441fdb14035" memberName="sendState"
              virtualName="" explicitFocusOrder="0" pos="291 65 140 21" posRelativeX="736835cb60c73832"
              posRelativeY="736835cb60c73832" tooltip="Send current keyboard state"
              buttonText="Send Current State" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TOGGLEBUTTON name="ShowNumbers Button" id="d5b687b5b62b72bc" memberName="showNumbersButton"
                virtualName="" explicitFocusOrder="0" pos="288 45 147 18" posRelativeX="736835cb60c73832"
                posRelativeY="736835cb60c73832" tooltip="Show MIDI note numbers on keys"
                buttonText="Show Note Numbers" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]
