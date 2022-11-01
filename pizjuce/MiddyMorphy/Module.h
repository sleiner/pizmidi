#pragma once

#include "XYItem.h"

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

class Module : public XYItem
{
public:
    virtual void mouseDown(const juce::MouseEvent& e);

    virtual void mouseDrag(const juce::MouseEvent&);

    virtual void mouseUp(const juce::MouseEvent& e);

    juce::Colour colour;

    juce::String name;

    int size;
};
