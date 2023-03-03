/**
 * @file
 * @brief DivisionResult<T> struct represents results of division with remainder
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

#ifndef COM_SAXBOPHONE_ARBY_DIVISION_HPP
#define COM_SAXBOPHONE_ARBY_DIVISION_HPP

#include <concepts> // convertible_to
#include <type_traits>
#include <utility> // pair

#include <cstddef> // size_t


namespace com::saxbophone::arby {
    /**
     * @brief Represents the result of Integer division with remainder
     * @note DivisionResult supports being used in structured bindings, i.e. the
     * following code is valid:
     * @code
     * auto [quotient, remainder] = DivisionResult();
     * @endcode
     * @warn You shouldn't create a DivisionResult<> with a remainder larger than its
     * quotient (this doesn't make any sense, at least not for positive number
     * division), but this isn't checked or enforced in any way by this struct.
     * Likewise, a DivisionResult with a zero quotient AND a zero remainder also makes
     * no sense (no division operation can produce this combination).
     */
    template <typename T>
    struct DivisionResult {
        /**
         * @brief Default-initialises both quotient and remainder
         */
        constexpr DivisionResult() {}

        /**
         * @brief Initialises both quotient and remainder to separate values
         */
        constexpr DivisionResult(T quotient, T remainder) : quotient(quotient), remainder(remainder) {}

        /**
         * @brief Provides support for structured bindings
         * @note We only provide access as a copy
         */
        template <std::size_t N>
        constexpr typename std::tuple_element<N, DivisionResult>::type get() const {
            if constexpr (N == 0) return quotient;
            else if constexpr (N == 1) return remainder;
        }

        /**
         * @brief cast to other template of self
         * @pre `(U)T{}` must be well-defined
         */
        template <typename U> requires std::convertible_to<T, U>
        constexpr operator DivisionResult<U>() const {
            return {(U)quotient, (U)remainder};
        }

        constexpr T floor() const {
            return quotient;
        }

        constexpr T ceil() const {
            return quotient + (remainder > 0); // ceil is one more than quotient if we have a non-zero remainder
        }

        T quotient = {}; /**< quotient, i.e. the main "result" part of the division */
        T remainder = {}; /**< remainder of division, i.e. if zero then the answer is a whole number */
    };

    // TODO: provide floor()/ceil() free function overloads?
}

/**
 * @note for structured binding support
 */
template <typename T>
struct std::tuple_size<com::saxbophone::arby::DivisionResult<T>> : std::integral_constant<std::size_t, 2> {};

/**
 * @note for structured binding support
 */
template <std::size_t N, typename T>
struct std::tuple_element<N, com::saxbophone::arby::DivisionResult<T>> {
    using type = T; /**< required for structured binding support */
};

#endif // include guard
