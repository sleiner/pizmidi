#include "imagePluginEditor.h"

using juce::jmin;
using juce::roundToInt;

//==============================================================================
imagePluginEditor::imagePluginEditor (imagePluginFilter* const ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      bankSlider (nullptr),
      progSlider (nullptr),
      textEditor (0)
{
    setMouseClickGrabsKeyboardFocus (false);

    addAndMakeVisible (container = new Component());

    container->addAndMakeVisible (imagepad = new MidiPad());
    imagepad->setTriggeredOnMouseDown (true);
    imagepad->addListener (this);
    imagepad->addMouseListener (this, true);
    imagepad->addKeyListener (this);

    textEditor = new juce::TextEditor ("text editor");
    textEditor->setMultiLine (true);
    textEditor->setReturnKeyStartsNewLine (true);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (false);
    textEditor->setSelectAllWhenFocused (true);
    textEditor->addListener (this);
    textEditor->setText (getFilter()->text);
    textEditor->setColour (juce::TextEditor::outlineColourId, juce::Colours::black);

    bankSlider = new juce::Slider ("bank");
    bankSlider->setRange (0, 127, 1);
    bankSlider->setSliderStyle (juce::Slider::IncDecButtons);
    bankSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, 20);
    bankSlider->addListener (this);

    progSlider = new juce::Slider ("program");
    progSlider->setRange (0, 127, 1);
    progSlider->setSliderStyle (juce::Slider::IncDecButtons);
    progSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, 20);
    progSlider->addListener (this);

    chanSlider = new ChannelSlider ("channel");
    chanSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, 20);
    chanSlider->addListener (this);

    container->addAndMakeVisible (label = new juce::Label ("new label",
                                                           "Insert Piz Here->  image v" + juce::String (JucePlugin_VersionString) + "\n\n- Double-click to toggle full-screen\n- Drag & drop to load images (png/jpg/gif/svg)\n- Supports MIDI Program Change"));
    label->setFont (juce::Font (15.0000f, juce::Font::plain));
    label->setJustificationType (juce::Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (juce::Label::backgroundColourId, juce::Colour (0xc7f3ef9f));
    label->setColour (juce::Label::textColourId, juce::Colours::black);
    label->setColour (juce::Label::outlineColourId, juce::Colours::black);
    label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x0));
    label->setMouseClickGrabsKeyboardFocus (false);
    label->addMouseListener (this, false);
    label->setVisible (false);

    colourSelector = new juce::ColourSelector (juce::ColourSelector::showColourAtTop | juce::ColourSelector::showSliders | juce::ColourSelector::showColourspace);
    colourSelector->setName (juce::String ("color"));
    colourSelector->setCurrentColour (getFilter()->bgcolor);
    colourSelector->addChangeListener (this);

    textColourSelector = new juce::ColourSelector (juce::ColourSelector::showColourAtTop | juce::ColourSelector::showSliders | juce::ColourSelector::showColourspace);
    textColourSelector->setName (juce::String ("textcolor"));
    textColourSelector->setCurrentColour (getFilter()->textcolor);
    textColourSelector->addChangeListener (this);

    // add the triangular resizer component for the bottom-right of the UI
    container->addAndMakeVisible (resizer = new juce::ResizableCornerComponent (this, &resizeLimits));
    resizer->setMouseClickGrabsKeyboardFocus (false);
    resizeLimits.setSizeLimits (50, 50, 1600, 1600);

    container->setMouseClickGrabsKeyboardFocus (false);
    container->addChildComponent (textEditor);
    container->addChildComponent (colourSelector);
    container->addChildComponent (textColourSelector);
    fullscreen = false;

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize (ownerFilter->lastUIWidth,
             ownerFilter->lastUIHeight);
    resized();

    // register ourselves with the filter - it will use its ChangeBroadcaster base
    // class to tell us when something has changed, and this will call our changeListenerCallback()
    // method.
    ownerFilter->addChangeListener (this);
    updateParametersFromFilter();
}

