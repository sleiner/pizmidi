#include "BigClockEditor.h"

using juce::jmin;
using juce::roundToInt;

TimeDisplay::TimeDisplay()
    : Button (juce::String())
{
    setSize (600, 400);
    textcolor = juce::Colour (0xff000000);
    setMouseClickGrabsKeyboardFocus (false);
}

TimeDisplay::~TimeDisplay()
{
}

//==============================================================================
void TimeDisplay::paintButton (juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    g.setColour (textcolor);
    g.setFont (juce::Font (jmin ((float) getHeight() * .95f, (float) getWidth() * .16f), juce::Font::bold));
    g.drawFittedText (time,
                      0,
                      0,
                      getWidth(),
                      getHeight(),
                      juce::Justification::centred,
                      true);
}

void TimeDisplay::resized()
{
    repaint();
}

//==============================================================================
BigClockEditor::BigClockEditor (BigClockFilter* const ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      showtextbox (ownerFilter->showcues)
{
    addChildComponent (modeLabel = new juce::Label ("Mode Label", juce::String()));
    modeLabel->setMouseClickGrabsKeyboardFocus (false);
    modeLabel->setFont (juce::Font (12.f, juce::Font::bold));
    modeLabel->setJustificationType (juce::Justification::topRight);

    addAndMakeVisible (infoLabel = new TimeDisplay());
    infoLabel->addListener (this);
    infoLabel->addMouseListener (this, false);

    colourSelector = new juce::ColourSelector (juce::ColourSelector::showColourAtTop | juce::ColourSelector::showSliders | juce::ColourSelector::showColourspace);
    colourSelector->setName (juce::String (L"color"));
    colourSelector->setCurrentColour (getFilter()->bgcolor);
    colourSelector->addChangeListener (this);

    addAndMakeVisible (textBox = new juce::TextEditor());
    textBox->setMultiLine (false);
    textBox->setReturnKeyStartsNewLine (false);
    textBox->setReadOnly (false);
    textBox->setScrollbarsShown (false);
    textBox->setCaretVisible (true);
    textBox->setPopupMenuEnabled (true);
    textBox->setText (juce::String());
    textBox->addListener (this);

    addAndMakeVisible (cueLabel = new juce::Label ("Cue Label", juce::String()));

    addAndMakeVisible (runButton = new juce::TextButton ("Run"));
    runButton->setMouseClickGrabsKeyboardFocus (false);
    runButton->setButtonText (">");
    runButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    runButton->addListener (this);

    addAndMakeVisible (resetButton = new juce::TextButton ("Reset"));
    resetButton->setMouseClickGrabsKeyboardFocus (false);
    resetButton->setButtonText ("0");
    resetButton->setConnectedEdges (juce::Button::ConnectedOnLeft | juce::Button::ConnectedOnRight);
    resetButton->addListener (this);

    addAndMakeVisible (resizer = new juce::ResizableCornerComponent (this, &resizeLimits));
    resizer->setMouseClickGrabsKeyboardFocus (false);
    resizeLimits.setSizeLimits (10, 10, 1200, 200);

    startTimer (50);
    ownerFilter->addChangeListener (this);
    updateParametersFromFilter();
}

BigClockEditor::~BigClockEditor()
{
    stopTimer();
    getFilter()->removeChangeListener (this);

    deleteAllChildren();
}

//==============================================================================
void BigClockEditor::paint (juce::Graphics& g)
{
    g.fillAll (getFilter()->bgcolor);
}

void BigClockEditor::resized()
{
    infoLabel->setBounds (0, 2 + ((showtextbox && hosttime) ? 4 : 0), getWidth() - 4, getHeight() - ((showtextbox && hosttime) ? 20 : 0));
    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
    textBox->setBounds (0, getHeight() - 16, getWidth() - 16, 16);
    cueLabel->setBounds (0, 0, getWidth(), 16);
    if (getFilter()->mode == StopwatchMode)
    {
        modeLabel->setBounds (getWidth() - 112, 0, 80, 16);
        runButton->setVisible (true);
        resetButton->setVisible (true);
        runButton->setBounds (getWidth() - 32, 0, 16, 16);
        resetButton->setBounds (getWidth() - 16, 0, 16, 16);
    }
    else
    {
        modeLabel->setBounds (getWidth() - 80, 0, 80, 16);
        runButton->setVisible (false);
        resetButton->setVisible (false);
    }

    textBox->setVisible (showtextbox && hosttime);
    cueLabel->setVisible (showtextbox && hosttime);

    getFilter()->lastUIWidth  = getWidth();
    getFilter()->lastUIHeight = getHeight();
}

void BigClockEditor::buttonStateChanged (juce::Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked->isDown())
    {
        juce::ModifierKeys mousebutton = juce::ModifierKeys::getCurrentModifiers();
        if (mousebutton.isPopupMenu())
        {
            bool samplemode = getFilter()->getParameter (kSamples) >= 0.5f;
            juce::PopupMenu m, sub1, sub2, sub3, sub4, clockmode;
            sub1.addCustomItem (-1, *colourSelector, 300, 300, false);
            m.addSubMenu (L"Color", sub1);
            m.addSeparator();
            clockmode.addItem (100, "Host Timeline", true, getFilter()->mode == HostTimeMode);
            clockmode.addItem (101, "Recording Time", true, getFilter()->mode == RecTimeMode);
            clockmode.addItem (102, "Stopwatch", true, getFilter()->mode == StopwatchMode);
            clockmode.addItem (103, "Plugin Time", true, getFilter()->mode == PluginTimeMode);
            clockmode.addItem (104, "Actual Time", true, getFilter()->mode == ActualTimeMode);
            m.addSubMenu ("Clock Mode", clockmode);
            m.addItem (120, "Reset", getFilter()->mode != HostTimeMode && getFilter()->mode != ActualTimeMode);
            m.addSeparator();
            sub4.addItem (998, L"Show Cues", true, showtextbox);
            sub4.addItem (996, L"Import...");
            sub4.addItem (997, L"Export...", getFilter()->cues.size() > 0);
            sub4.addItem (999, L"Clear", getFilter()->cues.size() > 0);
            for (int i = 0; i < getFilter()->cues.size(); i++)
            {
                sub4.addItem (1000 + i,
                              getFilter()->ppqToString (getFilter()->cues[i]->ppq,
                                                        getFilter()->lastPosInfo.timeSigNumerator,
                                                        getFilter()->lastPosInfo.timeSigDenominator,
                                                        getFilter()->lastPosInfo.bpm,
                                                        barsbeats)
                                  + " - " + getFilter()->cues[i]->text,
                              true,
                              getFilter()->cues[i]->enabled);
            }
            m.addSubMenu (L"Cues", sub4, getFilter()->mode == HostTimeMode);
            m.addSeparator();
            m.addItem (3, L"Bars|Beats|Ticks", true, barsbeats && ! samplemode);
            float ppqn = getFilter()->getParameter (kTicks);
            sub2.addItem (30, L"hide ticks", true, ppqn == 1.0);
            sub2.addItem (31, L"4", true, ppqn < 1.0 && ppqn >= 0.9);
            sub2.addItem (32, L"16", true, ppqn < 0.9 && ppqn >= 0.8);
            sub2.addItem (33, L"48", true, ppqn < 0.8 && ppqn >= 0.7);
            sub2.addItem (34, L"96", true, ppqn < 0.7 && ppqn >= 0.65);
            sub2.addItem (35, L"100", true, ppqn < 0.65 && ppqn >= 0.6);
            sub2.addItem (36, L"120", true, ppqn < 0.6 && ppqn >= 0.5);
            sub2.addItem (37, L"192", true, ppqn < 0.5 && ppqn >= 0.4);
            sub2.addItem (38, L"240", true, ppqn < 0.4 && ppqn >= 0.3);
            sub2.addItem (39, L"384", true, ppqn < 0.3 && ppqn >= 0.2);
            sub2.addItem (40, L"480", true, ppqn < 0.2 && ppqn >= 0.1);
            sub2.addItem (41, L"768", true, ppqn < 0.1 && ppqn > 0.0);
            sub2.addItem (42, L"960", true, ppqn == 0.0);
            m.addSubMenu (L"Ticks per beat", sub2, barsbeats && ! samplemode);
            m.addSeparator();
            m.addItem (4, L"Hour:Min:Sec", true, ! barsbeats && ! samplemode);
            float fps = getFilter()->getParameter (kFrames);
            sub3.addItem (50, L"ms (1000)", true, fps == 0.0);
            sub3.addItem (51, L"120", true, fps == 1.0);
            sub3.addItem (52, L"100", true, fps < 1.0 && fps >= 0.9);
            sub3.addItem (53, L"75", true, fps < 0.9 && fps >= 0.8);
            sub3.addItem (54, L"60", true, fps < 0.8 && fps >= 0.7);
            sub3.addItem (55, L"50", true, fps < 0.7 && fps >= 0.6);
            sub3.addItem (56, L"30", true, fps < 0.6 && fps >= 0.5);
            sub3.addItem (61, L"29.97 drop", true, fps < 0.5 && fps >= 0.4);
            sub3.addItem (57, L"25", true, fps < 0.4 && fps >= 0.3);
            sub3.addItem (58, L"24", true, fps < 0.3 && fps >= 0.2);
            sub3.addItem (59, L"15", true, fps < 0.2 && fps >= 0.1);
            sub3.addItem (60, L"10", true, fps < 0.1 && fps > 0.0);
            m.addSubMenu (L"Frames per second", sub3, ! barsbeats && ! samplemode);
            m.addItem (1, L"Show ms/frames", ! barsbeats && ! samplemode, getFilter()->getParameter (kShowms) >= 0.5);
            m.addItem (6, L"Show subframes", ! barsbeats && ! samplemode && getFilter()->getParameter (kShowms) >= 0.5, getFilter()->getParameter (kShowsubfr) >= 0.5);
            m.addItem (2, L"Show hours", ! barsbeats && ! samplemode, getFilter()->getParameter (kShowhrs) >= 0.5);
            m.addSeparator();
            m.addItem (5, L"Samples", true, samplemode);
            int result = m.show();
            if (result > 0)
            {
                if (result == 1)
                {
                    if (getFilter()->getParameter (kShowms) >= 0.5)
                        getFilter()->setParameter (kShowms, 0.0);
                    else
                        getFilter()->setParameter (kShowms, 1.0);
                }
                else if (result == 2)
                {
                    if (getFilter()->getParameter (kShowhrs) >= 0.5f)
                        getFilter()->setParameter (kShowhrs, 0.0f);
                    else
                        getFilter()->setParameter (kShowhrs, 1.0f);
                }
                else if (result == 3)
                {
                    if (barsbeats && ! samplemode)
                    {
                    }
                    else
                    {
                        getFilter()->setParameter (kBarsBeats, 1.0f);
                        getFilter()->setParameter (kSamples, 0.0f);
                    }
                }
                else if (result >= 30 && result < 50)
                {
                    if (result == 30)
                        getFilter()->setParameter (kTicks, 1.0f);
                    else if (result == 31)
                        getFilter()->setParameter (kTicks, 0.91f);
                    else if (result == 32)
                        getFilter()->setParameter (kTicks, 0.81f);
                    else if (result == 33)
                        getFilter()->setParameter (kTicks, 0.71f);
                    else if (result == 34)
                        getFilter()->setParameter (kTicks, 0.65f);
                    else if (result == 35)
                        getFilter()->setParameter (kTicks, 0.61f);
                    else if (result == 36)
                        getFilter()->setParameter (kTicks, 0.51f);
                    else if (result == 37)
                        getFilter()->setParameter (kTicks, 0.41f);
                    else if (result == 38)
                        getFilter()->setParameter (kTicks, 0.31f);
                    else if (result == 39)
                        getFilter()->setParameter (kTicks, 0.21f);
                    else if (result == 40)
                        getFilter()->setParameter (kTicks, 0.11f);
                    else if (result == 41)
                        getFilter()->setParameter (kTicks, 0.051f);
                    else if (result == 42)
                        getFilter()->setParameter (kTicks, 0.0f);
                }
                else if (result == 4)
                {
                    if (! barsbeats && ! samplemode)
                    {
                    }
                    else
                    {
                        getFilter()->setParameter (kBarsBeats, 0.0f);
                        getFilter()->setParameter (kSamples, 0.0f);
                    }
                }
                else if (result >= 50 && result < 70)
                {
                    if (result == 50)
                        getFilter()->setParameter (kFrames, 0.0f);
                    else if (result == 51)
                        getFilter()->setParameter (kFrames, 1.0f);
                    else if (result == 52)
                        getFilter()->setParameter (kFrames, 0.9f);
                    else if (result == 53)
                        getFilter()->setParameter (kFrames, 0.8f);
                    else if (result == 54)
                        getFilter()->setParameter (kFrames, 0.7f);
                    else if (result == 55)
                        getFilter()->setParameter (kFrames, 0.6f);
                    else if (result == 56)
                        getFilter()->setParameter (kFrames, 0.5f);
                    else if (result == 61)
                        getFilter()->setParameter (kFrames, 0.4f);
                    else if (result == 57)
                        getFilter()->setParameter (kFrames, 0.3f);
                    else if (result == 58)
                        getFilter()->setParameter (kFrames, 0.2f);
                    else if (result == 59)
                        getFilter()->setParameter (kFrames, 0.1f);
                    else if (result == 60)
                        getFilter()->setParameter (kFrames, 0.05f);
                }
                else if (result == 5)
                {
                    if (! samplemode)
                        getFilter()->setParameter (kSamples, 1.0f);
                }
                else if (result == 6)
                {
                    getFilter()->setParameter (kShowsubfr, getFilter()->getParameter (kShowsubfr) >= 0.5f ? 0.0f : 1.0f);
                }

                else if (result == 100)
                {
                    getFilter()->setParameter (kClockMode, 0.f);
                }
                else if (result == 101)
                {
                    getFilter()->setParameter (kClockMode, 1.f / (float) (numModes - 1));
                }
                else if (result == 102)
                {
                    getFilter()->setParameter (kClockMode, 2.f / (float) (numModes - 1));
                }
                else if (result == 103)
                {
                    getFilter()->setParameter (kClockMode, 3.f / (float) (numModes - 1));
                }
                else if (result == 104)
                {
                    getFilter()->setParameter (kClockMode, 4.f / (float) (numModes - 1));
                }
                else if (result == 120)
                {
                    if (getFilter()->mode == StopwatchMode)
                        getFilter()->setParameter (kClearWatch, 1.f);
                    else if (getFilter()->mode == RecTimeMode)
                        getFilter()->rectime = 0;
                    else if (getFilter()->mode == PluginTimeMode)
                        getFilter()->plugintime = juce::Time::getMillisecondCounter();
                }

                else if (result == 998)
                {
                    getFilter()->showcues = ! showtextbox;
                    updateParametersFromFilter();
                    resized();
                    repaint();
                }
                else if (result == 996)
                {
                    juce::FileChooser myChooser ("Import cue file...",
                                                 juce::File (getFilter()->getCurrentPath()),
                                                 "*.xml");

                    if (myChooser.browseForFileToOpen())
                    {
                        juce::File file (myChooser.getResult());
                        getFilter()->loadCues (file);
                    }
                }
                else if (result == 997)
                {
                    juce::FileChooser myChooser ("Export cue file...",
                                                 juce::File (getFilter()->getCurrentPath() + juce::File::getSeparatorString() + "cues.xml"));

                    if (myChooser.browseForFileToSave (true))
                    {
                        juce::File cuefile (myChooser.getResult());
                        if (! cuefile.hasFileExtension ("xml"))
                            cuefile = cuefile.withFileExtension ("xml");

                        getFilter()->saveCues (cuefile);
                    }
                }
                else if (result == 999)
                {
                    getFilter()->cues.clear();
                    cueLabel->setText (juce::String(), juce::dontSendNotification);
                }
                else if (result >= 1000)
                {
                    getFilter()->setCueEnabled (result - 1000, ! (getFilter()->cues[result - 1000]->enabled));
                    updateParametersFromFilter();
                }
            }
        }
    }
}
void BigClockEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    juce::ModifierKeys mousebutton = juce::ModifierKeys::getCurrentModifiers();
    if (! mousebutton.isPopupMenu())
    {
        if (buttonThatWasClicked == runButton)
        {
            getFilter()->setParameter (kRunWatch, 1.f - getFilter()->getParameter (kRunWatch));
        }
        else if (buttonThatWasClicked == resetButton)
        {
            getFilter()->setParameter (kClearWatch, 1.f);
        }
        else if (buttonThatWasClicked == infoLabel)
        {
            bool samplemode = getFilter()->getParameter (kSamples) >= 0.5f;
            if (barsbeats && ! samplemode)
            {
                barsbeats = false;
                getFilter()->setParameter (kBarsBeats, 0.0);
                getFilter()->setParameter (kSamples, 0.0);
            }
            else if (! barsbeats && ! samplemode)
            {
                getFilter()->setParameter (kSamples, 1.0);
            }
            else if (samplemode)
            {
                barsbeats = true;
                getFilter()->setParameter (kBarsBeats, 1.0);
                getFilter()->setParameter (kSamples, 0.0);
            }
            updateParametersFromFilter();
        }
    }
}

