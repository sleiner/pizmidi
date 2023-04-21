#include "KeySelector.h"

KeySelector::KeySelector(juce::MidiKeyboardState& state)
    : MidiKeyboardComponent(state, MidiKeyboardComponent::horizontalKeyboard)
{
    keyboardState = &state;
    this->setColour(MidiKeyboardComponent::textLabelColourId, juce::Colours::transparentBlack);
}

KeySelector::~KeySelector() = default;

bool KeySelector::mouseDownOnKey(int midiNoteNumber, const juce::MouseEvent& e)
{
    if (keyboardState->isNoteOn(this->getMidiChannel(), midiNoteNumber))
    {
        keyboardState->noteOff(this->getMidiChannel(), midiNoteNumber, 1.f);
    }
    else
    {
        keyboardState->noteOn(this->getMidiChannel(), midiNoteNumber, 1.f);
    }
    return false;
}