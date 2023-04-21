#ifndef PIZMIDI_PLAYBAR_H
#define PIZMIDI_PLAYBAR_H

#include "juce_gui_basics/juce_gui_basics.h"

class PianoRoll;

class PlayBar : public juce::Component
{
public:
    explicit PlayBar(PianoRoll* pianoRoll);

    ~PlayBar() override;

    void paint(juce::Graphics& g) override;

private:
    PianoRoll* pianoRoll;
};

#endif //PIZMIDI_PLAYBAR_H
