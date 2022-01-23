// SPDX-FileCopyrightText:  2022 Simon Leiner
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <functional>
#include <iostream>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <juce_audio_basics/juce_audio_basics.h>

#include "piz/MidiProcessor.h"

// We need some operators for JUCE objects that JUCE does not offer itself:
namespace juce
{

bool operator== (MidiMessageMetadata const& lhs, MidiMessageMetadata const& rhs);
void PrintTo (const MidiMessageMetadata& m, std::ostream* os);
void PrintTo (const MidiBuffer& m, std::ostream* os);

} // namespace juce

namespace piz
{

using ::testing::StartsWith;
using ::testing::TestWithParam;

using ProcessorGenerator = std::function<std::unique_ptr<MidiProcessor>()>;

void PrintTo (const ProcessorGenerator& g, std::ostream* os);

//==================================================================================================
class ProcessorTest : public ::testing::Test
{
public:
    void SetUp() override;

protected:
    [[nodiscard]] juce::RangedAudioParameter* getParam (juce::String id);
    [[nodiscard]] juce::MidiBuffer applyProcessor (
        juce::MidiBuffer const& inMessages,
        MidiProcessor::Status processorStatus = MidiProcessor::Status::kActive);

    [[nodiscard]] virtual std::unique_ptr<MidiProcessor> generateProcessor() const = 0;

    std::unique_ptr<MidiProcessor> processor_;
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters_;
};

#define PROCESSOR_TEST_INTERNAL(classname, suffix)                        \
    class classname##Test##suffix : public ::piz::ProcessorTest           \
    {                                                                     \
    protected:                                                            \
        std::unique_ptr<MidiProcessor> generateProcessor() const override \
        {                                                                 \
            return PIZ_CREATE_MIDI_PROCESSOR (classname);                 \
        }                                                                 \
    }

/// Create a test fixture for the MIDI processor with the given classname.
/// The name of the fixture will be <classname>Test.
#define PROCESSOR_TEST(classname) PROCESSOR_TEST_INTERNAL (classname, )

/// Create a test fixture for the MIDI processor with the given classname.
/// The name of the fixture will be <classname>TestBase. This is helpful if
/// you want to define <classname>Test yourself (and probably inherit from
/// <classname>TestBase).
#define PROCESSOR_TEST_BASE(classname) PROCESSOR_TEST_INTERNAL (classname, Base)

PIZ_DECLARE_MIDI_PROCESSOR_TYPE (Channelize);

//==================================================================================================
MATCHER_P (JContainerEq, expected, ::testing::PrintToString (expected))
{
    // Helper function that will turn a JUCE container into an std::vector
    auto vectorize = [] (auto juceContainer)
    {
        using iterator = decltype (juceContainer.begin());
        std::vector<typename iterator::value_type> v;

        for (auto&& element : juceContainer)
        {
            v.push_back (element);
        }

        return v;
    };

    return ::testing::ExplainMatchResult (::testing::ContainerEq (vectorize (expected)),
                                          vectorize (arg),
                                          result_listener);
}

} // namespace piz
