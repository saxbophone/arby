/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <iostream>

#include <arby/arby.hpp>

namespace com::saxbophone::arby {
    Uint::Uint() {}
    Uint::Uint(uintmax_t value) {
        std::cout << Uint::BASE << std::endl;
    }
    Uint::Uint(std::string digits) {}
    // bool Uint::operator==(const Uint& rhs) const = default;
}
