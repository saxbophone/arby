/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <cstdint>
#include <string>

#include <arby/arby.hpp>

namespace com::saxbophone::arby {
    Uint::Uint() : Uint::Uint(0) {}

    Uint::Uint(uintmax_t value) {
        // cmath can probably do this quicker, but we'd have to use floating point! :S
        uintmax_t current_radix = 1;
        uintmax_t current_value = value;
        while (current_value > Uint::BASE) {
            current_radix *= Uint::BASE;
            current_value /= Uint::BASE;
            this->_digits.push_back(0);
        }
        // iterate backwards over digits
        for (auto it = this->_digits.rbegin(); it != this->_digits.rend(); ++it) {
            Uint::StorageType digit = current_radix / value;
            value -= digit * current_radix;
            *it = digit;
            current_radix /= Uint::BASE;
        }
    }

    Uint::Uint(std::string digits) {}

    Uint::operator uintmax_t() const {
        uintmax_t accumulator = 0;
        uintmax_t current_radix = 1;
        for (auto digit : this->_digits) {
            accumulator += digit * current_radix;
            current_radix *= Uint::BASE;
        }
        return accumulator;
    }

    Uint::operator std::string() const {
        return {};
    }
}
