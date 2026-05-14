#include "supernovas/julian_clock.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

using namespace novas::literals;
using namespace std::chrono_literals;

SCENARIO("julian_date meets type requirements", "[unit][julian_date]") {
  CHECK(std::is_constructible_v<novas::julian_date>);
  CHECK(std::is_default_constructible_v<novas::julian_date>);
  CHECK(std::is_trivially_move_assignable_v<novas::julian_date>);
  CHECK(std::is_trivially_move_constructible_v<novas::julian_date>);
  CHECK(std::is_trivially_copyable_v<novas::julian_date>);
  CHECK(std::is_trivially_copy_assignable_v<novas::julian_date>);
  CHECK(std::is_trivially_copy_constructible_v<novas::julian_date>);
  CHECK(std::is_standard_layout_v<novas::julian_date>);
}

SCENARIO("developers can construct julian_dates", "[unit][julian_date]") {
  WHEN("a julian_date is default constructed") {
    const novas::julian_date jd;
    THEN("the julian_date is 0.0") {
      CHECK(jd.day() == 0_jdn);
      CHECK(jd.time() == 0ns);
    }
  }
  GIVEN("a julian_day") {
    const auto day{12345_jdn};
    WHEN("when the julian_date is constructed") {
      const novas::julian_date actual{day};
      THEN("the julian_date is correct") {
        CHECK(actual.day() == day);
        CHECK(actual.time() == 0ns);
      }
    }
  }
  GIVEN("a julian_day and julian_time") {
    // clang-format off
    const auto [actual_day, actual_time, expected_day, expected_time]
    {
      GENERATE(table<novas::julian_day_number, novas::julian_time, novas::julian_day_number, novas::julian_time>(
        {{0_jdn,               0ns,  0_jdn,              0ns },
         {0_jdn,             -20ns, -1_jdn, 86399999999980ns },
         {0_jdn,  86400000000020ns,  1_jdn,             20ns },
         {0_jdn, 172800000000000ns,  2_jdn,              0ns }
        }))};
    // clang-format on
    WHEN("the julian_date is constructed") {
      const novas::julian_date actual{actual_day, actual_time};
      THEN("the julian_date is correct") {
        CHECK(actual.day() == expected_day);
        CHECK(actual.time() == expected_time);
      }
    }
  }
  GIVEN("a floating point Julian date") {
    const auto [date, expected_day, expected_time]{
        GENERATE(table<double, novas::julian_day_number, novas::julian_time>(
            {{0.0, 0_jdn, 0ns},
             {0.5, 0_jdn, 43200000000000ns},
             {-0.5, -1_jdn, 43200000000000ns},
             {-0.25, -1_jdn, 64800000000000ns}}))};
    WHEN("the julian_date is constructed") {
      const novas::julian_date actual{date};
      THEN("the Julian date is correct") {
        CHECK(actual.day() == expected_day);
        CHECK(actual.time() == expected_time);
      }
    }
  }
}

SCENARIO("developers can create julian_date literals", "[unit][julian_date]") {
  GIVEN("a Julian date literal") {
    using namespace std::chrono_literals;
    using namespace novas::literals;
    const auto [date, expected_day, expected_time]{GENERATE(
        table<novas::julian_date, novas::julian_day_number, novas::julian_time>(
            {{0.0_jd, 0_jdn, 0ns}, {0.5_jd, 0_jdn, 43200000000000ns}}))};
    THEN("the date is correct") {
      CHECK(date.day() == expected_day);
      CHECK(date.time() == expected_time);
    }
  }
}
  }
}
}