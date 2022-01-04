#pragma once

#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "XYItem.h"

using namespace juce;

class Module : public XYItem {
  public:
    virtual void mouseDown(const MouseEvent & e );

    virtual void mouseDrag(const MouseEvent &);

    virtual void mouseUp(const MouseEvent & e);

    Colour colour;

    String name;

    int size;

};
