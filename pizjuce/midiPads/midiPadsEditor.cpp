/*==============================================================================
midiPads
==============================================================================*/
#include "midiPadsEditor.h"

//==============================================================================
midiPadsEditor::midiPadsEditor (midiPads* const ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      lastTouchedPad (0)
{
    setMouseClickGrabsKeyboardFocus (false);

    addAndMakeVisible (container = new fullScreenContainer());
    container->addMouseListener (this, false);
    fullscreen = false;

    container->addAndMakeVisible (padEditor = new Component());

    bool useaft  = ownerFilter->getParameter (kSendAft) >= 0.5;
    dontsend     = false;
    automateHost = false;

    bool usex[numPads];
    bool usey[numPads];
    for (int i = 0; i < numPads; i++)
    {
        //toggled[i] = false;
        showdots[i]   = true;
        showvalues[i] = true;
        usex[i]       = getFilter()->UseX[i];
        usey[i]       = getFilter()->UseY[i];
        container->addAndMakeVisible (midiPad[i] = new MidiPad (i));
        midiPad[i]->setButtonText (String());
        midiPad[i]->setTriggeredOnMouseDown (false);
        midiPad[i]->addListener (this);
        midiPad[i]->addMouseListener (this, true);
        midiPad[i]->setToggleState (getFilter()->togglestate[i], dontSendNotification);
        sending[i] = false;

        midiPad[i]->showdot = getFilter()->toggle[i];
        if (! usex[i])
        {
            midiPad[i]->setXFloat (0.5f);
            midiPad[i]->showx = false;
        }
        else
        {
            midiPad[i]->setXFloat (getFilter()->getParameter (i + xpos));
            midiPad[i]->showx = true;
        }
        if (getFilter()->SendOff[i])
        {
            midiPad[i]->setYInt (getFilter()->Ydata2[i]);
        }
        else
            midiPad[i]->setYFloat (getFilter()->getParameter (i + ypos));

        midiPad[i]->setHex (getFilter()->hex);
        lastx[i] = 0;
        lasty[i] = 0;
    }
    padEditor->addAndMakeVisible (vSlider = new Slider ("new slider"));
    vSlider->setRange (0, 127, 1);
    vSlider->setSliderStyle (Slider::LinearBar);
    vSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    vSlider->addListener (this);

    padEditor->addAndMakeVisible (nSlider = new Slider ("note slider"));
    nSlider->setRange (0, 127, 1);
    nSlider->setSliderStyle (Slider::LinearBar);
    nSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    nSlider->addListener (this);

    padEditor->addAndMakeVisible (ySlider = new Slider ("y-cc slider"));
    ySlider->setRange (0, 127, 1);
    ySlider->setSliderStyle (Slider::LinearBar);
    ySlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    ySlider->addListener (this);

    padEditor->addAndMakeVisible (oSlider = new Slider ("new slider"));
    oSlider->setRange (0, 127, 1);
    oSlider->setSliderStyle (Slider::LinearBar);
    oSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    oSlider->addListener (this);

    padEditor->addAndMakeVisible (triggerSlider = new Slider ("new slider"));
    triggerSlider->setRange (0, 127, 1);
    triggerSlider->setSliderStyle (Slider::LinearBar);
    triggerSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    triggerSlider->addListener (this);

    padEditor->addAndMakeVisible (xSlider = new Slider ("new slider"));
    xSlider->setRange (0, 127, 1);
    xSlider->setSliderStyle (Slider::LinearBar);
    xSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    xSlider->addListener (this);

    padEditor->addAndMakeVisible (xoSlider = new Slider ("new slider"));
    xoSlider->setRange (0, 127, 1);
    xoSlider->setSliderStyle (Slider::LinearBar);
    xoSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    xoSlider->addListener (this);

    padEditor->addAndMakeVisible (roundnessSlider = new Slider ("Pad Roundness"));
    roundnessSlider->setRange (0, 0.5, 0);
    roundnessSlider->setSliderStyle (Slider::LinearBar);
    roundnessSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    roundnessSlider->setTooltip ("Pad Roundness");
    roundnessSlider->setMouseClickGrabsKeyboardFocus (false);
    roundnessSlider->addListener (this);

    padEditor->addAndMakeVisible (textEditor = new TextEditor ("text editor"));
    textEditor->setMultiLine (true);
    textEditor->setReturnKeyStartsNewLine (true);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (false);
    textEditor->setSelectAllWhenFocused (true);
    textEditor->addListener (this);
    textEditor->setColour (TextEditor::outlineColourId, Colours::black);

    padEditor->addAndMakeVisible (colourSelector1 = new ColourSelector (ColourSelector::showColourAtTop | ColourSelector::showSliders | ColourSelector::showColourspace));
    colourSelector1->setName ("Pad Color");
    colourSelector1->addChangeListener (this);

    padEditor->addAndMakeVisible (iconSizeSlider = new Slider ("Icon Size"));
    iconSizeSlider->setRange (0.1, 1.0, 0);
    iconSizeSlider->setSliderStyle (Slider::LinearBar);
    iconSizeSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    iconSizeSlider->setTooltip ("Icon Size");
    iconSizeSlider->setMouseClickGrabsKeyboardFocus (false);
    iconSizeSlider->addListener (this);

    container->addAndMakeVisible (hueSlider = new Slider ("Hue"));
    hueSlider->setTooltip ("Hue");
    hueSlider->setRange (0, 1, 0);
    hueSlider->setSliderStyle (Slider::LinearBar);
    hueSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    hueSlider->addListener (this);
    hueSlider->setMouseClickGrabsKeyboardFocus (false);

    container->addAndMakeVisible (saturationSlider = new Slider ("Saturation"));
    saturationSlider->setTooltip ("Saturation");
    saturationSlider->setRange (0, 1, 0);
    saturationSlider->setSliderStyle (Slider::LinearBar);
    saturationSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    saturationSlider->addListener (this);
    saturationSlider->setMouseClickGrabsKeyboardFocus (false);

    container->addAndMakeVisible (brigtnessSlider = new Slider ("Brightness"));
    brigtnessSlider->setTooltip ("Brightness");
    brigtnessSlider->setRange (0, 1, 0);
    brigtnessSlider->setSliderStyle (Slider::LinearBar);
    brigtnessSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    brigtnessSlider->addListener (this);
    brigtnessSlider->setMouseClickGrabsKeyboardFocus (false);

    container->addAndMakeVisible (padOpacitySlider = new Slider ("Pad Opacity"));
    padOpacitySlider->setTooltip ("Pad Opacity");
    padOpacitySlider->setRange (0, 1, 0);
    padOpacitySlider->setSliderStyle (Slider::LinearBar);
    padOpacitySlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    padOpacitySlider->addListener (this);
    padOpacitySlider->setMouseClickGrabsKeyboardFocus (false);

    container->addAndMakeVisible (globalRoundnessSlider = new Slider ("Pad Roundness"));
    globalRoundnessSlider->setTooltip ("Pad Roundness");
    globalRoundnessSlider->setRange (0, 0.5, 0);
    globalRoundnessSlider->setSliderStyle (Slider::LinearBar);
    globalRoundnessSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    globalRoundnessSlider->setMouseClickGrabsKeyboardFocus (false);
    globalRoundnessSlider->addListener (this);

    container->addAndMakeVisible (globalIconSizeSlider = new Slider ("Icon Size"));
    globalIconSizeSlider->setTooltip ("Icon Size");
    globalIconSizeSlider->setRange (0.1, 1.0, 0);
    globalIconSizeSlider->setSliderStyle (Slider::LinearBar);
    globalIconSizeSlider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    globalIconSizeSlider->setMouseClickGrabsKeyboardFocus (false);
    globalIconSizeSlider->addListener (this);

    container->addAndMakeVisible (velocitySlider = new Slider ("Velocity Scale"));
    velocitySlider->setTooltip ("Velocity Scale Factor");
    velocitySlider->setRange (0, 2, 0.01);
    velocitySlider->setSliderStyle (Slider::LinearBar);
    velocitySlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    velocitySlider->setMouseClickGrabsKeyboardFocus (false);
    velocitySlider->addListener (this);

    container->addAndMakeVisible (valueSlider = new Slider ("CC Value Scale"));
    valueSlider->setTooltip ("CC Value Scale Factor");
    valueSlider->setRange (0, 2, 0.01);
    valueSlider->setSliderStyle (Slider::LinearBar);
    valueSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    valueSlider->setMouseClickGrabsKeyboardFocus (false);
    valueSlider->addListener (this);

    container->addAndMakeVisible (menuButton = new TextButton ("Menu Button"));
    menuButton->setButtonText ("menu");
    menuButton->addListener (this);
    menuButton->setTriggeredOnMouseDown (true);
    menuButton->setMouseClickGrabsKeyboardFocus (false);

    container->addAndMakeVisible (icSlider = new Slider ("new slider"));
    icSlider->setRange (1, 16, 1);
    icSlider->setSliderStyle (Slider::LinearBar);
    icSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    icSlider->setMouseClickGrabsKeyboardFocus (false);
    icSlider->addListener (this);

    container->addAndMakeVisible (cSlider = new Slider ("new slider"));
    cSlider->setRange (1, 16, 1);
    cSlider->setSliderStyle (Slider::LinearBar);
    cSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    cSlider->setMouseClickGrabsKeyboardFocus (false);
    cSlider->addListener (this);

    squares = 16;

    // add the triangular resizer component for the bottom-right of the UI
    container->addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));
    resizer->setMouseClickGrabsKeyboardFocus (false);
    resizeLimits.setSizeLimits (50, 50, 1600, 1600);

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize (ownerFilter->lastUIWidth,
             ownerFilter->lastUIHeight);
    resized();

    ownerFilter->addChangeListener (this);
    updateParametersFromFilter();
}

