#include "piz_LookAndFeel.h"


//==============================================================================
PizLookAndFeel::PizLookAndFeel()
{
#ifdef MRALIASPRO
	setColour (ComboBox::outlineColourId,           Colour(0xff222222));
    setColour (ComboBox::backgroundColourId,        Colour(0xff87868a));
    setColour (ComboBox::textColourId,              Colours::black);
#else
    //setColour (ComboBox::outlineColourId,           Colours::black);
    setColour (ComboBox::backgroundColourId,        Colours::lightgrey);
    //setColour (ComboBox::textColourId,              Colours::white);
#endif
    setColour (ComboBox::buttonColourId,            Colours::black);
    setColour (TextButton::buttonColourId,          findColour (TextButton::buttonColourId));
    setColour (ListBox::outlineColourId,            findColour (ComboBox::outlineColourId));
    setColour (Slider::thumbColourId,               Colours::lightgrey);
    setColour (Slider::trackColourId,               Colours::black);
    setColour (Slider::textBoxOutlineColourId,      Colours::black);
    setColour (Slider::textBoxTextColourId,         Colours::white);
    setColour (PopupMenu::backgroundColourId,             Colour (0xffeeeeee));
    setColour (PopupMenu::highlightedBackgroundColourId,  Colours::black);
    setColour (PopupMenu::highlightedTextColourId,        Colours::white);
    setColour (TextEditor::focusedOutlineColourId,  findColour (TextButton::buttonColourId));
	setColour (TooltipWindow::backgroundColourId,   Colour(0xffc0c8cf));
}

PizLookAndFeel::~PizLookAndFeel()
{
}

//==============================================================================
void PizLookAndFeel::drawButtonBackground (Graphics& g,
                                                 Button& button,
                                                 const Colour& backgroundColour,
                                                 bool isMouseOverButton,
                                                 bool isButtonDown)
{
    const int width = button.getWidth();
    const int height = button.getHeight();

    const float indent = 2.0f;
    const int cornerSize = 0;

    Path p;
    p.addRoundedRectangle (indent, indent,
                           width - indent * 2.0f,
                           height - indent * 2.0f,
                           (float) cornerSize);

    Colour bc (backgroundColour);//.withMultipliedSaturation (0.3f));

    if (isMouseOverButton)
    {
        if (isButtonDown)
            bc = bc.brighter();
        else if (bc.getBrightness() > 0.5f)
            bc = bc.darker (0.1f);
        else
            bc = bc.brighter (0.1f);
    }

    g.setColour (bc);
    g.fillPath (p);

    g.setColour (bc.contrasting().withAlpha ((isMouseOverButton) ? 0.6f : 0.4f));
    g.strokePath (p, PathStrokeType ((isMouseOverButton) ? 2.0f : 1.4f));
}

void PizLookAndFeel::drawTickBox (Graphics& g,
                                        Component& /*component*/,
                                        int x, int y, int w, int h,
                                        const bool ticked,
                                        const bool isEnabled,
                                        const bool /*isMouseOverButton*/,
                                        const bool isButtonDown)
{
    Path box;
    box.addRoundedRectangle (0.0f, 2.0f, 6.0f, 6.0f, 1.0f);

    g.setColour (isEnabled ? Colours::blue.withAlpha (isButtonDown ? 0.3f : 0.1f)
                           : Colours::lightgrey.withAlpha (0.1f));

    AffineTransform trans (AffineTransform::scale (w / 9.0f, h / 9.0f)
                               .translated ((float) x, (float) y));

    g.fillPath (box, trans);

    g.setColour (Colours::black.withAlpha (0.6f));
    g.strokePath (box, PathStrokeType (0.9f), trans);

    if (ticked)
    {
        Path tick;
        tick.startNewSubPath (1.5f, 3.0f);
        tick.lineTo (3.0f, 6.0f);
        tick.lineTo (6.0f, 0.0f);

        g.setColour (isEnabled ? Colours::black : Colours::grey);
        g.strokePath (tick, PathStrokeType (2.5f), trans);
    }
}

