#ifndef PIZMIDI_TIMELINE_H
#define PIZMIDI_TIMELINE_H

#include "juce_gui_basics/juce_gui_basics.h"

class PianoRoll;

class Timeline : public juce::Component, public juce::ChangeBroadcaster
{
public:
    Timeline();
    ~Timeline() override;

    void setPianoRoll(PianoRoll* pr);

    void paint(juce::Graphics& g) override;
    float getStartPixel();
    float getEndPixel();
    double getLength();
    double getStart();

    void setLoop(double start, double length);

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;

    int scrollOffset;

private:
    double loopStart;
    double loopEnd;
    PianoRoll* roll;
};

#endif //PIZMIDI_TIMELINE_H
