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
#include "juce_gui_basics/juce_gui_basics.h"
//[/Headers]

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PizButton : public juce::Button
{
public:
    //==============================================================================
    PizButton();
    ~PizButton() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void paintButton (juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

    // Binary resources:
    static const char* pirate_png;
    static const int pirate_pngSize;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    juce::Image cachedImage_pirate_png_1;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PizButton)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
