#include "MidiChordAnalyzer.h"
#include "MidiChordAnalyzerEditor.h"

//==============================================================================
/**
    This function must be implemented to create a new instance of your
    plugin object.
*/
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiChordAnalyzer();
}

//==============================================================================
MidiChordAnalyzerPrograms::MidiChordAnalyzerPrograms ()
: BankStorage("MidiChordAnalyzerValues", 1, numProgs)
{
	for (int p=0;p<getNumPrograms();p++)
	{
		ValueTree progv("ProgValues");
	}
}
void MidiChordAnalyzerPrograms::loadDefaultValues()
{
	setGlobal("lastProgram", 0);
	setGlobal("Version", 4);

	for (int b = 0; b < getNumBanks(); b++)
	{
		for (int p = 0; p < getNumPrograms(); p++)
		{
			set(b, p, "progIndex", p);

			set(b, p, "Channel", 0);
			set(b, p, "Flats", false);

			set(b, p, "Name", "Program "+ String(p+1));
			set(b, p, "lastUIWidth", 600);
			set(b, p, "lastUIHeight", 400);
		}
	}
}

//==============================================================================
MidiChordAnalyzer::MidiChordAnalyzer() : programs(0), curProgram(0)
{
	DBG("MidiChordAnalyzer()");

	fillChordDatabase();
	programs = new MidiChordAnalyzerPrograms();
    if (!loadDefaultFxb())
	{
		channel = 0;
		flats = false;
		lastUIWidth = 480;
		lastUIHeight = 200;
	}

	init = true;
	setCurrentProgram(0);
}

MidiChordAnalyzer::~MidiChordAnalyzer()
{
	DBG("~MidiChordAnalyzer()");
	if (programs) deleteAndZero(programs);
}

//==============================================================================
const String MidiChordAnalyzer::getName() const
{
    return JucePlugin_Name;
}

int MidiChordAnalyzer::getNumParameters()
{
    return numParams;
}

void MidiChordAnalyzer::setCurrentProgram (int index)
{
    //save non-parameter info to the old program, except the first time
    if (!init)
		copySettingsToProgram(curProgram);
    init = false;

    //then set the new program
    curProgram = index;
	programs->setGlobal("lastProgram", index);
	channel = programs->get(0, index, "Channel");
	flats = programs->get(0, index, "Flats");

    lastUIWidth = programs->get(0, index, "lastUIWidth");
    lastUIHeight = programs->get(0, index, "lastUIHeight");
}

float MidiChordAnalyzer::getParameter (int index)
{
    switch (index)
    {
    case kChannel: return ((float)channel)/16.f;
	case kFlats: return flats ? 1.f : 0.f;

	default: return 0.0f;
    }
}

void MidiChordAnalyzer::setParameter (int index, float newValue)
{
    if (index==kChannel)
    {
        channel = roundToInt(newValue*16.f);
        sendChangeMessage();
    }
    else if (index==kFlats)
    {
        flats = newValue>0.f;
        sendChangeMessage();
    }
}

const String MidiChordAnalyzer::getParameterName (int index)
{
    if (index == kChannel)        return "Channel";
	if (index == kFlats)		  return "Flats";
	return String();
}

const String MidiChordAnalyzer::getParameterText (int index)
{
    if (index == kChannel)
		return channel==0 ? "Any" : String(channel);
    if (index == kFlats)
        return flats ? "Yes" : "No";
	return String();
}

const String MidiChordAnalyzer::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String MidiChordAnalyzer::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool MidiChordAnalyzer::isInputChannelStereoPair (int index) const
{
    return true;
}

bool MidiChordAnalyzer::isOutputChannelStereoPair (int index) const
{
    return true;
}

//==============================================================================
void MidiChordAnalyzer::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void MidiChordAnalyzer::releaseResources()
{
}

void MidiChordAnalyzer::processBlock (AudioSampleBuffer& buffer,
                                MidiBuffer& midiMessages)
{

	chordKbState.processNextMidiBuffer(midiMessages,0,buffer.getNumSamples(),true);

	for(auto&& msgMetadata : midiMessages)
	{
		auto m = msgMetadata.getMessage();

		if (channel==0 || m.isForChannel(channel))
		{
			if (m.isNoteOn()) {
				sendChangeMessage();
			}
			else if (m.isNoteOff()) {
				sendChangeMessage();
			}
		}
	}


    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
AudioProcessorEditor* MidiChordAnalyzer::createEditor()
{
    return new MidiChordAnalyzerEditor (this);
}

//==============================================================================
void MidiChordAnalyzer::getStateInformation (MemoryBlock& destData)
{
	copySettingsToProgram(curProgram);
	programs->dumpTo(destData);
}

void MidiChordAnalyzer::getCurrentProgramStateInformation (MemoryBlock& destData)
{
	copySettingsToProgram(curProgram);
	programs->dumpProgramTo(0, curProgram, destData);
}

void MidiChordAnalyzer::setStateInformation (const void* data, int sizeInBytes)
{
	MemoryInputStream m(data,sizeInBytes,false);
	ValueTree vt = ValueTree::readFromStream(m);

	programs->loadFrom(vt);
	init=true;
	setCurrentProgram(vt.getProperty("lastProgram",0));
}

void MidiChordAnalyzer::setCurrentProgramStateInformation (const void* data, int sizeInBytes)
{
	MemoryInputStream m(data,sizeInBytes,false);
	ValueTree vt = ValueTree::readFromStream(m);

	programs->loadNoteMatrixFrom(vt, curProgram);
	init=true;
	setCurrentProgram(curProgram);
}

void MidiChordAnalyzer::copySettingsToProgram(int index)
{
	programs->set(0,index,"Channel",channel);
    programs->set(0,index,"Name",getProgramName(index));
	programs->set(0,index,"lastUIHeight",lastUIHeight);
    programs->set(0,index,"lastUIWidth",lastUIWidth);
	programs->set(0,index,"Flats",flats);
}
