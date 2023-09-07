add_library(supernovas INTERFACE)
target_include_directories(supernovas INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}")
add_library(supernovas::supernovas ALIAS supernovas)