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

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

namespace com::saxbophone::arby {
    class Uint {
    private:
        /*
         * unsigned int is meant to be the "natural" unsigned type for the
         * system, so it seems like a good idea to use it as a base word to
         * make up the digits of our arbitrary-size integers
         */
        using StorageType = unsigned int;
        // XXX: remove this, it won't work on platforms where both uintmax_t and uint are same width!
        static constexpr uintmax_t BASE = (uintmax_t)std::numeric_limits<StorageType>::max() + 1u;
        std::vector<StorageType> _digits;
    public:
        Uint();
        Uint(uintmax_t value);
        Uint(std::string digits);
        bool operator==(const Uint& rhs) const = default;
        explicit operator uintmax_t() const;
        explicit operator std::string() const;
    };
}

// adding template specialisation to std::numeric_limits<> for arby::Uint
template <>
class std::numeric_limits<com::saxbophone::arby::Uint> {
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
    static com::saxbophone::arby::Uint min() { return 0; };
    static com::saxbophone::arby::Uint lowest() { return 0; };
    static com::saxbophone::arby::Uint max() { return 0; }; // N/A
    static com::saxbophone::arby::Uint epsilon() { return 0; } // N/A
    static com::saxbophone::arby::Uint round_error() { return 0; } // N/A
    static com::saxbophone::arby::Uint infinity() { return 0; } // N/A
    static com::saxbophone::arby::Uint quiet_NaN() { return 0; } // N/A
    static com::saxbophone::arby::Uint signaling_NaN() { return 0; } // N/A
    static com::saxbophone::arby::Uint denorm_min() { return 0; } // N/A
};

#endif // include guard
