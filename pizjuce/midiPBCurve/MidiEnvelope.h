#ifndef __PIZ_MidiEnvelope_HEADER__
#define __PIZ_MidiEnvelope_HEADER__

#include "curve.h"

//==============================================================================
class MidiEnvelope : public Component
{
public:
    //==============================================================================
    MidiEnvelope (const int envelopeType,
                  AudioProcessorEditor* owner,
                  MidiCurve* plugin);
    ~MidiEnvelope() override;

    //==============================================================================
    void updateParameters (const bool repaintComponent = true);

    //==============================================================================
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;
    void mouseMove (const MouseEvent& e) override;
    void mouseDoubleClick (const MouseEvent& e) override;

    void paint (Graphics& g) override;
    void resized() override;
    float getValue (float input);

protected:
    int findPointByMousePos (const int x, const int y);

    AudioProcessorEditor* owner;
    MidiCurve* plugin;

    int draggingPoint;
    int hoveringPoint;
    Point<float> mouseDownPoint;
    float points[MAX_ENVELOPE_POINTS][2];
    float oldpoints[MAX_ENVELOPE_POINTS][2];
    void setPointActive (int point, bool active);
    bool isPointActive (int point);
    void setPointControl (int point, bool control);
    bool isPointControl (int point);
    Label* labelX;
    Label* labelY;
    int findInactivePoint();
    int addPoint (float x, float y, bool control = false);
};

#endif
