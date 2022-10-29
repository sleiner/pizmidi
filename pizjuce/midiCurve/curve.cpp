#include "curve.h"

#include "curvegui.h"

using juce::roundToInt;

//==============================================================================
/*
    This function must be implemented to create the actual plugin object that
    you want to use.
*/
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiCurve();
}

//==============================================================================
MidiCurvePrograms::MidiCurvePrograms()
    : BankStorage("midiCurveValues", 1, kNumPrograms)
{
    for (int p = 0; p < getNumPrograms(); p++)
    {
        set(p, "progIndex", p);

        set(p, "x0", 0.f);
        set(p, "y0", 0.f);
        set(p, "PointType0", 1.f);
        set(p, "x1", 64.f * fmidiScaler);
        set(p, "y1", 64.f * fmidiScaler);
        set(p, "PointType1", 1.f);
        for (int i = 2; i < MAX_ENVELOPE_POINTS - 1; i++)
        {
            set(p, "x" + juce::String(i), (float) roundToInt(127.f * (float) i / (MAX_ENVELOPE_POINTS - 1)) * fmidiScaler);
            set(p, "y" + juce::String(i), (float) roundToInt(127.f * (float) i / (MAX_ENVELOPE_POINTS - 1)) * fmidiScaler);
            set(p, "PointType" + juce::String(i), 0.f);
        }
        set(p, "x" + juce::String(MAX_ENVELOPE_POINTS - 1), 1.f);
        set(p, "y" + juce::String(MAX_ENVELOPE_POINTS - 1), 1.f);
        set(p, "PointType" + juce::String(MAX_ENVELOPE_POINTS - 1), 1.f);

        set(p, "CC", 0.f);
        set(p, "CCNumber", 1.f * fmidiScaler);
        set(p, "Velocity", 1.f);
        set(p, "Aftertouch", 0.f);
        set(p, "ChannelPressure", 0.f);
        set(p, "PitchBend", 0.f);
        set(p, "Channel", 0.f);

        set(p, "Name", "Program " + juce::String(p + 1));
        set(p, "lastUIWidth", 600);
        set(p, "lastUIHeight", 400);
    }
}

//==============================================================================
MidiCurve::MidiCurve()
{
    // create built-in programs
    programs                 = new MidiCurvePrograms;
    juce::String defaultBank = juce::String();
    loadDefaultFxb();

    init = true;
    setCurrentProgram(0);
}

MidiCurve::~MidiCurve()
{
    if (programs)
        delete programs;
}

//==============================================================================
float MidiCurve::getParameter(int index)
{
    if (index < getNumParameters())
        return param[index];
    return 0.f;
}

void MidiCurve::setParameter(int index, float newValue)
{
    if (index < getNumParameters())
    {
        if (param[index] != newValue)
        {
            if (index == 0)
                param[index] = 0.f;
            else if (index == (MAX_ENVELOPE_POINTS * 2 - 2))
                param[index] = 1.f;
            else
            {
                param[index] = newValue;
                programs->set(curProgram, getParameterName(index), newValue);
            }
            updatePath();
            sendChangeMessage();
        }
    }
}

const juce::String MidiCurve::getParameterName(int index)
{
    if (index < kNumPointParams)
    {
        if (index % 2 == 0)
            return "x" + juce::String(index / 2);
        return "y" + juce::String(index / 2);
    }
    if (index < kNumPointParams + MAX_ENVELOPE_POINTS)
    {
        return "PointType" + juce::String(index - kActive);
    }
    if (index == kCC)
        return "CC";
    if (index == kCCNumber)
        return "CCNumber";
    if (index == kVelocity)
        return "Velocity";
    if (index == kChannelPressure)
        return "ChannelPressure";
    if (index == kAftertouch)
        return "AfterTouch";
    if (index == kChannel)
        return "Channel";
    if (index == kPitchBend)
        return "PitchBend";

    return "param" + juce::String(index);
}

const juce::String MidiCurve::getParameterText(int index)
{
    if (index < kNumPointParams)
    {
        return juce::String(roundToInt(127.f * param[index]));
    }
    if (index < kNumPointParams + MAX_ENVELOPE_POINTS)
    {
        if (param[index] < 0.5f)
            return "Off";
        if (param[index] < 1.f)
            return "Control";
        return "Corner";
    }
    if (index == kCC || index == kVelocity || index == kPitchBend
        || index == kChannelPressure || index == kAftertouch)
    {
        return param[index] < 0.5f ? "Off" : "On";
    }
    if (index == kCCNumber)
    {
        return juce::String(roundToInt(127.f * param[index]));
    }
    if (index == kChannel)
    {
        if (roundToInt(param[kChannel] * 16.0f) == 0)
            return juce::String("Any");
        return juce::String(roundToInt(param[kChannel] * 16.0f));
    }
    return juce::String();
}