imagePluginEditor::~imagePluginEditor()
{
    getFilter()->removeChangeListener (this);
    getFilter()->icon = imagepad->getIconPath();
    if (container->isOnDesktop())
        container->removeFromDesktop();
    if (label)
        deleteAndZero (label);
    if (textEditor)
        deleteAndZero (textEditor);
    if (bankSlider)
        deleteAndZero (bankSlider);
    if (progSlider)
        deleteAndZero (progSlider);
    if (imagepad)
        deleteAndZero (imagepad);
    if (colourSelector)
        deleteAndZero (colourSelector);
    if (textColourSelector)
        deleteAndZero (textColourSelector);
    if (resizer)
        deleteAndZero (resizer);
    if (container)
        deleteAndZero (container);
}

//==============================================================================
void imagePluginEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0x00000000));
}

void imagePluginEditor::resized()
{
    if (! fullscreen)
    {
        container->setBounds (0, 0, proportionOfWidth (1.0f), proportionOfHeight (1.0f));
        resizer->setVisible (true);
    }
    else
    {
        auto* display = juce::Desktop::getInstance().getDisplays().getDisplayForPoint (juce::Point<int> (container->getScreenX(), container->getScreenY()), false);
        if (display != nullptr)
        {
            container->setBounds (display->userArea);
        }
        resizer->setVisible (false);
    }

    imagepad->setBounds (container->getBounds());
    imagepad->repaint();

    label->setBounds (roundToInt (container->getWidth() * (0.5000f)) - ((307) / 2),
                      roundToInt (container->getHeight() * (0.5000f)) - ((96) / 2),
                      307,
                      96);

    int resizersize = jmin (container->proportionOfWidth (0.05f), container->proportionOfHeight (0.05f));
    if (resizersize < 12)
        resizersize = 12;
    resizer->setBounds (container->getWidth() - resizersize, container->getHeight() - resizersize, resizersize, resizersize);

    getFilter()->lastUIWidth  = getWidth();
    getFilter()->lastUIHeight = getHeight();
}

void imagePluginEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
}

bool imagePluginEditor::keyPressed (const juce::KeyPress& key, Component* originatingComponent)
{
    int prog = getFilter()->getCurrentProgram();
    if (key.isKeyCode (juce::KeyPress::pageUpKey))
    {
        prog++;
        if (prog == getFilter()->getNumPrograms())
            prog = 0;
        getFilter()->setCurrentProgram (prog);
        getFilter()->updateHostDisplay();
        return true;
    }
    else if (key.isKeyCode (juce::KeyPress::pageDownKey))
    {
        if (prog == 0)
            prog = getFilter()->getNumPrograms();
        prog--;
        getFilter()->setCurrentProgram (prog);
        getFilter()->updateHostDisplay();
        return true;
    }
    return false;
}

