#ifndef __PIZLOOKANDFEEL__
#define __PIZLOOKANDFEEL__

#include "JuceHeader.h"

//==============================================================================
class PizLookAndFeel    : public LookAndFeel
{
public:
    //==============================================================================
    /** Creates the default JUCE look and feel. */
    PizLookAndFeel();

    /** Destructor. */
    virtual ~PizLookAndFeel();

    //==============================================================================
    /** Draws the lozenge-shaped background for a standard button. */
    virtual void drawButtonBackground (Graphics& g,
                                       Button& button,
                                       const Colour& backgroundColour,
                                       bool isMouseOverButton,
                                       bool isButtonDown);


    /** Draws the contents of a standard ToggleButton. */
    virtual void drawToggleButton (Graphics& g,
                                   ToggleButton& button,
                                   bool isMouseOverButton,
                                   bool isButtonDown);

    virtual void drawTickBox (Graphics& g,
                              Component& component,
                              int x, int y, int w, int h,
                              const bool ticked,
                              const bool isEnabled,
                              const bool isMouseOverButton,
                              const bool isButtonDown);

    //==============================================================================
    virtual void drawTextEditorOutline (Graphics& g,
                                        int width, int height,
                                        TextEditor& textEditor);

    //==============================================================================
    /** Fills the background of a popup menu component. */
    virtual void drawPopupMenuBackground (Graphics& g, int width, int height);
    virtual const Font getPopupMenuFont();

    virtual void drawMenuBarBackground (Graphics& g, int width, int height,
                                        bool isMouseOverBar,
                                        MenuBarComponent& menuBar);

    //==============================================================================
    virtual void drawComboBox (Graphics& g, int width, int height,
                               bool isButtonDown,
                               int buttonX, int buttonY,
                               int buttonW, int buttonH,
                               ComboBox& box);

	virtual const Font getComboBoxFont (ComboBox& box);

    //==============================================================================
    virtual void drawLinearSlider (Graphics& g,
                                   int x, int y,
                                   int width, int height,
                                   float sliderPos,
                                   float minSliderPos,
                                   float maxSliderPos,
                                   const Slider::SliderStyle style,
                                   Slider& slider);

    virtual int getSliderThumbRadius (Slider& slider);

    virtual void drawRotarySlider (Graphics& g,
                                   int x, int y,
                                   int width, int height,
                                   float sliderPosProportional,
                                   float rotaryStartAngle,
                                   float rotaryEndAngle,
                                   Slider& slider);

    virtual Button* createSliderButton (bool isIncrement);

    //==============================================================================
    virtual void getTooltipSize (const String& tipText, int& width, int& height);

    virtual void drawTooltip (Graphics& g, const String& text, int width, int height);

    //==============================================================================
    virtual void drawCornerResizer (Graphics& g,
                                    int w, int h,
                                    bool isMouseOver,
                                    bool isMouseDragging);

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    PizLookAndFeel (const PizLookAndFeel&);
    const PizLookAndFeel& operator= (const PizLookAndFeel&);
};


#endif   // __JUCE_PIZLOOKANDFEEL_JUCEHEADER__
