#include "BigClock.h"
#include "BigClockEditor.h"


//==============================================================================
/**
    This function must be implemented to create the actual plugin object that
    you want to use.
*/
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BigClockFilter();
}

//==============================================================================
BigClockFilter::BigClockFilter()
{
	recording = false;
	runwatch=false;
	rectime=0;
	watchtime=0;
	plugintime=Time::getMillisecondCounter();
	mode=0;
    barsbeats = 1.0f;
    look = 1.0f;
    showms = 1.0f;
    showhrs= 0.0f;
    showsubfr=0.0f;
    ticks = 0.0f;
    frames = 0.0f;
    samples = 0.0f;
    lastUIWidth = 250;
    lastUIHeight = 30;

    sampleRate = getSampleRate();

    bgcolor = Colour (0xffb8bcc0);

    zeromem (&lastPosInfo, sizeof (lastPosInfo));
    lastPosInfo.timeSigNumerator = 4;
    lastPosInfo.timeSigDenominator = 4;
    lastPosInfo.bpm = 120;

	loadDefaultFxp();

	lastTimerTime=Time::getMillisecondCounter();
	startTimer(10);
}

BigClockFilter::~BigClockFilter()
{
	stopTimer();
}

//==============================================================================
int BigClockFilter::getNumParameters()
{
    return kNumParams;
}

float BigClockFilter::getParameter (int index)
{
    if      (index == kBarsBeats) return barsbeats;
    else if (index == kLook) return look;
    else if (index == kShowms) return showms;
    else if (index == kShowhrs) return showhrs;
    else if (index == kTicks) return ticks;
    else if (index == kFrames) return frames;
    else if (index == kSamples) return samples;
    else if (index == kShowsubfr) return showsubfr;
    else if (index == kClockMode) return (float)mode/(float)(numModes-1);
	else if (index == kRunWatch) return runwatch ? 1.f : 0.f;
	else if (index == kClearWatch) return 0.f;
    else return 0.0f;
}

void BigClockFilter::setParameter (int index, float newValue)
{
    if (index == 0)
    {
        if (barsbeats != newValue)
        {
            barsbeats = newValue;
            sendChangeMessage ();
        }
    }
    else if (index == 1)
    {
        if (look != newValue)
        {
            look = newValue;
            sendChangeMessage ();
        }
    }
    else if (index == 2)
    {
        if (showms != newValue)
        {
            showms = newValue;
            sendChangeMessage ();
        }
    }
    else if (index == 3)
    {
        if (showhrs != newValue)
        {
            showhrs = newValue;
            sendChangeMessage ();
        }
    }
    else if (index == 4)
    {
        if (ticks != newValue)
        {
            ticks = newValue;
            sendChangeMessage ();
        }
    }
    else if (index == 5)
    {
        if (frames != newValue)
        {
            frames = newValue;
            sendChangeMessage ();
        }
    }
    else if (index == 6)
    {
        if (samples != newValue)
        {
            samples = newValue;
            sendChangeMessage ();
        }
    }
    else if (index == 7)
    {
        if (showsubfr != newValue)
        {
            showsubfr = newValue;
            sendChangeMessage ();
        }
    }
    else if (index == 8)
    {
        if (getParameter(index) != newValue)
        {
            mode = roundToInt(newValue*(float)(numModes-1));
            sendChangeMessage ();
        }
    }
    else if (index == 9)
    {
        if (getParameter(index) != newValue)
        {
            runwatch = newValue>=0.5f;
            sendChangeMessage ();
        }
    }
    else if (index == 10)
    {
        if (newValue>0.f)
        {
            watchtime=0;
            sendChangeMessage ();
        }
    }
}

const String BigClockFilter::getParameterName (int index)
{
    if (index == 0) return L"mode";
    if (index == 1) return L"contrast";
    if (index == 2) return L"show ms/frames";
    if (index == 3) return L"show hours";
    if (index == 4) return L"ppqn";
    if (index == 5) return L"fps";
    if (index == 6) return L"samples";
    if (index == 7) return L"subframes";
    if (index == 8) return L"clockmode";
    if (index == 9) return L"runwatch";
    else return String();
}