void imagePluginEditor::buttonStateChanged (juce::Button* buttonThatWasClicked)
{
    getFilter()->icon = imagepad->getIconPath();
    if (imagepad->isDown())
    {
        juce::ModifierKeys mousebutton = juce::ModifierKeys::getCurrentModifiers();
        if (mousebutton.isPopupMenu())
        {
            juce::PopupMenu m, sub1, sub2, sub3, subB, subP, subC;
            m.addSectionHeader ("Text:");
            m.addCustomItem (1234, *textEditor, 200, 72, false);
            m.addSeparator();

            m.addItem (99999, "Load Image...");
            m.addItem (66, "Clear Image");
            sub1.addCustomItem (1234, *colourSelector, 300, 300, false);
            m.addSubMenu ("Background Color", sub1);
            sub2.addCustomItem (1234, *textColourSelector, 300, 300, false);
            m.addSubMenu ("Text Color", sub2);
            m.addSeparator();
            subB.addCustomItem (-1, *bankSlider, 100, 16, false);
            m.addSubMenu ("Bank", subB);
            subP.addCustomItem (-1, *progSlider, 100, 16, false);
            m.addSubMenu ("Program", subP);
            m.addSeparator();
            m.addItem (71, "Use PC/Bank Input", true, getFilter()->getUsePC());
            m.addItem (70, "Use Note Input", true, getFilter()->getNoteInput());
            subC.addCustomItem (-1, *chanSlider, 140, 16, false);
            m.addSubMenu ("Input Channel", subC);
            m.addSeparator();
            m.addItem (88, "Clear All Images");
            m.addItem (76, "Apply Colors to Bank");
            m.addItem (77, "Apply Size to Bank", ! fullscreen);
            m.addSeparator();
            m.addItem (99, "About");

            int result = m.show();
            if (result > 0)
            {
                if (result == 66)
                {
                    getFilter()->icon = juce::String ("");
                    imagepad->setImages (0);
                    imagepad->setIconPath ("");
                    imagepad->repaint();
                }
                else if (result == 99999)
                {
                    juce::FileChooser myChooser ("Load image...",
                                                 juce::File (getFilter()->iconPath),
                                                 "*.png;*.gif;*.svg;*.jpg");

                    if (myChooser.browseForFileToOpen())
                    {
                        juce::File file (myChooser.getResult());
                        if (imagepad->setImageFromFile (file))
                        {
                            //save the relative path
                            imagepad->setIconPath (file.getRelativePathFrom (juce::File (getFilter()->iconPath)));
                            getFilter()->icon = imagepad->getIconPath();
                        }
                    }
                }
                else if (result == 76)
                {
                    getFilter()->setBankColours (colourSelector->getCurrentColour(), textColourSelector->getCurrentColour());
                }
                else if (result == 77)
                {
                    getFilter()->applySizeToBank (getHeight(), getWidth());
                }
                else if (result == 88)
                {
                    getFilter()->clearAllImages();
                    imagepad->setImages (0);
                    imagepad->setIconPath ("");
                    imagepad->repaint();
                }
                else if (result == 99)
                {
                    label->setVisible (true);
                }
                else if (result == 70)
                {
                    getFilter()->setNoteInput (! getFilter()->getNoteInput());
                }
                else if (result == 71)
                {
                    getFilter()->setUsePC (! getFilter()->getUsePC());
                }
            }
        }
    }
}

void imagePluginEditor::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    if (sliderThatWasMoved == bankSlider)
    {
        getFilter()->setCurrentBank ((int) bankSlider->getValue());
        getFilter()->updateHostDisplay();
    }
    else if (sliderThatWasMoved == progSlider)
    {
        getFilter()->setCurrentProgram ((int) progSlider->getValue());
        getFilter()->updateHostDisplay();
    }
    else if (sliderThatWasMoved == chanSlider)
    {
        getFilter()->setParameterNotifyingHost (kChannel, (float) (chanSlider->getValue() * 0.0625));
        getFilter()->updateHostDisplay();
    }
}

void imagePluginEditor::mouseDown (const juce::MouseEvent& e)
{
    if (e.eventComponent == label)
        label->setVisible (false);
}

void imagePluginEditor::mouseDoubleClick (const juce::MouseEvent& e)
{
    if (! fullscreen)
    {
        container->addToDesktop (0);
        juce::Desktop::getInstance().setKioskModeComponent (container, false);
        container->enterModalState (false);
        fullscreen = true;
        resized();
    }
    else
    {
        container->exitModalState (0);
        juce::Desktop::getInstance().setKioskModeComponent (0);
        this->addChildComponent (container);
        fullscreen = false;
        resized();
    }
    getFilter()->fullscreen = fullscreen;
}

void imagePluginEditor::textEditorTextChanged (juce::TextEditor& editor)
{
    if (&editor == textEditor)
    {
        //imagepad->drawableButton->Label = textEditor->getText(),false;
        imagepad->setButtonText (textEditor->getText());
        imagepad->repaint();
    }
}

void imagePluginEditor::textEditorReturnKeyPressed (juce::TextEditor& editor)
{
    if (&editor == textEditor)
    {
        //imagepad->drawableButton->Label = textEditor->getText(),false;
        imagepad->setButtonText (textEditor->getText());
        imagepad->repaint();
        getFilter()->text = textEditor->getText();
        juce::PopupMenu::dismissAllActiveMenus();
    }
}

void imagePluginEditor::textEditorEscapeKeyPressed (juce::TextEditor& editor)
{
    juce::PopupMenu::dismissAllActiveMenus();
    textEditor->setText (getFilter()->text);
    imagepad->repaint();
}

