#pragma once


#include "ModulePane.h"

class ModulePaneModel;
class MidiMorph;
class ResizableBorderComponentEx;
class Scene;

class MorphPane : public ModulePane {
  public:
    MorphPane(ModulePaneModel * model, MidiMorph * core);

    void resized();


  private:
    ResizableBorderComponentEx* resizer;


  public:
    ~MorphPane();


  private:
    MidiMorph* core;


  public:
    void setSelectedScenes(Array<Scene*> * scenes, bool sendChangeMessage);

};
