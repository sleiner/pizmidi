#ifndef PIZMIDI_PIANOROLLBACKGROUND_H
#define PIZMIDI_PIANOROLLBACKGROUND_H

#include "juce_gui_basics/juce_gui_basics.h"

class PianoRoll;

class PianoRollBackground : public juce::Component
{
public:
    PianoRollBackground();

    ~PianoRollBackground() override;

    void paint(juce::Graphics& g) override;
};

#endif //PIZMIDI_PIANOROLLBACKGROUND_H
