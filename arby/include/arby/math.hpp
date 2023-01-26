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

#include <stdexcept>
#include <utility>

#include <arby/Nat.hpp>


namespace com::saxbophone::arby {
    /**
     * @returns base raised to the power of exponent
     * i.e. for base as \f$b\f$ and exponent as \f$x\f$: \f$b^x\f$
     * @param base,exponent parameters for the base and exponent
     * @todo Work out time-complexity
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
    /**
     * @brief Calculates integer log of `x` in `base` as bounds of \f$log_b(x)\f$
     * @code
     * auto [low, high] = ilog(b, x);
     * @endcode
     * @param base base to use for \f$b\f$
     * @param x value to use for \f$x\f$
     * @pre \f$b\geq2\f$
     * @pre \f$x\geq1\f$
     * @throws std::domain_error when preconditions are violated
     * @returns pair of \f$(low, high)\f$ where for \f$\mathbb{R}\ v = log_b(x)\f$:
     * - \f$low = floor(v)\f$
     * - \f$high = ceil(v)\f$
     * @post \f$low \leq high\f$
     * @remarks When \f$low = high\f$:
     * - \f$log_b(x)\in\mathbb{N}\f$
     * @remarks Otherwise:
     * - \f$log_b(x)\in\mathbb{R}\f$
     * @note Complexity: @f$ \mathcal{O}(n^2log(n)) @f$
     * @relatedalso Nat
     */
    constexpr std::pair<Nat, Nat> ilog(const Nat& base, const Nat& x) {
        if (base < 2) {
            throw std::domain_error("ilog: base cannot be < 2");
        } else if (x < 1) {
            throw std::domain_error("ilog: x cannot be < 1");
        }
        if (x == 1) { return {0, 0}; }
        if (base == 2) {
            auto bit_length = x.bit_length();
            return {x.bit_length() - (pow(2, bit_length) > x), x.bit_length()};
        }
        // find the smallest power of base that is just >= than x
        Nat power = 1;
        Nat floor = 0;
        Nat exponent = 0;
        while (power < x) {
            power *= base;
            floor = exponent++; // increment and store old value in floor
        }
        // floor = ceil when power = x
        return {power == x ? exponent : floor, exponent};
    }
}

#endif // include guard
