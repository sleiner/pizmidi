
#include "MidiMorphGUI.h"
#include "ControllerList.h"
#include "CursorGUI.h"
#include "D3CKLook.h"
#include "MidiMorph.h"
#include "MidiMorphPlugInInterface.h"
#include "MorphPane.h"
#include "MorphPaneModel.h"

MidiMorphGUI::MidiMorphGUI (juce::AudioProcessor* const ownerPlugIn)
    : AudioProcessorEditor (ownerPlugIn)
{
    juce::LookAndFeel::setDefaultLookAndFeel (new D3CKLook());
    this->setMouseClickGrabsKeyboardFocus (false);

    this->ownerFilter = ownerPlugIn;

    core = &((MidiMorphPlugInInterface*) ownerPlugIn)->core;
    core->addChangeListener (this);

    int w = core->guiBounds.getWidth();
    int h = core->guiBounds.getHeight();

    morphPaneModel = new MorphPaneModel (core);

    addAndMakeVisible (morphPane = new MorphPane (morphPaneModel, core));
    morphPane->addZoomedComp (new CursorGUI (core->getCursor()));
    morphPane->setMouseClickGrabsKeyboardFocus (false);

    controllerListModel = new ControllerList (core);

    addAndMakeVisible (controllerList = new juce::ListBox ("controls", controllerListModel));
    controllerList->setRowHeight (24);
    controllerList->getVerticalScrollBar().setAutoHide (false);
    controllerList->setMouseClickGrabsKeyboardFocus (false);

    this->addAndMakeVisible (resizer = new juce::ResizableBorderComponent (this, 0));
    resizer->setMouseClickGrabsKeyboardFocus (false);

    this->morphPane->setFromXml (core->getSavedGUIState());

    this->setSize (w, h);
}

MidiMorphGUI::~MidiMorphGUI()
{
    core->saveGUIState (morphPane->getXml ("morphpanestate"));
    core->removeChangeListener (this);

    deleteAndZero (morphPane);
    deleteAndZero (controllerList);
    deleteAndZero (resizer);

    deleteAndZero (morphPaneModel);
    deleteAndZero (controllerListModel);
}

juce::ApplicationCommandTarget* MidiMorphGUI::getNextCommandTarget()
{
    return nullptr;
}

void MidiMorphGUI::resized()
{
    this->resizer->setBounds (0, 0, getWidth(), getHeight());
    this->controllerList->setBounds (0, 0, core->controllerListWidth, getHeight());
    this->morphPane->setBounds (controllerList->getWidth(), 0, getWidth() - core->controllerListWidth, getHeight());
    core->paneSize.setSize (morphPane->getWidth(), morphPane->getHeight());
    core->guiBounds.setSize (getWidth(), getHeight());
}

void MidiMorphGUI::getAllCommands (juce::Array<juce::CommandID>& commands)
{
}

void MidiMorphGUI::getCommandInfo (const juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
}

bool MidiMorphGUI::perform (const InvocationInfo& info)
{
    return true;
}

void MidiMorphGUI::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    void* objectThatHasChanged = source;
    if (core->currentChangeBroadcastRecipient == core->getSelectedScenes())
    {
        this->controllerListModel->scenesSelected();
    }
    else if (core->currentChangeBroadcastRecipient == core->getCursor())
    {
        controllerListModel->distancesChanged();
        core->saveGUIState (morphPane->getXml ("morphpanestate"));
    }
    else if (core->currentChangeBroadcastRecipient == core->getScenes())
    {
        morphPane->updateContent();
        if (core->scenes.size() > 0)
        {
            morphPane->selectModule (core->scenes.size() - 1, true);
        }
    }
    else if (core->currentChangeBroadcastRecipient == core->getControllers())
    {
        controllerList->updateContent();
    }
    else if (objectThatHasChanged == morphPaneModel)
    {
        this->resized();
    }
    repaint();
}

void MidiMorphGUI::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::white);
    g.fillRect (0, 0, getWidth(), getHeight());
}
