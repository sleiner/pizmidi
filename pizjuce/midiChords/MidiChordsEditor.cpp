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

#include "MidiChordsEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MidiChordsEditor::MidiChordsEditor (MidiChords* const ownerFilter)
    : AudioProcessorEditor (ownerFilter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    toggleButton.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton.get());
    toggleButton->setButtonText (TRANS("Guess chord name"));
    toggleButton->addListener (this);

    toggleButton->setBounds (372, 50, 150, 24);

    chordNameLabel.reset (new juce::Label ("new label",
                                           TRANS("chord name")));
    addAndMakeVisible (chordNameLabel.get());
    chordNameLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
    chordNameLabel->setJustificationType (juce::Justification::centred);
    chordNameLabel->setEditable (false, false, false);
    chordNameLabel->setColour (juce::Label::backgroundColourId, juce::Colour (0x279a3c3c));
    chordNameLabel->setColour (juce::Label::outlineColourId, juce::Colour (0xb3000000));
    chordNameLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    chordNameLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    chordNameLabel->setBounds (375, 72, 144, 26);

    chordKeyboard.reset (new ChordsKeyboardComponent (ownerFilter->chordKbState, ownerFilter));
    addAndMakeVisible (chordKeyboard.get());

    triggerKeyboard.reset (new TriggerKeySelectorKeyboard (ownerFilter->triggerKbState, ownerFilter));
    addAndMakeVisible (triggerKeyboard.get());

    chordLearnButton.reset (new juce::TextButton ("chordLearn"));
    addAndMakeVisible (chordLearnButton.get());
    chordLearnButton->setTooltip (TRANS("Learn next input chord"));
    chordLearnButton->setButtonText (TRANS("Learn"));
    chordLearnButton->setConnectedEdges (juce::Button::ConnectedOnBottom);
    chordLearnButton->addListener (this);

    chordLearnButton->setBounds (6, 76, 45, 21);

    triggerLearnButton.reset (new juce::TextButton ("triggerLearn"));
    addAndMakeVisible (triggerLearnButton.get());
    triggerLearnButton->setTooltip (TRANS("When on, displayed trigger note follows input note"));
    triggerLearnButton->setButtonText (TRANS("Follow Input"));
    triggerLearnButton->setConnectedEdges (juce::Button::ConnectedOnBottom);
    triggerLearnButton->addListener (this);

    triggerLearnButton->setBounds (6, 204, 80, 21);

    channelSlider.reset (new ChannelSlider ("channel"));
    addAndMakeVisible (channelSlider.get());
    channelSlider->setTooltip (TRANS("Trigger channel"));
    channelSlider->setRange (0, 16, 1);
    channelSlider->setSliderStyle (juce::Slider::LinearBar);
    channelSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    channelSlider->setColour (juce::Slider::backgroundColourId, juce::Colours::white);
    channelSlider->addListener (this);

    channelSlider->setBounds (123, 323, 66, 16);

    outputLabel.reset (new juce::Label ("new label",
                                        TRANS("Output")));
    addAndMakeVisible (outputLabel.get());
    outputLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    outputLabel->setJustificationType (juce::Justification::centredRight);
    outputLabel->setEditable (false, false, false);
    outputLabel->setColour (juce::Label::textColourId, juce::Colours::white);
    outputLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    outputLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    outputLabel->setBounds (573, 76, 54, 24);

    pizButton.reset (new PizButton());
    addAndMakeVisible (pizButton.get());
    pizButton->setBounds (38, 3, 74, 40);

    triggerLabel.reset (new juce::Label ("new label",
                                         TRANS("Trigger Note:")));
    addAndMakeVisible (triggerLabel.get());
    triggerLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    triggerLabel->setJustificationType (juce::Justification::centredRight);
    triggerLabel->setEditable (false, false, false);
    triggerLabel->setColour (juce::Label::textColourId, juce::Colours::white);
    triggerLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    triggerLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    triggerLabel->setBounds (464, 203, 97, 24);

    clearChordButton.reset (new juce::TextButton ("clear"));
    addAndMakeVisible (clearChordButton.get());
    clearChordButton->setTooltip (TRANS("Clear current chord"));
    clearChordButton->setButtonText (TRANS("Clear"));
    clearChordButton->setConnectedEdges (juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    clearChordButton->addListener (this);

    clearChordButton->setBounds (291, 76, 40, 21);

    resetChordButton.reset (new juce::TextButton ("reset"));
    addAndMakeVisible (resetChordButton.get());
    resetChordButton->setTooltip (TRANS("Reset current chord to just the trigger note"));
    resetChordButton->setButtonText (TRANS("Reset"));
    resetChordButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    resetChordButton->addListener (this);

    resetChordButton->setBounds (331, 76, 40, 21);

    clearAllButton.reset (new juce::TextButton ("clear"));
    addAndMakeVisible (clearAllButton.get());
    clearAllButton->setButtonText (TRANS("Clear All"));
    clearAllButton->setConnectedEdges (juce::Button::ConnectedOnRight);
    clearAllButton->addListener (this);

    clearAllButton->setBounds (8, 429, 64, 24);

    resetAllButton.reset (new juce::TextButton ("reset"));
    addAndMakeVisible (resetAllButton.get());
    resetAllButton->setButtonText (TRANS("Reset All"));
    resetAllButton->setConnectedEdges (juce::Button::ConnectedOnLeft);
    resetAllButton->addListener (this);

    resetAllButton->setBounds (69, 441, 64, 24);

    transposeUpButton.reset (new juce::TextButton ("transpose"));
    addAndMakeVisible (transposeUpButton.get());
    transposeUpButton->setButtonText (TRANS("->"));
    transposeUpButton->setConnectedEdges (juce::Button::ConnectedOnLeft);
    transposeUpButton->addListener (this);

    transposeUpButton->setBounds (172, 425, 32, 24);

    transposeDownButton.reset (new juce::TextButton ("transpose"));
    addAndMakeVisible (transposeDownButton.get());
    transposeDownButton->setButtonText (TRANS("<-"));
    transposeDownButton->setConnectedEdges (juce::Button::ConnectedOnRight);
    transposeDownButton->addListener (this);

    transposeDownButton->setBounds (143, 425, 29, 24);

    transposeChordUpButton.reset (new juce::TextButton ("transpose"));
    addAndMakeVisible (transposeChordUpButton.get());
    transposeChordUpButton->setTooltip (TRANS("Shift chord up one semitone"));
    transposeChordUpButton->setButtonText (TRANS(">"));
    transposeChordUpButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    transposeChordUpButton->addListener (this);

    transposeChordUpButton->setBounds (159, 76, 23, 21);

    transposeChordDownButton.reset (new juce::TextButton ("transpose"));
    addAndMakeVisible (transposeChordDownButton.get());
    transposeChordDownButton->setTooltip (TRANS("Shift chord down one semitone"));
    transposeChordDownButton->setButtonText (TRANS("<"));
    transposeChordDownButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    transposeChordDownButton->addListener (this);

    transposeChordDownButton->setBounds (136, 76, 23, 21);

    transposeSlider.reset (new juce::Slider ("transpose"));
    addAndMakeVisible (transposeSlider.get());
    transposeSlider->setTooltip (TRANS("Transpose output by semitones"));
    transposeSlider->setRange (-12, 12, 1);
    transposeSlider->setSliderStyle (juce::Slider::LinearBar);
    transposeSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    transposeSlider->setColour (juce::Slider::backgroundColourId, juce::Colours::white);
    transposeSlider->addListener (this);

    transposeSlider->setBounds (475, 372, 66, 16);

    velocitySlider.reset (new juce::Slider ("velocity"));
    addAndMakeVisible (velocitySlider.get());
    velocitySlider->setTooltip (TRANS("Velocity for \"Play Chord\" button"));
    velocitySlider->setRange (1, 127, 1);
    velocitySlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    velocitySlider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    velocitySlider->setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0x7f000000));
    velocitySlider->addListener (this);

    velocitySlider->setBounds (397, 197, 22, 22);

    variationSlider.reset (new juce::Slider ("Variation"));
    addAndMakeVisible (variationSlider.get());
    variationSlider->setRange (0, 100, 0.1);
    variationSlider->setSliderStyle (juce::Slider::LinearBar);
    variationSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    variationSlider->addListener (this);

    variationSlider->setBounds (333, 432, 104, 16);

    normalButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (normalButton.get());
    normalButton->setTooltip (TRANS("Full mode: a separate chord is defined for every input note"));
    normalButton->setButtonText (TRANS("Full"));
    normalButton->setConnectedEdges (juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop);
    normalButton->setRadioGroupId (1);
    normalButton->addListener (this);

    normalButton->setBounds (442, 318, 64, 24);

    octaveButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (octaveButton.get());
    octaveButton->setTooltip (TRANS("Octave mode: 12 chords are defined which are transposed to every octave"));
    octaveButton->setButtonText (TRANS("Octave"));
    octaveButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop);
    octaveButton->setRadioGroupId (1);
    octaveButton->addListener (this);

    octaveButton->setBounds (506, 318, 64, 24);

    globalButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (globalButton.get());
    globalButton->setTooltip (TRANS("Global mode: one defined chord is transposed relative to the root note"));
    globalButton->setButtonText (TRANS("Global"));
    globalButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnTop);
    globalButton->setRadioGroupId (1);
    globalButton->addListener (this);

    globalButton->setBounds (570, 318, 64, 24);

    flatsButton.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (flatsButton.get());
    flatsButton->setButtonText (TRANS("flats"));
    flatsButton->addListener (this);

    flatsButton->setBounds (519, 76, 48, 24);

    transposeChordUpButton2.reset (new juce::TextButton ("transpose"));
    addAndMakeVisible (transposeChordUpButton2.get());
    transposeChordUpButton2->setTooltip (TRANS("Shift chord up one octave"));
    transposeChordUpButton2->setButtonText (TRANS(">>"));
    transposeChordUpButton2->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    transposeChordUpButton2->addListener (this);

    transposeChordUpButton2->setBounds (182, 76, 25, 21);

    transposeChordDownButton2.reset (new juce::TextButton ("transpose"));
    addAndMakeVisible (transposeChordDownButton2.get());
    transposeChordDownButton2->setTooltip (TRANS("Shift chord down one octave"));
    transposeChordDownButton2->setButtonText (TRANS("<<"));
    transposeChordDownButton2->setConnectedEdges (juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    transposeChordDownButton2->addListener (this);

    transposeChordDownButton2->setBounds (111, 76, 25, 21);

    chordMenuButton.reset (new juce::TextButton ("chordMenu"));
    addAndMakeVisible (chordMenuButton.get());
    chordMenuButton->setButtonText (TRANS("Menu"));
    chordMenuButton->setConnectedEdges (juce::Button::ConnectedOnBottom);
    chordMenuButton->addListener (this);

    chordMenuButton->setBounds (55, 76, 52, 21);

    presetNameLabel.reset (new juce::Label ("new label",
                                            TRANS("preset name")));
    addAndMakeVisible (presetNameLabel.get());
    presetNameLabel->setTooltip (TRANS("Preset Name (double-click to edit)"));
    presetNameLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
    presetNameLabel->setJustificationType (juce::Justification::centred);
    presetNameLabel->setEditable (true, true, false);
    presetNameLabel->setColour (juce::Label::backgroundColourId, juce::Colour (0x27a1b404));
    presetNameLabel->setColour (juce::Label::outlineColourId, juce::Colour (0xb3000000));
    presetNameLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    presetNameLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));
    presetNameLabel->addListener (this);

    presetNameLabel->setBounds (414, 12, 191, 26);

    presetMenuButton.reset (new juce::TextButton ("presets"));
    addAndMakeVisible (presetMenuButton.get());
    presetMenuButton->setTooltip (TRANS("Preset Menu"));
    presetMenuButton->setButtonText (TRANS(">"));
    presetMenuButton->setConnectedEdges (juce::Button::ConnectedOnLeft);
    presetMenuButton->addListener (this);

    presetMenuButton->setBounds (605, 12, 25, 26);

    chordSaveEditor.reset (new juce::TextEditor ("new text editor"));
    addAndMakeVisible (chordSaveEditor.get());
    chordSaveEditor->setMultiLine (false);
    chordSaveEditor->setReturnKeyStartsNewLine (false);
    chordSaveEditor->setReadOnly (false);
    chordSaveEditor->setScrollbarsShown (true);
    chordSaveEditor->setCaretVisible (true);
    chordSaveEditor->setPopupMenuEnabled (true);
    chordSaveEditor->setColour (juce::TextEditor::outlineColourId, juce::Colours::black);
    chordSaveEditor->setText (juce::String());

    chordSaveEditor->setBounds (82, -35, 150, 24);

    copyButton.reset (new juce::TextButton ("copy"));
    addAndMakeVisible (copyButton.get());
    copyButton->setButtonText (TRANS("Copy"));
    copyButton->setConnectedEdges (juce::Button::ConnectedOnRight | juce::Button::ConnectedOnTop);
    copyButton->addListener (this);

    copyButton->setBounds (211, 73, 38, 21);

    pasteButton.reset (new juce::TextButton ("paste"));
    addAndMakeVisible (pasteButton.get());
    pasteButton->setButtonText (TRANS("Paste"));
    pasteButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnTop);
    pasteButton->addListener (this);

    pasteButton->setBounds (249, 73, 38, 21);

    previewButton.reset (new juce::TextButton ("play"));
    addAndMakeVisible (previewButton.get());
    previewButton->setTooltip (TRANS("Plays currently displayed chord"));
    previewButton->setButtonText (TRANS("Play Chord"));
    previewButton->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff7ca17c));

    previewButton->setBounds (251, 196, 143, 24);

    chordEditor.reset (new juce::Label ("new label",
                                        TRANS("chord")));
    addAndMakeVisible (chordEditor.get());
    chordEditor->setTooltip (TRANS("Double-click to type a chord"));
    chordEditor->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    chordEditor->setJustificationType (juce::Justification::centred);
    chordEditor->setEditable (false, true, false);
    chordEditor->setColour (juce::Label::backgroundColourId, juce::Colours::white);
    chordEditor->setColour (juce::Label::outlineColourId, juce::Colour (0xb3000000));
    chordEditor->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    chordEditor->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));
    chordEditor->addListener (this);

    chordEditor->setBounds (178, 54, 144, 20);

    pcButton.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (pcButton.get());
    pcButton->setTooltip (TRANS("Change program when receiving MIDI Program Change"));
    pcButton->setButtonText (TRANS("Use Program Chg"));
    pcButton->addListener (this);

    pcButton->setBounds (500, 397, 134, 20);

    nextButton.reset (new juce::TextButton ("next"));
    addAndMakeVisible (nextButton.get());
    nextButton->setTooltip (TRANS("Select next higher trigger note"));
    nextButton->setButtonText (TRANS(">"));
    nextButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnBottom);
    nextButton->addListener (this);

    nextButton->setBounds (113, 204, 23, 21);

    prevButton.reset (new juce::TextButton ("prev"));
    addAndMakeVisible (prevButton.get());
    prevButton->setTooltip (TRANS("Select previous trigger note"));
    prevButton->setButtonText (TRANS("<"));
    prevButton->setConnectedEdges (juce::Button::ConnectedOnRight | juce::Button::ConnectedOnBottom);
    prevButton->addListener (this);

    prevButton->setBounds (90, 204, 23, 21);

    triggerNoteLabel.reset (new juce::Label ("new label",
                                             TRANS("G8 (127)")));
    addAndMakeVisible (triggerNoteLabel.get());
    triggerNoteLabel->setTooltip (TRANS("Currently displayed trigger note (double-click to edit)"));
    triggerNoteLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Bold"));
    triggerNoteLabel->setJustificationType (juce::Justification::centredLeft);
    triggerNoteLabel->setEditable (false, true, false);
    triggerNoteLabel->setColour (juce::Label::textColourId, juce::Colours::white);
    triggerNoteLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    triggerNoteLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colours::white);
    triggerNoteLabel->addListener (this);

    triggerNoteLabel->setBounds (558, 203, 73, 24);

    learnChanSlider.reset (new ChannelSlider ("channel"));
    addAndMakeVisible (learnChanSlider.get());
    learnChanSlider->setTooltip (TRANS("Chord Input Channel, used for Learn and left-click entering notes"));
    learnChanSlider->setRange (0, 16, 1);
    learnChanSlider->setSliderStyle (juce::Slider::LinearBar);
    learnChanSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    learnChanSlider->setColour (juce::Slider::backgroundColourId, juce::Colours::white);
    learnChanSlider->addListener (this);

    learnChanSlider->setBounds (6, 56, 38, 16);

    demoLabel.reset (new juce::Label ("new label",
                                      TRANS("UNREGISTERED\n"
                                      "DEMO VERSION")));
    addAndMakeVisible (demoLabel.get());
    demoLabel->setFont (juce::Font ("OCR A Extended", 10.00f, juce::Font::plain));
    demoLabel->setJustificationType (juce::Justification::centred);
    demoLabel->setEditable (false, false, false);
    demoLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    demoLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    demoLabel->setBounds (318, 5, 97, 24);

    guitar.reset (new ChordsGuitar (ownerFilter->chordKbState, ownerFilter));
    addAndMakeVisible (guitar.get());
    guitar->setName ("new component");

    versionLabel.reset (new juce::Label ("new label",
                                         TRANS("99.99.99")));
    addAndMakeVisible (versionLabel.get());
    versionLabel->setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    versionLabel->setJustificationType (juce::Justification::centredLeft);
    versionLabel->setEditable (false, false, false);
    versionLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    versionLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    versionLabel->setBounds (352, 28, 58, 24);

    transposeInputButton.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (transposeInputButton.get());
    transposeInputButton->setTooltip (TRANS("When checked, incoming trigger notes are transposed by the opposite of the \"Transpose\" slider."));
    transposeInputButton->setButtonText (TRANS("Also Transpose Input"));
    transposeInputButton->addListener (this);

    transposeInputButton->setBounds (545, 370, 91, 21);

    toAllChannelsButton.reset (new juce::ToggleButton ("new toggle button"));
    addAndMakeVisible (toAllChannelsButton.get());
    toAllChannelsButton->setTooltip (TRANS("When checked, CCs (and other control messages) are sent to all MIDI channels. Otherwise they are passed through on the original channel."));
    toAllChannelsButton->setButtonText (TRANS("CCs to All Chans"));
    toAllChannelsButton->addListener (this);

    toAllChannelsButton->setBounds (364, 397, 129, 20);

    infoButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (infoButton.get());
    infoButton->setButtonText (TRANS("?"));
    infoButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnTop);
    infoButton->addListener (this);

    infoButton->setBounds (-1, -1, 19, 18);

    specialMenuButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (specialMenuButton.get());
    specialMenuButton->setButtonText (TRANS("Global Functions..."));
    specialMenuButton->addListener (this);

    specialMenuButton->setBounds (6, 357, 140, 24);

    outputChannelSlider.reset (new ChannelSlider ("channel"));
    addAndMakeVisible (outputChannelSlider.get());
    outputChannelSlider->setTooltip (TRANS("Output channel (\"Multi\" is as saved, otherwise overrides chord\'s saved channel)"));
    outputChannelSlider->setRange (0, 16, 1);
    outputChannelSlider->setSliderStyle (juce::Slider::LinearBar);
    outputChannelSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    outputChannelSlider->setColour (juce::Slider::backgroundColourId, juce::Colours::white);
    outputChannelSlider->addListener (this);

    outputChannelSlider->setBounds (381, 372, 66, 16);

    applyChannelButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (applyChannelButton.get());
    applyChannelButton->setTooltip (TRANS("Applies input channel to selected chord"));
    applyChannelButton->setButtonText (TRANS("Apply"));
    applyChannelButton->setConnectedEdges (juce::Button::ConnectedOnLeft);
    applyChannelButton->addListener (this);

    applyChannelButton->setBounds (44, 56, 32, 16);

    label.reset (new juce::Label ("new label",
                                  TRANS("Out Channel")));
    addAndMakeVisible (label.get());
    label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (juce::Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label->setBounds (368, 357, 91, 13);

    label2.reset (new juce::Label ("new label",
                                   TRANS("Transpose")));
    addAndMakeVisible (label2.get());
    label2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (juce::Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    label2->setBounds (461, 357, 91, 13);

    viewButton.reset (new juce::TextButton ("view"));
    addAndMakeVisible (viewButton.get());
    viewButton->setTooltip (TRANS("Switch between keyboard & guitar views"));
    viewButton->setButtonText (TRANS("View"));
    viewButton->setConnectedEdges (juce::Button::ConnectedOnTop);
    viewButton->addListener (this);

    viewButton->setBounds (151, 190, 41, 21);

    setupButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (setupButton.get());
    setupButton->setButtonText (TRANS("Setup"));
    setupButton->setConnectedEdges (juce::Button::ConnectedOnTop);
    setupButton->addListener (this);

    setupButton->setBounds (195, 190, 45, 21);

    strumDirectionButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (strumDirectionButton.get());
    strumDirectionButton->setTooltip (TRANS("Strum Direction"));
    strumDirectionButton->setButtonText (TRANS("Down"));
    strumDirectionButton->addListener (this);

    strumDirectionButton->setBounds (267, 327, 48, 19);

    strumButton.reset (new juce::TextButton ("new button"));
    addAndMakeVisible (strumButton.get());
    strumButton->setButtonText (TRANS("Strum"));
    strumButton->setConnectedEdges (juce::Button::ConnectedOnRight);
    strumButton->addListener (this);

    strumButton->setBounds (142, 391, 54, 21);

    maxTimeSlider.reset (new VSTSlider ("maxTime"));
    addAndMakeVisible (maxTimeSlider.get());
    maxTimeSlider->setTooltip (TRANS("Max Strum Time"));
    maxTimeSlider->setRange (100, 3000, 1);
    maxTimeSlider->setSliderStyle (juce::Slider::LinearBar);
    maxTimeSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    maxTimeSlider->setColour (juce::Slider::backgroundColourId, juce::Colours::white);
    maxTimeSlider->addListener (this);

    maxTimeSlider->setBounds (202, 364, 66, 16);

    speedSlider.reset (new VSTSlider ("speed"));
    addAndMakeVisible (speedSlider.get());
    speedSlider->setTooltip (TRANS("Strum Speed"));
    speedSlider->setRange (0, 100, 1);
    speedSlider->setSliderStyle (juce::Slider::LinearBar);
    speedSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    speedSlider->setColour (juce::Slider::backgroundColourId, juce::Colours::white);
    speedSlider->addListener (this);

    speedSlider->setBounds (202, 393, 66, 16);

    accelSlider.reset (new VSTSlider ("accel"));
    addAndMakeVisible (accelSlider.get());
    accelSlider->setTooltip (TRANS("Strum Acceleration"));
    accelSlider->setRange (-100, 100, 1);
    accelSlider->setSliderStyle (juce::Slider::LinearBar);
    accelSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    accelSlider->setColour (juce::Slider::backgroundColourId, juce::Colours::white);
    accelSlider->addListener (this);

    accelSlider->setBounds (275, 393, 66, 16);

    velRampSlider.reset (new VSTSlider ("velRamp"));
    addAndMakeVisible (velRampSlider.get());
    velRampSlider->setTooltip (TRANS("Strum Velocity Ramp"));
    velRampSlider->setRange (-100, 100, 1);
    velRampSlider->setSliderStyle (juce::Slider::LinearBar);
    velRampSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    velRampSlider->setColour (juce::Slider::backgroundColourId, juce::Colours::white);
    velRampSlider->addListener (this);

    velRampSlider->setBounds (275, 364, 66, 16);

    infoBox.reset (new juce::TextEditor ("new text editor"));
    addAndMakeVisible (infoBox.get());
    infoBox->setMultiLine (true);
    infoBox->setReturnKeyStartsNewLine (false);
    infoBox->setReadOnly (true);
    infoBox->setScrollbarsShown (true);
    infoBox->setCaretVisible (false);
    infoBox->setPopupMenuEnabled (true);
    infoBox->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0xf2ffffff));
    infoBox->setColour (juce::TextEditor::outlineColourId, juce::Colours::black);
    infoBox->setColour (juce::TextEditor::shadowColourId, juce::Colour (0x38000000));
    infoBox->setText (juce::String());

    tuningSaveEditor.reset (new juce::TextEditor ("new text editor"));
    addAndMakeVisible (tuningSaveEditor.get());
    tuningSaveEditor->setMultiLine (false);
    tuningSaveEditor->setReturnKeyStartsNewLine (false);
    tuningSaveEditor->setReadOnly (false);
    tuningSaveEditor->setScrollbarsShown (true);
    tuningSaveEditor->setCaretVisible (true);
    tuningSaveEditor->setPopupMenuEnabled (true);
    tuningSaveEditor->setColour (juce::TextEditor::outlineColourId, juce::Colours::black);
    tuningSaveEditor->setText (juce::String());

    tuningSaveEditor->setBounds (253, -31, 150, 24);

    cachedImage_midichordsLogo_png_1 = juce::ImageCache::getFromMemory (midichordsLogo_png, midichordsLogo_pngSize);

    //[UserPreSize]

    fretsSlider = new FretsSlider (L"new slider");
    fretsSlider->setRange (5, maxFrets, 1);
    fretsSlider->setSliderStyle (Slider::LinearHorizontal);
    fretsSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    fretsSlider->addListener (this);
	fretsSlider->setMouseClickGrabsKeyboardFocus(false);

    stringsSlider = new StringsSlider (L"new slider");
    stringsSlider->setRange (1, maxStrings, 1);
    stringsSlider->setSliderStyle (Slider::LinearHorizontal);
    stringsSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    stringsSlider->addListener (this);
	stringsSlider->setMouseClickGrabsKeyboardFocus(false);

	for (int i=0;i<maxStrings;i++) {
		stringSlider[i]= new NoteSlider (L"new slider");
		stringSlider[i]->setRange (-1, 127, 1);
		stringSlider[i]->setSliderStyle (Slider::LinearHorizontal);
		stringSlider[i]->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
		stringSlider[i]->addListener (this);
		stringSlider[i]->setBottomOctave(ownerFilter->bottomOctave);
		stringSlider[i]->setMouseClickGrabsKeyboardFocus(false);
	}

	this->setMouseClickGrabsKeyboardFocus(false);
	chordKeyboard->setMouseClickGrabsKeyboardFocus(false);
	triggerKeyboard->setMouseClickGrabsKeyboardFocus(false);
	chordLearnButton->setMouseClickGrabsKeyboardFocus(false);
	triggerLearnButton->setMouseClickGrabsKeyboardFocus(false);
	channelSlider->setMouseClickGrabsKeyboardFocus(false);
	learnChanSlider->setMouseClickGrabsKeyboardFocus(false);
	outputLabel->setMouseClickGrabsKeyboardFocus(false);
	pizButton->setMouseClickGrabsKeyboardFocus(false);
	triggerLabel->setMouseClickGrabsKeyboardFocus(false);
	clearChordButton->setMouseClickGrabsKeyboardFocus(false);
	resetChordButton->setMouseClickGrabsKeyboardFocus(false);
	clearAllButton->setMouseClickGrabsKeyboardFocus(false);
	resetAllButton->setMouseClickGrabsKeyboardFocus(false);
	transposeUpButton->setMouseClickGrabsKeyboardFocus(false);
	transposeDownButton->setMouseClickGrabsKeyboardFocus(false);
	transposeChordUpButton->setMouseClickGrabsKeyboardFocus(false);
	transposeChordDownButton->setMouseClickGrabsKeyboardFocus(false);
	transposeSlider->setMouseClickGrabsKeyboardFocus(false);
	velocitySlider->setMouseClickGrabsKeyboardFocus(false);
	variationSlider->setMouseClickGrabsKeyboardFocus(false);
	normalButton->setMouseClickGrabsKeyboardFocus(false);
	octaveButton->setMouseClickGrabsKeyboardFocus(false);
	globalButton->setMouseClickGrabsKeyboardFocus(false);
	toggleButton->setMouseClickGrabsKeyboardFocus(false);
	chordNameLabel->setMouseClickGrabsKeyboardFocus(false);
	flatsButton->setMouseClickGrabsKeyboardFocus(false);
    chordMenuButton->setMouseClickGrabsKeyboardFocus(false);
    presetNameLabel->setMouseClickGrabsKeyboardFocus(false);
    presetMenuButton->setMouseClickGrabsKeyboardFocus(false);
	toAllChannelsButton->setMouseClickGrabsKeyboardFocus(false);
	specialMenuButton->setMouseClickGrabsKeyboardFocus(false);
	applyChannelButton->setMouseClickGrabsKeyboardFocus(false);
	outputChannelSlider->setMouseClickGrabsKeyboardFocus(false);
	label->setMouseClickGrabsKeyboardFocus(false);
	label2->setMouseClickGrabsKeyboardFocus(false);
	guitar->setMouseClickGrabsKeyboardFocus(false);
	setupButton->setMouseClickGrabsKeyboardFocus(false);
	strumButton->setMouseClickGrabsKeyboardFocus(false);
	strumDirectionButton->setMouseClickGrabsKeyboardFocus(false);

	accelSlider->setOwner(ownerFilter,kAccel);
	velRampSlider->setOwner(ownerFilter,kVelRamp);
	speedSlider->setOwner(ownerFilter,kSpeed);
	maxTimeSlider->setOwner(ownerFilter,kMaxDelay);

	guitar->setMouseCursor(MouseCursor::PointingHandCursor);
	chordKeyboard->setMouseCursor(MouseCursor::PointingHandCursor);
	triggerKeyboard->setMouseCursor(MouseCursor::PointingHandCursor);

	//channelSlider->setAllText("Any");
	learnChanSlider->setAllText("All");
	outputChannelSlider->setAllText("Multi");
	chordSaveEditor->addListener(this);
	chordSaveEditor->setTextToShowWhenEmpty("Save chord (type a name)",Colours::grey);
	tuningSaveEditor->addListener(this);
	tuningSaveEditor->setTextToShowWhenEmpty("Save tuning (type name)",Colours::grey);
	pizButton->addListener(this);
	pizButton->setTooltip("http://thepiz.org/plugins");
	previewButton->addMouseListener(this,false);
	velocitySlider->setPopupDisplayEnabled(true,true,this);
	velocitySlider->setDoubleClickReturnValue(true,100);
	triggerKeyboard->addMouseListener(this,false);

	variationSlider->setVisible(false);
	transposeDownButton->setVisible(false);
	transposeUpButton->setVisible(false);
	guitar->setVisible(false);

	File chordPath(getFilter()->dataPath+File::getSeparatorString()+"mappings");
	browser = new FileBrowserComponent(FileBrowserComponent::openMode|FileBrowserComponent::useTreeView|FileBrowserComponent::canSelectFiles,
			chordPath,&fileFilter,0);
	browser->addListener(this);

	Font const defaultFont = infoBox->getFont();
	infoBox->setVisible(false);
	infoBox->setFont(Font(18.f,Font::bold));
	infoBox->insertTextAtCaret("Insert Piz Here-> midiChords v." + String(JucePlugin_VersionString) + "\n\n");
	infoBox->setFont(defaultFont.boldened());
	infoBox->insertTextAtCaret("== Host Info ==\n");
	infoBox->setFont(defaultFont);
	infoBox->insertTextAtCaret(getFilter()->hostInfo + "\n\n");
	infoBox->setFont(defaultFont.boldened());
	infoBox->insertTextAtCaret("== Documentation ==\n");
	infoBox->setFont(defaultFont);
	infoBox->insertTextAtCaret(File(getFilter()->dataPath+File::getSeparatorString()+"readme.txt").loadFileAsString());

	const int middleC = getFilter()->getBottomOctave()+5;
	chordKeyboard->setOctaveForMiddleC(middleC);
	triggerKeyboard->setOctaveForMiddleC(middleC);
	guitar->setOctaveForMiddleC(middleC);

	mode = Normal;
    //[/UserPreSize]

    setSize (640, 420);


    //[Constructor] You can add your own custom stuff here..
	versionLabel->setText(JucePlugin_VersionString,dontSendNotification);
	ownerFilter->addChangeListener(this);
	if (!ownerFilter->demo)
		demoLabel->setVisible(false);
	updateParametersFromFilter();
    //[/Constructor]
}

