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
#include "juce_audio_utils/juce_audio_utils.h"

#include "MidiChordAnalyzer.h"
#include "../_common/ChannelSlider.h"
#include "../_common/VSTSlider.h"

using namespace juce;

class MidiChordAnalyzerEditor;
class ChordAnalyzerKeyboardComponent : public MidiKeyboardComponent
{
public:
	ChordAnalyzerKeyboardComponent(MidiKeyboardState& kbstate, MidiChordAnalyzer* ownerFilter)
		: MidiKeyboardComponent(kbstate,MidiKeyboardComponent::horizontalKeyboard),
		owner(0)
	{
		owner = ownerFilter;
		s = &kbstate;
		this->setMidiChannel(1);
		this->setLowestVisibleKey(36);
	}
	~ChordAnalyzerKeyboardComponent() override {};

	int getNumHeldNotes(int channel)
	{
		int num=0;
		for (int i=1;i<=16;i++) {
			if (channel==0 || channel==i) {
				for (int n=0;n<128;n++)
				{
					if (s->isNoteOn(i, n))
						++num;
				}
			}
		}
		return num;
	}

private:
	MidiChordAnalyzer* owner;
	MidiKeyboardState* s;

	bool mouseDownOnKey(int midiNoteNumber, const MouseEvent &e) override {
		MidiChordAnalyzerEditor* editor = ((MidiChordAnalyzerEditor*)(this->getParentComponent()));
		if (e.mods.isPopupMenu()) {
			s->allNotesOff(this->getMidiChannel());
			return false;
		}
		else {
			if (s->isNoteOn(this->getMidiChannel(),midiNoteNumber)) {
				s->noteOff(this->getMidiChannel(),midiNoteNumber,1.f);
			}
			else {
				s->noteOn(this->getMidiChannel(),midiNoteNumber,1.f);
			}
			return false;
		}
		return true;
	}
};
//[/Headers]

#include "../_common/PizButton.h"


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiChordAnalyzerEditor  : public juce::AudioProcessorEditor,
                                 public juce::ChangeListener,
                                 public juce::Timer,
                                 public juce::Button::Listener,
                                 public juce::Slider::Listener
{
public:
    //==============================================================================
    MidiChordAnalyzerEditor (MidiChordAnalyzer* const ownerFilter);
    ~MidiChordAnalyzerEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	friend class ChordAnalyzerKeyboardComponent;
	void changeListenerCallback (ChangeBroadcaster* source) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDoubleClick (const MouseEvent& e) override;
	void mouseUp (const MouseEvent& e) override;
	String const getCurrentChordName(int channel);
	void timerCallback() override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;

    // Binary resources:
    static const char* snake_gif;
    static const int snake_gifSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    TooltipWindow tooltipWindow;
    void updateParametersFromFilter();
	int numHeldNotes;

    MidiChordAnalyzer* getFilter() const throw()       { return (MidiChordAnalyzer*) getAudioProcessor(); }
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> chordNameLabel;
    std::unique_ptr<ChordAnalyzerKeyboardComponent> chordKeyboard;
    std::unique_ptr<PizButton> pizButton;
    std::unique_ptr<juce::ToggleButton> flatsButton;
    std::unique_ptr<juce::TextButton> copyButton;
    std::unique_ptr<ChannelSlider> learnChanSlider;
    std::unique_ptr<juce::Label> versionLabel;
    juce::Image cachedImage_snake_gif_1;
    juce::Path internalPath2;
    juce::Path internalPath3;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiChordAnalyzerEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

