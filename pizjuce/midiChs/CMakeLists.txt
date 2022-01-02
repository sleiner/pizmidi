piz_add_plugin(
  piz_midiChs
  PRODUCT_NAME "midiChs"
  DESCRIPTION "piz midi chs"
  PLUGIN_CODE "mChJ"
  IS_MIDI_EFFECT TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  VERSION "1.1.0")

target_sources(
  piz_midiChs
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiChsProcessor.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiChsEditor.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiChsProcessor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiChsEditor.cpp")

target_link_libraries(piz_midiChs PUBLIC juce::juce_events
                                         juce::juce_gui_basics)
