include(GoogleTest)

juce_add_console_app(piz_unit_tests)
gtest_discover_tests(piz_unit_tests TEST_PREFIX piz.unit.)
target_sources(
  piz_unit_tests
  PRIVATE # Headers
          "${CMAKE_CURRENT_LIST_DIR}/unit_tests.h"
          # Sources
          "${CMAKE_CURRENT_LIST_DIR}/baseline.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/channelize.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/unit_tests.cpp")

target_compile_definitions(piz_unit_tests PRIVATE PIZ_MULTIPLUGIN)
target_link_libraries(
  piz_unit_tests
  PRIVATE #
          gmock #
          gtest #
          piz_processors_without_gui #
)