void BigClockEditor::textEditorReturnKeyPressed (juce::TextEditor& editor)
{
    if (textBox->getText().isNotEmpty())
    {
        getFilter()->addCue (getFilter()->lastPosInfo.ppqPosition * 60.0 / getFilter()->lastPosInfo.bpm, getFilter()->lastPosInfo.ppqPosition, textBox->getText());
        textBox->clear();
    }
}

//==============================================================================

void BigClockEditor::timerCallback()
{
    if (getFilter()->mode == ActualTimeMode || getFilter()->mode == PluginTimeMode || getFilter()->mode == StopwatchMode)
        updateParametersFromFilter();
}

void BigClockEditor::mouseEnter (const juce::MouseEvent& e)
{
    modeLabel->setVisible (true);
}

void BigClockEditor::mouseExit (const juce::MouseEvent& e)
{
    modeLabel->setVisible (false);
}

void BigClockEditor::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (source == getFilter())
    {
        updateParametersFromFilter();
    }
    else
    {
        auto* cs = (juce::ColourSelector*) source;
        if (cs)
        {
            getFilter()->bgcolor = (cs->getCurrentColour());
            infoLabel->textcolor = cs->getCurrentColour().contrasting (getFilter()->getParameter (kLook));
            repaint();
        }
    }
}

