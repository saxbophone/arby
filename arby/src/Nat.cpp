/*
 * This is a sample source file corresponding to a public header file.
 *
 * <Copyright information goes here>
 */

#include <cstdint>

#include <algorithm>
#include <sstream>
#include <string>

#include <arby/Nat.hpp>


namespace com::saxbophone::arby {
    Nat::Nat(std::string digits)
        // use user-defined-literal to convert the digits in the string
      : _digits(operator "" _uarb(digits.c_str())._digits)
      {}

    std::string Nat::_stringify_for_base(std::uint8_t base) const {
        Nat value = *this;
        std::ostringstream digits;
        do {
            auto [quotient, remainder] = Nat::divmod(value, base);
            if (remainder == 0) {
                digits << '0';
            } else {
                // regardless of what base is requested, we can use hex for all
                // of them as we're only doing one digit at a time
                digits << std::hex << remainder._digits.front();
            }
            value = quotient;
        } while (value > 0);
        // output the digits in little-endian order, so we need to reverse them
        std::string output = digits.str();
        std::reverse(output.begin(), output.end());
        return output;
    }

    std::ostream& operator<<(std::ostream& os, const Nat& object) {
        // the implementation of std::dec, std::hex and std::oct guarantees that
        // only one of them will be set in the IO stream flags if the proper
        // stdlib function is used to set those flags
        // we test for hex, bin, then fallback to dec
        uint8_t base = 10;
        if (os.flags() & os.hex) {
            base = 16;
        } else if (os.flags() & os.oct) {
            base = 8;
        }
        os << object._stringify_for_base(base);
        return os;
    }

    Nat::operator std::string() const {
        return this->_stringify_for_base(10);
    }
}
