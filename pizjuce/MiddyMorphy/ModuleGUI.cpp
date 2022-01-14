
#include "ModuleGUI.h"
#include "Module.h"
#include "ModulePane.h"

ModuleGUI::ModuleGUI (Module* module)
{
    // Bouml preserved body begin 0003C20D
    this->module = module;
    this->setMouseClickGrabsKeyboardFocus (false);
    // Bouml preserved body end 0003C20D
}

void ModuleGUI::mouseDrag (const MouseEvent& e)
{
    // Bouml preserved body begin 0003B80D
    module->mouseDrag (e);
    if (e.mods.isLeftButtonDown())
    {
        if (e.mods.isAltDown() || e.mods.isCtrlDown())
        {
            for (int i = 0; i < pane->selectedModules.getNumSelected(); i++)
            {
                pane->selectedModules.getSelectedItem (i)->drag (e);
            }
        }
        else
            pane->mouseDrag (e.getEventRelativeTo (pane));
    }

    // Bouml preserved body end 0003B80D
}

void ModuleGUI::mouseUp (const MouseEvent& e)
{
    // Bouml preserved body begin 0003B88D
    module->mouseUp (e);
    //pane->mouseUp(e.getEventRelativeTo(pane));
    if (e.mods.isLeftButtonDown() && (e.mods.isAltDown() || e.mods.isCtrlDown()))
    {
        if (e.mods.isCtrlDown())
        {
            pane->selectedModules.addToSelectionOnMouseUp (this, e.mods, e.getDistanceFromDragStart() > 0, this->selectionBool);
        }
        else if (pane->selectedModules.getNumSelected() == 1)
        {
            pane->selectedModules.deselectAll();
        }
    }

    // Bouml preserved body end 0003B88D
}

void ModuleGUI::mouseDown (const MouseEvent& e)
{
    // Bouml preserved body begin 0003B90D
    module->mouseDown (e);
    pane->mouseDown (e.getEventRelativeTo (pane));
    if (e.mods.isLeftButtonDown() && (e.mods.isAltDown() || e.mods.isCtrlDown()))
    {
        this->selectionBool = pane->selectedModules.addToSelectionOnMouseDown (this, e.mods);
        this->toFront (true);
        for (int i = 0; i < pane->selectedModules.getNumSelected(); i++)
        {
            pane->selectedModules.getSelectedItem (i)->startDrag (e);
        }
    }
    else if (e.mods.isRightButtonDown())
    {
        //this->selectionBool = pane->selectedModules.addToSelectionOnMouseDown(this,e.mods);
        this->toFront (true);
    }
    // Bouml preserved body end 0003B90D
}

void ModuleGUI::setOriginalBounds (const Rectangle<int> bounds)
{
    // Bouml preserved body begin 0003DF8D
    module->setXY ((float) bounds.getX(), (float) bounds.getY());
    // Bouml preserved body end 0003DF8D
}

Rectangle<int> ModuleGUI::getOriginalBounds()
{
    // Bouml preserved body begin 0003DA8D
    return Rectangle<int> (roundToInt (module->getX()), roundToInt (module->getY()), module->size, module->size);
    // Bouml preserved body end 0003DA8D
}

//==============================================================================

void ModuleGUI::paint (Graphics& g)
{
    // Bouml preserved body begin 0003DC0D
    int s = getWidth();
    g.setColour (module->colour);
    g.fillEllipse (2.f, 2.f, (float) (s - 4), (float) (s - 4));
    if (isSelected())
    {
        g.setColour (Colours::black);
        g.drawEllipse (2, 2, (float) (s - 4), (float) (s - 4), 2);
    }

    g.setColour (Colours::white);
    g.setFont (g.getCurrentFont().withHeight (s * 0.4f).withStyle (Font::bold));
    g.drawFittedText (module->name, 0, 0, s, s, Justification::centred, 1);
    // Bouml preserved body end 0003DC0D
}

void ModuleGUI::setPane (ModulePane* pane)
{
    // Bouml preserved body begin 0003DD0D
    this->pane = pane;
    // Bouml preserved body end 0003DD0D
}

void ModuleGUI::startDrag (juce::MouseEvent const& e)
{
    // Bouml preserved body begin 0003DE0D
    this->dragger.startDraggingComponent (this, e);
    // Bouml preserved body end 0003DE0D
}

void ModuleGUI::drag (const MouseEvent& e)
{
    // Bouml preserved body begin 0003DE8D
    dragger.dragComponent (this, e, nullptr);
    refreshOriginalBounds();
    // Bouml preserved body end 0003DE8D
}

bool ModuleGUI::isSelected()
{
    // Bouml preserved body begin 0003E40D
    return this->pane->selectedModules.isSelected (this);
    // Bouml preserved body end 0003E40D
}

Module* ModuleGUI::getModule()
{
    // Bouml preserved body begin 0003E88D
    return module;
    // Bouml preserved body end 0003E88D
}