const String BigClockFilter::getParameterText (int index)
{
    if (index == 0) {
        if (barsbeats>=0.5f) return String(L"bar/beat/tick");
        else return String(L"hour/min/sec");
    }
    else if (index == 1) return String(look, 2);
    else if (index == 2) {
        if (showms>=0.5f) return String(L"yes");
        else return String(L"no");
    }
    else if (index == 3) {
        if (showhrs>=0.5f) return String(L"yes");
        else return String(L"no");
    }
    else if (index == 4) {
        if     (ticks==0.0) return String(L"960");
        else if (ticks<0.1) return String(L"768");
        else if (ticks<0.2) return String(L"480");
        else if (ticks<0.3) return String(L"384");
        else if (ticks<0.4) return String(L"240");
        else if (ticks<0.5) return String(L"192");
        else if (ticks<0.6) return String(L"120");
        else if (ticks<0.65) return String(L"100");
        else if (ticks<0.7) return String(L"96");
        else if (ticks<0.8) return String(L"48");
        else if (ticks<0.9) return String(L"16");
        else if (ticks<1.0) return String(L"4");
        else return String(L"hide ticks");
    }
    else if (index == 5) {
        if     (frames==0.0) return String(L"ms (1000)");
        else if (frames<0.1) return String(L"10");
        else if (frames<0.2) return String(L"15");
        else if (frames<0.3) return String(L"24");
        else if (frames<0.4) return String(L"25");
        else if (frames<0.5) return String(L"29.97 Drop");
        else if (frames<0.6) return String(L"30");
        else if (frames<0.7) return String(L"50");
        else if (frames<0.8) return String(L"60");
        else if (frames<0.9) return String(L"75");
        else if (frames<1.0) return String(L"100");
        else return String(L"120");
    }
    else return String();
}

const String BigClockFilter::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String BigClockFilter::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool BigClockFilter::isInputChannelStereoPair (int index) const
{
    return true;
}

bool BigClockFilter::isOutputChannelStereoPair (int index) const
{
    return true;
}

//==============================================================================
void BigClockFilter::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // do your pre-playback setup stuff here..
    this->sampleRate=sampleRate;
}

void BigClockFilter::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void BigClockFilter::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    //sampleRate=getSampleRate();
    AudioPlayHead::CurrentPositionInfo pos;

    if (getPlayHead() != 0 && getPlayHead()->getCurrentPosition (pos))
    {
        if (memcmp (&pos, &lastPosInfo, sizeof (pos)) != 0)
        {
            lastPosInfo = pos;
			if (reaper)
			{
				if (lastPosInfo.isPlaying)
					lastPosInfo.ppqPosition = TimeMap2_timeToBeats(0,GetPlayPosition(),0,0,0,0);
				else
					lastPosInfo.ppqPosition = TimeMap2_timeToBeats(0,GetCursorPosition(),0,0,0,0);
			}
			recording = lastPosInfo.isRecording;
            sendChangeMessage ();
        }
    }
    else
    {
        zeromem (&lastPosInfo, sizeof (lastPosInfo));
        lastPosInfo.timeSigNumerator = 4;
        lastPosInfo.timeSigDenominator = 4;
        lastPosInfo.bpm = 120;
    }


	if (recording)
		rectime += (double)buffer.getNumSamples()/getSampleRate();
    //MidiBuffer::Iterator mid_buffer_iter(midiMessages);
    //MidiMessage midi_message(0);
    //int sample_number;
    //while(mid_buffer_iter.getNextEvent(midi_message,sample_number)) {
    //    if (midi_message.isProgramChange()) {
    //        lastUIHeight=0;
    //        sendChangeMessage(this);
    //    }
    //}
}

//==============================================================================
AudioProcessorEditor* BigClockFilter::createEditor()
{
    return new BigClockEditor (this);
}

