add_library(pizjuce_common INTERFACE)
target_include_directories(pizjuce_common INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_sources(
  pizjuce_common
  INTERFACE # Headers
            "${CMAKE_CURRENT_LIST_DIR}/BankStorage.h"
            "${CMAKE_CURRENT_LIST_DIR}/DrawablePad.h"
            # Sources
            "${CMAKE_CURRENT_LIST_DIR}/ChordFunctions.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/DrawablePad.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/GuitarNeckComponent.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/Main.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/PizButton.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/BankStorage.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/VSTSlider.cpp")
target_link_libraries(
  pizjuce_common
  INTERFACE reaper_sdk #
            juce::juce_core
            juce::juce_audio_basics
            juce::juce_audio_devices
            juce::juce_audio_utils
            juce::juce_audio_plugin_client
            juce::juce_audio_processors
            juce::juce_data_structures
            juce::juce_gui_basics)

add_library(pizjuce_build_options INTERFACE)
target_link_libraries(
  pizjuce_build_options
  INTERFACE juce::juce_recommended_config_flags
            # Currently, there are too many warnings, so this is still off:
            # juce::juce_recommended_warning_flags
            juce::juce_recommended_lto_flags)

target_compile_definitions(
  pizjuce_common INTERFACE JUCE_MODAL_LOOPS_PERMITTED=1
                           JUCE_VST3_CAN_REPLACE_VST2=0)

function(piz_add_plugin target)
  # Set up the plugin with JUCE
  juce_add_plugin(
    "${target}"
    # Plugin specifics:
    ${ARGN}
    # Common values:
    FORMATS VST3
    COMPANY_NAME "Pizeo"
    PLUGIN_MANUFACTURER_CODE "Pzeo")

  # Add common code and options
  target_link_libraries(
    "${target}"
    # The options need to be linked publically to take full effect:
    PUBLIC pizjuce_build_options
    # The actual sources only need to be compiled once per target:
    PRIVATE pizjuce_common)

  # Add plugin to installer
  piz_add_to_installer("${target}")
endfunction(piz_add_plugin)
