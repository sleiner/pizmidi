#pragma once


#include "juce_core/juce_core.h"
#include "juce_events/juce_events.h"

class MidiMorph;
class ControllerValue;
class Scene;
namespace juce { class MidiBuffer; }
namespace juce { class MidiMessage; }
using namespace juce;

class Controller : public juce::ChangeBroadcaster {
  public:
    Controller(MidiMorph * core);

    ~Controller();


  private:
    bool newMidi;

    bool valueChanged;

    int ccNo;

    int channel;

    int value;

    String name;


  public:
    enum states {
      undefined = 0,
      mutival,
      defined

    };


  private:
    Array<ControllerValue*> values;

    MidiMorph* core;

  friend class ControllerValue;
  friend class ControllerGUI;

  public:
    ControllerValue* getValue(int index);

    ControllerValue* getValue(const Scene* scene);

    //set value for particular scene
    void setValue(int newValue, Scene * scene);

    //refresh value of selected scenes
    void controllerChanged();

    int getState();

    int getInterpolatedValue();

    void addValue(ControllerValue * value);

    void learn();

    void removeValue(ControllerValue * value);

    int getNumValues();

    void getMidiMessage(juce::MidiBuffer & buffer, int pos);

    void remove();

    bool hasNewMidi();

    int getCcNo();

    void setCcNo(int val);

    int getChannel();

    void setChannel(int channel);

    String getName();

    void setName(String name);

    //set value for selected Scenes
    void setValue(int newValue);


  private:
    juce::MidiMessage* midiMessage;

    int lastSentCcNo;

    int lastSentChannel;

    int lastSentValue;

};
