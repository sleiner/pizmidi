#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    setColour(juce::ScrollBar::ColourIds::thumbColourId, juce::Colour::fromString("FFdddddd"));

    setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colour::fromString("FF999999"));
    setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colour::fromString("ff464646"));
    setColour(juce::ComboBox::ColourIds::focusedOutlineColourId, juce::Colour::fromString("ff464646"));
    setColour(juce::ComboBox::ColourIds::textColourId, juce::Colour::fromString("ff222222"));
    setColour(juce::ComboBox::ColourIds::arrowColourId, juce::Colour::fromString("ff222222"));
}

void CustomLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool, int, int, int, int, juce::ComboBox& box)
{
    auto cornerSize = box.findParentComponentOfClass<juce::ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
    juce::Rectangle<int> boxBounds(0, 0, width, height);

    g.setColour(box.findColour(juce::ComboBox::backgroundColourId));
    g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);

    g.setColour(box.findColour(juce::ComboBox::outlineColourId));
    g.drawRoundedRectangle(boxBounds.toFloat().reduced(0.5f, 0.5f), cornerSize, 1.0f);

    juce::Rectangle<float> arrowZone((float) (width - height - (height * 0.2f)), 0.0f, (float) height, (float) height);
    juce::Rectangle<float> actualArrowZone = arrowZone.reduced(height * 0.05f).withTrimmedTop(height * 0.3f).withTrimmedLeft(height * 0.2f).withTrimmedRight(height * 0.2f).withTrimmedBottom(height * 0.3f + height * 0.05f);

    float verticalOffset = actualArrowZone.getHeight() * 0.5f;
    juce::Path path;
    path.startNewSubPath(actualArrowZone.getX(), actualArrowZone.getCentreY() - verticalOffset);
    path.lineTo(actualArrowZone.getCentreX(), actualArrowZone.getCentreY() + verticalOffset);
    path.lineTo(actualArrowZone.getRight(), actualArrowZone.getCentreY() - verticalOffset);

    g.setColour(box.findColour(juce::ComboBox::arrowColourId).withAlpha((box.isEnabled() ? 0.9f : 0.2f)));
    g.strokePath(path, juce::PathStrokeType(height * 0.1f));
}