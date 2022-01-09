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
#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "MidiChords.h"
#include "../_common/ChannelSlider.h"
#include "../_common/NoteSlider.h"
#include "../_common/VSTSlider.h"
#include "../_common/GuitarNeckComponent.h"

class FretsSlider : public juce::Slider
{
public:
	FretsSlider(String name) : juce::Slider(name)	{};
	~FretsSlider() override {};

	String getTextFromValue(double value) override
	{
		const int n = roundToInt(value);
		return "Frets: "+String(n);
	}
};

class StringsSlider : public juce::Slider
{
public:
	StringsSlider(String name) : juce::Slider(name)	{};
	~StringsSlider() override {};

	String getTextFromValue(double value) override
	{
		const int n = roundToInt(value);
		return "Strings: "+String(n);
	}
};

class ChordPresetFileFilter : public juce::FileFilter
{
public:
	ChordPresetFileFilter()
		: juce::FileFilter("midiChords presets") {}
	~ChordPresetFileFilter() override {}
	bool isFileSuitable (const File &file) const override
	{
		return (file.hasFileExtension("chords")
			|| file.hasFileExtension("fxp")
			|| file.hasFileExtension("xml"));
	}
	bool isDirectorySuitable (const File &file) const override
	{
		return true;
	}
};

class ChordsGuitar : public GuitarNeckComponent
{
public:
	ChordsGuitar(MidiKeyboardState &state, MidiChords* ownerFilter)
		: GuitarNeckComponent(state),
		owner(0),
		erasing(false)
	{
		owner = ownerFilter;
		s = &state;
	}
private:
	MidiChords* owner;
    MidiKeyboardState* s;
	FrettedNote lastDraggedNote;
	bool erasing;

	void mouseDraggedToKey(int fret, int string, const MouseEvent& e) override
	{
		FrettedNote n(fret,string);
		if (n!=lastDraggedNote)
		{
			int midiNoteNumber = getNote(n);
			int oldNoteOnString = getNote(FrettedNote(getStringFret(string),string));
			if (s->isNoteOn(string+1,midiNoteNumber)) {
				/*if (!erasing)*/
					owner->selectChordNote(owner->getCurrentTrigger(),midiNoteNumber,false,string+1);
			}
			else/* if (!erasing)*/ {
				owner->selectChordNote(owner->getCurrentTrigger(),midiNoteNumber,true,string+1);
				if (oldNoteOnString>=0)
					owner->selectChordNote(owner->getCurrentTrigger(),oldNoteOnString,false,string+1);
			}
		}
		lastDraggedNote = n;
	}

	bool mouseDownOnKey(int fret, int string, const MouseEvent &e) override {
		FrettedNote n(fret,string);
		int midiNoteNumber = getNote(n);
		int oldNoteOnString = getNote(FrettedNote(getStringFret(string),string));
		if (s->isNoteOn(string+1,midiNoteNumber)) {
			owner->selectChordNote(owner->getCurrentTrigger(),midiNoteNumber,false,string+1);
			erasing = true;
		}
		else {
			owner->selectChordNote(owner->getCurrentTrigger(),midiNoteNumber,true,string+1);
			if (oldNoteOnString>=0)
				owner->selectChordNote(owner->getCurrentTrigger(),oldNoteOnString,false,string+1);
			erasing = false;
		}
		lastDraggedNote = n;
		return true;
	}
};

