include(FetchContent)

# Dependencies provide their libraries for installation. But for our own
# installers, we do not want a JUCE SDK to be included, we add the
# "EXCLUDE_FROM_ALL" parameter to the JUCE subdirectory being added. This has
# the notable effect that install commands are ignored.
function(piz_makeavailable name)
  message(STATUS "Fetching dependency: ${name}")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")

  FetchContent_Populate("${name}")
  add_subdirectory("${${name}_SOURCE_DIR}" "${${name}_BINARY_DIR}"
                   EXCLUDE_FROM_ALL)

  list(POP_BACK CMAKE_MESSAGE_INDENT)
endfunction(piz_makeavailable)

FetchContent_Declare(
  juce
  GIT_REPOSITORY https://github.com/sleiner/JUCE.git
  GIT_TAG 5a51ae15aa69a00d50b0a97d5e7a20e7acdfccab # 6.1.4 patched
)
piz_makeavailable(juce)

if(${PIZ_ENABLE_TESTS})
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG 3d81736c973fbcc938267cb296918bc3266dde12 # main as of 2022-01-21
  )
  piz_makeavailable(googletest)
endif()
