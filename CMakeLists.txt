cmake_minimum_required(VERSION 3.31)
project(
  supernovas
  VERSION 0.0.0
  DESCRIPTION "A C++ implementation of the United States Naval Observatory NOVAS library"
  HOMEPAGE_URL https://github.com/brobeson/supernovas
  LANGUAGES CXX
)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# add_subdirectory(include)

option(
  SUPERNOVAS_BUILD_TESTS
  "Build the supernovas tests. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
  ${PROJECT_IS_TOP_LEVEL}
)
if(SUPERNOVAS_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests/supernovas)
endif()