class ChordsKeyboardComponent : public juce::MidiKeyboardComponent
{
public:
	ChordsKeyboardComponent(MidiKeyboardState &state, MidiChords* ownerFilter)
		: juce::MidiKeyboardComponent(state,MidiKeyboardComponent::horizontalKeyboard),
		owner(nullptr)
	{
		owner = ownerFilter;
		s = &state;
		this->setMidiChannel(1);
		this->setLowestVisibleKey(36);
	}
	void drawBlackNote(int midiNoteNumber, Graphics& g, int x, int y, int w, int h, bool isDown, bool isOver, const Colour& textColour)
	{
		Colour c (textColour);
		const bool isReallyReallyDown = owner->getCurrentKbState()->isNoteOnForChannels(0xffff, midiNoteNumber);

		if (isReallyReallyDown)
			c = c.overlaidWith (findColour (keyDownOverlayColourId));
		else if (s->isNoteOnForChannels(0xffff,midiNoteNumber))
			c = c.overlaidWith(Colours::brown.withAlpha(0.4f));

		if (isOver)
			c = c.overlaidWith (findColour (mouseOverKeyOverlayColourId));

		g.setColour (c);
		g.fillRect (x, y, w, h);

		if (isReallyReallyDown)
		{
			g.setColour (textColour);
			g.drawRect (x, y, w, h);
		}
		else
		{
			const int xIndent = jmax (1, jmin (w, h) / 8);

			g.setColour (c.brighter());
			g.fillRect (x + xIndent, y, w - xIndent * 2, 7 * h / 8);
		}
		Font f (jmin (12.0f, w * 0.9f));
		f.setHorizontalScale (0.8f);
		g.setFont (f);
		g.setColour (Colours::white);
		g.drawFittedText (String(midiNoteNumber), x + 2, y + 2, w - 4, h - 4, Justification::centredBottom, 1);
	}

	void drawWhiteNote(int midiNoteNumber, Graphics& g, int x, int y, int w, int h, bool isDown, bool isOver, const Colour& lineColour, const Colour& textColour)
	{
		//const int chordChan = roundToInt(owner->getParameter(kLearnChannel)*16.f);
		Colour c (Colours::transparentWhite);
		const bool isReallyReallyDown = owner->getCurrentKbState()->isNoteOnForChannels(0xffff, midiNoteNumber);

		if (isReallyReallyDown)
			c = findColour (keyDownOverlayColourId);
		if (isOver)
			c = c.overlaidWith (findColour (mouseOverKeyOverlayColourId));

		g.setColour (c);
		g.fillRect (x, y, w, h);

		g.setColour (textColour);

		Font f (jmin (12.0f, getKeyWidth() * 0.9f));
		f.setHorizontalScale (0.8f);
		f.setBold (midiNoteNumber==60);
		g.setFont (f);
		Justification justification (Justification::centredBottom);

		g.drawFittedText (String(midiNoteNumber), x + 2, y + 2, w - 4, h - 16, Justification::centredBottom, 1);

		const String text (getWhiteNoteText (midiNoteNumber));
		if (! text.isEmpty())
			g.drawFittedText (text, x + 2, y + 2, w - 4, h - 4, justification, 1);
		g.setColour (lineColour);
		g.fillRect (x, y, 1, h);

		if (midiNoteNumber == getRangeEnd())
		{
			g.fillRect (x + w, y, 1, h);
		}



		//MidiKeyboardComponent::drawWhiteNote(midiNoteNumber,g,x,y,w,h,isDown,isOver,lineColour,textColour);
		//this->getMidiChannelsToDisplay()
		//Font f (jmin (12.0f, w * 0.9f));
		//f.setHorizontalScale (0.8f);
		//f.setBold (midiNoteNumber==60);
		//g.setFont (f);
		//g.setColour (textColour);
		//g.drawFittedText (String(midiNoteNumber), x + 2, y + 2, w - 4, h - 16, Justification::centredBottom, 1);
	}
	/*
	void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
	{
		owner->selectChordNote(owner->getCurrentTrigger(),midiNoteNumber,true,midiChannel);
	}

	void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber)
	{
		owner->selectChordNote(owner->getCurrentTrigger(),midiNoteNumber,false,midiChannel);
	}
	 */

protected:
	MidiChords* owner;
    MidiKeyboardState* s;

