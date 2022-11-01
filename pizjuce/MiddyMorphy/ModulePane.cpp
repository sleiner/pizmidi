
#include "ModulePane.h"

#include "Module.h"
#include "ModuleGUI.h"
#include "ModulePaneModel.h"

ModulePane::ModulePane(ModulePaneModel* modulePaneModel)
{
    this->model = modulePaneModel;
    this->model->setOwnerPane(this);
    this->updateContent();
    this->selectedModules.addChangeListener(this);
    this->addAndMakeVisible(&lasso);
    this->setMouseClickGrabsKeyboardFocus(false);
    this->setInterceptsMouseClicks(true, true);
}

ModulePane::~ModulePane()
{
    selectedModules.removeChangeListener(this);
    modules.clear();
}

void ModulePane::mouseDrag(const juce::MouseEvent& e)
{
    model->mouseDrag(e);
    if (e.mods.isMiddleButtonDown())
    {
        dragOrigin(e);
    }
    else if (e.mods.isLeftButtonDown() && (e.mods.isAltDown() || e.mods.isCtrlDown()))
    {
        this->lasso.dragLasso(e);
    }
}

void ModulePane::mouseDown(const juce::MouseEvent& e)
{
    model->mouseDown(e);

    if (e.mods.isMiddleButtonDown())
    {
        setMouseCursor(juce::MouseCursor::DraggingHandCursor);
        startDragOrigin(e);
    }
    else if (e.mods.isLeftButtonDown() && (e.mods.isAltDown() || e.mods.isCtrlDown()))
    {
        if (! e.mods.isCtrlDown())
        {
            this->selectedModules.deselectAll();
        }
        this->lasso.beginLasso(e, this);
    }
}

void ModulePane::mouseUp(const juce::MouseEvent& e)
{
    lasso.endLasso();
    model->mouseUp(e);

    setMouseCursor(juce::MouseCursor::NormalCursor);
}

void ModulePane::mouseWheelMove(const juce::MouseEvent& e, float wheelIncrementX, float wheelIncrementY)
{
    float factor = 1 + wheelIncrementY;
    this->zoom(factor, factor, (float) e.x, (float) e.y);
}

void ModulePane::updateContent()
{
    this->selectedModules.deselectAll();
    while (modules.size() > 0)
    {
        ModuleGUI* oldGUI = modules.removeAndReturn(0);
        removeZoomedComp(oldGUI);
    }

    for (int i = 0; i < model->getNumModules(); i++)
    {
        ModuleGUI* gui = model->createGUI(i);
        if (gui == nullptr)
        {
            gui = new ModuleGUI(model->getModule(i));
        }
        modules.add(gui);
        gui->setPane(this);
        addZoomedComp(gui);
    }
    rePositionChildren();
}

bool ModulePane::isModuleSelected(const Module* module)
{
    return true;
}

void ModulePane::paintOverChildren(juce::Graphics& g)
{
    g.drawRoundedRectangle(0.f, 0.f, (float) getWidth(), (float) getHeight(), 2.f, 2.f);
}

void ModulePane::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    void* objectThatHasChanged = source;
    if (objectThatHasChanged == &selectedModules)
    {
        repaint();
        model->selectionChanged(selectedModules.getItemArray());
    }
}

juce::SelectedItemSet<ModuleGUI*>& ModulePane::getLassoSelection()
{
    return this->selectedModules;
}

void ModulePane::findLassoItemsInArea(juce::Array<ModuleGUI*>& itemsFound, const juce::Rectangle<int>& area)
{
    for (int i = 0; i < this->modules.size(); i++)
    {
        ModuleGUI* current = modules[i];
        if (lasso.getBounds().contains(current->getBounds()))
        {
            itemsFound.add(current);
        }
    }
}

void ModulePane::selectModule(int index, bool deselectOthers)
{
    if (deselectOthers)
    {
        selectedModules.deselectAll();
    }
    this->selectedModules.addToSelection(modules[index]);
}

bool ModulePane::perform(const InvocationInfo& info)
{
    if (info.commandID == 0)
    {
    }
    else if (info.commandID == 1)
    {
        for (int i = modules.size(); --i >= 0;)
        {
            selectedModules.addToSelection(modules[i]);
        }
        return true;
    }
    return false;
}

juce::ApplicationCommandTarget* ModulePane::getNextCommandTarget()
{
    return nullptr;
}

void ModulePane::getCommandInfo(const juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
}

void ModulePane::getAllCommands(juce::Array<juce::CommandID>& commands)
{
    commands.add(0);
    commands.add(1);
}
