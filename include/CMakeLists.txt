add_library(supernovas.supernovas INTERFACE)
add_library(supernovas::supernovas ALIAS supernovas.supernovas)
target_compile_features(supernovas.supernovas INTERFACE cxx_std_20)
target_sources(
  supernovas.supernovas
  INTERFACE
  FILE_SET HEADERS
  FILES supernovas/julian_clock.hpp
)
