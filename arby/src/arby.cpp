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

    Uint::Uint(uintmax_t value) {}

    Uint::Uint(std::string digits) {}

    Uint::operator uintmax_t() const {
        return {};
    }

    Uint::operator std::string() const {
        return {};
    }
}
