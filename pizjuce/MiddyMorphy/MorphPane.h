#pragma once

#include "ModulePane.h"

class ModulePaneModel;
class MidiMorph;
class ResizableBorderComponentEx;
class Scene;

class MorphPane : public ModulePane
{
public:
    MorphPane (ModulePaneModel* model, MidiMorph* core);

    ~MorphPane() override;

    void resized() override;

    void setSelectedScenes (juce::Array<Scene*>* scenes, bool sendChangeMessage);

private:
    ResizableBorderComponentEx* resizer;
    MidiMorph* core;
};
