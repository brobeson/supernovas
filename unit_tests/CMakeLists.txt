find_package(Catch2 REQUIRED)
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