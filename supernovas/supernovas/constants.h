#ifndef SUPERNOVAS_CONSTANTS_H
#define SUPERNOVAS_CONSTANTS_H

namespace supernovas {
    /**
     * \brief Speed of light, measured in meters per second
     * \tparam Float A floating point type for the constant's precision.
    */
    template <typename Float>
    static constexpr Float c{299792458.0};
}
#endif
