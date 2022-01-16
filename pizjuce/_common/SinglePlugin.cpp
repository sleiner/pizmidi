// SPDX-FileCopyrightText:  2022 Simon Leiner
// SPDX-License-Identifier: GPL-3.0-only

#include <memory>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "piz/MidiProcessor.h"
#include "piz/PluginState.h"

namespace piz
{

using juce::AudioProcessorValueTreeState;
using juce::Identifier;
using juce::String;
using juce::ValueTree;

class SinglePlugin : public juce::AudioProcessor
{
public:
    SinglePlugin (std::unique_ptr<MidiProcessor> processor)
        : juce::AudioProcessor (BusesProperties()), // no audio input or output
          currentState_ (*this, nullptr, idOfSettings(), createLayout (*processor)),
          fullState_ (idOfSettings(), processor->getName(), 1, 1),
          processor_ (std::move (processor))
    {
        jassert (processor_ != nullptr);
    }

    //==============================================================================================
    void prepareToPlay (double, int) override {}
    void releaseResources() override {}

    //==============================================================================================
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    bool hasEditor() const override { return false; }

    //==============================================================================================
    const juce::String getName() const override { return processor_->getName(); }
    double getTailLengthSeconds() const override { return 0; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }

    //==============================================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    //==============================================================================================
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer& midiMessages) override
    {
        processor_->processMessages (midiMessages,
                                     isBypassed() ? MidiProcessor::Status::kBypassed
                                                  : MidiProcessor::Status::kActive);
    }

    void processBlockBypassed (juce::AudioBuffer<float>&, juce::MidiBuffer& midiMessages) override
    {
        processor_->processMessages (midiMessages, MidiProcessor::Status::kBypassed);
    }

    //==============================================================================================
    void getStateInformation (juce::MemoryBlock& destData) override
    {
        fullState_.setCurrentProcessor (processor_->getName());
        fullState_.updateStateOf (fullState_.getCurrentBank(),
                                  fullState_.getCurrentProgram(),
                                  currentState_.state);

        auto xml = fullState_.createXml();
        if (xml != nullptr)
        {
            copyXmlToBinary (*xml, destData);
        }
    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
        std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

        if (xmlState != nullptr)
        {
            fullState_.loadFrom (*xmlState);

            auto nextState = fullState_.getStateOf (fullState_.getCurrentBank(),
                                                    fullState_.getCurrentProgram());
            currentState_.state.copyPropertiesAndChildrenFrom (nextState, nullptr);
        }
    }

private:
    static AudioProcessorValueTreeState::ParameterLayout createLayout (MidiProcessor& processor)
    {
        auto params = processor.createParameters();
        return { params.begin(), params.end() };
    }

    bool isBypassed() const
    {
        auto bypassParam = getBypassParameter();
        if (bypassParam != nullptr)
        {
            return bypassParam->getValue() != 0;
        }

        // If no bypass parameter exists, we cannot be bypassed by it.
        return false;
    }

    /// This identifier will be embedded into AudioProcessorValueTreeState snapshots.
    /// When a snapshot is loaded, the existence of the correct ID will be checked.
    static Identifier idOfSettings() { return { "Pizeo.pizMidi" }; }

    // Number of MIDI banks / programs which can each store a set of plugin settings
    int numBanks() const { return 128; }
    int numPrograms() const { return 128; }

    /// Represents the AudioProcessorValueTreeState of the currently active program
    AudioProcessorValueTreeState currentState_;

    /// Represents the AudioProcessorValueTreeState of all programs
    PluginState fullState_;

    std::unique_ptr<MidiProcessor> processor_;
};

PIZ_DECLARE_MIDI_PROCESSOR_TYPE();

} // namespace piz

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    using namespace piz;
    return new SinglePlugin (PIZ_CREATE_MIDI_PROCESSOR());
}
