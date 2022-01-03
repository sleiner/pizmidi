#include "midiOut.h"
#include "midiOutEditor.h"



//==============================================================================
PizAudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiOutFilter();
}

JuceProgram::JuceProgram ()
{
    //default values
	zeromem(param,sizeof(param));
    param[kPower]   = 0.0f;
    param[kClock]   = 1.0f;
    param[kHostOut] = 1.0f;
    param[kStamped] = 1.0f;
	param[kChannel] = 0.0f;

    icon = String("");   // icon filename
	device = String();

    //program name
	name = L"Default";
}

//==============================================================================
MidiOutFilter::MidiOutFilter()
{
    programs = new JuceProgram[getNumPrograms()];

    devices = MidiOutput::getDevices();
    midiOutput = 0;
	loadDefaultFxb();
    curProgram=0;
    init=true;
    setCurrentProgram (0);

    samplesToNextClock=0;
    samplesToNextMTC=0;
    wasPlaying=false;
    startAt=-999.0;
    startMTCAt=-999.0;
    sendclock=false;
    sendmtc=false;
    mtcNumber=0;

    zeromem (&lastPosInfo, sizeof (lastPosInfo));
}

MidiOutFilter::~MidiOutFilter()
{
    if (midiOutput) {
		midiOutput->stopBackgroundThread();
	}
}

//==============================================================================
int MidiOutFilter::getNumParameters()
{
    return numParams;
}

float MidiOutFilter::getParameter (int index)
{
    return param[index];
}

void MidiOutFilter::setParameter (int index, float newValue)
{
    JuceProgram* ap = &programs[curProgram];

    if (param[index] != newValue) {
        param[index] = ap->param[index] = newValue;
        if (index==kPower) {
            if (param[kPower]>0.0) {
                setActiveDevice(activeDevice);
            }
            else {
				if (midiOutput) {
					midiOutput->stopBackgroundThread();
					midiOutput.reset();
				}
            }
        }
        sendChangeMessage();
    }
}

void MidiOutFilter::setActiveDevice(String name)
{
	activeDevice = programs[curProgram].device = name;
	int index = devices.indexOf(name);
	if (index==-1) {
		if (midiOutput)
		{
			midiOutput->stopBackgroundThread();
			midiOutput.reset();
		}
	}
	else {
		if (midiOutput) {
			midiOutput->stopBackgroundThread();
			midiOutput.reset();
		}
		midiOutput = MidiOutput::openDevice(index);
		if (midiOutput)
			midiOutput->startBackgroundThread();
		else
			setParameter(kPower,0);
	}
}

const String MidiOutFilter::getParameterName (int index)
{
    if (index == kPower) return L"Power";
    if (index == kClock) return L"Clock";
    if (index == kMTC) return L"MTC";
    if (index == kHostOut) return L"HostOut";
    if (index == kStamped) return L"Out Mode";
    return String();
}

const String MidiOutFilter::getParameterText (int index)
{
    if (index == kPower) {
         if (param[kPower]>0.f) return String(L"On");
        else return String(L"Off");
    }
    if (index == kClock) {
        if (param[kClock]>=0.5) return String(L"On");
        else return String(L"Off");
    }
    if (index == kMTC) {
        if (param[kMTC]>=0.5) return String(L"On");
        else return String(L"Off");
    }
    if (index == kHostOut) {
        if (param[kHostOut]>=0.5) return String(L"On");
        else return String(L"Off");
    }
    if (index == kStamped) {
        if (param[kStamped]>=0.5) return String(L"Immediate");
        else return String(L"Stamped");
    }
    if (index == kChannel) {
        if (roundToInt(param[kChannel]*16.f)==0) return String(L"All");
        else return String(roundToInt(param[kChannel]*16.f));
    }
    return String();
}

const String MidiOutFilter::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String MidiOutFilter::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool MidiOutFilter::isInputChannelStereoPair (int index) const
{
    if (getNumInputChannels()==2) return true;
    else return false;
}

