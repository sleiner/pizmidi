piz_add_plugin(
  piz_midiChs
  PRODUCT_NAME "midiChs"
  DESCRIPTION "piz midi chs"
  PLUGIN_CODE "mChJ"
  IS_MIDI_EFFECT TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_midiChs
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/MidiChsProcessor.h"
          "${CMAKE_CURRENT_LIST_DIR}/MidiChsEditor.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/MidiChsProcessor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/MidiChsEditor.cpp")

target_link_libraries(piz_midiChs PRIVATE juce::juce_events
                                          juce::juce_gui_basics)
