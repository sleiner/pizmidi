#pragma once

#include "juce_core/juce_core.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

class D3CKLook : public juce::LookAndFeel_V4
{
public:
    D3CKLook() : juce::LookAndFeel_V4() {}

    int getDefaultScrollbarWidth() override;

    //    int  thumbSize,      bool  isMouseOver,      bool  isMouseDown    )
    void drawScrollbar (Graphics& g, ScrollBar& scrollbar, int x, int y, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown) override;

    void drawScrollbarButton (Graphics& g, ScrollBar& scrollbar, int width, int height, int buttonDirection, bool isScrollbarVertical, bool isMouseOver, bool isButtonDown) override;

    //  (   &     )
    int getScrollbarButtonSize (ScrollBar& scrollbar) override;

    //getMinimumScrollbarThumbSize  (   &     )
    int getMinimumScrollbarThumbSize (ScrollBar& scrollbar) override;

    //  (   &  g,      int  width,      int  height,      TextEditor &  textEditor    )
    void drawTextEditorOutline (Graphics& g, int width, int height, juce::TextEditor& textEditor) override;

    void drawPopupMenuItem (Graphics& g, int width, int height, bool isSeperator, bool isActive, bool isHightlited, bool isTicked, bool hasSubmenu, const String& text, const String& shortcutText, Image* image, const juce::Colour* const textColour);

    void drawPopupMenuBackground (Graphics& g, int width, int height) override;

    DropShadower* createDropShadowerForComponent (juce::Component* comp) override;
};
