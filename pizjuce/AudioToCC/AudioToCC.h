/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-7 by Raw Material Software ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the
   GNU General Public License, as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later version.

   JUCE is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with JUCE; if not, visit www.gnu.org/licenses or write to the
   Free Software Foundation, Inc., 59 Temple Place, Suite 330,
   Boston, MA 02111-1307 USA

  ------------------------------------------------------------------------------

   If you'd like to release a closed-source product which uses JUCE, commercial
   licenses are also available: visit www.rawmaterialsoftware.com/juce for
   more information.

  ==============================================================================
*/

#ifndef AUDIOTOCCPLUGINFILTER_H
#define AUDIOTOCCPLUGINFILTER_H

#include <memory>

#include "../_common/PizAudioProcessor.h"

#include "juce_audio_devices/juce_audio_devices.h"

#define goodXmlChars "abcdefghijklmnopqrstuvwxyz0123456789"

class EnvelopeFollower
{
public:
   EnvelopeFollower();

   void setup( double attackMs, double releaseMs, double sampleRate );

   float process( float v );


protected:
   double envelope;
   double a;
   double r;
};


enum {
	kLOut,
	kROut,
	kLClip,
	kRClip,
    kGain,
    kPeakGain,
    kCCL,
    kCCR,
    kChannel,
    kStereo,
    kRate,
    kSmooth,
	kAttack,
	kRelease,
	kMode,
    kDevice,
	kMidiToHost,
	kAutomateHost,
    kGateThreshold,
    kGateCCL,
    kGateCCR,
    kGateOnValueCCL,
    kGateOnValueCCR,
    kGateOffValueCCL,
    kGateOffValueCCR,
    kGateResetL,
    kGateResetR,

    numParams,
	numChannels=2
};

#define maxGain (32.f)

class JuceProgram {
friend class AudioToCC;
public:
	JuceProgram ();
	~JuceProgram () {}
private:
    float param[numParams];
    String name;
	String device;
};

//==============================================================================
class AudioToCC  : public PizAudioProcessor,
                   public ChangeBroadcaster
{
public:
    //==============================================================================
    AudioToCC();
    ~AudioToCC();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

	void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();

    //==============================================================================
    const String getName() const {return JucePlugin_Name;}
	bool hasEditor() const {return true;}
    bool acceptsMidi() const {
#if JucePlugin_WantsMidiInput
        return true;
#else
        return false;
#endif
    }
    bool producesMidi() const {
#if JucePlugin_ProducesMidiOutput
        return true;
#else
        return false;
#endif
    }

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;
    double getTailLengthSeconds() const override { return 0; }

    //==============================================================================
    int getNumPrograms()                                        { return 16; }
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

	void setActiveDevice(String name);
	String getActiveDevice() {return activeDevice;}
    StringArray devices;
    int lastCCL, lastCCR;
    float lastInL, lastInR;
	bool lastGateCCL, lastGateCCR;

	//==============================================================================
    juce_UseDebuggingNewOperator

private:
    float param[numParams];

    unsigned int rateCounter;
    double continualRMS[numChannels];
	double guiContinualRMS[numChannels];
	unsigned int samp[numChannels];
    int smooth(int dnew, int old, int older, float inertia);
    int oldenv[numChannels];
    int olderenv[numChannels];

	EnvelopeFollower* envL;
	EnvelopeFollower* envR;
	EnvelopeFollower* peakenvL;
	EnvelopeFollower* peakenvR;

    JuceProgram *programs;
    int curProgram;

    std::unique_ptr<MidiOutput> midiOutput;
	String activeDevice;

	float maxAttack,maxRelease;

	bool resetGateL, resetGateR;
};


#endif
