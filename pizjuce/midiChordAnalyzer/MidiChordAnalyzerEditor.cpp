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

#include "MidiChordAnalyzerEditor.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
using juce::roundToInt;

//[/MiscUserDefs]

//==============================================================================
MidiChordAnalyzerEditor::MidiChordAnalyzerEditor(MidiChordAnalyzer* const ownerFilter)
    : AudioProcessorEditor(ownerFilter)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    chordNameLabel.reset(new juce::Label("new label",
                                         TRANS("G#Maj13b5#9")));
    addAndMakeVisible(chordNameLabel.get());
    chordNameLabel->setFont(juce::Font(46.00f, juce::Font::plain).withTypefaceStyle("Bold"));
    chordNameLabel->setJustificationType(juce::Justification::centred);
    chordNameLabel->setEditable(false, false, false);
    chordNameLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    chordNameLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    chordNameLabel->setBounds(8, 57, 460, 59);

    chordKeyboard.reset(new ChordAnalyzerKeyboardComponent(ownerFilter->chordKbState, ownerFilter));
    addAndMakeVisible(chordKeyboard.get());

    pizButton.reset(new PizButton());
    addAndMakeVisible(pizButton.get());
    pizButton->setBounds(49, 9, 74, 40);

    flatsButton.reset(new juce::ToggleButton("new toggle button"));
    addAndMakeVisible(flatsButton.get());
    flatsButton->setButtonText(TRANS("flats"));
    flatsButton->addListener(this);

    flatsButton->setBounds(472, 97, 48, 24);

    copyButton.reset(new juce::TextButton("copy"));
    addAndMakeVisible(copyButton.get());
    copyButton->setButtonText(TRANS("Copy"));
    copyButton->setConnectedEdges(juce::Button::ConnectedOnBottom);
    copyButton->addListener(this);

    copyButton->setBounds(544, 103, 48, 21);

    learnChanSlider.reset(new ChannelSlider("channel"));
    addAndMakeVisible(learnChanSlider.get());
    learnChanSlider->setTooltip(TRANS("Chord Input Channel, used for Learn and left-click entering notes"));
    learnChanSlider->setRange(0, 16, 1);
    learnChanSlider->setSliderStyle(juce::Slider::LinearBar);
    learnChanSlider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    learnChanSlider->setColour(juce::Slider::backgroundColourId, juce::Colours::white);
    learnChanSlider->addListener(this);

    learnChanSlider->setBounds(475, 81, 43, 16);

    versionLabel.reset(new juce::Label("new label",
                                       TRANS("99.99.99")));
    addAndMakeVisible(versionLabel.get());
    versionLabel->setFont(juce::Font(12.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    versionLabel->setJustificationType(juce::Justification::centredLeft);
    versionLabel->setEditable(false, false, false);
    versionLabel->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    versionLabel->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    versionLabel->setBounds(339, 23, 58, 24);

    cachedImage_snake_gif_1 = juce::ImageCache::getFromMemory(snake_gif, snake_gifSize);
    internalPath2.startNewSubPath(149.0f, 31.0f);
    internalPath2.lineTo(149.0f, 39.0f);
    internalPath2.lineTo(144.0f, 39.0f);
    internalPath2.lineTo(144.0f, 33.0f);
    internalPath2.lineTo(142.0f, 33.0f);
    internalPath2.lineTo(142.0f, 39.0f);
    internalPath2.lineTo(138.0f, 39.0f);
    internalPath2.lineTo(138.0f, 33.0f);
    internalPath2.lineTo(136.0f, 33.0f);
    internalPath2.lineTo(136.0f, 39.0f);
    internalPath2.lineTo(131.0f, 39.0f);
    internalPath2.lineTo(131.0f, 31.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(151.0f, 39.0f);
    internalPath2.lineTo(151.0f, 33.0f);
    internalPath2.lineTo(155.0f, 33.0f);
    internalPath2.lineTo(155.0f, 39.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(151.0f, 29.0f);
    internalPath2.lineTo(155.0f, 29.0f);
    internalPath2.lineTo(155.0f, 31.0f);
    internalPath2.lineTo(151.0f, 31.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(157.0f, 39.0f);
    internalPath2.lineTo(157.0f, 31.0f);
    internalPath2.lineTo(164.0f, 31.0f);
    internalPath2.lineTo(164.0f, 29.0f);
    internalPath2.lineTo(168.0f, 29.0f);
    internalPath2.lineTo(168.0f, 39.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(162.0f, 33.0f);
    internalPath2.lineTo(162.0f, 37.0f);
    internalPath2.lineTo(164.0f, 37.0f);
    internalPath2.lineTo(164.0f, 33.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(170.0f, 39.0f);
    internalPath2.lineTo(170.0f, 33.0f);
    internalPath2.lineTo(175.0f, 33.0f);
    internalPath2.lineTo(175.0f, 39.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(170.0f, 29.0f);
    internalPath2.lineTo(175.0f, 29.0f);
    internalPath2.lineTo(175.0f, 31.0f);
    internalPath2.lineTo(170.0f, 31.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(188.0f, 37.0f);
    internalPath2.lineTo(188.0f, 39.0f);
    internalPath2.lineTo(177.0f, 39.0f);
    internalPath2.lineTo(177.0f, 29.0f);
    internalPath2.lineTo(188.0f, 29.0f);
    internalPath2.lineTo(188.0f, 31.0f);
    internalPath2.lineTo(181.0f, 31.0f);
    internalPath2.lineTo(181.0f, 37.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(196.0f, 33.0f);
    internalPath2.lineTo(194.0f, 33.0f);
    internalPath2.lineTo(194.0f, 39.0f);
    internalPath2.lineTo(190.0f, 39.0f);
    internalPath2.lineTo(190.0f, 29.0f);
    internalPath2.lineTo(194.0f, 29.0f);
    internalPath2.lineTo(194.0f, 31.0f);
    internalPath2.lineTo(201.0f, 31.0f);
    internalPath2.lineTo(201.0f, 39.0f);
    internalPath2.lineTo(196.0f, 39.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(214.0f, 39.0f);
    internalPath2.lineTo(203.0f, 39.0f);
    internalPath2.lineTo(203.0f, 31.0f);
    internalPath2.lineTo(214.0f, 31.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(210.0f, 33.0f);
    internalPath2.lineTo(207.0f, 33.0f);
    internalPath2.lineTo(207.0f, 37.0f);
    internalPath2.lineTo(210.0f, 37.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(225.0f, 31.0f);
    internalPath2.lineTo(225.0f, 33.0f);
    internalPath2.lineTo(220.0f, 33.0f);
    internalPath2.lineTo(220.0f, 39.0f);
    internalPath2.lineTo(216.0f, 39.0f);
    internalPath2.lineTo(216.0f, 31.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(227.0f, 39.0f);
    internalPath2.lineTo(227.0f, 31.0f);
    internalPath2.lineTo(234.0f, 31.0f);
    internalPath2.lineTo(234.0f, 29.0f);
    internalPath2.lineTo(238.0f, 29.0f);
    internalPath2.lineTo(238.0f, 39.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(231.0f, 33.0f);
    internalPath2.lineTo(231.0f, 37.0f);
    internalPath2.lineTo(234.0f, 37.0f);
    internalPath2.lineTo(234.0f, 33.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(251.0f, 29.0f);
    internalPath2.lineTo(251.0f, 39.0f);
    internalPath2.lineTo(247.0f, 39.0f);
    internalPath2.lineTo(247.0f, 35.0f);
    internalPath2.lineTo(244.0f, 35.0f);
    internalPath2.lineTo(244.0f, 39.0f);
    internalPath2.lineTo(240.0f, 39.0f);
    internalPath2.lineTo(240.0f, 29.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(247.0f, 31.0f);
    internalPath2.lineTo(244.0f, 31.0f);
    internalPath2.lineTo(244.0f, 33.0f);
    internalPath2.lineTo(247.0f, 33.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(253.0f, 31.0f);
    internalPath2.lineTo(264.0f, 31.0f);
    internalPath2.lineTo(264.0f, 39.0f);
    internalPath2.lineTo(260.0f, 39.0f);
    internalPath2.lineTo(260.0f, 33.0f);
    internalPath2.lineTo(257.0f, 33.0f);
    internalPath2.lineTo(257.0f, 39.0f);
    internalPath2.lineTo(253.0f, 39.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(266.0f, 33.0f);
    internalPath2.lineTo(268.0f, 33.0f);
    internalPath2.lineTo(268.0f, 31.0f);
    internalPath2.lineTo(277.0f, 31.0f);
    internalPath2.lineTo(277.0f, 39.0f);
    internalPath2.lineTo(266.0f, 39.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(273.0f, 33.0f);
    internalPath2.lineTo(271.0f, 33.0f);
    internalPath2.lineTo(271.0f, 37.0f);
    internalPath2.lineTo(273.0f, 37.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(279.0f, 29.0f);
    internalPath2.lineTo(284.0f, 29.0f);
    internalPath2.lineTo(284.0f, 39.0f);
    internalPath2.lineTo(279.0f, 39.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(286.0f, 44.0f);
    internalPath2.lineTo(286.0f, 42.0f);
    internalPath2.lineTo(292.0f, 42.0f);
    internalPath2.lineTo(292.0f, 39.0f);
    internalPath2.lineTo(286.0f, 39.0f);
    internalPath2.lineTo(286.0f, 31.0f);
    internalPath2.lineTo(290.0f, 31.0f);
    internalPath2.lineTo(290.0f, 37.0f);
    internalPath2.lineTo(292.0f, 37.0f);
    internalPath2.lineTo(292.0f, 31.0f);
    internalPath2.lineTo(297.0f, 31.0f);
    internalPath2.lineTo(297.0f, 44.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(303.0f, 35.0f);
    internalPath2.lineTo(303.0f, 33.0f);
    internalPath2.lineTo(299.0f, 33.0f);
    internalPath2.lineTo(299.0f, 31.0f);
    internalPath2.lineTo(310.0f, 31.0f);
    internalPath2.lineTo(310.0f, 33.0f);
    internalPath2.lineTo(308.0f, 33.0f);
    internalPath2.lineTo(308.0f, 35.0f);
    internalPath2.lineTo(305.0f, 35.0f);
    internalPath2.lineTo(305.0f, 37.0f);
    internalPath2.lineTo(310.0f, 37.0f);
    internalPath2.lineTo(310.0f, 39.0f);
    internalPath2.lineTo(299.0f, 39.0f);
    internalPath2.lineTo(299.0f, 37.0f);
    internalPath2.lineTo(301.0f, 37.0f);
    internalPath2.lineTo(301.0f, 35.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(312.0f, 39.0f);
    internalPath2.lineTo(312.0f, 31.0f);
    internalPath2.lineTo(323.0f, 31.0f);
    internalPath2.lineTo(323.0f, 35.0f);
    internalPath2.lineTo(321.0f, 35.0f);
    internalPath2.lineTo(321.0f, 37.0f);
    internalPath2.lineTo(323.0f, 37.0f);
    internalPath2.lineTo(323.0f, 39.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(316.0f, 35.0f);
    internalPath2.lineTo(318.0f, 35.0f);
    internalPath2.lineTo(318.0f, 33.0f);
    internalPath2.lineTo(316.0f, 33.0f);
    internalPath2.closeSubPath();
    internalPath2.startNewSubPath(334.0f, 31.0f);
    internalPath2.lineTo(334.0f, 33.0f);
    internalPath2.lineTo(329.0f, 33.0f);
    internalPath2.lineTo(329.0f, 39.0f);
    internalPath2.lineTo(325.0f, 39.0f);
    internalPath2.lineTo(325.0f, 31.0f);
    internalPath2.closeSubPath();

    internalPath3.startNewSubPath(162.0f, 17.0f);
    internalPath3.lineTo(165.0f, 17.0f);
    internalPath3.lineTo(165.0f, 25.0f);
    internalPath3.lineTo(162.0f, 25.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(167.0f, 18.0f);
    internalPath3.lineTo(175.0f, 18.0f);
    internalPath3.lineTo(175.0f, 25.0f);
    internalPath3.lineTo(172.0f, 25.0f);
    internalPath3.lineTo(172.0f, 20.0f);
    internalPath3.lineTo(170.0f, 20.0f);
    internalPath3.lineTo(170.0f, 25.0f);
    internalPath3.lineTo(167.0f, 25.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(185.0f, 20.0f);
    internalPath3.lineTo(182.0f, 20.0f);
    internalPath3.lineTo(182.0f, 22.0f);
    internalPath3.lineTo(185.0f, 22.0f);
    internalPath3.lineTo(185.0f, 24.0f);
    internalPath3.lineTo(183.0f, 24.0f);
    internalPath3.lineTo(183.0f, 25.0f);
    internalPath3.lineTo(177.0f, 25.0f);
    internalPath3.lineTo(177.0f, 24.0f);
    internalPath3.lineTo(180.0f, 24.0f);
    internalPath3.lineTo(180.0f, 22.0f);
    internalPath3.lineTo(177.0f, 22.0f);
    internalPath3.lineTo(177.0f, 20.0f);
    internalPath3.lineTo(178.0f, 20.0f);
    internalPath3.lineTo(178.0f, 18.0f);
    internalPath3.lineTo(185.0f, 18.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(187.0f, 25.0f);
    internalPath3.lineTo(187.0f, 18.0f);
    internalPath3.lineTo(195.0f, 18.0f);
    internalPath3.lineTo(195.0f, 22.0f);
    internalPath3.lineTo(193.0f, 22.0f);
    internalPath3.lineTo(193.0f, 24.0f);
    internalPath3.lineTo(195.0f, 24.0f);
    internalPath3.lineTo(195.0f, 25.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(190.0f, 22.0f);
    internalPath3.lineTo(192.0f, 22.0f);
    internalPath3.lineTo(192.0f, 20.0f);
    internalPath3.lineTo(190.0f, 20.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(203.0f, 18.0f);
    internalPath3.lineTo(203.0f, 20.0f);
    internalPath3.lineTo(200.0f, 20.0f);
    internalPath3.lineTo(200.0f, 25.0f);
    internalPath3.lineTo(197.0f, 25.0f);
    internalPath3.lineTo(197.0f, 18.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(212.0f, 18.0f);
    internalPath3.lineTo(212.0f, 20.0f);
    internalPath3.lineTo(210.0f, 20.0f);
    internalPath3.lineTo(210.0f, 25.0f);
    internalPath3.lineTo(207.0f, 25.0f);
    internalPath3.lineTo(207.0f, 20.0f);
    internalPath3.lineTo(205.0f, 20.0f);
    internalPath3.lineTo(205.0f, 18.0f);
    internalPath3.lineTo(207.0f, 18.0f);
    internalPath3.lineTo(207.0f, 17.0f);
    internalPath3.lineTo(210.0f, 17.0f);
    internalPath3.lineTo(210.0f, 18.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(228.0f, 17.0f);
    internalPath3.lineTo(228.0f, 22.0f);
    internalPath3.lineTo(222.0f, 22.0f);
    internalPath3.lineTo(222.0f, 25.0f);
    internalPath3.lineTo(218.0f, 25.0f);
    internalPath3.lineTo(218.0f, 17.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(222.0f, 20.0f);
    internalPath3.lineTo(225.0f, 20.0f);
    internalPath3.lineTo(225.0f, 18.0f);
    internalPath3.lineTo(222.0f, 18.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(230.0f, 25.0f);
    internalPath3.lineTo(230.0f, 20.0f);
    internalPath3.lineTo(233.0f, 20.0f);
    internalPath3.lineTo(233.0f, 25.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(230.0f, 17.0f);
    internalPath3.lineTo(233.0f, 17.0f);
    internalPath3.lineTo(233.0f, 18.0f);
    internalPath3.lineTo(230.0f, 18.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(238.0f, 22.0f);
    internalPath3.lineTo(238.0f, 20.0f);
    internalPath3.lineTo(235.0f, 20.0f);
    internalPath3.lineTo(235.0f, 18.0f);
    internalPath3.lineTo(243.0f, 18.0f);
    internalPath3.lineTo(243.0f, 20.0f);
    internalPath3.lineTo(242.0f, 20.0f);
    internalPath3.lineTo(242.0f, 22.0f);
    internalPath3.lineTo(240.0f, 22.0f);
    internalPath3.lineTo(240.0f, 24.0f);
    internalPath3.lineTo(243.0f, 24.0f);
    internalPath3.lineTo(243.0f, 25.0f);
    internalPath3.lineTo(235.0f, 25.0f);
    internalPath3.lineTo(235.0f, 24.0f);
    internalPath3.lineTo(237.0f, 24.0f);
    internalPath3.lineTo(237.0f, 22.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(257.0f, 17.0f);
    internalPath3.lineTo(260.0f, 17.0f);
    internalPath3.lineTo(260.0f, 25.0f);
    internalPath3.lineTo(257.0f, 25.0f);
    internalPath3.lineTo(257.0f, 22.0f);
    internalPath3.lineTo(253.0f, 22.0f);
    internalPath3.lineTo(253.0f, 25.0f);
    internalPath3.lineTo(250.0f, 25.0f);
    internalPath3.lineTo(250.0f, 17.0f);
    internalPath3.lineTo(253.0f, 17.0f);
    internalPath3.lineTo(253.0f, 20.0f);
    internalPath3.lineTo(257.0f, 20.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(262.0f, 25.0f);
    internalPath3.lineTo(262.0f, 18.0f);
    internalPath3.lineTo(270.0f, 18.0f);
    internalPath3.lineTo(270.0f, 22.0f);
    internalPath3.lineTo(268.0f, 22.0f);
    internalPath3.lineTo(268.0f, 24.0f);
    internalPath3.lineTo(270.0f, 24.0f);
    internalPath3.lineTo(270.0f, 25.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(265.0f, 22.0f);
    internalPath3.lineTo(267.0f, 22.0f);
    internalPath3.lineTo(267.0f, 20.0f);
    internalPath3.lineTo(265.0f, 20.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(278.0f, 18.0f);
    internalPath3.lineTo(278.0f, 20.0f);
    internalPath3.lineTo(275.0f, 20.0f);
    internalPath3.lineTo(275.0f, 25.0f);
    internalPath3.lineTo(272.0f, 25.0f);
    internalPath3.lineTo(272.0f, 18.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(280.0f, 25.0f);
    internalPath3.lineTo(280.0f, 18.0f);
    internalPath3.lineTo(288.0f, 18.0f);
    internalPath3.lineTo(288.0f, 22.0f);
    internalPath3.lineTo(287.0f, 22.0f);
    internalPath3.lineTo(287.0f, 24.0f);
    internalPath3.lineTo(288.0f, 24.0f);
    internalPath3.lineTo(288.0f, 25.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(283.0f, 22.0f);
    internalPath3.lineTo(285.0f, 22.0f);
    internalPath3.lineTo(285.0f, 20.0f);
    internalPath3.lineTo(283.0f, 20.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(290.0f, 22.0f);
    internalPath3.lineTo(290.0f, 20.0f);
    internalPath3.lineTo(298.0f, 20.0f);
    internalPath3.lineTo(298.0f, 22.0f);
    internalPath3.closeSubPath();
    internalPath3.startNewSubPath(300.0f, 17.0f);
    internalPath3.lineTo(303.0f, 17.0f);
    internalPath3.lineTo(303.0f, 18.0f);
    internalPath3.lineTo(305.0f, 18.0f);
    internalPath3.lineTo(305.0f, 20.0f);
    internalPath3.lineTo(307.0f, 20.0f);
    internalPath3.lineTo(307.0f, 22.0f);
    internalPath3.lineTo(305.0f, 22.0f);
    internalPath3.lineTo(305.0f, 24.0f);
    internalPath3.lineTo(303.0f, 24.0f);
    internalPath3.lineTo(303.0f, 25.0f);
    internalPath3.lineTo(300.0f, 25.0f);
    internalPath3.lineTo(300.0f, 24.0f);
    internalPath3.lineTo(302.0f, 24.0f);
    internalPath3.lineTo(302.0f, 22.0f);
    internalPath3.lineTo(303.0f, 22.0f);
    internalPath3.lineTo(303.0f, 20.0f);
    internalPath3.lineTo(302.0f, 20.0f);
    internalPath3.lineTo(302.0f, 18.0f);
    internalPath3.lineTo(300.0f, 18.0f);
    internalPath3.closeSubPath();

    //[UserPreSize]

    this->setMouseClickGrabsKeyboardFocus(false);
    chordKeyboard->setMouseClickGrabsKeyboardFocus(false);
    learnChanSlider->setMouseClickGrabsKeyboardFocus(false);
    pizButton->setMouseClickGrabsKeyboardFocus(false);
    chordNameLabel->setMouseClickGrabsKeyboardFocus(false);
    chordNameLabel->setText(" ", juce::dontSendNotification);
    flatsButton->setMouseClickGrabsKeyboardFocus(false);
    chordKeyboard->setMouseCursor(juce::MouseCursor::PointingHandCursor);
    numHeldNotes = 0;

    learnChanSlider->setAllText("All");
    pizButton->addListener(this);
    pizButton->setTooltip("http://thepiz.org/plugins");

    const int middleC = getFilter()->getBottomOctave() + 5;
    chordKeyboard->setOctaveForMiddleC(middleC);
    chordKeyboard->setScrollButtonsVisible(false);

    //[/UserPreSize]

    setSize(600, 180);

    //[Constructor] You can add your own custom stuff here..
    versionLabel->setText(JucePlugin_VersionString, juce::dontSendNotification);
    ownerFilter->addChangeListener(this);
    updateParametersFromFilter();
    //[/Constructor]
}

MidiChordAnalyzerEditor::~MidiChordAnalyzerEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    getFilter()->removeChangeListener(this);
    //[/Destructor_pre]

    chordNameLabel  = nullptr;
    chordKeyboard   = nullptr;
    pizButton       = nullptr;
    flatsButton     = nullptr;
    copyButton      = nullptr;
    learnChanSlider = nullptr;
    versionLabel    = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MidiChordAnalyzerEditor::paint(juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(juce::Colour(0xffd8d8d8));

    {
        int x = 0, y = 0, width = proportionOfWidth(1.0000f), height = 100;
        juce::Colour fillColour1 = juce::Colours::white, fillColour2 = juce::Colour(0x00e7e7e7);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               61.0f - 0.0f + x,
                                               static_cast<float>(-15) - 0.0f + y,
                                               fillColour2,
                                               61.0f - 0.0f + x,
                                               23.0f - 0.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
    }

    {
        int x = 479, y = 9, width = 111, height = 61;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(juce::Colours::black);
        g.drawImage(cachedImage_snake_gif_1,
                    x,
                    y,
                    width,
                    height,
                    0,
                    0,
                    cachedImage_snake_gif_1.getWidth(),
                    cachedImage_snake_gif_1.getHeight());
    }

    {
        int x = 477, y = 73, width = 38, height = 8;
        juce::String text(TRANS("Channel"));
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(10.70f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        float x = 0, y = 0;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillPath(internalPath2, juce::AffineTransform::translation(x, y));
    }

    {
        float x = 0, y = 0;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.fillPath(internalPath3, juce::AffineTransform::translation(x, y));
    }

    {
        int x = 8, y = 57, width = 460, height = 59;
        juce::Colour fillColour1 = juce::Colours::black, fillColour2 = juce::Colour(0x279a3c3c);
        juce::Colour strokeColour = juce::Colour(0xb3000000);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               100.0f - 8.0f + x,
                                               16.0f - 57.0f + y,
                                               fillColour2,
                                               100.0f - 8.0f + x,
                                               63.0f - 57.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
        g.setColour(strokeColour);
        g.drawRect(x, y, width, height, 1);
    }

    {
        int x = 0, y = 149, width = proportionOfWidth(1.0000f), height = 33;
        juce::Colour fillColour1 = juce::Colour(0x00000000), fillColour2 = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill(juce::ColourGradient(fillColour1,
                                               69.0f - 0.0f + x,
                                               168.0f - 149.0f + y,
                                               fillColour2,
                                               69.0f - 0.0f + x,
                                               294.0f - 149.0f + y,
                                               false));
        g.fillRect(x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MidiChordAnalyzerEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    chordKeyboard->setBounds(8, 124, getWidth() - 16, 47);
    //[UserResized] Add your own custom resize handling here..
    chordKeyboard->setKeyWidth((float) chordKeyboard->getWidth() / 75.2f);
    //[/UserResized]
}

void MidiChordAnalyzerEditor::buttonClicked(juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == flatsButton.get())
    {
        //[UserButtonCode_flatsButton] -- add your button handler code here..
        getFilter()->setParameterNotifyingHost(kFlats, flatsButton->getToggleState() ? 1.f : 0.f);
        //[/UserButtonCode_flatsButton]
    }
    else if (buttonThatWasClicked == copyButton.get())
    {
        //[UserButtonCode_copyButton] -- add your button handler code here..
        const int channel = roundToInt(getFilter()->getParameter(kChannel) * 16.f);
        juce::String chordString;
        for (int n = 0; n < 128; n++)
        {
            for (int c = 1; c <= 16; c++)
            {
                if ((channel == 0 || channel == c) && getFilter()->chordKbState.isNoteOn(c, n))
                {
                    chordString += getNoteName(n, getFilter()->bottomOctave) + "." + juce::String(c) + " ";
                }
            }
        }
        juce::SystemClipboard::copyTextToClipboard(chordString);
        //[/UserButtonCode_copyButton]
    }

    //[UserbuttonClicked_Post]
    else if (buttonThatWasClicked == pizButton.get())
    {
        juce::URL("http://thepiz.org/plugins/?p=midiChordAnalyzer").launchInDefaultBrowser();
    }
    //[/UserbuttonClicked_Post]
}

void MidiChordAnalyzerEditor::sliderValueChanged(juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == learnChanSlider.get())
    {
        //[UserSliderCode_learnChanSlider] -- add your slider handling code here..
        getFilter()->setParameterNotifyingHost(kChannel, (float) learnChanSlider->getValue() / 16.f);
        //[/UserSliderCode_learnChanSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MidiChordAnalyzerEditor::mouseDown(const juce::MouseEvent& e)
{
}

void MidiChordAnalyzerEditor::mouseDoubleClick(const juce::MouseEvent& e)
{
}

void MidiChordAnalyzerEditor::mouseUp(const juce::MouseEvent& e)
{
}

void MidiChordAnalyzerEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == getFilter())
        updateParametersFromFilter();
}

void MidiChordAnalyzerEditor::updateParametersFromFilter()
{
    MidiChordAnalyzer* const filter = getFilter();
    const bool flats                = filter->getParameter(kFlats) > 0;
    const int chordChan             = roundToInt(filter->getParameter(kChannel) * 16.f);

    learnChanSlider->setValue(chordChan, juce::dontSendNotification);
    flatsButton->setToggleState(flats, juce::dontSendNotification);
    if (chordChan == 0)
        chordKeyboard->setMidiChannelsToDisplay(0xffff);
    else
        chordKeyboard->setMidiChannelsToDisplay(1 << (chordChan - 1));

    if (numHeldNotes < chordKeyboard->getNumHeldNotes(chordChan))
    {
        chordNameLabel->setText(getCurrentChordName(chordChan), juce::dontSendNotification);
        juce::Desktop::getInstance().getAnimator().animateComponent(chordNameLabel.get(), chordNameLabel->getBounds(), 1.f, 0, false, 1.0, 1.0);
    }
    else if (getCurrentChordName(chordChan) == " ")
    {
        juce::Desktop::getInstance().getAnimator().animateComponent(chordNameLabel.get(), chordNameLabel->getBounds(), 0.f, 500, false, 2.0, 0.5);
    }
    else
        startTimer(100);
    numHeldNotes = chordKeyboard->getNumHeldNotes(chordChan);
}

void MidiChordAnalyzerEditor::timerCallback()
{
    const int chordChan = roundToInt(getFilter()->getParameter(kChannel) * 16.f);
    if (getCurrentChordName(chordChan) == " ")
    {
        juce::Desktop::getInstance().getAnimator().animateComponent(chordNameLabel.get(), chordNameLabel->getBounds(), 0.f, 500, false, 2.0, 0.5);
    }
    else
    {
        chordNameLabel->setText(getCurrentChordName(chordChan), juce::dontSendNotification);
        juce::Desktop::getInstance().getAnimator().animateComponent(chordNameLabel.get(), chordNameLabel->getBounds(), 1.f, 0, false, 1.0, 1.0);
    }
    stopTimer();
}

juce::String const MidiChordAnalyzerEditor::getCurrentChordName(int channel)
{
    juce::Array<int> chord;
    for (int n = 0; n < 128; n++)
    {
        for (int c = 1; c <= 16; c++)
        {
            if ((channel == 0 || channel == c) && getFilter()->chordKbState.isNoteOn(c, n))
                chord.add(n);
        }
    }
    return getFirstRecognizedChord(chord, getFilter()->getParameter(kFlats) > 0.f);
}

//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiChordAnalyzerEditor"
                 componentName="" parentClasses="public juce::AudioProcessorEditor, public juce::ChangeListener, public juce::Timer"
                 constructorParams="MidiChordAnalyzer* const ownerFilter" variableInitialisers="AudioProcessorEditor (ownerFilter)"
                 snapPixels="8" snapActive="0" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="600" initialHeight="180">
  <BACKGROUND backgroundColour="ffd8d8d8">
    <RECT pos="0 0 100% 100" fill="linear: 61 -15, 61 23, 0=ffffffff, 1=e7e7e7"
          hasStroke="0"/>
    <IMAGE pos="479 9 111 61" resource="snake_gif" opacity="1.0" mode="0"/>
    <TEXT pos="477 73 38 8" fill="solid: ff000000" hasStroke="0" text="Channel"
          fontname="Default font" fontsize="10.7" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <PATH pos="0 0 100 100" fill="solid: ff000000" hasStroke="0" nonZeroWinding="1">s 148.57 30.73 l 148.57 39.44 l 144.21 39.44 l 144.21 32.91 l 142.03 32.91 l 142.03 39.44 l 137.68 39.44 l 137.68 32.91 l 135.5 32.91 l 135.5 39.44 l 131.14 39.44 l 131.14 30.73 x s 150.74 39.44 l 150.74 32.91 l 155.1 32.91 l 155.1 39.44 x s 150.74 28.56 l 155.1 28.56 l 155.1 30.73 l 150.74 30.73 x s 157.28 39.44 l 157.28 30.73 l 163.81 30.73 l 163.81 28.56 l 168.17 28.56 l 168.17 39.44 x s 161.72 32.91 l 161.72 37.27 l 163.81 37.27 l 163.81 32.91 x s 170.34 39.44 l 170.34 32.91 l 174.7 32.91 l 174.7 39.44 x s 170.34 28.56 l 174.7 28.56 l 174.7 30.73 l 170.34 30.73 x s 187.77 37.27 l 187.77 39.44 l 176.88 39.44 l 176.88 28.56 l 187.77 28.56 l 187.77 30.73 l 181.23 30.73 l 181.23 37.27 x s 196.48 32.91 l 194.3 32.91 l 194.3 39.44 l 189.94 39.44 l 189.94 28.56 l 194.3 28.56 l 194.3 30.73 l 200.83 30.73 l 200.83 39.44 l 196.48 39.44 x s 213.9 39.44 l 203.01 39.44 l 203.01 30.73 l 213.9 30.73 x s 209.54 32.91 l 207.37 32.91 l 207.37 37.27 l 209.54 37.27 x s 224.79 30.73 l 224.79 32.91 l 220.43 32.91 l 220.43 39.44 l 216.08 39.44 l 216.08 30.73 x s 226.97 39.44 l 226.97 30.73 l 233.5 30.73 l 233.5 28.56 l 237.86 28.56 l 237.86 39.44 x s 231.41 32.91 l 231.41 37.27 l 233.5 37.27 l 233.5 32.91 x s 250.92 28.56 l 250.92 39.44 l 246.57 39.44 l 246.57 35.09 l 244.39 35.09 l 244.39 39.44 l 240.03 39.44 l 240.03 28.56 x s 246.57 30.73 l 244.39 30.73 l 244.39 32.91 l 246.57 32.91 x s 253.1 30.73 l 263.99 30.73 l 263.99 39.44 l 259.63 39.44 l 259.63 32.91 l 257.46 32.91 l 257.46 39.44 l 253.1 39.44 x s 266.17 32.91 l 268.34 32.91 l 268.34 30.73 l 277.06 30.73 l 277.06 39.44 l 266.17 39.44 x s 272.7 32.91 l 270.52 32.91 l 270.52 37.27 l 272.7 37.27 x s 279.23 28.56 l 283.59 28.56 l 283.59 39.44 l 279.23 39.44 x s 285.77 43.8 l 285.77 41.62 l 292.3 41.62 l 292.3 39.44 l 285.77 39.44 l 285.77 30.73 l 290.12 30.73 l 290.12 37.27 l 292.3 37.27 l 292.3 30.73 l 296.66 30.73 l 296.66 43.8 x s 303.19 35.09 l 303.19 32.91 l 298.83 32.91 l 298.83 30.73 l 309.72 30.73 l 309.72 32.91 l 307.54 32.91 l 307.54 35.09 l 305.37 35.09 l 305.37 37.27 l 309.72 37.27 l 309.72 39.44 l 298.83 39.44 l 298.83 37.27 l 301.01 37.27 l 301.01 35.09 x s 311.9 39.44 l 311.9 30.73 l 322.79 30.73 l 322.79 35.09 l 320.61 35.09 l 320.61 37.27 l 322.79 37.27 l 322.79 39.44 x s 316.26 35 l 318.43 35 l 318.43 33 l 316.26 33 x s 333.68 30.73 l 333.68 32.91 l 329.32 32.91 l 329.32 39.44 l 324.97 39.44 l 324.97 30.73 x</PATH>
    <PATH pos="0 0 100 100" fill="solid: ff000000" hasStroke="0" nonZeroWinding="1">s 161.67 16.83 l 165 16.83 l 165 25.17 l 161.67 25.17 x s 166.67 18.5 l 175 18.5 l 175 25.17 l 171.67 25.17 l 171.67 20.17 l 170 20.17 l 170 25.17 l 166.67 25.17 x s 185 20.17 l 181.67 20.17 l 181.67 21.83 l 185 21.83 l 185 23.5 l 183.33 23.5 l 183.33 25.17 l 176.67 25.17 l 176.67 23.5 l 180 23.5 l 180 21.83 l 176.67 21.83 l 176.67 20.17 l 178.33 20.17 l 178.33 18.5 l 185 18.5 x s 186.67 25.17 l 186.67 18.5 l 195 18.5 l 195 21.83 l 193.33 21.83 l 193.33 23.5 l 195 23.5 l 195 25.17 x s 190 21.77 l 191.67 21.77 l 191.67 20.23 l 190 20.23 x s 203.33 18.5 l 203.33 20.17 l 200 20.17 l 200 25.17 l 196.67 25.17 l 196.67 18.5 x s 211.67 18.5 l 211.67 20.17 l 210 20.17 l 210 25.17 l 206.67 25.17 l 206.67 20.17 l 205 20.17 l 205 18.5 l 206.67 18.5 l 206.67 16.83 l 210 16.83 l 210 18.5 x s 228.33 16.83 l 228.33 21.83 l 221.67 21.83 l 221.67 25.17 l 218.33 25.17 l 218.33 16.83 x s 221.67 20.17 l 225 20.17 l 225 18.5 l 221.67 18.5 x s 230 25.17 l 230 20.17 l 233.33 20.17 l 233.33 25.17 x s 230 16.83 l 233.33 16.83 l 233.33 18.5 l 230 18.5 x s 238.33 21.83 l 238.33 20.17 l 235 20.17 l 235 18.5 l 243.33 18.5 l 243.33 20.17 l 241.67 20.17 l 241.67 21.83 l 240 21.83 l 240 23.5 l 243.33 23.5 l 243.33 25.17 l 235 25.17 l 235 23.5 l 236.67 23.5 l 236.67 21.83 x s 256.67 16.83 l 260 16.83 l 260 25.17 l 256.67 25.17 l 256.67 21.83 l 253.33 21.83 l 253.33 25.17 l 250 25.17 l 250 16.83 l 253.33 16.83 l 253.33 20.17 l 256.67 20.17 x s 261.67 25.17 l 261.67 18.5 l 270 18.5 l 270 21.83 l 268.33 21.83 l 268.33 23.5 l 270 23.5 l 270 25.17 x s 265 21.77 l 266.67 21.77 l 266.67 20.23 l 265 20.23 x s 278.33 18.5 l 278.33 20.17 l 275 20.17 l 275 25.17 l 271.67 25.17 l 271.67 18.5 x s 280 25.17 l 280 18.5 l 288.33 18.5 l 288.33 21.83 l 286.67 21.83 l 286.67 23.5 l 288.33 23.5 l 288.33 25.17 x s 283.33 21.77 l 285 21.77 l 285 20.23 l 283.33 20.23 x s 290 21.83 l 290 20.17 l 298.33 20.17 l 298.33 21.83 x s 300 16.83 l 303.33 16.83 l 303.33 18.5 l 305 18.5 l 305 20.17 l 306.67 20.17 l 306.67 21.83 l 305 21.83 l 305 23.5 l 303.33 23.5 l 303.33 25.17 l 300 25.17 l 300 23.5 l 301.67 23.5 l 301.67 21.83 l 303.33 21.83 l 303.33 20.17 l 301.67 20.17 l 301.67 18.5 l 300 18.5 x</PATH>
    <RECT pos="8 57 460 59" fill="linear: 100 16, 100 63, 0=ff000000, 1=279a3c3c"
          hasStroke="1" stroke="1, mitered, butt" strokeColour="solid: b3000000"/>
    <RECT pos="0 149 100% 33" fill="linear: 69 168, 69 294, 0=0, 1=ff000000"
          hasStroke="0"/>
  </BACKGROUND>
  <LABEL name="new label" id="84701c0c650fe2e" memberName="chordNameLabel"
         virtualName="" explicitFocusOrder="0" pos="8 57 460 59" edTextCol="ff000000"
         edBkgCol="0" labelText="G#Maj13b5#9" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="46.0" kerning="0.0" bold="1" italic="0" justification="36"
         typefaceStyle="Bold"/>
  <GENERICCOMPONENT name="" id="8e79ee27297830eb" memberName="chordKeyboard" virtualName=""
                    explicitFocusOrder="0" pos="8 124 16M 47" class="ChordAnalyzerKeyboardComponent"
                    params="ownerFilter-&gt;chordKbState, ownerFilter"/>
  <JUCERCOMP name="" id="bad9b853cca3ec4a" memberName="pizButton" virtualName=""
             explicitFocusOrder="0" pos="49 9 74 40" sourceFile="../_common/PizButton.cpp"
             constructorParams=""/>
  <TOGGLEBUTTON name="new toggle button" id="fb8f2d76e48f6291" memberName="flatsButton"
                virtualName="" explicitFocusOrder="0" pos="472 97 48 24" buttonText="flats"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="copy" id="7eadb69efd0e274e" memberName="copyButton" virtualName=""
              explicitFocusOrder="0" pos="544 103 48 21" buttonText="Copy"
              connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="channel" id="51b00c0c435d1b05" memberName="learnChanSlider"
          virtualName="ChannelSlider" explicitFocusOrder="0" pos="475 81 43 16"
          tooltip="Chord Input Channel, used for Learn and left-click entering notes"
          bkgcol="ffffffff" min="0.0" max="16.0" int="1.0" style="LinearBar"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="9ec8a16d4577cd67" memberName="versionLabel"
         virtualName="" explicitFocusOrder="0" pos="339 23 58 24" edTextCol="ff000000"
         edBkgCol="0" labelText="99.99.99" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="12.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: snake_gif, 4914, "../../../../../Pictures/8/snake.gif"
static const unsigned char resource_MidiChordAnalyzerEditor_snake_gif[] = { 71, 73, 70, 56, 57, 97, 244, 1, 33, 1, 162, 0, 0, 0, 0, 0, 153, 51, 102, 204, 51, 102, 153, 153, 102, 255, 255, 153, 255, 255, 255, 0, 0, 0, 0, 0, 0, 33, 249, 4, 9, 0, 0, 5, 0, 44, 0, 0, 0, 0, 244, 1, 33, 1, 64, 8, 254, 0, 11, 8, 28, 72, 176, 160, 193, 131, 8, 19, 42, 92, 200, 176, 161, 195, 135, 16, 35, 74, 156, 72, 177, 162, 197, 139, 24, 51, 106, 220, 200, 177, 163, 199, 143, 32, 67, 138, 28, 73, 178, 164, 201, 147, 40, 83, 26, 4, 192, 178, 165, 75, 0, 4, 98, 202, 156, 73, 115, 230, 75, 150, 53, 111, 34, 188, 201, 83, 165, 207, 159, 64, 131, 10, 29, 74, 180, 168, 64, 158, 72, 97, 214, 92, 26, 83, 128, 211, 167, 76, 163, 74, 157, 74, 51, 169, 85, 151, 70, 179, 106, 221, 202, 181, 171, 87, 137, 46, 169, 138, 101, 250, 180, 172, 212, 0, 104, 209, 142, 93, 59, 246, 170, 213, 175, 71, 221, 222, 28, 64, 183, 174, 221, 187, 118, 229, 2, 128, 203, 183, 175, 95, 134, 110, 217, 82, 45, 75, 152, 112, 205, 180, 136, 17, 11, 94, 204, 184, 106, 203, 162, 86, 241, 74, 158, 76, 185, 178, 101, 186, 73, 255, 106, 222, 188, 53, 172, 216, 196, 160, 65, 47, 22, 45, 51, 113, 105, 210, 52, 11, 59, 109, 106, 54, 106, 218, 198, 76, 177, 126, 228, 121, 185, 182, 237, 219, 184, 51, 115, 222, 189, 83, 111, 82, 220, 117, 125, 235, 238, 232, 249, 112, 232, 227, 168, 97, 159, 86, 27, 85, 245, 82, 231, 51, 147, 43, 119, 252, 18, 35, 109, 224, 216, 179, 107, 151, 252, 150, 55, 81, 189, 219, 195, 254, 139, 15, 126, 21, 240, 205, 233, 205, 85, 171, 23, 128, 30, 54, 242, 215, 108, 215, 179, 207, 89, 125, 34, 210, 241, 248, 243, 3, 119, 235, 93, 163, 92, 253, 0, 6, 56, 217, 112, 96, 181, 52, 157, 124, 134, 69, 23, 90, 123, 10, 194, 23, 147, 105, 233, 169, 39, 88, 125, 14, 93, 39, 160, 126, 87, 93, 120, 151, 92, 253, 197, 21, 153, 134, 32, 134, 72, 25, 82, 20, 5, 198, 224, 131, 16, 46, 231, 32, 1, 41, 178, 182, 94, 99, 208, 173, 69, 97, 68, 22, 130, 168, 215, 69, 31, 138, 120, 163, 74, 224, 137, 232, 227, 143, 251, 233, 228, 159, 129, 237, 189, 71, 218, 113, 50, 73, 120, 162, 114, 178, 217, 247, 146, 142, 51, 150, 244, 27, 144, 3, 10, 103, 229, 92, 84, 102, 119, 229, 99, 16, 101, 104, 99, 148, 35, 21, 183, 164, 84, 242, 81, 101, 228, 138, 84, 53, 105, 221, 147, 33, 246, 20, 84, 142, 89, 198, 169, 225, 149, 67, 213, 136, 161, 144, 157, 189, 52, 38, 83, 71, 42, 198, 164, 154, 25, 217, 121, 161, 155, 70, 185, 37, 167, 128, 91, 150, 231, 29, 150, 249, 17, 10, 87, 162, 75, 182, 40, 21, 160, 82, 10, 26, 160, 163, 124, 37, 170, 169, 166, 29, 138, 100, 41, 118, 36, 118, 170, 144, 152, 75, 10, 39, 212, 167, 151, 134, 42, 234, 170, 29, 222, 23, 30, 129, 255, 172, 122, 138, 41, 103, 174, 254, 8, 107, 172, 184, 66, 198, 230, 171, 170, 230, 234, 43, 143, 181, 2, 201, 225, 175, 196, 206, 198, 40, 168, 179, 22, 171, 44, 80, 83, 30, 138, 217, 142, 203, 70, 75, 16, 170, 150, 225, 41, 237, 181, 121, 194, 233, 108, 149, 155, 38, 139, 45, 74, 212, 86, 11, 230, 183, 228, 62, 234, 229, 182, 131, 250, 86, 110, 160, 187, 106, 57, 238, 186, 240, 210, 42, 28, 186, 136, 118, 23, 111, 111, 46, 137, 231, 237, 189, 252, 174, 186, 37, 189, 219, 41, 90, 238, 177, 1, 247, 218, 239, 193, 223, 206, 11, 112, 109, 6, 19, 75, 240, 120, 251, 146, 219, 45, 180, 8, 107, 246, 223, 194, 121, 189, 203, 234, 195, 0, 54, 156, 105, 162, 0, 219, 91, 177, 185, 28, 31, 26, 177, 168, 225, 226, 55, 241, 202, 61, 98, 220, 241, 201, 35, 235, 154, 239, 194, 214, 58, 156, 178, 203, 56, 231, 44, 46, 165, 49, 107, 117, 51, 148, 60, 71, 123, 165, 206, 188, 178, 252, 175, 176, 26, 247, 92, 40, 199, 51, 235, 219, 18, 94, 53, 43, 61, 173, 209, 84, 171, 43, 179, 182, 238, 6, 45, 117, 86, 63, 215, 171, 245, 214, 96, 91, 212, 53, 212, 73, 135, 93, 231, 216, 42, 71, 109, 246, 218, 21, 161, 13, 51, 219, 103, 183, 59, 167, 218, 112, 215, 77, 163, 220, 35, 210, 109, 119, 87, 254, 88, 87, 70, 241, 222, 128, 151, 136, 247, 134, 111, 7, 110, 248, 225, 102, 31, 235, 49, 226, 140, 55, 14, 247, 223, 142, 71, 46, 57, 216, 2, 79, 110, 249, 229, 152, 103, 174, 249, 230, 13, 77, 60, 144, 200, 156, 135, 46, 122, 220, 74, 137, 133, 160, 146, 81, 89, 21, 27, 151, 163, 183, 238, 250, 231, 68, 46, 25, 227, 158, 12, 126, 189, 113, 213, 160, 191, 62, 185, 158, 48, 206, 78, 128, 239, 44, 34, 73, 251, 137, 182, 3, 107, 40, 210, 185, 235, 30, 51, 239, 195, 143, 214, 39, 154, 193, 67, 63, 83, 153, 165, 66, 78, 92, 176, 68, 63, 187, 184, 242, 9, 225, 62, 236, 73, 204, 243, 41, 169, 138, 204, 173, 133, 230, 153, 207, 157, 94, 216, 82, 11, 182, 87, 118, 129, 104, 103, 63, 192, 173, 145, 91, 137, 179, 245, 157, 159, 199, 190, 159, 140, 1, 223, 94, 107, 233, 3, 224, 153, 144, 227, 190, 226, 225, 171, 105, 242, 187, 211, 246, 14, 214, 178, 4, 22, 76, 111, 7, 49, 81, 255, 214, 55, 189, 4, 53, 79, 124, 126, 26, 95, 0, 19, 52, 59, 8, 142, 106, 112, 14, 4, 218, 2, 253, 117, 174, 16, 34, 239, 125, 5, 225, 201, 5, 141, 195, 191, 21, 210, 68, 122, 73, 82, 223, 124, 82, 231, 193, 20, 130, 112, 110, 222, 43, 217, 203, 232, 199, 181, 227, 153, 80, 103, 35, 254, 188, 27, 78, 138, 52, 192, 243, 181, 80, 134, 171, 89, 12, 234, 38, 197, 58, 248, 61, 173, 77, 53, 92, 83, 252, 32, 102, 53, 177, 217, 79, 126, 57, 68, 160, 8, 89, 50, 36, 223, 160, 199, 72, 27, 68, 208, 84, 132, 55, 33, 30, 154, 71, 139, 94, 107, 34, 184, 154, 245, 195, 54, 146, 199, 74, 97, 210, 33, 21, 13, 216, 69, 43, 209, 142, 130, 228, 43, 98, 0, 100, 164, 70, 28, 201, 177, 81, 81, 36, 73, 9, 221, 184, 195, 68, 145, 236, 137, 10, 164, 227, 169, 98, 215, 60, 35, 194, 112, 41, 252, 49, 214, 13, 1, 137, 66, 102, 129, 44, 145, 89, 12, 100, 127, 176, 167, 29, 51, 182, 138, 145, 46, 164, 14, 23, 125, 50, 197, 32, 105, 146, 123, 188, 225, 36, 178, 78, 185, 73, 163, 201, 164, 112, 223, 41, 229, 42, 89, 137, 74, 159, 169, 50, 55, 176, 172, 165, 73, 216, 184, 69, 90, 234, 114, 151, 127, 220, 153, 47, 127, 249, 147, 190, 245, 210, 147, 196, 220, 200, 20, 145, 153, 204, 191, 12, 210, 89, 158, 107, 230, 74, 74, 57, 76, 105, 202, 235, 153, 63, 236, 150, 228, 130, 233, 183, 74, 90, 115, 89, 87, 36, 100, 55, 43, 183, 53, 110, 230, 205, 155, 223, 172, 216, 208, 196, 249, 70, 102, 74, 203, 156, 227, 84, 100, 58, 247, 22, 206, 108, 6, 17, 87, 240, 100, 255, 24, 58, 231, 121, 185, 139, 17, 205, 157, 173, 68, 99, 218, 170, 217, 151, 110, 101, 205, 84, 252, 156, 230, 45, 77, 150, 203, 107, 10, 180, 144, 251, 228, 200, 202, 8, 217, 208, 95, 202, 50, 85, 17, 221, 204, 69, 103, 153, 201, 73, 178, 179, 147, 4, 125, 221, 70, 41, 217, 199, 98, 25, 243, 163, 40, 197, 104, 73, 249, 105, 169, 124, 114, 71, 110, 33, 197, 39, 203, 160, 233, 61, 39, 30, 141, 164, 163, 76, 168, 66, 17, 201, 80, 121, 26, 110, 101, 202, 90, 168, 62, 125, 218, 76, 151, 98, 114, 47, 58, 189, 22, 53, 51, 90, 84, 163, 62, 144, 168, 73, 237, 84, 62, 43, 202, 210, 145, 18, 238, 158, 81, 245, 21, 55, 169, 154, 85, 216, 213, 180, 171, 253, 106, 41, 86, 193, 74, 86, 145, 178, 105, 172, 101, 77, 171, 89, 147, 167, 214, 182, 74, 147, 169, 110, 141, 235, 110, 182, 36, 215, 186, 106, 238, 42, 175, 4, 101, 94, 233, 100, 215, 190, 150, 147, 84, 43, 100, 153, 95, 7, 155, 74, 71, 1, 54, 66, 72, 156, 97, 237, 86, 106, 67, 168, 18, 246, 177, 236, 26, 34, 1, 146, 18, 31, 255, 5, 86, 56, 147, 101, 44, 100, 55, 235, 145, 195, 254, 15, 143, 161, 44, 163, 33, 99, 5, 84, 206, 222, 203, 179, 166, 179, 96, 106, 84, 155, 199, 242, 133, 118, 49, 142, 37, 101, 71, 254, 217, 106, 218, 194, 202, 229, 64, 137, 221, 95, 251, 94, 11, 91, 205, 46, 242, 164, 3, 165, 109, 109, 15, 201, 219, 177, 44, 104, 124, 26, 172, 137, 101, 27, 3, 80, 144, 248, 176, 167, 104, 29, 46, 233, 74, 87, 92, 22, 58, 40, 57, 187, 173, 160, 24, 47, 203, 213, 135, 240, 50, 103, 194, 141, 170, 96, 127, 43, 217, 179, 232, 241, 145, 186, 117, 109, 17, 201, 130, 68, 12, 162, 87, 180, 72, 149, 164, 71, 129, 216, 220, 208, 205, 118, 83, 178, 42, 175, 117, 165, 35, 29, 247, 144, 113, 181, 137, 85, 44, 138, 146, 203, 92, 184, 118, 207, 170, 232, 170, 47, 226, 46, 121, 76, 114, 90, 81, 175, 209, 131, 207, 0, 171, 235, 63, 208, 146, 241, 191, 5, 142, 173, 135, 230, 43, 78, 5, 135, 205, 55, 38, 252, 222, 66, 144, 50, 70, 215, 174, 229, 116, 20, 110, 205, 121, 251, 251, 59, 208, 66, 146, 169, 8, 118, 96, 120, 151, 247, 220, 148, 106, 47, 136, 18, 84, 14, 104, 151, 200, 91, 8, 17, 24, 192, 40, 182, 137, 111, 207, 248, 80, 27, 95, 38, 146, 12, 196, 166, 145, 207, 9, 87, 253, 77, 48, 70, 172, 29, 240, 123, 157, 199, 191, 31, 187, 8, 128, 49, 76, 162, 40, 227, 219, 37, 167, 46, 185, 157, 209, 37, 225, 119, 191, 204, 209, 33, 31, 240, 179, 212, 107, 109, 143, 91, 254, 8, 228, 237, 78, 101, 159, 49, 38, 243, 252, 144, 44, 102, 161, 202, 25, 164, 167, 196, 236, 152, 88, 236, 26, 19, 15, 207, 197, 109, 201, 168, 151, 113, 58, 211, 47, 57, 248, 43, 13, 188, 51, 14, 53, 220, 88, 8, 55, 38, 69, 4, 36, 159, 118, 131, 124, 226, 23, 137, 69, 195, 131, 54, 37, 126, 167, 166, 100, 136, 122, 56, 191, 99, 86, 180, 143, 186, 27, 65, 71, 83, 121, 189, 121, 204, 50, 84, 38, 45, 96, 193, 68, 121, 117, 57, 181, 41, 75, 26, 204, 101, 101, 134, 58, 141, 34, 238, 172, 63, 219, 104, 80, 42, 133, 249, 204, 12, 194, 238, 132, 85, 157, 230, 226, 194, 152, 195, 69, 51, 115, 29, 145, 173, 210, 251, 218, 25, 99, 206, 46, 242, 81, 109, 237, 197, 233, 100, 87, 185, 88, 126, 117, 107, 217, 156, 166, 79, 111, 120, 214, 80, 100, 180, 147, 158, 109, 171, 170, 37, 144, 174, 212, 206, 244, 145, 13, 236, 196, 226, 186, 249, 133, 72, 186, 54, 19, 149, 157, 63, 105, 7, 151, 222, 186, 38, 183, 168, 211, 197, 87, 240, 169, 155, 201, 177, 22, 228, 150, 214, 188, 98, 248, 74, 148, 217, 79, 197, 119, 72, 110, 189, 239, 131, 194, 113, 105, 246, 46, 115, 192, 179, 69, 221, 225, 161, 198, 202, 221, 158, 184, 20, 121, 106, 104, 113, 47, 59, 206, 231, 142, 166, 95, 254, 157, 255, 49, 118, 251, 91, 178, 120, 221, 51, 183, 97, 253, 235, 10, 145, 92, 152, 26, 143, 101, 141, 93, 150, 197, 92, 189, 124, 206, 38, 95, 163, 218, 80, 27, 90, 143, 11, 145, 227, 184, 86, 184, 241, 162, 125, 223, 120, 233, 27, 230, 62, 231, 10, 207, 47, 152, 115, 89, 131, 59, 220, 77, 183, 100, 38, 23, 60, 82, 111, 91, 108, 98, 52, 172, 162, 212, 35, 126, 239, 152, 75, 87, 190, 92, 71, 186, 215, 181, 42, 244, 92, 31, 18, 232, 139, 142, 250, 215, 27, 141, 118, 137, 215, 122, 109, 73, 223, 58, 194, 19, 174, 246, 218, 30, 157, 91, 164, 246, 235, 221, 157, 214, 242, 175, 111, 180, 239, 118, 223, 123, 178, 1, 79, 216, 76, 19, 126, 237, 12, 79, 187, 213, 219, 42, 203, 195, 175, 157, 237, 115, 63, 234, 226, 147, 234, 229, 188, 63, 158, 200, 32, 119, 187, 217, 41, 95, 249, 152, 94, 222, 187, 192, 141, 211, 87, 185, 39, 248, 27, 11, 253, 243, 31, 207, 60, 191, 199, 107, 57, 195, 215, 29, 245, 15, 78, 188, 27, 251, 29, 56, 215, 199, 29, 246, 7, 159, 121, 74, 31, 14, 247, 200, 151, 252, 244, 184, 31, 122, 162, 119, 79, 231, 158, 169, 219, 242, 193, 47, 230, 240, 81, 90, 124, 126, 253, 27, 249, 201, 191, 218, 174, 59, 220, 124, 108, 145, 28, 250, 209, 71, 244, 244, 121, 61, 121, 219, 254, 182, 221, 237, 217, 79, 24, 136, 237, 137, 253, 143, 133, 221, 54, 229, 183, 185, 200, 197, 171, 123, 250, 122, 254, 236, 0, 104, 246, 237, 225, 79, 116, 199, 103, 174, 253, 52, 123, 191, 87, 110, 78, 182, 215, 203, 182, 215, 242, 215, 125, 246, 21, 122, 162, 151, 126, 61, 196, 127, 87, 165, 127, 215, 179, 41, 255, 100, 127, 156, 35, 123, 190, 166, 128, 20, 247, 125, 132, 54, 127, 59, 5, 128, 31, 229, 128, 15, 136, 128, 131, 247, 118, 234, 119, 126, 29, 88, 127, 28, 24, 98, 18, 120, 127, 170, 231, 112, 30, 72, 90, 165, 215, 112, 162, 102, 128, 38, 232, 123, 65, 247, 78, 16, 200, 130, 13, 87, 130, 119, 53, 130, 154, 54, 118, 191, 178, 125, 52, 136, 119, 12, 182, 45, 54, 120, 131, 48, 56, 109, 70, 247, 131, 36, 40, 130, 60, 72, 132, 93, 197, 52, 67, 104, 122, 241, 247, 123, 58, 232, 124, 230, 166, 121, 235, 87, 41, 227, 199, 119, 192, 87, 75, 56, 136, 126, 254, 7, 47, 72, 232, 130, 2, 119, 124, 93, 184, 57, 39, 56, 84, 89, 24, 126, 145, 5, 130, 253, 119, 134, 168, 84, 134, 72, 135, 134, 228, 69, 129, 241, 20, 133, 243, 228, 134, 0, 151, 130, 112, 120, 114, 114, 232, 131, 116, 88, 135, 91, 200, 135, 120, 152, 135, 192, 4, 130, 96, 232, 58, 43, 40, 113, 130, 24, 135, 255, 79, 104, 134, 108, 248, 86, 39, 40, 128, 137, 232, 71, 97, 87, 136, 90, 120, 136, 96, 54, 134, 145, 248, 115, 79, 119, 135, 129, 88, 87, 69, 151, 137, 7, 248, 125, 144, 8, 86, 218, 4, 138, 230, 7, 116, 26, 104, 138, 170, 168, 135, 79, 151, 138, 171, 248, 138, 86, 248, 52, 174, 8, 139, 180, 8, 106, 179, 88, 139, 184, 152, 111, 183, 152, 139, 188, 184, 128, 65, 216, 139, 192, 24, 140, 194, 216, 54, 178, 49, 138, 195, 120, 140, 46, 55, 90, 94, 101, 129, 200, 104, 138, 120, 229, 100, 49, 193, 41, 205, 56, 141, 165, 70, 89, 108, 97, 141, 153, 133, 66, 187, 72, 141, 118, 5, 141, 220, 85, 138, 220, 40, 136, 75, 215, 102, 148, 70, 31, 213, 22, 141, 141, 24, 142, 144, 53, 142, 196, 38, 67, 99, 194, 140, 234, 152, 80, 229, 17, 62, 83, 17, 96, 173, 134, 30, 227, 82, 125, 241, 152, 85, 122, 54, 22, 203, 213, 60, 224, 184, 143, 100, 5, 88, 244, 152, 90, 128, 230, 66, 170, 99, 142, 157, 40, 144, 126, 168, 95, 12, 242, 143, 213, 165, 144, 11, 201, 144, 214, 196, 142, 79, 166, 109, 17, 41, 145, 188, 87, 103, 250, 72, 145, 255, 87, 113, 174, 182, 106, 216, 166, 109, 242, 22, 74, 202, 168, 81, 179, 229, 145, 131, 8, 146, 149, 246, 106, 128, 22, 105, 25, 121, 105, 233, 200, 138, 255, 206, 166, 146, 105, 152, 144, 189, 147, 91, 251, 181, 114, 49, 249, 98, 125, 56, 93, 73, 232, 132, 90, 103, 147, 160, 151, 99, 176, 161, 147, 13, 82, 146, 61, 169, 144, 218, 215, 105, 33, 136, 137, 193, 104, 145, 232, 81, 108, 219, 182, 114, 83, 54, 105, 239, 8, 143, 177, 103, 137, 92, 120, 104, 68, 217, 104, 75, 185, 147, 123, 20, 97, 99, 153, 148, 101, 201, 94, 16, 9, 95, 94, 185, 112, 65, 169, 120, 191, 152, 139, 222, 24, 147, 237, 195, 95, 4, 246, 110, 196, 179, 121, 96, 215, 132, 158, 166, 149, 184, 40, 149, 122, 196, 24, 187, 213, 34, 37, 233, 142, 188, 197, 151, 5, 64, 128, 208, 245, 150, 176, 88, 144, 233, 133, 106, 230, 3, 105, 62, 198, 98, 168, 131, 145, 24, 199, 71, 63, 41, 56, 118, 56, 71, 148, 136, 57, 83, 39, 119, 102, 178, 98, 103, 249, 153, 43, 50, 108, 35, 73, 152, 240, 86, 153, 50, 57, 147, 213, 248, 135, 1, 168, 154, 219, 244, 133, 80, 9, 150, 125, 70, 96, 168, 89, 98, 48, 201, 106, 237, 117, 154, 60, 201, 24, 90, 153, 153, 110, 121, 153, 47, 8, 155, 248, 179, 113, 44, 41, 101, 5, 71, 68, 124, 6, 60, 190, 163, 148, 176, 97, 128, 190, 217, 113, 138, 169, 52, 154, 178, 151, 195, 73, 140, 14, 105, 156, 231, 229, 110, 21, 166, 90, 204, 137, 143, 255, 239, 199, 149, 208, 182, 141, 8, 179, 78, 5, 136, 80, 91, 201, 146, 23, 247, 30, 97, 116, 71, 218, 134, 71, 160, 121, 149, 111, 102, 96, 51, 72, 126, 209, 57, 48, 10, 227, 126, 51, 6, 121, 215, 105, 156, 23, 121, 144, 46, 100, 65, 239, 9, 159, 243, 150, 133, 224, 153, 61, 249, 73, 57, 87, 120, 132, 249, 169, 66, 127, 166, 62, 41, 54, 67, 217, 89, 143, 60, 38, 100, 152, 198, 154, 33, 99, 140, 214, 135, 127, 179, 55, 99, 41, 135, 102, 102, 65, 149, 175, 37, 26, 181, 233, 160, 91, 38, 137, 106, 200, 124, 24, 186, 131, 203, 103, 100, 205, 101, 148, 33, 137, 101, 45, 134, 145, 43, 20, 153, 240, 233, 28, 75, 196, 110, 22, 138, 159, 226, 57, 87, 78, 41, 103, 45, 71, 98, 57, 41, 146, 196, 230, 94, 123, 50, 151, 187, 217, 142, 90, 22, 163, 66, 154, 141, 192, 201, 105, 39, 250, 101, 29, 41, 126, 136, 233, 163, 128, 39, 149, 97, 100, 151, 100, 233, 103, 214, 214, 66, 86, 150, 150, 18, 57, 110, 122, 73, 124, 7, 74, 118, 243, 89, 131, 221, 101, 165, 104, 249, 110, 24, 22, 108, 38, 246, 99, 36, 154, 113, 77, 154, 163, 220, 183, 163, 161, 248, 156, 10, 10, 85, 140, 121, 148, 7, 201, 103, 141, 180, 155, 185, 57, 160, 192, 105, 167, 115, 154, 162, 210, 87, 160, 138, 214, 80, 255, 64, 26, 74, 124, 186, 148, 170, 37, 163, 76, 170, 112, 134, 122, 168, 81, 138, 146, 101, 218, 131, 128, 56, 145, 203, 232, 162, 91, 202, 109, 108, 54, 62, 75, 170, 99, 142, 234, 147, 192, 39, 168, 156, 120, 159, 208, 73, 167, 121, 73, 170, 98, 26, 91, 154, 234, 95, 142, 196, 165, 71, 84, 142, 45, 25, 170, 20, 138, 169, 7, 180, 137, 39, 148, 67, 180, 230, 154, 108, 217, 163, 150, 138, 103, 113, 151, 167, 128, 153, 65, 24, 214, 34, 72, 233, 143, 150, 22, 159, 77, 234, 164, 184, 10, 117, 247, 53, 106, 107, 169, 136, 170, 122, 103, 229, 135, 166, 179, 233, 152, 89, 218, 142, 184, 121, 143, 6, 9, 163, 44, 119, 158, 229, 86, 157, 171, 25, 169, 115, 72, 168, 154, 56, 173, 102, 26, 172, 166, 38, 24, 17, 138, 157, 72, 26, 150, 123, 101, 171, 23, 216, 172, 173, 185, 172, 31, 68, 174, 229, 138, 170, 223, 134, 175, 229, 217, 150, 191, 89, 71, 108, 106, 164, 164, 249, 166, 213, 197, 170, 97, 42, 118, 242, 234, 116, 123, 40, 121, 99, 90, 148, 26, 218, 128, 19, 149, 171, 246, 58, 175, 56, 169, 28, 164, 121, 101, 88, 202, 66, 5, 52, 76, 172, 185, 153, 251, 186, 176, 65, 39, 156, 244, 122, 63, 206, 22, 174, 51, 121, 37, 155, 202, 167, 127, 218, 152, 181, 137, 142, 222, 214, 177, 177, 201, 172, 255, 79, 218, 117, 68, 39, 99, 1, 73, 177, 7, 91, 174, 58, 87, 156, 205, 227, 165, 85, 121, 164, 95, 122, 147, 139, 168, 120, 41, 49, 165, 206, 234, 61, 6, 122, 179, 14, 251, 135, 245, 185, 140, 218, 153, 172, 98, 169, 22, 139, 202, 148, 31, 215, 75, 31, 41, 167, 191, 154, 175, 147, 106, 157, 57, 251, 82, 134, 153, 169, 15, 10, 60, 239, 105, 153, 19, 59, 175, 86, 171, 124, 252, 154, 181, 8, 219, 176, 231, 57, 178, 154, 89, 182, 231, 122, 142, 178, 243, 169, 99, 75, 182, 185, 7, 178, 116, 151, 176, 190, 136, 181, 233, 42, 174, 113, 212, 181, 80, 8, 183, 96, 42, 183, 51, 218, 174, 215, 232, 154, 22, 234, 177, 152, 55, 179, 106, 27, 184, 120, 121, 181, 67, 91, 129, 122, 203, 138, 46, 36, 176, 90, 122, 184, 130, 171, 159, 13, 38, 115, 232, 138, 69, 155, 102, 75, 140, 123, 169, 103, 87, 184, 163, 9, 180, 26, 121, 123, 153, 169, 184, 245, 150, 182, 119, 138, 110, 240, 39, 127, 35, 183, 174, 23, 132, 154, 91, 59, 184, 110, 43, 121, 117, 202, 183, 18, 171, 180, 5, 5, 184, 170, 75, 156, 122, 161, 114, 240, 25, 179, 135, 153, 186, 196, 27, 183, 161, 75, 83, 84, 115, 59, 201, 155, 128, 153, 187, 146, 253, 24, 176, 118, 75, 147, 183, 219, 117, 251, 151, 160, 202, 59, 122, 31, 216, 188, 255, 66, 57, 185, 167, 200, 179, 38, 201, 171, 139, 27, 185, 33, 235, 189, 208, 107, 170, 93, 41, 130, 246, 249, 124, 250, 58, 116, 49, 121, 188, 153, 90, 189, 111, 43, 190, 186, 40, 178, 210, 24, 86, 47, 215, 190, 238, 59, 60, 230, 185, 179, 220, 171, 181, 14, 101, 191, 207, 27, 84, 215, 247, 187, 91, 231, 178, 193, 171, 172, 126, 219, 171, 128, 235, 181, 77, 107, 139, 174, 219, 122, 85, 103, 192, 114, 183, 41, 91, 54, 192, 206, 149, 163, 20, 60, 92, 5, 170, 191, 21, 108, 171, 67, 121, 189, 13, 44, 186, 195, 8, 158, 30, 252, 73, 27, 12, 188, 228, 187, 122, 240, 187, 142, 19, 252, 192, 204, 75, 191, 231, 43, 191, 146, 107, 190, 201, 103, 194, 41, 156, 76, 149, 154, 183, 24, 204, 193, 92, 105, 174, 164, 248, 156, 108, 139, 123, 47, 12, 195, 58, 37, 168, 64, 220, 87, 69, 252, 181, 3, 105, 167, 209, 74, 196, 23, 117, 194, 252, 200, 186, 14, 156, 195, 242, 104, 123, 45, 92, 86, 70, 75, 179, 76, 204, 121, 77, 104, 197, 90, 76, 197, 165, 218, 197, 233, 212, 120, 70, 236, 137, 59, 140, 133, 67, 28, 87, 75, 117, 198, 74, 156, 198, 106, 252, 196, 104, 60, 179, 82, 172, 119, 98, 188, 182, 96, 44, 132, 116, 156, 197, 166, 5, 199, 53, 92, 199, 18, 236, 123, 121, 252, 88, 190, 10, 173, 255, 247, 123, 196, 157, 199, 199, 62, 236, 199, 245, 26, 193, 22, 149, 200, 100, 140, 122, 133, 28, 129, 57, 100, 136, 131, 54, 200, 210, 229, 175, 23, 202, 122, 193, 73, 195, 119, 104, 147, 15, 11, 177, 135, 92, 63, 95, 172, 200, 209, 167, 201, 173, 43, 199, 143, 83, 202, 145, 156, 136, 147, 76, 159, 11, 44, 53, 88, 220, 195, 189, 136, 202, 27, 26, 203, 35, 51, 203, 54, 204, 141, 216, 187, 170, 143, 75, 99, 161, 139, 201, 226, 88, 79, 44, 90, 187, 167, 197, 202, 166, 188, 138, 196, 108, 99, 191, 124, 204, 201, 43, 204, 202, 140, 190, 190, 236, 198, 223, 187, 194, 57, 184, 203, 95, 25, 191, 161, 108, 179, 107, 172, 162, 205, 11, 205, 188, 72, 158, 25, 216, 205, 50, 197, 189, 212, 28, 142, 226, 124, 203, 228, 44, 85, 7, 11, 200, 251, 216, 203, 176, 156, 204, 111, 210, 206, 238, 44, 144, 240, 28, 66, 235, 236, 125, 158, 204, 136, 180, 156, 205, 201, 104, 203, 225, 9, 206, 97, 136, 183, 252, 140, 205, 254, 204, 181, 0, 77, 47, 73, 156, 187, 123, 105, 124, 184, 83, 85, 9, 157, 96, 2, 157, 193, 255, 123, 137, 253, 124, 117, 245, 215, 144, 219, 12, 132, 19, 93, 191, 214, 252, 148, 6, 93, 205, 2, 124, 209, 140, 179, 209, 137, 41, 195, 16, 71, 208, 184, 116, 206, 169, 154, 206, 192, 218, 204, 255, 164, 247, 202, 114, 194, 210, 177, 184, 207, 224, 183, 40, 24, 120, 170, 245, 140, 160, 140, 204, 194, 36, 205, 185, 42, 189, 210, 52, 189, 186, 70, 40, 209, 59, 253, 87, 61, 77, 157, 54, 195, 180, 71, 253, 177, 247, 12, 94, 29, 13, 204, 157, 187, 134, 40, 93, 168, 31, 205, 197, 33, 189, 180, 12, 200, 78, 81, 237, 208, 83, 237, 184, 38, 101, 135, 198, 76, 212, 57, 205, 204, 77, 205, 211, 35, 76, 119, 4, 156, 214, 206, 59, 210, 108, 77, 178, 242, 92, 55, 186, 155, 175, 107, 29, 212, 60, 28, 109, 141, 75, 213, 63, 253, 154, 115, 93, 197, 123, 13, 148, 54, 253, 199, 101, 157, 215, 156, 200, 126, 111, 125, 205, 224, 244, 213, 132, 221, 175, 85, 189, 59, 135, 45, 212, 127, 109, 213, 129, 189, 216, 227, 220, 202, 63, 213, 215, 122, 173, 84, 73, 77, 217, 33, 103, 217, 151, 93, 209, 29, 40, 165, 119, 204, 217, 39, 189, 132, 152, 29, 184, 235, 27, 209, 212, 247, 172, 51, 29, 215, 118, 243, 213, 67, 253, 193, 43, 58, 168, 34, 107, 200, 141, 29, 200, 15, 101, 206, 168, 232, 218, 128, 45, 205, 112, 237, 214, 91, 156, 190, 145, 237, 216, 2, 37, 134, 90, 20, 219, 74, 55, 49, 187, 234, 200, 233, 182, 204, 47, 157, 213, 163, 35, 40, 198, 205, 113, 200, 221, 187, 192, 205, 187, 51, 108, 85, 254, 213, 189, 202, 143, 253, 134, 128, 115, 221, 170, 140, 182, 63, 204, 219, 175, 221, 221, 58, 203, 215, 53, 137, 50, 150, 184, 221, 220, 13, 218, 136, 125, 208, 197, 139, 204, 195, 77, 220, 238, 13, 217, 208, 93, 203, 151, 76, 222, 229, 109, 222, 126, 13, 223, 31, 171, 214, 97, 204, 223, 153, 237, 223, 241, 189, 199, 158, 221, 56, 152, 205, 222, 193, 231, 84, 93, 157, 56, 125, 173, 224, 80, 60, 119, 103, 189, 223, 2, 46, 148, 4, 254, 223, 246, 125, 223, 186, 163, 187, 16, 190, 224, 8, 215, 224, 237, 93, 223, 128, 120, 225, 241, 123, 27, 32, 46, 215, 26, 220, 225, 30, 126, 126, 39, 142, 226, 91, 184, 208, 112, 137, 108, 45, 238, 226, 28, 152, 207, 232, 204, 97, 51, 78, 79, 49, 102, 227, 234, 184, 85, 42, 94, 210, 126, 204, 227, 239, 60, 95, 57, 254, 217, 192, 21, 194, 36, 46, 179, 0, 28, 120, 53, 151, 228, 247, 154, 219, 19, 78, 134, 203, 235, 228, 100, 77, 190, 48, 142, 200, 253, 75, 229, 90, 221, 214, 243, 173, 229, 17, 206, 83, 81, 238, 229, 132, 188, 43, 69, 46, 230, 110, 133, 37, 101, 110, 230, 140, 7, 211, 106, 94, 194, 99, 221, 230, 192, 24, 222, 112, 30, 149, 66, 62, 231, 160, 40, 231, 118, 142, 223, 97, 158, 231, 46, 172, 223, 124, 254, 231, 128, 78, 139, 1, 1, 0, 59, 0, 0 };

const char* MidiChordAnalyzerEditor::snake_gif   = (const char*) resource_MidiChordAnalyzerEditor_snake_gif;
const int MidiChordAnalyzerEditor::snake_gifSize = 4914;

//[EndFile] You can add extra defines here...
//[/EndFile]
