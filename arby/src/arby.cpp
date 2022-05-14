/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <cstdint>

#include <sstream>
#include <string>

#include <arby/arby.hpp>


namespace com::saxbophone::arby {
    Uint::Uint(std::string digits) {}

    std::string Uint::_stringify_for_base() const {
        // XXX: only base-10 implemented for now
        // TODO: add a param to specify base when we implement other bases
        Uint value = *this;
        std::string digits;
        do {
            auto [quotient, remainder] = Uint::divmod(value, 10);
            if (remainder._digits.size() == 0) {
                digits = '0' + digits;
            } else {
                digits = std::to_string(remainder._digits.front()) + digits;
            }
            value = quotient;
        } while (value > 0);
        return digits;
    }

    std::ostream& operator<<(std::ostream& os, const Uint& object) {
        os << object._stringify_for_base();
        return os;
    }

    Uint::operator std::string() const {
        return this->_stringify_for_base();
    }
}
