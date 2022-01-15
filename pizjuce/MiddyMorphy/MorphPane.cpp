
#include "MorphPane.h"
#include "MidiMorph.h"
#include "ModulePaneModel.h"
//#include "ResizableBorderComponentEx.h"
#include "Scene.h"

MorphPane::MorphPane (ModulePaneModel* model, MidiMorph* core)
    : ModulePane (model)
{
    // Bouml preserved body begin 00042F8D
    this->core = core;
    this->setSelectedScenes (core->getSelectedScenes(), false);
    //this->selectedModules.addChangeListener(core);
    //ComponentBoundsConstrainer* limit = new ComponentBoundsConstrainer();
    //limit->setMinimumSize(50,50);

    //addAndMakeVisible(resizer = new ResizableBorderComponentEx(this,limit));
    //resizer->setActiveBorders(false,true,true,false);

    // Bouml preserved body end 00042F8D
}

void MorphPane::resized()
{
    // Bouml preserved body begin 0004300D

    //resizer->setBounds(0,0,getWidth(),getHeight());

    //core->setPaneSize(getBounds());

    //getParentComponent()->resized();

    // Bouml preserved body end 0004300D
}

MorphPane::~MorphPane()
{
    // Bouml preserved body begin 0004790D
    //deleteAndZero(resizer);
    // Bouml preserved body end 0004790D
}

void MorphPane::setSelectedScenes (Array<Scene*>* scenes, bool sendChangeMessage)
{
    // Bouml preserved body begin 0004808D
    for (int i = 0; i < scenes->size(); i++)
    {
        int isc = core->getSceneIndex (scenes->operator[] (i));
        selectModule (isc, i == 0);
    }
    // Bouml preserved body end 0004808D
}