void PizLookAndFeel::drawToggleButton (Graphics& g,
                                             ToggleButton& button,
                                             bool isMouseOverButton,
                                             bool isButtonDown)
{
    if (button.hasKeyboardFocus (true))
    {
        g.setColour (button.findColour (TextEditor::focusedOutlineColourId));
        g.drawRect (0, 0, button.getWidth(), button.getHeight());
    }

    const int tickWidth = jmin (20, button.getHeight() - 4);

    drawTickBox (g, button, 4, (button.getHeight() - tickWidth) / 2,
                 tickWidth, tickWidth,
                 button.getToggleState(),
                 button.isEnabled(),
                 isMouseOverButton,
                 isButtonDown);

    g.setColour (button.findColour (ToggleButton::textColourId));
    g.setFont (jmin (15.0f, button.getHeight() * 0.6f));

    if (! button.isEnabled())
        g.setOpacity (0.5f);

    const int textX = tickWidth + 5;

    g.drawFittedText (button.getButtonText(),
                      textX, 4,
                      button.getWidth() - textX - 2, button.getHeight() - 8,
                      Justification::centredLeft, 10);
}

//==============================================================================
void PizLookAndFeel::drawPopupMenuBackground (Graphics& g, int width, int height)
{
    g.fillAll (findColour (PopupMenu::backgroundColourId));

    g.setColour (Colours::black.withAlpha (0.6f));
    g.drawRect (0, 0, width, height);
}

const Font PizLookAndFeel::getPopupMenuFont()
{
    return Font (13.0f);
}


//==============================================================================
void PizLookAndFeel::drawMenuBarBackground (Graphics& g, int /*width*/, int /*height*/,
                                                  bool, MenuBarComponent& menuBar)
{
    g.fillAll (menuBar.findColour (PopupMenu::backgroundColourId));
}


//==============================================================================
void PizLookAndFeel::drawTextEditorOutline (Graphics& g, int width, int height, TextEditor& textEditor)
{
    if (textEditor.isEnabled())
    {
        g.setColour (textEditor.findColour (TextEditor::outlineColourId));
        g.drawRect (0, 0, width, height);
    }
}

//==============================================================================
void PizLookAndFeel::drawComboBox (Graphics& g, int width, int height,
                                         const bool isButtonDown,
                                         int buttonX, int buttonY,
                                         int buttonW, int buttonH,
                                         ComboBox& box)
{
    g.fillAll (box.findColour (ComboBox::backgroundColourId));

    g.setColour (box.findColour ((isButtonDown) ? ComboBox::buttonColourId
                                                : ComboBox::backgroundColourId));
    g.fillRect (buttonX, buttonY, buttonW, buttonH);

    g.setColour (box.findColour (ComboBox::outlineColourId));
    g.drawRect (0, 0, width, height, 1);
    //g.drawRect (0, 0, width-1, height-1, 1);

    const float arrowX = 0.2f;
    const float arrowH = 0.3f;

    if (box.isEnabled())
    {
        Path p;
        p.addTriangle (buttonX + buttonW * 0.5f,            buttonY + buttonH * (0.55f + arrowH),
                       buttonX + buttonW * (1.0f - arrowX), buttonY + buttonH * 0.55f,
                       buttonX + buttonW * arrowX,          buttonY + buttonH * 0.55f);

        g.setColour (box.findColour ((isButtonDown) ? ComboBox::backgroundColourId
                                                    : ComboBox::buttonColourId));
        g.fillPath (p);
    }
}

const Font PizLookAndFeel::getComboBoxFont (ComboBox& box)
{
    Font f (jmin (15.0f, box.getHeight() * 0.85f));
    f.setHorizontalScale (0.9f);
    return f;
}

//==============================================================================
static void drawTriangle (Graphics& g, float x1, float y1, float x2, float y2, float x3, float y3, const Colour& fill, const Colour& outline) throw()
{
    Path p;
    p.addTriangle (x1, y1, x2, y2, x3, y3);
    g.setColour (fill);
    g.fillPath (p);

    g.setColour (outline);
    g.strokePath (p, PathStrokeType (0.3f));
}