	bool mouseDownOnKey(int midiNoteNumber, const MouseEvent &e) override
	{
		if (e.mods.isPopupMenu())
		{
			PopupMenu m;
			m.addSectionHeader(getNoteName(midiNoteNumber,owner->bottomOctave)+ " (" + String(midiNoteNumber)+")");
			for (int i=1;i<=16;i++)
			{
				m.addItem(i,"Ch "+String(i),true,s->isNoteOn(i,midiNoteNumber));
			}
			int result = m.show();
			if (result!=0)
			{
				if (s->isNoteOn(result,midiNoteNumber)) {
					//s->noteOff(result,midiNoteNumber);
					owner->selectChordNote(owner->getCurrentTrigger(),midiNoteNumber,false,result);
				}
				else {
					//s->noteOn(result,midiNoteNumber,127);
					owner->selectChordNote(owner->getCurrentTrigger(),midiNoteNumber,true,result);
				}
				repaint();
			}
		}
		else
		{
			const int chordChan = roundToInt(owner->getParameter(kLearnChannel)*16.f);
			if (chordChan==0)
			{
				if (s->isNoteOnForChannels(0xffff,midiNoteNumber))
				{
					for (int c=1;c<=16;c++)
					{
						if (s->isNoteOn(c,midiNoteNumber))
							//s->noteOff(c, midiNoteNumber);
							owner->selectChordNote(owner->getCurrentTrigger(),midiNoteNumber,false,c);
					}
				}
				else {
					//s->noteOn(1,midiNoteNumber,1.f);
					owner->selectChordNote(owner->getCurrentTrigger(),midiNoteNumber,true,1);
				}
			}
			else {
				if (s->isNoteOn(chordChan,midiNoteNumber)) {
					//s->noteOff(chordChan,midiNoteNumber);
					owner->selectChordNote(owner->getCurrentTrigger(),midiNoteNumber,false,chordChan);
				}
				else {
					//s->noteOn(chordChan,midiNoteNumber,1.f);
					owner->selectChordNote(owner->getCurrentTrigger(),midiNoteNumber,true,chordChan);
				}
			}
		}
		owner->setSavedGuitarVoicing(false);
		return false;
	}

    void mouseUpOnKey (int midiNoteNumber, const MouseEvent& e) override {
		// In any case, a mouse up event leads to the key being switched off.
		// But if that key was just toggled to "on", we have to send a note on message again.
		if (owner->getCurrentKbState()->isNoteOn(1, midiNoteNumber)) {
			owner->chordKbState.noteOn(1, midiNoteNumber, 1.0f);
		}
	}
};

