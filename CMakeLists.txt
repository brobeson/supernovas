cmake_minimum_required(VERSION 3.31)
project(
  supernovas
  VERSION 0.0.0
  DESCRIPTION "A C++ implementation of the United States Naval Observatory NOVAS library"
  HOMEPAGE_URL https://github.com/brobeson/supernovas
  LANGUAGES CXX
)

option(
  SUPERNOVAS_BUILD_TESTS
  "Build the supernovas tests. Default: ${PROJECT_IS_TOP_LEVEL}. Values: {on, off}."
  ${PROJECT_IS_TOP_LEVEL}
)
option(
  SUPERNOVAS_BUILD_COVERAGE
  "Instrument the code to measure test coverage. Default: off. Values: {on, off}."
  on
)
if(SUPERNOVAS_BUILD_COVERAGE)
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/coverage")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
    add_link_options(-fprofile-instr-generate)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(--coverage)
    add_link_options(--coverage)
  endif()
endif()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/MP>")

enable_testing()
if(SUPERNOVAS_BUILD_TESTS)
  add_subdirectory(tests/supernovas)
endif()

# Configure the Cppcheck project file. This is benign for users who do not
# want to us Cppcheck, so don't control it with an option.
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/cppcheck")
configure_file(project.cppcheck.in project.cppcheck)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/coverage")