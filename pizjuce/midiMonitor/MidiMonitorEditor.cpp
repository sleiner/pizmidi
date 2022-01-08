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

#include "MidiMonitorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MidiMonitorEditor::MidiMonitorEditor (MidiMonitorPlugin* const ownerFilter)
    : AudioProcessorEditor (ownerFilter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    clearButton.reset (new juce::TextButton ("clear"));
    addAndMakeVisible (clearButton.get());
    clearButton->addListener (this);

    clearButton->setBounds (8, 0, 40, 16);

    powerButton.reset (new juce::ToggleButton ("power"));
    addAndMakeVisible (powerButton.get());
    powerButton->setButtonText (TRANS("on"));
    powerButton->addListener (this);

    powerButton->setBounds (48, 0, 30, 16);

    thruButton.reset (new juce::ToggleButton ("thru"));
    addAndMakeVisible (thruButton.get());
    thruButton->addListener (this);

    thruButton->setBounds (80, 0, 40, 16);

    bytesButton.reset (new juce::ToggleButton ("bytes"));
    addAndMakeVisible (bytesButton.get());
    bytesButton->addListener (this);

    bytesButton->setBounds (118, 0, 40, 16);

    hueSlider.reset (new juce::Slider ("hue"));
    addAndMakeVisible (hueSlider.get());
    hueSlider->setTooltip (TRANS("Hue"));
    hueSlider->setRange (0, 1, 0.01);
    hueSlider->setSliderStyle (juce::Slider::LinearBar);
    hueSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    hueSlider->addListener (this);

    hueSlider->setBounds (192, -16, 40, 12);

    saturationSlider.reset (new juce::Slider ("saturation"));
    addAndMakeVisible (saturationSlider.get());
    saturationSlider->setTooltip (TRANS("Saturation"));
    saturationSlider->setRange (0, 1, 0.01);
    saturationSlider->setSliderStyle (juce::Slider::LinearBar);
    saturationSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    saturationSlider->addListener (this);

    saturationSlider->setBounds (240, -16, 40, 12);

    lightnessSlider.reset (new juce::Slider ("lightness"));
    addAndMakeVisible (lightnessSlider.get());
    lightnessSlider->setTooltip (TRANS("Lightness"));
    lightnessSlider->setRange (0, 1, 0.01);
    lightnessSlider->setSliderStyle (juce::Slider::LinearBar);
    lightnessSlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    lightnessSlider->addListener (this);

    lightnessSlider->setBounds (288, -16, 40, 12);

    resizer.reset (new ResizableCornerComponent (this, &resizeLimits));
    addAndMakeVisible (resizer.get());

    saveButton.reset (new juce::TextButton ("save"));
    addAndMakeVisible (saveButton.get());
    saveButton->addListener (this);

    saveButton->setBounds (344, 0, 40, 16);

    menuButton.reset (new juce::TextButton ("menu"));
    addAndMakeVisible (menuButton.get());
    menuButton->addListener (this);

    menuButton->setBounds (296, 0, 40, 16);

    midiOutputEditor.reset (new juce::TextEditor ("monitorbox"));
    addAndMakeVisible (midiOutputEditor.get());
    midiOutputEditor->setMultiLine (true);
    midiOutputEditor->setReturnKeyStartsNewLine (false);
    midiOutputEditor->setReadOnly (true);
    midiOutputEditor->setScrollbarsShown (true);
    midiOutputEditor->setCaretVisible (false);
    midiOutputEditor->setPopupMenuEnabled (true);
    midiOutputEditor->setText (juce::String());

    wrapButton.reset (new juce::ToggleButton ("wrap"));
    addAndMakeVisible (wrapButton.get());
    wrapButton->addListener (this);

    wrapButton->setBounds (200, 0, 41, 16);

    timeButton.reset (new juce::ToggleButton ("time"));
    addAndMakeVisible (timeButton.get());
    timeButton->addListener (this);

    timeButton->setBounds (160, 0, 40, 16);

    colorButton.reset (new juce::TextButton ("color"));
    addAndMakeVisible (colorButton.get());
    colorButton->addListener (this);

    colorButton->setBounds (248, 0, 40, 16);

    aboutBox.reset (new juce::TextEditor ("AboutBox"));
    addAndMakeVisible (aboutBox.get());
    aboutBox->setMultiLine (true);
    aboutBox->setReturnKeyStartsNewLine (false);
    aboutBox->setReadOnly (true);
    aboutBox->setScrollbarsShown (true);
    aboutBox->setCaretVisible (false);
    aboutBox->setPopupMenuEnabled (false);
    aboutBox->setText (TRANS("Insert Piz Here midiMonitor 1.3\n"
    "\n"
    "You can save a \"midiMonitor.fxp\" with your default settings.\n"
    "\n"
    "Controls:\n"
    "\"clear\" - Clears the display and recorded MIDI buffer.\n"
    "\n"
    "\"on\" - The plugin is on when this is checked.\n"
    "\"thru\" - When checked, input events are passed to the output.\n"
    "\"bytes\" - Displays raw MIDI data in hexadecimal format.\n"
    "\"time\" - Displays time of MIDI events.\n"
    "\"wrap\" - Toggles word wrapping for the monitor box.\n"
    "\n"
    "\"color\" - Set background colors by Hue, Saturation, Lightness.\n"
    "\n"
    "\"menu\" - Options for: \n"
    "     - Time format\n"
    "     - Maximum number of lines to display\n"
    "     - MIDI event types to display\n"
    "\n"
    "\"save\" - Save monitor log as MIDI file or plain text. Specify .mid or .txt extension, or leave blank to save both. MIDI file contains all events since \"clear\" button was pressed. Text file contains the current contents of the monitor display."));


    //[UserPreSize]
	aboutBox->setVisible(false);
	aboutBox->addMouseListener(this,false);

    addAndMakeVisible (maxLinesEditor = new Slider (L"new slider"));
    maxLinesEditor->setRange (1, 500000, 1);
    maxLinesEditor->setSliderStyle (Slider::LinearHorizontal);
    maxLinesEditor->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    maxLinesEditor->addListener (this);
    maxLinesEditor->setSkewFactor (0.2);

    setMouseClickGrabsKeyboardFocus (false);
    clearButton->setMouseClickGrabsKeyboardFocus (false);
    powerButton->setMouseClickGrabsKeyboardFocus (false);
    thruButton->setMouseClickGrabsKeyboardFocus (false);
    bytesButton->setMouseClickGrabsKeyboardFocus (false);
    wrapButton->setMouseClickGrabsKeyboardFocus (false);
    saveButton->setMouseClickGrabsKeyboardFocus (false);
    menuButton->setMouseClickGrabsKeyboardFocus (false);
    hueSlider->setMouseClickGrabsKeyboardFocus (false);
    saturationSlider->setMouseClickGrabsKeyboardFocus (false);
    lightnessSlider->setMouseClickGrabsKeyboardFocus (false);
    midiOutputEditor->setMouseClickGrabsKeyboardFocus (false);
    resizer->setMouseClickGrabsKeyboardFocus (false);

    resizeLimits.setSizeLimits (50, 50, 640, 800);
    midiOutputEditor->setScrollBarThickness(12);
    startTimer (1000 / 10); // 10fps
	numLines=0;

	bgcolor=Colour(getFilter()->getParameter(0),getFilter()->getParameter(1),getFilter()->getParameter(2),1.0f);
    fgcolor=Colour(bgcolor.contrasting(getFilter()->getParameter(3)));
#if 0
    //[/UserPreSize]

    setSize (392, 400);


    //[Constructor] You can add your own custom stuff here..
#endif
    ownerFilter->addChangeListener (this);
    ownerFilter->dispatchPendingMessages();
	updateParametersFromFilter();
    //[/Constructor]
}

