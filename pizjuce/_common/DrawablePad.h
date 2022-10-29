#ifndef MIDIPADS_DRAWABLEPAD_H
#define MIDIPADS_DRAWABLEPAD_H

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

class DrawablePad : public juce::Button
{
public:
    //==============================================================================
    DrawablePad(const juce::String& buttonName);

    //==============================================================================
    void setImages(juce::File normalImage,
                   juce::File overImage       = juce::File(),
                   juce::File downImage       = juce::File(),
                   juce::File disabledImage   = juce::File(),
                   juce::File normalImageOn   = juce::File(),
                   juce::File overImageOn     = juce::File(),
                   juce::File downImageOn     = juce::File(),
                   juce::File disabledImageOn = juce::File());
    void setImages(const juce::Drawable* normalImage,
                   const juce::Drawable* overImage       = 0,
                   const juce::Drawable* downImage       = 0,
                   const juce::Drawable* disabledImage   = 0,
                   const juce::Drawable* normalImageOn   = 0,
                   const juce::Drawable* overImageOn     = 0,
                   const juce::Drawable* downImageOn     = 0,
                   const juce::Drawable* disabledImageOn = 0);
    void setImagesFromUptr(std::unique_ptr<juce::Drawable> normalImage,
                           std::unique_ptr<juce::Drawable> overImage       = nullptr,
                           std::unique_ptr<juce::Drawable> downImage       = nullptr,
                           std::unique_ptr<juce::Drawable> disabledImage   = nullptr,
                           std::unique_ptr<juce::Drawable> normalImageOn   = nullptr,
                           std::unique_ptr<juce::Drawable> overImageOn     = nullptr,
                           std::unique_ptr<juce::Drawable> downImageOn     = nullptr,
                           std::unique_ptr<juce::Drawable> disabledImageOn = nullptr)
    {
        // TODO: Remove the raw-pointer function, then the additional copies
        //       can easily be eliminated.
        return setImages(normalImage.get(),
                         overImage.get(),
                         downImage.get(),
                         disabledImage.get(),
                         normalImageOn.get(),
                         overImageOn.get(),
                         downImageOn.get(),
                         disabledImageOn.get());
    }
    void deleteImages();

    //==============================================================================
    void setBackgroundColours(const juce::Colour& toggledOffColour,
                              const juce::Colour& toggledOnColour);

    const juce::Colour& getBackgroundColour() const throw();

    //==============================================================================
    /** Returns the image that the button is currently displaying. */
    const juce::Drawable* getCurrentImage() const throw();
    const juce::Drawable* getNormalImage() const throw();
    const juce::Drawable* getOverImage() const throw();
    const juce::Drawable* getDownImage() const throw();

    juce::String Label;

protected:
    /** @internal */
    void drawButtonBackground(juce::Graphics& g,
                              Button& button,
                              const juce::Colour& backgroundColour,
                              bool isMouseOverButton,
                              bool isButtonDown);

    void paintButton(juce::Graphics& g,
                     bool isMouseOverButton,
                     bool isButtonDown) override;
    void resized() override;

private:
    std::unique_ptr<juce::Drawable> normalImage;
    std::unique_ptr<juce::Drawable> overImage;
    std::unique_ptr<juce::Drawable> downImage;
    std::unique_ptr<juce::Drawable> disabledImage;
    std::unique_ptr<juce::Drawable> normalImageOn;
    std::unique_ptr<juce::Drawable> overImageOn;
    std::unique_ptr<juce::Drawable> downImageOn;
    std::unique_ptr<juce::Drawable> disabledImageOn;
    juce::Colour backgroundOff, backgroundOn;
    DrawablePad(const DrawablePad&);
    const DrawablePad& operator=(const DrawablePad&);

    JUCE_LEAK_DETECTOR(DrawablePad)
};

#endif
