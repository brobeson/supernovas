#ifndef SUPERNOVAS_JULIAN_CLOCK_HPP
#define SUPERNOVAS_JULIAN_CLOCK_HPP

#include <chrono>

namespace novas
{
  /// The amount of time into a Julian day
  using julian_time = std::chrono::nanoseconds;

  /// A number of Julian days relative to the epoch 4713 BCE
  class julian_day
  {
  public:
    using day_type = std::int32_t;

    constexpr explicit julian_day(const day_type day = 0) : m_day{day} {}

  private:
    day_type m_day;
  };
} // namespace novas

#endif
