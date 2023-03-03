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

#include <utility> // pair


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

        T& floor = _bounds.first;
        T& ceil = _bounds.second;
    private:
        std::pair<T, T> _bounds;
    };

    // TODO: provide floor()/ceil() free function overloads?
}

#endif // include guard