const juce::String MidiCurve::getInputChannelName(const int channelIndex) const
{
    if (channelIndex < getNumInputChannels())
        return juce::String(JucePlugin_Name) + juce::String(" ") + juce::String(channelIndex + 1);
    return juce::String();
}

const juce::String MidiCurve::getOutputChannelName(const int channelIndex) const
{
    if (channelIndex < getNumOutputChannels())
        return juce::String(JucePlugin_Name) + juce::String(" ") + juce::String(channelIndex + 1);
    return juce::String();
}

bool MidiCurve::isInputChannelStereoPair(int index) const
{
    return true;
}

bool MidiCurve::isOutputChannelStereoPair(int index) const
{
    return true;
}

//======================Programs================================================
void MidiCurve::setCurrentProgram(int index)
{
    //save non-parameter info to the old program, except the first time
    if (! init)
    {
        programs->set(curProgram, "lastUIHeight", lastUIHeight);
        programs->set(curProgram, "lastUIWidth", lastUIWidth);
    }
    init = false;

    if (curProgram != index)
    {
        lastMsg.lastCCIn  = -1;
        lastMsg.lastCCOut = -1;
    }
    //then set the new program
    curProgram = index;
    programs->setGlobal("lastProgram", index);
    resetPoints(false);
    for (int i = 0; i < getNumParameters(); i++)
    {
        param[i] = programs->get(index, getParameterName(i));
    }
    lastUIWidth  = programs->get(index, "lastUIWidth");
    lastUIHeight = programs->get(index, "lastUIHeight");

    updatePath();
    sendChangeMessage();
}

void MidiCurve::changeProgramName(int index, const juce::String& newName)
{
    if (index < getNumPrograms())
        programs->set(index, "Name", newName);
}

const juce::String MidiCurve::getProgramName(int index)
{
    if (index < getNumPrograms())
        return programs->get(index, "Name");
    return juce::String();
}

int MidiCurve::getCurrentProgram()
{
    return curProgram;
}

//==============================================================================
void MidiCurve::prepareToPlay(double sampleRate, int samplesPerBlock)
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
    if (n < MAX_ENVELOPE_POINTS)
    {
        if (! y)
            return points[n].p.getX();
        else
            return 1.f - points[n].p.getY();
    }
    return -1.f;
}

float MidiCurve::findValue(float input)
{
    juce::PathFlatteningIterator it(path, {}, fmidiScaler);
    while (it.next())
    {
        if (it.x1 == input)
            return 1.f - it.y1;
        if (it.x2 >= input)
        {
            return 1.f - (float) linearInterpolate(input, it.y1, it.y2, it.x1, it.x2);
        }
    }

    return -1.f;
}

double MidiCurve::linearInterpolate(double x, double y1, double y2, double x1, double x2)
{
    double slope = (y2 - y1) / (x2 - x1);
    double y0    = y1 - slope * x1;
    return slope * x + y0;
}