midiPadsEditor::~midiPadsEditor()
{
    getFilter()->removeChangeListener (this);
    if (container->isOnDesktop())
        container->removeFromDesktop();
    for (int i = 0; i < numPads; i++)
    {
        getFilter()->icon[i]  = midiPad[i]->getIconPath();
        getFilter()->lastx[i] = lastx[i];
        getFilter()->lasty[i] = lasty[i];
        delete midiPad[i];
    }
    delete colourSelector1;
    delete vSlider;
    delete nSlider;
    delete ySlider;
    delete oSlider;
    delete triggerSlider;
    delete xSlider;
    delete xoSlider;
    delete roundnessSlider;
    delete textEditor;
    delete iconSizeSlider;
    delete cSlider;
    delete icSlider;
    delete hueSlider;
    delete saturationSlider;
    delete brigtnessSlider;
    delete padOpacitySlider;
    delete globalRoundnessSlider;
    delete globalIconSizeSlider;
    delete velocitySlider;
    delete valueSlider;
    delete menuButton;
    delete resizer;
    delete container;
    delete padEditor;
}

//==============================================================================
void midiPadsEditor::paint (Graphics& g)
{
    g.fillAll (color);
    for (int i = 0; i < numPads; i++)
        midiPad[i]->setColour (getFilter()->padcolor[i].withAlpha (getFilter()->contrast));
}

void midiPadsEditor::resized()
{
    getFilter()->lastUIWidth  = getWidth();
    getFilter()->lastUIHeight = getHeight();

    for (int i = 0; i < numPads; i++)
    {
        midiPad[i]->setHex (getFilter()->hex);
        midiPad[i]->setVisible (getFilter()->isPadVisible (i));
        midiPad[i]->setBounds (getFilter()->getPadBounds (i));
    }

    if (! fullscreen)
    {
        container->setBounds (0, 0, proportionOfWidth (1.0f), proportionOfHeight (1.0f));
        int resizersize = jmin (container->proportionOfWidth (0.05f), container->proportionOfHeight (0.05f));
        if (resizersize < 12)
            resizersize = 12;
        resizer->setBounds (container->getWidth() - resizersize, container->getHeight() - resizersize, resizersize, resizersize);
        resizer->setVisible (true);
    }
    else
    {
        float xscale  = (float) container->getWidth() / (float) getWidth();
        float yscale  = (float) container->getHeight() / (float) getHeight();
        auto* display = Desktop::getInstance().getDisplays().getDisplayForPoint (container->getScreenPosition(), false);
        if (display != nullptr)
        {
            container->setBounds (display->userArea);
        }
        for (int i = 0; i < numPads; i++)
        {
            midiPad[i]->setBounds (roundToInt ((float) midiPad[i]->getPosition().getX() * xscale),
                                   roundToInt ((float) midiPad[i]->getPosition().getY() * yscale),
                                   roundToInt ((float) midiPad[i]->getWidth() * xscale),
                                   roundToInt ((float) midiPad[i]->getHeight() * yscale));
        }
        resizer->setVisible (false);
    }
    hueSlider->setBounds (container->proportionOfWidth (0.0275f), container->proportionOfHeight (0.0179f), container->proportionOfWidth (0.1100f), container->proportionOfHeight (0.0358f));
    saturationSlider->setBounds (container->proportionOfWidth (0.1512f), container->proportionOfHeight (0.0179f), container->proportionOfWidth (0.1100f), container->proportionOfHeight (0.0358f));
    brigtnessSlider->setBounds (container->proportionOfWidth (0.2749f), container->proportionOfHeight (0.0179f), container->proportionOfWidth (0.1100f), container->proportionOfHeight (0.0358f));
    padOpacitySlider->setBounds (container->proportionOfWidth (0.3986f), container->proportionOfHeight (0.0179f), container->proportionOfWidth (0.1100f), container->proportionOfHeight (0.0358f));
    velocitySlider->setBounds (container->proportionOfWidth (0.5498f), container->proportionOfHeight (0.0179f), container->proportionOfWidth (0.1649f), container->proportionOfHeight (0.0358f));
    valueSlider->setBounds (container->proportionOfWidth (0.7285f), container->proportionOfHeight (0.0179f), container->proportionOfWidth (0.1649f), container->proportionOfHeight (0.0358f));
    menuButton->setBounds (container->proportionOfWidth (0.8935f), container->proportionOfHeight (0.0179f), container->proportionOfWidth (0.0962f), jmax (12, container->proportionOfHeight (0.0358f)));
}

//-------------------------------------------------------------------------------
void midiPadsEditor::mouseDrag (const MouseEvent& e)
{
    if (e.eventComponent->getName().compare ("MidiPad"))
        return;
    if (e.mods.isAltDown() && ! e.mods.isCommandDown() && getFilter()->editmode && ! fullscreen)
    {
        if (e.mods.isShiftDown())
        {
            dragging = false;
            if (! resizing)
            {
                resizing = ! e.mouseWasClicked();

                if (resizing)
                {
                    dragstart = e.getEventRelativeTo (container).getMouseDownPosition() - e.getMouseDownPosition();
                }
            }
        }
        else
        {
            if (! dragging)
            {
                dragging = ! e.mouseWasClicked();

                if (dragging)
                {
                    dragstart = e.getEventRelativeTo (container).getMouseDownPosition() - e.getMouseDownPosition();
                }
            }
        }

        if (dragging)
        {
            e.eventComponent->setTopLeftPosition (
                dragstart.getX() + e.getEventRelativeTo (container).getDistanceFromDragStartX(),
                dragstart.getY() + e.getEventRelativeTo (container).getDistanceFromDragStartY());
        }
        else if (resizing)
        {
            e.eventComponent->setSize (
                jmax (20, e.getEventRelativeTo (container).x - e.eventComponent->getX()),
                jmax (20, e.getEventRelativeTo (container).y - e.eventComponent->getY()));
        }
    }
    else
    {
        for (int i = 0; i < numPads; i++)
        {
            if (midiPad[i]->isMouseButtonDown())
            {
                bool usex                = getFilter()->UseX[i];
                bool usey                = getFilter()->UseY[i];
                bool useaft              = getFilter()->getParameter (kSendAft) >= 0.5;
                ModifierKeys mousebutton = ModifierKeys::getCurrentModifiers();
                //this->setWantsKeyboardFocus(false);
                if (mousebutton.isMiddleButtonDown() || mousebutton.isCommandDown())
                {
                    //ignore
                }
                else if (mousebutton.isPopupMenu())
                {
                    //ignore
                }
                else
                {
                    if (usex || usey || useaft)
                    {
                        Point<int> xy = midiPad[i]->getMouseXYRelative();
                        //measure y from the bottom, make into range 0..1
                        float fy = 1.0f - (((float) (xy.getY())) / ((float) (midiPad[i]->getHeight())));
                        float fx = (((float) (xy.getX())) / ((float) (midiPad[i]->getWidth())));
                        int newx = (int) (fx * 127.1);
                        if (newx > 127)
                        {
                            newx = 127;
                            fx   = 1.0f;
                        }
                        else if (newx < 0)
                        {
                            newx = 0;
                            fx   = 0.0f;
                        }
                        if (getFilter()->UseXPB[i])
                        {
                            newx = roundToInt (fx * 16383.0f);
                            if (newx > 0x3fff)
                            {
                                newx = 0x3fff;
                                fx   = 1.0f;
                            }
                            else if (newx < 0)
                            {
                                newx = 0;
                                fx   = 0.0f;
                            }
                        }
                        int newy = (int) (fy * 127.1);
                        if (newy > 127)
                        {
                            newy = 127;
                            fy   = 1.0f;
                        }
                        else if (newy < 0)
                        {
                            newy = 0;
                            fy   = 0.0f;
                        }
                        //move the dot
                        if (usex)
                        {
                            midiPad[i]->setXFloat (fx);
                            if (automateHost)
                                getFilter()->setParameterNotifyingHost (i + xpos, fx);
                            else
                                getFilter()->setParameter (i + xpos, fx);
                        }
                        else
                        {
                            midiPad[i]->setXFloat (0.5f);
                        }
                        if (getFilter()->Ytype[i] == 0)
                        {
                            if (useaft || getFilter()->UseYCC[i])
                            {
                                midiPad[i]->setYFloat (fy);
                                if (automateHost)
                                    getFilter()->setParameterNotifyingHost (i + ypos, fy);
                                else
                                    getFilter()->setParameter (i + ypos, fy);
                            }
                        }
                        else
                        {
                            if (usey)
                            {
                                midiPad[i]->setYFloat (fy);
                                if (automateHost)
                                    getFilter()->setParameterNotifyingHost (i + ypos, fy);
                                else
                                    getFilter()->setParameter (i + ypos, fy);
                            }
                            else if (usex)
                            {
                                midiPad[i]->setYFloat (0.5f);
                                //if (automateHost) getFilter()->setParameterNotifyingHost(i+ypos,fy);
                                //else getFilter()->setParameter(i+ypos,fy);
                            }
                        }
                        if (showdots[i] || showvalues[i])
                            midiPad[i]->repaint();
                        if (usex && lastx[i] != newx)
                        {
                            getFilter()->buttondown[i] = true;
                            lastx[i]                   = newx;
                        }
                        if ((getFilter()->Ytype[i]) == 1 && usey && lasty[i] != newy)
                        {
                            getFilter()->send[i] = 1.0f;
                            lasty[i]             = newy;
                        }
                        else if ((getFilter()->Ytype[i]) == 0 && useaft && lasty[i] != newy)
                        {
                            getFilter()->buttondown[i] = true;
                            lasty[i]                   = newy;
                        }
                        else if (usey && (getFilter()->Ytype[i]) == 0 && getFilter()->UseYCC[i] && lasty[i] != newy)
                        {
                            getFilter()->buttondown[i] = true;
                            lasty[i]                   = newy;
                        }
                    }
                }
            }
        }
    }
}