class TriggerKeySelectorKeyboard : public MidiKeyboardComponent
{
public:
	TriggerKeySelectorKeyboard(MidiKeyboardState &state, MidiChords* ownerFilter)
		: MidiKeyboardComponent(state,MidiKeyboardComponent::horizontalKeyboard), owner(0)
	{
		s = &state;
		owner = ownerFilter;
		this->setMidiChannel(1);
		this->setLowestVisibleKey(36);
	}
	void drawBlackNote(int midiNoteNumber, Graphics& g, int x, int y, int w, int h, bool isDown, bool isOver, const Colour& textColour)
	{
		const int mode = roundToInt(owner->getParameter(kMode)*(numModes-1));
		Colour c (textColour);

		if (isDown || owner->getCurrentTrigger() == midiNoteNumber)
			c = c.overlaidWith (findColour (keyDownOverlayColourId));

		if (isOver)
			c = c.overlaidWith (findColour (mouseOverKeyOverlayColourId));

		if (owner->isNoteBypassed(midiNoteNumber))
			c = c.overlaidWith (Colours::orange.withAlpha(0.6f));

		if (mode==Octave) {
			int n=midiNoteNumber%12;
			while (n<128) {
				if (owner->isTriggerNotePlaying(n))
					c = c.overlaidWith (Colours::green.withAlpha(0.6f));
				n += 12;
			}
		}
		else {
			if (owner->isTriggerNotePlaying(midiNoteNumber))
				c = c.overlaidWith (Colours::green.withAlpha(0.6f));
		}

		g.setColour (c);
		g.fillRect (x, y, w, h);

		if (isDown)
		{
			g.setColour (textColour);
			g.drawRect (x, y, w, h);
		}
		else
		{
			const int xIndent = jmax (1, jmin (w, h) / 8);

			g.setColour (c.brighter());
			g.fillRect (x + xIndent, y, w - xIndent * 2, 7 * h / 8);
		}
		if (roundToInt(owner->getParameter(kMode)*(numModes-1))!=Octave) {
			Font f (jmin (12.0f, w * 0.9f));
			f.setHorizontalScale (0.8f);
			g.setFont (f);
			g.setColour (Colours::white);
			g.drawFittedText (String(midiNoteNumber), x + 2, y + 2, w - 4, h - 4, Justification::centredBottom, 1);
		}
	}
	void drawWhiteNote(int midiNoteNumber, Graphics& g, int x, int y, int w, int h, bool isDown, bool isOver, const Colour& lineColour, const Colour& textColour)
	{
		const int mode = roundToInt(owner->getParameter(kMode)*(numModes-1));
		Colour c (Colours::transparentWhite);

		if (isDown || owner->getCurrentTrigger() == midiNoteNumber)
			c = findColour (keyDownOverlayColourId);
		if (isOver)
			c = c.overlaidWith (findColour (mouseOverKeyOverlayColourId));
		if (owner->isNoteBypassed(midiNoteNumber))
			c = c.overlaidWith (Colours::orange.withAlpha(0.6f));
		if (mode==Octave) {
			int n=midiNoteNumber%12;
			while (n<128) {
				if (owner->isTriggerNotePlaying(n))
					c = c.overlaidWith (Colours::green.withAlpha(0.6f));
				n += 12;
			}
		}
		else {
			if (owner->isTriggerNotePlaying(midiNoteNumber))
				c = c.overlaidWith (Colours::green.withAlpha(0.6f));
		}

		g.setColour (c);
		g.fillRect (x, y, w, h);

		if (mode!=Octave) {

			g.setColour (textColour);

			Font f (jmin (12.0f, getKeyWidth() * 0.9f));
			f.setHorizontalScale (0.8f);
			f.setBold (midiNoteNumber==60);
			g.setFont (f);
			Justification justification (Justification::centredBottom);

			g.drawFittedText (String(midiNoteNumber), x + 2, y + 2, w - 4, h - 16, Justification::centredBottom, 1);

			const String text (getWhiteNoteText (midiNoteNumber));
			if (! text.isEmpty())
				g.drawFittedText (text, x + 2, y + 2, w - 4, h - 4, justification, 1);
		}

		if (mode==Global)
			g.setColour (Colours::red);
		else
			g.setColour (lineColour);

		g.fillRect (x, y, 1, h);

		if (midiNoteNumber == getRangeEnd())
		{
			g.fillRect (x + w, y, 1, h);
		}
	}

private:
    MidiKeyboardState* s;
	MidiChords* owner;
	bool bypassing;

	bool mouseDownOnKey(int midiNoteNumber, const MouseEvent &e) override
	{
		if (e.mods.isShiftDown() || e.mods.isPopupMenu())
		{
			bypassing = !owner->isNoteBypassed(midiNoteNumber);
			owner->setNoteBypassed(midiNoteNumber,bypassing);
			repaint();
		}
		else {
			if (roundToInt(owner->getParameter(kMode)*(numModes-1))==Global)
				owner->setParameterNotifyingHost(kRoot,midiNoteNumber/127.f);
			else
				owner->selectTrigger(midiNoteNumber);
		}
		return false;
	}

    void mouseUpOnKey (int midiNoteNumber, const MouseEvent& e) override {
		// In any case, a mouse up event leads to the key being switched off.
		// But if that key was just toggled to "on", we have to send a note on message again.
		if (midiNoteNumber == owner->getCurrentTrigger()) {
			owner->triggerKbState.noteOn(1, midiNoteNumber, 1.0f);
		}
	}

