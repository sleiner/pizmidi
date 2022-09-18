// SPDX-FileCopyrightText:  2022 Simon Leiner
// SPDX-License-Identifier: GPL-3.0-only

#include <cstdint>
#include <list>
#include <memory>
#include <utility>

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#ifdef PIZ_MULTIPLUGIN
#define PIZ_CREATE_MIDI_PROCESSOR(classname) pizCreateMidiProcessor_##classname()
#else
#define PIZ_CREATE_MIDI_PROCESSOR(classname) pizCreateMidiProcessor()
#endif

#define PIZ_DECLARE_MIDI_PROCESSOR_TYPE(classname) \
    ::std::unique_ptr<::piz::MidiProcessor> JUCE_CALLTYPE PIZ_CREATE_MIDI_PROCESSOR (classname)
#define PIZ_REGISTER_MIDI_PROCESSOR(classname)  \
    PIZ_DECLARE_MIDI_PROCESSOR_TYPE (classname) \
    {                                           \
        return ::std::make_unique<classname>(); \
    }

namespace piz
{

class MidiProcessor
{
public:
    using Identifier      = juce::Identifier;
    using MidiBuffer      = juce::MidiBuffer;
    using MidiMessage     = juce::MidiMessage;
    using ParameterGroup  = juce::AudioProcessorParameterGroup;
    using RangedParameter = juce::RangedAudioParameter;
    using String          = juce::String;

    using ParamList = std::list<std::unique_ptr<RangedParameter>>;

    enum class Status
    {
        kActive,
        kBypassed,
    };

    virtual ~MidiProcessor() = default;

    //==============================================================================================
    static const int kNumMidiChannels = 16;
    static const int kNumMidiNotes    = 128;

    //==============================================================================================
    [[nodiscard]] virtual String getName() const       = 0;
    [[nodiscard]] virtual ParamList createParameters() = 0;

    //==============================================================================================
    virtual void processMessages (MidiBuffer& messages, Status status) = 0;

protected:
    //==============================================================================================
    [[nodiscard]] String getParamID (String const& unwrapped_id)
    {
        return getName() + "_" + unwrapped_id;
    }

    template <class ParamType>
    static ParamType* addTo (ParamList& list,
                             std::unique_ptr<ParamType> param)
    {
        auto rawPtr = param.get();
        list.push_back (std::move (param));
        return rawPtr;
    }
};

} // namespace piz
