piz_add_plugin(
  piz_AudioToCC
  DESCRIPTION "Converts audio amplitude to midi CC messages"
  PLUGIN_CODE "A2CC"
  PRODUCT_NAME "Audio To CC"
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_AudioToCC
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/AudioToCC.h"
          "${CMAKE_CURRENT_LIST_DIR}/AudioToCCEditor.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/AudioToCCEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/AudioToCC.cpp")

target_link_libraries(
  piz_AudioToCC PRIVATE juce::juce_audio_devices juce::juce_core
                        juce::juce_gui_basics)
