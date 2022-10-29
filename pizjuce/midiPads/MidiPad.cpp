#include "MidiPad.h"

using juce::jlimit;
using juce::jmax;
using juce::jmin;
using juce::roundToInt;

MidiPad::MidiPad(int _index)
    : Button("MidiPad"),
      normalImage(nullptr),
      index(_index),
      text(nullptr)

{
    backgroundOff = juce::Colour(0xffbbbbff);
    backgroundOn  = juce::Colour(0xff3333ff);

    Description  = "x:0 y:0";
    showdot      = false;
    showvalues   = false;
    showx        = false;
    showy        = false;
    hex          = false;
    centeredText = false;
    x            = 0;
    y            = 0;
    roundness    = 0.4f;
    imageSize    = 0.5f;
    isPlaying    = false;
    setMouseClickGrabsKeyboardFocus(false);

    addAndMakeVisible(text = new juce::Label("label", ""));
    text->setMouseClickGrabsKeyboardFocus(false);
    text->setJustificationType(juce::Justification::centredTop);
    text->setEditable(false, false, false);
    text->setInterceptsMouseClicks(false, false);

    setSize(200, 200);
}

MidiPad::~MidiPad()
{
    if (text)
        delete text;
    deleteImages();
}

void MidiPad::deleteImages()
{
    normalImage.reset();
}

bool MidiPad::setImageFromFile(juce::File file)
{
    if (file.exists())
    {
        deleteImages();

        normalImage = juce::Drawable::createFromImageFile(file);
        if (normalImage != nullptr)
        {
            repaint();
        }
        else
            return false;
        return true;
    }
    return false;
}

void MidiPad::setImages(const juce::Drawable* normal)
{
    deleteImages();
    if (normal != nullptr)
    {
        normalImage = normal->createCopy();
    }
    repaint();
}

void MidiPad::setIconPath(juce::String name)
{
    iconPath = name;
}

juce::String MidiPad::getIconPath()
{
    return iconPath;
}

void MidiPad::setBackgroundColours(const juce::Colour& toggledOffColour,
                                   const juce::Colour& toggledOnColour)
{
    if (backgroundOff != toggledOffColour) // || backgroundOn != toggledOnColour)
    {
        backgroundOff = toggledOffColour;
        //backgroundOn = toggledOnColour;
        backgroundOn = toggledOffColour.contrasting(0.5f).withMultipliedAlpha(2.0);
        repaint();
    }
}

const juce::Colour& MidiPad::getBackgroundColour() const throw()
{
    return getToggleState() ? backgroundOn
                            : backgroundOff;
}

void MidiPad::drawButtonBackground(juce::Graphics& g,
                                   Button& button,
                                   const juce::Colour& backgroundColour,
                                   bool isMouseOverButton,
                                   bool isButtonDown)
{
    const int width  = button.getWidth();
    const int height = button.getHeight();

    const float indent   = 2.0f;
    roundness            = jlimit(0.f, 1.f, roundness);
    const int cornerSize = jmin(roundToInt(width * roundness),
                                roundToInt(height * roundness));
    juce::Colour bc(backgroundColour);
    if (isMouseOverButton)
    {
        if (isButtonDown)
            bc = bc.brighter();
        else if (bc.getBrightness() > 0.5f)
            bc = bc.darker(0.1f);
        else
            bc = bc.brighter(0.1f);
    }

    if (hex)
    {
        g.setColour(bc);
        g.fillPath(hexpath);
        g.setColour(bc.contrasting().withAlpha((isMouseOverButton) ? 0.6f : 0.4f));
        g.strokePath(hexpath, juce::PathStrokeType((isMouseOverButton) ? 2.0f : 1.4f));
    }
    else
    {
        juce::Path p;
        p.addRoundedRectangle(indent, indent, width - indent * 2.0f, height - indent * 2.0f, (float) cornerSize);
        g.setColour(bc);
        g.fillPath(p);
        g.setColour(bc.contrasting().withAlpha((isMouseOverButton) ? 0.6f : 0.4f));
        g.strokePath(p, juce::PathStrokeType((isMouseOverButton) ? 2.0f : 1.4f));
    }
}

