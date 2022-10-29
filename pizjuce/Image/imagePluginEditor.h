#ifndef IMAGEPLUGINEDITOR_H
#define IMAGEPLUGINEDITOR_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "../_common/ChannelSlider.h"
#include "imagePlugin.h"
#include "MidiPad.h"

class imagePluginEditor : public juce::AudioProcessorEditor,
                          public juce::Button::Listener,
                          public juce::Slider::Listener,
                          public juce::TextEditor::Listener,
                          public juce::ChangeListener,
                          public juce::KeyListener,
                          public juce::FileDragAndDropTarget

{
public:
    /** Constructor.

        When created, this will register itself with the filter for changes. It's
        safe to assume that the filter won't be deleted before this object is.
    */
    imagePluginEditor(imagePluginFilter* const ownerFilter);

    /** Destructor. */
    ~imagePluginEditor() override;

    //==============================================================================
    /** Our demo filter is a ChangeBroadcaster, and will call us back when one of
        its parameters changes.
    */
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void buttonClicked(juce::Button*) override;
    void buttonStateChanged(juce::Button* buttonThatWasClicked) override;
    bool keyPressed(const juce::KeyPress& key, Component* originatingComponent) override;
    void textEditorTextChanged(juce::TextEditor& editor) override;
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;
    void textEditorEscapeKeyPressed(juce::TextEditor& editor) override;
    void textEditorFocusLost(juce::TextEditor& editor) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void sliderValueChanged(juce::Slider* sliderThatWasMoved) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDoubleClick(const juce::MouseEvent& e) override;

    //==============================================================================
    /** Standard Juce paint callback. */
    void paint(juce::Graphics&) override;

    /** Standard Juce resize callback. */
    void resized() override;
    juce::Colour color, color2;

private:
    //==============================================================================
    MidiPad* imagepad;
    juce::TextEditor* textEditor;
    juce::ColourSelector* colourSelector;
    juce::ColourSelector* textColourSelector;
    juce::Slider* bankSlider;
    juce::Slider* progSlider;
    ChannelSlider* chanSlider;
    juce::ResizableCornerComponent* resizer;
    juce::ComponentBoundsConstrainer resizeLimits;
    void updateParametersFromFilter();
    Component* container;
    juce::Label* label;
    bool fullscreen;

    // handy wrapper method to avoid having to cast the filter to a imagePluginFilter
    // every time we need it
    imagePluginFilter* getFilter() const throw()
    {
        return (imagePluginFilter*) getAudioProcessor();
    }
};

#endif
