#ifndef PIZ_CHANNEL_SLIDER_HEADER
#define PIZ_CHANNEL_SLIDER_HEADER

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

class ChannelSlider : public juce::Slider
{
public:
    ChannelSlider(juce::String name)
        : juce::Slider(name), allText("Any")
    {
        this->setRange(0, 16, 1);
    };

    ~ChannelSlider(){};

    juce::String getTextFromValue(double value)
    {
        if (value < 0.5)
            return allText;
        return juce::String((int) value);
    }

    double getValueFromText(const juce::String& text)
    {
        if (! text.compareIgnoreCase(allText))
            return 0;
        return text.getDoubleValue();
    }

    void setAllText(juce::String text)
    {
        allText = text;
        updateText();
    }

private:
    juce::String allText;
};

#endif
