/*
==============================================================================
midiMonitor

some code borrowed from Jost midi monitor:
http://www.anticore.org/jucetice/

==============================================================================
*/
#include "MidiMonitor.h"

#include "MidiMonitorEditor.h"

using juce::roundToInt;

//==============================================================================
/**
    This function must be implemented to create the actual plugin object that
    you want to use.
*/
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiMonitorPlugin();
}

//==============================================================================
MidiMonitorPlugin::MidiMonitorPlugin()
    : loop(0), messages(0)
{
    iter         = 0;
    lastUIWidth  = 392;
    lastUIHeight = 250;
    loop         = new juce::MidiMessageSequence();
    messages     = new juce::MidiMessageSequence();

    useNotes = true;
    useCC    = true;
    usePB    = true;
    usePA    = true;
    useCP    = true;
    usePC    = true;
    useSysEx = true;
    useOther = true;
    useClock = true;

    waitingForFirstMessage = true;

    bghue    = 1.f; //0.61f;
    bgsat    = 0.f; //0.52f;
    bgbri    = 1.f; //0.11f;
    contrast = 1.0f;
    power    = 1.0f;
    thru     = 1.0f;
    bytes    = 1.0f;
    wrap     = 0.f;
    showtime = 0.f;
    ticks    = 0.f;
    frames   = 0.f;
    mode     = 0.f;
    maxLines = 5000;

    programName = "midiMonitor " + juce::String(JucePlugin_VersionString);

    if (! loadDefaultFxb())
        loadDefaultFxp();
}

MidiMonitorPlugin::~MidiMonitorPlugin()
{
    deleteAndZero(loop);
    deleteAndZero(messages);
}

//==============================================================================
int MidiMonitorPlugin::getNumParameters()
{
    return kNumParams;
}

float MidiMonitorPlugin::getParameter(int index)
{
    switch (index)
    {
        case kBgHue:
            return bghue;
            break;
        case kBgSat:
            return bgsat;
            break;
        case kBgBri:
            return bgbri;
            break;
        case kContrast:
            return contrast;
            break;
        case kPower:
            return power;
            break;
        case kThru:
            return thru;
            break;
        case kBytes:
            return bytes;
            break;
        case kWrap:
            return wrap;
            break;
        case kTime:
            return showtime;
            break;
        case kTicks:
            return ticks;
            break;
        case kTimeMode:
            return mode;
            break;
        case kShowms:
            return 1.f;
            break;
        case kShowhrs:
            return 1.f;
            break;
        case kShowsubfr:
            return 1.f;
            break;
        case kFrames:
            return frames;
            break;

        default:
            return 0.0f;
            break;
    }
}

void MidiMonitorPlugin::setParameter(int index, float newValue)
{
    switch (index)
    {
        case kBgHue:
            bghue = newValue;
            break;
        case kBgSat:
            bgsat = newValue;
            break;
        case kBgBri:
            bgbri = newValue;
            break;
        case kContrast:
            contrast = newValue;
            break;
        case kPower:
            power = newValue;
            break;
        case kThru:
            thru = newValue;
            break;
        case kBytes:
            bytes = newValue;
            break;
        case kWrap:
            wrap = newValue;
            break;
        case kTime:
            showtime = newValue;
            break;
        case kTicks:
            ticks = newValue;
            break;
        case kTimeMode:
            mode = newValue;
            break;
        case kFrames:
            frames = newValue;
            break;
        default:
            break;
    }
    sendChangeMessage();
}

const juce::String MidiMonitorPlugin::getParameterName(int index)
{
    switch (index)
    {
        case kBgHue:
            return juce::String("Hue");
            break;
        case kBgSat:
            return juce::String("Saturation");
            break;
        case kBgBri:
            return juce::String("Brightness");
            break;
        case kContrast:
            return juce::String("Contrast");
            break;
        case kPower:
            return juce::String("Power");
            break;
        case kThru:
            return juce::String("Thru");
            break;
        case kBytes:
            return juce::String("ShowBytes");
            break;
        case kWrap:
            return juce::String("WordWrap");
            break;
        case kTime:
            return juce::String("ShowTime");
            break;
        case kTicks:
            return juce::String("TicksPerBeat");
            break;
        case kTimeMode:
            return juce::String("TimeMode");
            break;
        case kFrames:
            return juce::String("FramesPerSec");
            break;
        default:
            return juce::String();
            break;
    }
}

const juce::String MidiMonitorPlugin::getParameterText(int index)
{
    switch (index)
    {
        case kBgHue:
            return juce::String(roundToInt(100.f * bghue));
            break;
        case kBgSat:
            return juce::String(roundToInt(100.f * bgsat));
            break;
        case kBgBri:
            return juce::String(roundToInt(100.f * bgbri));
            break;
        case kContrast:
            return juce::String(roundToInt(100.f * contrast));
            break;
        case kPower:
            return juce::String(power);
            break;
        case kThru:
            return juce::String(thru);
            break;
        case kBytes:
            return juce::String(bytes);
            break;
        case kWrap:
            return juce::String(wrap);
            break;
        case kTime:
            return juce::String(showtime);
            break;
        case kTicks:
            return juce::String(ticks);
            break;
        case kTimeMode:
            return juce::String(mode);
            break;
        case kFrames:
            return juce::String(frames);
            break;
        default:
            return juce::String();
            break;
    }
}

