#include "CpuRam.h"

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
    {
        resetToDefaultSettings();
    }
}

void CpuRam::resetToDefaultSettings()
{
    interval     = 0.12f;
    minimize     = 0.0f;
    showGraph    = true;
    lastUIWidth  = 230;
    lastUIHeight = 30;
    bgcolor      = juce::Colour(0xffb8bcc0);
}

CpuRam::~CpuRam()
{
}

//==============================================================================
int CpuRam::getNumParameters()
{
    return 3;
}

float CpuRam::getParameter(int index)
{
    if (index == 0)
    {
        return interval;
    }
    if (index == 1)
    {
        return minimize;
    }
    if (index == 2)
    {
        return showGraph ? 1.f : 0.f;
    }
    else
    {
        return 0.0f;
    }
}

void CpuRam::setParameter(int index, float newValue)
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
            {
                lastUIHeight = 0;
            }
            else
            {
                lastUIHeight = 30;
            }
            sendChangeMessage();
        }
    }
    else if (index == 2)
    {
        if (showGraph != (newValue >= 0.5f))
        {
            showGraph = newValue >= 0.5f;
            if (showGraph)
            {
                lastUIHeight = 30;
            }
            else
            {
                lastUIHeight = 15;
            }
            sendChangeMessage();
        }
    }
}

const juce::String CpuRam::getParameterName(int index)
{
    if (index == 0)
    {
        return "interval";
    }
    if (index == 1)
    {
        return "minimize";
    }
    if (index == 2)
    {
        return "show graph";
    }
    else
    {
        return juce::String();
    }
}

const juce::String CpuRam::getParameterText(int index)
{
    if (index == 0)
    {
        return juce::String((int) (interval * 1700.0) + 300) + juce::String(" ms");
    }
    if (index == 1)
    {
        if (minimize >= 0.5f)
        {
            return juce::String("yes");
        }
        else
        {
            return juce::String("no");
        }
    }
    if (index == 2)
    {
        return showGraph ? juce::String("yes") : juce::String("no");
    }
    else
    {
        return juce::String();
    }
}

const juce::String CpuRam::getInputChannelName(const int channelIndex) const
{
    return juce::String(channelIndex + 1);
}

const juce::String CpuRam::getOutputChannelName(const int channelIndex) const
{
    return juce::String(channelIndex + 1);
}

bool CpuRam::isInputChannelStereoPair(int index) const
{
    return true;
}

bool CpuRam::isOutputChannelStereoPair(int index) const
{
    return true;
}

//==============================================================================
void CpuRam::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // do your pre-playback setup stuff here..
}

void CpuRam::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void CpuRam::processBlock(juce::AudioSampleBuffer& buffer,
                          juce::MidiBuffer& midiMessages)
{
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    midiMessages.clear();
}

//==============================================================================
juce::AudioProcessorEditor* CpuRam::createEditor()
{
    return new CpuRamEditor(this);
}

//==============================================================================
void CpuRam::getStateInformation(juce::MemoryBlock& destData)
{
    // you can store your parameters as binary data if you want to or if you've got
    // a load of binary to put in there, but if you're not doing anything too heavy,
    // XML is a much cleaner way of doing it - here's an example of how to store your
    // params as XML

    // create an outer XML element
    juce::XmlElement xmlState("MYPLUGINSETTINGS");

    // add some attributes to it
    xmlState.setAttribute("pluginVersion", 1);
    xmlState.setAttribute("intervalLevel", interval);
    xmlState.setAttribute("showGraph", showGraph);
    xmlState.setAttribute("uiWidth", lastUIWidth);
    xmlState.setAttribute("uiHeight", lastUIHeight);
    xmlState.setAttribute("bgcolor", (int) (bgcolor.getARGB()));

    // you could also add as many child elements as you need to here

    // then use this helper function to stuff it into the binary blob and return it
    copyXmlToBinary(xmlState, destData);
}

void CpuRam::setStateInformation(const void* data, int sizeInBytes)
{
    // use this helper function to get the XML from this binary blob
    auto const xmlState = getXmlFromBinary(data, sizeInBytes);

    if (xmlState != nullptr)
    {
        // check that it's the right type of xml
        if (xmlState->hasTagName("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters
            interval     = (float) xmlState->getDoubleAttribute("intervalLevel", interval);
            showGraph    = xmlState->getBoolAttribute("showGraph", showGraph);
            lastUIWidth  = xmlState->getIntAttribute("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute("uiHeight", lastUIHeight);
            bgcolor      = juce::Colour(xmlState->getIntAttribute("bgcolor", bgcolor.getARGB()));
            sendChangeMessage();
        }
    }
}
