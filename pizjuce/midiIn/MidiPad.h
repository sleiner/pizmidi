#ifndef imagePluginFilter_PAD_H
#define imagePluginFilter_PAD_H

#include <juce_gui_basics/juce_gui_basics.h>

#include "../_common/DrawablePad.h"

class MidiPad : public juce::Component,
                public juce::Button::Listener,
                public juce::FileDragAndDropTarget
{
public:
    //==============================================================================
    MidiPad();
    ~MidiPad() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button*) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void setButtonText(const juce::String&);
    void setTooltip(juce::String text);
    void setColour(const juce::Colour&);
    void setTriggeredOnMouseDown(const bool);
    void addButtonListener(juce::Button::Listener* const);
    void clearIcon();

    DrawablePad* drawableButton;

private:
    juce::Label* label;
    MidiPad(const MidiPad&);
    const MidiPad& operator=(const MidiPad&);

    JUCE_LEAK_DETECTOR(MidiPad)
};

#endif
