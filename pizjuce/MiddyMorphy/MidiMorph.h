#pragma once

#include "Controller.h"
#include "Scene.h"

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>

class Cursor;
class ControllerValue;

class MidiMorph : public juce::ChangeBroadcaster
{
public:
    MidiMorph();
    ~MidiMorph() override;

    juce::OwnedArray<Scene> scenes;
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

    void controllerChanged(const Controller* controllerThatHasChanged);

    void distancesChanged();

    void cursorChanged();

    void showControllers(bool show);
    void addController(int ccNo, int Channel);
    void addController();
    void addScene(Scene* scene);
    int getNumControllers();
    int getControllerCCNo(int index);
    int getControllerValue(int index);

    void onMidiEvent(juce::MidiMessage& events);

    void getMidiMessages(int offset, juce::MidiBuffer& buffer);

    float getSumDistances();

    Controller* getController(int index);

    int getNumSelectedScenes();

    void setSelectedScenes(juce::Array<Scene*>& scenes);

    juce::Array<Scene*>* getSelectedScenes();

    Scene* getSelectedScene(int index);

    Cursor* getCursor();

    float getSumAffectionValues();

    void removeScene(Scene* scene);

    juce::OwnedArray<Controller>* getControllers();

    juce::XmlElement* getXml(juce::String tagname);

    void setFromXml(juce::XmlElement* xmlData);

    int getSceneIndex(Scene* scene);

    int getUpdateRateSmpls(int rate);

    void removeController(Controller* controllerToRemove);

    void saveGUIState(juce::XmlElement* state);

    juce::XmlElement* getSavedGUIState();

    void setPaneSize(juce::Rectangle<int> size);

    juce::Rectangle<int> getPaneSize();

    int controllerListWidth;

    void addSceneAtCursor();

    int getRefreshRate();

    bool hasNewMidi();

    juce::OwnedArray<Scene>* getScenes();

    void sendChangeMessage(void* ptr);

    void sceneMoved();

    void setAuditSelScene(bool shouldAudit);

    bool getAuditSelScene();

    bool needsRefresh();

    bool needsOneRefresh();

    void* currentChangeBroadcastRecipient;

private:
    juce::OwnedArray<Controller> controllers;
    juce::Array<Scene*> selectedScenes;
    juce::XmlElement* guiState;
    bool valuesChanged;
    bool valueChanged;
    bool sumDistancesChanged;
    float sumDistances;
    bool auditSelScene;
    void* lastRecipant;
};
