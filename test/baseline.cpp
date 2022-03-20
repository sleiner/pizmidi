
// SPDX-FileCopyrightText:  2022 Simon Leiner
// SPDX-License-Identifier: GPL-3.0-only
//
// Baseline tests are the ones that apply to all MIDI processors

#include <cstdint>

#include "unit_tests.h"

namespace piz
{

using juce::MidiBuffer;
using juce::MidiMessage;
using juce::RangedAudioParameter;

class BaselineTest : public ProcessorTest,
                     public ::testing::WithParamInterface<ProcessorGenerator>
{
protected:
    std::unique_ptr<MidiProcessor> generateProcessor() const override
    {
        auto generator = GetParam();
        return generator();
    }
};

TEST_P (BaselineTest, ParamIDsStartWithProcessorName)
{
    auto processorName = processor_->getName().toStdString();

    for (auto&& param : parameters_)
    {
        auto name = param->paramID.toStdString();
        EXPECT_THAT (name, StartsWith (processorName + "_"));
    }
}

TEST_P (BaselineTest, MessagesAreNotModifiedIfBypassed)
{
    for (int channel = 1; channel <= 16; channel++)
    {
        MidiBuffer inMessages;
        inMessages.addEvent (MidiMessage::noteOn (channel, 0x12, (std::uint8_t) 0x34), 1);
        inMessages.addEvent (MidiMessage::programChange (channel, 0x07u), 2);
        inMessages.addEvent (MidiMessage::aftertouchChange (channel, 0, 0x42u), 3);
        inMessages.addEvent (MidiMessage::channelPressureChange (channel, 0x42u), 4);
        inMessages.addEvent (MidiMessage::controllerEvent (channel, 0x23u, 0x45u), 5);
        inMessages.addEvent (MidiMessage::allNotesOff (channel), 6);
        inMessages.addEvent (MidiMessage::allSoundOff (channel), 7);
        inMessages.addEvent (MidiMessage::midiStart(), 8);
        inMessages.addEvent (MidiMessage::midiStop(), 9);
        inMessages.addEvent (MidiMessage::midiClock(), 10);
        inMessages.addEvent (MidiMessage::createSysExMessage ("test", 4), 11);

        auto outMessages = applyProcessor (inMessages, MidiProcessor::Status::kBypassed);
        EXPECT_THAT (outMessages, JContainerEq (inMessages));
    }
}

TEST_P (BaselineTest, SpecialMessagesArePassedWhenProcessorIsActive)
{
    for (int channel = 1; channel <= 1; channel++)
    {
        MidiBuffer inMessages;
        inMessages.addEvent (MidiMessage::allNotesOff (channel), 6);
        inMessages.addEvent (MidiMessage::allSoundOff (channel), 7);
        inMessages.addEvent (MidiMessage::midiStart(), 8);
        inMessages.addEvent (MidiMessage::midiStop(), 9);
        inMessages.addEvent (MidiMessage::midiClock(), 10);
        inMessages.addEvent (MidiMessage::createSysExMessage ("test", 4), 11);

        auto outMessages = applyProcessor (inMessages);
        EXPECT_THAT (outMessages, JContainerEq (inMessages));
    }
}

INSTANTIATE_TEST_SUITE_P (All, BaselineTest, ::testing::ValuesIn ({
                                                 // clang-format off
    ProcessorGenerator([]() { return PIZ_CREATE_MIDI_PROCESSOR (Channelize); }),
                                                 // clang-format on
                                             }));
} // namespace piz
