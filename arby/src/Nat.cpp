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
#include <arby/math.hpp> // ilog


#include <iostream>

namespace com::saxbophone::arby {
    Nat::Nat(std::string digits)
        // use user-defined-literal to convert the digits in the string
      : _digits(literals::operator "" _nat(digits.c_str())._digits)
      {}

    std::string Nat::_stringify_for_base(std::uint8_t base) const {
        // work out how many base digits are needed to represent this, as well as the max we can get out of uintmax_t
        Nat digits_needed = 1;
        if (not _digits.empty()) { // if > 0
            std::tie(digits_needed, std::ignore) = ilog(base, *this);
        }
        Nat max_possible;
        std::tie(max_possible, std::ignore) = ilog(base, std::numeric_limits<uintmax_t>::max());
        std::ostringstream digits;
        if (digits_needed > max_possible) { // we can't just print through uintmax_t
            // XXX: implement binary recursion
            return "";
        } else {
            digits << std::setw((uintmax_t)digits_needed);
            switch (base) {
            case 8:
                digits << std::oct;
                break;
            case 16:
                digits << std::hex;
                break;
            default:
                digits << std::dec;
                break;
            }
            digits << (uintmax_t)*this;
            return digits.str();
        }
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
