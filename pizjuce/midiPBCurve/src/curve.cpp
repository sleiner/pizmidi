#include "curve.h"
#include "curvegui.h"

//==============================================================================
/*
    This function must be implemented to create the actual plugin object that
    you want to use.
*/
PizAudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiCurve();
}

//==============================================================================
JuceProgram::JuceProgram ()
{
    //default program name
	name = "midiPBCurve";

    //default values
	for (int i=0;i<MAX_ENVELOPE_POINTS;i++) {
		param[i*2] = (float)roundFloatToInt(127.f*(float)i/(MAX_ENVELOPE_POINTS-1))*(float)midiScaler;
		param[i*2+1] = param[i*2];
		param[kActive + i] = 0.f;
	}
	param[kActive]=1.f;
	param[kActive+1]=1.f;
	param[2] = param[3] = 0.5f;
	param[kActive+MAX_ENVELOPE_POINTS-1]=1.f;

    param[kChannel] = 0.0f;
	param[kPitchBend] = 1.f;
	param[kPBRange] = 0.5f;
	param[kPBRange2] = 0.f;

    //default GUI size
    lastUIWidth = 600;
    lastUIHeight = 400;
}

//==============================================================================
MidiCurve::MidiCurve()
{
    // create built-in programs
    programs = new JuceProgram[getNumPrograms()];
    if (!loadDefaultFxb())
    {
	    for(int i=0;i<getNumPrograms();i++){
            programs[i].name = String("Program ") + String(i+1);
        }
    }
    //start up with the first program
    init = true;
	lastCCOut=lastCCIn=-1;
    setCurrentProgram (0);
}

MidiCurve::~MidiCurve()
{
    if (programs) delete [] programs;
}

//==============================================================================
float MidiCurve::getParameter (int index)
{
	if (index<getNumParameters())
		return param[index];
	return 0.f;
}

void MidiCurve::setParameter (int index, float newValue)
{
	if (index<getNumParameters()) {
		JuceProgram* ap = &programs[curProgram];

		if (param[index] != newValue) {
			if (index==0) param[index]=0.f;
			else if (index==(MAX_ENVELOPE_POINTS*2-2)) param[index]=1.f;
			else param[index] = ap->param[index] = newValue;
			updatePath();
			sendChangeMessage ();
		}
	}
}

const String MidiCurve::getParameterName (int index)
{
    if (index == kChannel) return "Channel";
    return "param"+String(index);
}

const String MidiCurve::getParameterText (int index)
{
    if (index==kChannel) {
        if (roundFloatToInt(param[kChannel]*16.0f)==0) return String("Any");
        else return String(roundFloatToInt(param[kChannel]*16.0f));
    }
	else if (index<getNumParameters())
		return String(roundFloatToInt(127.f*param[index]));
	else return String();
}

const String MidiCurve::getInputChannelName (const int channelIndex) const
{
	if (channelIndex<getNumInputChannels())
		return String(JucePlugin_Name) + String(" ") + String (channelIndex + 1);
	return String();
}

const String MidiCurve::getOutputChannelName (const int channelIndex) const
{
	if (channelIndex<getNumOutputChannels())
	    return String(JucePlugin_Name) + String(" ") + String (channelIndex + 1);
	return String();
}

bool MidiCurve::isInputChannelStereoPair (int index) const
{
    return true;
}

bool MidiCurve::isOutputChannelStereoPair (int index) const
{
    return true;
}


//======================Programs================================================
void MidiCurve::setCurrentProgram (int index)
{
    //save non-parameter info to the old program, except the first time
    if (!init) {
        programs[curProgram].lastUIHeight = lastUIHeight;
        programs[curProgram].lastUIWidth = lastUIWidth;
    }
    init = false;

    //then set the new program
	JuceProgram* ap = &programs[index];
    curProgram = index;
    for (int i=0;i<getNumParameters();i++) {
        param[i] = ap->param[i];
    }
    lastUIWidth = ap->lastUIWidth;
    lastUIHeight = ap->lastUIHeight;

	updatePath();
    sendChangeMessage();
}

void MidiCurve::changeProgramName(int index, const String &newName) {
	if (index<getNumPrograms())
		programs[index].name = newName;
}

const String MidiCurve::getProgramName(int index) {
	if (index<getNumPrograms())
	    return programs[index].name;
	return String();
}

