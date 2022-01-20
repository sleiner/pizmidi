add_subdirectory(gui)

add_library(piz_common_build_options INTERFACE)
target_link_libraries(
  piz_common_build_options
  INTERFACE juce::juce_recommended_config_flags
            # Currently, there are too many warnings, so this is still off:
            # juce::juce_recommended_warning_flags
            juce::juce_recommended_lto_flags)

target_compile_definitions(
  piz_common_build_options
  INTERFACE JUCER_ENABLE_GPL_MODE=1 #
            JUCE_DISPLAY_SPLASH_SCREEN=0 #
            JUCE_VST3_CAN_REPLACE_VST2=0)

# Add a pizMidi plugin. This is a wrapper around juce_add_plugin which sets up
# some common properties for all plugins. The arguments are the same as
# juce_add_plugin except for the following additions/modifications:
#
# * NEEDS_WEB_BROWSER defaults to FALSE if not given
# * NEEDS_CURL will always be the same as NEEDS_WEB_BROWSER
function(piz_add_plugin target)
  set(options "")
  set(oneValueArgs NEEDS_WEB_BROWSER NEEDS_CURL)
  set(multiValueArgs "")
  cmake_parse_arguments(PIZ_ARG "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  # The need for a web browser must be explicitly requested:
  if(NOT DEFINED PIZ_ARG_NEEDS_WEB_BROWSER)
    set(PIZ_ARG_NEEDS_WEB_BROWSER FALSE)
  endif()

  # For now, these are the same
  set(PIZ_ARG_NEEDS_CURL "${PIZ_ARG_NEEDS_WEB_BROWSER}")

  # Set up the plugin with JUCE
  juce_add_plugin(
    "${target}"
    # Plugin specifics:
    ${PIZ_ARG_UNPARSED_ARGUMENTS}
    # Default values set by this wrapper:
    NEEDS_CURL ${PIZ_ARG_NEEDS_CURL}
    NEEDS_WEB_BROWSER ${PIZ_ARG_NEEDS_WEB_BROWSER}
    # Common values:
    FORMATS VST3
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
    PUBLIC piz_common_build_options)

  # Add plugin to installer
  piz_add_to_installer("${target}")

  # Ensure that JUCE_WEB_BROWSER/JUCE_USE_CURL matches
  # JUCE_NEEDS_WEB_BROWSER/JUCE_NEEDS_CURL
  target_compile_definitions(
    "${target}" PUBLIC JUCE_WEB_BROWSER=$<BOOL:${PIZ_ARG_NEEDS_WEB_BROWSER}>
                       JUCE_USE_CURL=$<BOOL:${PIZ_ARG_NEEDS_CURL}>)
endfunction(piz_add_plugin)
