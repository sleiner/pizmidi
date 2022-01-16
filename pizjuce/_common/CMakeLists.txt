add_subdirectory(base)
add_subdirectory(gui)

# This will collect all the sources for MIDI processors without a GUI:
add_library(piz_processors_without_gui INTERFACE)

add_library(piz_single_plugin INTERFACE)
target_sources(piz_single_plugin
               INTERFACE "${CMAKE_CURRENT_LIST_DIR}/SinglePlugin.cpp")

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
            JUCE_DISABLE_JUCE_VERSION_PRINTING=1 #
            JUCE_DISPLAY_SPLASH_SCREEN=0 #
            JUCE_VST3_CAN_REPLACE_VST2=0)

# Add a pizMidi plugin. This is a wrapper around juce_add_plugin which sets up
# some common properties for all plugins. The arguments are the same as
# juce_add_plugin except for the following additions/modifications:
#
# * NEEDS_WEB_BROWSER defaults to FALSE if not given
# * NEEDS_CURL will always be the same as NEEDS_WEB_BROWSER
function(piz_add_plugin target)
  set(options "HAS_GUI")
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

  # We will create multiple targets here:
  #
  # * $target will be an INTERFACE target containing the plugin implementation
  #   and all of its dependencies. This allows them to be included easily into
  #   the unit test suite.
  # * $plugin_target will be a static library as created by juce_add_plugin()
  #   which will contain all the sources from $target. Furthermore, JUCE will
  #   create an additionl target for each plugin format. In order to not compile
  #   the sources from $target again for each format, $plugin_target will link
  #   $target only privately.
  # * $options_target will be linked publicly by $plugin_target in order to
  #   propagate fully to all clients. It will only contain build options, but no
  #   sources.
  set(plugin_target "${target}_Single")
  set(options_target "${target}_Options")

  add_library("${target}" INTERFACE)
  add_library("${options_target}" INTERFACE)

  # Set up the plugin with JUCE
  juce_add_plugin(
    "${plugin_target}"
    # Plugin specifics:
    ${PIZ_ARG_UNPARSED_ARGUMENTS}
    # Default values set by this wrapper:
    NEEDS_CURL ${PIZ_ARG_NEEDS_CURL}
    NEEDS_WEB_BROWSER ${PIZ_ARG_NEEDS_WEB_BROWSER}
    # Common values:
    FORMATS VST3
    COMPANY_NAME "Pizeo"
    PLUGIN_MANUFACTURER_CODE "Pzeo")

  target_link_libraries("${options_target}" INTERFACE piz_common_build_options)
  target_link_libraries(
    "${target}" INTERFACE piz_common_base
                          $<$<BOOL:${PIZ_ARG_HAS_GUI}>:piz_common_gui>)
  target_link_libraries("${target}" INTERFACE "${options_target}")
  target_link_libraries(
    "${plugin_target}"
    # The plugin implementation only needs to be compiled once for the target
    # containing the shared code, not once for every plugin format - so we
    # should not link publicly:
    PRIVATE "${target}"
    # The options need to be linked publicly to take full effect:
    PUBLIC "${options_target}")

  # Add plugin to installer
  piz_add_to_installer("${plugin_target}")

  if(NOT ${PIZ_ARG_HAS_GUI})
    target_link_libraries(piz_processors_without_gui INTERFACE "${target}")
    target_link_libraries("${plugin_target}" PRIVATE piz_single_plugin)
    target_link_libraries("${options_target}"
                          INTERFACE juce_recommended_warning_flags)
    target_compile_definitions("${options_target}"
                               INTERFACE JUCE_DISABLE_GRAPHICS)
  endif()

  # Ensure that JUCE_WEB_BROWSER/JUCE_USE_CURL matches
  # JUCE_NEEDS_WEB_BROWSER/JUCE_NEEDS_CURL. We set these only for the plugin
  # target to allow the unit test suite to set these definitions itself.
  target_compile_definitions(
    "${plugin_target}"
    PUBLIC JUCE_WEB_BROWSER=$<BOOL:${PIZ_ARG_NEEDS_WEB_BROWSER}>
           JUCE_USE_CURL=$<BOOL:${PIZ_ARG_NEEDS_CURL}>)
endfunction(piz_add_plugin)