MidiChordsEditor::~MidiChordsEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	getFilter()->removeChangeListener(this);

	deleteAndZero (browser);
    //[/Destructor_pre]

    toggleButton = nullptr;
    chordNameLabel = nullptr;
    chordKeyboard = nullptr;
    triggerKeyboard = nullptr;
    chordLearnButton = nullptr;
    triggerLearnButton = nullptr;
    channelSlider = nullptr;
    outputLabel = nullptr;
    pizButton = nullptr;
    triggerLabel = nullptr;
    clearChordButton = nullptr;
    resetChordButton = nullptr;
    clearAllButton = nullptr;
    resetAllButton = nullptr;
    transposeUpButton = nullptr;
    transposeDownButton = nullptr;
    transposeChordUpButton = nullptr;
    transposeChordDownButton = nullptr;
    transposeSlider = nullptr;
    velocitySlider = nullptr;
    variationSlider = nullptr;
    normalButton = nullptr;
    octaveButton = nullptr;
    globalButton = nullptr;
    flatsButton = nullptr;
    transposeChordUpButton2 = nullptr;
    transposeChordDownButton2 = nullptr;
    chordMenuButton = nullptr;
    presetNameLabel = nullptr;
    presetMenuButton = nullptr;
    chordSaveEditor = nullptr;
    copyButton = nullptr;
    pasteButton = nullptr;
    previewButton = nullptr;
    chordEditor = nullptr;
    pcButton = nullptr;
    nextButton = nullptr;
    prevButton = nullptr;
    triggerNoteLabel = nullptr;
    learnChanSlider = nullptr;
    demoLabel = nullptr;
    guitar = nullptr;
    versionLabel = nullptr;
    transposeInputButton = nullptr;
    toAllChannelsButton = nullptr;
    infoButton = nullptr;
    specialMenuButton = nullptr;
    outputChannelSlider = nullptr;
    applyChannelButton = nullptr;
    label = nullptr;
    label2 = nullptr;
    viewButton = nullptr;
    setupButton = nullptr;
    strumDirectionButton = nullptr;
    strumButton = nullptr;
    maxTimeSlider = nullptr;
    speedSlider = nullptr;
    accelSlider = nullptr;
    velRampSlider = nullptr;
    infoBox = nullptr;
    tuningSaveEditor = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    deleteAndZero (fretsSlider);
    deleteAndZero (stringsSlider);
	for (int i=0;i<maxStrings;i++)
		deleteAndZero (stringSlider[i]);
    //[/Destructor]
}

