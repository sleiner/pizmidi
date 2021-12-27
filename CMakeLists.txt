cmake_minimum_required(VERSION 3.22)

project(
  pizmidi
  VERSION 1.5.0
  LANGUAGES C CXX)

include(cmake/dependencies.cmake)

add_subdirectory(pizjuce)