//==============================================================================
void BigClockFilter::getStateInformation (MemoryBlock& destData)
{
    XmlElement xmlState ("MYPLUGINSETTINGS");

    xmlState.setAttribute ("pluginVersion", 1);
    xmlState.setAttribute ("barsbeats", barsbeats);
    xmlState.setAttribute ("contrast", look);
    xmlState.setAttribute ("showms", showms);
    xmlState.setAttribute ("showhrs", showhrs);
    xmlState.setAttribute ("showsubfr", showsubfr);
    xmlState.setAttribute ("ticks", ticks);
    xmlState.setAttribute ("frames", frames);
    xmlState.setAttribute ("samples", samples);
    xmlState.setAttribute ("showcues", showcues);
    xmlState.setAttribute ("uiWidth", lastUIWidth);
    xmlState.setAttribute ("uiHeight", lastUIHeight);
    xmlState.setAttribute ("bgcolor", (int)(bgcolor.getARGB()));
	xmlState.setAttribute ("clockmode", mode);
	xmlState.setAttribute ("rectime", rectime);
	xmlState.setAttribute ("plugintime", (int)plugintime);
	xmlState.setAttribute ("watchtime", (int)watchtime);

    for (int i=0;i<cues.size();i++) {
        XmlElement* xmlCue = new XmlElement("Cue");
        xmlCue->setAttribute ("time", cues[i]->time);
        xmlCue->setAttribute ("ppq", cues[i]->ppq);
        xmlCue->setAttribute ("text", cues[i]->text);
        xmlCue->setAttribute ("enabled", cues[i]->enabled);
        xmlState.addChildElement(xmlCue);
    }

    copyXmlToBinary (xmlState, destData);
}

void BigClockFilter::setStateInformation (const void* data, int sizeInBytes)
{
    cues.clear();
    // use this helper function to get the XML from this binary blob..
    auto const xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != 0)
    {
        // check that it's the right type of xml..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            barsbeats = (float) xmlState->getDoubleAttribute ("gainLeve", barsbeats);
            barsbeats = (float) xmlState->getDoubleAttribute ("barsbeats", barsbeats);
            look = (float) xmlState->getDoubleAttribute ("contrast", look);
            showms = (float) xmlState->getDoubleAttribute ("showms", showms);
            showhrs = (float) xmlState->getDoubleAttribute ("showhrs", showhrs);
            showsubfr = (float) xmlState->getDoubleAttribute ("showsubfr", showsubfr);
            ticks = (float) xmlState->getDoubleAttribute ("ticks", ticks);
            frames = (float) xmlState->getDoubleAttribute ("frames", frames);
            samples = (float) xmlState->getDoubleAttribute ("samples", samples);
            showcues = xmlState->getBoolAttribute("showcues",showcues);
			mode = xmlState->getIntAttribute ("clockmode", mode);
			rectime = xmlState->getDoubleAttribute ("rectime", rectime);
			plugintime = xmlState->getIntAttribute ("plugintime", plugintime);
			watchtime = xmlState->getIntAttribute ("watchtime", watchtime);

            lastUIWidth = xmlState->getIntAttribute ("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);
            bgcolor = Colour(xmlState->getIntAttribute ("bgcolor", bgcolor.getARGB()));

            for (auto *e : xmlState->getChildIterator()) {
                if (e->hasTagName("Cue")) {
                    cue* newcue = new cue();
                    newcue->ppq = e->getDoubleAttribute("ppq");
                    newcue->time = e->getDoubleAttribute("time");
                    newcue->text = e->getStringAttribute("text");
                    newcue->enabled = e->getBoolAttribute("enabled");
                    cues.addSorted(c,newcue);
                }
            }


            sendChangeMessage ();
        }
    }
}


