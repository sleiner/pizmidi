#include "MidiEnvelope.h"

//==============================================================================
MidiEnvelope::MidiEnvelope (const int envelopeType_,
                            AudioProcessorEditor* owner_,
                            MidiCurve* plugin_)
    : owner (owner_),
      plugin (plugin_),
      draggingPoint (-1),
      hoveringPoint (-1),
      labelX (0),
      labelY (0)
{
    addAndMakeVisible (labelX = new Label ("x label",
                                           "x: --"));
    labelX->setFont (Font (15.0000f, Font::plain));
    labelX->setJustificationType (Justification::centredLeft);
    labelX->setEditable (false, false, false);
    labelX->setColour (TextEditor::textColourId, Colours::black);
    labelX->setColour (TextEditor::backgroundColourId, Colour (0x0));
    labelX->setOpaque (false);
    labelX->setInterceptsMouseClicks (false, false);

    addAndMakeVisible (labelY = new Label ("y label",
                                           "y: --"));
    labelY->setFont (Font (15.0000f, Font::plain));
    labelY->setJustificationType (Justification::centredLeft);
    labelY->setEditable (false, false, false);
    labelY->setColour (TextEditor::textColourId, Colours::black);
    labelY->setColour (TextEditor::backgroundColourId, Colour (0x0));
    labelY->setOpaque (false);
    labelY->setInterceptsMouseClicks (false, false);
}

MidiEnvelope::~MidiEnvelope()
{
    deleteAndZero (labelX);
    deleteAndZero (labelY);
}

//==============================================================================
void MidiEnvelope::resized()
{
    labelX->setSize (100, 20);
    labelY->setSize (100, 20);
    labelX->setTopLeftPosition (0, 0);
    labelY->setTopLeftPosition (0, labelX->getHeight());
    updateParameters (false);
}

