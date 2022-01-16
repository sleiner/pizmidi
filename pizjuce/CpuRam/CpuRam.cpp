//#include "CpuRam.h"
#include "CpuRamEditor.h"

//==============================================================================
/**
    This function must be implemented to create the actual plugin object that
    you want to use.
*/
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CpuRam();
}

//==============================================================================
CpuRam::CpuRam()
{
    if (! loadDefaultFxb())
        resetToDefaultSettings();
}

void CpuRam::resetToDefaultSettings()
{
    interval     = 0.12f;
    minimize     = 0.0f;
    showGraph    = true;
    lastUIWidth  = 230;
    lastUIHeight = 30;
    bgcolor      = Colour (0xffb8bcc0);
}

CpuRam::~CpuRam()
{
}

//==============================================================================
int CpuRam::getNumParameters()
{
    return 3;
}

float CpuRam::getParameter (int index)
{
    if (index == 0)
        return interval;
    if (index == 1)
        return minimize;
    if (index == 2)
        return showGraph ? 1.f : 0.f;
    else
        return 0.0f;
}

void CpuRam::setParameter (int index, float newValue)
{
    if (index == 0)
    {
        if (interval != newValue)
        {
            interval = newValue;
            //sendChangeMessage (this);
        }
    }
    else if (index == 1)
    {
        if (minimize != newValue)
        {
            minimize = newValue;
            if (minimize >= 0.5f)
                lastUIHeight = 0;
            else
                lastUIHeight = 30;
            sendChangeMessage();
        }
    }
    else if (index == 2)
    {
        if (showGraph != (newValue >= 0.5f))
        {
            showGraph = newValue >= 0.5f;
            if (showGraph)
                lastUIHeight = 30;
            else
                lastUIHeight = 15;
            sendChangeMessage();
        }
    }
}

const String CpuRam::getParameterName (int index)
{
    if (index == 0)
        return "interval";
    if (index == 1)
        return "minimize";
    if (index == 2)
        return "show graph";
    else
        return String();
}

const String CpuRam::getParameterText (int index)
{
    if (index == 0)
    {
        return String ((int) (interval * 1700.0) + 300) + String (" ms");
    }
    if (index == 1)
    {
        if (minimize >= 0.5f)
            return String ("yes");
        else
            return String ("no");
    }
    if (index == 2)
    {
        return showGraph ? String ("yes") : String ("no");
    }
    else
        return String();
}
const String CpuRam::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String CpuRam::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool CpuRam::isInputChannelStereoPair (int index) const
{
    return true;
}

bool CpuRam::isOutputChannelStereoPair (int index) const
{
    return true;
}

//==============================================================================
void CpuRam::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // do your pre-playback setup stuff here..
}

void CpuRam::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void CpuRam::processBlock (AudioSampleBuffer& buffer,
                           MidiBuffer& midiMessages)
{
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    midiMessages.clear();
}

//==============================================================================
AudioProcessorEditor* CpuRam::createEditor()
{
    return new CpuRamEditor (this);
}

//==============================================================================
void CpuRam::getStateInformation (JUCE_NAMESPACE::MemoryBlock& destData)
{
    // you can store your parameters as binary data if you want to or if you've got
    // a load of binary to put in there, but if you're not doing anything too heavy,
    // XML is a much cleaner way of doing it - here's an example of how to store your
    // params as XML..

    // create an outer XML element..
    XmlElement xmlState ("MYPLUGINSETTINGS");

    // add some attributes to it..
    xmlState.setAttribute ("pluginVersion", 1);
    xmlState.setAttribute ("intervalLevel", interval);
    xmlState.setAttribute ("showGraph", showGraph);
    xmlState.setAttribute ("uiWidth", lastUIWidth);
    xmlState.setAttribute ("uiHeight", lastUIHeight);
    xmlState.setAttribute ("bgcolor", (int) (bgcolor.getARGB()));

    // you could also add as many child elements as you need to here..

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xmlState, destData);
}

void CpuRam::setStateInformation (const void* data, int sizeInBytes)
{
    // use this helper function to get the XML from this binary blob..
    auto const xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != 0)
    {
        // check that it's the right type of xml..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            interval     = (float) xmlState->getDoubleAttribute ("intervalLevel", interval);
            showGraph    = xmlState->getBoolAttribute ("showGraph", showGraph);
            lastUIWidth  = xmlState->getIntAttribute ("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);
            bgcolor      = Colour (xmlState->getIntAttribute ("bgcolor", bgcolor.getARGB()));
            sendChangeMessage();
        }
    }
}
