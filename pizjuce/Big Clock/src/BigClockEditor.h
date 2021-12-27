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
    ~TimeDisplay();

    //==============================================================================
    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown);
    void resized();
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
    ~BigClockEditor();

	void changeListenerCallback (ChangeBroadcaster* source);
    void buttonClicked (Button* buttonThatWasClicked);
    void buttonStateChanged (Button* buttonThatWasClicked);
    void textEditorReturnKeyPressed(TextEditor& editor);
    void textEditorEscapeKeyPressed(TextEditor& editor){};
    void textEditorTextChanged(TextEditor& editor){};
    void textEditorFocusLost(TextEditor& editor){};
	void timerCallback();
	void mouseEnter(const MouseEvent& e);
	void mouseExit(const MouseEvent& e);

    //==============================================================================
    void paint (Graphics& g);
    void resized();


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