//==============================================================================
void MidiChordsEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffd8d8d8));

    {
        int x = 0, y = 0, width = 640, height = 100;
        juce::Colour fillColour1 = juce::Colours::white, fillColour2 = juce::Colour (0x00e7e7e7);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             61.0f - 0.0f + x,
                                             static_cast<float> (-31) - 0.0f + y,
                                             fillColour2,
                                             61.0f - 0.0f + x,
                                             23.0f - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = 113, y = 7, width = 234, height = 45;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (juce::Colours::black);
        g.drawImageWithin (cachedImage_midichordsLogo_png_1,
                           x, y, width, height,
                           juce::RectanglePlacement::centred,
                           false);
    }

    {
        int x = 0, y = 348, width = proportionOfWidth (1.0000f), height = 72;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour (0xff828282);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             0.0f - 0.0f + x,
                                             331.0f - 348.0f + y,
                                             fillColour2,
                                             0.0f - 0.0f + x,
                                             360.0f - 348.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = 196, y = 323, width = 150, height = 92;
        juce::Colour strokeColour1 = juce::Colour (0x00ff0000), strokeColour2 = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (strokeColour1,
                                             230.0f - 196.0f + x,
                                             323.0f - 323.0f + y,
                                             strokeColour2,
                                             230.0f - 196.0f + x,
                                             377.0f - 323.0f + y,
                                             false));
        g.drawRect (x, y, width, height, 1);

    }

    {
        float x = 568.0f, y = 76.0f, width = 66.0f, height = 106.0f;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 10.000f);
    }

    {
        int x = 14, y = 319, width = 103, height = 24;
        juce::String text (TRANS("Trigger Channel:"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 330, y = 318, width = 126, height = 24;
        juce::String text (TRANS("Trigger Mode:"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 6, y = 225, width = getWidth() - 12, height = 93;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 6, y = 97, width = getWidth() - 12, height = 93;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        float x = 462.0f, y = 202.0f, width = 172.0f, height = 36.0f;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 10.000f);
    }

    {
        float x = 250.0f, y = 195.0f, width = 173.0f, height = 26.0f;
        juce::Colour fillColour1 = juce::Colour (0xff666666), fillColour2 = juce::Colour (0xffbfbfbf);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             248.0f - 250.0f + x,
                                             183.0f - 195.0f + y,
                                             fillColour2,
                                             248.0f - 250.0f + x,
                                             215.0f - 195.0f + y,
                                             false));
        g.fillRoundedRectangle (x, y, width, height, 12.000f);
    }

    {
        int x = 6, y = 46, width = 71, height = 8;
        juce::String text (TRANS("Input Channel"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (10.70f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 219, y = 328, width = 53, height = 16;
        juce::String text (TRANS("Strum:"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 204, y = 351, width = 62, height = 16;
        juce::String text (TRANS("Max Time"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 277, y = 351, width = 62, height = 16;
        juce::String text (TRANS("Vel. Ramp"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 277, y = 380, width = 62, height = 16;
        juce::String text (TRANS("Accel"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 204, y = 380, width = 62, height = 16;
        juce::String text (TRANS("Speed"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (12.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MidiChordsEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    chordKeyboard->setBounds (8, 99, getWidth() - 16, 89);
    triggerKeyboard->setBounds (8, 227, getWidth() - 16, 89);
    guitar->setBounds (8, 99, getWidth() - 16, 89);
    infoBox->setBounds (proportionOfWidth (0.5000f) - (500 / 2), proportionOfHeight (0.5000f) - (300 / 2), 500, 300);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MidiChordsEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleButton.get())
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kGuess,toggleButton->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_toggleButton]
    }
    else if (buttonThatWasClicked == chordLearnButton.get())
    {
        //[UserButtonCode_chordLearnButton] -- add your button handler code here..
		if (getFilter()->getParameter(kLearnChord)>0)
			getFilter()->setParameterNotifyingHost(kLearnChord,0.f);
		else
			getFilter()->setParameterNotifyingHost(kLearnChord,1.f);
        //[/UserButtonCode_chordLearnButton]
    }
    else if (buttonThatWasClicked == triggerLearnButton.get())
    {
        //[UserButtonCode_triggerLearnButton] -- add your button handler code here..
		if (getFilter()->getParameter(kFollowInput)>0)
			getFilter()->setParameterNotifyingHost(kFollowInput,0.f);
		else
			getFilter()->setParameterNotifyingHost(kFollowInput,1.f);
        //[/UserButtonCode_triggerLearnButton]
    }
    else if (buttonThatWasClicked == clearChordButton.get())
    {
        //[UserButtonCode_clearChordButton] -- add your button handler code here..
		getFilter()->clearChord(getFilter()->getCurrentTrigger());
        //[/UserButtonCode_clearChordButton]
    }
    else if (buttonThatWasClicked == resetChordButton.get())
    {
        //[UserButtonCode_resetChordButton] -- add your button handler code here..
		getFilter()->resetChord(getFilter()->getCurrentTrigger());
        //[/UserButtonCode_resetChordButton]
    }
    else if (buttonThatWasClicked == clearAllButton.get())
    {
        //[UserButtonCode_clearAllButton] -- add your button handler code here..
		getFilter()->clearAllChords();
        //[/UserButtonCode_clearAllButton]
    }
    else if (buttonThatWasClicked == resetAllButton.get())
    {
        //[UserButtonCode_resetAllButton] -- add your button handler code here..
		getFilter()->resetAllChords();
        //[/UserButtonCode_resetAllButton]
    }
    else if (buttonThatWasClicked == transposeUpButton.get())
    {
        //[UserButtonCode_transposeUpButton] -- add your button handler code here..
		getFilter()->transposeAll(true);
        //[/UserButtonCode_transposeUpButton]
    }
    else if (buttonThatWasClicked == transposeDownButton.get())
    {
        //[UserButtonCode_transposeDownButton] -- add your button handler code here..
		getFilter()->transposeAll(false);
        //[/UserButtonCode_transposeDownButton]
    }
    else if (buttonThatWasClicked == transposeChordUpButton.get())
    {
        //[UserButtonCode_transposeChordUpButton] -- add your button handler code here..
		getFilter()->transposeChord(getFilter()->getCurrentTrigger(),true);
        //[/UserButtonCode_transposeChordUpButton]
    }
    else if (buttonThatWasClicked == transposeChordDownButton.get())
    {
        //[UserButtonCode_transposeChordDownButton] -- add your button handler code here..
		getFilter()->transposeChord(getFilter()->getCurrentTrigger(),false);
        //[/UserButtonCode_transposeChordDownButton]
    }
    else if (buttonThatWasClicked == normalButton.get())
    {
        //[UserButtonCode_normalButton] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kMode,((float)Normal)/(float)(numModes-1));
        //[/UserButtonCode_normalButton]
    }
    else if (buttonThatWasClicked == octaveButton.get())
    {
        //[UserButtonCode_octaveButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost(kMode,((float)Octave)/(float)(numModes-1));
        //[/UserButtonCode_octaveButton]
    }
    else if (buttonThatWasClicked == globalButton.get())
    {
        //[UserButtonCode_globalButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost(kMode,((float)Global)/(float)(numModes-1));
        //[/UserButtonCode_globalButton]
    }
    else if (buttonThatWasClicked == flatsButton.get())
    {
        //[UserButtonCode_flatsButton] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kFlats,flatsButton->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_flatsButton]
    }
    else if (buttonThatWasClicked == transposeChordUpButton2.get())
    {
        //[UserButtonCode_transposeChordUpButton2] -- add your button handler code here..
		getFilter()->transposeCurrentChordByOctave(true);
        //[/UserButtonCode_transposeChordUpButton2]
    }
    else if (buttonThatWasClicked == transposeChordDownButton2.get())
    {
        //[UserButtonCode_transposeChordDownButton2] -- add your button handler code here..
		getFilter()->transposeCurrentChordByOctave(false);
        //[/UserButtonCode_transposeChordDownButton2]
    }
    else if (buttonThatWasClicked == chordMenuButton.get())
    {
        //[UserButtonCode_chordMenuButton] -- add your button handler code here..
		StringArray list;
		listChordFiles(list);
		PopupMenu menu;
		menu.addCustomItem(-1,*chordSaveEditor,150,24,false);
		menu.addSeparator();
		for (int i=0;i<list.size();i++)
		{
			if (list[i].isNotEmpty())
			{
				if (list[i].startsWith("--"))
					menu.addSeparator();
				else if (list[i].startsWithChar('#'))
					menu.addSectionHeader(list[i].fromFirstOccurrenceOf("#",false,false));
				else
					menu.addItem(i+1,list[i].upToFirstOccurrenceOf(":",false,false));
			}
		}
		//menu.showMenuAsync (PopupMenu::Options().withTargetComponent (chordMenuButton),
		//	ModalCallbackFunction::forComponent (chordMenuCallback, this));
		int result = menu.showAt(chordMenuButton.get());

		if (result!=0)
		{
			if (result>0) {
				loadChord(list[result-1]);
			}
		}
        //[/UserButtonCode_chordMenuButton]
    }
    else if (buttonThatWasClicked == presetMenuButton.get())
    {
        //[UserButtonCode_presetMenuButton] -- add your button handler code here..
		//Array<File> list;
		//listPresetFiles(list);
		browser->refresh();

		PopupMenu menu;
		menu.addItem(-1,"Save...");
		menu.addSeparator();
		menu.addCustomItem(-2,*browser,250,300,false);
		//for (int i=0;i<list.size();i++)
		//{
		//	if (list[i].existsAsFile())
		//	{
		//		menu.addItem(i+1,list[i].getFileNameWithoutExtension());
		//	}
		//}
		int result = menu.showAt(presetMenuButton.get());
		if (result!=0)
		{
			//if (result>0)
			//	loadPreset(list[result-1]);
			if (result==-1)
				getFilter()->savePreset(presetNameLabel->getText());
		}
        //[/UserButtonCode_presetMenuButton]
    }
    else if (buttonThatWasClicked == copyButton.get())
    {
        //[UserButtonCode_copyButton] -- add your button handler code here..
		const int t = getFilter()->getCurrentTrigger();
		String chordString = String(t) + ":";
		for (int n=0;n<128;n++)
		{
			for (int c=1;c<=16;c++)
			{
				if (getFilter()->progKbState[getFilter()->getCurrentProgram()][t].isNoteOn(c,n))
					chordString += " " + String(n-t)+"."+String(c);
			}
		}
		SystemClipboard::copyTextToClipboard(chordString);
        //[/UserButtonCode_copyButton]
    }
    else if (buttonThatWasClicked == pasteButton.get())
    {
        //[UserButtonCode_pasteButton] -- add your button handler code here..
		chordFromString(SystemClipboard::getTextFromClipboard());
        //[/UserButtonCode_pasteButton]
    }
    else if (buttonThatWasClicked == pcButton.get())
    {
        //[UserButtonCode_pcButton] -- add your button handler code here..
		getFilter()->toggleUsePC(pcButton->getToggleState());
        //[/UserButtonCode_pcButton]
    }
    else if (buttonThatWasClicked == nextButton.get())
    {
        //[UserButtonCode_nextButton] -- add your button handler code here..
		getFilter()->selectTrigger(getFilter()->getCurrentTrigger()+1);
        //[/UserButtonCode_nextButton]
    }
    else if (buttonThatWasClicked == prevButton.get())
    {
        //[UserButtonCode_prevButton] -- add your button handler code here..
		getFilter()->selectTrigger(getFilter()->getCurrentTrigger()-1);
        //[/UserButtonCode_prevButton]
    }
    else if (buttonThatWasClicked == transposeInputButton.get())
    {
        //[UserButtonCode_transposeInputButton] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kInputTranspose,transposeInputButton->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_transposeInputButton]
    }
    else if (buttonThatWasClicked == toAllChannelsButton.get())
    {
        //[UserButtonCode_toAllChannelsButton] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kToAllChannels,toAllChannelsButton->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_toAllChannelsButton]
    }
    else if (buttonThatWasClicked == infoButton.get())
    {
        //[UserButtonCode_infoButton] -- add your button handler code here..
		infoBox->moveCaretToTop(false);
		if (infoButton->getToggleState())
			Desktop::getInstance().getAnimator().fadeOut(infoBox.get(),150);
		else
			Desktop::getInstance().getAnimator().fadeIn(infoBox.get(),100);
		//infoBox->setVisible(!infoBox->isVisible());
		infoButton->setToggleState(!infoButton->getToggleState(),dontSendNotification);
        //[/UserButtonCode_infoButton]
    }
    else if (buttonThatWasClicked == specialMenuButton.get())
    {
        //[UserButtonCode_specialMenuButton] -- add your button handler code here..
		PopupMenu m;
		m.addItem(1,"Clear all chords");
		m.addItem(2,"Reset all chords");
		m.addItem(3,"Copy to all triggers (absolute)");
		m.addItem(4,"Copy to all triggers (relative)");
		m.addItem(5,"Transpose all up one semitone");
		m.addItem(6,"Transpose all down one semitone");
		int result = m.show();//At(specialMenuButton);
		if (result!=0)
		{
			if (result==1)
				getFilter()->clearAllChords();
			else if (result==2)
				getFilter()->resetAllChords();
			else if (result==3)
				getFilter()->copyChordToAllTriggers(true);
			else if (result==4)
				getFilter()->copyChordToAllTriggers(false);
			else if (result==5)
				getFilter()->transposeAll(true);
			else if (result==6)
				getFilter()->transposeAll(false);
		}
        //[/UserButtonCode_specialMenuButton]
    }
    else if (buttonThatWasClicked == applyChannelButton.get())
    {
        //[UserButtonCode_applyChannelButton] -- add your button handler code here..
		getFilter()->applyChannelToChord();
        //[/UserButtonCode_applyChannelButton]
    }
    else if (buttonThatWasClicked == viewButton.get())
    {
        //[UserButtonCode_viewButton] -- add your button handler code here..
		setupButton->setEnabled(!guitar->isVisible());
		guitar->setVisible(!guitar->isVisible());
		getFilter()->setGuitarView(guitar->isVisible());
        //[/UserButtonCode_viewButton]
    }
    else if (buttonThatWasClicked == setupButton.get())
    {
        //[UserButtonCode_setupButton] -- add your button handler code here..
		getFilter()->fillGuitarPresetList();
		PopupMenu m, custom, chordfiles;
		for (int i=0;i<getFilter()->guitarPresets.size();i++)
			m.addItem(i+1,getFilter()->guitarPresets[i].guitarName,true,isGuitarPreset(i));

		m.addSeparator();
		custom.addCustomItem(-1,*tuningSaveEditor,160,18,false);
		custom.addSeparator();

		//File chordsPath(getFilter()->dataPath+File::getSeparatorString()+"chords");
		//Array<File> files;
		//chordsPath.findChildFiles(files,File::findFiles,true);
		//for (int i=0;i<files.size();i++)
		//{
		//	if (files[i].hasFileExtension("txt"))
		//		chordfiles.addItem(10000+i,files[i].getFileNameWithoutExtension(),true,
		//			getFilter()->guitarPresets[guitarPreset].chordFile==files[i].getFileName());
		//}
		//custom.addSubMenu("Chord File",chordfiles);

		custom.addCustomItem(-1,*fretsSlider,150,18,false);
		custom.addCustomItem(-1,*stringsSlider,150,18,false);
		for (int i=0;i<maxStrings;i++)
			custom.addCustomItem(-1,*stringSlider[i],150,18,false);
		m.addSubMenu("Customize",custom);

		int result = m.showAt(setupButton.get());
		if (result>0) {
			if (result>=10000) {
				;
			}
			else
				setUpGuitar(result-1);
		}
        //[/UserButtonCode_setupButton]
    }
    else if (buttonThatWasClicked == strumDirectionButton.get())
    {
        //[UserButtonCode_strumDirectionButton] -- add your button handler code here..
		getFilter()->setStrumDirection(!getFilter()->getStrumDirection());
        //[/UserButtonCode_strumDirectionButton]
    }
    else if (buttonThatWasClicked == strumButton.get())
    {
        //[UserButtonCode_strumButton] -- add your button handler code here..
		getFilter()->setParameterNotifyingHost(kStrum,strumButton->getToggleState() ? 0.f : 1.f);
        //[/UserButtonCode_strumButton]
    }

    //[UserbuttonClicked_Post]
    else if (buttonThatWasClicked == pizButton.get())
    {
		URL("http://thepiz.org/plugins/?p=midiChords").launchInDefaultBrowser();
	}
    //[/UserbuttonClicked_Post]
}

void MidiChordsEditor::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == channelSlider.get())
    {
        //[UserSliderCode_channelSlider] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kChannel,(float)channelSlider->getValue()/16.f);
        //[/UserSliderCode_channelSlider]
    }
    else if (sliderThatWasMoved == transposeSlider.get())
    {
        //[UserSliderCode_transposeSlider] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kTranspose,(float)(transposeSlider->getValue()+48)/96.f);
        //[/UserSliderCode_transposeSlider]
    }
    else if (sliderThatWasMoved == velocitySlider.get())
    {
        //[UserSliderCode_velocitySlider] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kVelocity,(float)(velocitySlider->getValue()-1)/126.f);
        //[/UserSliderCode_velocitySlider]
    }
    else if (sliderThatWasMoved == variationSlider.get())
    {
        //[UserSliderCode_variationSlider] -- add your slider handling code here..
        //[/UserSliderCode_variationSlider]
    }
    else if (sliderThatWasMoved == learnChanSlider.get())
    {
        //[UserSliderCode_learnChanSlider] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kLearnChannel,(float)learnChanSlider->getValue()/16.f);
        //[/UserSliderCode_learnChanSlider]
    }
    else if (sliderThatWasMoved == outputChannelSlider.get())
    {
        //[UserSliderCode_outputChannelSlider] -- add your slider handling code here..
		getFilter()->setParameterNotifyingHost(kOutChannel,(float)outputChannelSlider->getValue()/16.f);
        //[/UserSliderCode_outputChannelSlider]
    }
    else if (sliderThatWasMoved == maxTimeSlider.get())
    {
        //[UserSliderCode_maxTimeSlider] -- add your slider handling code here..
		maxTimeSlider->setVSTParam();
        //[/UserSliderCode_maxTimeSlider]
    }
    else if (sliderThatWasMoved == speedSlider.get())
    {
        //[UserSliderCode_speedSlider] -- add your slider handling code here..
		speedSlider->setVSTParam();
        //[/UserSliderCode_speedSlider]
    }
    else if (sliderThatWasMoved == accelSlider.get())
    {
        //[UserSliderCode_accelSlider] -- add your slider handling code here..
		accelSlider->setVSTParam();
        //[/UserSliderCode_accelSlider]
    }
    else if (sliderThatWasMoved == velRampSlider.get())
    {
        //[UserSliderCode_velRampSlider] -- add your slider handling code here..
		velRampSlider->setVSTParam();
        //[/UserSliderCode_velRampSlider]
    }

    //[UsersliderValueChanged_Post]
    else if (sliderThatWasMoved == fretsSlider)
    {
		getFilter()->setNumFrets(roundToInt(fretsSlider->getValue()),true);
		guitar->setNumFrets(roundToInt(fretsSlider->getValue()));
    }
    else if (sliderThatWasMoved == stringsSlider)
    {
		const int s = roundToInt(stringsSlider->getValue());
		getFilter()->setNumStrings(s,true);
		guitar->setNumStrings(s);
		for (int i=0;i<maxStrings;i++)
		{
			stringSlider[i]->setEnabled(i<s);
		}
    }
    else if (sliderThatWasMoved == stringSlider[0])
    {
		getFilter()->setStringValue(0,roundToInt(stringSlider[0]->getValue()),true);
		guitar->setStringNote(0,roundToInt(stringSlider[0]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[1])
    {
		getFilter()->setStringValue(1,roundToInt(stringSlider[1]->getValue()),true);
		guitar->setStringNote(1,roundToInt(stringSlider[1]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[2])
    {
		getFilter()->setStringValue(2,roundToInt(stringSlider[2]->getValue()),true);
		guitar->setStringNote(2,roundToInt(stringSlider[2]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[3])
    {
		getFilter()->setStringValue(3,roundToInt(stringSlider[3]->getValue()),true);
		guitar->setStringNote(3,roundToInt(stringSlider[3]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[4])
    {
		getFilter()->setStringValue(4,roundToInt(stringSlider[4]->getValue()),true);
		guitar->setStringNote(4,roundToInt(stringSlider[4]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[5])
    {
		getFilter()->setStringValue(5,roundToInt(stringSlider[5]->getValue()),true);
		guitar->setStringNote(5,roundToInt(stringSlider[5]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[6])
    {
		getFilter()->setStringValue(6,roundToInt(stringSlider[6]->getValue()),true);
		guitar->setStringNote(6,roundToInt(stringSlider[6]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[7])
    {
		getFilter()->setStringValue(7,roundToInt(stringSlider[7]->getValue()),true);
		guitar->setStringNote(7,roundToInt(stringSlider[7]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[8])
    {
		getFilter()->setStringValue(8,roundToInt(stringSlider[8]->getValue()),true);
		guitar->setStringNote(8,roundToInt(stringSlider[8]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[9])
    {
		getFilter()->setStringValue(9,roundToInt(stringSlider[9]->getValue()),true);
		guitar->setStringNote(9,roundToInt(stringSlider[9]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[10])
    {
		getFilter()->setStringValue(10,roundToInt(stringSlider[10]->getValue()),true);
		guitar->setStringNote(10,roundToInt(stringSlider[10]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[11])
    {
		getFilter()->setStringValue(11,roundToInt(stringSlider[11]->getValue()),true);
		guitar->setStringNote(11,roundToInt(stringSlider[11]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[12])
    {
		getFilter()->setStringValue(12,roundToInt(stringSlider[12]->getValue()),true);
		guitar->setStringNote(12,roundToInt(stringSlider[12]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[13])
    {
		getFilter()->setStringValue(13,roundToInt(stringSlider[13]->getValue()),true);
		guitar->setStringNote(13,roundToInt(stringSlider[13]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[14])
    {
		getFilter()->setStringValue(14,roundToInt(stringSlider[14]->getValue()),true);
		guitar->setStringNote(14,roundToInt(stringSlider[14]->getValue()));
    }
    else if (sliderThatWasMoved == stringSlider[15])
    {
		getFilter()->setStringValue(15,roundToInt(stringSlider[15]->getValue()),true);
		guitar->setStringNote(15,roundToInt(stringSlider[15]->getValue()));
    }
    //[/UsersliderValueChanged_Post]
}

void MidiChordsEditor::labelTextChanged (juce::Label* labelThatHasChanged)
{
    //[UserlabelTextChanged_Pre]
    //[/UserlabelTextChanged_Pre]

    if (labelThatHasChanged == presetNameLabel.get())
    {
        //[UserLabelCode_presetNameLabel] -- add your label text handling code here..
		getFilter()->changeProgramName(getFilter()->getCurrentProgram(),presetNameLabel->getText());
		getFilter()->updateHostDisplay();
        //[/UserLabelCode_presetNameLabel]
    }
    else if (labelThatHasChanged == chordEditor.get())
    {
        //[UserLabelCode_chordEditor] -- add your label text handling code here..
		chordFromString(chordEditor->getText());
        //[/UserLabelCode_chordEditor]
    }
    else if (labelThatHasChanged == triggerNoteLabel.get())
    {
        //[UserLabelCode_triggerNoteLabel] -- add your label text handling code here..
		getFilter()->selectTrigger(triggerNoteLabel->getText().getIntValue());
        //[/UserLabelCode_triggerNoteLabel]
    }

    //[UserlabelTextChanged_Post]
    //[/UserlabelTextChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MidiChordsEditor::chordMenuCallback (int result, MidiChordsEditor* editor)
{
	if (result!=0)
	{
		if (result>0) {
		//	loadChord(list[result-1]);
		}
	}
}

void MidiChordsEditor::mouseDown(const MouseEvent& e)
{
	if (e.eventComponent==previewButton.get() && !e.mods.isPopupMenu())
		getFilter()->playCurrentChord(true);
	if (e.eventComponent!=infoBox.get() && infoBox->isVisible()) {
		infoBox->setVisible(false);
		infoButton->setToggleState(false,dontSendNotification);
	}
}

void MidiChordsEditor::mouseDoubleClick (const MouseEvent& e)
{
	if (e.eventComponent==triggerKeyboard.get() && (e.mods.isShiftDown() || e.mods.isPopupMenu()))
	{
		for (int i=0;i<128;i++)
			getFilter()->setNoteBypassed(i,false);
		triggerKeyboard->repaint();
	}
}

void MidiChordsEditor::mouseUp (const MouseEvent& e)
{
	if (e.eventComponent==previewButton.get()) {
		if (e.mods.isPopupMenu())
		{
			if (getFilter()->isPreviewChordPlaying()) {
				getFilter()->playCurrentChord(false);
				previewButton->setToggleState(false,dontSendNotification);
			}
			else {
				getFilter()->playCurrentChord(true);
				previewButton->setToggleState(true,dontSendNotification);
			}
		}
		else {
			getFilter()->playCurrentChord(false);
			previewButton->setToggleState(false,dontSendNotification);
		}
	}
}

void MidiChordsEditor::chordFromString(String chordString)
{
	const int root = chordString.upToFirstOccurrenceOf(":",false,false).getIntValue();
	const int t = getFilter()->getCurrentTrigger();
	StringArray sa;
	if(chordString.contains(":"))
		chordString = chordString.fromLastOccurrenceOf(":",false,false);
	if (chordString.containsAnyOf("abcdefgABCDEFGmMrRopP#+")/* || !chordString.contains("0")*/)
		chordString = getIntervalStringFromNoteNames(t,chordString,getFilter()->bottomOctave);

	sa.addTokens(chordString," ,",String());
	if (sa.size()>0)
		getFilter()->clearChord(t);
	if (ModifierKeys::getCurrentModifiers().isCommandDown())
	{
		//absolute
		for(int i=0;i<sa.size();i++)
		{
			String s = sa[i].trim();
			if (s.isNotEmpty())
			{
				if(s.contains("."))
					getFilter()->selectChordNote(t,root+s.upToFirstOccurrenceOf(".",false,true).getIntValue(),true,s.fromFirstOccurrenceOf(".",false,true).getIntValue());
				else
					getFilter()->selectChordNote(t,root+s.getIntValue(),true);
			}
		}
	}
	else {
		//relative
		for(int i=0;i<sa.size();i++)
		{
			String s = sa[i].trim();
			if (s.isNotEmpty())
			{
				if(s.contains("."))
					getFilter()->selectChordNote(t,t+s.upToFirstOccurrenceOf(".",false,true).getIntValue(),true,s.fromFirstOccurrenceOf(".",false,true).getIntValue());
				else
					getFilter()->selectChordNote(t,t+s.getIntValue(),true);
			}
		}
	}
	if (guitar->isVisible())
			getFilter()->translateToGuitarChord();
}

void MidiChordsEditor::changeListenerCallback (ChangeBroadcaster* source)
{
	if (source==getFilter())
		updateParametersFromFilter();
}

void MidiChordsEditor::updateParametersFromFilter()
{
	MidiChords* const filter = getFilter();
	const int newMode = roundToInt(filter->getParameter(kMode)*(numModes-1));
	const int chordChan = roundToInt(filter->getParameter(kLearnChannel)*16.f);
	const int outChan = roundToInt(filter->getParameter(kOutChannel)*16.f);
	const int previewVel = roundToInt(filter->getParameter(kVelocity)*126.f)+1;
	const int transpose = roundToInt(filter->getParameter(kTranspose)*96.f)-48;
	const int s = filter->getNumStrings();
	const bool flats = filter->getParameter(kFlats)>0;
	const int p = filter->getCurrentProgram();

	guitar->setVisible(filter->getGuitarView());
	setupButton->setEnabled(filter->getGuitarView());
	fretsSlider->setValue(filter->getNumFrets(),dontSendNotification);
	guitar->setNumFrets(filter->getNumFrets());

	stringsSlider->setValue(s,dontSendNotification);
	guitar->setNumStrings(s);

	guitarPreset = -1;
	for (int i=0;i<filter->guitarPresets.size();i++) {
		if (isGuitarPreset(i))
		{
			guitarPreset = i;
			break;
		}
	}

	for (int i=0;i<maxStrings;i++)
	{
		stringSlider[i]->setEnabled(i<s);
		stringSlider[i]->setValue(filter->getStringValue(i),dontSendNotification);
		guitar->setStringNote(i,filter->getStringValue(i));
	}

	channelSlider->setValue(filter->getParameter(kChannel)*16.f,dontSendNotification);
	learnChanSlider->setValue(chordChan,dontSendNotification);
	outputChannelSlider->setValue(outChan,dontSendNotification);
	velocitySlider->setValue(previewVel,dontSendNotification);
	transposeSlider->setValue(transpose,dontSendNotification);
	chordLearnButton->setToggleState(filter->getParameter(kLearnChord)>0,dontSendNotification);
	triggerLearnButton->setToggleState(filter->getParameter(kFollowInput)>0,dontSendNotification);
	toggleButton->setToggleState(filter->getParameter(kGuess)>0,dontSendNotification);
	flatsButton->setToggleState(flats,dontSendNotification);
	guitar->setFlats(flats);
	pcButton->setToggleState(filter->getParameter(kUseProgCh)>0,dontSendNotification);
	transposeInputButton->setToggleState(filter->getParameter(kInputTranspose)>0,dontSendNotification);
	toAllChannelsButton->setToggleState(filter->getParameter(kToAllChannels)>0,dontSendNotification);

	accelSlider->setVSTSlider();
	velRampSlider->setVSTSlider();
	speedSlider->setVSTSlider();
	maxTimeSlider->setVSTSlider();
	strumButton->setToggleState(filter->getParameter(kStrum)>0,dontSendNotification);
	strumDirectionButton->setEnabled(strumButton->getToggleState());
	accelSlider->setEnabled(strumButton->getToggleState());
	velRampSlider->setEnabled(strumButton->getToggleState());
	speedSlider->setEnabled(strumButton->getToggleState());
	maxTimeSlider->setEnabled(strumButton->getToggleState());
	if (filter->getStrumDirection())
		strumDirectionButton->setButtonText("Up");
	else
		strumDirectionButton->setButtonText("Down");

	if (chordChan==0) chordKeyboard->setMidiChannelsToDisplay(0xffff);
	else chordKeyboard->setMidiChannelsToDisplay(1<<(chordChan-1));

	normalButton->setToggleState(newMode==Normal,dontSendNotification);
	octaveButton->setToggleState(newMode==Octave,dontSendNotification);
	globalButton->setToggleState(newMode==Global,dontSendNotification);

	chordNameLabel->setText(getCurrentChordName(),dontSendNotification);

	if (guitar->isVisible())
		guitar->handleAsyncUpdate();


	const int t = getFilter()->getCurrentTrigger();
	String chordString;// = String(t) + ":";
	for (int n=0;n<128;n++) {
		for (int c=1;c<=16;c++) {
			if (getFilter()->progKbState[p][t].isNoteOn(c,n))
				chordString += String(n-t)/*+"."+String(c)*/+" ";
		}
	}
	chordEditor->setText(chordString.trimEnd(),dontSendNotification);

	if (mode!=newMode)
	{
		if (newMode==Global) {
			triggerLabel->setText("Root Note:",dontSendNotification);
			triggerKeyboard->setKeyWidth(16.f);
			triggerKeyboard->setAvailableRange(0,127);
			triggerKeyboard->setLowestVisibleKey(36);
			triggerNoteLabel->setText(getNoteName(t,getFilter()->bottomOctave)+" ("+String(t)+")",dontSendNotification);
		}
		else if (newMode==Octave) {
			triggerLabel->setText("Trigger Note:",dontSendNotification);
			triggerKeyboard->setAvailableRange(60,71);
			triggerKeyboard->setKeyWidth((float)triggerKeyboard->getWidth()/7.f);
			triggerNoteLabel->setText(getNoteNameWithoutOctave(t,!flats),dontSendNotification);
		}
		else {
			triggerLabel->setText("Trigger Note:",dontSendNotification);
			triggerKeyboard->setKeyWidth(16.f);
			triggerKeyboard->setAvailableRange(0,127);
			triggerKeyboard->setLowestVisibleKey(36);
			triggerNoteLabel->setText(getNoteName(t,getFilter()->bottomOctave)+" ("+String(t)+")",dontSendNotification);
		}
		mode = newMode;
	}
	else {
		if (newMode!=Octave)
			triggerNoteLabel->setText(getNoteName(t,getFilter()->bottomOctave)+" ("+String(t)+")",dontSendNotification);
		else
			triggerNoteLabel->setText(getNoteNameWithoutOctave(t),dontSendNotification);
	}

	if (presetNameLabel->getText() != getFilter()->getProgramName(p))
		presetNameLabel->setText(getFilter()->getProgramName(p),dontSendNotification);

	triggerKeyboard->repaint();
	chordKeyboard->repaint();
}

String const MidiChordsEditor::getCurrentChordName()
{
	if (getFilter()->getParameter(kGuess)==0.f)
		return String();

	Array<int> chord;
	for (int n=0;n<128;n++)
	{
		for (int c=1;c<=16;c++)
		{
			if (getFilter()->progKbState[getFilter()->getCurrentProgram()][getFilter()->getCurrentTrigger()].isNoteOn(c,n))
				chord.add(n);
		}
	}
	return getFirstRecognizedChord(chord,getFilter()->getParameter(kFlats)>0.f);
}

void MidiChordsEditor::listChordFiles(StringArray &list)
{
	String chordPath = getFilter()->dataPath + File::getSeparatorString() + "chords";

	if (guitarPreset>=0 && guitar->isVisible())
		chordPath += File::getSeparatorString() + getFilter()->guitarPresets[guitarPreset].chordFile;
	else
		chordPath += File::getSeparatorString() + "Chords.txt";

	File chordFile = File(chordPath);
	if (!chordFile.exists())
		chordFile = getFilter()->dataPath + File::getSeparatorString() + "chords" + File::getSeparatorString() + "Chords.txt";

	StringArray s;
	s.addLines(chordFile.loadFileAsString());
	for (int line=0;line<s.size();line++)
	{
		if (!s[line].startsWithChar(';'))
			list.add(s[line]);
	}

	chordFile = getFilter()->dataPath + File::getSeparatorString() + "chords" + File::getSeparatorString() + "User.txt";
	s.clear();
	s.addLines(chordFile.loadFileAsString());
	for (int line=0;line<s.size();line++)
	{
		if (!s[line].startsWithChar(';'))
			list.add(s[line]);
	}
}

void MidiChordsEditor::listPresetFiles(Array<File> &list)
{
	File mappingsPath(getFilter()->dataPath+File::getSeparatorString()+"mappings");
	Array<File> files;
	mappingsPath.findChildFiles(files,File::findFiles,true);
	for (int i=0;i<files.size();i++)
	{
		if (files[i].hasFileExtension("chords") || files[i].hasFileExtension("fxp") || files[i].hasFileExtension("xml"))
			list.add(files[i]);
	}
}

void MidiChordsEditor::loadChord(String chorddef)
{
	const int t = getFilter()->getCurrentTrigger();
	getFilter()->clearChord(t);
	StringArray sa;
	sa.addTokens(chorddef.fromLastOccurrenceOf(":",false,true)," ",String());
	for(int i=0;i<sa.size();i++)
	{
		if (sa[i].trim().isNotEmpty())
		{
			if(sa[i].contains("."))
				getFilter()->selectChordNote(t,t+sa[i].upToFirstOccurrenceOf(".",false,true).getIntValue(),true,sa[i].fromFirstOccurrenceOf(".",false,true).getIntValue());
			else
				getFilter()->selectChordNote(t,t+sa[i].getIntValue(),true);
		}
	}
	if (guitar->isVisible())
		getFilter()->translateToGuitarChord();
}

void MidiChordsEditor::loadPreset(File file)
{
	if (file.hasFileExtension("fxp"))
		getFilter()->loadFxpFile(file);
	else if (file.hasFileExtension("fxb"))
		getFilter()->loadFxbFile(file);
	else if (file.hasFileExtension("xml"))
		getFilter()->readChorderPreset(file);
	else
	{
		//const int t = getFilter()->getCurrentTrigger();

		getFilter()->clearAllChords();

		StringArray lines;
		lines.addLines(file.loadFileAsString());
		for(int ln=0;ln<lines.size();ln++)
		{
			if (!lines[ln].startsWithChar(';'))
			{
				if (lines[ln].upToFirstOccurrenceOf(":",false,false).equalsIgnoreCase("Mode"))
				{
					String s = lines[ln].fromLastOccurrenceOf(":",false,true);
					if (s.equalsIgnoreCase("Full"))
						getFilter()->setParameterNotifyingHost(kMode,((float)Normal)/(float)(numModes-1));
					if (s.equalsIgnoreCase("Octave"))
						getFilter()->setParameterNotifyingHost(kMode,((float)Octave)/(float)(numModes-1));
					if (s.equalsIgnoreCase("Global"))
						getFilter()->setParameterNotifyingHost(kMode,((float)Global)/(float)(numModes-1));
				}
				else
				{
					int t = lines[ln].upToFirstOccurrenceOf(":",false,false).getIntValue();
					//getFilter()->clearChord(t);
					StringArray sa;
					sa.addTokens(lines[ln].fromLastOccurrenceOf(":",false,true)," ",String());
					for(int i=0;i<sa.size();i++)
					{
						if(sa[i].trim().isNotEmpty())
						{
							if(sa[i].contains("."))
								getFilter()->selectChordNote(t,t+sa[i].upToFirstOccurrenceOf(".",false,true).getIntValue(),true,sa[i].fromFirstOccurrenceOf(".",false,true).getIntValue());
							else
								getFilter()->selectChordNote(t,t+sa[i].getIntValue(),true);
						}
					}
				}
			}
		}
		presetNameLabel->setText(file.getFileNameWithoutExtension(),dontSendNotification);
		getFilter()->changeProgramName(getFilter()->getCurrentProgram(),file.getFileNameWithoutExtension());
		getFilter()->updateHostDisplay();
	}
}

void MidiChordsEditor::saveChord(String name)
{
	const int curProgram = getFilter()->getCurrentProgram();
	const int t = getFilter()->getCurrentTrigger();
	String chordString = name + ":";
	for (int c=1;c<=16;c++) {
		for (int n=0;n<128;n++)
		{
			if (getFilter()->progKbState[curProgram][t].isNoteOn(c,n))
				chordString += " " + String(n-t) + "." + String(c);
		}
	}
	File chordFile(getFilter()->dataPath+File::getSeparatorString()+"chords"+File::getSeparatorString()+"User.txt");
	if (chordFile.create())
		chordFile.appendText(chordString+"\n");
}

void MidiChordsEditor::textEditorTextChanged(TextEditor &editor) {

}

void MidiChordsEditor::textEditorReturnKeyPressed(TextEditor &editor) {
	if (&editor==chordSaveEditor.get()) {
		saveChord(chordSaveEditor->getText());
		PopupMenu::dismissAllActiveMenus();
		chordSaveEditor->clear();
	}
	else if (&editor==tuningSaveEditor.get())
	{
		String tuningString="\""+tuningSaveEditor->getText()+"\", "
			+ String("Chords.txt, ")
			+ String(roundToInt(fretsSlider->getValue()))+" frets, ";
		for (int i=0;i<roundToInt(stringsSlider->getValue());i++)
		{
			if (i>0) tuningString += ",";
			tuningString += String(roundToInt(stringSlider[i]->getValue()));
		}
		File tuningFile(getFilter()->dataPath+File::getSeparatorString()+"guitars"+File::getSeparatorString()+"GuitarPresets.txt");
		if (tuningFile.create())
			tuningFile.appendText(tuningString+"\n");

		getFilter()->fillGuitarPresetList();
		PopupMenu::dismissAllActiveMenus();
		tuningSaveEditor->clear();
	}
}

void MidiChordsEditor::textEditorEscapeKeyPressed(TextEditor &editor) {
	PopupMenu::dismissAllActiveMenus();
}

void MidiChordsEditor::textEditorFocusLost(TextEditor &editor) {

}

void MidiChordsEditor::filesDropped (const StringArray& filenames, int mouseX, int mouseY)
{
	File file = File(filenames[0]);
	if ( file.hasFileExtension("chords") || file.hasFileExtension("fxp") || file.hasFileExtension("fxb")
		|| file.hasFileExtension("xml"))
		loadPreset(file);
	else if (file.getFileName() == "midiChordsKey.txt" || file.getFileName() == "midiChordsKey.zip") {
		getFilter()->readKeyFile(File(filenames[0]));
		if (!getFilter()->demo) {
			demoLabel->setVisible(false);
		}
	}
}

bool MidiChordsEditor::isInterestedInFileDrag (const StringArray& files){
    File file = File(files[0]);
	if ( file.hasFileExtension("chords") || file.hasFileExtension("fxp") || file.hasFileExtension("fxb") || file.hasFileExtension("xml"))
		return true;
	if ( file.getFileName() == "midiChordsKey.txt" || file.getFileName() == "midiChordsKey.zip" )
		return true;
    return false;
}

void MidiChordsEditor::fileClicked(const juce::File &file,const juce::MouseEvent &)
{
	if (file.existsAsFile())
		loadPreset(file);
}

void MidiChordsEditor::fileDoubleClicked (const File &file)
{
	if (file.existsAsFile())
	{
		//loadPreset(file);
		PopupMenu::dismissAllActiveMenus();
	}
}

void MidiChordsEditor::browserRootChanged (const File& newRoot)
{

}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiChordsEditor" componentName=""
                 parentClasses="public juce::AudioProcessorEditor, public juce::ChangeListener, public juce::TextEditor::Listener, public juce::FileDragAndDropTarget, public juce::FileBrowserListener"
                 constructorParams="MidiChords* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="0" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="640" initialHeight="420">
  <BACKGROUND backgroundColour="ffd8d8d8">
    <RECT pos="0 0 640 100" fill="linear: 61 -31, 61 23, 0=ffffffff, 1=e7e7e7"
          hasStroke="0"/>
    <IMAGE pos="113 7 234 45" resource="midichordsLogo_png" opacity="1.0"
           mode="1"/>
    <RECT pos="0 348 100% 72" fill="linear: 0 331, 0 360, 0=ff000000, 1=ff828282"
          hasStroke="0"/>
    <RECT pos="196 323 150 92" fill="solid: a52a85" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="linear: 230 323, 230 377, 0=ff0000, 1=ff000000"/>
    <ROUNDRECT pos="568 76 66 106" cornerSize="10.0" fill="solid: ff000000"
               hasStroke="0"/>
    <TEXT pos="14 319 103 24" fill="solid: ff000000" hasStroke="0" text="Trigger Channel:"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="330 318 126 24" fill="solid: ff000000" hasStroke="0" text="Trigger Mode:"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <RECT pos="6 225 12M 93" fill="solid: ff000000" hasStroke="0"/>
    <RECT pos="6 97 12M 93" fill="solid: ff000000" hasStroke="0"/>
    <ROUNDRECT pos="462 202 172 36" cornerSize="10.0" fill="solid: ff000000"
               hasStroke="0"/>
    <ROUNDRECT pos="250 195 173 26" cornerSize="12.0" fill="linear: 248 183, 248 215, 0=ff666666, 1=ffbfbfbf"
               hasStroke="0"/>
    <TEXT pos="6 46 71 8" fill="solid: ff000000" hasStroke="0" text="Input Channel"
          fontname="Default font" fontsize="10.7" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="219 328 53 16" fill="solid: ff000000" hasStroke="0" text="Strum:"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="204 351 62 16" fill="solid: ff000000" hasStroke="0" text="Max Time"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="277 351 62 16" fill="solid: ff000000" hasStroke="0" text="Vel. Ramp"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="277 380 62 16" fill="solid: ff000000" hasStroke="0" text="Accel"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="204 380 62 16" fill="solid: ff000000" hasStroke="0" text="Speed"
          fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
  </BACKGROUND>
  <TOGGLEBUTTON name="new toggle button" id="58723bf0e9d70b49" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="372 50 150 24" buttonText="Guess chord name"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="new label" id="84701c0c650fe2e" memberName="chordNameLabel"
         virtualName="" explicitFocusOrder="0" pos="375 72 144 26" bkgCol="279a3c3c"
         outlineCol="b3000000" edTextCol="ff000000" edBkgCol="0" labelText="chord name"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
         italic="0" justification="36" typefaceStyle="Bold"/>
  <GENERICCOMPONENT name="" id="8e79ee27297830eb" memberName="chordKeyboard" virtualName=""
                    explicitFocusOrder="0" pos="8 99 16M 89" class="ChordsKeyboardComponent"
                    params="ownerFilter-&gt;chordKbState, ownerFilter"/>
  <GENERICCOMPONENT name="" id="c10b054d27a5aad0" memberName="triggerKeyboard" virtualName=""
                    explicitFocusOrder="0" pos="8 227 16M 89" class="TriggerKeySelectorKeyboard"
                    params="ownerFilter-&gt;triggerKbState, ownerFilter"/>
  <TEXTBUTTON name="chordLearn" id="79a1c0e1c1db8e39" memberName="chordLearnButton"
              virtualName="" explicitFocusOrder="0" pos="6 76 45 21" tooltip="Learn next input chord"
              buttonText="Learn" connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="triggerLearn" id="99ff483968f1f65d" memberName="triggerLearnButton"
              virtualName="" explicitFocusOrder="0" pos="6 204 80 21" tooltip="When on, displayed trigger note follows input note"
              buttonText="Follow Input" connectedEdges="8" needsCallback="1"
              radioGroupId="0"/>
  <SLIDER name="channel" id="c08fa2a5fc963392" memberName="channelSlider"
          virtualName="ChannelSlider" explicitFocusOrder="0" pos="123 323 66 16"
          tooltip="Trigger channel" bkgcol="ffffffff" min="0.0" max="16.0"
          int="1.0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="53b1a84fa5d6099" memberName="outputLabel"
         virtualName="" explicitFocusOrder="0" pos="573 76 54 24" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Output" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <JUCERCOMP name="" id="bad9b853cca3ec4a" memberName="pizButton" virtualName=""
             explicitFocusOrder="0" pos="38 3 74 40" sourceFile="../_common/PizButton.cpp"
             constructorParams=""/>
  <LABEL name="new label" id="9c19fc858eab3133" memberName="triggerLabel"
         virtualName="" explicitFocusOrder="0" pos="464 203 97 24" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Trigger Note:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="34"/>
  <TEXTBUTTON name="clear" id="729ec8a8b3111b3" memberName="clearChordButton"
              virtualName="" explicitFocusOrder="0" pos="291 76 40 21" tooltip="Clear current chord"
              buttonText="Clear" connectedEdges="10" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="reset" id="706edfe98019c25d" memberName="resetChordButton"
              virtualName="" explicitFocusOrder="0" pos="331 76 40 21" tooltip="Reset current chord to just the trigger note"
              buttonText="Reset" connectedEdges="9" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="clear" id="ee6085145dcb39d2" memberName="clearAllButton"
              virtualName="" explicitFocusOrder="0" pos="8 429 64 24" buttonText="Clear All"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="reset" id="d63567d98d87183b" memberName="resetAllButton"
              virtualName="" explicitFocusOrder="0" pos="69 441 64 24" buttonText="Reset All"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="transpose" id="f59df0e454cbaec8" memberName="transposeUpButton"
              virtualName="" explicitFocusOrder="0" pos="172 425 32 24" buttonText="-&gt;"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="transpose" id="3fe107b4cf733d10" memberName="transposeDownButton"
              virtualName="" explicitFocusOrder="0" pos="143 425 29 24" buttonText="&lt;-"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="transpose" id="b392a950edbb9665" memberName="transposeChordUpButton"
              virtualName="" explicitFocusOrder="0" pos="159 76 23 21" tooltip="Shift chord up one semitone"
              buttonText="&gt;" connectedEdges="11" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="transpose" id="330f12de98d7240c" memberName="transposeChordDownButton"
              virtualName="" explicitFocusOrder="0" pos="136 76 23 21" tooltip="Shift chord down one semitone"
              buttonText="&lt;" connectedEdges="11" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="transpose" id="e04428ac0ed97dc9" memberName="transposeSlider"
          virtualName="" explicitFocusOrder="0" pos="475 372 66 16" tooltip="Transpose output by semitones"
          bkgcol="ffffffff" min="-12.0" max="12.0" int="1.0" style="LinearBar"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="velocity" id="d173a9d6025fabc0" memberName="velocitySlider"
          virtualName="" explicitFocusOrder="0" pos="397 197 22 22" tooltip="Velocity for &quot;Play Chord&quot; button"
          rotarysliderfill="7f000000" min="1.0" max="127.0" int="1.0" style="RotaryVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="Variation" id="992cbb80661db8e9" memberName="variationSlider"
          virtualName="" explicitFocusOrder="0" pos="333 432 104 16" min="0.0"
          max="100.0" int="0.1" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TEXTBUTTON name="new button" id="ea513687ce233abe" memberName="normalButton"
              virtualName="" explicitFocusOrder="0" pos="442 318 64 24" tooltip="Full mode: a separate chord is defined for every input note"
              buttonText="Full" connectedEdges="6" needsCallback="1" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="5d77a8c6aac13eed" memberName="octaveButton"
              virtualName="" explicitFocusOrder="0" pos="506 318 64 24" tooltip="Octave mode: 12 chords are defined which are transposed to every octave"
              buttonText="Octave" connectedEdges="7" needsCallback="1" radioGroupId="1"/>
  <TEXTBUTTON name="new button" id="4ec8d27f5555ec05" memberName="globalButton"
              virtualName="" explicitFocusOrder="0" pos="570 318 64 24" tooltip="Global mode: one defined chord is transposed relative to the root note"
              buttonText="Global" connectedEdges="5" needsCallback="1" radioGroupId="1"/>
  <TOGGLEBUTTON name="new toggle button" id="fb8f2d76e48f6291" memberName="flatsButton"
                virtualName="" explicitFocusOrder="0" pos="519 76 48 24" buttonText="flats"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="transpose" id="9c9792c55f67c803" memberName="transposeChordUpButton2"
              virtualName="" explicitFocusOrder="0" pos="182 76 25 21" tooltip="Shift chord up one octave"
              buttonText="&gt;&gt;" connectedEdges="9" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="transpose" id="1f97de47eccc1b3c" memberName="transposeChordDownButton2"
              virtualName="" explicitFocusOrder="0" pos="111 76 25 21" tooltip="Shift chord down one octave"
              buttonText="&lt;&lt;" connectedEdges="10" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="chordMenu" id="384cb651e909e8bd" memberName="chordMenuButton"
              virtualName="" explicitFocusOrder="0" pos="55 76 52 21" buttonText="Menu"
              connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="4ef6103879c88f80" memberName="presetNameLabel"
         virtualName="" explicitFocusOrder="0" pos="414 12 191 26" tooltip="Preset Name (double-click to edit)"
         bkgCol="27a1b404" outlineCol="b3000000" edTextCol="ff000000"
         edBkgCol="0" labelText="preset name" editableSingleClick="1"
         editableDoubleClick="1" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="1" italic="0" justification="36"
         typefaceStyle="Bold"/>
  <TEXTBUTTON name="presets" id="fdd2c5f83e8c75c9" memberName="presetMenuButton"
              virtualName="" explicitFocusOrder="0" pos="605 12 25 26" tooltip="Preset Menu"
              buttonText="&gt;" connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="fbf538174362a2ae" memberName="chordSaveEditor"
              virtualName="" explicitFocusOrder="0" pos="82 -35 150 24" outlinecol="ff000000"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TEXTBUTTON name="copy" id="7eadb69efd0e274e" memberName="copyButton" virtualName=""
              explicitFocusOrder="0" pos="211 73 38 21" buttonText="Copy" connectedEdges="6"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="paste" id="c6429d9f9bdc84d1" memberName="pasteButton" virtualName=""
              explicitFocusOrder="0" pos="249 73 38 21" buttonText="Paste"
              connectedEdges="5" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="play" id="92127eade5a319e5" memberName="previewButton"
              virtualName="" explicitFocusOrder="0" pos="251 196 143 24" tooltip="Plays currently displayed chord"
              bgColOff="ff7ca17c" buttonText="Play Chord" connectedEdges="0"
              needsCallback="0" radioGroupId="0"/>
  <LABEL name="new label" id="5bdae6a44eaf90fa" memberName="chordEditor"
         virtualName="" explicitFocusOrder="0" pos="178 54 144 20" tooltip="Double-click to type a chord"
         bkgCol="ffffffff" outlineCol="b3000000" edTextCol="ff000000"
         edBkgCol="0" labelText="chord" editableSingleClick="0" editableDoubleClick="1"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <TOGGLEBUTTON name="new toggle button" id="985f12fa947001cc" memberName="pcButton"
                virtualName="" explicitFocusOrder="0" pos="500 397 134 20" tooltip="Change program when receiving MIDI Program Change"
                buttonText="Use Program Chg" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <TEXTBUTTON name="next" id="112bbd95115b0fee" memberName="nextButton" virtualName=""
              explicitFocusOrder="0" pos="113 204 23 21" tooltip="Select next higher trigger note"
              buttonText="&gt;" connectedEdges="9" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="prev" id="fec45c7b44c486c2" memberName="prevButton" virtualName=""
              explicitFocusOrder="0" pos="90 204 23 21" tooltip="Select previous trigger note"
              buttonText="&lt;" connectedEdges="10" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="f9ca130084b6577" memberName="triggerNoteLabel"
         virtualName="" explicitFocusOrder="0" pos="558 203 73 24" tooltip="Currently displayed trigger note (double-click to edit)"
         textCol="ffffffff" edTextCol="ff000000" edBkgCol="ffffffff" labelText="G8 (127)"
         editableSingleClick="0" editableDoubleClick="1" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
         italic="0" justification="33" typefaceStyle="Bold"/>
  <SLIDER name="channel" id="51b00c0c435d1b05" memberName="learnChanSlider"
          virtualName="ChannelSlider" explicitFocusOrder="0" pos="6 56 38 16"
          tooltip="Chord Input Channel, used for Learn and left-click entering notes"
          bkgcol="ffffffff" min="0.0" max="16.0" int="1.0" style="LinearBar"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="b13bd78310d4145" memberName="demoLabel"
         virtualName="" explicitFocusOrder="0" pos="318 5 97 24" edTextCol="ff000000"
         edBkgCol="0" labelText="UNREGISTERED&#10;DEMO VERSION" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="OCR A Extended"
         fontsize="10.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="new component" id="fe343618ae664d49" memberName="guitar"
                    virtualName="" explicitFocusOrder="0" pos="8 99 16M 89" class="ChordsGuitar"
                    params="ownerFilter-&gt;chordKbState, ownerFilter"/>
  <LABEL name="new label" id="9ec8a16d4577cd67" memberName="versionLabel"
         virtualName="" explicitFocusOrder="0" pos="352 28 58 24" edTextCol="ff000000"
         edBkgCol="0" labelText="99.99.99" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="new toggle button" id="d3b81c8771f45be2" memberName="transposeInputButton"
                virtualName="" explicitFocusOrder="0" pos="545 370 91 21" tooltip="When checked, incoming trigger notes are transposed by the opposite of the &quot;Transpose&quot; slider."
                buttonText="Also Transpose Input" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="b5393aba43fddbf9" memberName="toAllChannelsButton"
                virtualName="" explicitFocusOrder="0" pos="364 397 129 20" tooltip="When checked, CCs (and other control messages) are sent to all MIDI channels. Otherwise they are passed through on the original channel."
                buttonText="CCs to All Chans" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <TEXTBUTTON name="new button" id="57021a22a2d1dc95" memberName="infoButton"
              virtualName="" explicitFocusOrder="0" pos="-1 -1 19 18" buttonText="?"
              connectedEdges="5" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="cca12d6113282f95" memberName="specialMenuButton"
              virtualName="" explicitFocusOrder="0" pos="6 357 140 24" buttonText="Global Functions..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="channel" id="4905788d8bca0925" memberName="outputChannelSlider"
          virtualName="ChannelSlider" explicitFocusOrder="0" pos="381 372 66 16"
          tooltip="Output channel (&quot;Multi&quot; is as saved, otherwise overrides chord's saved channel)"
          bkgcol="ffffffff" min="0.0" max="16.0" int="1.0" style="LinearBar"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TEXTBUTTON name="new button" id="8efd6a9fa5389ff6" memberName="applyChannelButton"
              virtualName="" explicitFocusOrder="0" pos="44 56 32 16" tooltip="Applies input channel to selected chord"
              buttonText="Apply" connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="17e9cd77311da5c7" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="368 357 91 13" edTextCol="ff000000"
         edBkgCol="0" labelText="Out Channel" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="beb727c502d48577" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="461 357 91 13" edTextCol="ff000000"
         edBkgCol="0" labelText="Transpose" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="view" id="d372ec945cfb2f67" memberName="viewButton" virtualName=""
              explicitFocusOrder="0" pos="151 190 41 21" tooltip="Switch between keyboard &amp; guitar views"
              buttonText="View" connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="1079b29645f4ab3e" memberName="setupButton"
              virtualName="" explicitFocusOrder="0" pos="195 190 45 21" buttonText="Setup"
              connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3afc44df79bf17dc" memberName="strumDirectionButton"
              virtualName="" explicitFocusOrder="0" pos="267 327 48 19" tooltip="Strum Direction"
              buttonText="Down" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="d7138ce195ecfe1d" memberName="strumButton"
              virtualName="" explicitFocusOrder="0" pos="142 391 54 21" buttonText="Strum"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="maxTime" id="f7231d7c45deba2d" memberName="maxTimeSlider"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="202 364 66 16"
          tooltip="Max Strum Time" bkgcol="ffffffff" min="100.0" max="3000.0"
          int="1.0" style="LinearBar" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="speed" id="a243163cabbb95ac" memberName="speedSlider" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="202 393 66 16" tooltip="Strum Speed"
          bkgcol="ffffffff" min="0.0" max="100.0" int="1.0" style="LinearBar"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="accel" id="5cdb7ff5a14641ba" memberName="accelSlider" virtualName="VSTSlider"
          explicitFocusOrder="0" pos="275 393 66 16" tooltip="Strum Acceleration"
          bkgcol="ffffffff" min="-100.0" max="100.0" int="1.0" style="LinearBar"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="velRamp" id="5280e3c28d4cfc6a" memberName="velRampSlider"
          virtualName="VSTSlider" explicitFocusOrder="0" pos="275 364 66 16"
          tooltip="Strum Velocity Ramp" bkgcol="ffffffff" min="-100.0"
          max="100.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TEXTEDITOR name="new text editor" id="2319ccc237bcf9fc" memberName="infoBox"
              virtualName="" explicitFocusOrder="0" pos="50%c 50%c 500 300"
              bkgcol="f2ffffff" outlinecol="ff000000" shadowcol="38000000"
              initialText="" multiline="1" retKeyStartsLine="0" readonly="1"
              scrollbars="1" caret="0" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="663ee95273f8530a" memberName="tuningSaveEditor"
              virtualName="" explicitFocusOrder="0" pos="253 -31 150 24" outlinecol="ff000000"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: midichordsLogo_png, 14039, "../../../../../Pictures/midichords logo.png"
static const unsigned char resource_MidiChordsEditor_midichordsLogo_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,2,219,0,0,0,128,8,6,0,0,0,218,166,165,181,0,0,0,4,115,66,73,84,8,8,8,8,124,
8,100,136,0,0,0,9,112,72,89,115,0,0,15,95,0,0,15,95,1,215,138,218,201,0,0,0,25,116,69,88,116,83,111,102,116,119,97,114,101,0,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,155,238,60,26,0,
0,32,0,73,68,65,84,120,156,237,157,119,184,92,85,213,135,223,149,64,168,161,5,136,116,16,1,65,144,26,4,169,10,72,111,65,20,1,21,252,252,64,64,148,34,160,32,82,252,80,148,222,4,5,84,68,65,144,162,116,16,
16,18,164,247,42,189,26,122,175,9,73,126,223,31,251,92,157,204,157,185,179,207,204,105,51,179,222,231,57,79,50,115,119,89,103,207,41,107,175,189,246,90,38,9,39,31,204,108,58,224,112,96,83,96,246,12,154,
124,18,56,69,210,69,25,180,229,56,61,141,153,29,0,236,214,102,245,15,128,103,128,167,129,51,36,221,147,149,92,0,102,182,24,240,51,96,85,96,24,48,25,184,9,216,95,210,171,89,246,213,175,152,153,1,123,3,
99,129,71,128,227,36,61,220,70,59,203,3,223,7,150,0,206,147,116,114,166,130,58,142,211,243,152,43,219,249,97,102,91,2,127,205,184,217,55,129,5,36,125,152,113,187,142,211,83,152,217,145,192,1,25,52,53,
5,56,25,248,177,164,247,50,104,15,51,59,10,248,65,131,63,237,35,233,184,44,250,232,119,204,236,115,192,173,53,95,253,85,210,214,109,180,115,37,176,81,205,87,43,72,186,175,83,249,156,222,198,204,214,32,
76,246,222,1,126,39,105,124,201,34,57,37,50,172,108,1,122,156,207,229,208,230,156,192,210,57,180,235,56,78,99,134,19,44,155,103,101,216,230,234,41,191,119,210,179,118,221,231,47,148,220,142,211,39,152,
217,92,192,181,192,54,192,206,192,56,51,251,83,242,189,211,135,184,178,157,47,231,231,208,230,191,36,221,157,67,187,78,159,96,102,27,155,217,15,205,108,76,217,178,116,25,99,205,108,155,156,251,176,156,
219,239,39,166,111,241,185,232,118,156,254,97,115,96,198,186,239,182,7,30,54,179,173,74,144,199,41,153,233,202,22,160,151,145,116,111,178,148,57,148,207,246,183,128,145,53,159,239,1,198,53,41,251,36,112,
97,118,18,58,253,70,157,107,211,36,51,91,86,210,227,101,202,84,32,31,0,167,71,150,157,11,88,14,88,161,238,251,83,204,236,114,73,31,117,40,203,115,192,26,13,190,127,182,195,118,29,199,41,159,59,155,124,
63,26,184,216,204,206,5,246,148,244,122,129,50,57,37,226,202,118,206,72,186,29,184,189,217,223,205,108,107,166,85,182,111,144,180,79,238,130,57,253,202,90,53,255,31,1,172,6,244,139,178,253,142,164,189,
210,84,48,179,227,128,218,58,163,129,69,128,71,59,148,229,76,194,36,124,182,154,239,94,7,254,208,97,187,142,227,148,140,164,135,204,108,55,194,94,143,225,13,138,124,13,248,162,153,237,38,233,226,98,165,
115,202,192,221,72,28,167,191,88,170,197,103,103,90,206,104,240,221,130,157,54,42,233,58,96,110,224,179,192,138,192,178,192,104,73,247,119,218,182,227,56,229,35,233,52,194,30,140,102,17,112,70,3,23,153,
217,185,102,54,170,56,201,156,50,112,101,219,113,250,139,250,213,44,95,221,26,154,127,55,248,110,129,44,26,150,244,177,164,7,36,221,43,233,33,73,83,178,104,215,113,156,106,32,233,14,96,37,224,231,132,
168,70,141,216,142,224,203,61,182,48,193,156,194,113,101,219,113,28,167,57,141,124,179,103,42,92,10,199,113,186,18,73,19,37,29,72,112,217,123,176,73,177,121,129,11,205,236,207,102,54,119,113,210,57,69,
225,202,182,227,56,142,227,56,78,142,72,186,19,88,25,56,130,144,196,170,17,95,5,30,42,32,234,145,83,48,190,132,220,7,152,217,194,192,116,146,158,26,162,204,236,132,232,11,3,215,196,68,66,152,193,55,115,
144,103,62,96,113,154,95,127,83,129,55,128,151,129,215,37,77,205,65,134,217,128,133,8,126,179,181,225,214,222,7,94,0,94,206,170,95,51,91,0,152,121,168,168,31,102,54,51,193,119,119,32,172,216,84,224,9,
73,19,50,232,127,24,33,54,251,60,132,40,27,181,44,108,102,235,54,169,58,133,112,13,120,70,195,12,72,50,26,126,10,152,159,105,175,185,137,192,253,146,222,207,185,255,133,129,197,72,31,94,80,192,179,146,
158,201,92,168,62,197,204,102,33,248,254,143,102,90,163,215,71,4,215,165,9,89,185,21,37,150,210,209,192,195,106,146,197,206,204,70,16,220,29,6,194,213,229,250,155,39,207,187,133,8,22,221,218,13,132,19,
9,207,223,23,37,53,83,72,187,22,73,147,128,31,155,217,69,192,239,9,239,220,122,230,5,46,48,179,243,128,239,74,122,173,64,17,157,188,144,228,71,137,7,33,212,151,106,142,99,51,110,255,136,154,182,143,111,
240,247,77,128,219,8,138,149,26,28,255,2,54,204,64,142,153,128,67,8,17,23,26,245,211,236,120,9,216,21,24,158,209,120,44,12,92,26,209,239,187,192,129,192,12,29,246,119,96,139,241,95,4,184,24,248,176,137,
28,47,3,7,19,38,75,237,244,191,47,240,94,202,49,111,116,13,44,94,246,189,210,198,185,31,89,119,30,47,182,209,198,140,13,198,99,215,54,218,217,14,120,107,136,49,254,24,184,43,143,113,6,214,5,94,236,240,
26,16,33,125,253,198,101,255,174,41,206,251,192,58,249,223,111,179,157,73,117,237,236,215,129,76,163,128,223,69,140,245,68,224,24,96,182,14,199,96,7,194,196,93,192,223,154,200,243,123,66,150,195,70,114,
188,1,156,64,48,22,100,241,155,204,73,8,191,57,181,197,249,79,74,250,157,163,236,235,40,199,235,115,4,112,120,114,239,55,27,135,151,129,47,151,45,171,31,25,252,222,101,11,208,239,7,249,43,219,181,237,
191,85,243,253,194,192,149,41,94,180,191,236,64,134,57,8,161,210,58,121,209,223,3,44,212,225,88,236,70,122,197,243,9,96,149,14,250,124,184,166,173,119,128,97,53,127,219,30,120,59,82,142,59,218,121,241,
2,111,118,56,238,3,199,17,101,223,43,109,156,123,149,148,237,251,35,199,249,231,57,140,195,223,50,186,6,4,92,89,246,239,154,226,188,43,165,108,3,99,129,87,83,142,247,75,192,70,29,140,193,21,117,237,205,
83,243,183,245,129,9,145,114,60,1,44,210,225,239,177,121,114,62,105,206,255,21,96,243,178,175,165,156,175,211,21,129,123,91,140,195,121,181,191,157,31,221,119,184,207,118,15,99,102,51,50,109,152,178,217,
205,108,129,36,254,231,131,192,70,41,154,219,207,204,254,167,77,81,206,0,150,108,179,238,0,43,0,215,37,75,241,169,48,179,97,102,118,50,240,43,96,150,148,213,23,7,110,50,179,29,219,233,23,248,100,205,87,
35,129,165,18,121,142,2,254,196,180,113,150,135,98,21,130,165,43,45,239,180,81,167,17,185,186,56,244,1,111,69,150,235,56,172,96,3,178,186,6,0,150,207,176,173,190,193,204,126,8,92,64,112,91,75,195,104,
224,114,51,59,160,205,174,63,85,247,121,149,68,158,189,128,171,128,249,34,219,89,156,248,132,80,131,48,179,189,9,201,180,70,167,172,58,15,240,55,51,251,73,187,125,87,29,73,247,0,99,128,195,8,86,238,70,
124,133,224,203,189,109,97,130,57,153,226,202,118,111,51,27,131,127,227,107,8,74,231,200,193,197,91,114,80,162,64,70,147,248,103,103,181,217,99,9,130,21,32,45,7,2,123,116,208,239,12,192,111,147,108,160,
105,152,41,169,91,203,252,4,55,150,31,180,33,199,183,205,108,157,148,117,254,7,24,79,176,232,183,195,51,192,217,132,107,166,31,105,116,159,188,209,70,59,223,3,174,35,172,52,12,69,125,138,231,44,56,152,
112,205,189,148,65,91,31,102,208,70,95,145,76,212,127,78,122,95,249,1,134,1,71,182,185,105,110,142,186,207,115,153,217,153,192,113,52,78,182,50,20,27,152,217,55,211,10,144,40,136,199,210,190,190,97,192,
97,102,182,125,155,245,43,143,66,24,208,67,9,74,247,189,77,138,205,3,156,111,102,167,154,217,244,77,202,56,21,197,55,72,246,31,203,52,248,238,29,224,103,192,249,4,223,206,153,9,27,234,142,35,220,252,3,
44,70,176,188,61,151,162,191,49,117,159,63,6,190,4,60,64,115,107,169,17,44,64,27,18,92,1,6,2,254,191,148,212,139,38,217,252,119,104,131,63,157,73,80,64,110,37,184,114,12,48,61,65,169,63,4,216,162,238,
251,63,155,217,10,146,106,203,167,229,124,6,111,82,4,184,27,216,135,224,46,34,194,68,105,27,130,53,187,86,1,219,8,184,49,182,51,73,215,2,215,38,43,2,51,0,151,0,27,212,20,57,154,160,140,53,98,170,194,134,
158,126,102,177,6,223,165,222,180,42,233,94,194,178,61,102,54,3,255,85,188,78,3,82,43,48,41,251,126,134,228,90,174,235,123,40,22,2,174,102,240,249,71,95,123,21,100,70,51,187,181,141,122,109,191,39,205,
236,211,132,223,184,158,139,129,63,3,183,16,92,75,6,24,78,112,241,219,151,48,81,174,229,12,51,187,75,157,109,90,60,145,198,207,159,167,8,19,194,113,132,103,244,72,96,61,66,6,196,218,132,43,27,1,103,197,
118,102,102,139,211,56,49,212,37,192,185,192,205,4,87,145,1,134,19,222,49,123,19,246,234,212,114,154,153,221,46,233,137,33,250,27,5,108,75,184,215,242,12,161,247,14,112,3,240,103,101,176,137,125,0,73,
247,153,217,170,192,143,128,31,243,223,13,243,181,124,135,240,59,157,155,85,191,78,1,148,237,199,210,239,7,57,250,108,19,118,53,183,242,137,187,8,88,160,73,253,13,27,148,31,147,82,134,93,234,234,191,2,
76,159,162,254,194,132,13,69,191,5,86,104,99,12,174,173,235,255,93,34,124,0,9,10,201,49,13,206,255,135,41,250,158,37,98,252,149,156,219,136,38,109,92,85,87,118,208,38,167,148,227,81,239,167,127,100,217,
247,64,94,7,29,250,108,19,172,130,141,54,211,46,146,161,140,103,212,181,253,151,10,140,219,39,129,167,27,156,247,35,192,236,101,203,151,226,60,234,125,182,179,58,162,125,182,9,174,23,181,117,63,6,254,
55,178,238,222,12,222,72,120,90,202,49,120,37,226,124,174,104,246,187,18,38,10,181,101,239,79,217,255,201,117,245,39,3,123,68,214,221,157,193,27,247,127,215,162,206,9,57,253,230,205,142,179,115,188,126,
127,58,68,191,71,149,125,127,249,145,238,112,203,118,255,242,2,225,161,119,201,16,101,26,45,103,165,245,185,123,186,238,243,60,192,141,102,118,184,164,171,90,85,150,244,28,176,115,202,62,107,249,100,221,
231,31,74,186,52,162,95,1,251,154,217,103,8,147,142,102,237,117,194,84,194,139,251,216,33,202,220,84,215,255,188,25,246,223,111,140,74,97,217,156,139,96,221,173,119,235,184,70,210,179,217,138,85,29,204,
108,105,194,4,117,254,186,63,61,4,108,160,206,86,117,250,145,197,235,62,31,47,41,202,247,89,210,113,102,182,20,211,90,120,179,124,254,64,112,239,216,79,205,195,156,142,175,235,127,158,148,237,215,203,
251,43,73,167,196,84,148,244,171,228,252,191,55,68,123,245,108,156,70,184,12,72,179,239,41,10,51,155,23,56,149,176,161,182,25,255,200,186,95,39,95,220,103,187,255,152,74,152,253,47,221,66,209,70,210,203,
76,187,196,7,33,116,83,26,234,151,9,1,86,7,174,52,179,251,205,236,27,121,249,159,153,217,116,4,133,169,150,187,82,54,51,174,238,243,34,237,75,52,13,111,3,155,181,80,180,33,40,62,181,60,148,81,255,253,
200,244,192,231,34,143,37,24,172,104,79,165,61,95,251,174,192,204,86,32,184,137,212,43,218,119,3,235,74,122,177,120,169,186,158,122,55,156,187,83,214,207,235,249,51,9,248,150,164,125,135,80,180,1,174,
175,251,156,246,249,83,175,28,119,122,254,139,182,40,127,89,202,246,59,37,211,254,204,108,59,66,4,171,102,138,246,36,130,145,236,138,44,251,117,242,199,45,219,253,199,22,146,46,79,81,254,13,166,181,166,
166,218,192,37,233,125,51,251,30,33,150,107,125,221,229,8,254,127,71,152,217,9,192,111,36,101,25,57,97,102,6,95,227,19,83,182,81,159,88,33,173,101,191,17,79,16,126,135,71,34,202,222,65,8,27,247,89,130,
178,119,65,6,253,59,237,113,140,164,84,123,6,186,133,100,243,239,85,12,222,80,119,51,176,73,143,88,180,167,208,222,253,179,45,237,27,166,102,175,251,252,81,202,250,121,60,127,94,1,198,74,250,103,171,130,
146,94,52,179,203,129,77,147,175,206,75,217,87,214,231,223,106,101,239,255,8,238,78,235,50,173,175,121,214,12,248,108,95,152,69,99,102,54,154,96,205,222,122,136,98,119,2,59,73,114,131,75,23,226,202,118,
255,209,52,139,100,19,234,163,15,164,142,150,32,233,60,51,123,8,56,135,198,25,179,22,4,142,34,100,214,250,53,112,130,50,220,116,146,49,237,70,20,24,224,122,96,91,73,81,17,45,36,77,49,179,49,132,208,135,
47,84,120,92,122,153,15,128,67,37,29,85,182,32,121,144,68,184,185,12,152,181,238,79,215,19,38,133,189,18,246,113,162,164,237,210,86,50,179,177,84,103,21,184,211,231,207,125,132,223,52,205,38,247,45,9,
33,31,223,210,16,89,136,11,98,200,243,79,158,171,167,211,65,152,194,162,49,179,175,1,39,209,124,114,48,137,16,22,240,151,234,193,172,154,253,130,43,219,78,43,234,45,17,51,181,211,136,164,7,147,93,214,
63,4,190,75,227,7,203,236,192,254,192,94,102,118,14,112,116,14,179,248,237,83,134,207,91,51,227,254,15,136,85,180,7,80,136,8,114,123,198,114,244,35,31,17,18,188,180,98,42,240,26,33,50,207,4,224,50,73,
175,231,41,88,89,152,217,70,132,77,210,245,247,245,229,132,204,117,105,45,145,206,208,108,102,102,105,92,65,86,200,184,255,35,82,42,218,40,164,141,79,235,254,209,140,141,205,236,19,41,202,47,155,81,191,
149,195,173,217,253,133,43,219,78,43,58,182,108,15,144,188,184,15,53,179,95,18,54,61,238,67,227,13,47,35,128,157,128,111,38,75,152,7,72,122,184,221,126,235,232,212,231,118,74,38,82,56,101,240,86,59,150,
205,94,197,204,182,38,132,159,27,81,247,167,11,128,237,37,53,75,176,225,180,207,183,58,172,223,237,207,159,111,36,71,187,116,251,249,3,81,214,236,137,252,215,154,221,19,231,220,239,84,101,105,204,169,
46,245,150,173,142,147,110,72,250,32,217,145,190,4,193,31,242,182,38,69,13,216,12,184,223,204,126,218,105,191,25,145,149,210,239,56,165,145,36,8,57,159,193,138,246,89,192,118,174,104,87,150,152,125,30,
189,76,87,159,191,153,141,54,179,139,9,46,149,205,20,237,59,128,149,37,253,220,21,237,222,193,149,109,167,21,245,150,237,182,220,72,26,33,105,170,164,11,36,173,6,172,77,72,116,160,6,69,135,19,252,185,
127,150,85,223,29,224,238,28,78,87,99,102,223,38,100,5,173,95,217,60,21,216,217,95,240,149,166,153,97,162,95,232,218,231,111,50,193,125,8,216,170,73,145,137,132,216,240,171,187,219,72,239,225,110,36,78,
43,50,183,108,55,66,210,120,96,124,18,87,117,95,194,82,99,125,170,243,253,205,236,183,26,34,131,88,4,207,144,62,34,137,128,183,8,155,139,206,233,160,111,199,41,21,51,251,62,33,51,108,253,70,179,163,37,
237,87,130,72,253,198,191,129,247,218,168,247,46,240,40,193,245,160,155,153,64,56,151,180,188,11,60,78,136,11,222,85,36,190,217,167,209,92,201,134,96,205,222,217,149,236,222,197,149,109,167,21,153,249,
108,199,32,233,81,96,23,51,59,130,144,90,184,54,101,250,112,66,10,227,31,117,208,197,88,73,247,116,80,191,215,136,138,110,96,102,51,75,250,32,111,97,156,252,48,179,3,129,35,26,252,233,80,73,135,21,45,
79,159,178,167,164,139,203,22,162,68,246,149,244,231,178,133,40,138,196,154,125,18,33,73,86,35,38,2,135,18,50,66,250,138,82,15,227,110,36,78,43,50,137,70,146,150,36,75,223,54,12,78,162,176,84,138,102,
62,100,112,156,214,250,36,19,253,70,189,194,60,100,220,90,51,27,102,102,23,2,239,152,217,45,201,202,131,211,101,36,46,88,141,20,237,31,184,162,157,43,245,121,3,250,237,249,211,151,231,111,129,19,129,63,
209,92,209,190,3,88,73,210,145,174,104,247,62,174,108,59,173,232,216,178,109,102,51,154,217,220,105,235,37,49,69,235,211,210,70,39,42,72,54,121,61,89,247,245,79,204,44,87,235,124,197,169,95,194,93,176,
69,249,93,9,217,204,134,3,171,209,217,170,130,83,48,201,75,255,4,6,255,110,2,118,147,116,76,9,98,245,19,245,27,250,246,53,179,60,147,173,84,141,250,243,223,43,73,71,222,235,124,30,216,179,201,223,38,18,
238,199,213,51,140,178,229,84,28,87,182,157,86,116,228,179,109,102,59,16,252,20,159,55,179,83,205,108,225,20,117,231,0,54,170,251,58,237,195,169,222,50,190,60,112,190,153,141,76,217,78,175,80,159,9,112,
153,102,19,33,51,155,15,248,121,221,215,243,229,34,149,147,23,63,1,190,87,247,221,20,66,236,222,211,74,144,167,223,168,127,254,204,15,92,209,39,10,39,12,62,255,121,129,171,146,103,75,47,211,44,150,250,
237,184,53,187,47,113,159,237,156,49,179,175,16,82,221,214,167,173,29,96,158,186,207,91,152,89,163,216,211,16,172,180,127,144,116,95,86,242,69,208,118,52,18,51,27,70,72,159,59,176,140,246,29,96,103,51,
187,146,144,48,164,89,106,246,145,4,139,235,202,192,2,117,127,75,235,111,125,48,176,65,210,230,0,155,3,79,152,217,223,9,169,139,39,181,104,67,132,141,149,119,72,202,42,185,67,89,252,153,105,149,175,249,
129,251,204,236,38,224,89,66,66,151,225,132,151,197,42,12,190,110,199,21,33,100,175,97,102,70,88,25,88,133,48,230,3,190,242,99,234,138,46,103,102,71,38,255,23,240,52,112,187,164,123,219,236,250,187,13,
190,187,159,48,201,58,178,193,223,26,33,194,132,249,142,68,150,70,17,131,156,198,28,13,108,199,180,207,177,85,9,207,159,43,128,151,136,75,97,62,129,144,224,228,86,73,83,51,151,50,63,142,7,118,96,90,229,
115,69,224,241,36,135,194,139,196,157,255,139,252,247,252,187,65,73,189,152,176,169,126,142,228,179,251,102,247,57,174,108,231,136,153,173,9,156,151,178,218,226,201,209,140,47,155,217,98,145,15,220,70,
47,197,180,47,202,122,101,123,120,108,69,73,83,205,236,77,96,209,154,175,103,96,232,93,217,67,241,38,33,173,116,52,146,30,54,179,111,16,178,228,213,110,6,156,151,240,18,72,133,153,237,39,233,232,216,238,
35,191,43,12,73,183,152,217,117,192,122,53,95,207,15,124,37,162,250,91,192,25,185,8,214,251,252,146,184,132,74,75,1,7,212,127,105,102,223,151,116,98,27,253,206,214,224,187,21,147,163,29,142,34,100,121,
117,34,144,244,82,146,238,125,28,211,70,87,26,9,124,181,141,38,127,5,236,209,137,72,29,212,77,223,153,244,170,153,109,5,252,19,152,185,230,79,179,16,247,204,169,231,76,224,219,89,200,150,39,146,62,52,
179,229,129,189,8,19,213,179,37,189,82,178,88,78,137,184,27,73,190,108,158,67,155,11,3,43,69,150,125,141,105,195,76,77,5,158,79,217,223,163,117,159,83,165,250,37,60,108,154,89,176,211,178,139,164,9,105,
43,73,250,43,176,35,233,67,254,53,34,250,55,77,162,119,188,90,247,245,179,25,200,208,41,7,147,62,252,152,128,189,36,189,156,131,60,121,81,63,81,172,255,92,36,237,78,48,7,104,247,89,242,70,135,253,214,
147,199,51,45,47,178,250,253,59,106,71,210,237,192,38,132,201,106,167,108,150,178,252,51,45,62,231,78,178,42,179,17,217,92,139,93,115,253,73,122,78,210,62,146,142,113,69,219,113,101,59,95,174,201,161,
205,23,129,168,37,229,100,185,183,182,236,99,146,222,79,217,223,109,76,155,34,247,193,52,149,37,141,3,62,7,92,145,178,223,90,158,6,118,144,116,65,187,13,72,58,135,224,78,146,118,178,80,79,218,229,252,
90,183,151,231,37,189,214,97,255,29,35,233,22,224,179,12,222,124,218,140,183,128,29,37,157,149,159,84,185,112,93,221,231,171,74,145,34,112,87,135,245,219,117,35,185,161,195,126,235,105,87,142,50,184,182,
238,115,187,191,127,199,237,72,186,30,88,131,193,62,204,105,73,235,66,88,235,246,54,137,148,207,239,172,72,242,40,172,78,122,249,235,233,166,235,207,113,254,131,185,251,93,126,36,126,154,255,203,208,62,
219,105,120,18,56,83,210,205,41,100,88,138,176,243,121,58,224,152,118,98,76,155,217,214,192,215,129,199,128,67,36,181,101,33,78,150,213,190,73,240,83,159,147,176,148,216,40,206,179,8,190,140,79,18,54,
68,254,37,171,244,209,102,54,19,240,125,96,5,130,47,121,125,186,234,70,136,160,164,223,1,252,70,82,43,31,239,218,254,22,35,88,146,103,2,142,151,84,153,12,112,201,245,185,25,65,241,94,2,88,136,224,38,52,
153,224,35,250,28,240,20,112,129,164,172,86,39,10,35,217,51,176,45,176,37,225,37,253,251,178,44,76,201,134,220,93,9,171,82,243,17,23,223,92,132,149,144,219,129,211,219,185,7,204,108,78,130,143,254,103,
8,247,93,84,92,245,6,114,76,32,40,110,191,150,212,78,82,150,82,72,92,56,198,18,162,98,252,174,157,149,177,100,83,247,78,132,123,228,124,73,151,118,32,207,116,192,46,4,197,123,20,241,27,206,95,36,140,255,
111,36,213,111,114,30,170,191,81,192,225,192,104,194,187,227,202,116,18,103,139,153,13,228,74,88,135,116,231,255,18,193,112,113,186,164,172,87,107,28,39,119,92,217,118,28,199,113,28,199,113,156,156,112,
55,18,199,113,28,199,113,28,199,201,9,87,182,29,199,113,28,199,113,28,39,39,92,217,118,28,199,113,28,199,113,156,156,112,101,219,113,28,199,113,28,199,113,114,194,149,109,199,113,28,199,113,28,199,201,
9,87,182,29,199,113,28,199,113,28,39,39,92,217,118,28,199,113,28,199,113,156,156,112,101,219,113,28,199,113,28,199,113,114,194,149,109,199,113,28,199,113,28,199,201,9,87,182,29,199,113,28,199,113,28,39,
39,166,43,91,128,126,195,204,214,1,190,11,44,9,204,11,88,93,17,1,111,0,79,2,191,147,116,113,177,18,58,221,138,153,205,1,236,3,172,3,44,10,204,208,160,216,36,224,89,224,22,224,104,73,175,20,38,96,23,96,
102,139,3,95,0,22,0,230,175,249,119,52,48,60,131,46,62,2,158,2,174,6,78,150,244,126,6,109,58,142,227,56,21,198,36,149,45,67,223,96,102,251,1,191,76,89,237,84,73,187,231,33,143,211,59,152,217,60,192,253,
192,39,82,84,123,11,88,94,210,115,249,72,213,29,152,217,112,96,51,96,55,224,75,12,158,0,231,197,139,192,234,146,158,45,168,63,199,113,28,167,4,92,217,46,8,51,251,36,240,8,48,34,101,85,1,159,151,116,107,
246,82,57,189,130,153,157,13,236,216,70,213,191,74,218,58,107,121,186,5,51,219,25,56,12,88,168,36,17,238,0,214,144,244,113,73,253,59,142,227,56,57,227,62,219,197,177,26,233,21,109,8,86,182,117,179,21,
197,233,65,214,105,179,222,186,89,10,209,77,152,217,225,192,111,41,79,209,6,24,3,172,84,98,255,142,227,56,78,206,184,207,118,113,44,217,65,221,101,50,147,194,25,132,153,205,5,172,158,28,175,2,55,3,247,
118,139,181,209,204,102,6,22,108,179,250,28,102,54,191,164,9,89,202,84,101,204,108,58,224,215,192,183,202,150,37,97,101,224,182,162,58,51,179,209,192,231,9,138,254,11,192,63,129,7,36,77,45,74,6,199,113,
170,77,242,156,92,30,88,131,176,191,236,54,224,22,73,175,149,42,88,151,226,202,118,113,204,214,65,221,217,51,147,194,153,6,51,219,20,184,24,152,190,238,79,15,153,217,186,93,242,96,153,149,206,252,140,
103,7,250,66,217,54,51,3,46,2,54,47,91,150,26,10,179,172,155,217,183,128,51,24,124,189,140,55,179,141,36,125,80,148,44,142,227,84,19,51,155,29,184,158,193,171,110,83,204,108,59,73,23,148,32,86,87,227,
110,36,78,223,98,102,171,2,23,48,88,209,6,248,12,112,117,178,121,206,233,29,118,161,90,138,54,192,67,69,116,98,102,155,1,167,211,120,98,182,22,112,126,17,114,56,142,83,121,174,160,177,123,219,112,224,
28,51,91,187,96,121,186,30,87,182,157,126,102,87,96,198,33,254,190,18,65,9,113,122,128,196,125,226,200,178,229,104,192,157,5,245,179,39,67,63,243,55,53,179,79,21,36,139,227,56,21,196,204,198,16,220,204,
154,49,61,224,17,210,82,226,202,182,211,151,36,22,235,45,35,138,142,205,91,22,167,48,142,3,230,40,91,136,58,142,151,244,175,188,59,73,98,176,127,33,162,168,95,239,142,211,223,196,60,3,54,53,179,70,121,
28,156,38,184,178,93,28,157,108,62,242,141,75,217,51,55,48,42,162,220,167,243,22,36,3,58,189,62,122,254,250,50,179,13,128,175,149,45,71,13,83,9,46,29,251,23,212,223,226,52,118,151,170,167,27,174,119,199,
113,242,35,230,25,48,43,33,225,151,19,137,111,144,44,142,78,18,135,60,157,153,20,206,0,49,138,118,154,114,101,242,58,240,1,48,115,27,117,167,210,217,181,89,121,204,108,24,112,98,27,85,95,38,36,10,122,
15,120,31,152,146,129,56,31,2,143,3,87,75,42,196,87,59,161,151,174,119,199,113,242,35,205,179,226,169,60,5,233,37,92,217,46,142,71,59,168,91,228,75,185,95,136,141,240,82,249,72,48,146,100,102,143,1,43,
180,81,253,41,73,31,102,45,83,197,248,42,233,44,182,19,128,111,0,255,232,161,112,120,61,115,189,59,142,147,43,254,172,200,1,119,35,41,142,235,129,187,219,168,247,28,240,151,140,101,113,226,67,229,21,149,
186,187,83,142,106,179,222,49,153,74,81,49,18,171,246,193,41,170,188,2,172,35,233,186,30,82,180,161,247,174,119,199,113,242,193,159,21,57,224,202,118,65,36,9,82,190,10,140,79,81,237,65,96,91,73,239,228,
35,149,211,43,72,58,7,56,136,224,238,16,195,68,224,23,146,78,203,79,170,74,176,45,176,116,138,242,7,74,122,34,47,97,28,199,113,156,254,195,221,72,10,36,121,137,175,109,102,43,18,54,44,141,98,240,236,80,
192,91,4,139,246,109,61,102,93,115,114,68,210,207,204,236,55,132,144,133,11,2,35,26,20,155,68,112,147,184,79,210,139,69,202,87,52,137,85,251,39,41,170,220,11,252,46,39,113,28,199,113,156,62,197,149,237,
18,144,116,15,112,79,217,114,56,189,71,146,241,242,154,178,229,168,8,95,6,150,73,81,254,64,159,220,58,142,227,56,89,227,110,36,142,227,244,28,73,90,246,52,190,218,183,75,186,50,47,121,28,199,113,156,254,
197,149,109,199,113,122,145,109,128,101,83,148,63,60,47,65,28,199,113,156,254,198,149,109,199,113,122,138,196,170,157,198,87,251,78,73,151,231,37,143,227,56,142,211,223,184,178,237,56,78,175,177,53,176,
92,138,242,135,229,37,136,227,56,142,227,184,178,237,56,78,207,208,134,85,251,46,73,151,229,37,143,227,56,142,227,184,178,237,56,78,47,177,21,176,124,138,242,110,213,118,28,199,113,114,197,149,109,199,
113,122,130,54,172,218,119,75,186,52,47,121,28,199,113,28,7,92,217,118,28,167,119,216,2,88,33,69,121,183,106,59,142,227,56,185,227,202,182,227,56,189,194,33,41,202,222,35,233,146,220,36,113,28,199,113,
156,132,255,100,144,52,179,185,128,245,128,181,128,69,128,135,128,179,37,61,210,172,178,153,173,12,108,153,148,31,13,188,10,60,3,220,7,92,34,105,82,140,16,102,182,48,33,219,219,18,192,220,192,108,192,
43,132,148,229,227,129,107,37,77,78,121,110,133,99,102,115,16,172,107,171,19,82,177,207,69,124,150,206,73,192,155,192,27,192,235,192,69,146,238,206,67,206,170,99,102,35,129,37,107,142,165,128,249,24,156,
218,190,158,119,128,39,129,71,128,243,37,189,149,167,156,69,146,184,72,172,10,108,14,44,68,184,79,102,166,245,152,0,136,48,54,111,16,174,177,135,128,115,36,125,152,143,180,197,99,102,91,0,43,166,168,226,
113,181,51,196,204,22,37,60,251,150,36,92,159,179,209,248,218,156,74,120,79,60,7,60,79,120,95,92,39,233,253,34,228,44,2,51,91,8,248,18,176,32,225,29,48,112,180,186,95,63,226,191,247,232,27,132,119,224,
13,146,30,200,85,224,12,49,179,209,192,23,8,122,196,60,192,63,9,239,239,135,58,104,115,56,176,9,176,26,225,250,154,155,161,199,241,67,224,101,194,248,221,13,92,35,233,141,118,251,175,10,201,56,44,202,
127,223,137,75,2,139,3,51,180,168,250,49,225,126,123,18,184,170,151,245,10,51,155,145,48,54,203,212,28,163,50,104,250,35,194,115,107,224,186,26,56,38,0,247,73,82,203,22,146,50,139,16,30,122,170,59,62,
6,246,149,68,237,65,8,171,117,71,131,242,181,199,43,192,207,128,89,235,235,215,180,51,18,184,136,240,0,30,170,173,215,128,159,2,51,54,107,171,236,3,248,17,65,97,30,234,60,210,30,255,0,22,45,251,220,10,
28,195,181,128,235,50,26,187,119,129,19,129,185,155,244,245,249,200,118,158,172,192,184,124,18,120,52,227,107,235,85,96,143,178,207,45,195,49,186,51,197,185,223,3,88,217,50,23,60,62,95,137,28,155,27,83,
182,187,37,112,91,135,215,226,219,192,41,192,167,203,30,167,14,198,247,19,132,247,221,253,25,223,167,2,158,5,78,5,150,45,251,60,91,140,193,146,132,119,117,189,252,31,3,219,182,209,222,116,192,254,132,
73,89,39,227,55,5,184,1,88,183,236,49,106,115,92,231,6,126,65,120,167,101,113,61,221,0,124,113,136,254,98,175,225,13,202,30,155,26,153,215,4,110,77,126,235,172,239,191,86,199,99,192,190,192,28,45,100,
100,70,194,140,167,89,67,83,129,141,146,194,195,128,253,128,137,41,4,121,20,88,177,65,199,139,1,15,166,60,169,134,109,149,125,0,7,230,248,67,158,87,246,249,21,48,126,35,129,243,114,26,191,167,104,240,
146,162,75,148,109,130,149,240,133,156,198,230,99,96,201,178,127,255,12,198,104,179,148,231,189,117,217,50,151,48,70,153,42,219,132,213,187,241,25,95,143,147,128,3,128,97,101,143,87,138,113,157,1,248,
33,97,229,40,175,119,192,192,49,153,48,41,25,85,246,121,55,24,135,153,129,39,90,60,107,86,77,209,222,82,192,237,57,140,225,21,192,124,101,143,87,138,113,216,129,236,148,236,218,99,10,240,131,38,125,118,
141,178,77,88,61,58,183,128,123,47,230,120,30,248,194,16,178,178,109,68,35,55,19,172,223,227,218,20,98,34,176,97,77,167,107,17,44,107,237,180,245,26,21,155,225,3,47,230,252,35,174,84,246,57,230,56,118,
203,145,189,213,182,254,120,7,216,188,174,223,110,81,182,247,202,121,108,186,126,50,71,235,85,182,218,227,94,250,204,170,157,140,81,38,202,54,193,72,114,97,206,215,228,141,192,34,101,143,89,196,152,142,
97,104,5,51,175,227,13,224,43,101,159,127,221,88,196,76,120,127,27,217,214,238,192,251,57,142,223,11,192,42,101,143,89,139,49,152,129,176,154,145,247,181,244,123,96,134,186,190,43,175,108,19,220,136,14,
202,249,58,105,231,152,2,252,28,24,94,47,243,48,96,123,90,179,58,240,0,65,73,110,135,17,192,95,204,236,179,102,182,3,112,45,97,105,164,29,70,1,215,36,62,230,165,99,102,75,16,150,16,243,100,149,156,219,
47,5,51,219,156,48,145,91,50,231,174,70,2,127,53,179,3,114,238,39,15,218,189,231,98,25,147,115,251,185,98,102,27,147,238,254,56,92,201,211,218,137,199,2,123,18,222,3,99,115,238,110,109,96,188,153,205,
151,115,63,109,99,102,255,75,176,236,47,94,66,247,115,2,231,153,217,49,102,22,187,39,40,111,54,140,40,179,109,226,119,220,16,51,27,101,102,87,16,172,247,51,103,38,217,96,22,0,198,153,217,154,57,246,209,
54,102,54,55,193,157,242,59,5,116,247,77,224,31,137,175,125,55,113,28,240,127,228,123,157,180,195,48,194,74,215,95,205,108,150,250,63,124,38,178,145,145,29,10,49,146,112,1,253,129,160,124,119,194,124,
4,31,238,42,176,72,1,125,44,80,64,31,133,98,102,123,3,127,5,102,45,168,203,97,192,145,102,118,182,153,181,218,80,82,37,22,206,185,253,249,115,110,63,111,246,72,81,246,1,224,226,188,4,233,85,204,108,73,
194,170,230,137,192,44,45,138,103,197,66,192,165,102,86,169,151,169,153,205,96,102,103,0,191,161,245,198,180,188,217,7,184,214,204,230,45,89,14,8,27,209,90,49,43,77,140,108,102,182,12,193,109,100,227,
44,133,26,130,153,128,139,146,141,189,149,193,204,150,38,236,129,88,163,192,110,87,7,238,48,179,52,97,83,75,195,204,246,3,190,95,182,28,45,216,12,184,161,118,18,51,140,176,99,184,40,230,38,187,112,131,
187,154,217,178,25,181,213,9,183,18,124,233,242,164,219,21,162,255,96,102,195,205,236,52,224,88,202,9,61,185,35,193,63,188,42,22,161,86,220,148,115,251,51,152,89,22,187,181,11,39,137,250,144,230,229,236,
86,237,20,36,247,234,126,132,232,82,101,88,1,87,6,254,148,68,226,41,29,51,155,29,184,6,248,159,178,101,169,97,29,224,214,100,133,181,76,102,143,44,55,200,130,106,102,155,0,183,16,54,130,23,201,60,192,
37,245,22,200,178,48,179,245,41,103,28,32,76,110,175,55,179,207,150,208,119,52,201,228,168,91,34,73,173,2,92,49,176,154,51,12,152,163,92,121,218,102,56,176,83,217,66,72,122,143,48,35,207,147,34,39,68,
185,145,44,121,158,11,236,90,178,40,91,18,172,116,221,192,245,5,244,209,173,215,215,183,136,159,176,61,72,240,53,118,34,48,179,207,16,92,188,126,73,216,68,95,22,91,17,194,190,149,74,226,210,50,142,224,
226,82,53,22,3,110,74,66,241,150,197,108,145,229,166,81,108,205,108,95,224,210,20,245,179,102,57,96,239,146,250,254,15,137,75,229,229,196,79,90,242,96,78,194,100,178,202,239,131,178,159,71,105,89,137,
240,158,234,250,164,54,155,149,45,64,194,174,192,191,115,108,191,18,150,157,78,48,179,17,192,95,8,27,114,171,192,242,101,11,16,201,229,132,141,50,121,210,117,215,151,153,13,35,121,136,69,242,51,183,106,
71,49,189,153,29,76,136,79,188,106,217,194,36,148,58,57,55,179,79,18,226,69,87,217,234,55,47,193,247,246,11,37,245,159,106,165,208,204,70,152,217,239,128,163,41,95,15,217,215,204,230,44,171,115,51,27,
75,48,4,116,234,94,155,5,163,201,127,15,90,91,36,251,244,182,46,91,142,54,216,0,202,191,200,59,101,169,36,145,76,169,72,122,144,16,221,226,79,132,132,4,206,96,206,38,88,169,156,20,72,154,42,105,119,224,
187,4,5,200,9,108,72,188,63,251,139,192,5,57,202,210,75,172,78,88,166,173,194,139,127,128,77,18,151,161,194,73,140,4,15,104,198,85,0,0,22,72,73,68,65,84,23,18,172,199,85,103,36,112,129,153,85,122,143,
79,226,99,126,61,21,88,153,78,152,3,216,179,140,142,205,108,3,130,91,227,244,101,244,223,101,108,65,247,184,127,214,178,24,116,167,224,245,204,14,148,158,41,80,210,115,192,142,137,171,196,103,9,51,196,
81,12,61,198,235,18,118,3,247,52,102,246,53,66,232,177,118,120,132,176,169,237,241,33,202,44,72,80,190,86,163,55,174,233,65,72,58,5,56,37,121,145,126,138,176,212,55,11,67,91,165,243,222,213,95,38,219,
165,40,251,107,73,31,231,38,137,83,203,84,194,6,175,203,25,188,218,55,35,225,217,184,74,242,111,236,6,195,225,132,204,169,191,202,72,198,52,252,28,104,119,227,216,36,66,88,211,7,147,99,66,139,242,115,
17,2,22,44,75,216,112,216,206,230,241,185,128,179,205,108,125,73,83,219,168,159,55,203,19,92,9,59,9,44,240,24,65,89,191,139,255,238,151,154,142,160,11,204,65,24,131,245,73,23,229,106,19,10,246,5,54,179,
217,128,51,105,239,157,245,14,225,30,251,7,33,134,121,35,102,36,184,61,125,137,108,178,40,150,205,122,41,202,190,2,156,67,208,13,223,33,196,42,239,100,111,221,12,132,213,163,209,4,35,207,166,196,27,171,
239,27,248,79,39,49,5,39,17,50,64,30,193,208,137,113,98,142,247,128,95,19,20,132,183,82,212,171,84,204,237,148,177,34,119,137,60,199,191,149,45,107,7,231,56,47,141,179,138,181,58,62,0,118,73,217,215,82,
116,158,205,174,254,40,61,131,100,7,99,255,70,228,57,46,93,182,172,109,156,91,108,66,149,73,192,39,202,150,183,236,131,248,56,219,237,30,239,18,178,168,205,19,41,207,156,192,195,41,218,63,182,132,49,251,
60,173,179,27,55,58,166,0,71,1,51,117,208,183,1,187,209,126,66,147,239,22,60,86,79,69,202,213,206,120,14,28,207,2,235,164,144,233,235,196,103,117,158,12,204,86,240,152,253,166,205,113,184,4,152,43,69,
63,211,19,178,155,78,238,96,236,27,29,133,198,217,38,196,223,143,145,235,38,96,100,206,178,172,66,88,105,142,145,103,11,73,29,185,145,188,78,184,240,199,74,58,136,224,8,254,106,155,109,61,7,124,94,210,
174,146,246,32,204,26,98,41,107,99,133,19,199,119,72,63,171,126,156,144,109,236,55,105,42,73,122,148,16,50,233,16,242,143,16,227,148,75,172,91,193,56,73,47,229,42,137,243,55,194,132,237,24,73,81,239,0,
73,111,18,172,137,177,239,140,50,34,52,236,72,250,253,12,143,2,107,74,218,79,210,135,237,118,172,192,169,132,13,124,237,108,146,254,122,187,125,231,76,187,251,67,206,3,150,151,116,99,108,5,73,103,19,222,
5,49,12,7,62,215,142,96,237,96,102,159,32,125,84,155,201,192,190,146,182,144,244,70,108,37,73,31,75,58,144,144,179,225,201,148,125,86,137,5,35,203,253,94,210,187,121,10,34,233,78,66,142,138,31,19,38,144,
205,248,163,164,75,160,125,159,237,127,3,107,73,186,165,166,243,183,9,102,251,180,188,6,172,47,233,254,154,182,254,73,200,204,21,131,43,219,213,102,155,148,229,47,3,198,40,248,193,167,70,210,100,73,135,
19,38,108,239,180,211,134,83,109,146,205,145,177,126,169,143,229,41,75,159,243,111,96,172,164,173,36,189,144,182,178,164,103,8,74,84,12,133,250,76,39,225,6,183,72,89,237,38,66,182,223,91,90,150,140,36,
25,163,245,9,86,208,52,140,169,114,82,160,20,76,5,246,151,180,157,164,118,220,69,79,35,172,146,198,80,228,198,192,173,72,167,127,189,74,176,36,31,219,110,135,201,117,57,6,184,161,221,54,74,38,118,188,
30,201,85,138,4,73,83,36,29,65,8,212,81,191,87,111,50,33,233,206,127,38,84,237,40,219,79,18,102,238,141,78,40,173,101,251,125,96,83,73,141,252,113,99,173,81,69,37,69,113,82,98,102,139,147,110,7,255,225,
132,37,151,183,59,237,91,210,53,4,43,247,115,157,182,229,84,142,249,136,247,115,140,157,180,59,241,76,5,78,34,88,179,59,77,18,116,107,100,185,162,55,194,175,72,186,100,98,247,2,155,73,138,85,236,162,81,
88,183,222,141,248,137,9,4,11,114,85,162,117,181,203,187,192,150,146,142,106,183,129,100,5,229,154,200,226,237,102,181,110,135,52,89,88,239,1,86,150,116,67,167,157,38,227,241,37,224,172,78,219,42,129,
88,171,124,187,251,195,218,66,210,149,4,119,217,85,8,43,249,91,0,11,72,58,88,210,164,129,114,105,149,237,7,8,138,246,51,77,254,254,122,138,182,38,3,95,145,212,44,70,117,172,85,114,166,20,125,58,197,146,
38,233,208,159,36,29,146,188,88,50,33,177,142,127,14,184,51,171,54,157,74,144,38,50,197,80,27,107,157,244,220,11,172,38,233,123,25,45,213,198,222,155,69,111,244,77,147,36,230,49,96,195,44,140,4,205,80,
216,236,248,117,224,170,20,213,202,78,116,211,9,79,3,171,75,186,44,131,182,98,93,46,230,202,160,175,88,98,223,141,111,17,38,28,207,103,213,113,226,86,178,19,240,147,172,218,44,136,88,101,251,123,102,246,
199,34,35,24,37,43,234,119,73,250,181,164,75,37,189,82,95,38,141,178,125,27,193,71,123,40,139,115,154,135,239,174,146,174,24,226,239,177,15,174,110,10,112,222,111,164,177,20,28,152,135,0,201,245,186,14,
193,175,212,233,13,210,196,196,109,119,31,137,51,45,31,0,251,17,92,188,238,200,176,221,215,34,203,21,173,108,167,217,103,178,93,163,151,107,214,40,68,212,217,134,176,34,28,67,183,70,160,24,71,216,179,
243,80,70,237,197,186,159,124,148,81,127,49,196,190,27,79,205,82,209,174,69,210,79,129,29,128,137,121,180,159,3,105,252,205,119,0,30,53,179,147,146,56,249,165,147,70,217,222,38,89,130,24,138,88,171,228,
120,73,191,109,81,38,118,115,73,108,248,40,167,120,98,31,40,79,43,132,78,204,133,100,105,119,44,112,124,94,125,56,133,146,230,229,211,205,214,189,170,112,5,176,140,164,163,37,101,189,241,56,86,113,156,
169,224,180,237,177,86,206,151,37,221,147,171,36,53,36,207,178,113,145,197,139,180,212,102,197,25,132,61,92,177,147,176,24,98,247,19,164,89,153,111,27,51,155,157,248,184,218,55,228,40,10,146,206,33,236,
9,40,228,220,59,36,237,36,127,38,66,126,138,199,204,236,124,51,43,53,73,87,26,101,123,82,235,34,209,196,196,188,141,157,101,186,101,187,186,196,90,32,115,223,196,166,144,28,102,111,186,111,233,204,25,
204,211,41,202,46,147,155,20,189,207,75,192,87,37,109,42,233,217,60,58,144,244,17,33,84,94,43,140,98,13,43,177,86,225,235,114,149,162,49,215,70,150,235,38,203,246,20,96,47,73,255,171,236,99,226,199,94,
187,105,158,43,157,144,102,101,174,136,119,227,77,132,156,31,47,231,221,87,39,72,250,7,97,19,114,90,134,19,50,87,223,102,102,55,154,217,230,5,79,220,129,106,103,144,116,101,187,251,137,189,190,98,173,
91,29,147,44,157,253,168,168,254,156,236,145,244,62,33,105,65,12,174,108,167,71,132,40,14,159,150,116,126,1,253,197,110,42,44,114,127,206,44,145,229,254,149,171,20,157,245,25,123,14,101,243,54,33,80,194,
9,57,181,127,29,173,93,92,223,39,196,113,46,130,52,122,87,33,239,198,100,127,211,58,84,92,225,166,115,119,211,181,9,113,202,31,54,179,111,155,89,97,19,120,87,182,157,42,80,152,178,157,240,11,224,153,130,
251,116,178,37,214,10,181,134,153,245,106,22,205,60,120,137,176,9,126,183,60,55,252,213,17,235,50,232,155,225,123,143,199,129,207,73,186,58,175,14,146,235,184,85,232,196,191,36,171,44,85,163,72,67,212,
163,64,94,19,158,76,144,52,158,16,30,184,83,62,13,156,14,60,107,102,7,153,89,238,46,87,174,108,59,85,160,80,101,59,137,120,114,118,145,125,58,153,243,84,100,185,81,192,206,121,10,210,99,60,38,233,230,
130,251,244,103,125,127,114,29,65,209,126,180,128,190,14,34,164,137,111,196,77,192,238,5,200,144,22,17,63,17,205,138,179,25,58,73,75,21,248,26,217,185,111,141,38,196,195,126,206,204,78,52,179,220,226,
249,247,130,178,237,27,36,187,159,162,31,40,0,127,47,161,79,39,59,198,167,40,187,143,153,13,207,77,18,167,83,220,178,221,127,156,2,108,20,17,116,33,19,36,77,36,68,168,216,6,56,134,16,123,251,132,228,243,
70,157,100,251,204,145,143,178,12,133,27,67,146,156,170,12,215,168,104,36,189,71,72,90,215,105,140,255,90,102,1,246,4,30,79,54,83,142,201,176,109,160,55,148,109,183,118,116,63,133,62,80,18,98,54,101,57,
213,229,28,226,125,125,63,9,124,57,71,89,156,206,240,103,125,255,48,25,216,93,210,119,115,136,108,51,36,10,92,36,233,7,146,54,148,180,87,242,185,104,55,198,88,202,120,47,66,23,188,27,147,201,211,182,132,
68,120,89,38,146,26,216,76,121,123,178,153,114,179,172,54,83,186,178,237,56,78,215,145,248,97,166,217,188,119,80,25,59,208,157,40,220,178,221,63,236,36,233,212,178,133,112,186,159,36,93,250,33,192,146,
192,239,201,222,253,101,109,224,82,224,161,44,54,83,186,178,237,56,78,183,114,122,138,178,203,1,91,230,37,136,211,17,254,172,239,31,98,247,90,56,78,20,146,254,45,105,103,96,37,224,66,178,87,186,151,38,
131,205,148,189,160,108,187,207,182,227,244,33,201,70,190,135,83,84,57,56,47,89,156,142,112,203,182,227,56,29,33,233,62,73,95,6,150,2,78,37,251,189,96,29,109,166,236,5,101,219,173,29,142,211,191,164,89,
146,94,201,204,54,203,77,18,167,93,252,89,239,56,78,38,72,122,66,210,238,192,194,192,97,64,150,217,72,97,218,205,148,231,153,217,178,49,149,92,217,118,28,167,155,57,147,16,27,58,22,183,110,87,143,216,
103,189,91,182,29,199,137,66,210,107,146,14,37,40,221,187,3,79,102,220,197,112,224,43,192,189,102,118,138,153,13,153,177,181,202,202,246,196,200,114,174,108,59,78,159,146,132,236,58,42,69,149,85,205,108,
195,188,228,113,218,194,221,72,28,199,201,5,73,31,38,155,114,151,36,68,165,186,45,227,46,134,19,148,249,199,205,236,123,102,54,93,163,66,85,86,182,221,178,237,56,78,12,167,17,159,190,29,220,186,93,53,
252,89,239,56,78,174,72,154,42,233,66,73,171,241,223,72,35,89,134,87,156,147,16,187,253,6,51,27,89,255,199,94,80,182,125,131,164,227,244,49,146,62,32,36,170,136,101,13,51,251,98,94,242,56,169,113,203,
182,227,56,133,33,105,188,164,45,128,101,8,174,136,177,158,20,49,172,1,92,109,102,179,213,126,217,11,202,182,91,59,28,199,57,133,116,27,97,220,186,93,29,252,89,239,56,78,225,72,250,151,164,111,3,139,2,
63,3,178,202,104,186,58,112,141,153,205,62,240,133,43,219,142,227,116,61,73,22,184,99,83,84,89,215,204,214,202,75,30,39,21,110,217,118,28,167,52,36,189,36,233,32,194,102,202,189,128,103,51,104,246,115,
212,228,130,112,101,219,113,156,94,225,100,224,141,20,229,127,146,151,32,78,42,252,89,239,56,78,233,72,122,79,210,9,192,167,128,237,129,123,58,108,114,27,51,251,52,184,178,237,84,131,50,126,195,21,74,
232,211,201,17,73,239,2,199,167,168,178,190,153,173,150,151,60,78,52,110,217,118,156,193,20,190,31,205,204,102,5,22,47,186,223,170,33,105,178,164,115,37,173,4,172,15,92,221,102,83,195,128,31,13,252,167,
170,196,42,219,35,204,204,114,149,196,201,155,153,75,232,243,155,37,244,233,228,207,137,192,91,41,202,187,117,187,124,60,206,182,227,12,102,184,153,21,109,136,218,150,114,222,199,149,69,210,117,146,54,
34,24,232,174,107,163,137,237,205,108,209,202,42,219,146,38,17,31,150,197,35,146,116,55,179,20,217,153,153,173,7,172,90,100,159,78,49,72,122,155,160,112,199,178,177,153,173,146,151,60,78,20,177,150,109,
95,197,116,250,141,194,20,95,51,155,5,248,94,81,253,117,27,73,58,248,245,129,177,192,83,41,170,78,7,108,89,89,101,59,193,93,73,250,131,194,148,109,51,219,0,184,164,168,254,156,82,56,30,120,39,69,121,143,
76,82,46,85,180,108,199,134,2,251,68,174,82,116,214,103,150,225,204,156,236,72,19,219,185,144,119,99,18,166,238,106,220,189,178,37,146,46,38,132,12,252,83,138,106,139,185,178,237,228,73,172,194,179,64,
174,82,36,152,217,214,132,64,246,190,76,214,195,72,122,19,56,41,69,149,45,204,204,95,50,229,81,69,203,118,108,24,201,245,114,149,162,179,62,211,132,194,116,138,227,221,20,101,115,127,55,154,217,188,192,
181,132,248,208,78,4,146,38,2,59,1,55,68,86,169,188,178,29,59,51,119,55,146,106,18,251,176,95,206,204,230,200,83,16,51,219,7,184,0,191,86,250,133,227,128,247,82,148,255,113,94,130,56,45,169,162,101,251,
213,200,114,75,153,89,33,198,130,26,98,19,50,197,158,131,83,44,111,0,83,35,203,174,157,167,32,102,182,52,112,43,48,38,207,126,122,17,73,147,129,95,69,22,175,188,178,237,150,237,238,38,86,217,30,6,236,
150,135,0,102,54,220,204,126,69,200,48,88,245,235,221,201,8,73,175,19,18,221,196,50,214,204,150,205,75,30,103,72,186,217,178,13,112,138,153,77,151,155,36,53,152,217,97,196,187,145,184,101,187,130,72,154,
74,124,136,210,111,153,89,46,147,204,36,139,238,205,192,98,121,180,223,39,220,25,89,206,149,109,39,87,158,73,81,246,80,51,91,63,203,206,205,108,36,193,109,36,23,69,222,169,60,199,0,239,71,150,53,224,200,
28,101,113,154,83,69,203,246,115,41,202,110,9,156,153,119,84,44,51,219,139,116,209,115,210,156,131,83,44,207,68,150,91,10,248,117,214,147,57,51,219,25,184,10,200,117,69,57,47,204,108,38,51,171,130,254,
58,58,178,220,176,42,8,59,20,174,108,119,55,119,1,19,34,203,142,0,174,50,179,189,179,232,216,204,22,4,110,2,54,206,162,61,167,251,144,244,42,112,90,138,42,155,154,217,151,242,146,199,105,74,21,45,219,
183,3,255,78,81,254,27,4,215,165,92,48,179,111,146,46,67,234,84,224,175,57,137,227,116,206,165,41,202,126,29,184,214,204,230,233,180,83,11,28,1,252,22,152,190,211,246,138,196,204,70,152,217,79,204,236,
17,130,139,224,187,102,118,139,153,109,82,162,88,43,71,150,123,217,149,109,39,55,36,137,116,15,252,225,192,177,102,118,118,39,75,103,102,182,46,112,27,240,217,118,219,112,122,134,163,136,87,230,0,78,50,
179,88,107,133,147,13,149,179,108,39,75,253,231,165,172,246,125,51,59,203,204,102,207,74,14,51,155,206,204,14,6,206,36,172,190,196,50,78,82,154,201,130,83,44,23,165,44,191,14,112,167,153,173,212,110,135,
102,54,39,240,23,224,192,118,219,40,139,228,158,186,27,56,12,248,52,193,37,116,102,96,53,224,50,51,59,188,104,75,119,18,3,61,118,213,252,149,94,81,182,125,211,91,117,57,3,152,146,178,206,142,192,237,105,
125,104,205,108,70,51,59,6,184,30,152,63,101,159,78,15,34,233,101,224,215,41,170,44,9,140,55,179,229,114,18,201,25,76,85,51,72,254,177,141,58,223,0,30,204,98,133,196,204,150,33,108,94,59,156,96,136,72,
67,154,176,100,78,193,72,122,16,184,37,101,181,133,129,155,205,44,117,44,108,51,219,16,120,16,216,38,109,221,138,112,50,240,153,38,127,51,66,248,214,107,204,172,144,247,190,153,45,74,240,119,111,38,83,
61,47,20,178,169,163,3,186,214,178,109,102,35,128,217,25,250,33,25,107,1,153,193,204,134,218,20,243,49,240,118,178,59,182,82,72,186,39,81,128,247,79,89,117,89,224,14,51,59,147,48,27,127,116,136,178,11,
1,155,2,219,3,75,180,37,104,23,145,248,134,142,36,40,31,67,89,187,98,39,211,115,15,113,125,9,120,79,82,172,239,115,21,249,37,240,29,226,159,19,75,0,247,155,217,51,4,101,231,93,130,239,119,22,113,139,63,
34,36,68,184,65,146,251,212,6,42,249,156,79,158,93,39,1,123,166,172,186,32,112,181,153,253,29,184,151,160,228,60,72,112,169,27,42,198,242,40,194,203,123,217,228,216,148,246,12,73,227,128,223,181,81,207,
41,150,111,17,174,143,52,191,241,12,192,9,102,246,101,194,111,124,45,225,253,223,136,25,9,145,107,182,4,182,232,64,206,82,73,220,103,118,140,40,186,30,240,172,153,221,0,252,29,120,137,16,145,103,224,104,
247,249,61,61,193,205,117,14,66,28,242,149,128,237,128,185,82,180,113,181,43,219,25,145,100,95,218,30,248,42,240,121,178,181,194,108,8,188,24,33,195,59,4,171,238,121,192,95,36,165,181,40,231,197,33,132,
155,253,211,41,235,205,8,236,145,28,125,141,153,173,74,240,221,219,138,96,181,207,114,85,106,92,68,255,147,129,199,9,19,159,179,36,165,201,160,85,42,146,94,52,179,163,72,159,188,102,209,228,200,131,73,
102,118,26,176,79,133,238,211,178,136,94,193,52,51,75,220,211,138,226,7,132,248,195,237,44,223,111,144,28,69,242,26,176,189,95,83,213,71,210,191,204,236,39,192,47,218,168,190,86,114,244,3,171,165,40,59,
29,176,126,114,84,5,1,151,246,138,27,73,169,202,118,226,79,244,119,224,55,132,217,85,209,203,157,3,204,70,80,198,206,5,254,108,102,149,216,0,33,233,35,96,35,210,165,56,117,18,204,108,39,194,146,227,119,
9,86,179,50,238,219,233,128,165,9,209,16,238,50,179,52,15,192,42,112,4,97,178,80,21,70,16,82,35,255,95,217,130,148,141,164,143,137,119,53,43,218,186,61,137,96,64,137,13,213,86,38,147,129,111,184,175,118,
87,113,20,193,69,194,105,78,154,36,64,85,228,159,146,94,234,21,101,187,108,165,242,79,192,234,37,203,80,207,151,105,111,198,156,11,146,158,37,4,232,31,202,29,164,72,222,44,91,128,24,204,108,45,194,206,
241,42,221,171,115,0,127,55,179,81,101,11,18,75,146,241,235,59,101,203,209,128,3,204,204,227,220,198,63,235,71,228,42,69,3,36,61,65,216,160,214,114,117,177,68,62,2,182,145,116,101,217,130,56,241,40,176,
39,112,116,217,178,36,76,166,122,202,237,45,196,103,163,174,34,7,65,120,129,55,243,247,169,39,102,19,75,236,3,51,118,67,204,219,145,229,98,251,205,156,196,55,187,140,148,189,49,148,25,18,103,16,137,197,
101,29,194,174,226,50,185,159,224,242,19,67,217,190,202,27,145,46,10,65,81,204,74,151,45,99,74,186,30,56,171,108,57,234,48,138,201,222,22,251,140,44,235,89,250,86,100,185,82,228,75,54,180,173,9,60,93,
70,255,45,120,23,216,68,210,37,37,202,80,245,235,171,210,72,218,143,116,49,212,243,96,50,97,21,231,177,200,242,31,228,40,203,127,72,12,37,221,154,43,227,34,73,227,32,40,219,175,71,84,152,36,41,38,245,
113,76,91,16,191,36,247,124,100,185,52,161,189,178,102,12,21,240,25,111,194,82,102,54,111,217,66,212,146,68,135,88,155,242,98,192,222,71,152,28,61,28,89,190,236,229,227,92,211,245,118,72,149,101,107,198,
15,136,127,153,20,197,242,5,244,17,251,108,142,45,151,53,49,207,122,37,47,222,82,72,246,41,172,9,60,80,150,12,13,120,21,88,79,210,63,74,150,163,234,215,87,229,145,244,83,96,7,178,217,136,157,150,143,129,
175,74,186,8,120,37,178,78,97,191,165,164,115,8,97,255,186,137,231,9,174,130,64,80,182,95,138,168,20,83,38,77,185,216,229,184,103,50,46,151,7,133,204,238,218,100,10,229,220,184,67,146,68,182,216,134,224,
175,86,36,127,4,214,144,244,26,225,129,50,53,162,78,217,254,143,85,190,190,170,44,91,67,146,223,126,77,66,194,165,170,240,106,1,125,100,253,12,207,154,103,34,202,60,155,183,16,173,144,52,129,224,50,248,
231,178,101,33,228,18,88,89,210,29,101,11,66,220,117,51,133,98,174,245,174,37,81,42,191,72,177,227,244,44,176,102,162,104,67,156,126,54,53,178,92,102,72,58,20,216,9,152,84,100,191,109,242,42,176,65,237,
254,137,97,192,213,17,21,175,137,105,93,210,163,196,61,16,163,218,35,132,181,105,165,44,190,78,8,159,83,22,247,82,190,245,179,25,119,73,138,117,197,41,20,73,83,37,237,79,136,75,27,187,132,220,46,31,1,
187,72,250,250,64,8,187,100,211,230,117,17,117,203,246,129,188,190,228,254,135,34,102,252,42,71,146,89,242,11,84,103,108,115,119,171,146,244,36,240,100,68,209,152,247,65,30,92,30,81,230,239,185,75,17,
129,164,247,37,125,141,96,181,138,117,195,204,154,95,1,107,75,138,93,253,205,155,152,235,102,188,164,50,87,161,187,2,73,55,3,171,18,50,32,231,205,101,192,138,146,110,175,249,46,230,94,188,165,12,221,66,
210,89,192,151,168,246,10,201,195,132,213,166,105,246,167,13,3,46,140,168,124,65,138,142,90,149,125,13,184,49,166,161,228,199,188,162,69,177,139,147,108,95,165,144,132,161,74,155,13,170,40,210,252,110,
165,32,233,108,66,148,139,243,115,234,226,113,96,53,73,167,55,248,91,171,88,180,31,17,247,224,201,147,203,168,166,159,227,4,210,39,101,168,12,146,222,37,236,105,56,149,114,87,127,238,35,88,40,139,160,
213,243,224,13,202,155,128,252,141,214,43,37,49,239,170,194,144,116,18,193,149,170,200,61,40,207,19,150,251,247,72,34,165,84,133,191,209,122,226,81,249,247,81,85,144,244,12,225,218,250,14,241,123,215,
210,48,153,144,251,98,11,73,245,193,2,46,165,181,101,189,180,223,82,210,141,4,157,161,106,251,111,166,16,86,235,87,146,52,216,213,44,9,89,186,63,33,22,96,163,227,72,73,196,30,192,44,132,184,189,141,218,
122,143,176,92,145,166,189,209,192,35,77,218,187,19,152,57,77,123,121,28,132,176,104,127,24,98,12,203,56,14,43,123,92,218,24,199,205,128,135,50,58,255,15,9,138,212,200,33,250,51,130,133,168,81,253,137,
132,77,71,85,24,151,181,9,214,255,178,175,169,129,227,81,96,209,178,199,37,195,241,157,135,16,131,123,66,129,99,40,130,43,211,167,10,60,207,153,129,127,52,145,229,125,96,157,146,127,135,141,8,19,203,70,
242,253,180,236,235,100,8,185,13,248,10,97,47,64,94,215,202,171,192,222,192,12,101,159,239,16,227,176,45,65,137,107,36,255,121,192,240,178,101,236,198,3,152,143,16,241,108,82,70,215,210,109,4,119,202,
161,250,92,147,16,1,164,210,191,37,33,224,194,223,9,19,189,178,222,135,19,128,159,2,11,13,37,171,37,2,99,102,95,3,54,39,36,100,153,158,144,138,242,42,73,103,146,18,51,155,21,216,151,144,12,96,21,194,108,
252,159,192,239,53,237,114,69,108,123,243,1,223,39,68,63,248,12,112,7,225,165,113,166,194,134,187,210,73,178,250,173,15,140,37,248,92,141,34,132,72,75,155,102,183,29,38,19,66,217,189,12,92,5,92,32,169,
40,107,89,230,36,49,156,119,36,108,28,91,130,48,225,138,97,2,97,243,210,141,192,233,10,254,185,49,253,237,74,120,209,175,65,120,169,141,7,206,85,152,65,87,2,51,91,144,240,50,219,10,88,156,144,189,170,
168,120,238,239,18,174,175,251,8,171,56,23,73,234,230,80,76,13,73,226,210,111,69,200,222,183,32,33,51,233,130,201,49,50,163,110,62,34,172,182,92,1,252,92,5,47,197,38,201,183,6,158,205,99,128,23,8,207,
230,179,36,221,90,164,44,141,48,179,117,8,201,155,214,34,140,249,13,4,183,195,179,52,240,178,170,40,102,54,156,144,97,110,157,228,248,20,33,106,207,172,4,35,84,171,76,129,147,9,6,169,247,8,147,159,23,
8,134,171,113,192,173,10,174,111,149,38,73,83,63,144,216,109,110,66,6,214,27,129,227,85,193,12,199,221,68,146,73,241,235,4,253,98,9,96,49,226,194,30,127,64,48,98,221,11,252,54,246,62,55,179,149,128,111,
19,140,61,163,9,58,225,13,192,201,10,177,241,43,131,153,205,69,72,156,183,34,48,39,225,253,56,103,221,255,59,9,27,58,133,48,249,120,139,240,46,252,23,97,210,114,27,193,93,183,229,181,253,255,141,212,182,
216,80,65,128,28,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* MidiChordsEditor::midichordsLogo_png = (const char*) resource_MidiChordsEditor_midichordsLogo_png;
const int MidiChordsEditor::midichordsLogo_pngSize = 14039;


//[EndFile] You can add extra defines here...
//[/EndFile]

