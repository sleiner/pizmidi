#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class Controller;
class Scene;

class ControllerValue
{
public:
    ControllerValue(Controller* controller, Scene* scene);

    ~ControllerValue();

    bool active;

    Scene* scene;

    int getValue();
    void setValue(int newValue);

    Scene* getScene();

    Controller* getController();

    void getMidiMessage(juce::MidiBuffer& buffer, int pos);

    friend class Controller;

private:
    int value;
    Controller* controller;
};