void PizLookAndFeel::drawLinearSlider (Graphics& g,
                                             int x, int y,
                                             int w, int h,
                                             float sliderPos,
                                             float minSliderPos,
                                             float maxSliderPos,
                                             const Slider::SliderStyle style,
                                             Slider& slider)
{
    g.fillAll (slider.findColour (Slider::backgroundColourId));

    if (style == Slider::LinearBar)
    {
        g.setColour (slider.findColour (Slider::thumbColourId));
        g.fillRect (x, y, (int) sliderPos - x, h);

        g.setColour (slider.findColour (Slider::textBoxTextColourId).withMultipliedAlpha (0.5f));
        g.drawRect (x, y, (int) sliderPos - x, h);
    }
    else
    {
        g.setColour (slider.findColour (Slider::trackColourId)
                           .withMultipliedAlpha (slider.isEnabled() ? 1.0f : 0.3f));

        if (slider.isHorizontal())
        {
            int trackWidth = roundFloatToInt (h * 0.2f);
            g.fillRect (x, y + roundFloatToInt (h * 0.4f),
                        w, trackWidth);
        }
        else
        {
            int trackWidth = /*jmin (4, */roundFloatToInt (w * 0.15f)/*)*/;
            g.fillRect (x + roundFloatToInt(w*0.5f - (float)trackWidth), y,
                        trackWidth, h);
            //g.fillRect (x + roundFloatToInt (w * 0.5f - jmin (3.0f, w * 0.1f)), y,
            //            trackWidth, h);
        }

        float alpha = 0.35f;

        if (slider.isEnabled())
            alpha = slider.isMouseOverOrDragging() ? 1.0f : 0.7f;

        const Colour fill (slider.findColour (Slider::thumbColourId).withAlpha (alpha));
        const Colour outline (Colours::black.withAlpha (slider.isEnabled() ? 0.7f : 0.35f));

        if (style == Slider::TwoValueVertical || style == Slider::ThreeValueVertical)
        {
            drawTriangle (g, x + w * 0.5f + jmin (4.0f, w * 0.3f), minSliderPos,
                          x + w * 0.5f - jmin (8.0f, w * 0.4f), minSliderPos - 7.0f,
                          x + w * 0.5f - jmin (8.0f, w * 0.4f), minSliderPos,
                          fill, outline);

            drawTriangle (g, x + w * 0.5f + jmin (4.0f, w * 0.3f), maxSliderPos,
                          x + w * 0.5f - jmin (8.0f, w * 0.4f), maxSliderPos,
                          x + w * 0.5f - jmin (8.0f, w * 0.4f), maxSliderPos + 7.0f,
                          fill, outline);
        }
        else if (style == Slider::TwoValueHorizontal || style == Slider::ThreeValueHorizontal)
        {
            drawTriangle (g, minSliderPos, y + h * 0.6f - jmin (4.0f, h * 0.3f),
                          minSliderPos - 7.0f, y + h * 0.9f ,
                          minSliderPos, y + h * 0.9f,
                          fill, outline);

            drawTriangle (g, maxSliderPos, y + h * 0.6f - jmin (4.0f, h * 0.3f),
                          maxSliderPos, y + h * 0.9f,
                          maxSliderPos + 7.0f, y + h * 0.9f,
                          fill, outline);
        }

        if (style == Slider::LinearHorizontal || style == Slider::ThreeValueHorizontal)
        {
            drawTriangle (g, sliderPos, y + h * 0.9f,
                          sliderPos - 7.0f, y + h * 0.2f,
                          sliderPos + 7.0f, y + h * 0.2f,
                          fill, outline);

            g.setColour (slider.findColour(Slider::textBoxTextColourId).withMultipliedAlpha(0.6f));
            g.setFont ((float)h,Font::bold);
            double val = slider.getValue();
            if (fabs(val)<0.0000001) val=0.;
            g.drawText (String(val),
                        0, 0, w, h,
                        Justification::centred, true);
        }
        else if (style == Slider::LinearVertical || style == Slider::ThreeValueVertical)
        {
            //drawTriangle (g, x + w * 0.5f - jmin (4.0f, w * 0.3f), sliderPos,
            //              x + w * 0.5f + jmin (8.0f, w * 0.4f), sliderPos - 7.0f,
            //              x + w * 0.5f + jmin (8.0f, w * 0.4f), sliderPos + 7.0f,
            //              fill, outline);
            drawTriangle (g, x + w*0.3f, sliderPos,
                          x + w*0.8f, sliderPos - 7.0f,
                          x + w*0.8f, sliderPos + 7.0f,
                          fill, outline);
            g.setColour (slider.findColour(Slider::textBoxTextColourId).withMultipliedAlpha(0.6f));
            g.setFont ((float)w*0.5f,Font::bold);
            String val=String(slider.getValue(),1);
            if (val==String("100.0")) val="100";
            g.drawFittedText (val,
                        0, 0, w, h,
                        Justification::centred, 1);
        }
    }
}

