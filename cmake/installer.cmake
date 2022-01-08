# Global Parameters (must be set before CPack is initialized)
# ##############################################################################

# Installation Prefixes
# ---------------------

# The plugins are always installed into the folder $PREFIX/$PLUGIN_FORMAT. The
# prefixes differ by operating system.
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  # On macOS, we install the plugins into the _global_ folder. This makes the
  # plugin available to all users and, according to Steinberg, is the much more
  # common location for installing plugins.
  #
  # Source: https://helpcenter.steinberg.de/hc/en-us/articles/115000171310
  # Permalink:
  # https://web.archive.org/web/20210415101713/https://helpcenter.steinberg.de/hc/en-us/articles/115000171310
  set(GLOBAL_PLUGIN_INSTALL_PREFIX "/Library/Audio/Plug-Ins")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # If this is set to something other than an empty string, windows packages
  # (even plain ZIPs) are just empty.
  set(GLOBAL_PLUGIN_INSTALL_PREFIX "")
else() # Linux
  # For Linux, there does not seem to exist a common standard for where to put
  # VST plugins. So, we just pick a default path that suits us.
  set(GLOBAL_PLUGIN_INSTALL_PREFIX "/usr/lib/pizmidi")
endif()

set(CPACK_PACKAGING_INSTALL_PREFIX
    ${GLOBAL_PLUGIN_INSTALL_PREFIX}
    CACHE FILEPATH "Installation prefix for installers being built")

set(CPACK_PACKAGE_DIRECTORY
    "${PROJECT_BINARY_DIR}/packages"
    CACHE FILEPATH "Output path for built packages")

if(${CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT})
  set(CMAKE_INSTALL_PREFIX
      ${GLOBAL_PLUGIN_INSTALL_PREFIX}
      CACHE FILEPATH
            "Installation prefix when installing from a dev environment")
endif()

# UI Tweaks
# ---------

set(CPACK_PACKAGE_VENDOR
    "Pizeo"
    CACHE STRING "Name of the package vendor")
set(CPACK_PACKAGE_DESCRIPTION
    "Useful tools for audio production and MIDI manipulation"
    CACHE STRING "A description of the project, used in graphical installers")
set(CPACK_PACKAGE_CHECKSUM
    "SHA256"
    CACHE STRING "List of algorithms used to compute package checksums")
set(CPACK_RESOURCE_FILE_LICENSE
    "${PROJECT_SOURCE_DIR}/LICENSE.txt"
    CACHE FILEPATH "License shown in graphical installers")
set(CPACK_RESOURCE_FILE_README
    "${PROJECT_SOURCE_DIR}/packaging/DESCRIPTION.txt"
    CACHE FILEPATH "")

# Initialize CPack
# ##############################################################################
include(CPack)

# API for use with targets
# ##############################################################################

# piz_add_to_installer - Add a JUCE plugin to the installer
function(piz_add_to_installer target)
  get_target_property(friendly_name "${target}" JUCE_PRODUCT_NAME)
  get_target_property(formats "${target}" JUCE_FORMATS)

  foreach(format IN LISTS formats)
    install(
      TARGETS "${target}_${format}" #
      BUNDLE
      LIBRARY DESTINATION "${format}/${CPACK_PACKAGE_VENDOR}" #
              COMPONENT "${target}_${format}")

    # The installer structure will contain top-level nodes for each plugin
    # format and sub-nodes for the individual plugins under each format.
    cpack_add_component_group(
      "${format}"
      DISPLAY_NAME "${format}"
      EXPANDED BOLD_TITLE)
    cpack_add_component(
      "${target}_${format}"
      DISPLAY_NAME "${friendly_name}"
      GROUP "${format}")
  endforeach()
endfunction()
