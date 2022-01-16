piz_add_plugin(
  piz_KVRBrowser
  PRODUCT_NAME "KVR Browser"
  DESCRIPTION "Web Browser"
  PLUGIN_CODE "KVRb"
  NEEDS_WEB_BROWSER TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT TRUE
  EDITOR_WANTS_KEYBOARD_FOCUS TRUE)

target_sources(
  piz_KVRBrowser
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/WebBrowserPluginEditor.h"
          "${CMAKE_CURRENT_LIST_DIR}/WebBrowserFilter.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/WebBrowserPluginEditor.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/WebBrowserFilter.cpp")

target_link_libraries(
  piz_KVRBrowser PRIVATE juce::juce_events juce::juce_gui_extra piz_common_gui)
