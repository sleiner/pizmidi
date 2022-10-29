#ifndef DEMOJUCEPLUGINEDITOR_H
#define DEMOJUCEPLUGINEDITOR_H

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "CpuRam.h"
#include "cputime.h"

class CpuGraph : public juce::Component
{
public:
    CpuGraph()
    {
        for (int i = 0; i < numPoints; i++)
            points[i] = 0.f;
        setMouseClickGrabsKeyboardFocus (false);
    }
    ~CpuGraph() override{};

    void addPoint (float value)
    {
        for (int i = 1; i < numPoints; i++)
            points[i - 1] = points[i];
        points[numPoints - 1] = value;
        this->repaint();
    }

private:
    enum
    {
        numPoints = 128
    };
    float points[numPoints];
    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::black);
        g.setColour (juce::Colours::green);
        for (int i = 0; i < numPoints; i++)
        {
            float x = ((float) i / float (numPoints)) * getWidth();
            g.drawLine (x, (float) getHeight(), x, (float) getHeight() - (float) getHeight() * points[i]);
        }
    }
};

//==============================================================================
/**
    This is the Component that our filter will use as its UI.

    One or more of these is created by the CpuRam::createEditor() method,
    and they will be deleted at some later time by the wrapper code.

    To demonstrate the correct way of connecting a filter to its UI, this
    class is a ChangeListener, and our demo filter is a ChangeBroadcaster. The
    editor component registers with the filter when it's created and deregisters
    when it's destroyed. When the filter's parameters are changed, it broadcasts
    a message and this editor responds by updating its display.
*/
class CpuRamEditor : public juce::AudioProcessorEditor,
                     public juce::ChangeListener,
                     public juce::Slider::Listener,
                     public juce::Timer
{
public:
    /** Constructor.

        When created, this will register itself with the filter for changes. It's
        safe to assume that the filter won't be deleted before this object is.
    */
    CpuRamEditor (CpuRam* const ownerFilter);

    /** Destructor. */
    ~CpuRamEditor() override;

    //==============================================================================
    /** Our demo filter is a ChangeBroadcaster, and will call us back when one of
        its parameters changes.
    */
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    void timerCallback() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void mouseUp (const juce::MouseEvent&) override;

    //==============================================================================
    /** Standard Juce paint callback. */
    void paint (juce::Graphics& g) override;

    /** Standard Juce resize callback. */
    void resized() override;

private:
    //==============================================================================
    juce::Label* infoLabel;
    juce::Label* memLabel2;
    juce::Slider* slider;
    CpuGraph* graph;
    juce::ColourSelector* colourSelector;
    //TooltipWindow tooltipWindow;
    //ResizableCornerComponent* resizer;
    //ComponentBoundsConstrainer resizeLimits;

    CProcessorUsage pu;
    float CPULoad();
    MEMORYSTATUSEX RAMLoad();
    LARGE_INTEGER oi64, ok64, ou64;

    float lastMinimize;

    void updateParametersFromFilter();

    // handy wrapper method to avoid having to cast the filter to a CpuRam
    // every time we need it
    CpuRam* getFilter() const throw()
    {
        return (CpuRam*) getAudioProcessor();
    }
};

#endif