int MidiCurve::getCurrentProgram() {
    return curProgram;
}

//==============================================================================
void MidiCurve::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // do your pre-playback setup stuff here..
}

void MidiCurve::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

float MidiCurve::getPointValue(int n, int y)
{
	if (n<MAX_ENVELOPE_POINTS)
	{
		if (!y)
			return points[n].p.getX();
		else
			return 1.f-points[n].p.getY();
	}
	return -1.f;
}

float MidiCurve::findValue(float input)
{
	PathFlatteningIterator it(path,AffineTransform::identity,(float)midiScaler);
	while (it.next()) {
		if (it.x1==input) return 1.f-it.y1;
		if (it.x2>=input) {
			return 1.f-(float)linearInterpolate(input,it.y1,it.y2,it.x1,it.x2);
		}
	}

	return -1.f;
}

double MidiCurve::linearInterpolate(double x,double y1,double y2,double x1,double x2) {
      double slope = (y2-y1)/(x2-x1);
      double y0 = y1 - slope * x1;
      return slope * x + y0;
}

void MidiCurve::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
    const int channel = roundFloatToInt(param[kChannel]*16.0f);
	MidiBuffer output;
    MidiBuffer::Iterator mid_buffer_iter(midiMessages);
    MidiMessage midi_message(0xFE);
    int sample_number;
    while(mid_buffer_iter.getNextEvent(midi_message,sample_number)) {
		if (midi_message.isForChannel(channel) || channel==0)
		{
			if (midi_message.isPitchWheel()) {
				if (param[kPitchBend]>=0.5f)
				{
					int v = midi_message.getPitchWheelValue();
					const uint8* data = midi_message.getRawData();
					lastCCIn = v;
					v=roundFloatToInt(16383.f * findValue(v*(float)0.00006103888));
					lastCCOut = v;
					MidiMessage out = MidiMessage(data[0], v & 0x007f, (v & 0x3f80)>>7);
					output.addEvent(out,sample_number);
					sendChangeMessage();
				}
				else output.addEvent(midi_message,sample_number);
			}
			//else if (midi_message.isProgramChange()) {
				//setCurrentProgram(midi_message.getProgramChangeNumber());
			//}
			else output.addEvent(midi_message,sample_number);
		}
		else output.addEvent(midi_message,sample_number);
    }
	midiMessages=output;
}


void MidiCurve::updatePath() {
	points.clear();
	for (int i=0;i<MAX_ENVELOPE_POINTS;i++) {
		midiPoint p = midiPoint(param[i*2],param[i*2+1],isPointActive(i),isPointControl(i));
		points.addSorted(pointComparator,p);
	}
    Path myPath;
	if (points[0].isActive) {
	    myPath.startNewSubPath (0.f, getPointValue(0,1));
		myPath.lineTo (0.f,getPointValue(0,1));
	}
	else {
	    myPath.startNewSubPath (0.f, 1.f);
		myPath.lineTo (0.f, 1.f);
	}
	for (int i = 1; i < MAX_ENVELOPE_POINTS; i++) {
		if (points[i].isActive && !points[i].isControl)
		{
			int prev = getPrevActivePoint(i);
			if (points[prev].isControl) {
				if (points[getPrevActivePoint(prev)].isControl) {
					myPath.cubicTo(getPointValue(getPrevActivePoint(prev),0), getPointValue(getPrevActivePoint(prev),1),
								   getPointValue(prev,0), getPointValue(prev,1),
				      			   getPointValue(i,0),   getPointValue(i,1));
				}
				else {
					myPath.quadraticTo(getPointValue(prev,0), getPointValue(prev,1),
				      				   getPointValue(i,0),   getPointValue(i,1));
				}
			}
			else {
				myPath.lineTo (getPointValue(i,0), getPointValue(i,1));
			}
		}
	}
	if (!points[MAX_ENVELOPE_POINTS-1].isActive) {
		myPath.lineTo (1.f, 0.f);
	}
	path=myPath;
}

int MidiCurve::getPrevActivePoint(int currentPoint) {
	for (int i=currentPoint-1;i>0;i--) {
		if (points[i].isActive) return i;
	}
	return 0;
}