void BigClockFilter::saveCues(File cuefile) {
    XmlElement xmlState ("BigClockCues");

    double fps;
    if      (getParameter(kFrames)==0.0) fps=1000.0;
    else if (getParameter(kFrames)<0.1) fps=10.0;
    else if (getParameter(kFrames)<0.2) fps=15.0;
    else if (getParameter(kFrames)<0.3) fps=24.0;
    else if (getParameter(kFrames)<0.4) fps=25.0;
    else if (getParameter(kFrames)<0.5) fps=29.97;
    else if (getParameter(kFrames)<0.6) fps=30.0;
    else if (getParameter(kFrames)<0.7) fps=50.0;
    else if (getParameter(kFrames)<0.8) fps=60.0;
    else if (getParameter(kFrames)<0.9) fps=75.0;
    else if (getParameter(kFrames)<1.0) fps=100.0;
    else fps=120.0;
    for (int i=0;i<cues.size();i++) {
        XmlElement* xmlCue = new XmlElement("Cue");
        if (getParameter(kBarsBeats)<0.5f) xmlCue->setAttribute ("time", secondsToSmpteString(cues[i]->time,fps));
        else xmlCue->setAttribute ("barsbeats", ppqToBarsbeatsString(cues[i]->ppq,lastPosInfo.timeSigNumerator,lastPosInfo.timeSigDenominator));
        xmlCue->setAttribute ("text", cues[i]->text);
        xmlCue->setAttribute ("enabled", cues[i]->enabled);
        xmlState.addChildElement(xmlCue);
    }
    if (getParameter(kBarsBeats)<0.5f) {
        XmlElement* fr = new XmlElement("FrameRate");
        fr->setAttribute ("rate", fps);
        xmlState.addChildElement(fr);
    }
    else {
        XmlElement* ts = new XmlElement("TimeSig");
        ts->setAttribute ("numerator", lastPosInfo.timeSigNumerator);
        ts->setAttribute ("denominator", lastPosInfo.timeSigDenominator);
        xmlState.addChildElement(ts);

        double tpb;
        if     (getParameter(kTicks)==0.0) tpb=960.0;
        else if (getParameter(kTicks)<0.1) tpb=768.0;
        else if (getParameter(kTicks)<0.2) tpb=480.0;
        else if (getParameter(kTicks)<0.3) tpb=384.0;
        else if (getParameter(kTicks)<0.4) tpb=240.0;
        else if (getParameter(kTicks)<0.5) tpb=192.0;
        else if (getParameter(kTicks)<0.6) tpb=120.0;
        else if (getParameter(kTicks)<0.65) tpb=100.0;
        else if (getParameter(kTicks)<0.7) tpb=96.0;
        else if (getParameter(kTicks)<0.8) tpb=48.0;
        else if (getParameter(kTicks)<0.9) tpb=16.0;
        else if (getParameter(kTicks)<1.0) tpb=4.0;
        else tpb=0.0;
        XmlElement* ticks = new XmlElement("TicksPerBeat");
        ticks->setAttribute("value",tpb);
        xmlState.addChildElement(ticks);
    }

    xmlState.writeTo(cuefile);
}

void BigClockFilter::loadCues(File cuefile) {
    String xml = cuefile.loadFileAsString();
    XmlDocument xmldoc(xml);
    auto const xmlState = xmldoc.getDocumentElement();
    if (xmlState != 0)
    {
        // check that it's the right type of xml..
        if (xmlState->hasTagName ("BigClockCues"))
        {
            for (auto *e : xmlState->getChildIterator()) {
                double fps=24.0;
                double tpb=960.0;
                int n=4;
                int d=4;
                if (e->hasTagName("FrameRate")) {
                    fps = e->getDoubleAttribute("rate");
                    if      (fps==1000.0) setParameter(kFrames,0.0f);
                    else if (fps==10.0) setParameter(kFrames,0.09f);
                    else if (fps==15.0) setParameter(kFrames,0.19f);
                    else if (fps==24.0) setParameter(kFrames,0.29f);
                    else if (fps==25.0) setParameter(kFrames,0.39f);
                    else if (fps==29.97) setParameter(kFrames,0.49f);
                    else if (fps==30.0) setParameter(kFrames,0.59f);
                    else if (fps==50.0) setParameter(kFrames,0.69f);
                    else if (fps==60.0) setParameter(kFrames,0.79f);
                    else if (fps==75.0) setParameter(kFrames,0.89f);
                    else if (fps==100.0) setParameter(kFrames,0.99f);
                    else setParameter(kFrames,1.0f);
                }
                if (e->hasTagName("TimeSig")) {
                    n = e->getIntAttribute("numerator",4);
                    d = e->getIntAttribute("denominator",4);
                }
                if (e->hasTagName("TicksPerBeat")) {
                    double tpb = e->getDoubleAttribute("value");
                    if      (tpb==960.0)setParameter(kTicks,0.0f) ;
                    else if (tpb==768.0)setParameter(kTicks,0.09f) ;
                    else if (tpb==480.0)setParameter(kTicks,0.19f) ;
                    else if (tpb==384.0)setParameter(kTicks,0.29f) ;
                    else if (tpb==240.0)setParameter(kTicks,0.39f) ;
                    else if (tpb==192.0)setParameter(kTicks,0.49f) ;
                    else if (tpb==120.0)setParameter(kTicks,0.59f) ;
                    else if (tpb==100.0)setParameter(kTicks,0.64f);
                    else if (tpb==96.0) setParameter(kTicks,0.69f) ;
                    else if (tpb==48.0) setParameter(kTicks,0.79f) ;
                    else if (tpb==16.0) setParameter(kTicks,0.89f) ;
                    else if (tpb==4.0)  setParameter(kTicks,0.99f) ;
                    else setParameter(kTicks,1.0f);
                }
                else if (e->hasTagName("Cue")) {
                    cue* newcue = new cue();
                    newcue->ppq = barsbeatsStringToPpq(e->getStringAttribute("barsbeats",ppqToBarsbeatsString(smpteStringToSeconds(e->getStringAttribute("time"),fps)*lastPosInfo.bpm/60.0)));
                    newcue->time = smpteStringToSeconds(e->getStringAttribute("time",secondsToSmpteString(newcue->ppq*60.0/lastPosInfo.bpm,fps)),fps);
                    newcue->text = e->getStringAttribute("text");
                    newcue->enabled = e->getBoolAttribute("enabled",true);
                    cues.addSorted(c,newcue);
                }
            }
            sendChangeMessage ();
        }
    }
}


