// SPDX-FileCopyrightText:  2022 Simon Leiner
// SPDX-License-Identifier: GPL-3.0-only

#include <algorithm>
#include <string>

#include "unit_tests.h"

namespace piz
{

using juce::AudioParameterInt;
using juce::MidiBuffer;
using juce::MidiMessage;

PROCESSOR_TEST_BASE (Channelize);

class ChannelizeTest : public ChannelizeTestBase
{
public:
    void SetUp() override
    {
        ChannelizeTestBase::SetUp();

        ASSERT_NE (
            pOutChannel_ = dynamic_cast<AudioParameterInt*> (getParam ("Channelize_outChannel")),
            nullptr);
    }

protected:
    // parameters
    AudioParameterInt* pOutChannel_;
};

TEST_F (ChannelizeTest, MessagesAreRoutedToOutChannel)
{
    for (int outChannel = 1; outChannel <= 16; outChannel++)
    {
        *pOutChannel_ = outChannel;
        SCOPED_TRACE ("outChannel = " + std::to_string (outChannel));

        MidiBuffer inMsgs, expectedMsgs;
        for (int inChannel = 1; inChannel <= 16; inChannel++)
        {
            inMsgs.addEvent (MidiMessage::noteOn (inChannel, 0x12, (std::uint8_t) 0x34), 0);
            inMsgs.addEvent (MidiMessage::programChange (inChannel, 0x07u), 0);
            inMsgs.addEvent (MidiMessage::aftertouchChange (inChannel, 0, 0x42u), 0);
            inMsgs.addEvent (MidiMessage::channelPressureChange (inChannel, 0x42u), 0);
            inMsgs.addEvent (MidiMessage::controllerEvent (inChannel, 0x23u, 0x45u), 0);
            inMsgs.addEvent (MidiMessage::allNotesOff (inChannel), 0);
            inMsgs.addEvent (MidiMessage::allSoundOff (inChannel), 0);

            expectedMsgs.addEvent (MidiMessage::noteOn (outChannel, 0x12, (std::uint8_t) 0x34), 0);
            expectedMsgs.addEvent (MidiMessage::programChange (outChannel, 0x07u), 0);
            expectedMsgs.addEvent (MidiMessage::aftertouchChange (outChannel, 0, 0x42u), 0);
            expectedMsgs.addEvent (MidiMessage::channelPressureChange (outChannel, 0x42u), 0);
            expectedMsgs.addEvent (MidiMessage::controllerEvent (outChannel, 0x23u, 0x45u), 0);
            expectedMsgs.addEvent (MidiMessage::allNotesOff (outChannel), 0);
            expectedMsgs.addEvent (MidiMessage::allSoundOff (outChannel), 0);
        }

        auto outMsgs = applyProcessor (inMsgs);
        EXPECT_THAT (outMsgs, JContainerEq (expectedMsgs));
    }
}

TEST_F (ChannelizeTest, NoteOffIsSentToPreviousOutputChannel)
{
    int inChannel = 1;

    for (int prevOutChannel = 1, outChannel = 1;
         outChannel <= 16;
         prevOutChannel = outChannel, outChannel++)
    {
        *pOutChannel_ = outChannel;
        SCOPED_TRACE ("prevOutChannel = " + std::to_string (prevOutChannel) + ", "
                      + "outChannel = " + std::to_string (outChannel));

        MidiBuffer inMsgs;
        inMsgs.addEvent (MidiMessage::noteOff (inChannel, 0x12, (std::uint8_t) 0x56), 0);
        inMsgs.addEvent (MidiMessage::noteOn (inChannel, 0x12, (std::uint8_t) 0x34), 0);

        MidiBuffer expectedMsgs;
        expectedMsgs.addEvent (MidiMessage::noteOff (prevOutChannel, 0x12, (std::uint8_t) 0x56), 0);
        expectedMsgs.addEvent (MidiMessage::noteOn (outChannel, 0x12, (std::uint8_t) 0x34), 0);

        auto outMsgs = applyProcessor (inMsgs);
        EXPECT_THAT (outMsgs, JContainerEq (expectedMsgs));
    }
}

TEST_F (ChannelizeTest, NoteOffIsSentToMatchingOutputChannelIfBypassed)
{
    int inChannel = 1;
    SCOPED_TRACE ("inChannel = " + std::to_string (inChannel));

    for (int outChannel = 1; outChannel <= 16; outChannel++)
    {
        *pOutChannel_ = outChannel;
        SCOPED_TRACE ("outChannel = " + std::to_string (outChannel));

        { // Send Note On - processor is active => Note On @ output channel
            MidiBuffer inMsgs;
            inMsgs.addEvent (MidiMessage::noteOn (inChannel, 0x12, (std::uint8_t) 0x34), 0);

            MidiBuffer expectedMsgs;
            expectedMsgs.addEvent (MidiMessage::noteOn (outChannel, 0x12, (std::uint8_t) 0x34), 0);

            auto outMsgs = applyProcessor (inMsgs);
            EXPECT_THAT (outMsgs, JContainerEq (expectedMsgs));
        }

        { // Send Note Off - processor is active => Note Off @ output channel
            MidiBuffer inMsgs;
            inMsgs.addEvent (MidiMessage::noteOff (inChannel, 0x12, (std::uint8_t) 0x34), 0);

            MidiBuffer expectedMsgs;
            expectedMsgs.addEvent (MidiMessage::noteOff (outChannel, 0x12, (std::uint8_t) 0x34), 0);

            auto outMsgs = applyProcessor (inMsgs);
            EXPECT_THAT (outMsgs, JContainerEq (expectedMsgs));
        }

        { // Send Note Off - processor is bypassed => Note Off @ output channel
            MidiBuffer inMsgs;
            inMsgs.addEvent (MidiMessage::noteOff (inChannel, 0x12, (std::uint8_t) 0x34), 0);

            MidiBuffer expectedMsgs;
            expectedMsgs.addEvent (MidiMessage::noteOff (outChannel, 0x12, (std::uint8_t) 0x34), 0);

            auto outMsgs = applyProcessor (inMsgs, MidiProcessor::Status::kBypassed);
            EXPECT_THAT (outMsgs, JContainerEq (expectedMsgs));
        }

        { // Send Note On - processor is bypassed => Note On @ input channel
            MidiBuffer inMsgs;
            inMsgs.addEvent (MidiMessage::noteOn (inChannel, 0x12, (std::uint8_t) 0x34), 0);

            MidiBuffer expectedMsgs;
            expectedMsgs.addEvent (MidiMessage::noteOn (inChannel, 0x12, (std::uint8_t) 0x34), 0);

            auto outMsgs = applyProcessor (inMsgs, MidiProcessor::Status::kBypassed);
            EXPECT_THAT (outMsgs, JContainerEq (expectedMsgs));
        }

        { // Send Note Off - processor is bypassed => Note Off @ input channel
            MidiBuffer inMsgs;
            inMsgs.addEvent (MidiMessage::noteOff (inChannel, 0x12, (std::uint8_t) 0x34), 0);

            MidiBuffer expectedMsgs;
            expectedMsgs.addEvent (MidiMessage::noteOff (inChannel, 0x12, (std::uint8_t) 0x34), 0);

            auto outMsgs = applyProcessor (inMsgs, MidiProcessor::Status::kBypassed);
            EXPECT_THAT (outMsgs, JContainerEq (expectedMsgs));
        }
    }
}

} // namespace piz
