#include "PianoRollBackground.h"

#include "PianoRoll.h"

PianoRollBackground::PianoRollBackground()
{
    setBufferedToImage(true);
}

PianoRollBackground::~PianoRollBackground() = default;

void PianoRollBackground::paint(juce::Graphics& g)
{
    const PianoRoll* roll = (PianoRoll*) getParentComponent();
    int noteNumber        = 0;
    auto y                = (float) getHeight();
    float yinc            = (float) getHeight() / 128.f;

    while (y > 0)
    {
        if (getNoteNameWithoutOctave(noteNumber).contains("#"))
        {
            g.setColour(juce::Colour::fromString("ffe9e9e9"));
        }
        else if (noteNumber == 60)
        {
            g.setColour(juce::Colours::yellow);
        }
        else
        {
            g.setColour(juce::Colours::white);
        }

        g.fillRect(0.f, std::round(y - yinc), (float) getWidth(), std::round(yinc));

        if (getNoteNameWithoutOctave(noteNumber).contains("F") && ! getNoteNameWithoutOctave(noteNumber).contains("#"))
        {
            g.setColour(juce::Colour::fromString("ffd1d1d1"));
            g.drawHorizontalLine(y, 0.f, (float) getWidth());
        }

        if (getNoteNameWithoutOctave(noteNumber).contains("C") && ! getNoteNameWithoutOctave(noteNumber).contains("#"))
        {
            g.setColour(juce::Colour::fromString("ff808080"));
            g.drawHorizontalLine(y, 0.f, (float) getWidth());
        }

        noteNumber++;
        y -= yinc;
    }

    float x = roll->gridSize;
    while (x < getWidth())
    {
        //draw grid
        if (fmod(x, roll->barSize) >= 0.0001 && fmod(x, roll->beatSize) >= 0.0001)
        {
            g.setColour(juce::Colours::lightgrey);
            g.drawVerticalLine(x, 0.f, (float) getHeight());
        }
        x += roll->gridSize;
    }

    x = roll->beatSize;
    while (x < getWidth())
    {
        //draw beats
        if (fmod(x, roll->barSize) >= 0.0001)
        {
            g.setColour(juce::Colour::fromString("ffb3b3b3"));
            g.drawVerticalLine(x, 0.f, (float) getHeight());
        }
        x += roll->beatSize;
    }

    x = roll->barSize;
    while (x < getWidth())
    {
        //draw bars
        g.setColour(juce::Colour::fromString("ff808080"));
        g.drawVerticalLine(x, 0.f, (float) getHeight());
        x += roll->barSize;
    }
}
