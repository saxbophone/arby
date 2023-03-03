/**
 * @file
 * @brief Interval<T> struct represents closed mathematical intervals
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

#ifndef COM_SAXBOPHONE_ARBY_INTERVAL_HPP
#define COM_SAXBOPHONE_ARBY_INTERVAL_HPP

#include <concepts> // convertible_to
#include <type_traits>
#include <utility> // pair

#include <cstddef> // size_t


namespace com::saxbophone::arby {
    /**
     * @brief Represents a closed mathematical interval of type T
     * @details AKA an interval over \f$[floor, ceil]\f$
     * @note Interval supports being used in structured bindings, i.e. the
     * following code is valid:
     * @code
     * auto [floor, ceil] = Interval();
     * @endcode
     */
    template <typename T>
    struct Interval {
        /**
         * @brief Default-initialises both floor and ceil
         */
        constexpr Interval() {}

        /**
         * @brief Initialises both floor and ceil to given value
         */
        constexpr Interval(T value) : Interval(value, value) {}

        /**
         * @brief Initialises both floor and ceil to separate values
         */
        constexpr Interval(T floor, T ceil) : floor(floor), ceil(ceil) {}

        /**
         * @brief Provides support for structured bindings
         * @note We only provide access as a copy
         */
        template <std::size_t N>
        constexpr typename std::tuple_element<N, Interval>::type get() const {
            if constexpr (N == 0) return floor;
            else if constexpr (N == 1) return ceil;
        }

        /**
         * @brief cast to other template of self
         * @pre `(U)T{}` must be well-defined
         */
        template <typename U> requires std::convertible_to<T, U>
        constexpr operator Interval<U>() const {
            return {(U)floor, (U)ceil};
        }

        T floor = {}; /**< The minimum bound of this Interval, inclusive */
        T ceil = {}; /**< The maximum bound of this Interval, inclusive */
    };

    // TODO: provide floor()/ceil() free function overloads?
}

/**
 * @note for structured binding support
 */
template <typename T>
struct std::tuple_size<com::saxbophone::arby::Interval<T>> : std::integral_constant<std::size_t, 2> {};

/**
 * @note for structured binding support
 */
template <std::size_t N, typename T>
struct std::tuple_element<N, com::saxbophone::arby::Interval<T>> {
    using type = T; /**< required for structured binding support */
};

#endif // include guard
