#include "supernovas/julian_clock.hpp"
#include <catch2/catch_test_macros.hpp>

SCENARIO("julian_day meets type requirements", "[unit]")
{
  CHECK(std::is_constructible_v<novas::julian_day>);
  CHECK(std::is_default_constructible_v<novas::julian_day>);
  CHECK(std::is_trivially_move_assignable_v<novas::julian_day>);
  CHECK(std::is_trivially_move_constructible_v<novas::julian_day>);
  CHECK(std::is_trivially_copyable_v<novas::julian_day>);
  CHECK(std::is_trivially_copy_assignable_v<novas::julian_day>);
  CHECK(std::is_trivially_copy_constructible_v<novas::julian_day>);
  CHECK(std::is_standard_layout_v<novas::julian_day>);
  CHECK(std::is_integral_v<novas::julian_day::day_type>);
}
