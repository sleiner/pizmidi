#include "midiIn.h"
#include "midiInEditor.h"



//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiInFilter();
}

JuceProgram::JuceProgram ()
{
    //default values
    param[kPower]  = 0.0f;
    param[kHostIn] = 1.0f;
	param[kChannel] = 0.f;

    icon = String("");   // icon filename

    //program name
	name = "Default";
}

//==============================================================================
MidiInFilter::MidiInFilter()
{
    programs = new JuceProgram[getNumPrograms()];

    devices = MidiInput::getDevices();
    midiInput = 0;
	loadDefaultFxb();
    curProgram=0;
    init=true;
    setCurrentProgram (0);

    wasPlaying=false;

    zeromem (&lastPosInfo, sizeof (lastPosInfo));
}

//==============================================================================
int MidiInFilter::getNumParameters()
{
    return numParams;
}

float MidiInFilter::getParameter (int index)
{
    return param[index];
}

void MidiInFilter::setParameter (int index, float newValue)
{
    JuceProgram* ap = &programs[curProgram];

    if (param[index] != newValue) {
        param[index] = ap->param[index] = newValue;
        if (index==kPower) {
            if (param[kPower]>0.0) {
                setActiveDevice(activeDevice);
            }
            else {
				if (midiInput) midiInput->stop();
                midiInput=NULL;
            }
        }
        sendChangeMessage();
    }
}

void MidiInFilter::setActiveDevice(String name)
{
	activeDevice = programs[curProgram].device = name;
	int index = devices.indexOf(name);
	if (index==-1) {
		if (midiInput) midiInput->stop();
		midiInput = NULL;
	}
	else {
		if (midiInput) midiInput->stop();
		midiInput = MidiInput::openDevice(index,&collector);
		midiInput->start();
	}
}

const String MidiInFilter::getParameterName (int index)
{
    if (index == kPower) return "Power";
    if (index == kHostIn) return "HostIn";
    return String();
}

const String MidiInFilter::getParameterText (int index)
{
    if (index == kPower) {
         if (param[kPower]>0.f) return String("On");
        else return String("Off");
    }
    if (index == kHostIn) {
        if (param[kHostIn]>=0.5) return String("On");
        else return String("Off");
    }
    return String();
}

const String MidiInFilter::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String MidiInFilter::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool MidiInFilter::isInputChannelStereoPair (int index) const
{
    if (getNumInputChannels()==2) return true;
    else return false;
}

bool MidiInFilter::isOutputChannelStereoPair (int index) const
{
    if (getNumOutputChannels()==2) return true;
    else return false;
}
void MidiInFilter::setCurrentProgram (int index)
{
    if (!init) {
        programs[curProgram].icon = icon;
    }
    init=false;

	JuceProgram* ap = &programs[index];
    curProgram = index;
	setActiveDevice(ap->device);
    for (int i=0;i<getNumParameters();i++) {
        setParameter(i, ap->param[i]);
    }
    icon = ap->icon;
}

void MidiInFilter::changeProgramName(int index, const String &newName) {
    programs[curProgram].name = newName;
}

const String MidiInFilter::getProgramName(int index) {
    return programs[index].name;
}

int MidiInFilter::getCurrentProgram() {
    return curProgram;
}
//==============================================================================
AudioProcessorEditor* MidiInFilter::createEditor()
{
    return new MidiInEditor (this);
}
//==============================================================================
void MidiInFilter::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	this->collector.reset(sampleRate);
	if (midiInput) midiInput->start();
}

void MidiInFilter::releaseResources()
{
	if (midiInput) midiInput->stop();
	this->collector.reset(44100);
}

