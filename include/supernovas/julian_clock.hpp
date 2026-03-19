#ifndef SUPERNOVAS_JULIAN_CLOCK_HPP
#define SUPERNOVAS_JULIAN_CLOCK_HPP

#include <chrono>

namespace novas {
/// The amount of time into a Julian day
using julian_time = std::chrono::nanoseconds;

namespace detail {
[[nodiscard]] auto fromJulianCalendar(const std::chrono::year_month_day &date) {
  using namespace std::chrono_literals;
  if (1582y / std::chrono::October / 4d < date ||
      date < -4717y / std::chrono::March / 1d) {
    throw std::invalid_argument{"is not a valid date"};
  }
  if (date.year() == 0y) {
    throw std::invalid_argument{"0 is not a valid year"};
  }
  const auto year{date.year() < 0y ? static_cast<int>(date.year()) + 1
                                   : static_cast<int>(date.year())};
  const auto month{static_cast<int>(static_cast<unsigned int>(date.month()))};
  const auto day{static_cast<int>(static_cast<unsigned int>(date.day()))};
  // https://en.wikipedia.org/wiki/Julian_day#Converting_Julian_calendar_date_to_Julian_day_number
  // Last retrieved 2026 March 27
  return (367 * year) - (7 * (year + 5001 + (month - 9) / 7) / 4) +
         (275 * month / 9) + day + 1'729'777;
}

[[nodiscard]] auto
fromGregorianCalendar(const std::chrono::year_month_day &date) {
  const auto year{static_cast<int>(date.year())};
  const auto month{static_cast<int>(static_cast<unsigned int>(date.month()))};
  const auto day{static_cast<int>(static_cast<unsigned int>(date.day()))};
  // https://en.wikipedia.org/wiki/Julian_day#Converting_Gregorian_calendar_date_to_Julian_day_number
  // Last retrieved 2026 March 27
  const auto month1{(month - 14) / 12};
  return (1461 * (year + 4800 + month1) / 4) +
         (367 * (month - 2 - 12 * month1) / 12) -
         (3 * ((year + 4900 + month1) / 100) / 4) + day - 32'075;
}

[[nodiscard]] constexpr auto
fromCalendar(const std::chrono::year_month_day &date) {
  using namespace std::chrono_literals;
  if (date < 1582y / std::chrono::October / 15d) {
    return fromJulianCalendar(date);
  }
  return fromGregorianCalendar(date);
}
} // namespace detail

/// A number of Julian days relative to the epoch 4713 BCE
class julian_day_number {
public:
  /// The data type of the underlying day count.
  using day_type = std::int32_t;

  /**
   * \brief Construct a Julian day from a raw number of days.
   * \param[in] day The number days since start of the Julian epoch.
   */
  constexpr explicit julian_day_number(const day_type day = 0) : m_day{day} {}

  /**
   * \brief Convert a calendar date to the Julian day
   * \param[in] date Convert this calendar date. The constructor automatically
   * handles Gregorian dates (1582 October 15 and later) and Julian dates
   * (before 1582 October 15).
   * \warning The following dates are not valid:
   * - Any date in year 0. Calendar dates go from 31 December 0001 BCE to
   *   1 January 0001 CE; there is no year 0.
   * - Dates between October 5-14 1582 (inclusive). These are the dates "lost"
   *   in the transition from the Julian calendar to the Gregorian calendar.
   * - Any date prior to 1 March 4717 BCE. The algorithm simply doesn't work.
   * - The upper limit is not known; I haven't tested to see how high it can go.
   */
  constexpr explicit julian_day_number(const std::chrono::year_month_day &date)
      : m_day{detail::fromCalendar(date)} {}

  /// \return The number of days since the start of the Julian epoch.
  constexpr day_type day() const noexcept { return m_day; }

  /**
   * \brief Compare two Julian days
   * \return The same result as comparing two integers.
   */
  constexpr bool operator==(const julian_day_number &) const noexcept = default;

  /// \copydoc operator==(const julian_day_number&)
  constexpr std::strong_ordering
  operator<=>(const julian_day_number &) const noexcept = default;

