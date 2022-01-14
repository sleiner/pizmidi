#ifndef IMAGEPLUGINEDITOR_H
#define IMAGEPLUGINEDITOR_H

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_gui_extra/juce_gui_extra.h"

#include "../_common/ChannelSlider.h"
#include "MidiPad.h"
#include "imagePlugin.h"

class imagePluginEditor : public AudioProcessorEditor,
                          public Button::Listener,
                          public Slider::Listener,
                          public TextEditor::Listener,
                          public ChangeListener,
                          public KeyListener,
                          public FileDragAndDropTarget

{
public:
    /** Constructor.

        When created, this will register itself with the filter for changes. It's
        safe to assume that the filter won't be deleted before this object is.
    */
    imagePluginEditor (imagePluginFilter* const ownerFilter);

    /** Destructor. */
    ~imagePluginEditor() override;

    //==============================================================================
    /** Our demo filter is a ChangeBroadcaster, and will call us back when one of
        its parameters changes.
    */
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void buttonClicked (Button*) override;
    void buttonStateChanged (Button* buttonThatWasClicked) override;
    bool keyPressed (const KeyPress& key, Component* originatingComponent) override;
    void textEditorTextChanged (TextEditor& editor) override;
    void textEditorReturnKeyPressed (TextEditor& editor) override;
    void textEditorEscapeKeyPressed (TextEditor& editor) override;
    void textEditorFocusLost (TextEditor& editor) override;
    void filesDropped (const StringArray& files, int x, int y) override;
    bool isInterestedInFileDrag (const StringArray& files) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseDoubleClick (const MouseEvent& e) override;

    //==============================================================================
    /** Standard Juce paint callback. */
    void paint (Graphics&) override;

    /** Standard Juce resize callback. */
    void resized() override;
    Colour color, color2;

private:
    //==============================================================================
    MidiPad* imagepad;
    TextEditor* textEditor;
    ColourSelector* colourSelector;
    ColourSelector* textColourSelector;
    Slider* bankSlider;
    Slider* progSlider;
    ChannelSlider* chanSlider;
    ResizableCornerComponent* resizer;
    ComponentBoundsConstrainer resizeLimits;
    void updateParametersFromFilter();
    Component* container;
    Label* label;
    bool fullscreen;

    // handy wrapper method to avoid having to cast the filter to a imagePluginFilter
    // every time we need it..
    imagePluginFilter* getFilter() const throw() { return (imagePluginFilter*) getAudioProcessor(); }
};

#endif
