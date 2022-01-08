piz_add_plugin(
  piz_midiKeyboard
  PRODUCT_NAME "midiKeyboard"
  DESCRIPTION "MIDI Keyboard Plugin"
  PLUGIN_CODE "keyb"
  IS_MIDI_EFFECT TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE)

target_sources(
  piz_midiKeyboard
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/PizKeyboard.h"
          "${CMAKE_CURRENT_LIST_DIR}/PizKeyboardComponent.h"
          "${CMAKE_CURRENT_LIST_DIR}/PizKeyboardEditor.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/PizKeyboard.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/PizKeyboardComponent.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/PizKeyboardEditor.cpp")

target_link_libraries(
  piz_midiKeyboard
  PRIVATE juce::juce_audio_processors juce::juce_audio_utils juce::juce_events
          juce::juce_graphics juce::juce_gui_basics)
