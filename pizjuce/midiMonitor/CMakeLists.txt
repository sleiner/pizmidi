piz_add_plugin(
  piz_midiMonitor
  PRODUCT_NAME "midiMonitor"
  DESCRIPTION "piz midi monitor"
  PLUGIN_CODE "mmMM"
  IS_MIDI_EFFECT TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_midiMonitor
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/MidiMonitor.h"
          "${CMAKE_CURRENT_LIST_DIR}/MidiMonitorEditor.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/MidiMonitor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/MidiMonitorEditor.cpp")

target_link_libraries(
  piz_midiMonitor PRIVATE juce::juce_audio_devices juce::juce_events
                          juce::juce_gui_basics piz_common_gui)