int MidiCurve::getNextActivePoint(int currentPoint) {
	for (int i=currentPoint+1;i<MAX_ENVELOPE_POINTS;i++) {
		if (points[i].isActive) return i;
	}
	return MAX_ENVELOPE_POINTS-1;
}

void MidiCurve::resetPoints() {
	for (int i=0;i<MAX_ENVELOPE_POINTS;i++) {
		param[i*2] = (float)roundFloatToInt(127.f*(float)i/(MAX_ENVELOPE_POINTS-1))*(float)midiScaler;
		param[i*2+1] = param[i*2];
		param[kActive + i] = 0.f;
	}
	param[kActive]=1.f;
	param[kActive+1]=1.f;
	param[2] = param[3] = 8192.f*(float)0.00006103888;
	param[kActive+MAX_ENVELOPE_POINTS-1]=1.f;
	updatePath();
	sendChangeMessage();
}

bool MidiCurve::isPointActive(int point)
{
	if(point<0) return false;
	return getParameter(point + kActive) > 0.5f;
}

bool MidiCurve::isPointControl(int point)
{
	if(point<0) return false;
	return getParameter(point + kActive) > 0.1f && getParameter(point + kActive) < 0.9f;
}

//==============================================================================
AudioProcessorEditor* MidiCurve::createEditor()
{
    return new CurveEditor (this);
}

//==============================================================================
void MidiCurve::getCurrentProgramStateInformation (MemoryBlock& destData)
{
    // make sure the non-parameter settings are copied to the current program
    programs[curProgram].lastUIHeight = lastUIHeight;
    programs[curProgram].lastUIWidth = lastUIWidth;
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

    for (int i=0;i<kNumParams;i++) {
        xmlState.setAttribute (String(i), param[i]);
    }

    xmlState.setAttribute ("uiWidth", lastUIWidth);
    xmlState.setAttribute ("uiHeight", lastUIHeight);

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xmlState, destData);
}
void MidiCurve::getStateInformation(MemoryBlock &destData) {
    // make sure the non-parameter settings are copied to the current program
    programs[curProgram].lastUIHeight = lastUIHeight;
    programs[curProgram].lastUIWidth = lastUIWidth;

    XmlElement xmlState ("MYPLUGINSETTINGS");
    xmlState.setAttribute ("pluginVersion", 1);
    xmlState.setAttribute ("program", getCurrentProgram());
    for (int p=0;p<getNumPrograms();p++) {
        String prefix = "P" + String(p) + ".";
        xmlState.setAttribute (prefix+"progname", programs[p].name);
        for (int i=0;i<kNumParams;i++) {
            xmlState.setAttribute (prefix+String(i), programs[p].param[i]);
        }
        xmlState.setAttribute(prefix+"uiWidth", programs[p].lastUIWidth);
        xmlState.setAttribute(prefix+"uiHeight", programs[p].lastUIHeight);
    }
    copyXmlToBinary (xmlState, destData);
}

void MidiCurve::setCurrentProgramStateInformation (const void* data, int sizeInBytes)
{
    // use this helper function to get the XML from this binary blob..
    auto const xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != nullptr)
    {
        // check that it's the right type of xml..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            changeProgramName(getCurrentProgram(),xmlState->getStringAttribute ("progname", "Default"));
            for (int i=0;i<kNumParams;i++) {
                param[i] = (float) xmlState->getDoubleAttribute (String(i), param[i]);
            }
            lastUIWidth = xmlState->getIntAttribute ("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);

            sendChangeMessage ();
        }
    }
}

void MidiCurve::setStateInformation (const void* data, int sizeInBytes) {
    auto const xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            for (int p=0;p<getNumPrograms();p++) {
                String prefix = "P" + String(p) + ".";
                for (int i=0;i<kNumParams;i++) {
                    programs[p].param[i] = (float) xmlState->getDoubleAttribute (prefix+String(i), programs[p].param[i]);
                }
                programs[p].lastUIWidth = xmlState->getIntAttribute (prefix+"uiWidth", programs[p].lastUIWidth);
                programs[p].lastUIHeight = xmlState->getIntAttribute (prefix+"uiHeight", programs[p].lastUIHeight);
                programs[p].name = xmlState->getStringAttribute (prefix+"progname", programs[p].name);
            }
            init=true;
            setCurrentProgram(xmlState->getIntAttribute("program", 0));
        }
    }
}
