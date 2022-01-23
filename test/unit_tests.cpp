// SPDX-FileCopyrightText:  2022 Simon Leiner
// SPDX-License-Identifier: GPL-3.0-only

#include "unit_tests.h"

#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace juce
{

bool operator== (MidiMessageMetadata const& lhs,
                 MidiMessageMetadata const& rhs)
{
    return lhs.samplePosition == rhs.samplePosition
        && lhs.numBytes == rhs.numBytes
        && std::memcmp (lhs.data, rhs.data, static_cast<std::size_t> (lhs.numBytes)) == 0;
}

void PrintTo (const MidiMessageMetadata& m, std::ostream* os)
{
    *os << m.getMessage().getDescription() << " Position " << m.samplePosition;
}

void PrintTo (const MidiBuffer& m, std::ostream* os)
{
    *os << "MidiBuffer { ";
    for (auto&& msg : m)
    {
        *os << "(";
        PrintTo (msg, os);
        *os << "), ";
    }
    *os << "}";
}

} // namespace juce

namespace piz
{

class TestError : public std::runtime_error
{
    using std::runtime_error::runtime_error; // Use same constructors
};

void PrintTo (const ProcessorGenerator& g, std::ostream* os)
{
    *os << g()->getName();
}

void ProcessorTest::SetUp()
{
    processor_ = generateProcessor();
    ASSERT_NE (processor_, nullptr);

    parameters_ = processor_->createParameters();
}

juce::RangedAudioParameter* ProcessorTest::getParam (juce::String id)
{
    auto found = std::find_if (parameters_.begin(),
                               parameters_.end(),
                               [&id] (auto const& param)
                               { return param->paramID == id; });

    if (found == parameters_.end())
    {
        return nullptr;
    }

    return found->get();
}

juce::MidiBuffer ProcessorTest::applyProcessor (juce::MidiBuffer const& inMessages,
                                                MidiProcessor::Status processorStatus)
{
    auto outMessages = inMessages;
    processor_->processMessages (outMessages, processorStatus);
    return outMessages;
}

} // namespace piz
