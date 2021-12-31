piz_add_plugin(
  piz_midiChords
  PRODUCT_NAME "midiChords"
  DESCRIPTION "midiChords"
  PLUGIN_CODE "mCrd"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  VERSION "1.2.5")

target_sources(
  piz_midiChords
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/src/ChordsKeyboardComponent.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiChords.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiChordsEditor.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/TriggerKeySelectorKeyboard.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiChords.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/MidiChordsEditor.cpp")

target_link_libraries(
  piz_midiChords PUBLIC juce::juce_audio_utils juce::juce_cryptography
                        juce::juce_gui_basics)
