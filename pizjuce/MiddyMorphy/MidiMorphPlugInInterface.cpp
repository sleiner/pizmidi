
#include "MidiMorphPlugInInterface.h"
#include "MidiMorphGUI.h"

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiMorphPlugInInterface();
}

MidiMorphPlugInInterface::MidiMorphPlugInInterface()
{
    // Bouml preserved body begin 0003038D
    this->lastGUIw = 300;
    this->lastGUIh = 400;
    this->samplePos = 0;
    this->sendPos = 0;
    loadDefaultFxb();
    // Bouml preserved body end 0003038D
}

MidiMorphPlugInInterface::~MidiMorphPlugInInterface()
{
    // Bouml preserved body begin 0003040D
    // Bouml preserved body end 0003040D
}

void MidiMorphPlugInInterface::prepareToPlay (double sampleRate, int estimatedSamplesPerBlock)
{
    // Bouml preserved body begin 0002D90D
    // Bouml preserved body end 0002D90D
}

//
const String MidiMorphPlugInInterface::getInputChannelName (const int channelIndex) const
{
    // Bouml preserved body begin 0002DA0D
    return "getInputChannelName(" + String (channelIndex) + ")";
    // Bouml preserved body end 0002DA0D
}

//virtual  AudioProcessor::getOutputChannelName  (       )  const
const String MidiMorphPlugInInterface::getOutputChannelName (const int channelIndex) const
{
    // Bouml preserved body begin 0002DA8D
    return "getOutputChannelName(" + String (channelIndex) + ")";
    // Bouml preserved body end 0002DA8D
}

//virtual bool AudioProcessor::isInputChannelStereoPair  (  int  index   )  const
bool MidiMorphPlugInInterface::isInputChannelStereoPair (int index) const
{
    // Bouml preserved body begin 0002DB0D
    return true;
    // Bouml preserved body end 0002DB0D
}

//virtual bool AudioProcessor::isInputChannelStereoPair  (  int  index   )  const
bool MidiMorphPlugInInterface::isOutputChannelStereoPair (int index) const
{
    // Bouml preserved body begin 0002DB8D
    return true;
    // Bouml preserved body end 0002DB8D
}

//virtual AudioProcessorEditor* AudioProcessor::createEditor  (    )

AudioProcessorEditor* MidiMorphPlugInInterface::createEditor()
{
    // Bouml preserved body begin 0002DC0D
    return new MidiMorphGUI (this);
    // Bouml preserved body end 0002DC0D
}

int MidiMorphPlugInInterface::getNumParameters()
{
    // Bouml preserved body begin 0002DC8D
    return 2;
    // Bouml preserved body end 0002DC8D
}

const String MidiMorphPlugInInterface::getParameterName (int parameterIndex)
{
    // Bouml preserved body begin 0002DD0D
    if (parameterIndex == 0)
    {
        return "cursor X";
    }
    else if (parameterIndex == 1)
    {
        return "cursor Y";
    }
    return String();
    // Bouml preserved body end 0002DD0D
}

float MidiMorphPlugInInterface::getParameter (int parameterIndex)
{
    // Bouml preserved body begin 0002DD8D
    if (parameterIndex == 0)
    {
        return core.getCursorXRatio();
    }
    else if (parameterIndex == 1)
    {
        return core.getCursorYRatio();
    }
    return 0.f;
    // Bouml preserved body end 0002DD8D
}

const String MidiMorphPlugInInterface::getParameterText (int parameterIndex)
{
    // Bouml preserved body begin 0002DE0D
    return String (getParameter (parameterIndex));
    // Bouml preserved body end 0002DE0D
}

void MidiMorphPlugInInterface::setParameter (int parameterIndex, float newValue)
{
    // Bouml preserved body begin 0002DE8D
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
    // Bouml preserved body end 0002DE8D
}

//bool AudioProcessor::isParameterAutomatable  (  int  parameterIndex   )
bool MidiMorphPlugInInterface::isParameterAutomatable (int parameterIndex)
{
    // Bouml preserved body begin 0002DF0D
    return true;
    // Bouml preserved body end 0002DF0D
}

int MidiMorphPlugInInterface::getNumPrograms()
{
    // Bouml preserved body begin 0002DF8D
    return 0;
    // Bouml preserved body end 0002DF8D
}

int MidiMorphPlugInInterface::getCurrentProgram()
{
    // Bouml preserved body begin 0002E00D
    return 0;
    // Bouml preserved body end 0002E00D
}

//  (  int     )
void MidiMorphPlugInInterface::setCurrentProgram (int index)
{
    // Bouml preserved body begin 0002E08D
    // Bouml preserved body end 0002E08D
}

// AudioProcessor::getProgramName  (  int  index   )
const String MidiMorphPlugInInterface::getProgramName (int index)
{
    // Bouml preserved body begin 0002E10D
    return "getProgramName(" + String (index) + ")";
    // Bouml preserved body end 0002E10D
}

//
void MidiMorphPlugInInterface::changeProgramName (int index, const String& newName)
{
    // Bouml preserved body begin 0002E18D
    // Bouml preserved body end 0002E18D
}

void MidiMorphPlugInInterface::setStateInformation (const void* data, int sizeInBytes)
{
    // Bouml preserved body begin 0002E30D
    auto state = AudioProcessor::getXmlFromBinary (data, sizeInBytes);
    if (state != 0)
    {
        core.setFromXml (state.get());
    }
    // Bouml preserved body end 0002E30D
}

void MidiMorphPlugInInterface::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // Bouml preserved body begin 0002EB0D
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
        //midiMessages.addEvents(core.getMidiMessages(delta),0,99999,0);
        sendPos += stepSize;
    }
    samplePos += buffer.getNumSamples();
    // Bouml preserved body end 0002EB0D
}

//  (       )
void MidiMorphPlugInInterface::getStateInformation (JUCE_NAMESPACE::MemoryBlock& destData)
{
    // Bouml preserved body begin 0002EB8D
    AudioProcessor::copyXmlToBinary (*core.getXml ("midimorph"), destData);
    // Bouml preserved body end 0002EB8D
}

void MidiMorphPlugInInterface::releaseResources()
{
    // Bouml preserved body begin 0002EC8D
    // Bouml preserved body end 0002EC8D
}

bool MidiMorphPlugInInterface::acceptsMidi() const
{
    // Bouml preserved body begin 0002ED0D
    return true;
    // Bouml preserved body end 0002ED0D
}

bool MidiMorphPlugInInterface::producesMidi() const
{
    // Bouml preserved body begin 0002ED8D
    return true;
    // Bouml preserved body end 0002ED8D
}

const String MidiMorphPlugInInterface::getName() const
{
    // Bouml preserved body begin 0002EE0D
    return ("MidiMorph");
    // Bouml preserved body end 0002EE0D
}
