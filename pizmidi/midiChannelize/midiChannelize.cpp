// SPDX-FileCopyrightText:  2011 Reuben Vinal, 2022 Simon Leiner
// SPDX-License-Identifier: GPL-3.0-only

#include "piz/MidiProcessor.h"

#include <cstdint>
#include <list>
#include <utility>

#include "piz/Array.h"

namespace piz
{
class Channelize : public MidiProcessor
{
    struct Parameters
    {
        juce::AudioParameterInt* outChannel = nullptr;
    };

public:
    std::list<std::unique_ptr<RangedParameter>> createParameters() override
    {
        std::list<std::unique_ptr<RangedParameter>> list;
        params_.outChannel = addTo (list,
                                    std::make_unique<juce::AudioParameterInt> (
                                        getParamID ("outChannel"), // ID
                                        "Output Channel",          // name
                                        1,                         // min value
                                        16,                        // max value
                                        1));
        return list;
    }

    String getName() const override { return "Channelize"; }

    Channelize()
    {
        for (std::uint8_t channel = 1; channel <= kNumMidiChannels; channel++)
        {
            lastChannel_[channel].fill (channel);
        }
    }

    void processMessages (MidiBuffer& midiEvents, Status status) override
    {
        auto outChannelParam = params_.outChannel->get();

        MidiBuffer inEvents;
        inEvents.swapWith (midiEvents);

        for (auto&& event : inEvents)
        {
            auto msgIn = event.getMessage();

            // When bypassed, we still need to process the messages in order to prevent hanging
            // notes. But of course, theoutput channel is determined by the channel of the
            // incoming message instead of the outChannel parameter.
            auto outChannel = (status == Status::kBypassed) ? msgIn.getChannel() : outChannelParam;
            auto msgOut     = transformMessage (msgIn, static_cast<std::uint8_t> (outChannel));

            midiEvents.addEvent (msgOut, event.samplePosition);
        }
    }

    MidiMessage transformMessage (MidiMessage msg, std::uint8_t outChannel)
    {
        auto inChannel = msg.getChannel();
        bool isSysex   = msg.getRawData()[0] >= 0xF0;

        if (isSysex)
        {
            // System Exclusive messages are just passed on
        }
        else
        {
            if (msg.isNoteOff())
            {
                msg.setChannel (lastChannel_[inChannel][msg.getNoteNumber()]);
            }
            else
            {
                msg.setChannel (outChannel);
            }

            if (msg.isNoteOn())
            {
                lastChannel_[inChannel][msg.getNoteNumber()] = outChannel;
            }
        }

        return msg;
    }

private:
    /// The last channel on to which an oncoming note was sent, split by note (128 possible values)
    /// and input channel (16) possible values. Since JUCE numbers MIDI channels from 1 to 16, we go
    /// along with the 1-based indexing here.
    Array<Array<std::uint8_t, 128>, kNumMidiChannels + 1> lastChannel_;

    Parameters params_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Channelize)
};

PIZ_REGISTER_MIDI_PROCESSOR (Channelize)
} // namespace piz
