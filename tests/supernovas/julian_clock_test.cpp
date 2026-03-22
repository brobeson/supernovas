#include "supernovas/julian_clock.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

using namespace novas::literals;

SCENARIO("julian_day meets type requirements", "[unit]") {
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

SCENARIO("developers can creat julian_day literals", "[unit]") {
  CHECK(0_jd == novas::julian_day{});
  CHECK(2'451'545_jd == novas::julian_day{2'451'545});
}

SCENARIO("developers can retrieve the raw day number") {
  // There are no classes of input data for this. Run a few assertions to
  // ensure it's not hard-coded `return 0` or something like that.
  CHECK((0_jd).day() == 0);
  CHECK((1_jd).day() == 1);
  CHECK(novas::julian_day{-1}.day() == -1);
}
