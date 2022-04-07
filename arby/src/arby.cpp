/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <cstdint>
#include <string>

#include <arby/arby.hpp>


namespace com::saxbophone::arby {
    Uint::Uint(std::string digits) {}

    Uint::operator std::string() const {
        return {};
    }
}