MidiMonitorEditor::~MidiMonitorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    stopTimer ();
    getFilter()->dispatchPendingMessages();
    getFilter()->removeChangeListener (this);
    //[/Destructor_pre]

    clearButton = nullptr;
    powerButton = nullptr;
    thruButton = nullptr;
    bytesButton = nullptr;
    hueSlider = nullptr;
    saturationSlider = nullptr;
    lightnessSlider = nullptr;
    resizer = nullptr;
    saveButton = nullptr;
    menuButton = nullptr;
    midiOutputEditor = nullptr;
    wrapButton = nullptr;
    timeButton = nullptr;
    colorButton = nullptr;
    aboutBox = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	deleteAndZero (maxLinesEditor);
    //[/Destructor]
}

//==============================================================================
void MidiMonitorEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
	g.fillAll (Colour (bgcolor.contrasting(0.2f*(getFilter()->getParameter(3)))));
#if 0
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffa1a1a1));

    //[UserPaint] Add your own custom painting code here..
#endif
    //[/UserPaint]
}

void MidiMonitorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
    midiOutputEditor->setBounds (8, 16, getWidth() - 16, getHeight() - 24);
    aboutBox->setBounds (8, 16, getWidth() - 16, getHeight() - 24);
    //[UserResized] Add your own custom resize handling here..
    getFilter()->lastUIWidth = getWidth();
    getFilter()->lastUIHeight = getHeight();
    //[/UserResized]
}

void MidiMonitorEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == clearButton.get())
    {
        //[UserButtonCode_clearButton] -- add your button handler code here..
        midiOutputEditor->clear ();
        getFilter()->clearLoop();
		numLines = 0;
        //[/UserButtonCode_clearButton]
    }
    else if (buttonThatWasClicked == powerButton.get())
    {
        //[UserButtonCode_powerButton] -- add your button handler code here..
        if (getFilter()->getParameter(4)>=0.5f)
            getFilter()->setParameterNotifyingHost(4,0.0f);
        else
            getFilter()->setParameterNotifyingHost(4,1.0f);
        //[/UserButtonCode_powerButton]
    }
    else if (buttonThatWasClicked == thruButton.get())
    {
        //[UserButtonCode_thruButton] -- add your button handler code here..
        if (getFilter()->getParameter(5)>=0.5f)
            getFilter()->setParameterNotifyingHost(5,0.0f);
        else
            getFilter()->setParameterNotifyingHost(5,1.0f);
        //[/UserButtonCode_thruButton]
    }
    else if (buttonThatWasClicked == bytesButton.get())
    {
        //[UserButtonCode_bytesButton] -- add your button handler code here..
        if (getFilter()->getParameter(6)>=0.5f)
            getFilter()->setParameterNotifyingHost(6,0.0f);
        else
            getFilter()->setParameterNotifyingHost(6,1.0f);
        //[/UserButtonCode_bytesButton]
    }
    else if (buttonThatWasClicked == saveButton.get())
    {
        //[UserButtonCode_saveButton] -- add your button handler code here..
        FileChooser myChooser ("Save MIDI file/log...",File("midiMonitorLog"),"*.txt; *.mid");

        if (myChooser.browseForFileToSave(true))
        {
            File logFile (myChooser.getResult());
            if (logFile.getFileExtension().isEmpty()) {
                getFilter()->writeMidiFile(logFile.withFileExtension("mid"));
                logFile.withFileExtension("txt").replaceWithText(midiOutputEditor->getText());
            }
            else if (logFile.hasFileExtension("mid")) {
                getFilter()->writeMidiFile(logFile);
            }
            else logFile.replaceWithText(midiOutputEditor->getText());
        }
        //[/UserButtonCode_saveButton]
    }
    else if (buttonThatWasClicked == menuButton.get())
    {
        //[UserButtonCode_menuButton] -- add your button handler code here..
        PopupMenu m, sub2, sub3;
		m.addSectionHeader("Time Mode:");
		m.addItem (101,"Bars|Beats|Ticks",true,timemode==0);
		float ppqn = getFilter()->getParameter(kTicks);
        sub2.addItem(30,L"hide ticks",true,ppqn==1.0);
        sub2.addItem(31,L"4",true,ppqn<1.0 && ppqn>=0.9);
        sub2.addItem(32,L"16",true,ppqn<0.9 && ppqn>=0.8);
        sub2.addItem(33,L"48",true,ppqn<0.8 && ppqn>=0.7);
        sub2.addItem(34,L"96",true,ppqn<0.7 && ppqn>=0.65);
        sub2.addItem(35,L"100",true,ppqn<0.65 && ppqn>=0.6);
        sub2.addItem(36,L"120",true,ppqn<0.6 && ppqn>=0.5);
        sub2.addItem(37,L"192",true,ppqn<0.5 && ppqn>=0.4);
        sub2.addItem(38,L"240",true,ppqn<0.4 && ppqn>=0.3);
        sub2.addItem(39,L"384",true,ppqn<0.3 && ppqn>=0.2);
        sub2.addItem(40,L"480",true,ppqn<0.2 && ppqn>=0.1);
        sub2.addItem(41,L"768",true,ppqn<0.1 && ppqn>0.0);
        sub2.addItem(42,L"960",true,ppqn==0.0);
        m.addSubMenu (L"Ticks per beat", sub2, timemode==0);
		m.addItem (102,"SMPTE",true,timemode==1);
        float fps = getFilter()->getParameter(kFrames);
        sub3.addItem(50,L"ms (1000)",true,fps==0.0);
        sub3.addItem(51,L"120",true,fps==1.0);
        sub3.addItem(52,L"100",true,fps<1.0 && fps>=0.9);
        sub3.addItem(53,L"75",true,fps<0.9 && fps>=0.8);
        sub3.addItem(54,L"60",true,fps<0.8 && fps>=0.7);
        sub3.addItem(55,L"50",true,fps<0.7 && fps>=0.6);
        sub3.addItem(56,L"30",true,fps<0.6 && fps>=0.5);
        sub3.addItem(61,L"29.97 drop",true,fps<0.5 && fps>=0.4);
        sub3.addItem(57,L"25",true,fps<0.4 && fps>=0.3);
        sub3.addItem(58,L"24",true,fps<0.3 && fps>=0.2);
        sub3.addItem(59,L"15",true,fps<0.2 && fps>=0.1);
        sub3.addItem(60,L"10",true,fps<0.1 && fps>0.0);
        m.addSubMenu (L"Frames per second", sub3, timemode==1);
		m.addItem (103,"Sample Delta",true,timemode==2);
        m.addSeparator();

		m.addSectionHeader("Max Lines:");
		m.addCustomItem(-1,*maxLinesEditor,200,16,false);
        m.addSeparator();

		m.addSectionHeader("Filter Events:");
        m.addItem (1, String("Notes"), true, getFilter()->useNotes);
        m.addItem (2, String("CCs"), true, getFilter()->useCC);
        m.addItem (3, String("Pitch Wheel"), true, getFilter()->usePB);
        m.addItem (4, String("Program Change"), true, getFilter()->usePC);
        m.addItem (5, String("Poly Aftertouch"), true, getFilter()->usePA);
        m.addItem (6, String("Channel Pressure"), true, getFilter()->useCP);
        m.addItem (7, String("Clock"), true, getFilter()->useClock);
        m.addItem (8, String("SysEx"), true, getFilter()->useSysEx);
        m.addItem (9, String("Other"), true, getFilter()->useOther);
		m.addSeparator();
		m.addItem (9999,"About...");

        int result = m.showAt(menuButton.get());
        switch (result) {
            case 1:
                getFilter()->useNotes = !getFilter()->useNotes;
                break;
            case 2:
                getFilter()->useCC = !getFilter()->useCC;
                break;
            case 3:
                getFilter()->usePB = !getFilter()->usePB;
                break;
            case 4:
                getFilter()->usePC = !getFilter()->usePC;
                break;
            case 5:
                getFilter()->usePA = !getFilter()->usePA;
                break;
            case 6:
                getFilter()->useCP = !getFilter()->useCP;
                break;
            case 7:
                getFilter()->useClock = !getFilter()->useClock;
                break;
            case 8:
                getFilter()->useSysEx = !getFilter()->useSysEx;
                break;
            case 9:
                getFilter()->useOther = !getFilter()->useOther;
                break;

			case 30: getFilter()->setParameter(kTicks,1.0f);   break;
			case 31: getFilter()->setParameter(kTicks,0.91f);  break;
			case 32: getFilter()->setParameter(kTicks,0.81f);  break;
			case 33: getFilter()->setParameter(kTicks,0.71f);  break;
			case 34: getFilter()->setParameter(kTicks,0.65f);  break;
			case 35: getFilter()->setParameter(kTicks,0.61f);  break;
			case 36: getFilter()->setParameter(kTicks,0.51f);  break;
			case 37: getFilter()->setParameter(kTicks,0.41f);  break;
			case 38: getFilter()->setParameter(kTicks,0.31f);  break;
			case 39: getFilter()->setParameter(kTicks,0.21f);  break;
			case 40: getFilter()->setParameter(kTicks,0.11f);  break;
			case 41: getFilter()->setParameter(kTicks,0.051f); break;
			case 42: getFilter()->setParameter(kTicks,0.0f);   break;

            case 50: getFilter()->setParameter(kFrames,0.0f);  break;
            case 51: getFilter()->setParameter(kFrames,1.0f);  break;
            case 52: getFilter()->setParameter(kFrames,0.9f);  break;
            case 53: getFilter()->setParameter(kFrames,0.8f);  break;
            case 54: getFilter()->setParameter(kFrames,0.7f);  break;
            case 55: getFilter()->setParameter(kFrames,0.6f);  break;
            case 56: getFilter()->setParameter(kFrames,0.5f);  break;
            case 61: getFilter()->setParameter(kFrames,0.4f);  break;
            case 57: getFilter()->setParameter(kFrames,0.3f);  break;
            case 58: getFilter()->setParameter(kFrames,0.2f);  break;
            case 59: getFilter()->setParameter(kFrames,0.1f);  break;
			case 60: getFilter()->setParameter(kFrames,0.05f); break;


			case 101: getFilter()->setParameter(kTimeMode,0.0f);   break;
			case 102: getFilter()->setParameter(kTimeMode,0.5f);   break;
			case 103: getFilter()->setParameter(kTimeMode,1.0f);   break;

			case 9999: aboutBox->setVisible(true);
            default: break;
        }
        //[/UserButtonCode_menuButton]
    }
    else if (buttonThatWasClicked == wrapButton.get())
    {
        //[UserButtonCode_wrapButton] -- add your button handler code here..
        if (getFilter()->getParameter(7)>=0.5f)
            getFilter()->setParameterNotifyingHost(7,0.0f);
        else
            getFilter()->setParameterNotifyingHost(7,1.0f);
        //[/UserButtonCode_wrapButton]
    }
    else if (buttonThatWasClicked == timeButton.get())
    {
        //[UserButtonCode_timeButton] -- add your button handler code here..
        if (getFilter()->getParameter(kTime)>=0.5f)
            getFilter()->setParameterNotifyingHost(kTime,0.0f);
        else
            getFilter()->setParameterNotifyingHost(kTime,1.0f);
        //[/UserButtonCode_timeButton]
    }
    else if (buttonThatWasClicked == colorButton.get())
    {
        //[UserButtonCode_colorButton] -- add your button handler code here..
        PopupMenu m;
		m.addCustomItem(-1,*hueSlider,100,16,false);
		m.addCustomItem(-1,*saturationSlider,100,16,false);
		m.addCustomItem(-1,*lightnessSlider,100,16,false);
        //int result =
			m.showAt(colorButton.get());
		//switch (result) {
		//	default: break;
		//}
        //[/UserButtonCode_colorButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void MidiMonitorEditor::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == hueSlider.get())
    {
        //[UserSliderCode_hueSlider] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(0,(float)hueSlider->getValue());
        repaint();
        //[/UserSliderCode_hueSlider]
    }
    else if (sliderThatWasMoved == saturationSlider.get())
    {
        //[UserSliderCode_saturationSlider] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(1,(float)saturationSlider->getValue());
        repaint();
        //[/UserSliderCode_saturationSlider]
    }
    else if (sliderThatWasMoved == lightnessSlider.get())
    {
        //[UserSliderCode_lightnessSlider] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(2,(float)lightnessSlider->getValue());
        repaint();
        //[/UserSliderCode_lightnessSlider]
    }

    //[UsersliderValueChanged_Post]
    else if (sliderThatWasMoved == maxLinesEditor)
    {
        //[UserSliderCode_hueSlider] -- add your slider handling code here..
        getFilter()->setMaxLines((int)maxLinesEditor->getValue());
        //[/UserSliderCode_hueSlider]
    }
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MidiMonitorEditor::mouseDown(const MouseEvent &e)
{
	if (e.eventComponent == aboutBox.get())
	{
		aboutBox->setVisible(false);
	}
}
//==============================================================================
void MidiMonitorEditor::timerCallback ()
{
    int hours, minutes, seconds, frames;
    MidiMessage::SmpteTimecodeType timeCode;
	bool showtime = getFilter()->getParameter(kTime)>=0.5f;

	getFilter()->getCallbackLock().enter();
	MidiMessageSequence messages(*getFilter()->getMessages());
	getFilter()->getMessages()->clear();
	getFilter()->getCallbackLock().exit();

	for (int i=0;i<messages.getNumEvents();i++)
	{
        String midiLine = String();
		MidiMessage msg = messages.getEventPointer(i)->message;
		{
            bool ignore=true;
            if (msg.isNoteOnOrOff () && getFilter()->useNotes)
            {
                ignore=false;
                if (msg.getChannel()>0) midiLine = "Ch." + String(msg.getChannel()) + "  ";
                midiLine += "Note ";
                if (msg.isNoteOn())
                {
                    midiLine += "On: ";
                }
                else
                {
                    midiLine += "Off: ";
                }
                midiLine += MidiMessage::getMidiNoteName(msg.getNoteNumber(),true,true,3)
                    + " (" + String(msg.getNoteNumber())  + "), Velocity: "
                    + String(msg.getVelocity());
            }
            else if (msg.isController() && getFilter()->useCC) {
                ignore=false;
                if (msg.getChannel()>0) midiLine = "Ch." + String(msg.getChannel()) + "  ";
                midiLine += "CC #" + String(msg.getControllerNumber());
                auto controllerDescription = msg.getControllerName(msg.getControllerNumber());
                if (controllerDescription != nullptr)
                {
                    midiLine += " (" + String(controllerDescription) + "),";
                }
                midiLine += " Value: " + String(msg.getControllerValue());
            }
            else if (msg.isPitchWheel() && getFilter()->usePB) {
                ignore=false;
                if (msg.getChannel()>0) midiLine = "Ch." + String(msg.getChannel()) + "  ";
                midiLine += "Pitch Wheel: " + String(msg.getPitchWheelValue());
                if (msg.getPitchWheelValue()==0x2000)
                    midiLine += " (Center)";
                else if (msg.getPitchWheelValue()>0x2000)
                    midiLine += " (+" + String(msg.getPitchWheelValue()-0x2000) + ")";
                else
                    midiLine += " (" + String(msg.getPitchWheelValue()-0x2000) + ")";
            }
            else if (msg.isAftertouch() && getFilter()->usePA) {
                ignore=false;
                if (msg.getChannel()>0) midiLine = "Ch." + String(msg.getChannel()) + "  ";
                midiLine += "Poly Aftertouch: "
                    + MidiMessage::getMidiNoteName(msg.getNoteNumber(),true,true,3)
                    + " (" + String(msg.getNoteNumber()) + "), Value: "
                    + String(msg.getAfterTouchValue());
            }
            else if (msg.isChannelPressure() && getFilter()->useCP) {
                ignore=false;
                if (msg.getChannel()>0) midiLine = "Ch." + String(msg.getChannel()) + "  ";
                midiLine += "Channel Pressure: " + String(msg.getChannelPressureValue());
            }
            else if (msg.isProgramChange() && getFilter()->usePC) {
                ignore=false;
                if (msg.getChannel()>0) midiLine = "Ch." + String(msg.getChannel()) + "  ";
                midiLine += "Program Change: " + String(msg.getProgramChangeNumber()+1);
            }
            else if (msg.isSysEx() && getFilter()->useSysEx)
            {
                ignore=false;
                midiLine = "Sysex: ";
                midiLine += String (msg.getSysExDataSize());
                midiLine += " bytes";
            }
            else if (msg.isTimeSignatureMetaEvent () && getFilter()->useOther)
            {
                ignore=false;
                int newNumerator, newDenominator;
                msg.getTimeSignatureInfo (newNumerator, newDenominator);

                midiLine = "Time Signature: ";
                midiLine += String (newNumerator);
                midiLine += " / ";
                midiLine += String (newDenominator);
            }
            else if (msg.isTempoMetaEvent () && getFilter()->useOther)
            {
                ignore=false;
                midiLine = "Tempo: ";
                midiLine += String (msg.getTempoSecondsPerQuarterNote ());
                //midiLine += " ";
                //midiLine += String (msg.getTempoMetaEventTickLength (ticksPerQuarterNote));
            }
            else if (msg.isMidiMachineControlMessage() && getFilter()->useOther)
            {
                ignore=false;
                midiLine = "MMC: ";

                switch (msg.getMidiMachineControlCommand())
                {
                case MidiMessage::mmc_stop:             midiLine += "stop"; break;
                case MidiMessage::mmc_play:             midiLine += "play"; break;
                case MidiMessage::mmc_deferredplay:     midiLine += "deferredplay"; break;
                case MidiMessage::mmc_fastforward:      midiLine += "fastforward"; break;
                case MidiMessage::mmc_rewind:           midiLine += "rewind"; break;
                case MidiMessage::mmc_recordStart:      midiLine += "recordStart"; break;
                case MidiMessage::mmc_recordStop:       midiLine += "recordStop"; break;
                case MidiMessage::mmc_pause:            midiLine += "pause"; break;
                }
            }
            else if (msg.isMidiStart () && getFilter()->useOther)
            {
                ignore=false;
                midiLine = "MIDI Start";
            }
            else if (msg.isMidiContinue () && getFilter()->useOther)
            {
                ignore=false;
                midiLine = "MIDI Continue";
            }
            else if (msg.isMidiStop () && getFilter()->useOther)
            {
                ignore=false;
                midiLine = "MIDI Stop";
            }
            else if (msg.isSongPositionPointer () && getFilter()->useOther)
            {
                ignore=false;
                midiLine = "Song Position: ";
                midiLine += String (msg.getSongPositionPointerMidiBeat ());
            }
            else if (msg.isQuarterFrame () && getFilter()->useOther)
            {
                ignore=false;
                midiLine = "MTC Quarter Frame: ";
                midiLine += String (msg.getQuarterFrameSequenceNumber ());
                midiLine += " ";
                midiLine += String (msg.getQuarterFrameValue ());
            }
            else if (msg.isFullFrame () && getFilter()->useOther)
            {
                ignore=false;
                midiLine = "MTC Full Frame: ";

                msg.getFullFrameParameters (hours, minutes, seconds, frames, timeCode);

                midiLine += String (hours);
                midiLine += ":";
                midiLine += String (minutes);
                midiLine += ":";
                midiLine += String (seconds);
                midiLine += ":";
                midiLine += String (frames);

                midiLine += " timecode: ";
                switch (timeCode) {
                    case MidiMessage::fps24:      midiLine += "fps24"; break;
                    case MidiMessage::fps25:      midiLine += "fps25"; break;
                    case MidiMessage::fps30drop:  midiLine += "fps30drop"; break;
                    case MidiMessage::fps30:      midiLine += "fps30"; break;
                }
            }
            else if (msg.isMidiMachineControlGoto (hours, minutes, seconds, frames) && getFilter()->useOther)
            {
                ignore=false;
                midiLine = "MMC GOTO: ";
                midiLine += String (hours);
                midiLine += ":";
                midiLine += String (minutes);
                midiLine += ":";
                midiLine += String (seconds);
                midiLine += ":";
                midiLine += String (frames);
            }
            else if (msg.isMidiClock() && getFilter()->useClock)
            {
                ignore=false;
                midiLine = "MIDI Clock";
            }
            else if (msg.isActiveSense() && getFilter()->useOther)
            {
                ignore=false;
                midiLine = "Active Sense";
            }
            else if (msg.getRawData()[0]==0xff && getFilter()->useOther)
            {
                ignore=false;
                midiLine = "MIDI Reset";
            }
            else if (msg.getRawData()[0]==0xf9 && getFilter()->useOther)
            {
                ignore=false;
                midiLine = "MIDI Tick";
            }
			else if (getFilter()->useOther)
			{
				ignore=false;
				midiLine += "??? (" + String(msg.getRawDataSize()) + " bytes)";
			}

            if (!ignore) {
				while (numLines>=maxLines)
				{
					//int end = midiOutputEditor->getText().upToFirstOccurrenceOf("\n",true,true).length();
					//const Range<int> firstLine(0,end);
					midiOutputEditor->setHighlightedRegion(Range<int>(0,midiOutputEditor->getText().upToFirstOccurrenceOf("\n",true,true).length()));
					midiOutputEditor->setReadOnly(false);
					midiOutputEditor->cut();
					midiOutputEditor->setReadOnly(true);
					--numLines;
				}
				++numLines;

                midiOutputEditor->setCaretPosition(midiOutputEditor->getText().length());

                if (showbytes) {
                    Font defaultFont = midiOutputEditor->getFont();
					String bytesString = numLines>1 ? "\n" : String();
					bytesString += String::toHexString(msg.getRawData(),msg.getRawDataSize()).toUpperCase() + "  ";
                    midiOutputEditor->setFont(Font("Courier New",midiOutputEditor->getFont().getHeight(),Font::plain));
                    midiOutputEditor->insertTextAtCaret (bytesString);
                    midiOutputEditor->setFont(defaultFont);
                }
				if (showtime) {
					String timeString = (numLines>1 && !showbytes) ? "\n" : String();
					timeString += ppqToString(msg.getTimeStamp(),getFilter()->getTimeSigNumerator(),getFilter()->getTimeSigDenominator(),getFilter()->lastPosInfo.bpm) + "  ";
					Font defaultFont = midiOutputEditor->getFont();
					midiOutputEditor->setFont(Font("Courier New",midiOutputEditor->getFont().getHeight(),Font::bold));
					midiOutputEditor->insertTextAtCaret (timeString);
					midiOutputEditor->setFont(defaultFont);
                }
				if (!showtime && !showbytes && numLines>1)
					midiLine = "\n" + midiLine;
                midiOutputEditor->insertTextAtCaret (midiLine);
            }
        }
    }
}

//==============================================================================
void MidiMonitorEditor::changeListenerCallback (ChangeBroadcaster* source)
{
    updateParametersFromFilter();
}

//==============================================================================
void MidiMonitorEditor::updateParametersFromFilter()
{
    MidiMonitorPlugin* const filter = getFilter();

    filter->getCallbackLock().enter();

    // take a local copy of the info we need while we've got the lock..
    const float hue = filter->getParameter(0);
    const float sat = filter->getParameter(1);
    const float bri = filter->getParameter(2);
    const float contrast = filter->getParameter(3);
    const float power = filter->getParameter(4);
    const float thru = filter->getParameter(5);
    const float wrap = filter->getParameter(kWrap);
    showbytes = filter->getParameter(kBytes)>=0.5f;
    showtime = filter->getParameter(kTime)>=0.5f;
	const float ftmode = filter->getParameter(kTimeMode);
	maxLines = filter->getMaxLines();

    // ..release the lock ASAP
    filter->getCallbackLock().exit();

	if (ftmode==0.0) timemode=0;
	else if (ftmode<1.f) timemode=1;
	else timemode=2;

    //set sliders
    hueSlider->setValue(hue,sendNotification);
    saturationSlider->setValue(sat,sendNotification);
    lightnessSlider->setValue(bri,sendNotification);
    //slider4->setValue(contrast,sendNotification);
	maxLinesEditor->setValue(maxLines,dontSendNotification);

    //set button states
    if (power>=0.5f) {
        powerButton->setButtonText ("on");
        powerButton->setToggleState (true, false);
    }
    else {
        powerButton->setButtonText ("off");
        powerButton->setToggleState (false, false);
    }
    thruButton->setToggleState (thru>=0.5f, false);
    wrapButton->setToggleState (wrap>=0.5f, false);
    bytesButton->setToggleState (showbytes, false);
    timeButton->setToggleState (showtime, false);

    setSize (filter->lastUIWidth, filter->lastUIHeight);

    //set colors
    bgcolor=Colour(hue,sat,bri,1.0f);
    fgcolor=Colour(bgcolor.contrasting(contrast));

    clearButton->setColour (TextButton::textColourOnId, fgcolor);
    clearButton->setColour (TextButton::textColourOffId, fgcolor);
    clearButton->setColour (TextButton::buttonColourId, bgcolor);
    saveButton->setColour (TextButton::textColourOnId, fgcolor);
    saveButton->setColour (TextButton::textColourOffId, fgcolor);
    saveButton->setColour (TextButton::buttonColourId, bgcolor);
    menuButton->setColour (TextButton::textColourOnId, fgcolor);
    menuButton->setColour (TextButton::textColourOffId, fgcolor);
    menuButton->setColour (TextButton::buttonColourId, bgcolor);
    colorButton->setColour (TextButton::textColourOnId, fgcolor);
    colorButton->setColour (TextButton::textColourOffId, fgcolor);
    colorButton->setColour (TextButton::buttonColourId, bgcolor);

    midiOutputEditor->setColour (TextEditor::backgroundColourId, bgcolor);
	midiOutputEditor->setColour (TextEditor::textColourId, fgcolor);
    midiOutputEditor->setMultiLine (true, wrap>=0.5f);
    repaint();
}

const String MidiMonitorEditor::ppqToString (const double sppq,
                                          const int numerator,
                                          const int denominator,
                                          const double bpm)
{
    const wchar_t* const sign = (sppq < 0) ? L"-" : L"";
    const long double absSecs = fabs (sppq*60.0/bpm);

	if (timemode==2)
	{
		//buffer #/sample delta
		return "b."+ String((int)sppq) + " s." + String(roundToInt(fmod(sppq,1.0)*1000000)).paddedLeft('0',4);
	}
	else if (timemode==3) {
		//total samples
		const uint64 samples = roundDoubleToInt(getFilter()->getSampleRate()*absSecs);
		return sign+String(samples);
	}
	else if (timemode==0)
	{
		//bars/beats/ticks
		const double ppq = fabs(sppq);
		const double ppqPerBar = ((double)numerator * 4.0 / (double)denominator);
		const double beats  = (fmod (ppq, ppqPerBar) / ppqPerBar) * numerator;
		const float t = getFilter()->getParameter(kTicks);

		float tpb;
		if      (t==0.0) tpb=960.0;
		else if (t<0.1) tpb=768.0;
		else if (t<0.2) tpb=480.0;
		else if (t<0.3) tpb=384.0;
		else if (t<0.4) tpb=240.0;
		else if (t<0.5) tpb=192.0;
		else if (t<0.6) tpb=120.0;
		else if (t<0.65) tpb=100.0;
		else if (t<0.7) tpb=96.0;
		else if (t<0.8) tpb=48.0;
		else if (t<0.9) tpb=16.0;
		else if (t<1.0) tpb=4.0;
		else tpb=0.0;

		int bar = (int) (ppq / ppqPerBar) + 1;
		int beat = ((int) beats) + 1;
		int ticks = tpb==960.0 ? roundToInt(fmod (beats, 1.0) * tpb) : (int)(fmod (beats, 1.0) * tpb);

		if (ticks==(int)tpb)
		{
			ticks=0;
			++beat;
			if (beat==(int)ppqPerBar)
			{
				beat=0;
				++bar;
			}
		}

		if(sppq<0) {
			bar-=1;
			beat=numerator-beat+1;
			ticks=(int)tpb-ticks-1;
		}

		String padding = String();
		if      (ticks<10 && tpb>100.0) padding = L"00";
		else if (ticks<100 && tpb>100.0) padding = L"0";
		else if (ticks<10 && tpb>=10.0) padding = L"0";

		String s;
		if (tpb>0.0)
			return (bar==0?L"":sign) + String(bar) + "|" + String(beat) + "|" + padding + String(ticks);
		else
			return (bar==0?L"":sign) + String(bar) + "|" + String(beat);
	}
    else {
		//SMPTE
		const bool showms = getFilter()->getParameter(kShowms)>=0.5f;
		const bool showhrs = getFilter()->getParameter(kShowhrs)>=0.5f;
		const bool showsubfr = getFilter()->getParameter(kShowsubfr)>=0.5f;
		const float f = getFilter()->getParameter(kFrames);

		bool dropframe = false;
		float fps;
		if      (f==0.0) fps=1000.0;
		else if (f<0.1) fps=10.0;
		else if (f<0.2) fps=15.0;
		else if (f<0.3) fps=24.0;
		else if (f<0.4) fps=25.0;
		else if (f<0.5) {
			fps=29.97f;
			dropframe=true;
		}
		else if (f<0.6) fps=30.0;
		else if (f<0.7) fps=50.0;
		else if (f<0.8) fps=60.0;
		else if (f<0.9) fps=75.0;
		else if (f<1.0) fps=100.0;
		else fps=120.0;

		int hours;
		int mins;

		if (showhrs) {
			hours = (int) (absSecs / (60.0 * 60.0));
			mins  = ((int) (absSecs / 60.0)) % 60;
		}
		else mins = (int) (absSecs / 60.0);
		const int secs  = ((int) absSecs) % 60;


		String s1;
		if (showhrs) {
			if (showms) {
				if (fps==1000.0)
					s1 = String::formatted (L"%s%02d:%02d:%02d.%03d",
											  sign, hours, mins, secs,
											  int64 (absSecs * 1000) % 1000);
				else if (fps<=10.0)
					s1 = String::formatted (L"%s%02d:%02d:%02d:%.1d",
											  sign, hours, mins, secs,
											  int64(absSecs*fps) % (int)fps);
				else if (fps<=100.0) {
					if (dropframe) {
						int64 frameNumber = int64(absSecs*29.97);
						frameNumber +=  18*(frameNumber/17982) + 2*(((frameNumber%17982) - 2) / 1798);
						int frames   = int(frameNumber % 30);
						int dseconds = int((frameNumber / 30) % 60);
						int dminutes = int(((frameNumber / 30) / 60) % 60);
						int dhours   = int((((frameNumber / 30) / 60) / 60) % 24);

						s1 = String::formatted (L"%s%02d;%02d;%02d;%.2d",
												  sign, dhours, dminutes, dseconds,
												  frames);
					}
					else
						s1 = String::formatted (L"%s%02d:%02d:%02d:%.2d",
												  sign, hours, mins, secs,
												  int64(absSecs*fps) % (int)fps);
				}
				else
					s1 = String::formatted (L"%s%02d:%02d:%02d:%03d",
											  sign, hours, mins, secs,
											  int64(absSecs*fps) % (int)fps);
				if (showsubfr)
					s1+=String(L"::") + String::formatted (L"%02d", (int64(absSecs*fps*100.0) % 100));
			}
			else {
				s1 = String::formatted (L"%s%02d:%02d:%02d",
										  sign, hours, mins, secs);
			}
		}
		else {
			if (showms) {
				if (fps==1000.0)
					s1 = String::formatted (L"%s%d:%02d.%03d",
											  sign, mins, secs,
											  int64 (absSecs * 1000) % 1000);
				else if (fps<=10.0)
					s1 = String::formatted (L"%s%d:%02d:%.1d",
											  sign, mins, secs,
											  int64(absSecs*fps) % (int)fps);
				else if (fps<=100.0) {
					if (dropframe) {
						int64 frameNumber = int64(absSecs*29.97);
						frameNumber +=  18*(frameNumber/17982) + 2*(((frameNumber%17982) - 2) / 1798);
						int frames   = int(frameNumber % 30);
						int dseconds = int((frameNumber / 30) % 60);
						int dminutes = int(((frameNumber / 30) / 60) % 60);

						s1 = String::formatted (L"%s%d:%02d;%.2d",
												  sign, dminutes, dseconds,
												  frames);
					}
					else
						s1 = String::formatted (L"%s%d:%02d:%.2d",
												  sign, mins, secs,
												  int64(absSecs*fps) % (int)fps);
				}
				else
					s1 = String::formatted (L"%s%d:%02d:%03d",
											  sign, mins, secs,
											  int64(absSecs*fps) % (int)fps);
				if (showsubfr)
					s1 += String(L"::") + String::formatted (L"%02d", (int64(absSecs*fps*100.0) % 100));
			}
			else {
				s1 = String::formatted (L"%s%d:%02d",
										  sign, mins, secs);
			}
		}
		return s1;
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiMonitorEditor" componentName=""
                 parentClasses="public juce::AudioProcessorEditor, public juce::ChangeListener, public juce::Timer"
                 constructorParams="MidiMonitorPlugin* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="392" initialHeight="400">
  <BACKGROUND backgroundColour="ffa1a1a1"/>
  <TEXTBUTTON name="clear" id="f23ecb245471280e" memberName="clearButton" virtualName=""
              explicitFocusOrder="0" pos="8 0 40 16" buttonText="clear" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="power" id="c718096678cae19a" memberName="powerButton" virtualName=""
                explicitFocusOrder="0" pos="48 0 30 16" buttonText="on" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="thru" id="f9ffa6dc4b379fe2" memberName="thruButton" virtualName=""
                explicitFocusOrder="0" pos="80 0 40 16" buttonText="thru" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="bytes" id="8c2cee21d2931646" memberName="bytesButton" virtualName=""
                explicitFocusOrder="0" pos="118 0 40 16" buttonText="bytes" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="hue" id="dd023d10d928edd1" memberName="hueSlider" virtualName=""
          explicitFocusOrder="0" pos="192 -16 40 12" tooltip="Hue" min="0.0"
          max="1.0" int="0.01" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="saturation" id="c426691da7ce7b7c" memberName="saturationSlider"
          virtualName="" explicitFocusOrder="0" pos="240 -16 40 12" tooltip="Saturation"
          min="0.0" max="1.0" int="0.01" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="lightness" id="a671afc03bda74c5" memberName="lightnessSlider"
          virtualName="" explicitFocusOrder="0" pos="288 -16 40 12" tooltip="Lightness"
          min="0.0" max="1.0" int="0.01" style="LinearBar" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <GENERICCOMPONENT name="" id="b1de7dcd8c1d0b56" memberName="resizer" virtualName=""
                    explicitFocusOrder="0" pos="16R 16R 16 16" class="ResizableCornerComponent"
                    params="this, &amp;resizeLimits"/>
  <TEXTBUTTON name="save" id="c3cb98e37778ec52" memberName="saveButton" virtualName=""
              explicitFocusOrder="0" pos="344 0 40 16" buttonText="save" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="menu" id="5bd1f8f8732e683a" memberName="menuButton" virtualName=""
              explicitFocusOrder="0" pos="296 0 40 16" buttonText="menu" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="monitorbox" id="c54391c6e2b371b4" memberName="midiOutputEditor"
              virtualName="" explicitFocusOrder="0" pos="8 16 16M 24M" initialText=""
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
  <TOGGLEBUTTON name="wrap" id="929dfefc8dd8d762" memberName="wrapButton" virtualName=""
                explicitFocusOrder="0" pos="200 0 41 16" buttonText="wrap" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="time" id="149dbb93f3746640" memberName="timeButton" virtualName=""
                explicitFocusOrder="0" pos="160 0 40 16" buttonText="time" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="color" id="2ebe38c7bd47e9eb" memberName="colorButton" virtualName=""
              explicitFocusOrder="0" pos="248 0 40 16" buttonText="color" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="AboutBox" id="fdd8f9b9b85f35f5" memberName="aboutBox" virtualName=""
              explicitFocusOrder="0" pos="8 16 16M 24M" initialText="Insert Piz Here midiMonitor 1.3&#10;&#10;You can save a &quot;midiMonitor.fxp&quot; with your default settings.&#10;&#10;Controls:&#10;&quot;clear&quot; - Clears the display and recorded MIDI buffer.&#10;&#10;&quot;on&quot; - The plugin is on when this is checked.&#10;&quot;thru&quot; - When checked, input events are passed to the output.&#10;&quot;bytes&quot; - Displays raw MIDI data in hexadecimal format.&#10;&quot;time&quot; - Displays time of MIDI events.&#10;&quot;wrap&quot; - Toggles word wrapping for the monitor box.&#10;&#10;&quot;color&quot; - Set background colors by Hue, Saturation, Lightness.&#10;&#10;&quot;menu&quot; - Options for: &#10;     - Time format&#10;     - Maximum number of lines to display&#10;     - MIDI event types to display&#10;&#10;&quot;save&quot; - Save monitor log as MIDI file or plain text. Specify .mid or .txt extension, or leave blank to save both. MIDI file contains all events since &quot;clear&quot; button was pressed. Text file contains the current contents of the monitor display."
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