void BigClockFilter::addCue(double time, double ppq, String text) {
    cue* newcue = new cue(time,ppq,text);
    cues.addSorted(c,newcue);
    sendChangeMessage();
}

void BigClockFilter::setCueEnabled(int index, bool state) {
    cues[index]->enabled=state;
}

String BigClockFilter::getCue(double ppq, bool barsbeats) {
    for (int i=cues.size()-1;i>=0;i--) {
        if (!barsbeats) {
            if (cues[i]->time<=(ppq*60.0/lastPosInfo.bpm)
                && cues[i]->text.isNotEmpty()
                && cues[i]->enabled
                )
            {
                return ppqToString(cues[i]->time*lastPosInfo.bpm/60.0,
                    lastPosInfo.timeSigNumerator,
                    lastPosInfo.timeSigDenominator,
                    lastPosInfo.bpm,getParameter(kBarsBeats)>=0.5) + " - " + cues[i]->text;
            }
        }
        else {
            if (cues[i]->ppq<=ppq
                && cues[i]->text.isNotEmpty()
                && cues[i]->enabled
                )
            {
                return ppqToString(cues[i]->ppq,
                    lastPosInfo.timeSigNumerator,
                    lastPosInfo.timeSigDenominator,
                    lastPosInfo.bpm,getParameter(kBarsBeats)>=0.5) +  " - " + cues[i]->text;
            }
        }
    }
    return String();
}

const double BigClockFilter::secondsToPpq(const double seconds, const double bpm)
{
	return seconds * bpm / 60.0;
}