//==============================================================================
void MidiEnvelope::paint (Graphics& g)
{
    const int dotSize = MAX_ENVELOPE_DOT_SIZE;
    const int halfDotSize = dotSize / 2;

    g.fillAll (Colour (0xffffffff));

    for (int grid = 1; grid < 16383; grid++)
    {
        float inc = 1.f / 16383.f;
        float pbrange2 = plugin->getParameter (kPBRange2) * 24.f;
        int pbstep = roundToInt (8192.f / (jmax (1.f, plugin->getParameter (kPBRange) * 24.f)));
        int pbstep2 = pbrange2 == 0.f ? pbstep : roundToInt (8192.f / (jmax (1.f, pbrange2)));
        if (grid == 8192)
        { //center line
            g.setColour (Colours::blueviolet);
            g.drawLine (0.f, ((float) (16383 - grid) * inc) * getHeight(), (float) getWidth(), ((float) (16383 - grid) * inc) * getHeight(), 1.f);
            g.drawLine (((float) grid * inc) * getWidth(), 0.f, ((float) grid * inc) * getWidth(), (float) getHeight(), 1.f);
        }
        //else if (grid==683   || grid==1366  || grid==2048  ||
        //	     grid==2731  || grid==3414  || //grid==4096  ||
        //		 grid==4779  || grid==5462  || grid==6144  ||
        //		 grid==6827  || grid==7510  || grid==8192  ||
        //		 grid==8875  || grid==9557  || grid==10240 ||
        //		 grid==10923 || grid==11605 || //grid==12288 ||
        //		 grid==12971 || grid==13653 || grid==14336 ||
        //		 grid==15019 || grid==15701 ) { //semitones for +/-12 range
        else if (grid > 8192 && grid % pbstep == 0)
        {
            g.setColour (Colours::lightgrey);
            g.drawLine (0.f, ((float) (16383 - grid) * inc) * getHeight(), (float) getWidth(), ((float) (16383 - grid) * inc) * getHeight(), 1.f);
            g.drawLine (((float) grid * inc) * getWidth(), 0.f, ((float) grid * inc) * getWidth(), (float) getHeight(), 1.f);
        }
        else if (grid < 8192 && grid % pbstep2 == 0)
        {
            g.setColour (Colours::lightgrey);
            g.drawLine (0.f, ((float) (16383 - grid) * inc) * getHeight(), (float) getWidth(), ((float) (16383 - grid) * inc) * getHeight(), 1.f);
            g.drawLine (((float) grid * inc) * getWidth(), 0.f, ((float) grid * inc) * getWidth(), (float) getHeight(), 1.f);
        }
        else if (grid % 4096 == 0)
        {
            g.setColour (Colours::lightgoldenrodyellow);
            g.drawLine (0.f, ((float) (16383 - grid) * inc) * getHeight(), (float) getWidth(), ((float) (16383 - grid) * inc) * getHeight(), 1.f);
            g.drawLine (((float) grid * inc) * getWidth(), 0.f, ((float) grid * inc) * getWidth(), (float) getHeight(), 1.f);
        }
        if (grid == 8192)
        { //center line
            g.setColour (Colours::blueviolet);
            g.drawLine (0.f, ((float) (16383 - grid) * inc) * getHeight(), (float) getWidth(), ((float) (16383 - grid) * inc) * getHeight(), 2.f);
            g.drawLine (((float) grid * inc) * getWidth(), 0.f, ((float) grid * inc) * getWidth(), (float) getHeight(), 2.f);
        }
        //else {
        //	g.setColour(Colours::ivory);
        //	g.setPixel(((float)grid*inc)*getWidth(),((float)(16383-grid)*inc)*getHeight());
        //}
    }
    Path myPath = plugin->path;
    myPath.applyTransform (AffineTransform::scale ((float) getWidth(), (float) getHeight()));
    g.setColour (Colours::black);
    g.strokePath (myPath, PathStrokeType (2.0f));

    g.setColour (Colours::black);
    g.drawEllipse ((float) getWidth() * (float) roundToInt (16383.0 * (mouseDownPoint.getX() / (double) getWidth())) / 16383.f - 2,
                   (float) getHeight() * (float) roundToInt (16383.0 * (mouseDownPoint.getY() / (double) getHeight())) / 16383.f - 2,
                   4,
                   4,
                   1);
    for (int i = 0; i < MAX_ENVELOPE_POINTS; i++)
    {
        int x = (int) (getWidth() * points[i][0]);
        int y = (int) (getHeight() * points[i][1]);

        if (isPointActive (i))
        {
            g.setColour (Colours::white);
            g.fillEllipse ((float) (x - halfDotSize), (float) (y - halfDotSize), (float) dotSize, (float) dotSize);
            if (draggingPoint == i)
            {
                g.setColour (Colours::red);
                g.drawHorizontalLine (y, 0, (float) getWidth());
                g.drawVerticalLine (x, 0, (float) getHeight());
                g.setColour (Colours::black);
                g.fillEllipse ((float) (x - halfDotSize), (float) (y - halfDotSize), (float) dotSize, (float) dotSize);
                g.setColour (Colours::red);
                g.drawEllipse ((float) (x - halfDotSize), (float) (y - halfDotSize), (float) dotSize, (float) dotSize, 2.f);
            }
            else if (hoveringPoint == i)
            {
                g.setColour (Colours::green);
                g.drawHorizontalLine (y, 0, (float) getWidth());
                g.drawVerticalLine (x, 0, (float) getHeight());
                g.fillEllipse ((float) (x - halfDotSize), (float) (y - halfDotSize), (float) dotSize, (float) dotSize);
            }
            else
            {
                g.setColour (Colours::black);
                if (i == 0 || i == (MAX_ENVELOPE_POINTS - 1))
                    g.drawEllipse ((float) (x - halfDotSize), (float) (y - halfDotSize), (float) dotSize, (float) dotSize, 3);
                else
                    g.drawEllipse ((float) (x - halfDotSize), (float) (y - halfDotSize), (float) dotSize, (float) dotSize, 1);
            }
            if (isPointControl (i))
            {
                g.setColour (Colours::blue);
                g.drawEllipse ((float) (x - dotSize), (float) (y - dotSize), dotSize * 2.f, dotSize * 2.f, 1);
                //g.setColour (Colours::lightblue);
                //float dashes[2] = {2.f,2.f};
                //g.drawDashedLine (x,y,points[i-1][0]*getWidth(),points[i-1][1]*getHeight(),dashes,2);
                //g.drawDashedLine (x,y,points[i+1][0]*getWidth(),points[i+1][1]*getHeight(),dashes,2);
            }
        }
        //if (!isPointActive(i)) {
        //	g.setColour (Colours::darkgrey);
        //	g.drawLine(x-dotSize,y-dotSize,x+dotSize,y+dotSize,1);
        //	g.drawLine(x-dotSize,y+dotSize,x+dotSize,y-dotSize,1);
        //}
        //else if ((!isPointActive(i) || isPointControl(i)) && isPointControl(i-1)) {
        //	x = (int) (getWidth()*points[i-1][0]);
        //	y = (int) (getHeight()*points[i-1][1]);
        //	g.setColour (Colours::red);
        //	g.drawLine(x-dotSize,y-dotSize,x+dotSize,y+dotSize,1);
        //	g.drawLine(x-dotSize,y+dotSize,x+dotSize,y-dotSize,1);
        //}
    }
    g.setColour (Colours::darkgoldenrod);
    g.drawVerticalLine (roundToInt ((float) plugin->lastCCIn * getWidth() / 16383.f),
                        (float) getHeight() - ((float) plugin->lastCCOut * (float) getHeight() / 16383.f),
                        (float) getHeight());
    g.fillEllipse (((float) plugin->lastCCIn * getWidth() / 16383.f) - halfDotSize,
                   (float) getHeight() - ((float) plugin->lastCCOut * (float) getHeight() / 16383.f) - (float) halfDotSize,
                   (float) dotSize,
                   (float) dotSize);
}

