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
  CHECK(-2'451'545_jd == novas::julian_day{-2'451'545});
}

SCENARIO("developers can retrieve the raw day number") {
  // There are no classes of input data for this. Run a few assertions to
  // ensure it's not hard-coded `return 0` or something like that.
  CHECK((0_jd).day() == 0);
  CHECK((1_jd).day() == 1);
  CHECK(novas::julian_day{-1}.day() == -1);
}

SCENARIO("developers can negate a julian_day", "[unit]") {
  GIVEN("a julian_day") {
    const auto [original, expected]{
        GENERATE(table<novas::julian_day, novas::julian_day::day_type>(
            {{0_jd, 0},
             {1_jd, -1},
             {novas::julian_day{-1}, 1},
             {2'147'483'647_jd, -2'147'483'647},
             {novas::julian_day{-2'147'483'648}, -2'147'483'648}}))};
    CAPTURE(original, expected);
    WHEN("the day is negated") {
      THEN("the new day is correct") { CHECK((-original).day() == expected); }
    }
  }
}

SCENARIO("developers can increment and decrement julian_days") {
  GIVEN("a julian_day") {
    const auto [start_day, expected_incremented, expected_decremented]{
        GENERATE(table<novas::julian_day, novas::julian_day, novas::julian_day>(
            {{0_jd, 1_jd, -1_jd},
             {-1_jd, 0_jd, -2_jd},
             {1_jd, 2_jd, 0_jd},
             {2147483647_jd, -2147483648_jd, 2147483646_jd},
             {-2147483648_jd, -2147483647_jd, 2147483647_jd}}))};
    CAPTURE(start_day);
    WHEN("the day is post-incremented") {
      auto actual_end_day{start_day};
      const auto actual_start_day{actual_end_day++};
      THEN("the new day is one more than the starting day") {
        CHECK(actual_end_day == expected_incremented);
      }
      THEN("the returned day is the same as the starting day") {
        CHECK(actual_start_day == start_day);
      }
    }
    WHEN("the day is pre-incremented") {
      auto actual_end_day{start_day};
      const auto actual_start_day{++actual_end_day};
      THEN("the new day is one more than the starting day") {
        CHECK(actual_end_day == expected_incremented);
      }
      THEN("the returned day is the same as the new day") {
        CHECK(actual_start_day == expected_incremented);
      }
    }
    WHEN("the day is post-decremented") {
      auto actual_end_day{start_day};
      const auto actual_start_day{actual_end_day--};
      THEN("the new day is one less than the starting day") {
        CHECK(actual_end_day == expected_decremented);
      }
      THEN("the returned day is the same as the starting day") {
        CHECK(actual_start_day == start_day);
      }
    }
    WHEN("the day is pre-decremented") {
      auto actual_end_day{start_day};
      const auto actual_start_day{--actual_end_day};
      THEN("the new day is one less than the starting day") {
        CHECK(actual_end_day == expected_decremented);
      }
      THEN("the returned day is the same as the new day") {
        CHECK(actual_start_day == expected_decremented);
      }
    }
  }
}

SCENARIO("developers can add and subtract days") {
  GIVEN("a julian_day") {
    auto [start_day, delta_days, expected_added, expected_subtracted]{GENERATE(
        table<novas::julian_day, std::chrono::days::rep, novas::julian_day,
              novas::julian_day>({{0_jd, 3, 3_jd, -3_jd}}))};
    CAPTURE(start_day, delta_days);
    WHEN("the delta is added with compound assignment") {
      start_day += std::chrono::days{delta_days};
      THEN("the julian_day matches") { CHECK(start_day == expected_added); }
    }
    WHEN("the delta is added") {
      const auto actual_day{start_day + std::chrono::days{delta_days}};
      THEN("the new julian_day is correct") {
        CHECK(actual_day == expected_added);
      }
    }
    WHEN("the delta is subtracted with compound assignment") {
      start_day -= std::chrono::days{delta_days};
      THEN("the julian_day matches") {
        CHECK(start_day == expected_subtracted);
      }
    }
    WHEN("the delta is subtracted") {
      const auto actual_day{start_day - std::chrono::days{delta_days}};
      THEN("the new julian_day is correct") {
        CHECK(actual_day == expected_subtracted);
      }
    }
  }
}

SCENARIO("developers can compare two julian_days") {
  // Let the compiler generate the comparison operators, but keep this test
  // in case changes to the class cause the compiler to generate unexpected
  // comparisons.
  GIVEN("two julian_day") {
    const auto [a, b, expected_equal, expected_unequal, expected_less,
                expected_less_equal, expected_greater, expected_greater_equal]{
        GENERATE(table<novas::julian_day, novas::julian_day, bool, bool, bool,
                       bool, bool, bool>(
            {{0_jd, 0_jd, true, false, false, true, false, true},
             {0_jd, 1_jd, false, true, true, true, false, false},
             {0_jd, -1_jd, false, true, false, false, true, true}}))};
    WHEN("the two days are compared for equality") {
      const auto actual{a == b};
      THEN("the result is correct") { CHECK(actual == expected_equal); }
    }
    WHEN("The two days are compared for inequality") {
      const auto actual{a != b};
      THEN("The result is correct") { CHECK(actual == expected_unequal); }
    }
    WHEN("The two days are compared for less-than") {
      const auto actual{a < b};
      THEN("The result is correct") { CHECK(actual == expected_less); }
    }
    WHEN("The two days are compared for less-than-or-equal") {
      const auto actual{a <= b};
      THEN("The result is correct") { CHECK(actual == expected_less_equal); }
    }
    WHEN("The two days are compared for greater-than") {
      const auto actual{a > b};
      THEN("The result is correct") { CHECK(actual == expected_greater); }
    }
    WHEN("The two days are compared for greater-than-or-equal") {
      const auto actual{a >= b};
      THEN("The result is correct") { CHECK(actual == expected_greater_equal); }
    }
  }
}