const juce::String MidiMonitorPlugin::getInputChannelName(const int channelIndex) const
{
    return juce::String(channelIndex + 1);
}

const juce::String MidiMonitorPlugin::getOutputChannelName(const int channelIndex) const
{
    return juce::String(channelIndex + 1);
}

bool MidiMonitorPlugin::isInputChannelStereoPair(int index) const
{
    return true;
}

bool MidiMonitorPlugin::isOutputChannelStereoPair(int index) const
{
    return true;
}

//==============================================================================
void MidiMonitorPlugin::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    midiCollector.reset(sampleRate);
}

void MidiMonitorPlugin::releaseResources()
{
    midiCollector.reset(44100);
}

void MidiMonitorPlugin::processBlock(juce::AudioSampleBuffer& buffer,
                                     juce::MidiBuffer& midiMessages)
{
    ++iter;
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
    if (power >= 0.5f)
    {
        juce::AudioPlayHead::CurrentPositionInfo pos;
        if (getPlayHead() != 0 && getPlayHead()->getCurrentPosition(pos))
        {
            if (memcmp(&pos, &lastPosInfo, sizeof(pos)) != 0)
            {
                lastPosInfo = pos;
            }
        }
        else
        { //no timeinfo
            zeromem(&lastPosInfo, sizeof(lastPosInfo));
            lastPosInfo.timeSigNumerator   = 4;
            lastPosInfo.timeSigDenominator = 4;
            lastPosInfo.bpm                = 120;
            lastPosInfo.ppqPosition        = samples * (120 / (60 * getSampleRate()));
        }
        //if (param[kSync]>=0.5f) { //sample sync
        //    lastPosInfo.ppqPosition = samples*(lastPosInfo.bpm/(60*getSampleRate()));
        //    lastPosInfo.isPlaying = true;
        //}
        const double ppqPerSample = lastPosInfo.bpm / (60.0 * getSampleRate());
        //const bool playing = lastPosInfo.isPlaying || lastPosInfo.isRecording;

        n = lastPosInfo.timeSigNumerator;
        d = lastPosInfo.timeSigDenominator;

        double counter = juce::Time::getMillisecondCounterHiRes();
        for (auto&& msgMetadata : midiMessages)
        {
            auto message   = msgMetadata.getMessage();
            auto samplePos = msgMetadata.samplePosition;

            bool use = false;
            if ((message.isNoteOnOrOff() && useNotes)
                || (message.isController() && useCC)
                || (message.isPitchWheel() && usePB)
                || (message.isProgramChange() && usePC)
                || (message.isAftertouch() && usePA)
                || (message.isChannelPressure() && useCP)
                || (message.isMidiClock() && useClock)
                || (message.isSysEx() && useSysEx)
                || ((message.isActiveSense()
                     || message.isMetaEvent()
                     || message.isMidiContinue()
                     || message.isMidiStart()
                     || message.isMidiStop()
                     || message.isMidiMachineControlMessage()
                     || message.isQuarterFrame()
                     || message.isSongPositionPointer())
                    && useOther))
            {
                if (waitingForFirstMessage)
                {
                    samples                = 0;
                    waitingForFirstMessage = false;
                }
                //message.setTimeStamp(counter);
                //midiCollector.addMessageToQueue (message);

                message.setTimeStamp(0);
                if (getActiveEditor() != 0)
                {
                    if (getParameter(kTimeMode) == 1.f)
                        messages->addEvent(message, (double) iter + (double) samplePos * 0.000001);
                    else
                        messages->addEvent(message, lastPosInfo.ppqPosition + ((double) samplePos) * ppqPerSample);
                }
                loop->addEvent(message, roundToInt((samples + (double) samplePos) * ppqPerSample * 960.0));
                if (message.isNoteOnOrOff())
                {
                    loop->updateMatchedPairs();
                }
            }
        }
    }
    if (thru < 0.5f)
        midiMessages.clear();
    samples += buffer.getNumSamples();
}

//==============================================================================
juce::AudioProcessorEditor* MidiMonitorPlugin::createEditor()
{
    return new MidiMonitorEditor(this);
}

