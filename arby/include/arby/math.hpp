/**
 * @file
 * @brief cmath-like math support functions
 * @note This file forms part of arby
 * @details arby is a C++ library providing arbitrary-precision integer types
 * @warning arby is alpha-quality software
 *
 * @author Joshua Saxby <joshua.a.saxby@gmail.com>
 * @date May 2022
 *
 * @copyright Copyright Joshua Saxby <joshua.a.saxby@gmail.com> 2022
 *
 * @copyright
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef COM_SAXBOPHONE_ARBY_MATH_HPP
#define COM_SAXBOPHONE_ARBY_MATH_HPP

#include <utility>

#include <arby/Nat.hpp>


namespace com::saxbophone::arby {
    /**
     * @returns base raised to the power of exponent
     * @param base,exponent parameters for the base and exponent
     * @todo This currently uses a divide-and-conquer approach that divides
     * exponent by 2 each time, for a binary-recursion on the order of
     * \f$\mathcal{O}(n\log{}n)\f$. This is fine, but it would be nice to
     * see if we can make incremental improvements to the optimisation by
     * using factors, logarithms or something else to divide the exponent
     * into more than 2 chunks at each level of recursion.
     * @relatedalso Nat
     */
    constexpr Nat pow(const Nat& base, const Nat& exponent) {
        // use divide-and-conquer recursion to break up huge powers into products of smaller powers
        // exponent = 0 is our base case to terminate the recursion
        if (exponent == 0) {
            return 1;
        } else if (exponent == 1) {
            // exponent = 1 is an additional base case mainly to prevent a redundant level of recursion to 0
            return base;
        } else if (exponent == 2) {
            // exponent = 2 is our final base case, as it seems a waste to leave it to the catch-all case below
            return base * base;
        }
        auto [quotient, remainder] = Nat::divmod(exponent, 2);
        // instead of calculating x^n, do x^(n/2)
        Nat power = pow(base, quotient);
        power *= power;
        // and multiply by base again if n was odd
        if (remainder == 1) {
            power *= base;
        }
        return power;
    }
    // returns pair of {ceil, floor} for integer logarithm of x, in given base
    constexpr std::pair<Nat, Nat> ilog(const Nat& base, const Nat& x) {
        return {};
    }
}

#endif // include guard
