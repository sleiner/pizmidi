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
          "${CMAKE_CURRENT_LIST_DIR}/data/MidiLoop.h"
          "${CMAKE_CURRENT_LIST_DIR}/data/PizNote.h"
          "${CMAKE_CURRENT_LIST_DIR}/ui/PianoRoll.h"
          "${CMAKE_CURRENT_LIST_DIR}/data/piz_MidiMessageSequence.h"
          "${CMAKE_CURRENT_LIST_DIR}/PizLooper.h"
          "${CMAKE_CURRENT_LIST_DIR}/PizLooperEditor.h"
          "${CMAKE_CURRENT_LIST_DIR}/ui/KeySelector.h"
          "${CMAKE_CURRENT_LIST_DIR}/ui/PlayBar.h"
          "${CMAKE_CURRENT_LIST_DIR}/ui/PianoRollBackground.h"
          "${CMAKE_CURRENT_LIST_DIR}/ui/PianoRollNotes.h"
          "${CMAKE_CURRENT_LIST_DIR}/ui/PianoRollViewport.h"
          "${CMAKE_CURRENT_LIST_DIR}/ui/Timeline.h"
          "${CMAKE_CURRENT_LIST_DIR}/ui/CustomLookAndFeel.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/data/MidiLoop.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ui/PianoRoll.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/data/piz_MidiMessageSequence.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/PizLooper.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/PizLooperEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/PizLooperProcessing.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ui/KeySelector.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ui/PlayBar.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ui/PianoRollBackground.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ui/PianoRollNotes.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ui/PianoRollViewport.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ui/CustomLookAndFeel.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/ui/Timeline.cpp")

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
