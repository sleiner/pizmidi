
#include "ModuleGUI.h"
#include "Module.h"
#include "ModulePane.h"

using juce::roundToInt;

ModuleGUI::ModuleGUI(Module* module)
{
    this->module = module;
    this->setMouseClickGrabsKeyboardFocus(false);
}

void ModuleGUI::mouseDrag(const juce::MouseEvent& e)
{
    module->mouseDrag(e);
    if (e.mods.isLeftButtonDown())
    {
        if (e.mods.isAltDown() || e.mods.isCtrlDown())
        {
            for (int i = 0; i < pane->selectedModules.getNumSelected(); i++)
            {
                pane->selectedModules.getSelectedItem(i)->drag(e);
            }
        }
        else
            pane->mouseDrag(e.getEventRelativeTo(pane));
    }
}

void ModuleGUI::mouseUp(const juce::MouseEvent& e)
{
    module->mouseUp(e);

    if (e.mods.isLeftButtonDown() && (e.mods.isAltDown() || e.mods.isCtrlDown()))
    {
        if (e.mods.isCtrlDown())
        {
            pane->selectedModules.addToSelectionOnMouseUp(this, e.mods, e.getDistanceFromDragStart() > 0, this->selectionBool);
        }
        else if (pane->selectedModules.getNumSelected() == 1)
        {
            pane->selectedModules.deselectAll();
        }
    }
}

void ModuleGUI::mouseDown(const juce::MouseEvent& e)
{
    module->mouseDown(e);
    pane->mouseDown(e.getEventRelativeTo(pane));
    if (e.mods.isLeftButtonDown() && (e.mods.isAltDown() || e.mods.isCtrlDown()))
    {
        this->selectionBool = pane->selectedModules.addToSelectionOnMouseDown(this, e.mods);
        this->toFront(true);
        for (int i = 0; i < pane->selectedModules.getNumSelected(); i++)
        {
            pane->selectedModules.getSelectedItem(i)->startDrag(e);
        }
    }
    else if (e.mods.isRightButtonDown())
    {
        this->toFront(true);
    }
}

void ModuleGUI::setOriginalBounds(const juce::Rectangle<int> bounds)
{
    module->setXY((float) bounds.getX(), (float) bounds.getY());
}

juce::Rectangle<int> ModuleGUI::getOriginalBounds()
{
    return { roundToInt(module->getX()), roundToInt(module->getY()), module->size, module->size };
}

void ModuleGUI::paint(juce::Graphics& g)
{
    int s = getWidth();
    g.setColour(module->colour);
    g.fillEllipse(2.f, 2.f, (float) (s - 4), (float) (s - 4));
    if (isSelected())
    {
        g.setColour(juce::Colours::black);
        g.drawEllipse(2, 2, (float) (s - 4), (float) (s - 4), 2);
    }

    g.setColour(juce::Colours::white);
    g.setFont(g.getCurrentFont().withHeight(s * 0.4f).withStyle(juce::Font::bold));
    g.drawFittedText(module->name, 0, 0, s, s, juce::Justification::centred, 1);
}

void ModuleGUI::setPane(ModulePane* paneToSet)
{
    this->pane = paneToSet;
}

void ModuleGUI::startDrag(juce::MouseEvent const& e)
{
    this->dragger.startDraggingComponent(this, e);
}

void ModuleGUI::drag(const juce::MouseEvent& e)
{
    dragger.dragComponent(this, e, nullptr);
    refreshOriginalBounds();
}

bool ModuleGUI::isSelected()
{
    return this->pane->selectedModules.isSelected(this);
}

Module* ModuleGUI::getModule()
{
    return module;
}
