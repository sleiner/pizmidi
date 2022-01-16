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

#include "piz/JArray.h"
#include "piz/PizAudioProcessor.h"

#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_core/juce_core.h"

#define goodXmlChars "abcdefghijklmnopqrstuvwxyz0123456789"

class EnvelopeFollower
{
public:
    EnvelopeFollower();

    void setup (double attackMs, double releaseMs, double sampleRate);

    float process (float v);

protected:
    double envelope;
    double a;
    double r;
};

enum
{
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
    numChannels = 2
};

#define maxGain (32.f)

class JuceProgram
{
    friend class AudioToCC;

public:
    JuceProgram();
    ~JuceProgram() {}

private:
    float param[numParams];
    String name;
    MidiDeviceInfo device;
};

//==============================================================================
class AudioToCC : public PizAudioProcessor,
                  public ChangeBroadcaster
{
public:
    //==============================================================================
    AudioToCC();
    ~AudioToCC() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer& buffer,
                       MidiBuffer& midiMessages) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;

    //==============================================================================
    const String getName() const override { return JucePlugin_Name; }
    bool hasEditor() const override { return true; }
    bool acceptsMidi() const override
    {
#if JucePlugin_WantsMidiInput
        return true;
#else
        return false;
#endif
    }
    bool producesMidi() const override
    {
#if JucePlugin_ProducesMidiOutput
        return true;
#else
        return false;
#endif
    }

    int getNumParameters() override;

    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const String getParameterName (int index) override;
    const String getParameterText (int index) override;

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;
    double getTailLengthSeconds() const override { return 0; }

    //==============================================================================
    int getNumPrograms() override { return 16; }
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void setActiveDevice (String name);
    void setActiveDevice (MidiDeviceInfo const& device);
    MidiDeviceInfo getActiveDevice() { return activeDevice; }
    MidiDeviceInfo getDeviceByName (String name) const;

    piz::JArray<MidiDeviceInfo> devices;
    int lastCCL, lastCCR;
    float lastInL, lastInR;
    bool lastGateCCL, lastGateCCR;

    //==============================================================================
    juce_UseDebuggingNewOperator

        private : float param[numParams];

    unsigned int rateCounter;
    double continualRMS[numChannels];
    double guiContinualRMS[numChannels];
    unsigned int samp[numChannels];
    int smooth (int dnew, int old, int older, float inertia);
    int oldenv[numChannels];
    int olderenv[numChannels];

    EnvelopeFollower* envL;
    EnvelopeFollower* envR;
    EnvelopeFollower* peakenvL;
    EnvelopeFollower* peakenvR;

    JuceProgram* programs;
    int curProgram;

    std::unique_ptr<MidiOutput> midiOutput;
    MidiDeviceInfo activeDevice;

    float maxAttack, maxRelease;

    bool resetGateL, resetGateR;
};

#endif
