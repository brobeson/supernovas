#ifndef SUPERNOVAS_JULIAN_CLOCK_HPP
#define SUPERNOVAS_JULIAN_CLOCK_HPP

#include <chrono>

namespace novas {
/// The amount of time into a Julian day
using julian_time = std::chrono::nanoseconds;

/// A number of Julian days relative to the epoch 4713 BCE
class julian_day {
public:
  /// The data type of the underlying day count.
  using day_type = std::int32_t;

  /**
   * \brief Construct a Julian day from a raw number of days.
   * \param[in] day The number days since start of the Julian epoch.
   */
  constexpr explicit julian_day(const day_type day = 0) : m_day{day} {}

  /// \return The number of days since the start of the Julian epoch.
  constexpr day_type day() const noexcept { return m_day; }

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
 * \brief Compare two Julian days for equality
 * \param[in] a,b Compare these two Julian days.
 * \retval true \a a.day() == \a b.day()
 * \retval false \a a.day() != \a b.day()
 */
constexpr bool operator==(const julian_day &a, const julian_day &b) {
  return a.day() == b.day();
}

/**
 * \brief Negate a Julian day
 * \param[in] jd Negate this Julian day.
 * \return Effectively, `julian_day{-jd.day()}`
 */
constexpr julian_day operator-(const julian_day &jd) {
  return julian_day{-jd.day()};
}

/**
 * \brief Add a number of days to a Julian day
 * \param[in] jd Add days to this Julian day.
 * \param[in] days Add these days to \a jd.
 * \return Effectively \a jd.day() + \a days.count().
 */
constexpr julian_day operator+(const julian_day &jd,
                               const std::chrono::days &days) {
  return julian_day(jd.day() + days.count());
}

/**
 * \brief Subtract a number of days from a Julian day
 * \param[in] jd Subtract days from this Julian day.
 * \param[in] days Subtract these days from \a jd.
 * \return Effectively \a jd.day() - \a days.count().
 */
constexpr julian_day operator-(const julian_day &jd,
                               const std::chrono::days &days) {
  return julian_day(jd.day() - days.count());
}

namespace literals {
/**
 * \brief Construct a literal Julian day.
 * \param[in] d The number of days since the start of the Julian epoch.
 * \return The Julian day, as if by `julian_day{d};`
 * \note To use this literal operator, you must include
 * `using namespace novas::literals;`.
 * \warning This operator quietly narrows if
 * `d > std::numeric_limits<julian_day::day_type>::max()`.
 * \code{.cpp}
 * using namespace novas::literals;
 * constexpr auto jd2000{2'451'544_jd};
 * \endcode
 */
constexpr julian_day operator""_jd(unsigned long long d) noexcept {
  return julian_day(d);
}
} // namespace literals
} // namespace novas

#endif
