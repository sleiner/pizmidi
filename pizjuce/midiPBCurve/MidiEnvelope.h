#ifndef __PIZ_MidiEnvelope_HEADER__
#define __PIZ_MidiEnvelope_HEADER__

#include "curve.h"

//==============================================================================
class MidiEnvelope : public juce::Component
{
public:
    //==============================================================================
    MidiEnvelope(const int envelopeType,
                 juce::AudioProcessorEditor* owner,
                 MidiCurve* plugin);
    ~MidiEnvelope() override;

    //==============================================================================
    void updateParameters(const bool repaintComponent = true);

    //==============================================================================
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseMove(const juce::MouseEvent& e) override;
    void mouseDoubleClick(const juce::MouseEvent& e) override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    float getValue(float input);

protected:
    int findPointByMousePos(const int x, const int y);

    juce::AudioProcessorEditor* owner;
    MidiCurve* plugin;

    int draggingPoint;
    int hoveringPoint;
    juce::Point<float> mouseDownPoint;
    float points[MAX_ENVELOPE_POINTS][2];
    float oldpoints[MAX_ENVELOPE_POINTS][2];
    void setPointActive(int point, bool active);
    bool isPointActive(int point);
    void setPointControl(int point, bool control);
    bool isPointControl(int point);
    juce::Label* labelX;
    juce::Label* labelY;
    int findInactivePoint();
    int addPoint(float x, float y, bool control = false);
};

#endif