//==============================================================================
const String BigClockFilter::ppqToString (const double sppq,
                                          const int numerator,
                                          const int denominator,
                                          const double bpm,
                                          const bool mode)
{
    if (numerator == 0 || denominator == 0)
        return L"1|1|0";
    const wchar_t* const sign = (sppq < 0) ? L"-" : L"";

    const double ppq = fabs(sppq);
    const double ppqPerBar = ((double)numerator * 4.0 / (double)denominator);
    double beats  = (fmod (ppq, ppqPerBar) / ppqPerBar) * numerator;

    float tpb;
    if     (getParameter(kTicks)==0.0) tpb=960.0;
    else if (getParameter(kTicks)<0.1) tpb=768.0;
    else if (getParameter(kTicks)<0.2) tpb=480.0;
    else if (getParameter(kTicks)<0.3) tpb=384.0;
    else if (getParameter(kTicks)<0.4) tpb=240.0;
    else if (getParameter(kTicks)<0.5) tpb=192.0;
    else if (getParameter(kTicks)<0.6) tpb=120.0;
    else if (getParameter(kTicks)<0.65) tpb=100.0;
    else if (getParameter(kTicks)<0.7) tpb=96.0;
    else if (getParameter(kTicks)<0.8) tpb=48.0;
    else if (getParameter(kTicks)<0.9) tpb=16.0;
    else if (getParameter(kTicks)<1.0) tpb=4.0;
    else tpb=0.0;

    bool dropframe = false;
    float fps;
    if      (getParameter(kFrames)==0.0) fps=1000.0;
    else if (getParameter(kFrames)<0.1) fps=10.0;
    else if (getParameter(kFrames)<0.2) fps=15.0;
    else if (getParameter(kFrames)<0.3) fps=24.0;
    else if (getParameter(kFrames)<0.4) fps=25.0;
    else if (getParameter(kFrames)<0.5) {
        fps=29.97f;
        dropframe=true;
    }
    else if (getParameter(kFrames)<0.6) fps=30.0;
    else if (getParameter(kFrames)<0.7) fps=50.0;
    else if (getParameter(kFrames)<0.8) fps=60.0;
    else if (getParameter(kFrames)<0.9) fps=75.0;
    else if (getParameter(kFrames)<1.0) fps=100.0;
    else fps=120.0;

    int bar       = (int) (ppq / ppqPerBar) + 1;
    int beat      = ((int) beats) + 1;
    int ticks     = ((int) (fmod (beats, 1.0) * tpb));

    if(sppq<0) {
        bar-=1;
        beat=numerator-beat+1;
        ticks=(int)tpb-ticks-1;
    }

    long double seconds = (long double)(sppq*60.0/bpm);
    //if (playing) seconds = sec;
    //else seconds =

    const long double absSecs = fabs (seconds);
    const uint64 samples = roundToInt(sampleRate*absSecs);

    if (getParameter(kSamples)) return sign+String(samples);
    else {

    int hours;
    int mins;

    bool showms = getParameter(kShowms)>=0.5f;
    bool showhrs= getParameter(kShowhrs)>=0.5f;
    if (showhrs) {
        hours = (int) (absSecs / (60.0 * 60.0));
        mins  = ((int) (absSecs / 60.0)) % 60;
    }
    else mins = (int) (absSecs / 60.0);
    const int secs  = ((int) absSecs) % 60;


    String s1;
    if (showhrs) {
        if (showms) {
            if (fps==1000.0)
                s1 = String::formatted (L"%s%02d:%02d:%02d.%03d",
                                          sign, hours, mins, secs,
                                          int64 (absSecs * 1000) % 1000);
            else if (fps<=10.0)
                s1 = String::formatted (L"%s%02d:%02d:%02d:%.1d",
                                          sign, hours, mins, secs,
                                          int64(absSecs*fps) % (int)fps);
            else if (fps<=100.0) {
                if (dropframe) {
                    int64 frameNumber = int64(absSecs*29.97);
                    frameNumber +=  18*(frameNumber/17982) + 2*(((frameNumber%17982) - 2) / 1798);
                    int frames   = int(frameNumber % 30);
                    int dseconds = int((frameNumber / 30) % 60);
                    int dminutes = int(((frameNumber / 30) / 60) % 60);
                    int dhours   = int((((frameNumber / 30) / 60) / 60) % 24);

                    s1 = String::formatted (L"%s%02d;%02d;%02d;%.2d",
                                              sign, dhours, dminutes, dseconds,
                                              frames);
                }
                else
                    s1 = String::formatted (L"%s%02d:%02d:%02d:%.2d",
                                              sign, hours, mins, secs,
                                              int64(absSecs*fps) % (int)fps);
            }
            else
                s1 = String::formatted (L"%s%02d:%02d:%02d:%03d",
                                          sign, hours, mins, secs,
                                          int64(absSecs*fps) % (int)fps);
            if (showsubfr>=0.5f)
                s1+=String(L"::") + String::formatted (L"%02d", (int64(absSecs*fps*100.0) % 100));
        }
        else {
            s1 = String::formatted (L"%s%02d:%02d:%02d",
                                      sign, hours, mins, secs);
        }
    }
    else {
        if (showms) {
            if (fps==1000.0)
                s1 = String::formatted (L"%s%d:%02d.%03d",
                                          sign, mins, secs,
                                          int64 (absSecs * 1000) % 1000);
            else if (fps<=10.0)
                s1 = String::formatted (L"%s%d:%02d:%.1d",
                                          sign, mins, secs,
                                          int64(absSecs*fps) % (int)fps);
            else if (fps<=100.0) {
                if (dropframe) {
                    int64 frameNumber = int64(absSecs*29.97);
                    frameNumber +=  18*(frameNumber/17982) + 2*(((frameNumber%17982) - 2) / 1798);
                    int frames   = int(frameNumber % 30);
                    int dseconds = int((frameNumber / 30) % 60);
                    int dminutes = int(((frameNumber / 30) / 60) % 60);

                    s1 = String::formatted (L"%s%d:%02d;%.2d",
                                              sign, dminutes, dseconds,
                                              frames);
                }
                else
                    s1 = String::formatted (L"%s%d:%02d:%.2d",
                                              sign, mins, secs,
                                              int64(absSecs*fps) % (int)fps);
            }
            else
                s1 = String::formatted (L"%s%d:%02d:%03d",
                                          sign, mins, secs,
                                          int64(absSecs*fps) % (int)fps);
            if (showsubfr>=0.5f)
                s1+=String(L"::") + String::formatted (L"%02d", (int64(absSecs*fps*100.0) % 100));
        }
        else {
            s1 = String::formatted (L"%s%d:%02d",
                                      sign, mins, secs);
        }
    }


    String padding = String();
    if      (ticks<10 && tpb>100.0) padding = L"00";
    else if (ticks<100 && tpb>100.0) padding = L"0";
    else if (ticks<10 && tpb>=10.0) padding = L"0";

    String s;
    if (tpb>0.0) s << (bar==0?L"":sign) << bar << L"|" << beat << L"|" << padding << ticks;
    else s << (bar==0?L"":sign) << bar << L" | " << beat;
    if (mode) return s;
    else return s1;
    }
}

