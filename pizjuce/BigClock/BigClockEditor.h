#ifndef BIGCLOCKPLUGINEDITOR_H
#define BIGCLOCKPLUGINEDITOR_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_gui_extra/juce_gui_extra.h"

#include "BigClock.h"

class TimeDisplay  : public Button
{
public:
    //==============================================================================
    TimeDisplay ();
    ~TimeDisplay() override;

    //==============================================================================
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
    void resized() override;
    String time;
    Colour textcolor;


    //==============================================================================
    juce_UseDebuggingNewOperator

private:


    TimeDisplay (const TimeDisplay&);
    const TimeDisplay& operator= (const TimeDisplay&);
};


//==============================================================================
class BigClockEditor   : public AudioProcessorEditor,
                              public Button::Listener,
                              public TextEditor::Listener,
                              public ChangeListener,
							  public Timer
{
public:
    BigClockEditor (BigClockFilter* const ownerFilter);
    ~BigClockEditor() override;

	void changeListenerCallback (ChangeBroadcaster* source) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void buttonStateChanged (Button* buttonThatWasClicked) override;
    void textEditorReturnKeyPressed(TextEditor& editor) override;
    void textEditorEscapeKeyPressed(TextEditor& editor) override{};
    void textEditorTextChanged(TextEditor& editor) override{};
    void textEditorFocusLost(TextEditor& editor) override{};
	void timerCallback() override;
	void mouseEnter(const MouseEvent& e) override;
	void mouseExit(const MouseEvent& e) override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;


private:
    //==============================================================================
    TimeDisplay* infoLabel;
    ColourSelector* colourSelector;
    TooltipWindow tooltipWindow;
    ResizableCornerComponent* resizer;
    ComponentBoundsConstrainer resizeLimits;
    TextEditor* textBox;
    Label* cueLabel;
	Label* modeLabel;
	TextButton* runButton;
	TextButton* resetButton;

    bool showtextbox;
	bool barsbeats;
	bool recording;
	bool hosttime;
	uint32 watchTime;

    void updateParametersFromFilter();

    BigClockFilter* getFilter() const throw()       { return (BigClockFilter*) getAudioProcessor(); }
};


#endif
