#ifndef PIZMIDI_KEYSELECTOR_H
#define PIZMIDI_KEYSELECTOR_H

#include "juce_audio_utils/juce_audio_utils.h"

class KeySelector : public juce::MidiKeyboardComponent
{
public:
    explicit KeySelector(juce::MidiKeyboardState& state);
    ~KeySelector() override;

private:
    bool mouseDownOnKey(int midiNoteNumber, const juce::MouseEvent& e) override;
    juce::MidiKeyboardState* keyboardState;
};

#endif //PIZMIDI_KEYSELECTOR_H