bool MidiOutFilter::isOutputChannelStereoPair (int index) const
{
    if (getNumOutputChannels()==2) return true;
    else return false;
}
void MidiOutFilter::setCurrentProgram (int index)
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

void MidiOutFilter::changeProgramName(int index, const String &newName) {
    programs[curProgram].name = newName;
}

const String MidiOutFilter::getProgramName(int index) {
    return programs[index].name;
}

int MidiOutFilter::getCurrentProgram() {
    return curProgram;
}
//==============================================================================
AudioProcessorEditor* MidiOutFilter::createEditor()
{
    return new MidiOutEditor (this);
}
//==============================================================================
void MidiOutFilter::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // do your pre-playback setup stuff here..
}

void MidiOutFilter::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void MidiOutFilter::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
    for (int i = 0; i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    const double SR=getSampleRate();
	const double iSR=1.0/SR;
    AudioPlayHead::CurrentPositionInfo pos;
    if (getPlayHead() != 0 && getPlayHead()->getCurrentPosition (pos))
    {
        if (memcmp (&pos, &lastPosInfo, sizeof (pos)) != 0)
        {
            if(param[kMTC]>=0.5f) {
                double frameRate=24.0;
                int mtcFrameRate=0;

                const double samplesPerPpq=60.0*SR/pos.bpm;
                const double samplesPerClock = SR/(4.0*frameRate);
                const long double seconds = (long double)(pos.ppqPosition*60.0f/pos.bpm) /*+ smpteOffset*/;
                const long double absSecs = fabs (seconds);
                const bool neg  = seconds < 0.0;

                int hours, mins, secs, frames;
                if (frameRate==29.97) {
                    int64 frameNumber = int64(absSecs*29.97);
                    frameNumber +=  18*(frameNumber/17982) + 2*(((frameNumber%17982) - 2) / 1798);

                    hours  = int((((frameNumber / 30) / 60) / 60) % 24);
                    mins   = int(((frameNumber / 30) / 60) % 60);
                    secs   = int((frameNumber / 30) % 60);
                    frames = int(frameNumber % 30);
                }
                else {
                    hours  = (int) (absSecs / (60.0 * 60.0));
                    mins   = ((int) (absSecs / 60.0)) % 60;
                    secs   = ((int) absSecs) % 60;
                    frames = (int)(int64(absSecs*frameRate) % (int)frameRate);
                }
                if (pos.isPlaying)
                {
                    double i=0.0;
                    const double clockppq = fmod(absSecs*frameRate*4.0,(long double)1.0);
                    samplesToNextMTC = (int)(samplesPerClock * (clockppq+i));
                    i+=1.0;
                    if (!wasPlaying) {
                        //this is so the song position pointer will be sent before any
                        //other data at the beginning of the song
                        MidiBuffer temp = midiMessages;
                        midiMessages.clear();

                        if (samplesToNextMTC<buffer.getNumSamples()) {
                            int mtcData;
                            switch (mtcNumber)
                            {
                            case 0: mtcData=frames&0x0f; break;
                            case 1: mtcData=(frames&0xf0)>>4; break;
                            case 2: mtcData=secs&0x0f; break;
                            case 3: mtcData=(secs&0xf0)>>4; break;
                            case 4: mtcData=mins&0x0f; break;
                            case 5: mtcData=(mins&0xf0)>>4; break;
                            case 6: mtcData=hours&0x0f; break;
                            case 7: mtcData=(hours&0x10)>>4 | mtcFrameRate; break;
                            }
                            MidiMessage midiclock(0xf1,(mtcNumber<<4)|(mtcData));
                            ++mtcNumber;
                            mtcNumber&=0x07;
                            midiMessages.addEvent(midiclock,samplesToNextMTC);
                            samplesToNextMTC = (int)(samplesPerClock * (clockppq+i));
                            i+=1.0;
                            startMTCAt=-999.0;
                            sendmtc=true;
                        }

                        midiMessages.addEvents(temp,0,buffer.getNumSamples(),0);
                    }
                    if (startMTCAt >-999.0 && (int)(samplesPerPpq*(startMTCAt-pos.ppqPosition))<buffer.getNumSamples()) {
                            samplesToNextMTC = (int)(samplesPerPpq*(startMTCAt-pos.ppqPosition));
                            int mtcData;
                            switch (mtcNumber)
                            {
                            case 0: mtcData=frames&0x0f; break;
                            case 1: mtcData=(frames&0xf0)>>4; break;
                            case 2: mtcData=secs&0x0f; break;
                            case 3: mtcData=(secs&0xf0)>>4; break;
                            case 4: mtcData=mins&0x0f; break;
                            case 5: mtcData=(mins&0xf0)>>4; break;
                            case 6: mtcData=hours&0x0f; break;
                            case 7: mtcData=(hours&0x10)>>4 | mtcFrameRate; break;
                            }
                            MidiMessage midiclock(0xf1,(mtcNumber<<4)|(mtcData));
                            ++mtcNumber;
                            mtcNumber&=0x07;
                            midiMessages.addEvent(midiclock,samplesToNextMTC);
                            samplesToNextMTC = (int)(samplesPerClock * (clockppq+i));
                            i+=1.0;
                            startMTCAt=-999.0;
                            sendmtc=true;
                    }
                    if (sendmtc) {
                        while (samplesToNextMTC<buffer.getNumSamples()) {
                            int mtcData;
                            switch (mtcNumber)
                            {
                            case 0: mtcData=frames&0x0f; break;
                            case 1: mtcData=(frames&0xf0)>>4; break;
                            case 2: mtcData=secs&0x0f; break;
                            case 3: mtcData=(secs&0xf0)>>4; break;
                            case 4: mtcData=mins&0x0f; break;
                            case 5: mtcData=(mins&0xf0)>>4; break;
                            case 6: mtcData=hours&0x0f; break;
                            case 7: mtcData=(hours&0x10)>>4 | mtcFrameRate; break;
                            }
                            MidiMessage midiclock(0xf1,(mtcNumber<<4)|(mtcData));
                            ++mtcNumber;
                            mtcNumber&=0x07;
                            midiMessages.addEvent(midiclock,samplesToNextMTC);
                            samplesToNextMTC = (int)(samplesPerClock * (clockppq+i));
                            i+=1.0;
                        }
                    }
                }
                else { //not playing
                    if (wasPlaying) {
                        //just stopped, send MIDI Stop
                        //MidiMessage stop(0xfc);
                        //midiMessages.addEvent(stop,0);
                        sendmtc=false;
                    }
                    uint8 ffsysexdata[] ={0xF0, 0x7F, 0x00, 0x01, 0x01, (uint8)(hours&0xff), (uint8)(mins&0xff), (uint8)(secs&0xff), (uint8)(frames&0xff), 0xF7};
                    MidiMessage fullframe(ffsysexdata,10);
                    if (midiOutput){
                        midiOutput->sendMessageNow(fullframe);
                    }
                    mtcNumber=0;
                }
            }
            if (param[kClock]>=0.5f) {
                //keep these as doubles to minimize rounding errors
                const double samplesPerPpq=60.0*SR/pos.bpm;
                const double ppqPerSample=pos.bpm/(60.0*SR);
                //const double ppqPerClock=1.0/24.0;
                const double samplesPerClock=2.5*SR/pos.bpm;
                const double clockppq = fmod(pos.ppqPosition*24.0,1.0);
                if (pos.isPlaying) {
                    double i=0.0;
                    samplesToNextClock = (int)(samplesPerClock * (clockppq+i));
                    i+=1.0;
                    if (!wasPlaying) {
                        //this is so the song position pointer will be sent before any
                        //other data at the beginning of the song
                        MidiBuffer temp = midiMessages;
                        midiMessages.clear();

                        //send song position pointer & continue
                        //MIDI beat == 16th note
                        const double beat = floor(pos.ppqPosition*4.0);
                        int intbeat = (int)beat;
                        if (pos.ppqPosition*4.0 - beat > 0.0000000000000001) {
                            intbeat+=1;
                        }
                        if (intbeat!=0) {
                            MidiMessage position( 0xf2, intbeat&0x007f, (intbeat&0x3f80)>>7 );
                            midiMessages.addEvent(position,0);
                        }
                        startAt = (double)intbeat*0.25;
                        samplesToNextClock = (int)(samplesPerPpq*(startAt-pos.ppqPosition));

                        if (samplesToNextClock<buffer.getNumSamples()) {
                            if (intbeat==0) {
                                MidiMessage start(0xfa);
                                midiMessages.addEvent(start,samplesToNextClock);
                            }
                            else {
                                MidiMessage start(0xfb);
                                midiMessages.addEvent(start,samplesToNextClock);
                            }
                            MidiMessage midiclock(0xf8);
                            midiMessages.addEvent(midiclock,samplesToNextClock);
                            samplesToNextClock = (int)(samplesPerClock * (clockppq+i));
                            i+=1.0;
                            startAt=-999.0;
                            sendclock=true;
                        }

                        midiMessages.addEvents(temp,0,buffer.getNumSamples(),0);
                    }
                    if (startAt >-999.0 && (int)(samplesPerPpq*(startAt-pos.ppqPosition))<buffer.getNumSamples()) {
                            samplesToNextClock = (int)(samplesPerPpq*(startAt-pos.ppqPosition));
                            if (pos.ppqPosition==0.0) {
                                MidiMessage start(0xfa);
                                midiMessages.addEvent(start,samplesToNextClock);
                            }
                            else {
                                MidiMessage start(0xfb);
                                midiMessages.addEvent(start,samplesToNextClock);
                            }
                            MidiMessage midiclock(0xf8);
                            midiMessages.addEvent(midiclock,samplesToNextClock);
                            samplesToNextClock = (int)(samplesPerClock * (clockppq+i));
                            i+=1.0;
                            startAt=-999.0;
                            sendclock=true;
                    }
                    if (sendclock) {
                        while (samplesToNextClock<buffer.getNumSamples()) {
                            MidiMessage midiclock(0xf8);
                            midiMessages.addEvent(midiclock,samplesToNextClock);
                            samplesToNextClock = (int)(samplesPerClock * (clockppq+i));
                            i+=1.0;
                        }
                    }
                }
                else { //not playing
                    if (wasPlaying) {
                        //just stopped, send MIDI Stop
                        MidiMessage stop(0xfc);
                        midiMessages.addEvent(stop,0);
                        sendclock=false;
                    }
                    const double beat = floor(pos.ppqPosition*4.0);
                    int intbeat = (int)beat;
                    if (pos.ppqPosition*4.0 - beat > 0.0000000000000001) {
                        intbeat+=1;
                    }
                    if (lastPosInfo.ppqPosition != pos.ppqPosition) {
                        MidiMessage position( 0xf2, intbeat&0x007f, (intbeat&0x3f80)>>7 );
                        midiMessages.addEvent(position,0);
                    }
                }
            }
        }
        wasPlaying = pos.isPlaying;
        lastPosInfo = pos;
    }
    else
    {
        zeromem (&lastPosInfo, sizeof (lastPosInfo));
        lastPosInfo.timeSigNumerator = 4;
        lastPosInfo.timeSigDenominator = 4;
        lastPosInfo.bpm = 120;
    }

	const int channel = roundToInt(param[kChannel]*16.f);
	if (channel==0)
	{
		if (midiOutput)
		{
			if (param[kStamped]>=0.5f)
			{
				MidiBuffer::Iterator mid_buffer_iter(midiMessages);
				MidiMessage midi_message(0xf8e);
				int sample_number;
				while(mid_buffer_iter.getNextEvent(midi_message,sample_number)) {
					midiOutput->sendMessageNow(midi_message);
				}
			}
			else midiOutput->sendBlockOfMessages(midiMessages,Time::getMillisecondCounterHiRes()+1.0,SR);
		}

		if (param[kHostOut]<0.5f)
			midiMessages.clear();
	}
	else
	{
		MidiBuffer output;
		MidiBuffer::Iterator mid_buffer_iter(midiMessages);
		MidiMessage midi_message(0xf8e);
		int sample_number;
		while(mid_buffer_iter.getNextEvent(midi_message,sample_number)) {
			if (midi_message.getChannel()==0 || midi_message.getChannel()==channel)
			{
				output.addEvent(midi_message,sample_number);
				if (midiOutput && param[kStamped]>=0.5f)
					midiOutput->sendMessageNow(midi_message);
			}
		}

		if (midiOutput && param[kStamped]<0.5f)
			midiOutput->sendBlockOfMessages(output,Time::getMillisecondCounterHiRes()+1.0,SR);

		if (param[kHostOut]<0.5f)
			midiMessages.clear();
		else
			midiMessages = output;
	}
}

