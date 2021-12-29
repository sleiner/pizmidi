cmake_minimum_required(VERSION 3.22)

project(
  pizmidi
  VERSION 1.5.0
  HOMEPAGE_URL "https://github.com/sleiner/pizmidi"
  LANGUAGES C CXX)

if(NOT ${CMAKE_SIZEOF_VOID_P} EQUAL 8)
  message(WARNING "\
sizeof(void*) = ${CMAKE_SIZEOF_VOID_P} detected! \
You are trying to configure a non-64 bit build, which is not supported.")
endif()

include(cmake/installer.cmake)
include(cmake/dependencies.cmake)

add_subdirectory(pizjuce)