void PizLookAndFeel::drawRotarySlider (Graphics& g,
                                    int x, int y,
                                    int width, int height,
                                    float sliderPos,
                                    const float rotaryStartAngle,
                                    const float rotaryEndAngle,
                                    Slider& slider)
{
    g.fillAll (slider.findColour (Slider::backgroundColourId));

    const float radius = jmin (width / 2, height / 2) - 2.0f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    //const float rx = centreX - radius;
    //const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
    if (slider.isEnabled())
        g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId).withMultipliedAlpha (isMouseOver ? 1.0f : 0.7f));
    else
        g.setColour (Colour (0x80808080));


    Path p;
    p.addEllipse (-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
    PathStrokeType (rw * 0.1f).createStrokedPath (p, p);
    g.fillPath (p, AffineTransform::rotation (angle).translated (centreX, centreY));

    Path m;
    m.addLineSegment (Line<float>(0.0f, 0.0f, 0.0f, -radius), rw * 0.2f);
    g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withMultipliedAlpha (isMouseOver ? 1.0f : 0.8f));
    g.fillPath (m, AffineTransform::rotation (angle).translated (centreX, centreY));

    g.setColour (slider.findColour (Slider::textBoxTextColourId).withMultipliedAlpha (0.5f));
	Font font("small fonts",slider.getHeight()*0.4f,Font::bold);
    g.setFont(font);
    g.drawFittedText(String(roundDoubleToInt(slider.getValue())),0,0,width,height,Justification::centred,1);
}


Button* PizLookAndFeel::createSliderButton (const bool isIncrement)
{
    if (isIncrement)
        return new ArrowButton ("u", 0.75f, Colours::white.withAlpha (0.8f));
    else
        return new ArrowButton ("d", 0.25f, Colours::white.withAlpha (0.8f));
}

int PizLookAndFeel::getSliderThumbRadius (Slider&)
{
    return 8;
}

//==============================================================================
void PizLookAndFeel::drawCornerResizer (Graphics& g,
                                              int w, int h,
                                              bool isMouseOver,
                                              bool isMouseDragging)
{
    g.setColour ((isMouseOver || isMouseDragging) ? Colours::lightgrey
                                                  : Colours::darkgrey);

    const float lineThickness = jmin (w, h) * 0.1f;

    for (float i = 0.0f; i < 1.0f; i += 0.3f)
    {
        g.drawLine (w * i,
                    h + 1.0f,
                    w + 1.0f,
                    h * i,
                    lineThickness);
    }
}

static const TextLayout layoutTooltipText (const String& text) throw()
{
    const float tooltipFontSize = 11.0f;
    const int maxToolTipWidth = 400;

    const Font f (tooltipFontSize, Font::plain);
    TextLayout tl (text, f);
    tl.layout (maxToolTipWidth, Justification::left, true);

    return tl;
}

void PizLookAndFeel::getTooltipSize (const String& tipText, int& width, int& height)
{
    const TextLayout tl (layoutTooltipText (tipText));

    width = tl.getWidth() + 14;
    height = tl.getHeight() + 10;
}

void PizLookAndFeel::drawTooltip (Graphics& g, const String& text, int width, int height)
{
    g.fillAll (findColour (TooltipWindow::backgroundColourId));

    const Colour textCol (findColour (TooltipWindow::textColourId));

    g.setColour (findColour (TooltipWindow::outlineColourId));
    g.drawRect (0, 0, width, height);

    const TextLayout tl (layoutTooltipText (text));

    g.setColour (findColour (TooltipWindow::textColourId));
    tl.drawWithin (g, 0, 0, width, height, Justification::centred);
}

