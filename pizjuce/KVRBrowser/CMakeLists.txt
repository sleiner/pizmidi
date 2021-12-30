piz_add_plugin(
  pizmidi_KVRBrowser
  PRODUCT_NAME "KVR Browser"
  DESCRIPTION "Web Browser"
  PLUGIN_CODE "KVRb"
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  EDITOR_WANTS_KEYBOARD_FOCUS TRUE
  VERSION "1.1.0")

target_sources(
  pizmidi_KVRBrowser
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/WebBrowserPluginEditor.h"
          "${CMAKE_CURRENT_LIST_DIR}/WebBrowserFilter.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/WebBrowserPluginEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/WebBrowserFilter.cpp")

target_link_libraries(pizmidi_KVRBrowser PUBLIC juce::juce_events
                                                juce::juce_gui_extra)
