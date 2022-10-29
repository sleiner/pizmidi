#ifndef MIDIPADSPLUGINEDITOR_H
#define MIDIPADSPLUGINEDITOR_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "MidiPad.h"
#include "midiPads.h"

class fullScreenContainer : public juce::Component
{
public:
    fullScreenContainer()
    {
        bgcolor = juce::Colours::white;
    }

    ~fullScreenContainer() override{};
    juce::Colour bgcolor;

private:
    void paint(juce::Graphics& g) override
    {
        g.fillAll(bgcolor);
    }
};

class midiPadsEditor : public juce::AudioProcessorEditor,
                       public juce::ChangeListener,
                       public juce::Button::Listener,
                       public juce::Slider::Listener,
                       public juce::TextEditor::Listener,
                       public juce::FileDragAndDropTarget
{
public:
    midiPadsEditor(midiPads* const ownerFilter);
    ~midiPadsEditor() override;

    //==============================================================================
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void buttonClicked(juce::Button*) override;
    void buttonStateChanged(juce::Button*) override;
    void sliderValueChanged(juce::Slider*) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseDoubleClick(const juce::MouseEvent& e) override;
    void textEditorTextChanged(juce::TextEditor&) override;
    void textEditorReturnKeyPressed(juce::TextEditor&) override;
    void textEditorEscapeKeyPressed(juce::TextEditor&) override;
    void textEditorFocusLost(juce::TextEditor&) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void paint(juce::Graphics&) override;
    void resized() override;

    juce::Colour color, color2;

private:
    //==============================================================================
    MidiPad* midiPad[numPads];

    juce::ColourSelector* colourSelector1;
    juce::Slider* vSlider;
    juce::Slider* nSlider;
    juce::Slider* ySlider;
    juce::Slider* oSlider;
    juce::Slider* triggerSlider;
    juce::Slider* xSlider;
    juce::Slider* xoSlider;
    juce::Slider* roundnessSlider;
    juce::Slider* iconSizeSlider;
    juce::Slider* cSlider;
    juce::Slider* icSlider;
    juce::Slider* hueSlider;
    juce::Slider* saturationSlider;
    juce::Slider* brigtnessSlider;
    juce::Slider* padOpacitySlider;
    juce::Slider* globalRoundnessSlider;
    juce::Slider* globalIconSizeSlider;
    juce::Slider* velocitySlider;
    juce::Slider* valueSlider;
    juce::TextEditor* textEditor;
    juce::TextButton* menuButton;
    fullScreenContainer* container;
    Component* padEditor;
    juce::ResizableCornerComponent* resizer;
    juce::ComponentBoundsConstrainer resizeLimits;
    juce::TooltipWindow tooltipWindow;

    juce::PopupMenu m, sub1, sub2, sub3, icons, layout, presets, fileMenu;

    void updateParametersFromFilter();
    void padTriggered();
    void dotMoved(int pad);
    int lastx[numPads];
    int lasty[numPads];
    int squares;
    bool showdots[numPads];
    bool showvalues[numPads];
    void sendMidi(int i, bool shiftclicked = false);
    void sendMidiOff(int i);
    bool sending[numPads];
    bool dontsend;
    bool fullscreen;
    bool ignore;
    bool automateHost;
    bool dragging;
    bool resizing;
    juce::Point<int> dragstart;
    int lastTouchedPad;
    int lastPadMenu;

    midiPads* getFilter() const throw()
    {
        return (midiPads*) getAudioProcessor();
    }
};

#endif
