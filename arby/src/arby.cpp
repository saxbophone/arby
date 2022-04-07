/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <cstdint>
#include <string>

#include <arby/arby.hpp>


namespace {
    // returns ceil(logₐ(n))
    constexpr uintmax_t fit(uintmax_t n, uintmax_t a) {
        uintmax_t remainder;
        uintmax_t exponent = 0;
        do {
            remainder = n / a;
            n = remainder;
            exponent++;
        } while (n > 0);
        return exponent;
    }
    // returns xⁿ
    constexpr uintmax_t exp(uintmax_t x, uintmax_t n) {
        if (n == 0) {
            return 1;
        } else {
            return x * exp(x, n - 1);
        }
    }
}

namespace com::saxbophone::arby {
    Uint::Uint() : Uint::Uint(0) {}

    Uint::Uint(uintmax_t value) : _digits(fit(value, Uint::BASE)) {
        uintmax_t power = exp(Uint::BASE, _digits.size());
        for (auto digit = _digits.rbegin(); digit != _digits.rend(); ++digit) {
            *digit = value / power;
            value %= power;
            power /= Uint::BASE;
        }
    }

    Uint::Uint(std::string digits) {}

    Uint::operator uintmax_t() const {
        uintmax_t accumulator = 0;
        uintmax_t current_radix = 1;
        for (auto digit : _digits) {
            accumulator += digit * current_radix;
            current_radix *= Uint::BASE;
        }
        return accumulator;
    }

    Uint::operator std::string() const {
        return {};
    }
}