//==============================================================================
void MidiOutFilter::getCurrentProgramStateInformation (MemoryBlock& destData)
{
    // make sure the non-parameter settings are copied to the current program
    programs[curProgram].icon = icon;

	XmlElement xmlState ("MYPLUGINSETTINGS");
    xmlState.setAttribute ("pluginVersion", 1);
    xmlState.setAttribute ("program", getCurrentProgram());
    xmlState.setAttribute ("progname", getProgramName(getCurrentProgram()));
    for (int i=0;i<getNumParameters();i++)
        xmlState.setAttribute (String(i), param[i]);
    xmlState.setAttribute ("icon", icon);
    xmlState.setAttribute ("device", activeDevice);
    copyXmlToBinary (xmlState, destData);
}

void MidiOutFilter::getStateInformation(MemoryBlock &destData)
{
    // make sure the non-parameter settings are copied to the current program
    programs[curProgram].icon = icon;

    XmlElement xmlState ("MYPLUGINSETTINGS");
    xmlState.setAttribute ("pluginVersion", 1);
    xmlState.setAttribute ("program", getCurrentProgram());
    for (int p=0;p<getNumPrograms();p++) {
        String prefix = "P" + String(p) + ".";
        xmlState.setAttribute (prefix+"progname", programs[p].name);
        for (int i=0;i<getNumParameters();i++)
            xmlState.setAttribute (prefix+String(i), programs[p].param[i]);
        xmlState.setAttribute (prefix+"icon", programs[p].icon);
		xmlState.setAttribute (prefix+"device", programs[p].device);
    }
    copyXmlToBinary (xmlState, destData);
}