double BigClockFilter::smpteStringToSeconds(String smpte,double fps) {

    StringArray smpteArray;
    smpteArray.addTokens(smpte,":;","");
    return smpteArray[0].getDoubleValue()*60*60 //hours
        +  smpteArray[1].getDoubleValue()*60    //minutes
        +  smpteArray[2].getDoubleValue()       //seconds
        +  smpteArray[3].getDoubleValue()/fps;  //frames
}

String BigClockFilter::secondsToSmpteString(double seconds, double fps) {
	if (fps<0)
	{
	    bool dropframe = false;
		if      (getParameter(kFrames)==0.0) fps=1000.0;
		else if (getParameter(kFrames)<0.1) fps=10.0;
		else if (getParameter(kFrames)<0.2) fps=15.0;
		else if (getParameter(kFrames)<0.3) fps=24.0;
		else if (getParameter(kFrames)<0.4) fps=25.0;
		else if (getParameter(kFrames)<0.5) {
			fps=29.97f;
			dropframe=true;
		}
		else if (getParameter(kFrames)<0.6) fps=30.0;
		else if (getParameter(kFrames)<0.7) fps=50.0;
		else if (getParameter(kFrames)<0.8) fps=60.0;
		else if (getParameter(kFrames)<0.9) fps=75.0;
		else if (getParameter(kFrames)<1.0) fps=100.0;
		else fps=120.0;
	}
    const double absSecs = fabs(seconds);
    const wchar_t* const sign = (seconds < 0) ? L"-" : L"";
    const int hours = (int) (absSecs / (60.0 * 60.0));
    const int mins  = ((int) (absSecs / 60.0)) % 60;
    const int secs  = ((int) absSecs) % 60;
    if (fps<=100.0) {
        if (fps==29.97) {
            int64 frameNumber = int64(seconds*29.97);
            frameNumber +=  18*(frameNumber/17982) + 2*(((frameNumber%17982) - 2) / 1798);
            int frames   = int(frameNumber % 30);
            int dseconds = int((frameNumber / 30) % 60);
            int dminutes = int(((frameNumber / 30) / 60) % 60);
            int dhours   = int((((frameNumber / 30) / 60) / 60) % 24);

            return String::formatted (L"%s%02d;%02d;%02d;%.2d",
                                      sign, dhours, dminutes, dseconds,
                                      frames);
        }
        else
            return String::formatted (L"%s%02d:%02d:%02d:%.2d",
                                      sign, hours, mins, secs,
                                      int64(absSecs*fps) % (int)fps);
    }
    else
        return String::formatted (L"%s%02d:%02d:%02d:%03d",
                                  sign, hours, mins, secs,
                                  int64(absSecs*fps) % (int)fps);
}

