#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

class D3CKLook : public juce::LookAndFeel_V1
{
public:
    D3CKLook()
        : juce::LookAndFeel_V1()
    {
    }

    int getDefaultScrollbarWidth() override;

    void drawScrollbar(juce::Graphics& g, juce::ScrollBar& scrollbar, int x, int y, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown) override;

    void drawScrollbarButton(juce::Graphics& g, juce::ScrollBar& scrollbar, int width, int height, int buttonDirection, bool isScrollbarVertical, bool isMouseOver, bool isButtonDown) override;

    int getScrollbarButtonSize(juce::ScrollBar& scrollbar) override;

    int getMinimumScrollbarThumbSize(juce::ScrollBar& scrollbar) override;

    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override;

    void drawPopupMenuItem(juce::Graphics& g, int width, int height, bool isSeperator, bool isActive, bool isHightlited, bool isTicked, bool hasSubmenu, const juce::String& text, const juce::String& shortcutText, juce::Image* image, const juce::Colour* const textColour);

    void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override;

    std::unique_ptr<juce::DropShadower> createDropShadowerForComponent(juce::Component& comp) override;
};
