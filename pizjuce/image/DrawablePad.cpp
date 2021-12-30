#include "DrawablePad.h"

//==============================================================================
DrawablePad::DrawablePad (const String& name)
    : Button (name),
      normalImage (nullptr)
{
    setSize (200, 200);
    backgroundOff = Colour (0xffbbbbff);
    backgroundOn = Colour (0xff3333ff);
    setMouseClickGrabsKeyboardFocus (false);
}

DrawablePad::~DrawablePad()
{
    deleteImages();
}

void DrawablePad::deleteImages()
{
    normalImage.reset();
}

void DrawablePad::setImages (File file)
{
	deleteImages();
	if (file.exists())
		normalImage = Drawable::createFromImageFile(file);
	repaint();
}

void DrawablePad::setBackgroundColours (const Colour& toggledOffColour,
                                const Colour& toggledOnColour)
{
    if (backgroundOff != toggledOffColour
         || backgroundOn != toggledOnColour)
    {
        backgroundOff = toggledOffColour;
        backgroundOn = toggledOnColour;

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
    //const int width = button.getWidth();
    //const int height = button.getHeight();

    //const float indent = 2.0f;
    //const int cornerSize = jmin (roundFloatToInt (width * roundness),
    //                             roundFloatToInt (height * roundness));

    //Colour bc (backgroundColour);
    //if (isMouseOverButton)
    //{
    //    if (isButtonDown)
    //        bc = bc.brighter();
    //    else if (bc.getBrightness() > 0.5f)
    //        bc = bc.darker (0.1f);
    //    else
    //        bc = bc.brighter (0.1f);
    //}

    //if (hex) {
    //    g.setColour (bc);
    //    g.fillPath (hexpath);
    //    g.setColour (bc.contrasting().withAlpha ((isMouseOverButton) ? 0.6f : 0.4f));
    //    g.strokePath (hexpath, PathStrokeType ((isMouseOverButton) ? 2.0f : 1.4f));
    //}
    //else {
    //    Path p;
    //    p.addRoundedRectangle (indent, indent,
    //                           width - indent * 2.0f,
    //                           height - indent * 2.0f,
    //                           (float) cornerSize);
    //    g.setColour (bc);
    //    g.fillPath (p);
    //    g.setColour (bc.contrasting().withAlpha ((isMouseOverButton) ? 0.6f : 0.4f));
    //    g.strokePath (p, PathStrokeType ((isMouseOverButton) ? 2.0f : 1.4f));
    //}
}

void DrawablePad::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    g.fillAll(getBackgroundColour());

    const int insetX = getWidth() / 100;
    const int insetY = getHeight() / 100;
    Rectangle<int> imageSpace(insetX, insetY, getWidth() - insetX * 2, getHeight() - insetY * 2);

    g.setImageResamplingQuality (Graphics::highResamplingQuality);
    g.setOpacity (1.0f);

    const Drawable* imageToDraw = 0;

    if (isEnabled())
    {
        imageToDraw = getCurrentImage();
    }
    //else
    //{
    //    imageToDraw = getToggleState() ? disabledImageOn
    //                                   : disabledImage;

    //    if (imageToDraw == 0)
    //    {
    //        g.setOpacity (0.4f);
    //        imageToDraw = getNormalImage();
    //    }
    //}

    if (imageToDraw != 0)
    {
        imageToDraw->drawWithin (g,
                                 imageSpace.toType<float>(),
                                 RectanglePlacement::centred,
								 1.f);
    }
}
void DrawablePad::resized() {
}

const Drawable* DrawablePad::getCurrentImage() const throw()
{

    return getNormalImage();
}

const Drawable* DrawablePad::getNormalImage() const throw()
{
    return normalImage.get();
}

const Drawable* DrawablePad::getOverImage() const throw()
{
    return normalImage.get();
}

const Drawable* DrawablePad::getDownImage() const throw()
{
    return normalImage.get();
}
