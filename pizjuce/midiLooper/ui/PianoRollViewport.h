#ifndef PIZMIDI_PIANOROLLVIEWPORT_H
#define PIZMIDI_PIANOROLLVIEWPORT_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "Timeline.h"

class PianoRollViewport : public juce::Viewport, public juce::ChangeBroadcaster
{
public:
    explicit PianoRollViewport(const juce::String& name);
    ~PianoRollViewport() override;

    void setTimeline(Timeline* t);
    void setPlayBar(Component* p);
    void setKeyboard(Viewport* kb);

    std::function<void(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel)> onMouseWheelMove;

    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override;

    void visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea) override;

private:
    Timeline* timeline;
    juce::Component* playBar;
    juce::Viewport* keyboard;
};

#endif //PIZMIDI_PIANOROLLVIEWPORT_H
