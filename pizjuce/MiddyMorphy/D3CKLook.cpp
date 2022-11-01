#include "D3CKLook.h"

using juce::jmin;

int D3CKLook::getDefaultScrollbarWidth()
{
    return 12;
}

void D3CKLook::drawScrollbar(juce::Graphics& g, juce::ScrollBar& scrollbar, int x, int y, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown)
{
    if (thumbSize > 0)
    {
        if (isScrollbarVertical)
        {
            if (isMouseOver)
            {
                g.fillRoundedRectangle((float) (x + 1), (float) (thumbStartPosition + 1), (float) (width - 2), (float) (thumbSize - 2), (float) jmin(width, height) * 0.25f);
            }
            else
            {
                g.drawRoundedRectangle((float) (x + 1), (float) (thumbStartPosition + 1), (float) (width - 2), (float) (thumbSize - 2), (float) jmin(width, height) * 0.25f, 1.f);
            }
        }
        else
        {
            if (isMouseOver)
            {
                g.fillRoundedRectangle((float) (thumbStartPosition + 1), (float) (y + 1), (float) (thumbSize - 2), (float) (height - 2), (float) jmin(width, height) * 0.25f);
            }
            else
            {
                g.drawRoundedRectangle((float) (thumbStartPosition + 1), (float) (y + 1), (float) (thumbSize - 2), (float) (height - 2), (float) jmin(width, height) * 0.25f, 1.f);
            }
        }
    }
}

void D3CKLook::drawScrollbarButton(juce::Graphics& g, juce::ScrollBar& scrollbar, int width, int height, int buttonDirection, bool isScrollbarVertical, bool isMouseOver, bool isButtonDown)
{
    if (isMouseOver)
    {
        g.fillRoundedRectangle(1, 1, (float) (width - 2), (float) (height - 2), (float) jmin(width, height) * 0.25f);
    }

    g.drawRoundedRectangle(1, 1, (float) (width - 2), (float) (height - 2), (float) jmin(width, height) * 0.25f, 1);
}

int D3CKLook::getScrollbarButtonSize(juce::ScrollBar& scrollbar)
{
    return 8;
}

int D3CKLook::getMinimumScrollbarThumbSize(juce::ScrollBar& scrollbar)
{
    return 8;
}

void D3CKLook::drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor)
{
    g.drawRoundedRectangle(1, 1, (float) (width - 2), (float) (height - 2), (float) jmin(width, height) * 0.5f, 1);
}

void D3CKLook::drawPopupMenuItem(juce::Graphics& g, int width, int height, const bool isSeperator, const bool isActive, const bool isHightlited, const bool isTicked, const bool hasSubmenu, const juce::String& text, const juce::String& shortcutText, juce::Image* image, const juce::Colour* const textColour)
{
    g.fillRect(0, 0, width, height);
    if (! isSeperator)
    {
        if (isHightlited)
        {
            g.setColour(juce::Colours::lightgrey);
        }
        else
        {
            g.setColour(juce::Colours::white);
        }
        g.fillRoundedRectangle(0, 0, (float) width, (float) height, (float) height * 0.5f);
        g.setColour(juce::Colours::green);
        g.drawRoundedRectangle(2, 2, (float) (width - 4), (float) (height - 4), (float) (height - 2) * 0.5f, 1);
        g.drawEllipse(2, 2, (float) (height - 4), (float) (height - 4), 1);
        if (isTicked)
        {
            g.fillEllipse(4, 4, (float) (height - 8), (float) (height - 8));
        }
        g.drawText(text, height + 10, 0, width, height, juce::Justification::left, true);
    }
    if (hasSubmenu)
    {
        g.fillEllipse((float) (width + 5), 5, (float) (height - 10), (float) (height - 10));
    }
}

void D3CKLook::drawPopupMenuBackground(juce::Graphics& g, int width, int height)
{
    //g.fillAll(Colours::white);
}

std::unique_ptr<juce::DropShadower> D3CKLook::createDropShadowerForComponent(juce::Component& comp)
{
    return nullptr;
}
