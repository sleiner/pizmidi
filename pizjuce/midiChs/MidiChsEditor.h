#ifndef MIDICHSPLUGINEDITOR_H
#define MIDICHSPLUGINEDITOR_H

#include "juce_gui_basics/juce_gui_basics.h"

#include "MidiChsProcessor.h"

//==============================================================================
/**
    This is the Component that our filter will use as its UI.

    One or more of these is created by the MidiChsProcessor::createEditor() method,
    and they will be deleted at some later time by the wrapper code.

    To demonstrate the correct way of connecting a filter to its UI, this
    class is a ChangeListener, and our demo filter is a ChangeBroadcaster. The
    editor component registers with the filter when it's created and deregisters
    when it's destroyed. When the filter's parameters are changed, it broadcasts
    a message and this editor responds by updating its display.
*/
class MidiChsEditor : public AudioProcessorEditor,
                      public ChangeListener,
                      public Slider::Listener,
                      public Button::Listener
{
public:
    /** Constructor.

        When created, this will register itself with the filter for changes. It's
        safe to assume that the filter won't be deleted before this object is.
    */
    MidiChsEditor (MidiChsProcessor* const ownerFilter);

    /** Destructor. */
    ~MidiChsEditor() override;

    //==============================================================================
    /** Our demo filter is a ChangeBroadcaster, and will call us back when one of
        its parameters changes.
    */
    void changeListenerCallback (ChangeBroadcaster* source) override;

    //==============================================================================
    /** Standard Juce paint callback. */
    void paint (Graphics& g) override;

    /** Standard Juce resize callback. */
    void resized() override;

    void buttonClicked (Button*) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;

private:
    //==============================================================================
    //Label* infoLabel[16];
    Colour bgcolor, fgcolor;
    TextButton* resetButton;
    TextButton* clearButton;
    Slider* slider[16];
    Label* label[16];
    TooltipWindow tooltipWindow;

    void updateParametersFromFilter();

    // handy wrapper method to avoid having to cast the filter to a MidiChsProcessor
    // every time we need it..
    MidiChsProcessor* getFilter() const throw() { return (MidiChsProcessor*) getAudioProcessor(); }
};

#endif
