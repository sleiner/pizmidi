piz_add_plugin(
  piz_Image
  DESCRIPTION "Converts audio amplitude to midi CC messages"
  PLUGIN_CODE "imag"
  PRODUCT_NAME "Image"
  HAS_GUI
  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE)

target_sources(
  piz_Image
  INTERFACE # Headers
            "${CMAKE_CURRENT_LIST_DIR}/imagePlugin.h"
            "${CMAKE_CURRENT_LIST_DIR}/imagePluginEditor.h"
            "${CMAKE_CURRENT_LIST_DIR}/MidiPad.h"
            # Sources
            "${CMAKE_CURRENT_LIST_DIR}/imagePluginEditor.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/MidiPad.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/imagePlugin.cpp")

target_link_libraries(
  piz_Image INTERFACE juce::juce_audio_processors juce::juce_gui_basics
                      juce::juce_gui_extra)
