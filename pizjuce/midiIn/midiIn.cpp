#include "midiIn.h"
#include "midiInEditor.h"

using juce::roundToInt;

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiInFilter();
}

JuceProgram::JuceProgram()
{
    //default values
    param[kPower]   = 0.0f;
    param[kHostIn]  = 1.0f;
    param[kChannel] = 0.f;

    icon = juce::String(""); // icon filename

    //program name
    name = "Default";
}

//==============================================================================
MidiInFilter::MidiInFilter()
{
    programs = new JuceProgram[getNumPrograms()];

    devices   = juce::MidiInput::getAvailableDevices();
    midiInput = nullptr;
    loadDefaultFxb();
    curProgram = 0;
    init       = true;
    setCurrentProgram(0);

    wasPlaying = false;

    zeromem(&lastPosInfo, sizeof(lastPosInfo));
}

//==============================================================================
int MidiInFilter::getNumParameters()
{
    return numParams;
}

float MidiInFilter::getParameter(int index)
{
    return param[index];
}

void MidiInFilter::setParameter(int index, float newValue)
{
    JuceProgram* ap = &programs[curProgram];

    if (param[index] != newValue)
    {
        param[index] = ap->param[index] = newValue;
        if (index == kPower)
        {
            if (param[kPower] > 0.0)
            {
                setActiveDevice(activeDevice);
            }
            else
            {
                if (midiInput)
                    midiInput->stop();
                midiInput = nullptr;
            }
        }
        sendChangeMessage();
    }
}

void MidiInFilter::setActiveDevice(juce::String name)
{
    setActiveDevice(getDeviceByName(name));
}

void MidiInFilter::setActiveDevice(juce::MidiDeviceInfo device)
{
    if (midiInput != nullptr)
    {
        midiInput->stop();
    }
    activeDevice = programs[curProgram].device = device;
    midiInput                                  = juce::MidiInput::openDevice(device.identifier, &collector);
    if (midiInput != nullptr)
    {
        midiInput->start();
    }
}

juce::MidiDeviceInfo MidiInFilter::getDeviceByName(juce::String name) const
{
    return devices.findIf([&](auto const& device)
                          { return name == device.name; });
}

const juce::String MidiInFilter::getParameterName(int index)
{
    if (index == kPower)
        return "Power";
    if (index == kHostIn)
        return "HostIn";
    return juce::String();
}

const juce::String MidiInFilter::getParameterText(int index)
{
    if (index == kPower)
    {
        if (param[kPower] > 0.f)
            return juce::String("On");
        else
            return juce::String("Off");
    }
    if (index == kHostIn)
    {
        if (param[kHostIn] >= 0.5)
            return juce::String("On");
        else
            return juce::String("Off");
    }
    return juce::String();
}

const juce::String MidiInFilter::getInputChannelName(const int channelIndex) const
{
    return juce::String(channelIndex + 1);
}

const juce::String MidiInFilter::getOutputChannelName(const int channelIndex) const
{
    return juce::String(channelIndex + 1);
}

bool MidiInFilter::isInputChannelStereoPair(int index) const
{
    if (getNumInputChannels() == 2)
        return true;
    else
        return false;
}

bool MidiInFilter::isOutputChannelStereoPair(int index) const
{
    if (getNumOutputChannels() == 2)
        return true;
    else
        return false;
}
void MidiInFilter::setCurrentProgram(int index)
{
    if (! init)
    {
        programs[curProgram].icon = icon;
    }
    init = false;

    JuceProgram* ap = &programs[index];
    curProgram      = index;
    setActiveDevice(ap->device);
    for (int i = 0; i < getNumParameters(); i++)
    {
        setParameter(i, ap->param[i]);
    }
    icon = ap->icon;
}

void MidiInFilter::changeProgramName(int index, const juce::String& newName)
{
    programs[curProgram].name = newName;
}

const juce::String MidiInFilter::getProgramName(int index)
{
    return programs[index].name;
}

int MidiInFilter::getCurrentProgram()
{
    return curProgram;
}
//==============================================================================
juce::AudioProcessorEditor* MidiInFilter::createEditor()
{
    return new MidiInEditor(this);
}
//==============================================================================
void MidiInFilter::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    this->collector.reset(sampleRate);
    if (midiInput)
        midiInput->start();
}

void MidiInFilter::releaseResources()
{
    if (midiInput)
        midiInput->stop();
    this->collector.reset(44100);
}