void MidiPad::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    juce::Rectangle<float> imageSpace;
    const float insetX = getWidth() * (1.f - imageSize) * 0.5f;
    const float insetY = getHeight() * (1.f - imageSize) * 0.5f;
    imageSpace.setBounds(insetX, insetY, getWidth() - insetX * 2, getHeight() - insetY * 2);
    MidiPad::drawButtonBackground(g, *this, getBackgroundColour(), isMouseOverButton, isButtonDown);

    g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);
    g.setOpacity(1.0f);

    const juce::Drawable* imageToDraw = nullptr;

    if (isEnabled())
    {
        imageToDraw = getCurrentImage();
    }
    else
    {
        //imageToDraw = getToggleState() ? disabledImageOn
        //                               : disabledImage;

        //if (imageToDraw == 0)
        //{
        g.setOpacity(0.4f);
        imageToDraw = getNormalImage();
        //}
    }

    if (imageToDraw != nullptr)
    {
        imageToDraw->drawWithin(g,
                                imageSpace,
                                juce::RectanglePlacement::centred,
                                1.f);
    }

    if (showvalues)
    {
        float fontsize = jmax(0.5f, jmin((float) (proportionOfWidth(0.2f)), (float) (proportionOfHeight(0.15f))));
        g.setFont(juce::Font(fontsize * 0.9f, juce::Font::plain));
        g.setColour(getBackgroundColour().contrasting(0.8f));
        juce::String xy = juce::String();
        if (showx && showy)
            xy = "x:" + juce::String((int) (x * 127.1)) + " y:" + juce::String((int) (y * 127.1));
        else if (showx)
            xy = "x:" + juce::String((int) (x * 127.1));
        else if (showy)
            xy = "y:" + juce::String((int) (y * 127.1));
        g.drawFittedText(xy,
                         proportionOfWidth(0.0447f),
                         proportionOfHeight(0.8057f),
                         proportionOfWidth(0.9137f),
                         proportionOfHeight(0.1355f),
                         juce::Justification::centred,
                         1);
    }
    if (showdot)
    {
        x              = jlimit(0.f, 1.f, x);
        y              = jlimit(0.f, 1.f, y);
        float diameter = jlimit(10.f, jmax(10.f, jmin(getHeight(), getWidth()) * 0.4f), jmax((float) (proportionOfHeight(0.125f)), (float) (proportionOfWidth(0.125f))));
        g.setColour(juce::Colour(0x88faa52a));
        g.fillEllipse((float) (proportionOfWidth(x)) - diameter * 0.5f,
                      (float) (proportionOfHeight(1.0f - y)) - diameter * 0.5f,
                      diameter,
                      diameter);
        g.setColour(juce::Colour(0x99a52a88));
        g.drawEllipse((float) (proportionOfWidth(x)) - diameter * 0.5f,
                      (float) (proportionOfHeight(1.0f - y)) - diameter * 0.5f,
                      diameter,
                      diameter,
                      diameter * 0.1f);
    }
}

void MidiPad::setColour(const juce::Colour& colour)
{
    setBackgroundColours(colour, colour);
    text->setColour(juce::Label::textColourId, colour.contrasting(0.8f));
}

void MidiPad::setCenteredText(bool centered)
{
    text->setJustificationType(centered ? juce::Justification::centred : juce::Justification::centredTop);
}

void MidiPad::resized()
{
    if (hex)
    {
        hexpath.clear();
        hexpath.startNewSubPath((float) getWidth() * (0.0100f), (float) getHeight() * (0.5000f));
        hexpath.lineTo((float) getWidth() * (0.2500f), (float) getHeight() * (0.0100f));
        hexpath.lineTo((float) getWidth() * (0.7500f), (float) getHeight() * (0.0100f));
        hexpath.lineTo((float) getWidth() * (0.9900f), (float) getHeight() * (0.5000f));
        hexpath.lineTo((float) getWidth() * (0.7500f), (float) getHeight() * (0.9900f));
        hexpath.lineTo((float) getWidth() * (0.2500f), (float) getHeight() * (0.9900f));
        hexpath.closeSubPath();
    }
    text->setBounds(4, 4, getWidth() - 8, getHeight() - 8);
    float fontsize = jmax(5.f, jmin((float) (proportionOfWidth(0.2f)), (float) (proportionOfHeight(0.15f))));
    text->setFont(juce::Font(fontsize, juce::Font::bold));
}

void MidiPad::setHex(bool newhex)
{
    hex = newhex;
    repaint();
}

const juce::Drawable* MidiPad::getCurrentImage() const throw()
{
    //if (isDown())
    //    return getDownImage();

    //if (isOver())
    //    return getOverImage();

    return getNormalImage();
}

const juce::Drawable* MidiPad::getNormalImage() const throw()
{
    return
        //(getToggleState() && normalImageOn != 0) ? normalImageOn :
        normalImage.get();
}

bool MidiPad::isHex()
{
    return hex;
}

bool MidiPad::hitTest(int x, int y)
{
    if (isHex())
    {
        float xnorm = (float) x / (float) getWidth();
        float ynorm = 1 - (float) y / (float) getHeight();
        if (xnorm < 0.25)
        {
            if (ynorm < (0.5 - 2 * xnorm))
                return false;
            else if (ynorm > (0.5 + 2 * xnorm))
                return false;
            else
                return true;
        }
        else if (xnorm > 0.75)
        {
            if (ynorm > (2.5 - 2 * xnorm))
                return false;
            else if (ynorm < (-1.5 + 2 * xnorm))
                return false;
            else
                return true;
        }
        else
            return true;
    }
    else
        return true;
}

void MidiPad::setText(const juce::String& name)
{
    text->setText(name, juce::dontSendNotification);
}

juce::String MidiPad::getText()
{
    return text->getText();
}
