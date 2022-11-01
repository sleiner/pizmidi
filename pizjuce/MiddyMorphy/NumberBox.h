#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class NumberBox : public juce::TextEditor
{
public:
    bool keyPressed(const juce::KeyPress& key) override;
};
