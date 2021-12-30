#ifndef DEMOJUCEPLUGINFILTER_H
#define DEMOJUCEPLUGINFILTER_H

#include "../_common/PizAudioProcessor.h"

#define denorm 0.000000000000001f

//==============================================================================
/**
    A simple plugin filter that just applies a gain change to the audio
    passing through it.

*/
class WebBrowserFilter  : public PizAudioProcessor,
                          public ChangeBroadcaster
{
public:
    //==============================================================================
    WebBrowserFilter();
    ~WebBrowserFilter();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

	void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();

    //==============================================================================
    const String getName() const;
	bool hasEditor() const {return true;}

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    double getTailLengthSeconds() const override { return 0; }

    //==============================================================================
    int getNumPrograms()                                        { return 0; }
    int getCurrentProgram()                                     { return 0; }
    void setCurrentProgram (int index)                          { }
    const String getProgramName (int index)                     { return String(); }
    void changeProgramName (int index, const String& newName)   { }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

    //==============================================================================
    // These properties are public so that our editor component can access them
    //  - a bit of a hacky way to do it, but it's only a demo!

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;

    String getURL() {return URL;}
    void setURL(const String& newURL) {URL=newURL;}
    bool initialPageLoaded;

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    // this is our gain - the UI and the host can access this by getting/setting
    // parameter 0.
    float gain;
    String URL;
    float lastinL;
    float lastoutL;
    float lastinR;
    float lastoutR;
    float R;
};


#endif
