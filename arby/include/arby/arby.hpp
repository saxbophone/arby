/**
 * @file
 * This is a sample public compilation unit.
 *
 * @author Your Name <your.email.address@goes.here>
 * @date Creation/Edit Date
 *
 * @copyright Copyright information goes here
 *
 * @copyright
 * Copyright information can span multiple paragraphs if needed, such as if you
 * use a well-known software license for which license header text (to be
 * placed in locations like these) are provided by the license custodians.
 *
 */

#ifndef COM_SAXBOPHONE_ARBY_ARBY_HPP
#define COM_SAXBOPHONE_ARBY_ARBY_HPP

#include <limits>

namespace com::saxbophone::arby {
    class Uint {
    public:
        constexpr Uint() {}
        constexpr Uint(int) {}
        constexpr bool operator==(const Uint& rhs) const = default;
    };
}

using namespace com::saxbophone;

// adding template specialisation to std::numeric_limits<> for arby::Uint
template <>
class std::numeric_limits<arby::Uint> {
public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false; // N/A
    static constexpr bool has_quiet_NaN = false; // N/A
    static constexpr bool has_signaling_NaN = false; // N/A
    static constexpr bool has_denorm = false; // N/A
    static constexpr bool has_denorm_loss = false; // N/A
    static constexpr std::float_round_style round_style = std::round_toward_zero;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_bounded = false;
    static constexpr bool is_modulo = false;
    static constexpr int digits = 0; // N/A
    static constexpr int digits10 = 0; // N/A
    static constexpr int max_digits10 = 0; // N/A
    static constexpr int radix = 2; // should be UINT_MAX+1 but won't fit in an int!
    static constexpr int min_exponent = 0; // N/A
    static constexpr int min_exponent10 = 0; // N/A
    static constexpr int max_exponent = 0; // N/A
    static constexpr int max_exponent10 = 0; // N/A
    static constexpr bool traps = true;
    static constexpr bool tinyness_before = false; // N/A
    // These methods can always be converted from constexpr to const if constexpr vector gives too much grief!
    static constexpr arby::Uint min() { return 0; };
    static constexpr arby::Uint lowest() { return 0; };
    static constexpr arby::Uint max() { return 0; }; // N/A
    static constexpr arby::Uint epsilon() { return 0; } // N/A
    static constexpr arby::Uint round_error() { return 0; } // N/A
    static constexpr arby::Uint infinity() { return 0; } // N/A
    static constexpr arby::Uint quiet_NaN() { return 0; } // N/A
    static constexpr arby::Uint signaling_NaN() { return 0; } // N/A
    static constexpr arby::Uint denorm_min() { return 0; } // N/A
};

#endif // include guard
