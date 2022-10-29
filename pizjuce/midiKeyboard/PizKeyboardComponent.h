#ifndef PIZMIDIKEYBOARDCOMPONENT_H
#define PIZMIDIKEYBOARDCOMPONENT_H

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_graphics/juce_graphics.h>

#include "PizKeyboard.h"

class PizKeyboardComponent : public juce::MidiKeyboardComponent
{
public:
    PizKeyboardComponent(juce::MidiKeyboardState& state, const Orientation orientation);
    bool keyStateChanged(bool isKeyDown) override;
    void setKeyPressForNote(const juce::KeyPress& key, const int midiNoteOffsetFromC)
    {
        MidiKeyboardComponent::setKeyPressForNote(key, midiNoteOffsetFromC);
        _keyPressNotes.add(midiNoteOffsetFromC);
        _keyPresses.add(key);
    }
    void setKeyPressBaseOctave(int newOctaveNumber)
    {
        baseOctave = newOctaveNumber;
        MidiKeyboardComponent::setKeyPressBaseOctave(newOctaveNumber);
        repaint();
        sendChangeMessage();
    }
    int getKeyPressBaseOctave()
    {
        return baseOctave;
    }
    bool toggle;
    void drawBlackNote(int midiNoteNumber, juce::Graphics& g, juce::Rectangle<float> area, bool isDown, bool isOver, const juce::Colour& textColour)
    {
        auto x = area.getX(), y = area.getY(), w = area.getWidth(), h = area.getHeight();
        MidiKeyboardComponent::drawBlackNote(midiNoteNumber, g, area, isDown, isOver, textColour);
        if (hasKeyboardFocus(false) || drawQwerty)
        {
            if (midiNoteNumber >= baseOctave * 12 && midiNoteNumber < 32 + baseOctave * 12)
            {
                int n            = midiNoteNumber - baseOctave * 12;
                juce::String key = juce::String::charToString(keymap[n]);
                g.setColour(juce::Colours::white.withMultipliedAlpha(0.5f));
                if (midiNoteNumber / 12 == baseOctave && midiNoteNumber % 12 == 0)
                {
                    g.drawEllipse((float) (x + w / 4), (float) (y + h - w), (float) (w / 2), (float) (w / 2), 2.f);
                }
                g.setFont(juce::Font((float) (w / 2), juce::Font::bold));
                g.drawText(key, x + w / 4, y + h - w, w / 2, w / 2, juce::Justification::centred, false);
            }
        }
        if (drawNoteNumber)
        {
            juce::Font f(juce::jmin(12.0f, w * 0.9f));
            f.setHorizontalScale(0.8f);
            g.setFont(f);
            g.setColour(juce::Colours::white);
            g.drawFittedText(juce::String(midiNoteNumber), x + 2, y + 2, w - 4, h - 4, juce::Justification::centredBottom, 1);
        }
    }
    void drawWhiteNote(int midiNoteNumber, juce::Graphics& g, juce::Rectangle<float> area, bool isDown, bool isOver, const juce::Colour& lineColour, const juce::Colour& textColour)
    {
        auto x = area.getX(), y = area.getY(), w = area.getWidth(), h = area.getHeight();
        MidiKeyboardComponent::drawWhiteNote(midiNoteNumber, g, area, isDown, isOver, lineColour, drawNoteNumber ? juce::Colours::transparentWhite : textColour);
        if (hasKeyboardFocus(false) || drawQwerty)
        {
            if (midiNoteNumber >= baseOctave * 12 && midiNoteNumber < 32 + baseOctave * 12)
            {
                int n            = midiNoteNumber - baseOctave * 12;
                juce::String key = juce::String::charToString(keymap[n]);
                g.setColour(juce::Colours::grey.withMultipliedAlpha(0.5f));
                if (midiNoteNumber / 12 == baseOctave && midiNoteNumber % 12 == 0)
                {
                    g.drawEllipse((float) (x + w / 4), (float) (y + h - 3 * w / 4), (float) (w / 2), (float) (w / 2), 2.f);
                }
                g.setFont(juce::Font((float) (w / 2), juce::Font::bold));
                g.drawText(key, x + w / 4, y + h - 3 * w / 4, w / 2, w / 2, juce::Justification::centred, false);
            }
        }
        if (drawNoteNumber)
        {
            juce::Font f(juce::jmin(12.0f, w * 0.9f));
            f.setHorizontalScale(0.8f);
            //f.setBold (midiNoteNumber%12==0);
            g.setFont(f);
            g.setColour(textColour);
            g.drawFittedText(juce::String(midiNoteNumber), x + 2, y + 2, w - 4, h - 4, juce::Justification::centredBottom, 1);
        }
    }

    void focusLost(Component::FocusChangeType cause) override
    {
        repaint();
    }

    void setDrawQwerty(bool qwerty)
    {
        drawQwerty = qwerty;
        repaint();
    }

    void setDrawNoteNumber(bool shouldDraw)
    {
        drawNoteNumber = shouldDraw;
        repaint();
    }

private:
    int baseOctave;
    juce::Array<juce::KeyPress> _keyPresses;
    juce::BigInteger _keysPressed;
    juce::Array<int> _keyPressNotes;
    bool mouseDownOnKey(int midiNoteNumber, const juce::MouseEvent& e) override;
    juce::MidiKeyboardState* s;
    bool drawQwerty;
    bool drawNoteNumber;

    bool isBlackKey(int noteNumber)
    {
        switch (noteNumber % 12)
        {
            case 0:
                return false; //C
            case 1:
                return true; //C#
            case 2:
                return false; //D
            case 3:
                return true; //D#
            case 4:
                return false; //E
            case 5:
                return false; //F
            case 6:
                return true; //F#
            case 7:
                return false; //G
            case 8:
                return true; //G#
            case 9:
                return false; //A
            case 10:
                return true; //A#
            case 11:
                return false; //B

            default:
                return false;
        }
    }
};

#endif