//==============================================================================
void BigClockEditor::updateParametersFromFilter()
{
    BigClockFilter* const filter = getFilter();

    filter->getCallbackLock().enter();
    const juce::AudioPlayHead::CurrentPositionInfo positionInfo (filter->lastPosInfo);
    barsbeats               = filter->getParameter (kBarsBeats) >= 0.5f;
    const float clockmode   = filter->getParameter (kClockMode);
    const bool watchrunning = filter->getParameter (kRunWatch) >= 0.5f;
    showtextbox             = filter->showcues;
    filter->getCallbackLock().exit();

    recording = positionInfo.isRecording;

    switch (roundToInt (clockmode * (float) (numModes - 1)))
    {
        case HostTimeMode:
            hosttime = true;
            modeLabel->setText ("Host Timeline", juce::dontSendNotification);
            infoLabel->time = filter->ppqToString (positionInfo.ppqPosition,
                                                   positionInfo.timeSigNumerator,
                                                   positionInfo.timeSigDenominator,
                                                   positionInfo.bpm,
                                                   barsbeats);
            break;
        case RecTimeMode:
            hosttime = false;
            modeLabel->setText ("Recording Time", juce::dontSendNotification);
            infoLabel->time = filter->ppqToString (filter->secondsToPpq (filter->rectime, positionInfo.bpm),
                                                   positionInfo.timeSigNumerator,
                                                   positionInfo.timeSigDenominator,
                                                   positionInfo.bpm,
                                                   barsbeats);
            break;
        case StopwatchMode:
            hosttime = false;
            modeLabel->setText ("Stopwatch", juce::dontSendNotification);
            infoLabel->time = filter->ppqToString (filter->secondsToPpq ((double) (filter->watchtime) * 0.001, positionInfo.bpm),
                                                   positionInfo.timeSigNumerator,
                                                   positionInfo.timeSigDenominator,
                                                   positionInfo.bpm,
                                                   barsbeats);
            break;
        case PluginTimeMode:
            hosttime = false;
            modeLabel->setText ("Plugin Time", juce::dontSendNotification);
            infoLabel->time = filter->ppqToString (filter->secondsToPpq ((double) (juce::Time::getMillisecondCounter() - filter->plugintime) * 0.001, positionInfo.bpm),
                                                   positionInfo.timeSigNumerator,
                                                   positionInfo.timeSigDenominator,
                                                   positionInfo.bpm,
                                                   barsbeats);
            break;
        case ActualTimeMode:
            hosttime = false;
            modeLabel->setText ("Actual Time", juce::dontSendNotification);
            infoLabel->time = juce::Time::getCurrentTime().toString (false, true);
            break;
        default:
            break;
    }

    infoLabel->textcolor = filter->bgcolor.contrasting (filter->getParameter (kLook));
    infoLabel->repaint();

    modeLabel->setColour (juce::Label::textColourId, infoLabel->textcolor);

    runButton->setButtonText (watchrunning ? "||" : ">");

    if (showtextbox)
    {
        cueLabel->setColour (juce::Label::textColourId, infoLabel->textcolor);
        cueLabel->setText (filter->getCue (positionInfo.ppqPosition, barsbeats), juce::sendNotification);
    }

    setSize (filter->lastUIWidth,
             filter->lastUIHeight);
    resized();
}
