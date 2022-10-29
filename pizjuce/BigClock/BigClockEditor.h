#ifndef BIGCLOCKPLUGINEDITOR_H
#define BIGCLOCKPLUGINEDITOR_H

#include "BigClock.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

class TimeDisplay : public juce::Button
{
public:
    //==============================================================================
    TimeDisplay();
    ~TimeDisplay() override;

    //==============================================================================
    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
    void resized() override;
    juce::String time;
    juce::Colour textcolor;

private:
    TimeDisplay(const TimeDisplay&);
    const TimeDisplay& operator=(const TimeDisplay&);

    JUCE_LEAK_DETECTOR(TimeDisplay)
};

//==============================================================================
class BigClockEditor : public juce::AudioProcessorEditor,
                       public juce::Button::Listener,
                       public juce::TextEditor::Listener,
                       public juce::ChangeListener,
                       public juce::Timer
{
public:
    BigClockEditor(BigClockFilter* const ownerFilter);
    ~BigClockEditor() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void buttonClicked(juce::Button* buttonThatWasClicked) override;
    void buttonStateChanged(juce::Button* buttonThatWasClicked) override;
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;
    void textEditorEscapeKeyPressed(juce::TextEditor& editor) override{};
    void textEditorTextChanged(juce::TextEditor& editor) override{};
    void textEditorFocusLost(juce::TextEditor& editor) override{};
    void timerCallback() override;
    void mouseEnter(const juce::MouseEvent& e) override;
    void mouseExit(const juce::MouseEvent& e) override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    TimeDisplay* infoLabel;
    juce::ColourSelector* colourSelector;
    juce::TooltipWindow tooltipWindow;
    juce::ResizableCornerComponent* resizer;
    juce::ComponentBoundsConstrainer resizeLimits;
    juce::TextEditor* textBox;
    juce::Label* cueLabel;
    juce::Label* modeLabel;
    juce::TextButton* runButton;
    juce::TextButton* resetButton;

    bool showtextbox;
    bool barsbeats;
    bool recording;
    bool hosttime;
    juce::uint32 watchTime;

    void updateParametersFromFilter();

    BigClockFilter* getFilter() const throw()
    {
        return (BigClockFilter*) getAudioProcessor();
    }

    JUCE_LEAK_DETECTOR(BigClockEditor)
};

#endif
