#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_core/juce_core.h"
#include "juce_events/juce_events.h"

#include "Controller.h"
#include "Scene.h"

class Cursor;
namespace juce { class XmlElement; }
namespace juce { class MidiMessage; }
namespace juce { class MidiBuffer; }
class ControllerValue;

using namespace juce;

class MidiMorph : public juce::ChangeBroadcaster {
  public:
    MidiMorph();
    ~MidiMorph() override;

    OwnedArray<Scene> scenes;
    Cursor* cursor;

    int refreshRate;
    juce::Rectangle<int> paneSize;
    juce::Rectangle<int> guiBounds;
    bool autoLearn;
    bool autoKey;

    void setCursorXRatio(float x);
    void setCursorYRatio(float y);
    float getCursorXRatio();
    float getCursorYRatio();

    //-can happen by input or slider movement.
    //
    //-if a scene is selected, (or multiple scenes) the value for that controller gets set.
    //-if no scene is selected and auto key is on, a scene gets added.
    void controllerChanged(const Controller* controllerThatHasChanged);

    //send interpolated controller vaules  that actually differ from the last output to midi out
    void distancesChanged();

    //foreach scene : moved();
    void cursorChanged();

    void showControllers(bool show);
    void addController(int ccNo, int Channel);
    void addController();
    void addScene(Scene * scene);
    int getNumControllers();
    int getControllerCCNo(int index);
    int getControllerValue(int index);

    //if controller != cursor controller
    //if   controller is in list
    //	if - scene/s is selected, .> set/add value for selected scenes/s
    //	else if  "autokey" is on, ->add scene at cursor pos, select scene, set/add value for cursor,
    //
    //else if "autolearn" is on add controller.
    //		if - scene/s is selected, .> set/add value for selected scenes/s
    //		else if no scene is selected && "autokey" is on, ->add scene at cursor pos, select scene, set/add value for cursor
    void onMidiEvent(juce::MidiMessage & events);

    void getMidiMessages(int offset, juce::MidiBuffer & buffer);

    float getSumDistances();

    Controller* getController(int index);

    int getNumSelectedScenes();

    void setSelectedScenes(Array<Scene*> & scenes);

    Array<Scene*>* getSelectedScenes();

    Scene* getSelectedScene(int index);

    Cursor* getCursor();

    float getSumAffectionValues();

    void removeScene(Scene * scene);

    OwnedArray<Controller>* getControllers();

    juce::XmlElement* getXml(const juce::String tagname);

    void setFromXml(juce::XmlElement * xmlData);

    int getSceneIndex(Scene * scene);

    int getUpdateRateSmpls(int rate);

    void removeController(Controller * controllerToRemove);

    void saveGUIState(juce::XmlElement * state);

    juce::XmlElement* getSavedGUIState();

    void setPaneSize(juce::Rectangle<int> size);

    juce::Rectangle<int> getPaneSize();

    int controllerListWidth;

    void addSceneAtCursor();

    int getRefreshRate();

    bool hasNewMidi();

    OwnedArray<Scene>* getScenes();

    void sendChangeMessage(void * ptr);

    void sceneMoved();

    void setAuditSelScene(bool shouldAudit);

    bool getAuditSelScene();

    bool needsRefresh();

    bool needsOneRefresh();

  private:
    OwnedArray<Controller> controllers;
    Array<Scene*> selectedScenes;
    juce::XmlElement* guiState;
    bool valuesChanged;
    bool valueChanged;
    bool sumDistancesChanged;
    float sumDistances;
    bool auditSelScene;
    void* lastRecipant;

};
