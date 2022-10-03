add_library(pizjuce_common INTERFACE)
target_include_directories(pizjuce_common INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_sources(
  pizjuce_common
  INTERFACE # Headers
            "${CMAKE_CURRENT_LIST_DIR}/BankStorage.h"
            "${CMAKE_CURRENT_LIST_DIR}/ChannelSlider.h"
            "${CMAKE_CURRENT_LIST_DIR}/ChordFunctions.h"
            "${CMAKE_CURRENT_LIST_DIR}/ClickableLabel.h"
            "${CMAKE_CURRENT_LIST_DIR}/DrawablePad.h"
            "${CMAKE_CURRENT_LIST_DIR}/MIDI.h"
            "${CMAKE_CURRENT_LIST_DIR}/NoteSlider.h"
            "${CMAKE_CURRENT_LIST_DIR}/PizArray.h"
            "${CMAKE_CURRENT_LIST_DIR}/PizAudioProcessor.h"
            "${CMAKE_CURRENT_LIST_DIR}/PizButton.h"
            "${CMAKE_CURRENT_LIST_DIR}/VSTSlider.h"
            "${CMAKE_CURRENT_LIST_DIR}/key.h"
            "${CMAKE_CURRENT_LIST_DIR}/midistuff.h"
            # Sources
            "${CMAKE_CURRENT_LIST_DIR}/ChordFunctions.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/DrawablePad.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/PizButton.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/BankStorage.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/VSTSlider.cpp")
target_link_libraries(
  pizjuce_common
  INTERFACE juce::juce_core #
            juce::juce_audio_processors #
            juce::juce_data_structures #
            juce::juce_gui_basics)

add_library(pizjuce_build_options INTERFACE)
target_link_libraries(
  pizjuce_build_options
  INTERFACE juce::juce_recommended_config_flags
            # Currently, there are too many warnings, so this is still off:
            # juce::juce_recommended_warning_flags
            juce::juce_recommended_lto_flags)

target_compile_definitions(
  pizjuce_common
  INTERFACE JUCE_MODAL_LOOPS_PERMITTED=1 #
            JUCER_ENABLE_GPL_MODE=1 #
            JUCE_DISPLAY_SPLASH_SCREEN=0 #
            JUCE_VST3_CAN_REPLACE_VST2=0)

function(_piz_set_target_default_property target property default_value)
  get_target_property(value "${target}" "${property}")
  if("${value}" STREQUAL "value-NOTFOUND")
    set_target_properties("${target}" PROPERTIES "${property}"
                                                 "${default_value}")
  endif()
endfunction(_piz_set_target_default_property)

function(piz_add_plugin target)
  # Set up the plugin with JUCE
  juce_add_plugin(
    "${target}"
    # Plugin specifics:
    ${ARGN}
    # Common values:
    FORMATS VST3
    NEEDS_CURL TRUE
    COMPANY_NAME "Pizeo"
    PLUGIN_MANUFACTURER_CODE "Pzeo")

  # Set language standards
  set_target_properties(
    "${target}"
    PROPERTIES CXX_STANDARD 17
               CXX_EXTENSIONS OFF
               C_STANDARD 17
               C_EXTENSIONS OFF)

  # Add common code and options
  target_link_libraries(
    "${target}"
    # The options need to be linked publically to take full effect:
    PUBLIC pizjuce_build_options
    # The actual sources only need to be compiled once per target:
    PRIVATE pizjuce_common)

  # Add plugin to installer
  piz_add_to_installer("${target}")

  # Ensure that JUCE_NEEDS_WEB_BROWSER is always defined:
  _piz_set_target_default_property(${target} JUCE_NEEDS_WEB_BROWSER FALSE)

  # Ensure that JUCE_WEB_BROWSER matches JUCE_NEEDS_WEB_BROWSER
  get_target_property(has_web_browser ${target} JUCE_NEEDS_WEB_BROWSER)
  target_compile_definitions("${target}"
                             PUBLIC JUCE_WEB_BROWSER=$<BOOL:${has_web_browser}>)
endfunction(piz_add_plugin)
