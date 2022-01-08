piz_add_plugin(
  piz_midiChords
  PRODUCT_NAME "midiChords"
  DESCRIPTION "midiChords"
  PLUGIN_CODE "mCrd"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_midiChords
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/MidiChords.h"
          "${CMAKE_CURRENT_LIST_DIR}/MidiChordsEditor.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/MidiChords.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/MidiChordsEditor.cpp")

target_link_libraries(
  piz_midiChords PRIVATE juce::juce_audio_utils juce::juce_cryptography
                         juce::juce_gui_basics)