void midiPadsEditor::mouseDown (const MouseEvent& e)
{
    dragging = false;
    resizing = false;
}

void midiPadsEditor::mouseDoubleClick (const MouseEvent& e)
{
    if (e.eventComponent == container && ! fullscreen && getFilter()->editmode)
    {
        if (squares < numPads)
        {
            for (int i = 0; i < numPads; i++)
            {
                if (! getFilter()->isPadVisible (i))
                {
                    if (midiPad[lastTouchedPad]->isVisible())
                    {
                        midiPad[i]->setSize (midiPad[lastTouchedPad]->getWidth(),
                                             midiPad[lastTouchedPad]->getHeight());
                        getFilter()->copyPadSettings (lastTouchedPad, i);
                    }
                    if (midiPad[i]->getWidth() < 1 || midiPad[i]->getHeight() < 1)
                        midiPad[i]->setBounds (e.x, e.y, getWidth() / 8, getHeight() / 8);
                    midiPad[i]->setVisible (true);
                    midiPad[i]->setTopLeftPosition (
                        e.x - midiPad[i]->getWidth() / 2,
                        e.y - midiPad[i]->getHeight() / 2);
                    getFilter()->setPadPosition (i,
                                                 (float) midiPad[i]->getPosition().getX() / (float) getWidth(),
                                                 (float) midiPad[i]->getPosition().getY() / (float) getHeight());
                    getFilter()->setPadSize (i,
                                             midiPad[i]->getWidth() / (float) getWidth(),
                                             midiPad[i]->getHeight() / (float) getHeight());
                    getFilter()->squares = ++squares;
                    getFilter()->setPadVisible (i, true);
                    updateParametersFromFilter();
                    return;
                }
            }
        }
    }
}

void midiPadsEditor::mouseUp (const MouseEvent& e)
{
    if (e.eventComponent->getName().compare ("MidiPad"))
        return;
    if (dragging)
    {
        dragging = false;
        getFilter()->setPadPosition (((MidiPad*) e.eventComponent)->getIndex(),
                                     e.eventComponent->getX() / (float) getWidth(),
                                     e.eventComponent->getY() / (float) getHeight());
    }
    else if (resizing)
    {
        resizing = false;
        getFilter()->setPadSize (((MidiPad*) e.eventComponent)->getIndex(),
                                 (float) e.eventComponent->getWidth() / (float) getWidth(),
                                 (float) e.eventComponent->getHeight() / (float) getHeight());
    }
    else if (e.mods.isRightButtonDown() && e.mods.isAltDown() && ! e.mods.isCommandDown()
             && getFilter()->editmode && ! fullscreen)
    {
        MidiPad* pad = ((MidiPad*) e.eventComponent);
        int i        = pad->getIndex();
        if (pad == midiPad[i] && squares > 1)
        {
            pad->setVisible (false);
            getFilter()->squares = --squares;
            getFilter()->setPadVisible (i, false);
        }
    }
    else
    {
        //for mouseup outside the original button
        if (getFilter()->usemouseup && ! dontsend)
        {
            for (int i = 0; i < numPads; i++)
            {
                if (midiPad[i]->isPlaying && ! getFilter()->toggle[i])
                {
                    sendMidiOff (i);
                }
            }
        }
        dontsend = false;
    }
}

