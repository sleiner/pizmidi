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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "WebBrowserFilter.h"

#include <juce_events/juce_events.h>
#include <juce_gui_extra/juce_gui_extra.h>

class MyBrowser : public juce::WebBrowserComponent,
                  public juce::ChangeBroadcaster
{
public:
    MyBrowser()
        : unloadPageWhenBrowserIsHidden(false){};
    ~MyBrowser() override{};
    bool pageAboutToLoad(const juce::String& newURL) override;
    juce::String getCurrentURL();
    friend class WebBrowserComponent;
    juce::String lastURL;

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
class WebBrowserPluginEditor : public juce::AudioProcessorEditor,
                               public juce::ChangeListener,
                               public juce::TextEditor::Listener,
                               public juce::Button::Listener
{
public:
    //==============================================================================
    WebBrowserPluginEditor(WebBrowserFilter* const ownerFilter);
    ~WebBrowserPluginEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;
    void textEditorEscapeKeyPressed(juce::TextEditor& editor) override{};
    void textEditorTextChanged(juce::TextEditor& editor) override{};
    void textEditorFocusLost(juce::TextEditor& editor) override{};
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    //[/UserMethods]

    void paint(juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* buttonThatWasClicked) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    juce::ComponentBoundsConstrainer resizeLimits;
    juce::TooltipWindow tooltipWindow;

    void updateParametersFromFilter();

    // handy wrapper method to avoid having to cast the filter to a DemoJuceFilter
    // every time we need it
    WebBrowserFilter* getFilter() const throw()
    {
        return (WebBrowserFilter*) getAudioProcessor();
    }

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> reloadButton;
    std::unique_ptr<MyBrowser> wb;
    std::unique_ptr<juce::TextButton> backButton;
    std::unique_ptr<juce::TextButton> forwardButton;
    std::unique_ptr<juce::TextButton> homeButton;
    std::unique_ptr<juce::TextButton> stopButton;
    std::unique_ptr<juce::TextEditor> urlBar;
    std::unique_ptr<juce::TextButton> goButton;
    std::unique_ptr<juce::ResizableCornerComponent> resizer;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WebBrowserPluginEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
