/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <cstdint>

#include <algorithm>
#include <sstream>
#include <string>

#include <arby/arby.hpp>


namespace com::saxbophone::arby {
    Uint::Uint(std::string digits)
        // use user-defined-literal to convert the digits in the string
      : _digits(operator "" _uarb(digits.c_str())._digits)
      {}

    std::string Uint::_stringify_for_base(std::uint8_t base) const {
        Uint value = *this;
        std::ostringstream digits;
        do {
            auto [quotient, remainder] = Uint::divmod(value, base);
            if (remainder._digits.size() == 0) {
                digits << '0';
            } else {
                digits << std::hex << remainder._digits.front();
            }
            value = quotient;
        } while (value > 0);
        std::string output = digits.str();
        std::reverse(output.begin(), output.end());
        return output;
    }

    std::ostream& operator<<(std::ostream& os, const Uint& object) {
        // the implementation of std::dec, std::hex and std::bin guarantees that
        // only one of them will be set in the IO stream flags if the proper
        // stdlib function is used to set those flags
        // we test for hex, bin, then fallback to dec
        uint8_t base = (os.flags() & os.hex) ? 16 : 10;
        os << object._stringify_for_base(base);
        return os;
    }

    Uint::operator std::string() const {
        return this->_stringify_for_base(10);
    }
}