//==============================================================================
void MidiEnvelope::mouseDown (const MouseEvent& e)
{
    draggingPoint = findPointByMousePos (e.x, e.y);
    if (draggingPoint != -1)
    {
        mouseDownPoint.setXY (points[draggingPoint][0] * getWidth(), points[draggingPoint][1] * getHeight());
        oldpoints[draggingPoint][0] = points[draggingPoint][0];
        oldpoints[draggingPoint][1] = points[draggingPoint][1];

        if (e.mods.isPopupMenu())
        {
            setPointActive (draggingPoint, false);
            setPointControl (draggingPoint, false);
        }
        else if (e.mods.isMiddleButtonDown() || e.mods.isAltDown())
        {
            if (draggingPoint > 0 && draggingPoint < (MAX_ENVELOPE_POINTS - 1))
                setPointControl (draggingPoint, ! isPointControl (draggingPoint));
        }
        else if (e.mods.isShiftDown())
        {
            if (e.mods.isCommandDown())
            {
                points[draggingPoint][1] = 0.5f;
                plugin->setParameterNotifyingHost ((draggingPoint * 2 + 1), (1.f - points[draggingPoint][1]));
            }
            else
            {
                points[draggingPoint][1] = 1.f - points[draggingPoint][0];
                plugin->setParameterNotifyingHost ((draggingPoint * 2 + 1), (1.f - points[draggingPoint][1]));
            }
        }
        else
        {
            labelX->setColour (Label::textColourId, Colours::red);
            labelY->setColour (Label::textColourId, Colours::red);
            Point<float> p (points[draggingPoint][0], 1.f - points[draggingPoint][1]);
            float pbrange = plugin->getParameter (kPBRange) * 48.f;
            float pbrange2 = plugin->getParameter (kPBRange2) * 48.f;
            if (pbrange2 == 0.f)
                pbrange2 = pbrange;
            if (p.getX() > 0.5f)
            {
                labelX->setText ("x: "
                                     + String (roundToInt (16383.f * p.getX()))
                                     + " (" + String (pbrange * (p.getX() - 0.5f), 2) + ")",
                                 dontSendNotification);
            }
            else
            {
                labelX->setText ("x: "
                                     + String (roundToInt (16383.f * p.getX()))
                                     + " (" + String (pbrange2 * (p.getX() - 0.5f), 2) + ")",
                                 dontSendNotification);
            }
            if (p.getY() > 0.5f)
            {
                labelY->setText ("y: "
                                     + String (roundToInt (16383.f * p.getY()))
                                     + " (" + String (pbrange * (p.getY() - 0.5f), 2) + ")",
                                 dontSendNotification);
            }
            else
            {
                labelY->setText ("y: "
                                     + String (roundToInt (16383.f * p.getY()))
                                     + " (" + String (pbrange2 * (p.getY() - 0.5f), 2) + ")",
                                 dontSendNotification);
            }
        }
    }
    else
    {
        mouseDownPoint.setXY ((float) e.x, (float) e.y);
        if (e.mods.isPopupMenu())
        {
            addPoint ((float) e.x, (float) e.y, false);
        }
        else if (e.mods.isAltDown() || e.mods.isMiddleButtonDown())
        {
            addPoint ((float) e.x, (float) e.y, true);
        }
        else
        {
            for (int i = 0; i < MAX_ENVELOPE_POINTS; i++)
            {
                oldpoints[i][0] = points[i][0];
                oldpoints[i][1] = points[i][1];
            }
        }
        //else {
        //	addPoint((float)e.x,(float)e.y,false);
        //}
    }

    repaint();
}

