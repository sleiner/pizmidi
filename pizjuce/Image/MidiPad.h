#ifndef imagePluginFilter_PAD_H
#define imagePluginFilter_PAD_H

#include <memory>

#include "../_common/PizAudioProcessor.h"

class MidiPad : public juce::Button
{
public:
    //==============================================================================
    MidiPad();
    ~MidiPad() override;

    void resized() override;
    void buttonClicked (juce::Button*);
    void setColour (const juce::Colour&);
    void setTextColour (const juce::Colour&);
    int getIndex()
    {
        return index;
    }
    juce::String getIconPath();
    void setIconPath (juce::String name);
    void setText (const juce::String& name);
    void setButtonText (const juce::String& newText);
    juce::String getText();

    //==============================================================================
    void setImages (const juce::Drawable* normalImage);
    bool setImageFromFile (juce::File file);

    //==============================================================================
    const juce::Drawable* getCurrentImage() const throw();
    const juce::Drawable* getNormalImage() const throw();
    const juce::Colour& getBackgroundColour() const throw();

    juce::String Description;
    float imageSize;
    void setCenteredText (bool centered);

protected:
    void drawButtonBackground (juce::Graphics& g,
                               Button& button,
                               const juce::Colour& backgroundColour,
                               bool isMouseOverButton,
                               bool isButtonDown);

    void paintButton (juce::Graphics& g,
                      bool isMouseOverButton,
                      bool isButtonDown) override;

private:
    bool centeredText;
    int index;
    std::unique_ptr<juce::Drawable> normalImage;
    juce::Colour bgColour, textColour;
    void deleteImages();
    juce::String iconPath;
    juce::Label* text;

    MidiPad (const MidiPad&);
    const MidiPad& operator= (const MidiPad&);

    JUCE_LEAK_DETECTOR (MidiPad)
};

#endif