void imagePluginEditor::textEditorFocusLost (juce::TextEditor& editor)
{
    getFilter()->text = textEditor->getText();
}

bool imagePluginEditor::isInterestedInFileDrag (const juce::StringArray& files)
{
    juce::File file = juce::File (files.joinIntoString (juce::String(), 0, 1));
    if (file.hasFileExtension ("png") || file.hasFileExtension ("gif") || file.hasFileExtension ("jpg") || file.hasFileExtension ("svg"))
        return true;
    return false;
}

void imagePluginEditor::filesDropped (const juce::StringArray& filenames, int mouseX, int mouseY)
{
    if (isInterestedInFileDrag (filenames))
    {
        juce::String filename = filenames.joinIntoString (juce::String(), 0, 1);
        juce::File file       = juce::File (filename);
        if (imagepad->setImageFromFile (file))
        {
            //save the relative path
            imagepad->setIconPath (file.getRelativePathFrom (juce::File (getFilter()->iconPath)));
            getFilter()->icon = imagepad->getIconPath();
        }
    }
}

//==============================================================================
void imagePluginEditor::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (source == getFilter())
    {
        updateParametersFromFilter();
    }
    else if (source == colourSelector)
    {
        auto* cs = (juce::ColourSelector*) source;
        imagepad->setColour (cs->getCurrentColour());
        getFilter()->bgcolor = cs->getCurrentColour();
    }
    else if (source == textColourSelector)
    {
        auto* cs = (juce::ColourSelector*) source;
        imagepad->setTextColour (cs->getCurrentColour());
        getFilter()->textcolor = cs->getCurrentColour();
    }
    //else if (source==bankColourSelector) {
    //       ColourSelector* cs = (ColourSelector*) source;
    //       imagepad->setColour(cs->getCurrentColour());
    //	getFilter()->setBankColours(cs->getCurrentColour(),textColourSelector->getCurrentColour());
    //   }
}

//==============================================================================
void imagePluginEditor::updateParametersFromFilter()
{
    imagePluginFilter* const filter = getFilter();
    // we use this lock to make sure the processBlock() method isn't writing to the
    // lastMidiMessage variable while we're trying to read it, but be extra-careful to
    // only hold the lock for a minimum amount of time
    filter->getCallbackLock().enter();

    // take a local copy of the info we need while we've got the lock

    const juce::String t         = filter->text;
    const juce::String icon      = filter->icon;
    const juce::Colour bgcolor   = filter->bgcolor;
    const juce::Colour textcolor = filter->textcolor;
    const int bank               = filter->getCurrentBank();
    const int prog               = filter->getCurrentProgram();
    const float chan             = filter->getParameter (kChannel);

    // ..release the lock ASAP
    filter->getCallbackLock().exit();

    // ..and after releasing the lock, we're free to do the time-consuming UI stuff..

    if (icon.isEmpty())
    {
        imagepad->setImages (nullptr);
        imagepad->setIconPath (juce::String());
    }
    else
    {
        juce::String fullpath = icon;
        if (! juce::File::getCurrentWorkingDirectory().getChildFile (fullpath).existsAsFile())
            fullpath = getFilter()->iconPath + juce::File::getSeparatorString() + icon;
        if (! imagepad->setImageFromFile (juce::File::getCurrentWorkingDirectory().getChildFile (fullpath)))
            imagepad->setImages (nullptr);
        imagepad->setIconPath (icon);
    }

    imagepad->setButtonText (t);
    textEditor->setText (t, false);
    imagepad->setColour (bgcolor);
    imagepad->setTextColour (textcolor);
    colourSelector->setCurrentColour (bgcolor);
    textColourSelector->setCurrentColour (textcolor);
    bankSlider->setValue (bank, juce::dontSendNotification);
    progSlider->setValue (prog, juce::dontSendNotification);
    chanSlider->setValue (chan * 16.0, juce::dontSendNotification);

    setSize (filter->lastUIWidth,
             filter->lastUIHeight);
    resized();
    imagepad->repaint();
}