int MidiEnvelope::findInactivePoint()
{
    for (int i = 1; i < MAX_ENVELOPE_POINTS - 1; i++)
    {
        if (! plugin->isPointActive (i))
            return i;
    }
    return -1;
}

void MidiEnvelope::mouseDoubleClick (const MouseEvent& e)
{
    int p = findPointByMousePos (e.x, e.y);
    if (p != -1)
    {
        setPointActive (p, false);
        setPointControl (p, false);
    }
    else
    {
        if (e.mods.isShiftDown())
        {
            if (e.mods.isCommandDown())
                addPoint ((float) e.x, 0.5f * getHeight(), false);
            else
                addPoint ((float) e.x, (getWidth() - (float) e.x) * getHeight() / getWidth(), false);
        }
        else
            addPoint ((float) e.x, (float) e.y, false);
    }
    repaint();
}

void MidiEnvelope::mouseDrag (const MouseEvent& e)
{
    int restrict = 0;
    if (e.mods.isCommandDown() && ! e.mods.isShiftDown())
    {
        if (abs (e.x - roundToInt (mouseDownPoint.getX())) < abs (e.y - roundToInt (mouseDownPoint.getY())))
        {
            restrict = -1;
        }
        else
            restrict = 1;
    }

    if (draggingPoint != -1)
    {
        int prevPoint = jmax (0, draggingPoint - 1);
        int nextPoint = jmin (MAX_ENVELOPE_POINTS, draggingPoint + 1);
        int paramNumber = draggingPoint * 2;

        // calculate X
        if (draggingPoint == 0)
        {
            points[draggingPoint][0] = 0.f;
        }
        else if (draggingPoint > 0 && draggingPoint < MAX_ENVELOPE_POINTS - 1)
        {
            float snapx = (float) getWidth() * (float) roundToInt (16383.0 * ((double) e.x / (double) getWidth())) / 16383.f;
            points[draggingPoint][0] = restrict == -1 ? oldpoints[draggingPoint][0] : (jmax (jmin (snapx, (float) getWidth()), 0.f)) / (float) getWidth();
        }
        else
        {
            points[draggingPoint][0] = 1.f;
        }
        plugin->setParameterNotifyingHost ((paramNumber), points[draggingPoint][0]);

        // calculate Y
        float snapy = (float) getHeight() * (float) roundToInt (16383.0 * ((double) e.y / (double) getHeight())) / 16383.f;
        if (e.mods.isShiftDown())
        {
            if (e.mods.isCommandDown())
                points[draggingPoint][1] = 0.5f;
            else
                points[draggingPoint][1] = 1.f - points[draggingPoint][0];
        }
        else
            points[draggingPoint][1] = restrict == 1 ? oldpoints[draggingPoint][1] : (jmax (0.f, jmin (snapy, (float) getHeight()))) / (float) getHeight();

        plugin->setParameterNotifyingHost ((paramNumber + 1), (1.f - points[draggingPoint][1]));
        labelX->setColour (Label::textColourId, Colours::red);
        labelY->setColour (Label::textColourId, Colours::red);
        Point<float> p (points[draggingPoint][0], 1.f - points[draggingPoint][1]);
        float pbrange = plugin->getParameter (kPBRange) * 48.f;
        float pbrange2 = plugin->getParameter (kPBRange2) * 48.f;
        if (pbrange2 == 0.f)
            pbrange2 = pbrange;
        if (p.getX() > 0.5f)
        {
            labelX->setText ("x: "
                                 + String (roundToInt (16383.f * p.getX()))
                                 + " (" + String (pbrange * (p.getX() - 0.5f), 2) + ")",
                             dontSendNotification);
        }
        else
        {
            labelX->setText ("x: "
                                 + String (roundToInt (16383.f * p.getX()))
                                 + " (" + String (pbrange2 * (p.getX() - 0.5f), 2) + ")",
                             dontSendNotification);
        }
        if (p.getY() > 0.5f)
        {
            labelY->setText ("y: "
                                 + String (roundToInt (16383.f * p.getY()))
                                 + " (" + String (pbrange * (p.getY() - 0.5f), 2) + ")",
                             dontSendNotification);
        }
        else
        {
            labelY->setText ("y: "
                                 + String (roundToInt (16383.f * p.getY()))
                                 + " (" + String (pbrange2 * (p.getY() - 0.5f), 2) + ")",
                             dontSendNotification);
        }

        repaint();
    }
    else
    {
        if (restrict == -1)
        {
            for (int i = 0; i < MAX_ENVELOPE_POINTS; i++)
            {
                float snapy = (float) getHeight() * (float) roundToInt (16383.0 * ((double) (oldpoints[i][1] * getHeight() + e.y - mouseDownPoint.getY()) / (double) getHeight())) / 16383.f;
                points[i][1] = (jmax (0.f, jmin (snapy, (float) getHeight()))) / (float) getHeight();
                points[i][0] = oldpoints[i][0];
                plugin->setParameterNotifyingHost ((i * 2), (points[i][0]));
                plugin->setParameterNotifyingHost ((i * 2 + 1), (1.f - points[i][1]));
            }
        }
        else if (restrict == 1)
        {
            for (int i = 0; i < MAX_ENVELOPE_POINTS; i++)
            {
                float snapx = (float) getWidth() * (float) roundToInt (16383.0 * ((double) (oldpoints[i][0] * getWidth() + e.x - mouseDownPoint.getX()) / (double) getWidth())) / 16383.f;
                points[i][0] = (jmax (jmin (snapx, (float) getWidth()), 0.f)) / (float) getWidth();
                points[i][1] = oldpoints[i][1];
                plugin->setParameterNotifyingHost ((i * 2), (points[i][0]));
                plugin->setParameterNotifyingHost ((i * 2 + 1), (1.f - points[i][1]));
            }
        }
        labelX->setColour (Label::textColourId, Colours::black);
        labelY->setColour (Label::textColourId, Colours::black);
        repaint();
    }
}

