piz_add_plugin(
  piz_midiLooper
  PRODUCT_NAME "midiLooper"
  DESCRIPTION "MIDI looper/sampler"
  PLUGIN_CODE "L0OP"
  IS_MIDI_EFFECT TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_midiLooper
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/MidiLoop.h"
          "${CMAKE_CURRENT_LIST_DIR}/PianoRoll.h"
          "${CMAKE_CURRENT_LIST_DIR}/piz_MidiMessageSequence.h"
          "${CMAKE_CURRENT_LIST_DIR}/PizLooper.h"
          "${CMAKE_CURRENT_LIST_DIR}/PizLooperEditor.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/MidiLoop.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/PianoRoll.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/piz_MidiMessageSequence.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/PizLooper.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/PizLooperEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/PizLooperProcessing.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/Timeline.cpp")

target_link_libraries(
  piz_midiLooper
  PRIVATE juce::juce_audio_basics
          juce::juce_audio_devices
          juce::juce_audio_processors
          juce::juce_audio_utils
          juce::juce_core
          juce::juce_cryptography
          juce::juce_events
          juce::juce_gui_basics)
