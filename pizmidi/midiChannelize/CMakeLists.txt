piz_add_plugin(
  piz_midiChannelize
  PRODUCT_NAME "midiChannelize"
  DESCRIPTION "midiChannelize"
  PLUGIN_CODE "mchz"
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(piz_midiChannelize
               INTERFACE "${CMAKE_CURRENT_LIST_DIR}/midiChannelize.cpp")

target_link_libraries(piz_midiChannelize INTERFACE juce::juce_audio_processors
                                                   juce::juce_core)
