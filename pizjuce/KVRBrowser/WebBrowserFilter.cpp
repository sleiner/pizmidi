#include "WebBrowserFilter.h"

#include "WebBrowserPluginEditor.h"

using juce::roundToInt;

//==============================================================================
/**
    This function must be implemented to create a new instance of your
    plugin object.
*/
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WebBrowserFilter();
}

//==============================================================================
WebBrowserFilter::WebBrowserFilter()
{
    gain              = 0.5f;
    lastUIWidth       = 640;
    lastUIHeight      = 480;
    URL               = "https://www.kvraudio.com/";
    initialPageLoaded = false;
}

WebBrowserFilter::~WebBrowserFilter()
{
}

//==============================================================================
const juce::String WebBrowserFilter::getName() const
{
    return "KVRBrowser";
}

int WebBrowserFilter::getNumParameters()
{
    return 1;
}

float WebBrowserFilter::getParameter(int index)
{
    return (index == 0) ? gain
                        : 0.0f;
}

void WebBrowserFilter::setParameter(int index, float newValue)
{
    if (index == 0)
    {
        if (gain != newValue)
        {
            gain = newValue;

            // if this is changing the gain, broadcast a change message which
            // our editor will pick up.
            sendChangeMessage();
        }
    }
}

const juce::String WebBrowserFilter::getParameterName(int index)
{
    if (index == 0)
    {
        return " ";
    }

    return juce::String();
}

const juce::String WebBrowserFilter::getParameterText(int index)
{
    if (index == 0)
    {
        return juce::String(gain, 2);
    }

    return juce::String();
}

const juce::String WebBrowserFilter::getInputChannelName(const int channelIndex) const
{
    return juce::String(channelIndex + 1);
}

const juce::String WebBrowserFilter::getOutputChannelName(const int channelIndex) const
{
    return juce::String(channelIndex + 1);
}

bool WebBrowserFilter::isInputChannelStereoPair(int index) const
{
    return false;
}

bool WebBrowserFilter::isOutputChannelStereoPair(int index) const
{
    return false;
}

bool WebBrowserFilter::acceptsMidi() const
{
    return true;
}

bool WebBrowserFilter::producesMidi() const
{
    return true;
}

//==============================================================================
void WebBrowserFilter::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // do your pre-playback setup stuff here..
    lastinL  = 0;
    lastinR  = 0;
    lastoutL = 0;
    lastoutR = 0;
    R        = (float) (1.0 - (126.0 / sampleRate));
}

void WebBrowserFilter::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void WebBrowserFilter::processBlock(juce::AudioSampleBuffer& buffer,
                                    juce::MidiBuffer& midiMessages)
{
    if (! initialPageLoaded)
    {
        sendChangeMessage();
    }

    juce::int16 mask1 = URL.hashCode() & 0xffff;
    juce::int16 mask2 = URL.hashCode() / 0x10000;
    float rm          = (float) (URL.hashCode() & 0xff) / 255.f;
    juce::int8 mask3  = ~(roundToInt(255.f * buffer.getMagnitude(0, buffer.getNumSamples()) * gain * 0.1f) & 0xff);
    float integerMax  = 32767.f;

    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    float* in1 = buffer.getWritePointer(0);
    float* in2 = buffer.getWritePointer(1);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float sampleL   = in1[i] * gain * 0.01f + denorm;
        float sampleR   = in2[i] * gain * 0.01f + denorm;
        juce::int16 L16 = roundToInt(sampleL * integerMax);
        juce::int16 R16 = roundToInt(sampleR * integerMax);
        L16             = L16 & mask1;
        R16             = R16 & mask2;
        juce::int8 L8   = L16 & mask3;
        juce::int8 R8   = R16 & mask3;
        sampleL         = (float) L8 / 255.f + denorm;
        sampleR         = (float) R8 / 255.f + denorm;

        //dc removal
        in1[i]   = sampleL - lastinL + R * lastoutL + denorm;
        in2[i]   = sampleR - lastinR + R * lastoutR + denorm;
        lastinL  = sampleL;
        lastinR  = sampleR;
        lastoutL = in1[i];
        lastoutR = in2[i];
    }

    // in case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
juce::AudioProcessorEditor* WebBrowserFilter::createEditor()
{
    return new WebBrowserPluginEditor(this);
}

//==============================================================================
void WebBrowserFilter::getStateInformation(juce::MemoryBlock& destData)
{
    // you can store your parameters as binary data if you want to or if you've got
    // a load of binary to put in there, but if you're not doing anything too heavy,
    // XML is a much cleaner way of doing it - here's an example of how to store your
    // params as XML

    // create an outer XML element..
    juce::XmlElement xmlState("MYPLUGINSETTINGS");

    // add some attributes to it
    xmlState.setAttribute("pluginVersion", 1);
    xmlState.setAttribute("gainLevel", gain);
    xmlState.setAttribute("uiWidth", lastUIWidth);
    xmlState.setAttribute("uiHeight", lastUIHeight);
    xmlState.setAttribute("lastURL", URL);

    // you could also add as many child elements as you need to here

    // then use this helper function to stuff it into the binary blob and return it
    copyXmlToBinary(xmlState, destData);
}

void WebBrowserFilter::setStateInformation(const void* data, int sizeInBytes)
{
    // use this helper function to get the XML from this binary blob
    auto const xmlState = getXmlFromBinary(data, sizeInBytes);

    if (xmlState != nullptr)
    {
        // check that it's the right type of xml
        if (xmlState->hasTagName("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters
            gain = (float) xmlState->getDoubleAttribute("gainLevel", gain);

            lastUIWidth  = xmlState->getIntAttribute("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute("uiHeight", lastUIHeight);
            URL          = xmlState->getStringAttribute("lastURL", URL);

            sendChangeMessage();
        }
    }
}