void midiPadsEditor::buttonStateChanged (Button* buttonThatWasClicked) //mousedown
{
    if (buttonThatWasClicked->getName().compare ("MidiPad"))
        return;
    MidiPad* pad = (MidiPad*) buttonThatWasClicked;
    if (pad->isDown())
    {
        int i       = pad->getIndex();
        bool usex   = getFilter()->UseX[i];
        bool usey   = getFilter()->UseY[i];
        bool useycc = getFilter()->UseYCC[i];
        bool useoff = getFilter()->SendOff[i];
        bool usexpb = getFilter()->UseXPB[i];
        bool toggle = getFilter()->toggle[i];
        //middle/ctrl click midi learn
        ModifierKeys mousebutton = ModifierKeys::getCurrentModifiers();
        if (mousebutton.isMiddleButtonDown() || mousebutton.isCommandDown())
        {
            if (mousebutton.isAltDown() && mousebutton.isShiftDown())
                getFilter()->midilisten[i] = 0.5f; //learn x trigger (not implemented!)
            else if (mousebutton.isShiftDown())
                getFilter()->midilisten[i] = 0.6f; //learn y trigger
            else if (mousebutton.isAltDown())
                getFilter()->midilisten[i] = 0.8f; //learn x
            else
                getFilter()->midilisten[i] = 1.0f;
        }
        //pad menu
        else if (mousebutton.isPopupMenu() && ! mousebutton.isAltDown())
        {
            if (! getFilter()->editmode)
                dontsend = true;
            else
            {
                lastPadMenu = i;
                bool iscc   = getFilter()->Ytype[i] == 1;
                colourSelector1->setCurrentColour (getFilter()->padcolor[i]);
                textEditor->setText (getFilter()->text[i], false);
                vSlider->setValue (getFilter()->Ydata2[i], dontSendNotification);
                oSlider->setValue (getFilter()->Yoff[i], dontSendNotification);
                nSlider->setValue (getFilter()->Ydata1[i], dontSendNotification);
                ySlider->setValue (getFilter()->Ycc[i], dontSendNotification);
                xSlider->setValue (getFilter()->Xcc[i], dontSendNotification);
                xoSlider->setValue (getFilter()->Xoff[i], dontSendNotification);
                triggerSlider->setValue (getFilter()->trigger[i], dontSendNotification);
                roundnessSlider->setValue (getFilter()->roundness[i], dontSendNotification);
                iconSizeSlider->setValue (getFilter()->iconsize[i], dontSendNotification);
                bool centered = getFilter()->centeredText[i];

                m.clear();
                sub1.clear();
                sub2.clear();
                sub3.clear();
                icons.clear();
                icons.addItem (99999, "Load...");
                Array<File> iconFiles;
                int j = 0;
                for (auto&& entry : RangedDirectoryIterator (File (getFilter()->iconPath), true))
                {
                    if (entry.getFile().hasFileExtension ("svg")
                        || entry.getFile().hasFileExtension ("png")
                        || entry.getFile().hasFileExtension ("jpg")
                        || entry.getFile().hasFileExtension ("gif"))
                    {
                        iconFiles.add (entry.getFile());
                        icons.addItem (100000 + j, entry.getFile().getRelativePathFrom (getFilter()->iconPath));
                        ++j;
                    }
                }

                m.addSectionHeader ("Pad Text:");
                m.addCustomItem (1, *textEditor, 200, 72, false);
                m.addSeparator();
                m.addItem (66, "Clear Icon");
                m.addSubMenu ("Icon", icons);
                sub1.addCustomItem (1234, *colourSelector1, 300, 300, false);
                m.addSubMenu ("Color", sub1);
                m.addItem (10, "Show Dot", true, showdots[i]);
                m.addItem (11, "Show Values", true, showvalues[i]);
                m.addItem (12, "Centered Text", true, centered);
                m.addItem (-1, "Pad Roundness:", false);
                m.addCustomItem (-1, *roundnessSlider, 64, 16, false);
                m.addItem (-1, "Icon Size:", false);
                m.addCustomItem (-1, *iconSizeSlider, 64, 16, false);
                m.addSeparator();

                m.addItem (2, String ("Note"), true, ! iscc);
                m.addItem (3, String ("CC"), true, iscc);
                m.addItem (4, String ("Use Y-Position"), true, usey);
                m.addItem (7, String ("Send Y-CC with Note"), true, useycc);
                m.addItem (5, String ("Use X-Position"), true, usex);
                m.addItem (8, String ("X is Pitch Bend"), usex, usexpb);
                m.addItem (6, String ("Send Off Values"), true, useoff);
                m.addItem (9, String ("Toggle Mode"), true, toggle);
                sub2.addItem (-1, "Note #:", false);
                sub2.addCustomItem (-1, *nSlider, 64, 16, false);
                sub2.addItem (-1, "Y-CC #:", false);
                sub2.addCustomItem (-1, *ySlider, 64, 16, false);
                sub2.addItem (-1, "On Value:", false);
                sub2.addCustomItem (-1, *vSlider, 64, 16, false);
                sub2.addItem (-1, "Off Value:", false);
                sub2.addCustomItem (-1, *oSlider, 64, 16, false);
                sub2.addItem (-1, "X-CC #:", false);
                sub2.addCustomItem (-1, *xSlider, 64, 16, false);
                sub2.addItem (-1, "X-CC Off Value:", false);
                sub2.addCustomItem (-1, *xoSlider, 64, 16, false);
                sub2.addItem (-1, "Trigger Note #:", false);
                sub2.addCustomItem (-1, *triggerSlider, 64, 16, false);
                m.addSubMenu ("Values", sub2);
                sub3.addItem (88, "Learn Note/Y-CC");
                sub3.addItem (89, "Learn Y-Off");
                sub3.addItem (90, "Learn X-CC");
                sub3.addItem (91, "Learn X-Off");
                sub3.addItem (92, "Learn Trigger");
                m.addSubMenu ("Learn", sub3);
                m.addSeparator();

                int result = m.show();
                if (result > 0)
                {
                    if (result >= 100000)
                    {
                        result -= 100000;
                        if (pad->setImageFromFile (iconFiles[result]))
                        {
                            //save the relative path
                            pad->setIconPath (iconFiles[result].getRelativePathFrom (File (getFilter()->iconPath)));
                            getFilter()->icon[pad->getIndex()] = pad->getIconPath();
                        }
                    }
                    else if (result == 99999)
                    {
                        FileChooser myChooser ("Load icon...",
                                               File (getFilter()->iconPath),
                                               "*.png;*.gif;*.svg;*.jpg");

                        if (myChooser.browseForFileToOpen())
                        {
                            File file (myChooser.getResult());
                            if (pad->setImageFromFile (file))
                            {
                                pad->setIconPath (file.getRelativePathFrom (File (getFilter()->iconPath)));
                                getFilter()->icon[pad->getIndex()] = pad->getIconPath();
                            }
                        }
                    }
                    else if (result == 2 || result == 3)
                    {
                        if (iscc)
                        {
                            getFilter()->Ytype[i] = 0;
                            midiPad[i]->setYInt (getFilter()->Ydata2[i]);
                            midiPad[i]->setButtonText ("CC " + String (getFilter()->Ydata1[i]));
                        }
                        else
                        {
                            getFilter()->Ytype[i] = 1;
                            if (getFilter()->SendOff[i])
                                midiPad[i]->setYInt (getFilter()->Yoff[i]);
                            else
                                midiPad[i]->setYInt (getFilter()->Ydata2[i]);
                            midiPad[i]->setButtonText (MidiMessage::getMidiNoteName (getFilter()->Ydata1[i], true, true, 3));
                        }
                    }
                    else if (result == 4)
                    {
                        if (usey)
                        {
                            //turning off use y
                            getFilter()->UseY[i] = false;
                            if (showdots[i] || showvalues[i])
                            {
                                if (usex)
                                    midiPad[i]->setYFloat (0.5f); //"horizontal slider" mode
                                if (getFilter()->SendOff[i])
                                {
                                    if (getFilter()->Ytype[i] == 0)
                                        midiPad[i]->setYInt (getFilter()->Yoff[i]);
                                    midiPad[i]->showy = false;
                                    midiPad[i]->repaint();
                                }
                                else
                                {
                                    midiPad[i]->setYInt (getFilter()->Ydata2[i]);
                                    midiPad[i]->showy = false;
                                    midiPad[i]->repaint();
                                }
                            }
                        }
                        else
                        {
                            getFilter()->UseY[i] = true;
                            midiPad[i]->showy    = true;
                            midiPad[i]->repaint();
                        }
                    }
                    else if (result == 5)
                    {
                        if (usex)
                        {
                            // turning off use x
                            getFilter()->UseX[i] = false;
                            if (showdots[i] || showvalues[i])
                            {
                                midiPad[i]->setXFloat (0.5f);
                                midiPad[i]->showx = false;
                                midiPad[i]->repaint();
                            }
                        }
                        else
                        {
                            getFilter()->UseX[i] = true;
                            midiPad[i]->showx    = true;
                            midiPad[i]->repaint();
                        }
                    }
                    else if (result == 6)
                    {
                        if (useoff)
                        {
                            getFilter()->SendOff[i] = 0.0f;
                            if (showdots[i] || showvalues[i])
                            {
                                midiPad[i]->setYInt (getFilter()->Ydata2[i]);
                                midiPad[i]->repaint();
                            }
                        }
                        else
                        {
                            getFilter()->SendOff[i] = 1.0f;
                            if (showdots[i] || showvalues[i])
                            {
                                if (getFilter()->Ytype[i] == 1)
                                {
                                    midiPad[i]->setYInt (getFilter()->Yoff[i]);
                                    midiPad[i]->repaint();
                                }
                            }
                        }
                    }
                    else if (result == 7)
                    {
                        getFilter()->UseYCC[i] = ! useycc;
                    }
                    else if (result == 8)
                    {
                        getFilter()->UseXPB[i] = ! usexpb;
                    }
                    else if (result == 9)
                    {
                        getFilter()->toggle[i] = ! toggle;
                        pad->showdot           = toggle;
                        pad->repaint();
                    }
                    else if (result == 10)
                    {
                        pad->showdot             = ! showdots[i];
                        getFilter()->showdots[i] = ! showdots[i];
                        showdots[i]              = ! showdots[i];
                        pad->repaint();
                    }
                    else if (result == 11)
                    {
                        pad->showvalues            = ! showvalues[i];
                        getFilter()->showvalues[i] = ! showvalues[i];
                        showvalues[i]              = ! showvalues[i];
                        pad->repaint();
                    }
                    else if (result == 12)
                    {
                        pad->setCenteredText (! centered);
                        getFilter()->centeredText[i] = ! centered;
                        pad->repaint();
                    }
                    else if (result == 66)
                    {
                        getFilter()->icon[i] = String();
                        pad->setImages (0);
                        pad->setIconPath (String());
                    }
                    else if (result == 88)
                        getFilter()->midilisten[i] = 1.0f;
                    else if (result == 89)
                        getFilter()->midilisten[i] = 0.9f;
                    else if (result == 90)
                        getFilter()->midilisten[i] = 0.8f;
                    else if (result == 91)
                        getFilter()->midilisten[i] = 0.7f;
                    else if (result == 92)
                        getFilter()->midilisten[i] = 0.6f;
                }
            }
        }
        //send midi
        else if (! mousebutton.isAltDown() && ! sending[i])
        {
            if (mousebutton.isShiftDown())
            { //shift-click: toggle
                getFilter()->lastxccvalue[i] = -1;
                getFilter()->lastyccvalue[i] = -1;
                sendMidi (i, true);
            }
            else
            {
                getFilter()->lastxccvalue[i] = -1;
                getFilter()->lastyccvalue[i] = -1;
                if (! (getFilter()->getParameter (kNoteOnTrig) >= 0.5f
                       && getFilter()->getParameter (kUseTrigger) >= 0.5f))
                    sendMidi (i);
            }
        }
    }
}

void midiPadsEditor::sendMidi (int i, bool shiftclicked)
{
    if (sending[i])
        return;
    sending[i] = true;
    if (getFilter()->togglestate[i])
    {
        getFilter()->togglestate[i] = false;
        midiPad[i]->setToggleState (false, dontSendNotification);
        sendMidiOff (i);
        sending[i] = false;
        return;
    }
    if (getFilter()->getParameter (kMono) >= 0.5f)
    {
        for (int n = 0; n < numPads; n++)
        {
            getFilter()->togglestate[n] = false;
            midiPad[n]->setToggleState (false, dontSendNotification);
        }
    }
    if (getFilter()->toggle[i] || shiftclicked)
    {
        dontsend                    = true;
        getFilter()->togglestate[i] = true;
        midiPad[i]->setToggleState (true, dontSendNotification);
    }
    midiPad[i]->isPlaying = true;
    if (! getFilter()->triggered[i])
    {
        Point<int> xy = midiPad[i]->getMouseXYRelative();
        //measure y from the bottom, make into range 0..1
        float fy = 1.0f - (((float) (xy.getY())) / ((float) (midiPad[i]->getHeight())));
        float fx = (((float) (xy.getX())) / ((float) (midiPad[i]->getWidth())));
        if (showdots[i] || showvalues[i])
        {
            if (getFilter()->UseX[i])
                midiPad[i]->setXFloat (fx);
            else
                midiPad[i]->setXFloat (0.5f);
            if (getFilter()->UseY[i])
                midiPad[i]->setYFloat (fy);
            else
            {
                if (getFilter()->UseX[i])
                    midiPad[i]->setYFloat (0.5f);
                else
                    midiPad[i]->setYInt (getFilter()->Ydata2[i]);
            }
            midiPad[i]->repaint();
        }
        if (getFilter()->UseX[i])
        {
            if (automateHost)
                getFilter()->setParameterNotifyingHost (i + xpos, fx);
            else
                getFilter()->setParameter (i + xpos, fx);
            getFilter()->buttondown[i] = true;
            lastx[i]                   = (int) (fx * 127.1);
        }
        if (automateHost)
            getFilter()->setParameterNotifyingHost (i + ypos, fy);
        else
            getFilter()->setParameter (i + ypos, fy);
        getFilter()->send[i] = 1.0f;
        lasty[i]             = (int) (fy * 127.1);
    }
    sending[i] = false;
}

