#include "DrawablePad.h"

//==============================================================================
DrawablePad::DrawablePad(const juce::String& name)
    : Button(name),
      //buttonState(buttonNormal),
      normalImage(nullptr),
      overImage(nullptr),
      downImage(nullptr),
      disabledImage(nullptr),
      normalImageOn(nullptr),
      overImageOn(nullptr),
      downImageOn(nullptr),
      disabledImageOn(nullptr)
{
    setSize(200, 200);
    backgroundOff = juce::Colour(0xffbbbbff);
    backgroundOn  = juce::Colour(0xff3333ff);
    Label         = "Pad";
    setMouseClickGrabsKeyboardFocus(false);
}

void DrawablePad::deleteImages()
{
    normalImage.reset();
    overImage.reset();
    downImage.reset();
    disabledImage.reset();
    normalImageOn.reset();
    overImageOn.reset();
    downImageOn.reset();
    disabledImageOn.reset();
}

void DrawablePad::setImages(const juce::Drawable* normal,
                            const juce::Drawable* over,
                            const juce::Drawable* down,
                            const juce::Drawable* disabled,
                            const juce::Drawable* normalOn,
                            const juce::Drawable* overOn,
                            const juce::Drawable* downOn,
                            const juce::Drawable* disabledOn)
{
    deleteImages();

    jassert(normal != nullptr); // you really need to give it at least a normal image..

    if (normal != nullptr)
    {
        normalImage = normal->createCopy();
    }
    if (over != nullptr)
    {
        overImage = over->createCopy();
    }
    if (down != nullptr)
    {
        downImage = down->createCopy();
    }
    if (disabled != nullptr)
    {
        disabledImage = disabled->createCopy();
    }
    if (normalOn != nullptr)
    {
        normalImageOn = normalOn->createCopy();
    }
    if (overOn != nullptr)
    {
        overImageOn = overOn->createCopy();
    }
    if (downOn != nullptr)
    {
        downImageOn = downOn->createCopy();
    }
    if (disabledOn != nullptr)
    {
        disabledImageOn = disabledOn->createCopy();
    }

    repaint();
}

static std::unique_ptr<juce::Drawable> drawableFromFile(const juce::File& f)
{
    if (f.exists())
    {
        return juce::Drawable::createFromImageFile(f);
    }

    return nullptr;
}

void DrawablePad::setImages(const juce::File normalImage,
                            const juce::File overImage,
                            const juce::File downImage,
                            const juce::File disabledImage,
                            const juce::File normalImageOn,
                            const juce::File overImageOn,
                            const juce::File downImageOn,
                            const juce::File disabledImageOn)
{
    setImagesFromUptr(drawableFromFile(normalImage),
                      drawableFromFile(overImage),
                      drawableFromFile(downImage),
                      drawableFromFile(disabledImage),
                      drawableFromFile(normalImageOn),
                      drawableFromFile(overImageOn),
                      drawableFromFile(downImageOn),
                      drawableFromFile(disabledImageOn));
}

void DrawablePad::setBackgroundColours(const juce::Colour& toggledOffColour,
                                       const juce::Colour& toggledOnColour)
{
    if (backgroundOff != toggledOffColour
        || backgroundOn != toggledOnColour)
    {
        backgroundOff = toggledOffColour;
        backgroundOn  = toggledOnColour;

        repaint();
    }
}

const juce::Colour& DrawablePad::getBackgroundColour() const throw()
{
    return getToggleState() ? backgroundOn
                            : backgroundOff;
}

void DrawablePad::drawButtonBackground(juce::Graphics& g,
                                       Button& button,
                                       const juce::Colour& backgroundColour,
                                       bool isMouseOverButton,
                                       bool isButtonDown)
{
}

void DrawablePad::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    juce::Rectangle<int> imageSpace;
    const int insetX = getWidth() / 100;
    const int insetY = getHeight() / 100;
    imageSpace.setBounds(insetX, insetY, getWidth() - insetX * 2, getHeight() - insetY * 2);

    g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);
    g.setOpacity(1.0f);

    const juce::Drawable* imageToDraw = nullptr;

    if (isEnabled())
    {
        imageToDraw = getCurrentImage();
    }
    else
    {
        imageToDraw = getToggleState() ? disabledImageOn.get()
                                       : disabledImage.get();

        if (imageToDraw == nullptr)
        {
            g.setOpacity(0.4f);
            imageToDraw = getNormalImage();
        }
    }

    if (imageToDraw != nullptr)
    {
        g.fillAll(juce::Colours::transparentBlack);
        imageToDraw->drawWithin(g,
                                imageSpace.toFloat(),
                                juce::RectanglePlacement::centred,
                                1.f);
    }
    else
    {
        g.fillAll(getBackgroundColour());
        //	g.setColour (getBackgroundColour().contrasting(0.8f));
        //	g.setFont (Font (9.f, Font::bold));
        //	g.drawText (Label,
        //				proportionOfWidth (0.0447f), proportionOfHeight (0.0499f), proportionOfWidth (0.9137f), proportionOfHeight (0.9137f),
        //				Justification::centred, true);
    }
}

void DrawablePad::resized()
{
}

const juce::Drawable* DrawablePad::getCurrentImage() const throw()
{
    if (isDown())
    {
        return getDownImage();
    }

    if (isOver())
    {
        return getOverImage();
    }

    return getNormalImage();
}

const juce::Drawable* DrawablePad::getNormalImage() const throw()
{
    return (getToggleState() && normalImageOn != nullptr) ? normalImageOn.get()
                                                          : normalImage.get();
}

const juce::Drawable* DrawablePad::getOverImage() const throw()
{
    const juce::Drawable* d = normalImage.get();

    if (getToggleState())
    {
        if (overImageOn != nullptr)
        {
            d = overImageOn.get();
        }
        else if (normalImageOn != nullptr)
        {
            d = normalImageOn.get();
        }
        else if (overImage != nullptr)
        {
            d = overImage.get();
        }
    }
    else
    {
        if (overImage != nullptr)
        {
            d = overImage.get();
        }
    }

    return d;
}

const juce::Drawable* DrawablePad::getDownImage() const throw()
{
    const juce::Drawable* d = normalImage.get();

    if (getToggleState())
    {
        if (downImageOn != nullptr)
        {
            d = downImageOn.get();
        }
        else if (overImageOn != nullptr)
        {
            d = overImageOn.get();
        }
        else if (normalImageOn != nullptr)
        {
            d = normalImageOn.get();
        }
        else if (downImage != nullptr)
        {
            d = downImage.get();
        }
        else
        {
            d = getOverImage();
        }
    }
    else
    {
        if (downImage != nullptr)
        {
            d = downImage.get();
        }
        else
        {
            d = getOverImage();
        }
    }

    return d;
}
