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

#include "WebBrowserPluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//==============================================================================
bool MyBrowser::pageAboutToLoad(const String& newURL) {
	if (!newURL.contains("googleads")) {
        lastURL=newURL;
    }
    sendChangeMessage();
    return true;
}

String MyBrowser::getCurrentURL() {
    return lastURL;
}

//[/MiscUserDefs]

//==============================================================================
WebBrowserPluginEditor::WebBrowserPluginEditor (WebBrowserFilter* const ownerFilter)
    : AudioProcessorEditor (ownerFilter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    reloadButton.reset (new juce::TextButton ("Reload"));
    addAndMakeVisible (reloadButton.get());
    reloadButton->addListener (this);

    reloadButton->setBounds (128, 0, 60, 22);

    wb.reset (new MyBrowser());
    addAndMakeVisible (wb.get());

    backButton.reset (new juce::TextButton ("Back"));
    addAndMakeVisible (backButton.get());
    backButton->addListener (this);

    backButton->setBounds (0, 0, 60, 22);

    forwardButton.reset (new juce::TextButton ("Forward"));
    addAndMakeVisible (forwardButton.get());
    forwardButton->addListener (this);

    forwardButton->setBounds (64, 0, 60, 22);

    homeButton.reset (new juce::TextButton ("Home"));
    addAndMakeVisible (homeButton.get());
    homeButton->addListener (this);

    homeButton->setBounds (256, 0, 60, 22);

    stopButton.reset (new juce::TextButton ("Stop"));
    addAndMakeVisible (stopButton.get());
    stopButton->addListener (this);

    stopButton->setBounds (192, 0, 60, 22);

    urlBar.reset (new juce::TextEditor ("URL Bar"));
    addAndMakeVisible (urlBar.get());
    urlBar->setMultiLine (false);
    urlBar->setReturnKeyStartsNewLine (false);
    urlBar->setReadOnly (false);
    urlBar->setScrollbarsShown (false);
    urlBar->setCaretVisible (true);
    urlBar->setPopupMenuEnabled (true);
    urlBar->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0xffcccccc));
    urlBar->setText (juce::String());

    goButton.reset (new juce::TextButton ("Go"));
    addAndMakeVisible (goButton.get());
    goButton->addListener (this);

    resizer.reset (new ResizableCornerComponent (this, &resizeLimits));
    addAndMakeVisible (resizer.get());


    //[UserPreSize]
	wb->addChangeListener(this);
    urlBar->addListener(this);
    resizeLimits.setSizeLimits (150, 150, 1600, 1600);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    setSize (ownerFilter->lastUIWidth, ownerFilter->lastUIHeight);
    ownerFilter->addChangeListener (this);
    updateParametersFromFilter();
    //[/Constructor]
}

WebBrowserPluginEditor::~WebBrowserPluginEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    getFilter()->setURL(wb->getCurrentURL());
    getFilter()->initialPageLoaded=false;
    getFilter()->removeChangeListener (this);
    //[/Destructor_pre]

    reloadButton = nullptr;
    wb = nullptr;
    backButton = nullptr;
    forwardButton = nullptr;
    homeButton = nullptr;
    stopButton = nullptr;
    urlBar = nullptr;
    goButton = nullptr;
    resizer = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void WebBrowserPluginEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colours::black);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void WebBrowserPluginEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    wb->setBounds (4, 22, getWidth() - 8, getHeight() - 26);
    urlBar->setBounds (320, 2, getWidth() - 370, 18);
    goButton->setBounds (getWidth() - 45, 0, 40, 22);
    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
    //[UserResized] Add your own custom resize handling here..
    getFilter()->lastUIWidth = getWidth();
    getFilter()->lastUIHeight = getHeight();
    //[/UserResized]
}

void WebBrowserPluginEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == reloadButton.get())
    {
        //[UserButtonCode_reloadButton] -- add your button handler code here..
        //if (wb->getCurrentURL().isNotEmpty()) wb->goToURL(wb->getCurrentURL());
		wb->refresh();
        //[/UserButtonCode_reloadButton]
    }
    else if (buttonThatWasClicked == backButton.get())
    {
        //[UserButtonCode_backButton] -- add your button handler code here..
		wb->goBack();
        //[/UserButtonCode_backButton]
    }
    else if (buttonThatWasClicked == forwardButton.get())
    {
        //[UserButtonCode_forwardButton] -- add your button handler code here..
        wb->goForward();
        //[/UserButtonCode_forwardButton]
    }
    else if (buttonThatWasClicked == homeButton.get())
    {
        //[UserButtonCode_homeButton] -- add your button handler code here..
        wb->goToURL("http://www.kvraudio.com/");
        //[/UserButtonCode_homeButton]
    }
    else if (buttonThatWasClicked == stopButton.get())
    {
        //[UserButtonCode_stopButton] -- add your button handler code here..
        wb->stop();
        //[/UserButtonCode_stopButton]
    }
    else if (buttonThatWasClicked == goButton.get())
    {
        //[UserButtonCode_goButton] -- add your button handler code here..
        wb->goToURL(urlBar->getText());
        //[/UserButtonCode_goButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//==============================================================================
void WebBrowserPluginEditor::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source==getFilter()) {
        updateParametersFromFilter();
    }
    else if (source==wb.get()) {
        getFilter()->setURL(wb->getCurrentURL());
        urlBar->setText(wb->getCurrentURL());
        getFilter()->initialPageLoaded=true;
    }
}

void WebBrowserPluginEditor::textEditorReturnKeyPressed(TextEditor& editor)
{
    wb->goToURL((const String)(editor.getText()));
}

void WebBrowserPluginEditor::updateParametersFromFilter()
{
    WebBrowserFilter* const filter = getFilter();

    // we use this lock to make sure the processBlock() method isn't writing to the
    // lastMidiMessage variable while we're trying to read it, but be extra-careful to
    // only hold the lock for a minimum amount of time..
    filter->getCallbackLock().enter();

    // take a local copy of the info we need while we've got the lock..
    const float newGain = filter->getParameter (0);
    String URL = filter->getURL();

    // ..release the lock ASAP
    filter->getCallbackLock().exit();

    if (URL.isNotEmpty()) wb->goToURL(URL);

    setSize (filter->lastUIWidth, filter->lastUIHeight);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="WebBrowserPluginEditor" componentName=""
                 parentClasses="public AudioProcessorEditor, public ChangeListener, public TextEditor::Listener"
                 constructorParams="WebBrowserFilter* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff000000"/>
  <TEXTBUTTON name="Reload" id="734905fefba3a33d" memberName="reloadButton"
              virtualName="" explicitFocusOrder="0" pos="128 0 60 22" buttonText="Reload"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="d88a1479243207f7" memberName="wb" virtualName="MyBrowser"
                    explicitFocusOrder="0" pos="4 22 8M 26M" class="WebBrowserComponent"
                    params=""/>
  <TEXTBUTTON name="Back" id="bcd5e2c00c992a59" memberName="backButton" virtualName=""
              explicitFocusOrder="0" pos="0 0 60 22" buttonText="Back" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Forward" id="d89d21061858fdfe" memberName="forwardButton"
              virtualName="" explicitFocusOrder="0" pos="64 0 60 22" buttonText="Forward"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Home" id="5046ad63da9fdf15" memberName="homeButton" virtualName=""
              explicitFocusOrder="0" pos="256 0 60 22" buttonText="Home" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Stop" id="e71da6988feab86a" memberName="stopButton" virtualName=""
              explicitFocusOrder="0" pos="192 0 60 22" buttonText="Stop" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="URL Bar" id="6a2d12ba982715bd" memberName="urlBar" virtualName=""
              explicitFocusOrder="0" pos="320 2 370M 18" bkgcol="ffcccccc"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="0" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="Go" id="8a8355624d328580" memberName="goButton" virtualName=""
              explicitFocusOrder="0" pos="45R 0 40 22" buttonText="Go" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="" id="d5aa690627d1ee79" memberName="resizer" virtualName=""
                    explicitFocusOrder="0" pos="16R 16R 16 16" class="ResizableCornerComponent"
                    params="this, &amp;resizeLimits"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

