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

#include <type_traits>
#include <utility> // pair

#include <cstddef> // size_t


namespace com::saxbophone::arby {
    template <typename T>
    class Interval {
    public:
        constexpr Interval() {}

        constexpr Interval(std::pair<T, T> p) : _bounds(p) {}

        constexpr Interval(T ceil, T floor) : Interval(std::pair{ceil, floor}) {}

        constexpr operator std::pair<T, T>() const {
            return _bounds;
        }

        // for structured binding support
        template <std::size_t N>
        decltype(auto) get() const {
            if constexpr (N == 0) return floor;
            else if constexpr (N == 1) return ceil;
        }

        T& floor = _bounds.first;
        T& ceil = _bounds.second;
    private:
        std::pair<T, T> _bounds;
    };

    // TODO: provide floor()/ceil() free function overloads?
}

// for structured binding support
template <typename T>
struct std::tuple_size<com::saxbophone::arby::Interval<T>> : std::integral_constant<std::size_t, 2> {};

template <std::size_t N, typename T>
struct std::tuple_element<N, com::saxbophone::arby::Interval<T>> {
    using type = T;
};

#endif // include guard