double BigClockFilter::barsbeatsStringToPpq(String barsbeats, int n, int d) {
    float tpb;
    if     (getParameter(kTicks)==0.0) tpb=960.0;
    else if (getParameter(kTicks)<0.1) tpb=768.0;
    else if (getParameter(kTicks)<0.2) tpb=480.0;
    else if (getParameter(kTicks)<0.3) tpb=384.0;
    else if (getParameter(kTicks)<0.4) tpb=240.0;
    else if (getParameter(kTicks)<0.5) tpb=192.0;
    else if (getParameter(kTicks)<0.6) tpb=120.0;
    else if (getParameter(kTicks)<0.65) tpb=100.0;
    else if (getParameter(kTicks)<0.7) tpb=96.0;
    else if (getParameter(kTicks)<0.8) tpb=48.0;
    else if (getParameter(kTicks)<0.9) tpb=16.0;
    else if (getParameter(kTicks)<1.0) tpb=4.0;
    else tpb=0.0;

    StringArray smpteArray;
    smpteArray.addTokens(barsbeats,":","");
    const double ppqPerBar = ((double)n * 4.0 / (double)d);
    const double ppqPerBeat = 4.0/(double)d;
    return (smpteArray[0].getIntValue()-1)*ppqPerBar + (smpteArray[1].getIntValue()-1)*ppqPerBeat + smpteArray[2].getDoubleValue()/tpb;
}

String BigClockFilter::ppqToBarsbeatsString(double ppq, int n, int d) {

    const double ppqPerBar = ((double)n * 4.0 / (double)d);
    double beats  = (fmod (ppq, ppqPerBar) / ppqPerBar) * n;

    float tpb;
    if     (getParameter(kTicks)==0.0) tpb=960.0;
    else if (getParameter(kTicks)<0.1) tpb=768.0;
    else if (getParameter(kTicks)<0.2) tpb=480.0;
    else if (getParameter(kTicks)<0.3) tpb=384.0;
    else if (getParameter(kTicks)<0.4) tpb=240.0;
    else if (getParameter(kTicks)<0.5) tpb=192.0;
    else if (getParameter(kTicks)<0.6) tpb=120.0;
    else if (getParameter(kTicks)<0.65) tpb=100.0;
    else if (getParameter(kTicks)<0.7) tpb=96.0;
    else if (getParameter(kTicks)<0.8) tpb=48.0;
    else if (getParameter(kTicks)<0.9) tpb=16.0;
    else if (getParameter(kTicks)<1.0) tpb=4.0;
    else tpb=0.0;

    const int bar       = (int) (ppq / ppqPerBar) + 1;
    const int beat      = ((int) beats) + 1;
    const int ticks     = ((int) (fmod (beats, 1.0) * tpb));

    String padding = String();
    if      (ticks<10 && tpb>100.0) padding = L"00";
    else if (ticks<100 && tpb>100.0) padding = L"0";
    else if (ticks<10 && tpb>=10.0) padding = L"0";

    String s;
    if (tpb>0.0) s << bar << L":" << beat << L":" << padding << ticks;
    else s << bar << L":" << beat;
    return s;
}

void BigClockFilter::timerCallback()
{
	if (runwatch)
		watchtime += Time::getMillisecondCounter()-lastTimerTime;
	lastTimerTime = Time::getMillisecondCounter();
}
