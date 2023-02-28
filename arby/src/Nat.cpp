/*
 * This file forms part of arby
 * arby is a C++ library providing arbitrary-precision integer types
 * Warning: arby is alpha-quality software
 *
 * Created by Joshua Saxby <joshua.a.saxby@gmail.com>, May 2022
 *
 * Copyright Joshua Saxby <joshua.a.saxby@gmail.com> 2022
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstdint>

#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <tuple>         // tie

#include <arby/Nat.hpp>


namespace com::saxbophone::arby {
    Nat::Nat(std::string digits)
        // use user-defined-literal to convert the digits in the string
      : _digits(literals::operator "" _nat(digits.c_str())._digits)
      {}

    std::string Nat::_stringify_for_base(std::uint8_t base) const {
        // find out how many digits of the given base can be squeezed into uintmax_t
        Nat max_possible;
        std::tie(max_possible, std::ignore) = ilog(base, std::numeric_limits<uintmax_t>::max());
        // we will build up the string using digits of this base, for efficiency
        const Nat chunk = ipow(base, (uintmax_t)max_possible);
        Nat value = *this;
        std::string digits;
        // build the digits up backwards, least-significant-first up to the most
        do {
            std::ostringstream output;
            auto [quotient, remainder] = divmod(value, chunk);
            // only pad to width of chunk if this is not the front chunk
            if (quotient != 0) {
                output << std::setfill('0') << std::setw((uintmax_t)max_possible);
            }
            switch (base) {
            case 8:
                output << std::oct;
                break;
            case 16:
                output << std::hex;
                break;
            default:
                output << std::dec;
                break;
            }
            output << (uintmax_t)remainder;
            digits = output.str() + digits;
            value = quotient;
        } while (value > 0);
        return digits;
    }

    /**
     * @see std::ostream& Nat::operator<<(std::ostream& os, const Nat& object)
     */
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
