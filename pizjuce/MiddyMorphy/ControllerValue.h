#pragma once


class Controller;
class Scene;
namespace juce { class MidiBuffer; } 

class ControllerValue {
  public:
    ControllerValue(Controller * controller, Scene * scene);

    //at destruction make shure to deletete reference in controller AND scene!!
    ~ControllerValue();

    bool active;


  private:
    int value;


  public:
    Scene* scene;


  private:
    Controller* controller;


  public:
    void setValue(int newValue);

    int getValue();

    float getValueRation();

    Scene* getScene();

    Controller* getController();

    void getMidiMessage(juce::MidiBuffer & buffer, int pos);

  friend class Controller;
};