void MidiCurve::processBlock(juce::AudioSampleBuffer& buffer,
                             juce::MidiBuffer& midiMessages)
{
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
    const int channel = roundToInt(param[kChannel] * 16.0f);
    juce::MidiBuffer output;
    for (auto&& msgMetadata : midiMessages)
    {
        auto midi_message  = msgMetadata.getMessage();
        auto sample_number = msgMetadata.samplePosition;

        if (midi_message.isProgramChange())
        {
            setCurrentProgram(midi_message.getProgramChangeNumber());
        }
        if (channel == 0 || midi_message.isForChannel(channel))
        {
            if (midi_message.isController())
            {
                if (param[kCC] >= 0.5f && midi_message.getControllerNumber() == roundToInt(param[kCCNumber] * 127.f))
                {
                    int v                   = midi_message.getControllerValue();
                    const juce::uint8* data = midi_message.getRawData();
                    lastMsg.lastCCIn        = v;
                    v                       = roundToInt(127.f * findValue(v * fmidiScaler));
                    lastMsg.lastCCOut       = v;
                    juce::MidiMessage cc    = juce::MidiMessage(data[0], data[1], v);
                    output.addEvent(cc, sample_number);
                    lastMsg.sendChangeMessage();
                }
                else
                    output.addEvent(midi_message, sample_number);
            }
            else if (midi_message.isNoteOn())
            {
                if (param[kVelocity] >= 0.5f)
                {
                    int v                   = midi_message.getVelocity();
                    const juce::uint8* data = midi_message.getRawData();
                    lastMsg.lastCCIn        = v;
                    v                       = roundToInt(127.f * findValue(v * fmidiScaler));
                    lastMsg.lastCCOut       = v;
                    juce::MidiMessage cc    = juce::MidiMessage(data[0], data[1], v);
                    output.addEvent(cc, sample_number);
                    lastMsg.sendChangeMessage();
                }
                else
                    output.addEvent(midi_message, sample_number);
            }
            else if (midi_message.isChannelPressure())
            {
                if (param[kChannelPressure] >= 0.5f)
                {
                    int v                   = midi_message.getChannelPressureValue();
                    const juce::uint8* data = midi_message.getRawData();
                    lastMsg.lastCCIn        = v;
                    v                       = roundToInt(127.f * findValue(v * fmidiScaler));
                    lastMsg.lastCCOut       = v;
                    juce::MidiMessage out   = juce::MidiMessage(data[0], v);
                    output.addEvent(out, sample_number);
                    lastMsg.sendChangeMessage();
                }
                else
                    output.addEvent(midi_message, sample_number);
            }
            else if (midi_message.isAftertouch())
            {
                if (param[kAftertouch] >= 0.5f)
                {
                    int v                   = midi_message.getAfterTouchValue();
                    const juce::uint8* data = midi_message.getRawData();
                    lastMsg.lastCCIn        = v;
                    v                       = roundToInt(127.f * findValue(v * fmidiScaler));
                    lastMsg.lastCCOut       = v;
                    juce::MidiMessage out   = juce::MidiMessage(data[0], data[1], v);
                    output.addEvent(out, sample_number);
                    lastMsg.sendChangeMessage();
                }
                else
                    output.addEvent(midi_message, sample_number);
            }
            else if (midi_message.isPitchWheel())
            {
                if (param[kPitchBend] >= 0.5f)
                {
                    int v                   = midi_message.getPitchWheelValue();
                    const juce::uint8* data = midi_message.getRawData();
                    lastMsg.lastCCIn        = (v & 0x3f80) >> 7;
                    v                       = findPBValue(v);
                    lastMsg.lastCCOut       = (v & 0x3f80) >> 7;
                    juce::MidiMessage out   = juce::MidiMessage(data[0], v & 0x007f, (v & 0x3f80) >> 7);
                    output.addEvent(out, sample_number);
                    lastMsg.sendChangeMessage();
                }
                else
                    output.addEvent(midi_message, sample_number);
            }
            else
                output.addEvent(midi_message, sample_number);
        }
        else
            output.addEvent(midi_message, sample_number);
    }
    midiMessages = output;
}

int MidiCurve::findPBValue(int input)
{
    float v = (float) input;
    juce::PathFlatteningIterator it(path, {}, (float) 0.0000610388);
    while (it.next())
    {
        if (it.x1 == v)
            return roundToInt(16383.f * (1.f - it.y1));
        if (it.x2 >= v)
        {
            return roundToInt(16383.f * (1.f - (float) linearInterpolate(v, it.y1, it.y2, it.x1, it.x2)));
        }
    }

    return -1;
}

void MidiCurve::updatePath()
{
    points.clear();
    for (int i = 0; i < MAX_ENVELOPE_POINTS; i++)
    {
        midiPoint p = midiPoint(param[i * 2], param[i * 2 + 1], isPointActive(i), isPointControl(i));
        points.addSorted(pointComparator, p);
    }
    juce::Path myPath;
    if (points[0].isActive)
    {
        myPath.startNewSubPath(0.f, getPointValue(0, 1));
        myPath.lineTo(0.f, getPointValue(0, 1));
    }
    else
    {
        myPath.startNewSubPath(0.f, 1.f);
        myPath.lineTo(0.f, 1.f);
    }
    for (int i = 1; i < MAX_ENVELOPE_POINTS; i++)
    {
        if (points[i].isActive && ! points[i].isControl)
        {
            int prev = getPrevActivePoint(i);
            if (points[prev].isControl)
            {
                if (points[getPrevActivePoint(prev)].isControl)
                {
                    myPath.cubicTo(getPointValue(getPrevActivePoint(prev), 0), getPointValue(getPrevActivePoint(prev), 1), getPointValue(prev, 0), getPointValue(prev, 1), getPointValue(i, 0), getPointValue(i, 1));
                }
                else
                {
                    myPath.quadraticTo(getPointValue(prev, 0), getPointValue(prev, 1), getPointValue(i, 0), getPointValue(i, 1));
                }
            }
            else
            {
                myPath.lineTo(getPointValue(i, 0), getPointValue(i, 1));
            }
        }
    }
    if (! points[MAX_ENVELOPE_POINTS - 1].isActive)
    {
        myPath.lineTo(1.f, 0.f);
    }
    path = myPath;
}

int MidiCurve::getPrevActivePoint(int currentPoint)
{
    for (int i = currentPoint - 1; i > 0; i--)
    {
        if (points[i].isActive)
            return i;
    }
    return 0;
}