  /**
   * \brief Pre-increment this Julian day
   * \return A reference to this Julian day after incrementing.
   */
  constexpr auto &operator++() noexcept {
    ++m_day;
    return *this;
  }

  /**
   * \brief Post-increment this Julian day
   * \return A copy of this Julian before incrementing.
   */
  constexpr auto operator++(int) noexcept {
    auto original{*this};
    ++(*this);
    return original;
  }

  /**
   * \brief Pre-decrement this Julian day
   * \return A reference to this Julian day after decrementing.
   */
  constexpr auto &operator--() noexcept {
    --m_day;
    return *this;
  }

  /**
   * \brief Post-decrement this Julian day
   * \return A copy of this Julian before decrementing.
   */
  constexpr auto operator--(int) noexcept {
    auto original{*this};
    --(*this);
    return original;
  }

  /**
   * \brief Add a number of days to this Julian day
   * \param[in] d Add this number of days to the Julian day.
   * \return A reference to this Julian day after adding \a d days.
   */
  constexpr auto &operator+=(const std::chrono::days &d) {
    m_day += d.count();
    return *this;
  }

  /**
   * \brief Subtract a number of days from this Julian day
   * \param[in] d Subtract this number of days from the Julian day.
   * \return A reference to this Julian day after subtracting \a d days.
   */
  constexpr auto &operator-=(const std::chrono::days &d) {
    m_day -= d.count();
    return *this;
  }

private:
  day_type m_day;
};

/**
 * \brief Convert a Julian day number to the calendar date
 * \param[in] jdn Convert this Julian day number.
 * \return The calendar date of \a jdn.
 */
constexpr auto calendar_date(const julian_day_number &jdn) {
  // From Practical Astronomy with Your Calculator or Spreadsheet by Peter
  // Duffett-Smith and Jonathan Zwart (4th Edition)
  // cspell:ignore Duffett Zwart
  const auto day{jdn.day() + 0.5};
  const auto I{static_cast<int>(day)};
  const auto F{day - I};
  auto B{I};
  if (I > 2'299'160) {
    const auto A{static_cast<int>((I - 1867216.25) / 36524.25)};
    B = I + A - A / 4 + 1;
  }
  const auto C{B + 1524};
  const auto D{static_cast<int>((C - 122.1) / 365.25)};
  const auto E{static_cast<int>(365.25 * D)};
  const auto G{static_cast<int>((C - E) / 30.6001)};
  const auto d{C - E + F - static_cast<int>(30.6001 * G)};
  const auto m{G < 13.5 ? G - 1 : G - 13};
  const auto y{m > 2.5 ? D - 4716 : D - 4715};
  // Smith and Zwart has a bug for JDNs BCE. The math above calculates a year 0
  // which does not exist, so all non-positive years are off by 1.
  return std::chrono::year{y > 0 ? y : y - 1} / std::chrono::month(m) /
         std::chrono::day(d);
}

/**
 * \brief Negate a Julian day
 * \param[in] n Negate this Julian day.
 * \return Effectively, `julian_day_number{-n.day()}`
 */
constexpr julian_day_number operator-(const julian_day_number &n) {
  return julian_day_number{-n.day()};
}

/**
 * \brief Add a number of days to a Julian day
 * \param[in] n Add days to this Julian day.
 * \param[in] days Add these days to \a n.
 * \return Effectively \a n.day() + \a days.count().
 */
constexpr julian_day_number operator+(const julian_day_number &n,
                                      const std::chrono::days &days) {
  return julian_day_number(n.day() + days.count());
}

/**
 * \brief Subtract a number of days from a Julian day
 * \param[in] n Subtract days from this Julian day.
 * \param[in] days Subtract these days from \a n.
 * \return Effectively \a .day() - \a days.count().
 */
constexpr julian_day_number operator-(const julian_day_number &n,
                                      const std::chrono::days &days) {
  return julian_day_number(n.day() - days.count());
}

/**
 * \brief Insert a Julian day into an output stream
 * \param[in] s Insert the Julian day into this stream.
 * \param[in] d Insert this Julian day into a stream.
 * \return A reference to the stream \a s after insert \a d.
 */
inline std::ostream &operator<<(std::ostream &s, const julian_day_number d) {
  s << d.day();
  return s;
}

namespace literals {
/**
 * \brief Construct a literal Julian day.
 * \param[in] d The number of days since the start of the Julian epoch.
 * \return The Julian day, as if by `julian_day_number{d};`
 * \note To use this literal operator, you must include
 * `using namespace novas::literals;`.
 * \warning This operator quietly narrows if
 * `d > std::numeric_limits<julian_day_number::day_type>::max()`.
 * \code{.cpp}
 * using namespace novas::literals;
 * constexpr auto d{2'451'544_jdn};
 * \endcode
 */
constexpr julian_day_number operator""_jdn(unsigned long long d) noexcept {
  return julian_day_number(d);
}
} // namespace literals

namespace detail {
constexpr std::chrono::nanoseconds ns_per_day{86'400'000'000'000};

// Ensure the time is in the range [0, 1-day); adjust the day to account for
// any change in time.
constexpr void correctDayAndTime(julian_day_number &day, julian_time &time) {
  using namespace std::chrono_literals;
  if (time < 0ns) {
    time -= ns_per_day;
    const auto days{std::chrono::duration_cast<std::chrono::days>(time)};
    day += days;
    time = time - std::chrono::duration_cast<std::chrono::nanoseconds>(days) +
           ns_per_day;
  } else if (time >= ns_per_day) {
    const auto days{std::chrono::duration_cast<std::chrono::days>(time)};
    day += days;
    time -= std::chrono::duration_cast<std::chrono::nanoseconds>(days);
  }
}

constexpr auto day_fraction_to_duration(const long double date) {
  const auto fraction{date - static_cast<long long int>(date)};
  const auto nanos{ns_per_day.count() * fraction};
  return std::chrono::nanoseconds{
      static_cast<std::chrono::nanoseconds::rep>(nanos)};
}
} // namespace detail

class julian_date {
public:
  /**
   * \brief Construct a Julian date
   * \param[in] day Construct the Julian date for this Julian day number.
   * \param[in] time The time into the Julian date.
   * \details This constructor appropriately wraps \a time less than 0 and
   * \a time greater than or equal to one day. For example,
   * `julian_date d{0_jdn, -20ns}` results in `d.day() == -1_jdn` and
   * `d.time() == 1d - 2ns`.
   */
  constexpr explicit julian_date(
      const julian_day_number &day = julian_day_number{0},
      const julian_time &time = julian_time{0})
      : m_day{day}, m_time{time} {
    detail::correctDayAndTime(m_day, m_time);
  }

