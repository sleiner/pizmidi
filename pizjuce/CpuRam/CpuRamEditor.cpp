#include "CpuRamEditor.h"

//==============================================================================
CpuRamEditor::CpuRamEditor (CpuRam* const ownerFilter)
    : AudioProcessorEditor (ownerFilter)
{
	setMouseClickGrabsKeyboardFocus(false);

	addAndMakeVisible (infoLabel = new Label(String("CPU"),String()));
	infoLabel->setMouseClickGrabsKeyboardFocus(false);
	infoLabel->setInterceptsMouseClicks(false,false);

	addAndMakeVisible (memLabel2 = new Label(String("RAM"),String()));
	memLabel2->setMouseClickGrabsKeyboardFocus(false);
	memLabel2->setInterceptsMouseClicks(false,false);

	addAndMakeVisible (slider = new Slider(String("interval")));
    slider->setMouseClickGrabsKeyboardFocus(false);
    slider->setSliderStyle(Slider::LinearBar);
    slider->setRange(300,2000,1);
    slider->setTextValueSuffix(String(" ms"));
    slider->addListener(this);

	addAndMakeVisible (graph = new CpuGraph());

    colourSelector = new ColourSelector(ColourSelector::showColourAtTop|ColourSelector::showSliders|ColourSelector::showColourspace);
    colourSelector->setName (String("color"));
    colourSelector->setCurrentColour (getFilter()->bgcolor);
    colourSelector->addChangeListener (this);

    // add the triangular resizer component for the bottom-right of the UI
    //addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));
    //resizer->setMouseClickGrabsKeyboardFocus(false);
    //resizeLimits.setSizeLimits (10, 10, 1200, 200);

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize (ownerFilter->lastUIWidth,
             ownerFilter->lastUIHeight);

    lastMinimize=getFilter()->getParameter(1);

    // register ourselves with the filter - it will use its ChangeBroadcaster base
    // class to tell us when something has changed, and this will call our changeListenerCallback()
    // method.
    ownerFilter->addChangeListener (this);

    oi64.QuadPart=0;
    ok64.QuadPart=0;
    ou64.QuadPart=0;

    updateParametersFromFilter();
}

CpuRamEditor::~CpuRamEditor()
{
    getFilter()->removeChangeListener (this);

    deleteAllChildren();
}

//==============================================================================
void CpuRamEditor::paint (Graphics& g)
{
    g.fillAll (getFilter()->bgcolor);
}

void CpuRamEditor::resized()
{
	//int h=getHeight();
	//int w=getWidth();
    infoLabel->setBounds (0, 2, 115, 11);
	if (getFilter()->getParameter(2)<0.5f) {
		memLabel2->setBounds (92, 2, 136, 11);
		graph->setVisible(false);
	}
    else {
		memLabel2->setBounds (0, 17, 115, 11);
		graph->setVisible(true);
		graph->setBounds (136, 1, 93, 28);
	}

    //resizer->setBounds (w - 16, h - 16, 16, 16);

    // if we've been resized, tell the filter so that it can store the new size
    // in its settings
    //getFilter()->lastUIWidth = w;
    //getFilter()->lastUIHeight = h;
}


void CpuRamEditor::sliderValueChanged(Slider* sliderThatWasMoved) {
    getFilter()->setParameterNotifyingHost(0,(float)(sliderThatWasMoved->getValue()-300)/1700.0f);
}

float CpuRamEditor::CPULoad() {
   return pu.GetUsage();
}

MEMORYSTATUSEX CpuRamEditor::RAMLoad() {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);

    GlobalMemoryStatusEx (&statex);
    return statex;
}

void CpuRamEditor::timerCallback()
{
	updateParametersFromFilter();
}

void CpuRamEditor::mouseUp (const MouseEvent& e)
{
	if (e.mods.isPopupMenu()) {
        PopupMenu m;
		colourSelector->setCurrentColour (getFilter()->bgcolor);
		m.addItem(3,"Show Graph",true,getFilter()->showGraph);
		m.addSectionHeader("Interval");
		m.addCustomItem (1, *slider, 230, 18, false);
		m.addSectionHeader("Background Color");
        m.addCustomItem (2, *colourSelector, 230, 230, false);
		int result = m.show();
		if (result==3) {
			getFilter()->setParameter(2,getFilter()->showGraph ? 0.f : 1.f);
		}
	}
}

//==============================================================================
void CpuRamEditor::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source==getFilter()) {
        updateParametersFromFilter();
    }
    else {
        ColourSelector* cs = (ColourSelector*) source;
        getFilter()->bgcolor=(cs->getCurrentColour());
		infoLabel->setColour(Label::textColourId,getFilter()->bgcolor.contrasting(0.8f));
		memLabel2->setColour(Label::textColourId,getFilter()->bgcolor.contrasting(0.8f));
		slider->setColour(Slider::textBoxTextColourId,getFilter()->bgcolor.contrasting(0.8f));
        repaint();
    }
}

//==============================================================================
void CpuRamEditor::updateParametersFromFilter()
{
    CpuRam* const filter = getFilter();

    lastMinimize = filter->getParameter(1);
    setSize (filter->lastUIWidth, filter->lastUIHeight);
	float cpu = CPULoad();
	graph->addPoint(cpu*0.01f);
	if (getFilter()->showGraph)
		infoLabel->setText(String("CPU Load: ") + String(cpu,1)+String("%"),dontSendNotification);
	else
		infoLabel->setText(String("CPU: ") + String(cpu,1)+String("%"),dontSendNotification);

    memLabel2->setText(String("Free RAM: ") + String((int)RAMLoad().ullAvailPhys/1048576)+String("MB"),dontSendNotification);

	infoLabel->setColour(Label::textColourId,filter->bgcolor.contrasting(0.8f));
	memLabel2->setColour(Label::textColourId,filter->bgcolor.contrasting(0.8f));
	slider->setColour(Slider::textBoxTextColourId,filter->bgcolor.contrasting(0.8f));

	float interval = filter->getParameter(0)*1700.0f+300.0f;
    slider->setValue(interval,dontSendNotification);
	startTimer((int)interval);
	pu.setInterval((int)interval);

	if (!lastMinimize) repaint();
}
