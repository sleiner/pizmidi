#ifndef PIZ_NOTE_SLIDER_HEADER
#define PIZ_NOTE_SLIDER_HEADER

#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "piz/midistuff.h"

class NoteSlider : public juce::Slider
{
public:
    NoteSlider (juce::String name) : juce::Slider (name), allText ("Off"), bottomOctave (-2)
    {
        this->setRange (-1, 127, 1);
    };
    ~NoteSlider(){};

    juce::String getTextFromValue (double value)
    {
        const int n = juce::roundToInt (value);
        if (n < 0)
            return allText;
        return getNoteName (n, bottomOctave) + " (" + juce::String ((int) value) + ")";
    }

    double getValueFromText (const juce::String& text)
    {
        if (! text.compareIgnoreCase (allText))
            return -1;
        if (text.containsAnyOf ("abcdefgABCDEFG"))
        {
            bool hasOctave;
            int n = getNoteValue (text, bottomOctave, hasOctave);
            if (n != NOT_A_NOTE)
                return n;
            return getValue();
        }
        return text.getDoubleValue();
    }

    void setAllText (juce::String text)
    {
        allText = text;
        updateText();
    }
    void setBottomOctave (int o)
    {
        bottomOctave = o;
    }

private:
    juce::String allText;
    int bottomOctave;
};

#endif