void MidiEnvelope::mouseUp (const MouseEvent& e)
{
    labelX->setColour (Label::textColourId, Colours::black);
    labelY->setColour (Label::textColourId, Colours::black);
    draggingPoint = -1;

    repaint();
}

void MidiEnvelope::mouseMove (const MouseEvent& e)
{
    hoveringPoint = findPointByMousePos (e.x, e.y);
    if (hoveringPoint != -1)
    {
        //int x = points[hoveringPoint][0]*getWidth()+16;
        //int y = points[hoveringPoint][1]*getHeight();
        //if (x>(getWidth()-labelX->getWidth())) x -= labelX->getWidth()-4;
        //if (y>(getHeight()-2*labelX->getHeight())) y -= labelX->getHeight()*2-4;

        //labelX->setTopLeftPosition(x,y);
        //labelY->setTopLeftPosition(x,y+labelX->getHeight());
        labelX->setColour (Label::textColourId, Colours::green);
        labelY->setColour (Label::textColourId, Colours::green);

        Point<float> p (points[hoveringPoint][0], 1.f - points[hoveringPoint][1]);
        float pbrange = plugin->getParameter (kPBRange) * 48.f;
        float pbrange2 = plugin->getParameter (kPBRange2) * 48.f;
        if (pbrange2 == 0.f)
            pbrange2 = pbrange;
        if (p.getX() > 0.5f)
        {
            labelX->setText ("x: "
                                 + String (roundToInt (16383.f * p.getX()))
                                 + " (" + String (pbrange * (p.getX() - 0.5f), 2) + ")",
                             dontSendNotification);
        }
        else
        {
            labelX->setText ("x: "
                                 + String (roundToInt (16383.f * p.getX()))
                                 + " (" + String (pbrange2 * (p.getX() - 0.5f), 2) + ")",
                             dontSendNotification);
        }
        if (p.getY() > 0.5f)
        {
            labelY->setText ("y: "
                                 + String (roundToInt (16383.f * p.getY()))
                                 + " (" + String (pbrange * (p.getY() - 0.5f), 2) + ")",
                             dontSendNotification);
        }
        else
        {
            labelY->setText ("y: "
                                 + String (roundToInt (16383.f * p.getY()))
                                 + " (" + String (pbrange2 * (p.getY() - 0.5f), 2) + ")",
                             dontSendNotification);
        }
    }
    else
    {
        //labelX->setTopLeftPosition(e.x+16,e.y);
        //labelY->setTopLeftPosition(e.x+16,e.y+labelX->getHeight());
        labelX->setColour (Label::textColourId, Colours::black);
        labelY->setColour (Label::textColourId, Colours::black);

        Point<float> p ((float) e.x / (float) getWidth(), 1.f - (float) e.y / (float) getHeight());
        float pbrange = plugin->getParameter (kPBRange) * 48.f;
        float pbrange2 = plugin->getParameter (kPBRange2) * 48.f;
        if (pbrange2 == 0.f)
            pbrange2 = pbrange;
        if (p.getX() > 0.5f)
        {
            labelX->setText ("x: "
                                 + String (roundToInt (16383.f * p.getX()))
                                 + " (" + String (pbrange * (p.getX() - 0.5f), 2) + ")",
                             dontSendNotification);
        }
        else
        {
            labelX->setText ("x: "
                                 + String (roundToInt (16383.f * p.getX()))
                                 + " (" + String (pbrange2 * (p.getX() - 0.5f), 2) + ")",
                             dontSendNotification);
        }
        if (p.getY() > 0.5f)
        {
            labelY->setText ("y: "
                                 + String (roundToInt (16383.f * p.getY()))
                                 + " (" + String (pbrange * (p.getY() - 0.5f), 2) + ")",
                             dontSendNotification);
        }
        else
        {
            labelY->setText ("y: "
                                 + String (roundToInt (16383.f * p.getY()))
                                 + " (" + String (pbrange2 * (p.getY() - 0.5f), 2) + ")",
                             dontSendNotification);
        }
    }

    repaint();
}

