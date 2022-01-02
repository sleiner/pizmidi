piz_add_plugin(
  piz_midiLooper
  PRODUCT_NAME "midiLooper"
  DESCRIPTION "MIDI looper/sampler"
  PLUGIN_CODE "L0OP"
  IS_MIDI_EFFECT TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  VERSION "1.4.10")

target_sources(
  piz_midiLooper
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiLoop.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/PianoRoll.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/piz_MidiMessageSequence.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/PizLooper.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/PizLooperEditor.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiLoop.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/PianoRoll.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/piz_MidiMessageSequence.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/PizLooper.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/PizLooperEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/PizLooperProcessing.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/Timeline.cpp")

target_link_libraries(
  piz_midiLooper
  PUBLIC juce::juce_audio_basics
         juce::juce_audio_devices
         juce::juce_audio_processors
         juce::juce_core
         juce::juce_cryptography
         juce::juce_events
         juce::juce_gui_basics)