void midiPadsEditor::buttonClicked (Button* buttonThatWasClicked) //mouseup
{
    if (buttonThatWasClicked == menuButton)
    {
        Array<File> layoutFiles, presetFiles;
        File (getFilter()->layoutPath).findChildFiles (layoutFiles, File::findFiles, true, "*.mpadl");
        File (getFilter()->presetPath).findChildFiles (presetFiles, File::findFiles, true, "*.mpads");
        //main menu
        bool sendoffvalue  = true;
        bool useyvalue     = true;
        bool usexvalue     = true;
        bool showalldots   = true;
        bool showallvalues = true;
        bool centeredNames = true;
        for (int i = 0; i < numPads; i++)
        {
            if (midiPad[i]->isVisible())
            {
                if (! getFilter()->SendOff[i])
                    sendoffvalue = false;
                if (! getFilter()->UseY[i])
                    useyvalue = false;
                if (! getFilter()->UseX[i])
                    usexvalue = false;
                if (! getFilter()->centeredText[i])
                    centeredNames = false;
                if (! showdots[i])
                    showalldots = false;
                if (! showvalues[i])
                    showallvalues = false;
            }
        }
        bool triggering = getFilter()->getParameter (kUseTrigger) >= 0.5f;
        bool thru       = getFilter()->getParameter (kThru) >= 0.5f;
        bool useinvel   = getFilter()->getParameter (kUseVel) >= 0.5f;
        bool useaft     = getFilter()->getParameter (kSendAft) >= 0.5f;
        bool monomode   = getFilter()->getParameter (kMono) >= 0.5f;
        bool editmode   = getFilter()->editmode;
        bool usemouseup = getFilter()->usemouseup;
        bool noteontrig = getFilter()->getParameter (kNoteOnTrig) >= 0.5f;

        m.clear();
        layout.clear();
        presets.clear();
        fileMenu.clear();

        m.addItem (1001, String ("Full Screen"), true, fullscreen);
        m.addItem (33, String ("Edit Mode"), true, editmode);
        m.addSeparator();
        for (int i = 0; i < layoutFiles.size(); i++)
            layout.addItem (100000 + i, layoutFiles[i].getFileNameWithoutExtension());
#ifdef _DEBUG
        layout.addItem (18, String ("1 Pad"), true);
        layout.addItem (17, String ("4 Pads"), true);
        layout.addItem (19, String ("4 Sliders"), true);
        layout.addItem (10, String ("10 Pads"), true);
        layout.addItem (14, String ("2 Pads, 12 Sliders"), true);
        layout.addItem (20, String ("16 Pads"), true);
        layout.addItem (21, String ("16 Sliders"), true);
        layout.addItem (22, String ("64 Pads"), true);
        layout.addItem (23, String ("8 Ch Mixer"), true);
        layout.addItem (24, String ("Hexagons"), true);
        layout.addItem (28, String ("2 XY, 5 Sliders, 21 Buttons"));
        layout.addItem (52, String ("2 XY, 49 Buttons"), true);
        layout.addItem (46, String ("12 Sliders, 2 XY, 25 Buttons"));
        layout.addItem (48, String ("6 Mixer Blocks"), true);
        layout.addItem (64, String ("8 Mixer Blocks w/sends"), true);
        layout.addItem (55, String ("1 XY, 6 Sliders, 48 Buttons"), true);
#endif
        layout.addItem (9998, String ("Load..."), true);
        layout.addItem (9999, String ("Save..."), true);
        m.addSubMenu ("Layout", layout);
        m.addItem (111, "Clear all icons");
        m.addItem (7, String ("Show Dots"), true, showalldots);
        m.addItem (77, String ("Show Values"), true, showallvalues);
        m.addItem (33333, "Centered Text", true, centeredNames);

        m.addItem (-1, "Pad Roundness:", false);
        m.addCustomItem (-1, *globalRoundnessSlider, 64, 16, false);
        m.addItem (-1, "Icon Size:", false);
        m.addCustomItem (-1, *globalIconSizeSlider, 64, 16, false);
        m.addSeparator();

        m.addItem (1, String ("Send off values"), true, sendoffvalue);
        m.addItem (2, String ("Use Midi Triggering"), true, triggering);
        m.addItem (3, String ("Use Input Velocity"), triggering, useinvel);
        //m.addItem (222, String("Piezo Mode"), triggering, noteontrig);
        m.addItem (4, String ("Use Y-Position"), true, useyvalue);
        m.addItem (5, String ("Use X-Position"), true, usexvalue);
        m.addItem (6, String ("Send Aftertouch"), true, useaft);
        m.addItem (8, String ("Mono Mode"), true, monomode);
        m.addItem (9, String ("Use mouseup anywhere"), true, usemouseup);
        m.addItem (50, String ("Midi Thru"), true, thru);
        m.addSeparator();
        m.addItem (-1, "In Channel:", false);
        m.addCustomItem (-1, *icSlider, 64, 16, false);
        m.addItem (-1, "Out Channel:", false);
        m.addCustomItem (-1, *cSlider, 64, 16, false);
        m.addSeparator();
        for (int i = 0; i < presetFiles.size(); i++)
            presets.addItem (200000 + i, presetFiles[i].getFileNameWithoutExtension());
        m.addSubMenu ("Presets", presets);
        fileMenu.addItem (5473, String ("Save Bank..."));
        fileMenu.addItem (54731, String ("Save Patch..."));
        fileMenu.addItem (9999, String ("Save Layout..."));
        fileMenu.addItem (1042, String ("Load Bank/Patch..."));
        m.addSubMenu ("Save/Load", fileMenu);
        m.addItem (-1, String ("- midiPads ") + String (JucePlugin_VersionString) + String (" -"), false);

        int result = m.showAt (menuButton);
        if (result > 0)
        {
            if (result >= 200000)
            {
                result -= 200000;
                if (presetFiles[result].hasFileExtension ("mpads"))
                    getFilter()->loadXmlPatch (getFilter()->getCurrentProgram(), presetFiles[result]);
                else if (presetFiles[result].hasFileExtension ("mpadb"))
                    getFilter()->loadXmlBank (presetFiles[result]);
                resized();
            }
            else if (result >= 100000)
            {
                result -= 100000;
                if (layoutFiles[result].hasFileExtension ("mpadl"))
                    getFilter()->loadXmlLayout (layoutFiles[result]);
                resized();
            }
            else if (result == 33333)
            {
                for (int i = 0; i < numPads; i++)
                {
                    getFilter()->centeredText[i] = ! centeredNames;
                    midiPad[i]->setCenteredText (! centeredNames);
                    //midiPad[i]->repaint();
                }
            }
            else if (result == 9999)
            {
                FileChooser myChooser ("Save layout...",
                                       File (getFilter()->layoutPath + File::getSeparatorString()
                                             + getFilter()->getProgramName (getFilter()->getCurrentProgram())
                                             + ".mpadl"),
                                       "*.mpadl");
                if (myChooser.browseForFileToSave (true))
                {
                    File file (myChooser.getResult());
                    //getFilter()->patchPath = file.getParentDirectory().getFullPathName();
                    if (! file.hasFileExtension ("mpadl"))
                        file = file.withFileExtension ("mpadl");

                    getFilter()->saveXmlLayout (file);
                }
            }
            else if (result == 9998)
            {
                FileChooser myChooser ("Load layout...",
                                       File (getFilter()->layoutPath),
                                       "*.mpadl");

                if (myChooser.browseForFileToOpen())
                {
                    File file (myChooser.getResult());
                    getFilter()->loadXmlLayout (file);
                    resized();
                }
            }
            else if (result == 5473)
            {
                FileChooser myChooser ("Save bank...",
                                       File (getFilter()->bankPath + File::getSeparatorString() + "midiPads.mpadb"),
                                       "*.mpadb");

                if (myChooser.browseForFileToSave (true))
                {
                    File file (myChooser.getResult());
                    //getFilter()->patchPath = file.getParentDirectory().getFullPathName();
                    if (! file.hasFileExtension ("mpadb"))
                        file = file.withFileExtension ("mpadb");

                    getFilter()->saveXmlBank (file);
                }
            }
            else if (result == 54731)
            {
                FileChooser myChooser ("Save patch...",
                                       File (getFilter()->presetPath
                                             + File::getSeparatorString()
                                             + getFilter()->getProgramName (getFilter()->getCurrentProgram())
                                             + ".mpads"),
                                       "*.mpads");

                if (myChooser.browseForFileToSave (true))
                {
                    File file (myChooser.getResult());
                    //getFilter()->patchPath = file.getParentDirectory().getFullPathName();
                    if (! file.hasFileExtension ("mpads"))
                        file = file.withFileExtension ("mpads");

                    getFilter()->saveXmlPatch (getFilter()->getCurrentProgram(), file);
                }
            }
            else if (result == 1042)
            {
                FileChooser myChooser ("Load bank/patch...",
                                       File (getFilter()->pluginPath),
                                       "*.mpads;*.mpadb;*.mpadl;*.fxb;*.fxp");

                if (myChooser.browseForFileToOpen())
                {
                    File file (myChooser.getResult());
                    if (file.hasFileExtension ("mpadb"))
                        getFilter()->loadXmlBank (file);
                    else if (file.hasFileExtension ("mpads"))
                        getFilter()->loadXmlPatch (getFilter()->getCurrentProgram(), file);
                    else if (file.hasFileExtension ("fxb"))
                        getFilter()->loadFxb (file);
                    else if (file.hasFileExtension ("fxp"))
                        getFilter()->loadFxp (file);
                    else if (file.hasFileExtension ("mpadl"))
                        getFilter()->loadXmlLayout (file);
                }
            }
            else if (result == 1)
            {
                if (sendoffvalue)
                {
                    for (int i = 0; i < numPads; i++)
                    {
                        getFilter()->SendOff[i] = 0.0f;
                        if (showdots[i] || showvalues[i])
                        {
                            midiPad[i]->setYInt (getFilter()->Ydata2[i]);
                            midiPad[i]->repaint();
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < numPads; i++)
                    {
                        getFilter()->SendOff[i] = 1.0f;
                        if (showdots[i] || showvalues[i])
                        {
                            if (getFilter()->Ytype[i] == 1)
                            {
                                midiPad[i]->setYInt (getFilter()->Yoff[i]);
                                midiPad[i]->repaint();
                            }
                        }
                    }
                }
            }
            else if (result == 1001)
            {
                if (! fullscreen)
                {
                    container->addToDesktop (0);
                    Desktop::getInstance().setKioskModeComponent (container, false);
                    fullscreen = true;
                    resized();
                }
                else
                {
                    Desktop::getInstance().setKioskModeComponent (0);
                    this->addChildComponent (container);
                    fullscreen = false;
                    resized();
                }
            }
            else if (result == 9)
            {
                getFilter()->usemouseup = ! getFilter()->usemouseup;
            }
            else if (result == 2)
            {
                if (getFilter()->getParameter (kUseTrigger) >= 0.5f)
                {
                    getFilter()->setParameterNotifyingHost (kUseTrigger, 0.0f);
                }
                else
                    getFilter()->setParameterNotifyingHost (kUseTrigger, 1.0f);
            }
            else if (result == 222)
            {
                if (getFilter()->getParameter (kNoteOnTrig) >= 0.5f)
                {
                    getFilter()->setParameterNotifyingHost (kNoteOnTrig, 0.0f);
                }
                else
                    getFilter()->setParameterNotifyingHost (kNoteOnTrig, 1.0f);
            }
            else if (result == 3)
            {
                if (getFilter()->getParameter (kUseVel) >= 0.5f)
                {
                    getFilter()->setParameterNotifyingHost (kUseVel, 0.0f);
                }
                else
                    getFilter()->setParameterNotifyingHost (kUseVel, 1.0f);
            }
            else if (result == 4)
            {
                if (useyvalue)
                {
                    //turning off use y
                    for (int i = 0; i < numPads; i++)
                    {
                        getFilter()->UseY[i] = false;
                        if (showdots[i] || showvalues[i])
                        {
                            if (getFilter()->SendOff[i])
                            {
                                if (getFilter()->Ytype[i] == 1)
                                    midiPad[i]->setYInt (getFilter()->Yoff[i]);
                                midiPad[i]->showy = false;
                                midiPad[i]->repaint();
                            }
                            else
                            {
                                midiPad[i]->setYInt (getFilter()->Ydata2[i]);
                                midiPad[i]->showy = false;
                                midiPad[i]->repaint();
                            }
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < numPads; i++)
                    {
                        getFilter()->UseY[i] = true;
                        midiPad[i]->showy    = true;
                        midiPad[i]->repaint();
                    }
                }
            }
            else if (result == 5)
            {
                if (usexvalue)
                {
                    // turning off use x
                    for (int i = 0; i < numPads; i++)
                    {
                        getFilter()->UseX[i] = false;
                        if (showdots[i] || showvalues[i])
                        {
                            midiPad[i]->setXFloat (0.5f);
                            midiPad[i]->showx = false;
                            midiPad[i]->repaint();
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < numPads; i++)
                    {
                        getFilter()->UseX[i] = false;
                        midiPad[i]->showx    = true;
                        midiPad[i]->repaint();
                    }
                }
            }
            else if (result == 6)
            {
                if (getFilter()->getParameter (kSendAft) >= 0.5f)
                {
                    getFilter()->setParameterNotifyingHost (kSendAft, 0.0f);
                }
                else
                    getFilter()->setParameterNotifyingHost (kSendAft, 1.0f);
            }
            else if (result == 7)
            {
                for (int i = 0; i < numPads; i++)
                {
                    getFilter()->showdots[i] = ! showalldots;
                    showdots[i]              = ! showalldots;
                    midiPad[i]->showdot      = ! showalldots;
                    midiPad[i]->repaint();
                }
            }
            else if (result == 77)
            {
                for (int i = 0; i < numPads; i++)
                {
                    getFilter()->showvalues[i] = ! showallvalues;
                    showvalues[i]              = ! showallvalues;
                    midiPad[i]->showvalues     = ! showallvalues;
                    midiPad[i]->repaint();
                }
            }
            else if (result == 8)
            {
                if (getFilter()->getParameter (kMono) >= 0.5f)
                {
                    getFilter()->setParameterNotifyingHost (kMono, 0.0f);
                }
                else
                    getFilter()->setParameterNotifyingHost (kMono, 1.0f);
            }

            else if (result == 50)
            {
                if (getFilter()->getParameter (kThru) >= 0.5f)
                {
                    getFilter()->setParameterNotifyingHost (kThru, 0.0f);
                }
                else
                    getFilter()->setParameterNotifyingHost (kThru, 1.0f);
            }
            else if (result == 18)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 1;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), onepad);
                squares = 1;
                resized();
            }
            else if (result == 17)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 4;
                squares              = 4;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), fourpads);
                resized();
            }
            else if (result == 19)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 4;
                squares              = 4;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), foursliders);
                resized();
            }
            else if (result == 10)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 10;
                squares              = 10;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), tenpads);
                resized();
            }
            else if (result == 14)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 14;
                squares              = 14;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), twelvepads);
                resized();
            }
            else if (result == 20)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 16;
                squares              = 16;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), sixteenpads);
                resized();
            }
            else if (result == 21)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 16;
                squares              = 16;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), sixteensliders);
                resized();
            }
            else if (result == 22)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 64;
                squares              = 64;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), sixtyfourpads);
                resized();
            }
            else if (result == 23)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 32;
                squares              = 32;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), mixerpads);
                resized();
            }
            else if (result == 24)
            {
                getFilter()->hex     = true;
                getFilter()->squares = 40;
                squares              = 40;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), hexagonpads);
                resized();
            }
            else if (result == 28)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 28;
                squares              = 28;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), arrangeit28);
                resized();
            }
            else if (result == 46)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 39;
                squares              = 39;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), arrangeit39);
                resized();
            }
            else if (result == 52)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 51;
                squares              = 51;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), arrangeit51);
                resized();
            }
            else if (result == 48)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 48;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), arrangeit48);
                squares = 48;
                resized();
            }
            else if (result == 64)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 64;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), arrangeit64);
                squares = 64;
                resized();
            }
            else if (result == 55)
            {
                getFilter()->hex     = false;
                getFilter()->squares = 55;
                getFilter()->setLayout (getFilter()->getCurrentProgram(), arrangeit55);
                squares = 55;
                resized();
            }
            else if (result == 111)
            {
                for (int i = 0; i < numPads; i++)
                {
                    getFilter()->icon[i] = String();
                    midiPad[i]->setImages (0);
                    midiPad[i]->setIconPath (String());
                }
            }
            else if (result == 33)
            {
                if (editmode)
                {
                    getFilter()->editmode = false;
                    hueSlider->setVisible (false);
                    saturationSlider->setVisible (false);
                    brigtnessSlider->setVisible (false);
                    padOpacitySlider->setVisible (false);
                    velocitySlider->setVisible (false);
                    valueSlider->setVisible (false);
                }
                else
                {
                    getFilter()->editmode = true;
                    hueSlider->setVisible (true);
                    saturationSlider->setVisible (true);
                    brigtnessSlider->setVisible (true);
                    padOpacitySlider->setVisible (true);
                    velocitySlider->setVisible (true);
                    valueSlider->setVisible (true);
                }
            }
        }
    }
    else if (! ModifierKeys::getCurrentModifiers().isAltDown())
    {
        if (! dontsend)
        {
            if (buttonThatWasClicked->getName().compare ("MidiPad"))
                return;
            int i = ((MidiPad*) buttonThatWasClicked)->getIndex();
            if (getFilter()->midilisten[i] < 0.5f && ! getFilter()->toggle[i])
            {
                //only send if not learning & not in toggle mode
                sendMidiOff (i);
            }
        }
        //else dontsend=false;
    }
    else
    {
        lastTouchedPad = ((MidiPad*) buttonThatWasClicked)->getIndex();
    }
}

