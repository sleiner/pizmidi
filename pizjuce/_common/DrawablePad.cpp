#include "DrawablePad.h"

//==============================================================================
DrawablePad::DrawablePad (const String& name)
    : Button (name),
      //buttonState(buttonNormal),
      normalImage (nullptr),
      overImage (nullptr),
      downImage (nullptr),
      disabledImage (nullptr),
      normalImageOn (nullptr),
      overImageOn (nullptr),
      downImageOn (nullptr),
      disabledImageOn (nullptr)
{
    setSize (200, 200);
    backgroundOff = Colour (0xffbbbbff);
    backgroundOn  = Colour (0xff3333ff);
    Label         = "Pad";
    setMouseClickGrabsKeyboardFocus (false);
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

void DrawablePad::setImages (const Drawable* normal,
                             const Drawable* over,
                             const Drawable* down,
                             const Drawable* disabled,
                             const Drawable* normalOn,
                             const Drawable* overOn,
                             const Drawable* downOn,
                             const Drawable* disabledOn)
{
    deleteImages();

    jassert (normal != nullptr); // you really need to give it at least a normal image..

    if (normal != nullptr)
        normalImage = normal->createCopy();
    if (over != nullptr)
        overImage = over->createCopy();
    if (down != nullptr)
        downImage = down->createCopy();
    if (disabled != nullptr)
        disabledImage = disabled->createCopy();
    if (normalOn != nullptr)
        normalImageOn = normalOn->createCopy();
    if (overOn != nullptr)
        overImageOn = overOn->createCopy();
    if (downOn != nullptr)
        downImageOn = downOn->createCopy();
    if (disabledOn != nullptr)
        disabledImageOn = disabledOn->createCopy();

    repaint();
}

static std::unique_ptr<Drawable> drawableFromFile (const File& f)
{
    if (f.exists())
    {
        return Drawable::createFromImageFile (f);
    }

    return nullptr;
}

void DrawablePad::setImages (const File normalImage,
                             const File overImage,
                             const File downImage,
                             const File disabledImage,
                             const File normalImageOn,
                             const File overImageOn,
                             const File downImageOn,
                             const File disabledImageOn)
{
    setImagesFromUptr (drawableFromFile (normalImage),
                       drawableFromFile (overImage),
                       drawableFromFile (downImage),
                       drawableFromFile (disabledImage),
                       drawableFromFile (normalImageOn),
                       drawableFromFile (overImageOn),
                       drawableFromFile (downImageOn),
                       drawableFromFile (disabledImageOn));
}

void DrawablePad::setBackgroundColours (const Colour& toggledOffColour,
                                        const Colour& toggledOnColour)
{
    if (backgroundOff != toggledOffColour
        || backgroundOn != toggledOnColour)
    {
        backgroundOff = toggledOffColour;
        backgroundOn  = toggledOnColour;

        repaint();
    }
}

const Colour& DrawablePad::getBackgroundColour() const throw()
{
    return getToggleState() ? backgroundOn
                            : backgroundOff;
}

void DrawablePad::drawButtonBackground (Graphics& g,
                                        Button& button,
                                        const Colour& backgroundColour,
                                        bool isMouseOverButton,
                                        bool isButtonDown)
{
}

void DrawablePad::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    Rectangle<int> imageSpace;
    const int insetX = getWidth() / 100;
    const int insetY = getHeight() / 100;
    imageSpace.setBounds (insetX, insetY, getWidth() - insetX * 2, getHeight() - insetY * 2);

    g.setImageResamplingQuality (Graphics::highResamplingQuality);
    g.setOpacity (1.0f);

    const Drawable* imageToDraw = nullptr;

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
            g.setOpacity (0.4f);
            imageToDraw = getNormalImage();
        }
    }

    if (imageToDraw != nullptr)
    {
        g.fillAll (Colours::transparentBlack);
        imageToDraw->drawWithin (g,
                                 imageSpace.toFloat(),
                                 RectanglePlacement::centred,
                                 1.f);
    }
    else
    {
        g.fillAll (getBackgroundColour());
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

const Drawable* DrawablePad::getCurrentImage() const throw()
{
    if (isDown())
        return getDownImage();

    if (isOver())
        return getOverImage();

    return getNormalImage();
}

const Drawable* DrawablePad::getNormalImage() const throw()
{
    return (getToggleState() && normalImageOn != nullptr) ? normalImageOn.get()
                                                          : normalImage.get();
}

const Drawable* DrawablePad::getOverImage() const throw()
{
    const Drawable* d = normalImage.get();

    if (getToggleState())
    {
        if (overImageOn != nullptr)
            d = overImageOn.get();
        else if (normalImageOn != nullptr)
            d = normalImageOn.get();
        else if (overImage != nullptr)
            d = overImage.get();
    }
    else
    {
        if (overImage != nullptr)
            d = overImage.get();
    }

    return d;
}

const Drawable* DrawablePad::getDownImage() const throw()
{
    const Drawable* d = normalImage.get();

    if (getToggleState())
    {
        if (downImageOn != nullptr)
            d = downImageOn.get();
        else if (overImageOn != nullptr)
            d = overImageOn.get();
        else if (normalImageOn != nullptr)
            d = normalImageOn.get();
        else if (downImage != nullptr)
            d = downImage.get();
        else
            d = getOverImage();
    }
    else
    {
        if (downImage != nullptr)
            d = downImage.get();
        else
            d = getOverImage();
    }

    return d;
}