	bool mouseDraggedToKey (int midiNoteNumber, const MouseEvent& e) override
	{
		if (e.mods.isShiftDown() || e.mods.isPopupMenu())
		{
			owner->setNoteBypassed(midiNoteNumber,bypassing);
			repaint();
		}
		else {
			if (roundToInt(owner->getParameter(kMode)*(numModes-1))==Global)
				owner->setParameterNotifyingHost(kRoot,midiNoteNumber/127.f);
			else
				owner->selectTrigger(midiNoteNumber);
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
class MidiChordsEditor  : public juce::AudioProcessorEditor,
                          public juce::ChangeListener,
                          public juce::TextEditor::Listener,
                          public juce::FileDragAndDropTarget,
                          public juce::FileBrowserListener,
                          public juce::Button::Listener,
                          public juce::Slider::Listener,
                          public juce::Label::Listener
{
public:
    //==============================================================================
    MidiChordsEditor (MidiChords* const ownerFilter);
    ~MidiChordsEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	friend class ChordsKeyboardComponent;
	friend class TriggerKeySelectorKeyboard;
	static void chordMenuCallback (int result, MidiChordsEditor* editor);
    void textEditorTextChanged(TextEditor&) override;
    void textEditorReturnKeyPressed(TextEditor&) override;
    void textEditorEscapeKeyPressed(TextEditor&) override;
    void textEditorFocusLost(TextEditor&) override;
	void changeListenerCallback (ChangeBroadcaster* source) override;
    bool isInterestedInFileDrag (const StringArray& files) override;
    void filesDropped (const StringArray& filenames, int mouseX, int mouseY) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDoubleClick (const MouseEvent& e) override;
	void mouseUp (const MouseEvent& e) override;
	void selectionChanged() override {};
	void fileClicked(const juce::File &file,const juce::MouseEvent &) override;
	void fileDoubleClicked (const File &file) override;
	void browserRootChanged (const File& newRoot) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void labelTextChanged (juce::Label* labelThatHasChanged) override;

    // Binary resources:
    static const char* midichordsLogo_png;
    static const int midichordsLogo_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    TooltipWindow tooltipWindow;
    void updateParametersFromFilter();
	String const getCurrentChordName();
	void listChordFiles(StringArray &list);
	void listPresetFiles(Array<File> &list);
	void loadChord(String chorddef);
	void saveChord(String name);
	void loadPreset(File file);
	void chordFromString(String chordString);

	bool isGuitarPreset(int index)
	{
		if (getFilter()->getNumStrings()!=getFilter()->guitarPresets[index].numStrings)
			return false;
		for (int i=0;i<getFilter()->getNumStrings();i++)
		{
			if (getFilter()->getStringValue(i)!=getFilter()->guitarPresets[index].stringNotes[i])
				return false;
		}
		return true;
	}

	void setUpGuitar(int index)
	{
		const int f = getFilter()->guitarPresets[index].numFrets;
		const int s = getFilter()->guitarPresets[index].numStrings;
		getFilter()->setNumFrets(f, false);
		guitar->setNumFrets(f);
		getFilter()->setNumStrings(s, false);
		guitar->setNumStrings(s);
		for (int i=0;i<maxStrings;i++)
		{
			if (i<s) {
				getFilter()->setStringValue(i, getFilter()->guitarPresets[index].stringNotes[i], false);
				guitar->setStringNote(i, getFilter()->guitarPresets[index].stringNotes[i]);
			}
			else {
				getFilter()->setStringValue(i, -1, false);
				guitar->setStringNote(i, -1);
			}
		}

		getFilter()->translateToGuitarChord();
		guitar->repaint();
	}

    MidiChords* getFilter() const throw()       { return (MidiChords*) getAudioProcessor(); }
	int mode;
	ChordPresetFileFilter fileFilter;
	FileBrowserComponent* browser;

    FretsSlider* fretsSlider;
    StringsSlider* stringsSlider;
    NoteSlider* stringSlider[maxStrings];
	int guitarPreset;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ToggleButton> toggleButton;
    std::unique_ptr<juce::Label> chordNameLabel;
    std::unique_ptr<ChordsKeyboardComponent> chordKeyboard;
    std::unique_ptr<TriggerKeySelectorKeyboard> triggerKeyboard;
    std::unique_ptr<juce::TextButton> chordLearnButton;
    std::unique_ptr<juce::TextButton> triggerLearnButton;
    std::unique_ptr<ChannelSlider> channelSlider;
    std::unique_ptr<juce::Label> outputLabel;
    std::unique_ptr<PizButton> pizButton;
    std::unique_ptr<juce::Label> triggerLabel;
    std::unique_ptr<juce::TextButton> clearChordButton;
    std::unique_ptr<juce::TextButton> resetChordButton;
    std::unique_ptr<juce::TextButton> clearAllButton;
    std::unique_ptr<juce::TextButton> resetAllButton;
    std::unique_ptr<juce::TextButton> transposeUpButton;
    std::unique_ptr<juce::TextButton> transposeDownButton;
    std::unique_ptr<juce::TextButton> transposeChordUpButton;
    std::unique_ptr<juce::TextButton> transposeChordDownButton;
    std::unique_ptr<juce::Slider> transposeSlider;
    std::unique_ptr<juce::Slider> velocitySlider;
    std::unique_ptr<juce::Slider> variationSlider;
    std::unique_ptr<juce::TextButton> normalButton;
    std::unique_ptr<juce::TextButton> octaveButton;
    std::unique_ptr<juce::TextButton> globalButton;
    std::unique_ptr<juce::ToggleButton> flatsButton;
    std::unique_ptr<juce::TextButton> transposeChordUpButton2;
    std::unique_ptr<juce::TextButton> transposeChordDownButton2;
    std::unique_ptr<juce::TextButton> chordMenuButton;
    std::unique_ptr<juce::Label> presetNameLabel;
    std::unique_ptr<juce::TextButton> presetMenuButton;
    std::unique_ptr<juce::TextEditor> chordSaveEditor;
    std::unique_ptr<juce::TextButton> copyButton;
    std::unique_ptr<juce::TextButton> pasteButton;
    std::unique_ptr<juce::TextButton> previewButton;
    std::unique_ptr<juce::Label> chordEditor;
    std::unique_ptr<juce::ToggleButton> pcButton;
    std::unique_ptr<juce::TextButton> nextButton;
    std::unique_ptr<juce::TextButton> prevButton;
    std::unique_ptr<juce::Label> triggerNoteLabel;
    std::unique_ptr<ChannelSlider> learnChanSlider;
    std::unique_ptr<juce::Label> demoLabel;
    std::unique_ptr<ChordsGuitar> guitar;
    std::unique_ptr<juce::Label> versionLabel;
    std::unique_ptr<juce::ToggleButton> transposeInputButton;
    std::unique_ptr<juce::ToggleButton> toAllChannelsButton;
    std::unique_ptr<juce::TextButton> infoButton;
    std::unique_ptr<juce::TextButton> specialMenuButton;
    std::unique_ptr<ChannelSlider> outputChannelSlider;
    std::unique_ptr<juce::TextButton> applyChannelButton;
    std::unique_ptr<juce::Label> label;
    std::unique_ptr<juce::Label> label2;
    std::unique_ptr<juce::TextButton> viewButton;
    std::unique_ptr<juce::TextButton> setupButton;
    std::unique_ptr<juce::TextButton> strumDirectionButton;
    std::unique_ptr<juce::TextButton> strumButton;
    std::unique_ptr<VSTSlider> maxTimeSlider;
    std::unique_ptr<VSTSlider> speedSlider;
    std::unique_ptr<VSTSlider> accelSlider;
    std::unique_ptr<VSTSlider> velRampSlider;
    std::unique_ptr<juce::TextEditor> infoBox;
    std::unique_ptr<juce::TextEditor> tuningSaveEditor;
    juce::Image cachedImage_midichordsLogo_png_1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiChordsEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