void midiPadsEditor::sendMidiOff (int i)
{
    if (midiPad[i]->isPlaying)
    {
        getFilter()->sendoff[i] = 1.0f;
        midiPad[i]->isPlaying   = false;
        if ((showdots[i] || showvalues[i]) && getFilter()->SendOff[i])
        {
            if (getFilter()->UseX[i])
            {
                midiPad[i]->setXInt (getFilter()->Xoff[i]);
                midiPad[i]->repaint();
                lastx[i] = getFilter()->Xoff[i];
            }
            if (getFilter()->Ytype[i] == 1)
            {
                midiPad[i]->setYInt (getFilter()->Yoff[i]);
                midiPad[i]->repaint();
                lasty[i] = getFilter()->Yoff[i];
            }
        }
    }
}

void midiPadsEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == hueSlider)
    {
        color              = Colour ((float) hueSlider->getValue(), (float) saturationSlider->getValue(), (float) brigtnessSlider->getValue(), 1.0f);
        container->bgcolor = color;
        color2             = color.contrasting ((float) padOpacitySlider->getValue());
        getFilter()->bghue = (float) (hueSlider->getValue());
        repaint();
    }
    else if (sliderThatWasMoved == saturationSlider)
    {
        color              = Colour ((float) hueSlider->getValue(), (float) saturationSlider->getValue(), (float) brigtnessSlider->getValue(), 1.0f);
        container->bgcolor = color;
        color2             = color.contrasting ((float) padOpacitySlider->getValue());
        getFilter()->bgsat = (float) (saturationSlider->getValue());
        repaint();
    }
    else if (sliderThatWasMoved == brigtnessSlider)
    {
        color              = Colour ((float) hueSlider->getValue(), (float) saturationSlider->getValue(), (float) brigtnessSlider->getValue(), 1.0f);
        container->bgcolor = color;
        color2             = color.contrasting ((float) padOpacitySlider->getValue());
        getFilter()->bgbri = (float) brigtnessSlider->getValue();
        repaint();
    }
    else if (sliderThatWasMoved == padOpacitySlider)
    {
        color2                = color.contrasting ((float) padOpacitySlider->getValue());
        getFilter()->contrast = (float) padOpacitySlider->getValue();
        repaint();
    }
    else if (sliderThatWasMoved == globalRoundnessSlider)
    {
        for (int i = 0; i < numPads; i++)
        {
            midiPad[i]->roundness = (float) globalRoundnessSlider->getValue();
            midiPad[i]->repaint();
            getFilter()->roundness[i] = (float) globalRoundnessSlider->getValue();
        }
    }
    else if (sliderThatWasMoved == globalIconSizeSlider)
    {
        for (int i = 0; i < numPads; i++)
        {
            midiPad[i]->imageSize = (float) globalIconSizeSlider->getValue();
            midiPad[i]->repaint();
            getFilter()->iconsize[i] = (float) globalIconSizeSlider->getValue();
        }
    }
    else if (sliderThatWasMoved == velocitySlider)
    {
        getFilter()->setParameter (kVelOffset, ((float) velocitySlider->getValue()) * 0.5f);
    }
    else if (sliderThatWasMoved == valueSlider)
    {
        getFilter()->setParameter (kCCOffset, ((float) valueSlider->getValue()) * 0.5f);
    }
    else if (sliderThatWasMoved == icSlider)
    {
        getFilter()->setParameterNotifyingHost (kChIn, ((float) icSlider->getValue() - 1) / 15.0f);
    }
    else if (sliderThatWasMoved == cSlider)
    {
        getFilter()->setParameterNotifyingHost (kChOut, ((float) cSlider->getValue() - 1.0f) / 15.0f);
    }
    else if (sliderThatWasMoved == vSlider)
    {
        getFilter()->Ydata2[lastPadMenu] = roundToInt (vSlider->getValue());
        if (! getFilter()->SendOff[lastPadMenu] && ! getFilter()->UseY[lastPadMenu])
        {
            midiPad[lastPadMenu]->setYInt (getFilter()->Ydata2[lastPadMenu]);
            midiPad[lastPadMenu]->repaint();
        }
    }
    else if (sliderThatWasMoved == nSlider)
    {
        getFilter()->Ydata1[lastPadMenu] = roundToInt (nSlider->getValue());
    }
    else if (sliderThatWasMoved == ySlider)
    {
        getFilter()->Ycc[lastPadMenu] = roundToInt (ySlider->getValue());
    }
    else if (sliderThatWasMoved == oSlider)
    {
        getFilter()->Yoff[lastPadMenu] = roundToInt (oSlider->getValue());
        if (getFilter()->SendOff[lastPadMenu] && getFilter()->Ytype[lastPadMenu] == 1)
        {
            midiPad[lastPadMenu]->setYInt (getFilter()->Yoff[lastPadMenu]);
            midiPad[lastPadMenu]->repaint();
        }
    }
    else if (sliderThatWasMoved == triggerSlider)
    {
        getFilter()->trigger[lastPadMenu] = roundToInt (triggerSlider->getValue());
    }
    else if (sliderThatWasMoved == xSlider)
    {
        getFilter()->Xcc[lastPadMenu] = roundToInt (xSlider->getValue());
        if (! getFilter()->SendOff[lastPadMenu])
        {
            midiPad[lastPadMenu]->setXFloat (getFilter()->getParameter (lastPadMenu + xpos));
            midiPad[lastPadMenu]->repaint();
        }
    }
    else if (sliderThatWasMoved == xoSlider)
    {
        getFilter()->Xoff[lastPadMenu] = roundToInt (xoSlider->getValue());
        if (getFilter()->SendOff[lastPadMenu])
        {
            midiPad[lastPadMenu]->setXInt (getFilter()->Xoff[lastPadMenu]);
            midiPad[lastPadMenu]->repaint();
        }
    }
    else if (sliderThatWasMoved == roundnessSlider)
    {
        getFilter()->roundness[lastPadMenu] = (float) roundnessSlider->getValue();
        midiPad[lastPadMenu]->roundness     = (float) roundnessSlider->getValue();
        midiPad[lastPadMenu]->repaint();
    }
    else if (sliderThatWasMoved == iconSizeSlider)
    {
        getFilter()->iconsize[lastPadMenu] = (float) iconSizeSlider->getValue();
        midiPad[lastPadMenu]->imageSize    = (float) iconSizeSlider->getValue();
        midiPad[lastPadMenu]->repaint();
    }
}

