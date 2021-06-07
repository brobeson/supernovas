FetchContent_Declare(
  date
  GIT_REPOSITORY https://github.com/HowardHinnant/date.git
  GIT_TAG v3.0.1
)
FetchContent_MakeAvailable(date)

add_library(supernovas INTERFACE)
target_include_directories(
  supernovas
  INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)
target_compile_features(supernovas INTERFACE cxx_std_17)
target_link_libraries(supernovas INTERFACE date::date)
add_library(supernovas::supernovas ALIAS supernovas)

add_lizard_target(
  complexity_analysis
  ALL
  ARGUMENTS
    --CCN 10
    --length 30
    --arguments 5
    --warnings_only
    --working_threads 10
    --modified
)