  /**
   * \copybrief julian_date(const julian_day_number&, const julian_time&)
   * \param[in] date The Julian date. The integer portion is the \a day(). The
   * fractional portion is the \a time().
   */
  constexpr explicit julian_date(const long double date)
      : julian_date{
            julian_day_number{static_cast<julian_day_number::day_type>(date)},
            detail::day_fraction_to_duration(date)} {}

  /// \return The Julian day number for this Julian date.
  [[nodiscard]] constexpr auto day() const noexcept { return m_day; }

  /// \return The time within the \a day().
  [[nodiscard]] constexpr auto time() const noexcept { return m_time; }

  /**
   * \brief Compare two Julian dates
   * \retval true The days and times are equal.
   * \retval false The days or times are not equal.
   */
  constexpr bool operator==(const julian_date &) const noexcept = default;

  /**
   * \brief Compare two Julian dates for ordering
   * \details Julian dates are ordered by Julian day first, and time if the
   * day numbers are equal.
   */
  constexpr std::strong_ordering
  operator<=>(const julian_date &) const noexcept = default;

  // template <typename Float> Float as() {
  //   static_assert(
  //       std::is_floating_point_v<Float>,
  //       "julian_date::as() can only convert to a floating point type");
  //   return static_cast<Float>(m_day) +
  //          static_cast<Float>(m_time.count()) / julian_time::period::den;
  // }

private:
  julian_day_number m_day;
  julian_time m_time;
};

std::ostream &operator<<(std::ostream &s, const julian_date &j) {
  s << j.day() << ' ' << j.time().count();
  return s;
}

constexpr auto operator-(const julian_date &j) {
  if (j.day().day() == 0) {
    if (j.time().count() == 0) {
      return j;
    }
    return julian_date{j.day(), -j.time()};
  }
  return julian_date{-j.day(), j.time()};
}

namespace literals {
/**
 * \brief Construct a literal Julian date.
 * \param[in] d The number of days since the start of the Julian epoch.
 * \return The Julian date.
 * \note To use this literal operator, you must include
 * `using namespace novas::literals;`.
 * \code{.cpp}
 * using namespace novas::literals;
 * constexpr auto d{2'451'544.0563_jd};
 * \endcode
 */
constexpr julian_date operator""_jd(long double date) noexcept {
  return julian_date{date};
}
} // namespace literals

#if 0
  /// Ratio of \f$86\,400 seconds : 1 day\f$
  using seconds_per_day = std::ratio<86'400, 1>;

