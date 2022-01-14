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

    void resized() override;

private:
    ResizableBorderComponentEx* resizer;

public:
    ~MorphPane() override;

private:
    MidiMorph* core;

public:
    void setSelectedScenes (Array<Scene*>* scenes, bool sendChangeMessage);
};
