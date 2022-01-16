
#include "MorphPaneModel.h"

#include "juce_events/juce_events.h"

#include "Cursor.h"
#include "CursorGUI.h"
#include "MidiMorph.h"
#include "Module.h"
#include "TextBoxSlider.h"

MorphPaneModel::MorphPaneModel (MidiMorph* core)
{
    // Bouml preserved body begin 0003CC0D
    this->core   = core;
    this->cursor = core->getCursor();

    addAndMakeVisible (rateBox = new TextBoxSlider (0));
    rateBox->setRange (1, 5000, 1, 2);
    rateBox->addListener (this);
    rateBox->setColour (rateBox->backgroundColourId, Colours::white);
    rateBox->setValue (core->refreshRate, dontSendNotification);

    // Bouml preserved body end 0003CC0D
}

int MorphPaneModel::getNumModules()
{
    // Bouml preserved body begin 0003CE0D
    return core->scenes.size();
    // Bouml preserved body end 0003CE0D
}

void MorphPaneModel::mouseDown (const MouseEvent& e)
{
    // Bouml preserved body begin 0003D20D
    if (e.mods.isLeftButtonDown() && ! e.mods.isCtrlDown() && ! e.mods.isAltDown())
    {
        int s = cursor->size / 2;
        cursor->setXY ((float) (e.x - s), (float) (e.y - s));
    }
    // Bouml preserved body end 0003D20D
}

void MorphPaneModel::mouseDrag (const MouseEvent& e)
{
    if (e.mods.isLeftButtonDown() && ! e.mods.isCtrlDown() && ! e.mods.isAltDown())
    {
        int s = cursor->size / 2;
        cursor->setXY ((float) (e.x - s), (float) (e.y - s));
    }
}

void MorphPaneModel::mouseUp (const MouseEvent& e)
{
    // Bouml preserved body begin 0003D28D
    if (e.mods.isRightButtonDown())
    {
        PopupMenu menu, sub1;
        menu.addItem (1, "add scene");
        menu.addItem (5, "add controller");
        menu.addSeparator();
        sub1.addCustomItem (2, *rateBox, 64, 20, false, nullptr);
        menu.addSubMenu ("midi out refresh rate", sub1);
        //menu.addItem(2,"Midi Out Refresh Rate:" + String(core->refreshRate));
        menu.addItem (3, "auto-learn controller", true, core->autoLearn);
        menu.addItem (4, "auto-key scene", true, core->autoKey);
        menu.addSeparator();
        menu.addItem (6, "audit selected scene", true, core->getAuditSelScene());
        menu.addItem (8, "show controllers", true, core->controllerListWidth != 0);
        menu.addItem (7, "select all");

        int result = menu.show();
        if (result == 1)
        {
            Scene* ns = new Scene (core);
            ns->setXY ((float) e.x, (float) e.y);
            core->addScene (ns);
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
            core->setAuditSelScene (! core->getAuditSelScene());
        }
        else if (result == 7)
        {
            ApplicationCommandTarget::InvocationInfo info (1);
            performCommand (info);
        }
        else if (result == 8)
        {
            if (core->controllerListWidth == 0)
                core->controllerListWidth = 120;
            else
                core->controllerListWidth = 0;
            core->sendChangeMessage (this);
        }
    }
    // Bouml preserved body end 0003D28D
}

Module* MorphPaneModel::getModule (int index)
{
    // Bouml preserved body begin 0003D80D
    return core->scenes[index];
    // Bouml preserved body end 0003D80D
}

ModuleGUI* MorphPaneModel::createGUI (int index)
{
    // Bouml preserved body begin 0003D98D
    return 0;
    // Bouml preserved body end 0003D98D
}

void MorphPaneModel::selectionChanged (const Array<ModuleGUI*>& modules)
{
    // Bouml preserved body begin 0003E78D
    Array<Scene*> scenes;
    for (int i = 0; i < modules.size(); i++)
    {
        scenes.add ((Scene*) modules[i]->getModule());
    }
    core->setSelectedScenes (scenes);
    // Bouml preserved body end 0003E78D
}

void MorphPaneModel::labelTextChanged (Label* labelThatHasChanged)
{
    if (labelThatHasChanged == rateBox)
    {
        core->refreshRate = roundToInt (rateBox->getValue());
    }
}
