#ifndef MIDIPADS_PAD_H
#define MIDIPADS_PAD_H

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

#define midiScaler (0.007874016f)

class MidiPad : public juce::Button
{
public:
    //==============================================================================
    MidiPad(int _index);
    ~MidiPad() override;

    void resized() override;
    void buttonClicked(Button*);
    void setColour(const juce::Colour&);
    bool isPlaying;

    void setXFloat(float v)
    {
        x = v;
    }

    void setYFloat(float v)
    {
        y = v;
    }

    void setXInt(int v)
    {
        x = v * midiScaler;
    }

    void setYInt(int v)
    {
        y = v * midiScaler;
    }

    int getIndex()
    {
        return index;
    }

    juce::String getIconPath();
    void setIconPath(juce::String name);
    void setText(const juce::String& name);
    juce::String getText();

    //==============================================================================
    void setImages(const juce::Drawable* normalImage);
    bool setImageFromFile(juce::File file);

    //==============================================================================
    void setBackgroundColours(const juce::Colour& toggledOffColour,
                              const juce::Colour& toggledOnColour);

    const juce::Colour& getBackgroundColour() const throw();

    //==============================================================================
    const juce::Drawable* getCurrentImage() const throw();
    const juce::Drawable* getNormalImage() const throw();

    juce::String Description;
    bool showx, showy;
    float roundness;
    bool showdot;
    bool showvalues;
    float imageSize;
    void setCenteredText(bool centered);
    void setHex(bool newhex);
    bool isHex();

protected:
    void
        drawButtonBackground(juce::Graphics& g,
                             Button& button,
                             const juce::Colour& backgroundColour,
                             bool isMouseOverButton,
                             bool isButtonDown);

    void paintButton(juce::Graphics& g,
                     bool isMouseOverButton,
                     bool isButtonDown) override;

private:
    bool centeredText;
    float x;
    float y;
    int index;
    bool hitTest(int x, int y) override;
    std::unique_ptr<juce::Drawable> normalImage;
    juce::Colour backgroundOff, backgroundOn;
    juce::Path hexpath;
    bool hex;
    void deleteImages();
    juce::String iconPath;
    juce::Label* text;

    MidiPad(const MidiPad&);
    const MidiPad& operator=(const MidiPad&);

    JUCE_LEAK_DETECTOR(MidiPad)
};

#endif
