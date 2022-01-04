#ifndef __PIZ_MidiEnvelope_HEADER__
#define __PIZ_MidiEnvelope_HEADER__

#include "curve.h"

class MidiIndicator : public Component
{
friend class MidiEnvelope;
public:
	MidiIndicator()
	{
		inmsg = -1;
		outmsg = -1;
	}
	~MidiIndicator() {}

	void paint (Graphics &g)
	{
		const int dotSize = MAX_ENVELOPE_DOT_SIZE;
		const int halfDotSize = dotSize / 2;
		g.fillAll(Colours::transparentBlack);
		g.setColour(Colours::darkgoldenrod);
		g.drawVerticalLine((int)((float)(inmsg*getWidth())*fmidiScaler),
						   (float)getHeight()-(float)(outmsg*getHeight())*fmidiScaler,
						   (float)getHeight());
		g.fillEllipse(((float)(inmsg*getWidth())*fmidiScaler)-(float)halfDotSize,
					  (float)getHeight()-(float)(outmsg*getHeight())*fmidiScaler-(float)halfDotSize,
					  (float)dotSize,(float)dotSize);
	}
private:
	int inmsg, outmsg;
};

//==============================================================================
class MidiEnvelope : public Component
{
public:

    //==============================================================================
    MidiEnvelope (const int envelopeType,
                       AudioProcessorEditor* owner,
                       MidiCurve* plugin);
	~MidiEnvelope();

    //==============================================================================
    void updateParameters (const bool repaintComponent = true);
    
    //==============================================================================
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);
    void mouseUp (const MouseEvent& e);
    void mouseMove (const MouseEvent& e);
    void mouseDoubleClick (const MouseEvent& e);

    void paint (Graphics& g);
    void resized ();
	float getValue(float input);
	void repaintIndicator(int in, int out) 
	{
		indicator->inmsg = in;
		indicator->outmsg = out;
		indicator->repaint();
	}


protected:

    int findPointByMousePos (const int x, const int y);

    AudioProcessorEditor* owner;
    MidiCurve* plugin;
	MidiIndicator* indicator;
    
    int draggingPoint;
	int hoveringPoint;
	Point<float> mouseDownPoint;
    float points [MAX_ENVELOPE_POINTS][2];
    float oldpoints [MAX_ENVELOPE_POINTS][2];
	void setPointActive(int point, bool active);
	bool isPointActive(int point);
	void setPointControl(int point, bool control);
	bool isPointControl(int point);
    Label* labelX;
    Label* labelY;
	int findInactivePoint();
	int addPoint(float x, float y, bool control=false);
};

#endif