void midiPadsEditor::textEditorTextChanged (TextEditor& editor)
{
    if (&editor == textEditor)
    {
        midiPad[lastPadMenu]->setText (textEditor->getText());
        midiPad[lastPadMenu]->repaint();
    }
}

void midiPadsEditor::textEditorReturnKeyPressed (TextEditor& editor)
{
    if (&editor == textEditor)
    {
        midiPad[lastPadMenu]->setText (textEditor->getText());
        getFilter()->text[lastPadMenu] = textEditor->getText();
        midiPad[lastPadMenu]->repaint();
    }
}

void midiPadsEditor::textEditorEscapeKeyPressed (TextEditor& editor)
{
    if (&editor == textEditor)
    {
        textEditor->setText (getFilter()->text[lastPadMenu]);
        midiPad[lastPadMenu]->repaint();
    }
}

void midiPadsEditor::textEditorFocusLost (TextEditor& editor)
{
    if (&editor == textEditor)
    {
        midiPad[lastPadMenu]->setText (textEditor->getText());
        getFilter()->text[lastPadMenu] = textEditor->getText();
        midiPad[lastPadMenu]->repaint();
    }
}

//==============================================================================
void midiPadsEditor::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == getFilter())
    {
        if (getFilter()->trig)
            padTriggered();
        else
        {
            for (int i = 0; i < numPads; i++)
            {
                if (getFilter()->dotmoved[i])
                {
                    dotMoved (i);
                    return;
                }
            }
        }
        updateParametersFromFilter();
    }
    else if (source == colourSelector1)
    {
        ColourSelector* cs = (ColourSelector*) source;
        midiPad[lastPadMenu]->setColour (cs->getCurrentColour().withAlpha (getFilter()->contrast));
        getFilter()->padcolor[lastPadMenu] = cs->getCurrentColour();
    }
}

void midiPadsEditor::padTriggered()
{
    if (getFilter()->getParameter (kNoteOnTrig) < 0.5f)
    {
        bool triggered[numPads];
        getFilter()->getCallbackLock().enter();
        memcpy (triggered, getFilter()->triggered, sizeof (triggered));
        getFilter()->getCallbackLock().exit();
        for (int i = 0; i < numPads; i++)
            midiPad[i]->setState (triggered[i] ? Button::buttonDown : Button::buttonNormal);
    }
    else
    {
        for (int i = 0; i < numPads; i++)
        {
            if (midiPad[i]->isMouseOver())
                sendMidi (i);
        }
    }
    getFilter()->trig = false;
}

void midiPadsEditor::dotMoved (int pad)
{
    float x = getFilter()->getParameter (pad + xpos);
    float y = getFilter()->getParameter (pad + ypos);
    if ((int) (x * 127.1) != lastx[pad] && getFilter()->UseX[pad])
    {
        midiPad[pad]->setXFloat (x);
        midiPad[pad]->repaint();
    }
    if ((int) (y * 127.1) != lasty[pad] && getFilter()->UseY[pad])
    {
        midiPad[pad]->setYFloat (y);
        midiPad[pad]->repaint();
    }
    getFilter()->dotmoved[pad] = false;
}

