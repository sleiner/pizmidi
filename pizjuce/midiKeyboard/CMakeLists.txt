piz_add_plugin(
  piz_midiKeyboard
  PRODUCT_NAME "midiKeyboard"
  DESCRIPTION "MIDI Keyboard Plugin"
  PLUGIN_CODE "keyb"
  IS_MIDI_EFFECT TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  VERSION "1.4.1")

target_sources(
  piz_midiKeyboard
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/src/PizKeyboard.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/PizKeyboardComponent.h"
          "${CMAKE_CURRENT_LIST_DIR}/src/PizKeyboardEditor.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/src/PizKeyboard.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/PizKeyboardComponent.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/src/PizKeyboardEditor.cpp")

target_link_libraries(
  piz_midiKeyboard
  PUBLIC juce::juce_audio_processors juce::juce_audio_utils juce::juce_events
         juce::juce_graphics juce::juce_gui_basics)
