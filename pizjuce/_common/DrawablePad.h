#ifndef MIDIPADS_DRAWABLEPAD_H
#define MIDIPADS_DRAWABLEPAD_H

#include <memory>

#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

class DrawablePad : public Button
{
public:
    //==============================================================================
    DrawablePad (const String& buttonName);

    //==============================================================================
    void setImages (File normalImage,
                    File overImage = File(),
                    File downImage = File(),
                    File disabledImage = File(),
                    File normalImageOn = File(),
                    File overImageOn = File(),
                    File downImageOn = File(),
                    File disabledImageOn = File());
    void setImages (const Drawable* normalImage,
                    const Drawable* overImage = 0,
                    const Drawable* downImage = 0,
                    const Drawable* disabledImage = 0,
                    const Drawable* normalImageOn = 0,
                    const Drawable* overImageOn = 0,
                    const Drawable* downImageOn = 0,
                    const Drawable* disabledImageOn = 0);
    void setImagesFromUptr (std::unique_ptr<Drawable> normalImage,
                            std::unique_ptr<Drawable> overImage = nullptr,
                            std::unique_ptr<Drawable> downImage = nullptr,
                            std::unique_ptr<Drawable> disabledImage = nullptr,
                            std::unique_ptr<Drawable> normalImageOn = nullptr,
                            std::unique_ptr<Drawable> overImageOn = nullptr,
                            std::unique_ptr<Drawable> downImageOn = nullptr,
                            std::unique_ptr<Drawable> disabledImageOn = nullptr)
    {
        // TODO: Remove the raw-pointer function, then the additional copies
        //       can easily be eliminated.
        return setImages (normalImage.get(),
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
    void setBackgroundColours (const Colour& toggledOffColour,
                               const Colour& toggledOnColour);

    const Colour& getBackgroundColour() const throw();

    //==============================================================================
    /** Returns the image that the button is currently displaying. */
    const Drawable* getCurrentImage() const throw();
    const Drawable* getNormalImage() const throw();
    const Drawable* getOverImage() const throw();
    const Drawable* getDownImage() const throw();

    //==============================================================================
    juce_UseDebuggingNewOperator

        String Label;

protected:
    /** @internal */
    void drawButtonBackground (Graphics& g,
                               Button& button,
                               const Colour& backgroundColour,
                               bool isMouseOverButton,
                               bool isButtonDown);

    void paintButton (Graphics& g,
                      bool isMouseOverButton,
                      bool isButtonDown) override;
    void resized() override;

private:
    //==============================================================================
    std::unique_ptr<Drawable> normalImage;
    std::unique_ptr<Drawable> overImage;
    std::unique_ptr<Drawable> downImage;
    std::unique_ptr<Drawable> disabledImage;
    std::unique_ptr<Drawable> normalImageOn;
    std::unique_ptr<Drawable> overImageOn;
    std::unique_ptr<Drawable> downImageOn;
    std::unique_ptr<Drawable> disabledImageOn;
    Colour backgroundOff, backgroundOn;
    DrawablePad (const DrawablePad&);
    const DrawablePad& operator= (const DrawablePad&);
};

#endif
