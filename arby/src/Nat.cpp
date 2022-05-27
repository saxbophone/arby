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
        Nat digits_needed;
        if (not _digits.empty()) { // if > 0
            std::tie(digits_needed, std::ignore) = ilog(base, *this);
        }
        digits_needed += 1;
        Nat max_possible;
        std::tie(max_possible, std::ignore) = ilog(base, std::numeric_limits<uintmax_t>::max());
        max_possible -= 1; // the highest of these digits probably can't go all the way, knock one off to be safe
        std::ostringstream digits;
        if (digits_needed > max_possible) { // we can't just print through uintmax_t
            // use binary divide-and-conquer to recursively generate digit-chunks
            auto [front_digits, back_digits] = divmod(digits_needed, 2);
            back_digits += front_digits; // back is basically front+remainder
            // divide into two Nat instances for front and back, print recursively
            Nat p = pow(base, back_digits);
            std::cout << "divmod(";
            if (*this <= std::numeric_limits<uintmax_t>::max()) {
                std::cout << (uintmax_t)*this;
            } else {
                std::cout << "{";
                for (auto dig : _digits) {
                    std::cout << dig << ", ";
                }
                std::cout << "}";
            }
            std::cout << ", ";
            if (p <= std::numeric_limits<uintmax_t>::max()) {
                std::cout << (uintmax_t)p;
            }
            std::cout << ")" << std::endl;
            auto [front, back] = divmod(*this, p);
            // generate a string for both parts
            std::string front_str = front._stringify_for_base(base);
            std::string back_str = back._stringify_for_base(base);
            // pad either of them with leading zeroes if needed
            if (front_str.length() < front_digits) {
                digits << std::string((uintmax_t)front_digits - front_str.length(), '0');
            }
            digits << front_str;
            if (back_str.length() < back_digits) {
                digits << std::string((uintmax_t)back_digits - back_str.length(), '0');
            }
            digits << back_str;
        } else {
            digits << std::setfill('0') << std::setw((uintmax_t)digits_needed);
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
        }
        return digits.str();
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
