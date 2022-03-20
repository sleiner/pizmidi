piz_add_plugin(
  piz_midiChordAnalyzer
  PRODUCT_NAME "midiChordAnalyzer"
  DESCRIPTION "Chord Analyzer"
  PLUGIN_CODE "mCrA"
  HAS_GUI
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_midiChordAnalyzer
  INTERFACE # Headers
            "${CMAKE_CURRENT_LIST_DIR}/MidiChordAnalyzerEditor.h"
            "${CMAKE_CURRENT_LIST_DIR}/MidiChordAnalyzer.h"
            # Sources
            "${CMAKE_CURRENT_LIST_DIR}/MidiChordAnalyzerEditor.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/MidiChordAnalyzer.cpp")

target_link_libraries(
  piz_midiChordAnalyzer INTERFACE juce::juce_audio_utils
                                  juce::juce_data_structures juce::juce_events)