int MidiCurve::getNextActivePoint(int currentPoint)
{
    for (int i = currentPoint + 1; i < MAX_ENVELOPE_POINTS; i++)
    {
        if (points[i].isActive)
            return i;
    }
    return MAX_ENVELOPE_POINTS - 1;
}

void MidiCurve::resetPoints(bool copyToProgram)
{
    for (int i = 0; i < MAX_ENVELOPE_POINTS; i++)
    {
        param[i * 2]       = (float) roundToInt(127.f * (float) i / (MAX_ENVELOPE_POINTS - 1)) * fmidiScaler;
        param[i * 2 + 1]   = param[i * 2];
        param[kActive + i] = 0.f;
    }
    param[kActive]                           = 1.f;
    param[kActive + 1]                       = 1.f;
    param[2]                                 = 64.f * fmidiScaler;
    param[3]                                 = 64.f * fmidiScaler;
    param[kActive + MAX_ENVELOPE_POINTS - 1] = 1.f;

    if (copyToProgram)
    {
        for (int i = 0; i < getNumParameters(); i++)
            programs->set(curProgram, getParameterName(i), param[i]);
    }
    updatePath();
    sendChangeMessage();
}

bool MidiCurve::isPointActive(int point)
{
    if (point < 0)
        return false;
    return getParameter(point + kActive) > 0.5f;
}

bool MidiCurve::isPointControl(int point)
{
    if (point < 0)
        return false;
    return getParameter(point + kActive) > 0.1f && getParameter(point + kActive) < 0.9f;
}

//==============================================================================
juce::AudioProcessorEditor* MidiCurve::createEditor()
{
    return new CurveEditor(this);
}

void MidiCurve::copySettingsToProgram(int index)
{
    for (int i = 0; i < getNumParameters(); i++)
        programs->set(index, getParameterName(i), param[i]);
    programs->set(index, "Name", getProgramName(index));
    programs->set(index, "lastUIHeight", lastUIHeight);
    programs->set(index, "lastUIWidth", lastUIWidth);
}

//==============================================================================
void MidiCurve::getCurrentProgramStateInformation(juce::MemoryBlock& destData)
{
    copySettingsToProgram(curProgram);
    programs->dumpProgramTo(0, curProgram, destData);
}

void MidiCurve::getStateInformation(juce::MemoryBlock& destData)
{
    copySettingsToProgram(curProgram);
    programs->dumpTo(destData);
}

void MidiCurve::setCurrentProgramStateInformation(const void* data, int sizeInBytes)
{
    //check for old format
    auto xmlState = getXmlFromBinary(data, sizeInBytes);
    if (xmlState != nullptr)
    {
        // check that it's the right type of xml..
        if (xmlState->hasTagName("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            changeProgramName(getCurrentProgram(), xmlState->getStringAttribute("progname", "Default"));
            for (int i = 0; i < kNumParams; i++)
            {
                param[i] = (float) xmlState->getDoubleAttribute(juce::String(i), param[i]);
            }
            lastUIWidth  = xmlState->getIntAttribute("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute("uiHeight", lastUIHeight);

            copySettingsToProgram(curProgram);

            updatePath();
            sendChangeMessage();
            this->dispatchPendingMessages();
        }
    }
    else
    {
        juce::MemoryInputStream stream(data, sizeInBytes, false);
        programs->loadProgram(curProgram, stream);
        init = true;
        setCurrentProgram(curProgram);
    }
}

void MidiCurve::setStateInformation(const void* data, int sizeInBytes)
{
    auto xmlState = getXmlFromBinary(data, sizeInBytes);
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName("MYPLUGINSETTINGS"))
        {
            for (int p = 0; p < getNumPrograms(); p++)
            {
                juce::String prefix = "P" + juce::String(p) + ".";
                for (int i = 0; i < kNumParams; i++)
                {
                    programs->set(p, getParameterName(i), (float) xmlState->getDoubleAttribute(prefix + juce::String(i), programs->get(p, getParameterName(i))));
                }
                programs->set(p, "lastUIWidth", xmlState->getIntAttribute(prefix + "uiWidth", programs->get(p, "lastUIWidth")));
                programs->set(p, "lastUIHeight", xmlState->getIntAttribute(prefix + "uiHeight", programs->get(p, "lastUIHeight")));
                programs->set(p, "Name", xmlState->getStringAttribute(prefix + "progname", programs->get(p, "Name")));
            }
            init = true;
            setCurrentProgram(xmlState->getIntAttribute("program", 0));
        }
    }
    else
    {
        juce::MemoryInputStream stream(data, sizeInBytes, false);
        juce::ValueTree vt = juce::ValueTree::readFromStream(stream);
        programs->loadFrom(vt);
        init = true;
        setCurrentProgram(vt.getProperty("lastProgram", 0));
    }
}