int MidiEnvelope::addPoint (float x, float y, bool control)
{
    int newPoint = findInactivePoint();
    if (newPoint != -1)
    {
        setPointActive (newPoint, true);
        setPointControl (newPoint, control);
        float snapx = (float) getWidth() * (float) roundToInt (16383.0 * (x / (double) getWidth())) / 16383.f;
        points[newPoint][0] = (jmax (jmin (snapx, (float) getWidth()), 0.f)) / (float) getWidth();
        float snapy = (float) getHeight() * (float) roundToInt (16383.0 * (y / (double) getHeight())) / 16383.f;
        points[newPoint][1] = (jmax (0.f, jmin (snapy, (float) getHeight()))) / (float) getHeight();
        plugin->setParameterNotifyingHost ((newPoint * 2), points[newPoint][0]);
        plugin->setParameterNotifyingHost ((newPoint * 2 + 1), (1.f - points[newPoint][1]));
    }
    return newPoint;
}

void MidiEnvelope::setPointActive (int point, bool active)
{
    if (isPointControl (point))
        plugin->setParameter (point + kActive, active ? 0.75f : 0.25f);
    else
        plugin->setParameter (point + kActive, active ? 1.f : 0.f);
}

void MidiEnvelope::setPointControl (int point, bool control)
{
    if (isPointActive (point))
        plugin->setParameter (point + kActive, control ? 0.75f : 1.f);
    else
        plugin->setParameter (point + kActive, control ? 0.25f : 0.f);
}

bool MidiEnvelope::isPointActive (int point)
{
    if (point < 0)
        return false;
    return plugin->getParameter (point + kActive) > 0.5f;
}

bool MidiEnvelope::isPointControl (int point)
{
    if (point < 0)
        return false;
    return plugin->getParameter (point + kActive) > 0.1f && plugin->getParameter (point + kActive) < 0.9f;
}

//==============================================================================
int MidiEnvelope::findPointByMousePos (const int x, const int y)
{
    const int pixelSnap = MAX_ENVELOPE_DOT_SIZE;

    for (int i = 0; i < MAX_ENVELOPE_POINTS; i++)
    {
        if ((x >= (points[i][0] * getWidth() - pixelSnap) && x <= (points[i][0] * getWidth() + pixelSnap))
            && (y >= (points[i][1] * getHeight() - pixelSnap) && y <= (points[i][1] * getHeight() + pixelSnap)))
        {
            if (isPointActive (i))
                return i;
        }
    }
    return -1;
}

void MidiEnvelope::updateParameters (const bool repaintComponent)
{
    for (int i = 0; i < MAX_ENVELOPE_POINTS; i++)
    {
        points[i][0] = plugin->getParameter (i * 2);
        points[i][1] = 1.f - plugin->getParameter (i * 2 + 1);
    }

    if (repaintComponent)
        repaint();
}
