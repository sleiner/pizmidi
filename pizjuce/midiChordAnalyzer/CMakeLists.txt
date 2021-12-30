piz_add_plugin(
  piz_midiChordAnalyzer
  PRODUCT_NAME "midiChordAnalyzer"
  DESCRIPTION "Chord Analyzer"
  PLUGIN_CODE "mCrA"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  VERSION "1.0.1")

target_sources(
  piz_midiChordAnalyzer
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiChordAnalyzerEditor.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiChordAnalyzer.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiChordAnalyzerEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiChordAnalyzer.cpp")

target_link_libraries(
  piz_midiChordAnalyzer PUBLIC juce::juce_audio_utils
                               juce::juce_data_structures juce::juce_events)