//==============================================================================
void MidiMonitorPlugin::getStateInformation(juce::MemoryBlock& destData)
{
    // you can store your parameters as binary data if you want to or if you've got
    // a load of binary to put in there, but if you're not doing anything too heavy,
    // XML is a much cleaner way of doing it - here's an example of how to store your
    // params as XML..

    // create an outer XML element..
    juce::XmlElement xmlState("MYPLUGINSETTINGS");

    // add some attributes to it..
    xmlState.setAttribute("pluginVersion", 1);
    xmlState.setAttribute("Hue", bghue);
    xmlState.setAttribute("Sat", bgsat);
    xmlState.setAttribute("Bri", bgbri);
    xmlState.setAttribute("Contrast", contrast);
    xmlState.setAttribute("Power", power);
    xmlState.setAttribute("Thru", thru);
    xmlState.setAttribute("ShowBytes", bytes);
    xmlState.setAttribute("WordWrap", wrap);
    xmlState.setAttribute("ShowTime", showtime);
    xmlState.setAttribute("TimeMode", mode);
    xmlState.setAttribute("TicksPerBeat", ticks);
    xmlState.setAttribute("FramesPerSec", frames);
    xmlState.setAttribute("MaxLines", maxLines);

    xmlState.setAttribute("useNotes", useNotes);
    xmlState.setAttribute("useCC", useCC);
    xmlState.setAttribute("usePB", usePB);
    xmlState.setAttribute("usePC", usePC);
    xmlState.setAttribute("useCP", useCP);
    xmlState.setAttribute("usePA", usePA);
    xmlState.setAttribute("useClock", useClock);
    xmlState.setAttribute("useSysEx", useSysEx);
    xmlState.setAttribute("useOther", useOther);

    xmlState.setAttribute("uiWidth", lastUIWidth);
    xmlState.setAttribute("uiHeight", lastUIHeight);

    // you could also add as many child elements as you need to here..

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary(xmlState, destData);
}

void MidiMonitorPlugin::setStateInformation(const void* data, int sizeInBytes)
{
    // use this helper function to get the XML from this binary blob..
    auto const xmlState = getXmlFromBinary(data, sizeInBytes);

    if (xmlState != nullptr)
    {
        // check that it's the right type of xml..
        if (xmlState->hasTagName("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            bghue    = (float) xmlState->getDoubleAttribute("Hue", bghue);
            bgsat    = (float) xmlState->getDoubleAttribute("Sat", bgsat);
            bgbri    = (float) xmlState->getDoubleAttribute("Bri", bgbri);
            contrast = (float) xmlState->getDoubleAttribute("Contrast", contrast);
            power    = (float) xmlState->getDoubleAttribute("Power", power);
            thru     = (float) xmlState->getDoubleAttribute("Thru", thru);
            bytes    = (float) xmlState->getDoubleAttribute("ShowBytes", bytes);
            wrap     = (float) xmlState->getDoubleAttribute("WordWrap", wrap);
            showtime = (float) xmlState->getDoubleAttribute("ShowTime", showtime);
            mode     = (float) xmlState->getDoubleAttribute("TimeMode", mode);
            ticks    = (float) xmlState->getDoubleAttribute("TicksPerBeat", ticks);
            frames   = (float) xmlState->getDoubleAttribute("FramesPerSec", frames);
            maxLines = xmlState->getIntAttribute("MaxLines", maxLines);

            useNotes = xmlState->getBoolAttribute("useNotes", true);
            useCC    = xmlState->getBoolAttribute("useCC", true);
            usePB    = xmlState->getBoolAttribute("usePB", true);
            usePC    = xmlState->getBoolAttribute("usePC", true);
            useCP    = xmlState->getBoolAttribute("useCP", true);
            usePA    = xmlState->getBoolAttribute("usePA", true);
            useClock = xmlState->getBoolAttribute("useClock", true);
            useSysEx = xmlState->getBoolAttribute("useSysEx", true);
            useOther = xmlState->getBoolAttribute("useOther", true);

            lastUIWidth  = xmlState->getIntAttribute("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute("uiHeight", lastUIHeight);

            sendChangeMessage();
        }
    }
    this->dispatchPendingMessages();
}

bool MidiMonitorPlugin::writeMidiFile(juce::File mid)
{
    juce::MidiFile midifile;
    midifile.setTicksPerQuarterNote(960);
    juce::MidiMessageSequence metadata;

    juce::MidiMessage nstart = juce::MidiMessage(0x9f, 62, 1, 0);
    metadata.addEvent(nstart, 0.0);

    juce::uint8 tn[]            = { 0xFF, 0x03, 11, 'm', 'i', 'd', 'i', 'M', 'o', 'n', 'i', 't', 'o', 'r' };
    juce::MidiMessage trackname = juce::MidiMessage(tn, 14, 0);
    loop->addEvent(trackname);

    midifile.addTrack(*loop);

    if (mid.exists())
        mid.deleteFile();
    if (mid.create())
    {
        juce::FileOutputStream file(mid);
        midifile.writeTo(file);
    }
    return true;
}

void MidiMonitorPlugin::clearLoop()
{
    loop->clear();
    samples                = 0;
    waitingForFirstMessage = true;
};
