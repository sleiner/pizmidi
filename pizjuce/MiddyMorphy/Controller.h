#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>

class MidiMorph;
class ControllerValue;
class Scene;

class Controller : public juce::ChangeBroadcaster
{
public:
    Controller (MidiMorph* core);

    ~Controller() override;

    enum states
    {
        undefined = 0,
        mutival,
        defined

    };

    ControllerValue* getValue (int index);

    ControllerValue* getValue (const Scene* scene);

    void setValue (int newValue, Scene* scene);

    void controllerChanged();

    int getState();

    int getInterpolatedValue();

    void addValue (ControllerValue* value);

    void learn();

    void removeValue (ControllerValue* value);

    int getNumValues();

    void getMidiMessage (juce::MidiBuffer& buffer, int pos);

    void remove();

    bool hasNewMidi();

    int getCcNo() const;

    void setCcNo (int val);

    int getChannel() const;

    void setChannel (int channel);

    juce::String getName();

    void setName (juce::String name);

    void setValueInSelectedScenes (int newValue);

private:
    bool newMidi;

    bool valueChanged;

    int ccNo;
    int channel;
    int value;

    juce::String name;

    juce::Array<ControllerValue*> values;

    MidiMorph* core;

    friend class ControllerValue;
    friend class ControllerGUI;

    juce::MidiMessage* midiMessage;
};
