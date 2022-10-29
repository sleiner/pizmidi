#ifndef __PIZ_MidiEnvelope_HEADER__
#define __PIZ_MidiEnvelope_HEADER__

#include "curve.h"

class MidiIndicator : public juce::Component
{
    friend class MidiEnvelope;

public:
    MidiIndicator()
    {
        inmsg  = -1;
        outmsg = -1;
    }

    ~MidiIndicator() override
    {
    }

    void paint(juce::Graphics& g) override
    {
        const int dotSize     = MAX_ENVELOPE_DOT_SIZE;
        const int halfDotSize = dotSize / 2;
        g.fillAll(juce::Colours::transparentBlack);
        g.setColour(juce::Colours::darkgoldenrod);
        g.drawVerticalLine((int) ((float) (inmsg * getWidth()) * fmidiScaler),
                           (float) getHeight() - (float) (outmsg * getHeight()) * fmidiScaler,
                           (float) getHeight());
        g.fillEllipse(((float) (inmsg * getWidth()) * fmidiScaler) - (float) halfDotSize,
                      (float) getHeight() - (float) (outmsg * getHeight()) * fmidiScaler - (float) halfDotSize,
                      (float) dotSize,
                      (float) dotSize);
    }

private:
    int inmsg, outmsg;
};

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

    void repaintIndicator(int in, int out)
    {
        indicator->inmsg  = in;
        indicator->outmsg = out;
        indicator->repaint();
    }

protected:
    int findPointByMousePos(const int x, const int y);

    juce::AudioProcessorEditor* owner;
    MidiCurve* plugin;
    MidiIndicator* indicator;

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
