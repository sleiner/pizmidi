piz_add_plugin(
  piz_midiIn
  PRODUCT_NAME "midiIn"
  DESCRIPTION "Receives external MIDI"
  PLUGIN_CODE "miii"
  IS_MIDI_EFFECT TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_midiIn
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/midiIn.h"
          "${CMAKE_CURRENT_LIST_DIR}/midiInEditor.h"
          "${CMAKE_CURRENT_LIST_DIR}/MidiPad.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/midiIn.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/midiInEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/MidiPad.cpp")

target_link_libraries(piz_midiIn PUBLIC juce::juce_audio_devices
                                        juce::juce_events)
