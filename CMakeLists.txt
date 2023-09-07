cmake_minimum_required(VERSION 3.22)
project(supernovas
  VERSION 0.0.0
  DESCRIPTION "C++ implementation of NOVAS"
  HOMEPAGE_URL "https://github.com/brobeson/supernovas"
  LANGUAGES CXX
)
list(APPEND CMAKE_MODULE_PATH "${supernovas_SOURCE_DIR}/cmake")
include(SupernovasOptions)

if(NOT (SUPERNOVAS_BUILD_TESTS AND BUILD_TESTING))
  enable_testing()
  add_subdirectory(tests)
endif()
