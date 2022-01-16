
#include "MidiMorphGUI.h"
#include "ControllerList.h"
#include "CursorGUI.h"
#include "D3CKLook.h"
#include "MidiMorph.h"
#include "MidiMorphPlugInInterface.h"
#include "MorphPane.h"
#include "MorphPaneModel.h"

MidiMorphGUI::MidiMorphGUI (AudioProcessor* const ownerPlugIn)
    : AudioProcessorEditor (ownerPlugIn)
{
    // Bouml preserved body begin 0002E80D

    LookAndFeel::setDefaultLookAndFeel (new D3CKLook());
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

    addAndMakeVisible (controllerList = new ListBox ("controls", controllerListModel));
    controllerList->setRowHeight (24);
    controllerList->getVerticalScrollBar().setAutoHide (false);
    controllerList->setMouseClickGrabsKeyboardFocus (false);

    this->addAndMakeVisible (resizer = new ResizableBorderComponent (this, 0));
    resizer->setMouseClickGrabsKeyboardFocus (false);

    this->morphPane->setFromXml (core->getSavedGUIState());

    //this->setWantsKeyboardFocus(true);

    //this->grabKeyboardFocus();

    this->setSize (w, h);

    // Bouml preserved body end 0002E80D
}

MidiMorphGUI::~MidiMorphGUI()
{
    // Bouml preserved body begin 0003188D
    core->saveGUIState (morphPane->getXml ("morphpanestate"));
    core->removeChangeListener (this);

    deleteAndZero (morphPane);
    //deleteAndZero (sceneOptionsBar);
    deleteAndZero (controllerList);
    deleteAndZero (resizer);

    deleteAndZero (morphPaneModel);
    //deleteAndZero(controllerAdd);
    deleteAndZero (controllerListModel);
    // Bouml preserved body end 0003188D
}

//ApplicationCommandTarget *  getNextCommandTarget
ApplicationCommandTarget* MidiMorphGUI::getNextCommandTarget()
{
    // Bouml preserved body begin 0003058D
    return 0;
    // Bouml preserved body end 0003058D
}

void MidiMorphGUI::resized()
{
    // Bouml preserved body begin 0002F30D
    this->resizer->setBounds (0, 0, getWidth(), getHeight());
    this->controllerList->setBounds (0, 0, core->controllerListWidth, getHeight());
    this->morphPane->setBounds (controllerList->getWidth(), 0, getWidth() - core->controllerListWidth, getHeight());
    core->paneSize.setSize (morphPane->getWidth(), morphPane->getHeight());
    core->guiBounds.setSize (getWidth(), getHeight());
    // Bouml preserved body end 0002F30D
}

//void  getAllCommands (Array< CommandID > &commands)=0
void MidiMorphGUI::getAllCommands (Array<CommandID>& commands)
{
    // Bouml preserved body begin 0003060D
    // Bouml preserved body end 0003060D
}

//void  getCommandInfo (const CommandID commandID, ApplicationCommandInfo &result)=
void MidiMorphGUI::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
    // Bouml preserved body begin 0003068D
    // Bouml preserved body end 0003068D
}

//perform ()=0
bool MidiMorphGUI::perform (const InvocationInfo& info)
{
    // Bouml preserved body begin 0003070D
    return true;
    // Bouml preserved body end 0003070D
}

void MidiMorphGUI::changeListenerCallback (ChangeBroadcaster* source)
{
    // Bouml preserved body begin 0003350D
    void* objectThatHasChanged = source;
    OwnedArray<Scene>* scenes  = core->getScenes();
    if (objectThatHasChanged == core->getSelectedScenes())
    {
        this->controllerListModel->scenesSelected();
        //morphPane->setSelectedScenes(
    }
    else if (objectThatHasChanged == core->getCursor())
    {
        controllerListModel->distancesChanged();
        core->saveGUIState (morphPane->getXml ("morphpanestate"));
        //    	ownerFilter->setParameterNotifyingHost(0,core->getCursorXRatio());
        //    	ownerFilter->setParameterNotifyingHost(1,core->getCursorYRatio());
    }
    else if (objectThatHasChanged == core->getScenes())
    {
        morphPane->updateContent();
        if (core->scenes.size() > 0)
        {
            morphPane->selectModule (core->scenes.size() - 1, true);
        }
    }
    else if (objectThatHasChanged == core->getControllers())
    {
        controllerList->updateContent();
    }
    //	else
    //	{
    //		repaint();
    //	}
    else if (objectThatHasChanged == morphPaneModel)
    {
        this->resized();
    }
    repaint();
    // Bouml preserved body end 0003350D
}

//==============================================================================

void MidiMorphGUI::paint (Graphics& g)
{
    // Bouml preserved body begin 00040A0D
    g.setColour (Colours::white);
    g.fillRect (0, 0, getWidth(), getHeight());
    // Bouml preserved body end 00040A0D
}
