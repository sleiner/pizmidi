
#include "MorphPane.h"

#include "MidiMorph.h"
#include "ModulePaneModel.h"
#include "Scene.h"

MorphPane::MorphPane(ModulePaneModel* model, MidiMorph* core)
    : ModulePane(model)
{
    this->core = core;
    this->setSelectedScenes(core->getSelectedScenes(), false);
    //this->selectedModules.addChangeListener(core);
    //ComponentBoundsConstrainer* limit = new ComponentBoundsConstrainer();
    //limit->setMinimumSize(50,50);

    //addAndMakeVisible(resizer = new ResizableBorderComponentEx(this,limit));
    //resizer->setActiveBorders(false,true,true,false);
}

void MorphPane::resized()
{
    //resizer->setBounds(0,0,getWidth(),getHeight());

    //core->setPaneSize(getBounds());

    //getParentComponent()->resized();
}

MorphPane::~MorphPane()
{
    //deleteAndZero(resizer);
}

void MorphPane::setSelectedScenes(juce::Array<Scene*>* scenes, bool sendChangeMessage)
{
    for (int i = 0; i < scenes->size(); i++)
    {
        int isc = core->getSceneIndex(scenes->operator[](i));
        selectModule(isc, i == 0);
    }
}
