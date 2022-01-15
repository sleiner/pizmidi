
#include "ModulePane.h"
#include "Module.h"
#include "ModuleGUI.h"
#include "ModulePaneModel.h"
//#include "D3CKStdCommands.h"

ModulePane::ModulePane (ModulePaneModel* modulePaneModel)
{
    // Bouml preserved body begin 0003C18D
    this->model = modulePaneModel;
    this->model->setOwnerPane (this);
    this->updateContent();
    this->selectedModules.addChangeListener (this);
    this->addAndMakeVisible (&lasso);
    this->setMouseClickGrabsKeyboardFocus (false);
    this->setInterceptsMouseClicks (true, true);
    // Bouml preserved body end 0003C18D
}

ModulePane::~ModulePane()
{
    // Bouml preserved body begin 0004208D
    selectedModules.removeChangeListener (this);
    modules.clear();
    // Bouml preserved body end 0004208D
}

//drag the origin around
//call the reArrangeChildren
void ModulePane::mouseDrag (const MouseEvent& e)
{
    // Bouml preserved body begin 0003B78D
    model->mouseDrag (e);
    if (e.mods.isMiddleButtonDown()) // || e.mods.isShiftDown())
    {
        dragOrigin (e);
    }
    else if (e.mods.isLeftButtonDown() && (e.mods.isAltDown() || e.mods.isCtrlDown()))
    {
        this->lasso.dragLasso (e);
    }
    // Bouml preserved body end 0003B78D
}

void ModulePane::mouseDown (const MouseEvent& e)
{
    // Bouml preserved body begin 0003B98D

    model->mouseDown (e);

    if (e.mods.isMiddleButtonDown()) // || e.mods.isShiftDown())
    {
        setMouseCursor (MouseCursor::DraggingHandCursor);
        startDragOrigin (e);
    }
    else if (e.mods.isLeftButtonDown() && (e.mods.isAltDown() || e.mods.isCtrlDown()))
    {
        if (! e.mods.isCtrlDown())
        {
            this->selectedModules.deselectAll();
        }
        this->lasso.beginLasso (e, this);
    }
    // Bouml preserved body end 0003B98D
}

void ModulePane::mouseUp (const MouseEvent& e)
{
    // Bouml preserved body begin 0003BA0D

    lasso.endLasso();
    model->mouseUp (e);

    setMouseCursor (MouseCursor::NormalCursor);
    // Bouml preserved body end 0003BA0D
}

void ModulePane::mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY)
{
    // Bouml preserved body begin 0003E18D
    float factor = 1 + wheelIncrementY;
    this->zoom (factor, factor, (float) e.x, (float) e.y);
    // Bouml preserved body end 0003E18D
}

void ModulePane::updateContent()
{
    // Bouml preserved body begin 0003D70D
    this->selectedModules.deselectAll();
    while (modules.size() > 0)
    {
        ModuleGUI* oldGUI = modules.removeAndReturn (0);
        removeZoomedComp (oldGUI);
    }
    //this->zoomedComponents.clear();
    for (int i = 0; i < model->getNumModules(); i++)
    {
        ModuleGUI* gui = model->createGUI (i);
        if (gui == 0)
        {
            gui = new ModuleGUI (model->getModule (i));
        }
        modules.add (gui);
        gui->setPane (this);
        addZoomedComp (gui);

        //zoomedComponents.add(gui);
    }
    rePositionChildren();
    // Bouml preserved body end 0003D70D
}

bool ModulePane::isModuleSelected (const Module* module)
{
    // Bouml preserved body begin 0003E30D
    return true;
    // Bouml preserved body end 0003E30D
}

//==============================================================================

void ModulePane::paintOverChildren (Graphics& g)
{
    // Bouml preserved body begin 0003E38D
    g.drawRoundedRectangle (0.f, 0.f, (float) getWidth(), (float) getHeight(), 2.f, 2.f);
    // Bouml preserved body end 0003E38D
}

void ModulePane::changeListenerCallback (ChangeBroadcaster* source)
{
    // Bouml preserved body begin 0003E48D
    void* objectThatHasChanged = source;
    if (objectThatHasChanged == &selectedModules)
    {
        repaint();
        model->selectionChanged (selectedModules.getItemArray());
    }
    // Bouml preserved body end 0003E48D
}

//  getLassoSelection
SelectedItemSet<ModuleGUI*>& ModulePane::getLassoSelection()
{
    // Bouml preserved body begin 0003E50D
    return this->selectedModules;
    // Bouml preserved body end 0003E50D
}

void ModulePane::findLassoItemsInArea (Array<ModuleGUI*>& itemsFound, const juce::Rectangle<int>& area)
{
    // Bouml preserved body begin 0003E58D
    for (int i = 0; i < this->modules.size(); i++)
    {
        ModuleGUI* current = modules[i];
        if (lasso.getBounds().contains (current->getBounds()))
        {
            itemsFound.add (current);
        }
    }
    // Bouml preserved body end 0003E58D
}

void ModulePane::selectModule (int index, bool deselectOthers)
{
    // Bouml preserved body begin 00047D8D
    if (deselectOthers)
    {
        selectedModules.deselectAll();
    }
    this->selectedModules.addToSelection (modules[index]);
    // Bouml preserved body end 00047D8D
}

//perform ()=0
bool ModulePane::perform (const InvocationInfo& info)
{
    // Bouml preserved body begin 0004910D
    if (info.commandID == 0) //D3CKStdCommands::del)
    {
    }
    else if (info.commandID == 1) //D3CKStdCommands::selectAll)
    {
        for (int i = modules.size(); --i >= 0;)
        {
            selectedModules.addToSelection (modules[i]);
        }
        return true;
        //this->selectedModules.changed();
    }
    return false;
    // Bouml preserved body end 0004910D
}

//ApplicationCommandTarget *  getNextCommandTarget
ApplicationCommandTarget* ModulePane::getNextCommandTarget()
{
    // Bouml preserved body begin 0004918D
    return 0;
    // Bouml preserved body end 0004918D
}

//void  getCommandInfo (const CommandID commandID, ApplicationCommandInfo &result)=
void ModulePane::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
    // Bouml preserved body begin 0004920D

    // Bouml preserved body end 0004920D
}

//void  getAllCommands (Array< CommandID > &commands)=0
void ModulePane::getAllCommands (Array<CommandID>& commands)
{
    // Bouml preserved body begin 0004928D
    commands.add (0); //D3CKStdCommands::del);
    commands.add (1); //D3CKStdCommands::selectAll);
    // Bouml preserved body end 0004928D
}