bool midiPadsEditor::isInterestedInFileDrag (const StringArray& files)
{
    File file = File (files.joinIntoString (String(), 0, 1));
    if (file.hasFileExtension ("png") || file.hasFileExtension ("gif") || file.hasFileExtension ("jpg") || file.hasFileExtension ("svg") || file.hasFileExtension ("mpads") || file.hasFileExtension ("mpadb") || file.hasFileExtension ("fxb") || file.hasFileExtension ("fxp"))
        return true;
    return false;
}

void midiPadsEditor::filesDropped (const juce::StringArray& filenames, int mouseX, int mouseY)
{
    if (isInterestedInFileDrag (filenames))
    {
        String filename = filenames.joinIntoString (String(), 0, 1);
        File file       = File (filename);
        if (file.hasFileExtension ("mpads"))
        {
            getFilter()->loadXmlPatch (getFilter()->getCurrentProgram(), file);
            resized();
        }
        else if (file.hasFileExtension ("mpadb"))
        {
            getFilter()->loadXmlBank (file);
            resized();
        }
        else if (file.hasFileExtension ("mpadl"))
        {
            getFilter()->loadXmlLayout (file);
            resized();
        }
        else if (file.hasFileExtension ("fxb"))
        {
            getFilter()->loadFxb (file);
            resized();
        }
        else if (file.hasFileExtension ("fxp"))
        {
            getFilter()->loadFxp (file);
            resized();
        }
        else
        {
            Component* c = getComponentAt (mouseX, mouseY);
            if (! c->getName().compare ("MidiPad"))
            {
                MidiPad* pad = (MidiPad*) c;
                if (pad->setImageFromFile (file))
                {
                    //save the relative path
                    pad->setIconPath (file.getRelativePathFrom (File (getFilter()->iconPath)));
                    getFilter()->icon[pad->getIndex()] = pad->getIconPath();
                }
            }
        }
    }
}

//==============================================================================
void midiPadsEditor::updateParametersFromFilter()
{
    midiPads* const filter = getFilter();
    int newMidiData[numPads];
    int newMidiType[numPads];
    int newMidiValue[numPads];
    //int newOffValue[numPads];
    //int newTrigger[numPads];
    //int newXCC[numPads];
    //int newYCC[numPads];
    int newXOff[numPads];
    float newX[numPads];
    float newY[numPads];
    bool usex[numPads];
    bool usey[numPads];
    bool useoff[numPads];
    String t[numPads];
    String icon[numPads];
    Colour padcolor[numPads];
    bool toggle[numPads];
    bool tstate[numPads];
    float roundness[numPads];
    float iconsize[numPads];
    bool centered[numPads];

    filter->getCallbackLock().enter();
    // take a local copy of the info we need while we've got the lock..

    memcpy (centered, filter->centeredText, sizeof (centered));
    memcpy (roundness, filter->roundness, sizeof (roundness));
    memcpy (iconsize, filter->iconsize, sizeof (iconsize));
    memcpy (tstate, filter->togglestate, sizeof (tstate));
    memcpy (newMidiData, filter->Ydata1, sizeof (newMidiData));
    memcpy (newMidiType, filter->Ytype, sizeof (newMidiType));
    memcpy (newMidiValue, filter->Ydata2, sizeof (newMidiValue));
    //memcpy(newOffValue,filter->Yoff,sizeof(newOffValue));
    //memcpy(newTrigger,filter->trigger,sizeof(newTrigger));
    //memcpy(newXCC,filter->Xcc,sizeof(newXCC));
    //memcpy(newYCC,filter->Ycc,sizeof(newYCC));
    memcpy (newXOff, filter->Xoff, sizeof (newXOff));
    memcpy (usey, filter->UseY, sizeof (usey));
    memcpy (usex, filter->UseX, sizeof (usex));
    memcpy (useoff, filter->SendOff, sizeof (useoff));
    memcpy (toggle, filter->toggle, sizeof (toggle));
    memcpy (showdots, filter->showdots, sizeof (showdots));
    memcpy (showvalues, filter->showvalues, sizeof (showvalues));
    for (int i = 0; i < numPads; i++)
    {
        t[i]        = filter->text[i];
        icon[i]     = filter->icon[i];
        padcolor[i] = filter->padcolor[i];
        newX[i]     = filter->getParameter (i + xpos);
        newY[i]     = filter->getParameter (i + ypos);
    }
    const float newVelocity = filter->getParameter (kVelOffset);
    const float newCCValue  = filter->getParameter (kCCOffset);
    const float newInCh     = filter->getParameter (kChIn);
    const float newOutCh    = filter->getParameter (kChOut);
    const float useaft      = filter->getParameter (kSendAft);
    const float hue         = filter->bghue;
    const float sat         = filter->bgsat;
    const float bri         = filter->bgbri;
    const float contrast    = filter->contrast;
    const int newsquares    = filter->squares;
    const bool editmode     = filter->editmode;
    const bool newhex       = filter->hex;
    const bool usemouseup   = filter->usemouseup;

    // ..release the lock ASAP
    filter->getCallbackLock().exit();

    // ..and after releasing the lock, we're free to do the time-consuming UI stuff..
    for (int i = 0; i < numPads; i++)
    {
        //reconstruct the full path to the icon
        if (icon[i].isEmpty())
        {
            midiPad[i]->setImages (0);
            midiPad[i]->setIconPath (String());
        }
        else
        {
            String fullpath = icon[i];
            if (! File::getCurrentWorkingDirectory().getChildFile (fullpath).existsAsFile())
                fullpath = getFilter()->iconPath + File::getSeparatorString() + icon[i];
            //Drawable* image = Drawable::createFromImageFile(File(fullpath));
            if (! midiPad[i]->setImageFromFile (File (fullpath)))
                midiPad[i]->setImages (0);
            midiPad[i]->setIconPath (icon[i]);
        }

        midiPad[i]->setHex (newhex);
        midiPad[i]->showdot    = toggle[i] ? false : showdots[i];
        midiPad[i]->showvalues = showvalues[i];
        midiPad[i]->setColour (padcolor[i].withAlpha (contrast));
        midiPad[i]->roundness = roundness[i];
        midiPad[i]->imageSize = iconsize[i];
        midiPad[i]->setToggleState (tstate[i], dontSendNotification);
        midiPad[i]->setText (t[i]);
        midiPad[i]->setCenteredText (centered[i]);

        if (showdots[i] || showvalues[i])
        {
            if (! usex[i])
            {
                midiPad[i]->showx = false;
                midiPad[i]->setXFloat (0.5f);
            }
            else
            {
                midiPad[i]->showx = true;
                if (useoff[i])
                    midiPad[i]->setXInt (newXOff[i]);
                else
                    midiPad[i]->setXFloat (newX[i]);
                lastx[i] = (int) (newX[i] * 127.1);
            }
            if (! usey[i])
            {
                midiPad[i]->showy = false;
                if (newMidiType[i] == 1 && useoff[i])
                    midiPad[i]->setYInt (newMidiValue[i]);
                else
                {
                    if (usex[i]) //"horizontal slider" mode
                        midiPad[i]->setYFloat (0.5f);
                    else
                        midiPad[i]->setYInt (newMidiValue[i]);
                }
            }
            else
            {
                midiPad[i]->showy = true;
                midiPad[i]->setYFloat (newY[i]);
                lasty[i] = (int) (newY[i] * 127.1);
            }
        }
        if (newMidiType[i] == 1)
            midiPad[i]->setButtonText ("CC " + String (newMidiData[i]));
        else
            midiPad[i]->setButtonText (MidiMessage::getMidiNoteName (newMidiData[i], true, true, 3));
    }

    color              = Colour (hue, sat, bri, 1.0f);
    container->bgcolor = color;
    color2             = Colour (color.contrasting (contrast));

    hueSlider->setValue (hue, dontSendNotification);
    saturationSlider->setValue (sat, dontSendNotification);
    brigtnessSlider->setValue (bri, dontSendNotification);
    padOpacitySlider->setValue (contrast, dontSendNotification);
    globalRoundnessSlider->setValue (roundness[0], dontSendNotification);
    globalIconSizeSlider->setValue (iconsize[0], dontSendNotification);

    if (! editmode)
    {
        getFilter()->editmode = false;
        hueSlider->setVisible (false);
        saturationSlider->setVisible (false);
        brigtnessSlider->setVisible (false);
        padOpacitySlider->setVisible (false);
    }
    else
    {
        getFilter()->editmode = true;
        hueSlider->setVisible (true);
        saturationSlider->setVisible (true);
        brigtnessSlider->setVisible (true);
        padOpacitySlider->setVisible (true);
    }

    velocitySlider->setValue (newVelocity * 2.0, dontSendNotification);
    valueSlider->setValue (newCCValue * 2.0, dontSendNotification);
    icSlider->setValue ((float) (1 + (int) (newInCh * 15.1)), dontSendNotification);
    cSlider->setValue ((float) (1 + (int) (newOutCh * 15.1)), dontSendNotification);

    if (getWidth() != filter->lastUIWidth || getHeight() != filter->lastUIHeight)
        setSize (filter->lastUIWidth, filter->lastUIHeight);
    if (squares != newsquares)
    {
        squares = newsquares;
        resized();
    }
    repaint();
}
