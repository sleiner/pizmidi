/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  14 Sep 2011 10:44:22am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_WEBBROWSERPLUGINEDITOR_WEBBROWSERPLUGINEDITOR_9C8171F9__
#define __JUCER_HEADER_WEBBROWSERPLUGINEDITOR_WEBBROWSERPLUGINEDITOR_9C8171F9__

#include "juce_events/juce_events.h"
#include "juce_gui_extra/juce_gui_extra.h"

//[Headers]     -- You can add your own extra header files here --
#include "WebBrowserFilter.h"

class MyBrowser : public WebBrowserComponent,
                  public ChangeBroadcaster
{
public:
	MyBrowser()
		: unloadPageWhenBrowserIsHidden(false)
	{};
    ~MyBrowser() {};
    bool pageAboutToLoad(const String& newURL);
    String getCurrentURL();
    friend class WebBrowserComponent;
    String lastURL;
private:
	bool unloadPageWhenBrowserIsHidden;
};

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class WebBrowserPluginEditor  : public AudioProcessorEditor,
                                public ChangeListener,
                                public TextEditor::Listener,
                                public Button::Listener
{
public:
    //==============================================================================
    WebBrowserPluginEditor (WebBrowserFilter* const ownerFilter);
    ~WebBrowserPluginEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void textEditorReturnKeyPressed(TextEditor& editor);
    void textEditorEscapeKeyPressed(TextEditor& editor){};
    void textEditorTextChanged(TextEditor& editor){};
    void textEditorFocusLost(TextEditor& editor){};
    void changeListenerCallback (ChangeBroadcaster* source);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ComponentBoundsConstrainer resizeLimits;
    TooltipWindow tooltipWindow;

    void updateParametersFromFilter();

    // handy wrapper method to avoid having to cast the filter to a DemoJuceFilter
    // every time we need it..
    WebBrowserFilter* getFilter() const throw()       { return (WebBrowserFilter*) getAudioProcessor(); }
    //[/UserVariables]

    //==============================================================================
    TextButton* reloadButton;
    MyBrowser* wb;
    TextButton* backButton;
    TextButton* forwardButton;
    TextButton* homeButton;
    TextButton* stopButton;
    TextEditor* urlBar;
    TextButton* goButton;
    ResizableCornerComponent* resizer;


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    WebBrowserPluginEditor (const WebBrowserPluginEditor&);
    const WebBrowserPluginEditor& operator= (const WebBrowserPluginEditor&);
};


#endif   // __JUCER_HEADER_WEBBROWSERPLUGINEDITOR_WEBBROWSERPLUGINEDITOR_9C8171F9__
