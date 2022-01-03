#ifndef MIDIPADSPLUGINEDITOR_H
#define MIDIPADSPLUGINEDITOR_H

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_gui_extra/juce_gui_extra.h"

#include "midiPads.h"
#include "MidiPad.h"

class fullScreenContainer : public Component
{
public:
	fullScreenContainer() {bgcolor=Colours::white;}
	~fullScreenContainer() {};
	Colour bgcolor;
private:
	void paint(Graphics& g) {g.fillAll(bgcolor);}
};

class midiPadsEditor   : public AudioProcessorEditor,
                         public ChangeListener,
                         public Button::Listener,
                         public Slider::Listener,
                         public TextEditor::Listener,
						 public FileDragAndDropTarget
{
public:
    midiPadsEditor (midiPads* const ownerFilter);
    ~midiPadsEditor();

    //==============================================================================
    void changeListenerCallback (ChangeBroadcaster* source);
    void buttonClicked (Button*);
    void buttonStateChanged (Button*);
    void sliderValueChanged (Slider*);
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);
    void mouseUp (const MouseEvent& e);
	void mouseDoubleClick(const MouseEvent& e);
    void textEditorTextChanged(TextEditor&);
    void textEditorReturnKeyPressed(TextEditor&);
    void textEditorEscapeKeyPressed(TextEditor&);
    void textEditorFocusLost(TextEditor&);
    void filesDropped (const StringArray &files, int x, int y);
    bool isInterestedInFileDrag (const StringArray& files);
    void paint (Graphics&);
    void resized();

    Colour color, color2;

private:
    //==============================================================================
    MidiPad* midiPad[numPads];

    ColourSelector* colourSelector1;
    Slider* vSlider;
    Slider* nSlider;
    Slider* ySlider;
    Slider* oSlider;
    Slider* triggerSlider;
    Slider* xSlider;
    Slider* xoSlider;
	Slider* roundnessSlider;
	Slider* iconSizeSlider;
    Slider* cSlider;
    Slider* icSlider;
    Slider* hueSlider;
    Slider* saturationSlider;
    Slider* brigtnessSlider;
    Slider* padOpacitySlider;
    Slider* globalRoundnessSlider;
	Slider* globalIconSizeSlider;
    Slider* velocitySlider;
    Slider* valueSlider;
    TextEditor* textEditor;
    TextButton* menuButton;
	fullScreenContainer* container;
	Component* padEditor;
    ResizableCornerComponent* resizer;
    ComponentBoundsConstrainer resizeLimits;
    TooltipWindow tooltipWindow;

	PopupMenu m, sub1, sub2, sub3, icons, layout, presets, fileMenu;

    void updateParametersFromFilter();
    void padTriggered();
    void dotMoved(int pad);
    int lastx[numPads];
    int lasty[numPads];
    int squares;
    bool showdots[numPads];
	bool showvalues[numPads];
    void sendMidi(int i, bool shiftclicked=false);
    void sendMidiOff(int i);
	bool sending[numPads];
    bool dontsend;
	bool fullscreen;
    bool ignore;
	bool automateHost;
	bool dragging;
	bool resizing;
	Point<int> dragstart;
	int lastTouchedPad;
	int lastPadMenu;

    midiPads* getFilter() const throw() { return (midiPads*) getAudioProcessor(); }
};


#endif