  /// Ratio of \f$8.64 x 10^13 nanoseconds : 1 day\f$
  using nanoseconds_per_day =
    std::ratio<seconds_per_day::num * std::nano::den, 1>;

  class julian_clock
  {
  public:
    using rep = std::int64_t; //< Data type representing one clock tick
    using period = std::nano; //< One click tick corresponds to one nanosecond
    /// Represents a period of time between two time points
    using duration = std::chrono::duration<rep, period>;
    /// Represents a single point in time in the Julian calendar
    using time_point = std::chrono::time_point<julian_clock>;

    /// julian_clock may jump forward and backward
    static constexpr bool is_steady{false};

    /**
     * \brief Get the current Julian date
     * \return The current Julian date, measured in julian_clock::period
     * duration from the Julian calendar epoch.
     */
    [[nodiscard]] static time_point now() noexcept
    {
      return current_time;
    }

    /**
     * \brief Set the current Julian date from an arbitrary Gregorian date
     * \param[in] gregorian Set the Julian date this Gregorian date.
     * \return The new Julian date
     * \warning This function requires C++ 20.
     * \details
     * \code
     * // Default now is the Julian epoch: 0
     * const auto epoch{novas::julian_clock::now()};
     *
     * // Set the clock to 2000 January 1
     * const auto d{novas::julian_clock::set_current_time(2000/1/1)};
     * \endcode
     */
    [[nodiscard]] static constexpr auto to_julian_date(
      std::chrono::year_month_day gregorian) noexcept
    {
      using namespace std::chrono_literals;
      return time_point{0ns};
    }

    [[nodiscard]] static constexpr auto to_gregorian_date(
      const time_point julian) noexcept
    {
      using namespace std::chrono_literals;
      return 0y / std::chrono::January / 1d;
    }

  private:
    // https://godbolt.org/z/1895Ycanv
    static inline time_point current_time{duration{0}};
  };

  using julian_date = julian_clock::time_point;

  namespace literals
  {
    [[nodiscard]] constexpr auto operator""_jd(unsigned long long int j)
    {
      return julian_date{julian_clock::duration{j}};
    }
  } // namespace literals

  template <typename Integer>
  [[nodiscard]] constexpr auto operator==(julian_date p, Integer q)
  {
    return p.time_since_epoch().count() == q;
  }

  /// \copydoc operator==(const julian_date, const Integer)
  template <typename Integer>
  [[nodiscard]] constexpr auto operator==(const Integer q, const julian_date p)
  {
    return p == 1;
  }
#endif
} // namespace novas

namespace std {
template <>
constexpr novas::julian_day numeric_limits<novas::julian_day>::max() noexcept {
  return novas::julian_day{
      std::numeric_limits<novas::julian_day::day_type>::max()};
}

template <>
constexpr novas::julian_day numeric_limits<novas::julian_day>::min() noexcept {
  return novas::julian_day{
      std::numeric_limits<novas::julian_day::day_type>::min()};
}
} // namespace std
#endif
