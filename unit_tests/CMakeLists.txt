find_package(Catch2 REQUIRED)
mark_as_advanced(Catch2_DIR)
add_executable(
  supernovas_unit_tests
  main.cpp
  julian_date_test.cpp
)
target_link_libraries(
  supernovas_unit_tests
  PRIVATE
    Catch2::Catch2
    supernovas::supernovas
)
target_compile_options(supernovas_unit_tests PRIVATE ${CMAKE_TOOLS_COMPILE_OPTIONS})
    
include(Catch)
catch_discover_tests(supernovas_unit_tests)

add_lizard_target(
  unit_test_analysis
  ALL
  ARGUMENTS
    --CCN 10
    --length 50
    --arguments 5
    --warnings_only
    --working_threads 10
    --modified
)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/coverage")
add_custom_target(
  coverage
  COMMAND
    gcovr
    --verbose
    --root "${CMAKE_CURRENT_SOURCE_DIR}"
    --html-details "${CMAKE_CURRENT_BINARY_DIR}/coverage/index.html"
    "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Calculating unit test coverage."
)
