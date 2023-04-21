#ifndef PIZMIDI_PIANOROLLNOTES_H
#define PIZMIDI_PIANOROLLNOTES_H

#include "juce_gui_basics/juce_gui_basics.h"

class PianoRoll;

class PianoRollNotes : public juce::Component
{
public:
    PianoRollNotes();

    ~PianoRollNotes() override;

    void paint(juce::Graphics& g) override;
};

#endif //PIZMIDI_PIANOROLLNOTES_H
