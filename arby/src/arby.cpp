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

    Uint::operator std::string() const {
        std::ostringstream os;
        os << std::dec << *this;
        return os.str();
    }

    std::ostream& operator<<(std::ostream& os, const Uint& object) {
        // this will store the leftover bits
        Uint value = object;
        // find the largest power of 10 that is just about not greater than value
        Uint power = 1;
        while (value > power * 10) {
            power *= 10;
        }
        // std::cout << (uintmax_t)power << std::endl;
        // now peel the digits off one by one
        while (power > 0) {
            auto [quotient, remainder] = Uint::divmod(value, power);
            // quotient should be in the range 0..9 and therefore only 1-digit
            if (quotient._digits.size() == 0) {
                os << '0';
            } else {
                os << std::to_string(quotient._digits.front());
            }
            value = remainder;
            power /= 10;
        }
        return os;
    }
}
