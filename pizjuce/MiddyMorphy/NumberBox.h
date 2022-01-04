#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

class NumberBox : public juce::TextEditor {
  public:
    // Component::keyPressed  (  const KeyPress &  key   )
    bool keyPressed(const KeyPress & key);

};