void MidiOutFilter::setCurrentProgramStateInformation (const void* data, int sizeInBytes)
{
    auto const xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            changeProgramName(getCurrentProgram(),xmlState->getStringAttribute ("progname", L"Default"));
            for (int i=0;i<getNumParameters();i++) {
                param[i] = (float) xmlState->getDoubleAttribute (String(i), param[i]);
            }
            icon = xmlState->getStringAttribute ("icon", icon);
			setActiveDevice(xmlState->getStringAttribute ("device", activeDevice));

            sendChangeMessage ();
        }
	}
}

void MidiOutFilter::setStateInformation (const void* data, int sizeInBytes) {
    auto const xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != 0)
    {
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            for (int p=0;p<getNumPrograms();p++) {
                String prefix = L"P" + String(p) + L".";
                for (int i=0;i<getNumParameters();i++) {
                    programs[p].param[i] = (float) xmlState->getDoubleAttribute (prefix+String(i), programs[p].param[i]);
                }
                programs[p].icon = xmlState->getStringAttribute (prefix+L"icon", programs[p].icon);
				programs[p].device = xmlState->getStringAttribute (prefix+L"device", programs[p].device);
                programs[p].name = xmlState->getStringAttribute (prefix+L"progname", programs[p].name);
            }
            init=true;
            setCurrentProgram(xmlState->getIntAttribute("program", 0));
        }
    }
}
