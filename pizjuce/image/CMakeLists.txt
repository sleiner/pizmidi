piz_add_plugin(
  piz_Image
  DESCRIPTION "Converts audio amplitude to midi CC messages"
  PLUGIN_CODE "imag"
  PRODUCT_NAME "Image"
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  VERSION "1.6.0")

target_sources(
  piz_Image
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/imagePlugin.h"
          "${CMAKE_CURRENT_LIST_DIR}/imagePluginEditor.h"
          "${CMAKE_CURRENT_LIST_DIR}/MidiPad.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/imagePluginEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/MidiPad.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/imagePlugin.cpp")

target_link_libraries(
  piz_Image PUBLIC juce::juce_audio_processors juce::juce_gui_basics
                   juce::juce_gui_extra)