void MidiInFilter::processBlock(juce::AudioSampleBuffer& buffer,
                                juce::MidiBuffer& midiMessages)
{
    for (int i = 0; i < getNumOutputChannels(); ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    const int channel = roundToInt(param[kChannel] * 16.f);
    if (param[kHostIn] < 0.5f)
        midiMessages.clear();
    collector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples());
    if (channel > 0)
    {
        juce::MidiBuffer output;
        for (auto&& msgMetadata : midiMessages)
        {
            auto midi_message = msgMetadata.getMessage();

            if (midi_message.getChannel() == 0 || midi_message.getChannel() == channel)
            {
                output.addEvent(midi_message, msgMetadata.samplePosition);
            }
        }
        midiMessages.clear();
        midiMessages = output;
    }
}

//==============================================================================
void MidiInFilter::getCurrentProgramStateInformation(juce::MemoryBlock& destData)
{
    // make sure the non-parameter settings are copied to the current program
    programs[curProgram].icon = icon;
    // you can store your parameters as binary data if you want to or if you've got
    // a load of binary to put in there, but if you're not doing anything too heavy,
    // XML is a much cleaner way of doing it - here's an example of how to store your
    // params as XML..

    // create an outer XML element..
    juce::XmlElement xmlState("MYPLUGINSETTINGS");

    // add some attributes to it..
    xmlState.setAttribute("pluginVersion", 1);

    xmlState.setAttribute("program", getCurrentProgram());
    xmlState.setAttribute("progname", getProgramName(getCurrentProgram()));

    for (int i = 0; i < getNumParameters(); i++)
    {
        xmlState.setAttribute(juce::String(i), param[i]);
    }

    xmlState.setAttribute("icon", icon);
    xmlState.setAttribute("device", activeDevice.name);

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary(xmlState, destData);
}
void MidiInFilter::getStateInformation(juce::MemoryBlock& destData)
{
    // make sure the non-parameter settings are copied to the current program
    programs[curProgram].icon = icon;

    juce::XmlElement xmlState("MYPLUGINSETTINGS");
    xmlState.setAttribute("pluginVersion", 1);
    xmlState.setAttribute("program", getCurrentProgram());
    for (int p = 0; p < getNumPrograms(); p++)
    {
        juce::String prefix = "P" + juce::String(p) + ".";
        xmlState.setAttribute(prefix + "progname", programs[p].name);
        for (int i = 0; i < getNumParameters(); i++)
        {
            xmlState.setAttribute(prefix + juce::String(i), programs[p].param[i]);
        }
        xmlState.setAttribute(prefix + "icon", programs[p].icon);
        xmlState.setAttribute(prefix + "device", programs[p].device.name);
    }
    copyXmlToBinary(xmlState, destData);
}

void MidiInFilter::setCurrentProgramStateInformation(const void* data, int sizeInBytes)
{
    // use this helper function to get the XML from this binary blob..
    auto const xmlState = getXmlFromBinary(data, sizeInBytes);

    if (xmlState != nullptr)
    {
        // check that it's the right type of xml..
        if (xmlState->hasTagName("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            changeProgramName(getCurrentProgram(), xmlState->getStringAttribute("progname", "Default"));
            for (int i = 0; i < getNumParameters(); i++)
            {
                param[i] = (float) xmlState->getDoubleAttribute(juce::String(i), param[i]);
            }
            icon = xmlState->getStringAttribute("icon", icon);
            setActiveDevice(xmlState->getStringAttribute("device", activeDevice.name));

            sendChangeMessage();
        }
    }
}

void MidiInFilter::setStateInformation(const void* data, int sizeInBytes)
{
    auto const xmlState = getXmlFromBinary(data, sizeInBytes);

    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName("MYPLUGINSETTINGS"))
        {
            for (int p = 0; p < getNumPrograms(); p++)
            {
                juce::String prefix = "P" + juce::String(p) + ".";
                for (int i = 0; i < getNumParameters(); i++)
                {
                    programs[p].param[i] = (float) xmlState->getDoubleAttribute(prefix + juce::String(i), programs[p].param[i]);
                }
                programs[p].icon   = xmlState->getStringAttribute(prefix + "icon", programs[p].icon);
                programs[p].device = getDeviceByName(xmlState->getStringAttribute(prefix + "device", programs[p].device.name));
                programs[p].name   = xmlState->getStringAttribute(prefix + "progname", programs[p].name);
            }
            init = true;
            setCurrentProgram(xmlState->getIntAttribute("program", 0));
        }
    }
}