void MidiInFilter::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
    for (int i = 0; i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

	const int channel = roundToInt(param[kChannel]*16.f);
	if (param[kHostIn]<0.5f) midiMessages.clear();
	collector.removeNextBlockOfMessages(midiMessages,buffer.getNumSamples());
	if (channel>0)
	{
		MidiBuffer output;
		MidiBuffer::Iterator mid_buffer_iter(midiMessages);
		MidiMessage midi_message(0xf8e);
		int sample_number;
		while(mid_buffer_iter.getNextEvent(midi_message,sample_number)) {
			if (midi_message.getChannel()==0 || midi_message.getChannel()==channel)
			{
				output.addEvent(midi_message,sample_number);
			}
		}
		midiMessages.clear();
		midiMessages = output;
	}
}

//==============================================================================
void MidiInFilter::getCurrentProgramStateInformation (MemoryBlock& destData)
{
    // make sure the non-parameter settings are copied to the current program
    programs[curProgram].icon = icon;
    // you can store your parameters as binary data if you want to or if you've got
    // a load of binary to put in there, but if you're not doing anything too heavy,
    // XML is a much cleaner way of doing it - here's an example of how to store your
    // params as XML..

    // create an outer XML element..
    XmlElement xmlState ("MYPLUGINSETTINGS");

    // add some attributes to it..
    xmlState.setAttribute ("pluginVersion", 1);

    xmlState.setAttribute ("program", getCurrentProgram());
    xmlState.setAttribute ("progname", getProgramName(getCurrentProgram()));

    for (int i=0;i<getNumParameters();i++) {
        xmlState.setAttribute (String(i), param[i]);
    }

    xmlState.setAttribute ("icon", icon);
	xmlState.setAttribute ("device", activeDevice);

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xmlState, destData);
}
void MidiInFilter::getStateInformation(MemoryBlock &destData) {
    // make sure the non-parameter settings are copied to the current program
    programs[curProgram].icon = icon;

    XmlElement xmlState ("MYPLUGINSETTINGS");
    xmlState.setAttribute ("pluginVersion", 1);
    xmlState.setAttribute ("program", getCurrentProgram());
    for (int p=0;p<getNumPrograms();p++) {
        String prefix = "P" + String(p) + ".";
        xmlState.setAttribute (prefix+"progname", programs[p].name);
        for (int i=0;i<getNumParameters();i++) {
            xmlState.setAttribute (prefix+String(i), programs[p].param[i]);
        }
        xmlState.setAttribute (prefix+"icon", programs[p].icon);
		xmlState.setAttribute (prefix+"device", programs[p].device);
    }
    copyXmlToBinary (xmlState, destData);
}

void MidiInFilter::setCurrentProgramStateInformation (const void* data, int sizeInBytes)
{
    // use this helper function to get the XML from this binary blob..
    auto const xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != 0)
    {
        // check that it's the right type of xml..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            changeProgramName(getCurrentProgram(),xmlState->getStringAttribute ("progname", "Default"));
            for (int i=0;i<getNumParameters();i++) {
                param[i] = (float) xmlState->getDoubleAttribute (String(i), param[i]);
            }
            icon = xmlState->getStringAttribute ("icon", icon);
			setActiveDevice(xmlState->getStringAttribute ("device", activeDevice));

            sendChangeMessage ();
        }
    }
}

void MidiInFilter::setStateInformation (const void* data, int sizeInBytes) {
    auto const xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != 0)
    {
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            for (int p=0;p<getNumPrograms();p++) {
                String prefix = "P" + String(p) + ".";
                for (int i=0;i<getNumParameters();i++) {
                    programs[p].param[i] = (float) xmlState->getDoubleAttribute (prefix+String(i), programs[p].param[i]);
                }
                programs[p].icon = xmlState->getStringAttribute (prefix+"icon", programs[p].icon);
				programs[p].device = xmlState->getStringAttribute (prefix+"device", programs[p].device);
                programs[p].name = xmlState->getStringAttribute (prefix+"progname", programs[p].name);
            }
            init=true;
            setCurrentProgram(xmlState->getIntAttribute("program", 0));
        }
    }
}
