
#include "MidiMorphPlugInInterface.h"
#include "MidiMorphGUI.h"

using juce::jmax;
using juce::roundToInt;

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiMorphPlugInInterface();
}

MidiMorphPlugInInterface::MidiMorphPlugInInterface()
{
    this->lastGUIw  = 300;
    this->lastGUIh  = 400;
    this->samplePos = 0;
    this->sendPos   = 0;
    loadDefaultFxb();
}

MidiMorphPlugInInterface::~MidiMorphPlugInInterface()
{
}

void MidiMorphPlugInInterface::prepareToPlay (double sampleRate, int estimatedSamplesPerBlock)
{
}

const juce::String MidiMorphPlugInInterface::getInputChannelName (const int channelIndex) const
{
    return "getInputChannelName(" + juce::String (channelIndex) + ")";
}

const juce::String MidiMorphPlugInInterface::getOutputChannelName (const int channelIndex) const
{
    return "getOutputChannelName(" + juce::String (channelIndex) + ")";
}

bool MidiMorphPlugInInterface::isInputChannelStereoPair (int index) const
{
    return true;
}

bool MidiMorphPlugInInterface::isOutputChannelStereoPair (int index) const
{
    return true;
}

juce::AudioProcessorEditor* MidiMorphPlugInInterface::createEditor()
{
    return new MidiMorphGUI (this);
}

int MidiMorphPlugInInterface::getNumParameters()
{
    return 2;
}

const juce::String MidiMorphPlugInInterface::getParameterName (int parameterIndex)
{
    if (parameterIndex == 0)
    {
        return "cursor X";
    }
    else if (parameterIndex == 1)
    {
        return "cursor Y";
    }
    return juce::String();
}

float MidiMorphPlugInInterface::getParameter (int parameterIndex)
{
    if (parameterIndex == 0)
    {
        return core.getCursorXRatio();
    }
    else if (parameterIndex == 1)
    {
        return core.getCursorYRatio();
    }
    return 0.f;
}

const juce::String MidiMorphPlugInInterface::getParameterText (int parameterIndex)
{
    return juce::String (getParameter (parameterIndex));
}

void MidiMorphPlugInInterface::setParameter (int parameterIndex, float newValue)
{
    if (newValue != getParameter (parameterIndex))
    {
        if (parameterIndex == 0)
        {
            core.setCursorXRatio (newValue);
        }
        else if (parameterIndex == 1)
        {
            core.setCursorYRatio (newValue);
        }
    }
}

bool MidiMorphPlugInInterface::isParameterAutomatable (int parameterIndex)
{
    return true;
}

int MidiMorphPlugInInterface::getNumPrograms()
{
    return 0;
}

int MidiMorphPlugInInterface::getCurrentProgram()
{
    return 0;
}

void MidiMorphPlugInInterface::setCurrentProgram (int index)
{
}

const juce::String MidiMorphPlugInInterface::getProgramName (int index)
{
    return "getProgramName(" + juce::String (index) + ")";
}

void MidiMorphPlugInInterface::changeProgramName (int index, const juce::String& newName)
{
}

void MidiMorphPlugInInterface::setStateInformation (const void* data, int sizeInBytes)
{
    auto state = AudioProcessor::getXmlFromBinary (data, sizeInBytes);
    if (state != nullptr)
    {
        core.setFromXml (state.get());
    }
}

void MidiMorphPlugInInterface::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages)
{
    for (auto&& m : midiMessages)
    {
        auto msg = m.getMessage();
        core.onMidiEvent (msg);
    }

    int delta;
    int stepSize = core.getUpdateRateSmpls (roundToInt (getSampleRate()));

    double newSamplepos = samplePos + buffer.getNumSamples();

    midiMessages.clear();

    while (sendPos <= newSamplepos && core.hasNewMidi())
    {
        delta = roundToInt (jmax ((double) 0, sendPos - samplePos));
        core.getMidiMessages (delta, midiMessages);

        sendPos += stepSize;
    }
    samplePos += buffer.getNumSamples();
}

void MidiMorphPlugInInterface::getStateInformation (juce::MemoryBlock& destData)
{
    juce::AudioProcessor::copyXmlToBinary (*core.getXml ("midimorph"), destData);
}

void MidiMorphPlugInInterface::releaseResources()
{
}

bool MidiMorphPlugInInterface::acceptsMidi() const
{
    return true;
}

bool MidiMorphPlugInInterface::producesMidi() const
{
    return true;
}

const juce::String MidiMorphPlugInInterface::getName() const
{
    return ("MidiMorph");
}
