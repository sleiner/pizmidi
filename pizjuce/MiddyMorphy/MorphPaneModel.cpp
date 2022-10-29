
#include "MorphPaneModel.h"

#include <juce_events/juce_events.h>

#include "Cursor.h"
#include "CursorGUI.h"
#include "MidiMorph.h"
#include "Module.h"
#include "TextBoxSlider.h"

using juce::roundToInt;

MorphPaneModel::MorphPaneModel(MidiMorph* core)
{
    this->core   = core;
    this->cursor = core->getCursor();

    addAndMakeVisible(rateBox = new TextBoxSlider(0));
    rateBox->setRange(1, 5000, 1, 2);
    rateBox->addListener(this);
    rateBox->setColour(rateBox->backgroundColourId, juce::Colours::white);
    rateBox->setValue(core->refreshRate, juce::dontSendNotification);
}

int MorphPaneModel::getNumModules()
{
    return core->scenes.size();
}

void MorphPaneModel::mouseDown(const juce::MouseEvent& e)
{
    if (e.mods.isLeftButtonDown() && ! e.mods.isCtrlDown() && ! e.mods.isAltDown())
    {
        int s = cursor->size / 2;
        cursor->setXY((float) (e.x - s), (float) (e.y - s));
    }
}

void MorphPaneModel::mouseDrag(const juce::MouseEvent& e)
{
    if (e.mods.isLeftButtonDown() && ! e.mods.isCtrlDown() && ! e.mods.isAltDown())
    {
        int s = cursor->size / 2;
        cursor->setXY((float) (e.x - s), (float) (e.y - s));
    }
}

void MorphPaneModel::mouseUp(const juce::MouseEvent& e)
{
    if (e.mods.isRightButtonDown())
    {
        juce::PopupMenu menu, sub1;
        menu.addItem(1, "add scene");
        menu.addItem(5, "add controller");
        menu.addSeparator();
        sub1.addCustomItem(2, *rateBox, 64, 20, false, nullptr);
        menu.addSubMenu("midi out refresh rate", sub1);
        //menu.addItem(2,"Midi Out Refresh Rate:" + String(core->refreshRate));
        menu.addItem(3, "auto-learn controller", true, core->autoLearn);
        menu.addItem(4, "auto-key scene", true, core->autoKey);
        menu.addSeparator();
        menu.addItem(6, "audit selected scene", true, core->getAuditSelScene());
        menu.addItem(8, "show controllers", true, core->controllerListWidth != 0);
        menu.addItem(7, "select all");

        int result = menu.show();
        if (result == 1)
        {
            auto* ns = new Scene(core);
            ns->setXY((float) e.x, (float) e.y);
            core->addScene(ns);
        }
        //		else if(result == 2)
        //		{
        //			Slider slider(T(""));
        //			slider.setRange(1,5000,1);
        //			slider.setSize(200,25);
        //			slider.setValue(core->refreshRate);
        //			DialogWindow::showModalDialog("",&slider,0,Colours::white,true);
        //			core->refreshRate = slider.getValue();
        //			//deleteAndZero(slider)
        //
        //		}
        else if (result == 3)
        {
            core->autoLearn = ! core->autoLearn;
        }
        else if (result == 4)
        {
            core->autoKey = ! core->autoKey;
        }
        else if (result == 5)
        {
            core->addController();
        }
        else if (result == 6)
        {
            core->setAuditSelScene(! core->getAuditSelScene());
        }
        else if (result == 7)
        {
            juce::ApplicationCommandTarget::InvocationInfo info(1);
            performCommand(info);
        }
        else if (result == 8)
        {
            if (core->controllerListWidth == 0)
                core->controllerListWidth = 120;
            else
                core->controllerListWidth = 0;
            core->sendChangeMessage(this);
        }
    }
}

Module* MorphPaneModel::getModule(int index)
{
    return core->scenes[index];
}

ModuleGUI* MorphPaneModel::createGUI(int index)
{
    return nullptr;
}

void MorphPaneModel::selectionChanged(const juce::Array<ModuleGUI*>& modules)
{
    juce::Array<Scene*> scenes;
    for (int i = 0; i < modules.size(); i++)
    {
        scenes.add((Scene*) modules[i]->getModule());
    }
    core->setSelectedScenes(scenes);
}

void MorphPaneModel::labelTextChanged(juce::Label* labelThatHasChanged)
{
    if (labelThatHasChanged == rateBox)
    {
        core->refreshRate = roundToInt(rateBox->getValue());
    }
}
