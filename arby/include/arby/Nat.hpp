/**
 * @file
 * @brief Nat class supporting arbitrary-size unsigned (Natural) integers
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

#ifndef COM_SAXBOPHONE_ARBY_NAT_HPP
#define COM_SAXBOPHONE_ARBY_NAT_HPP

#include <cmath>
#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <compare>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include <codlili/list.hpp>

#include <arby/DivisionResult.hpp>
#include <arby/Interval.hpp>


/**
 * @brief Main namespace
 * @details Usage:
 * @code{.cpp}
 * using namespace com::saxbophone;
 * using namespace com::saxbophone::arby::literals;
 * @endcode
 * - Introduces library symbols into scope `arby`
 * - Introduces literal operators into global scope, allowing them to be used as literals
 * @details OR:
 * @code{.cpp}
 * using namespace com::saxbophone::arby;
 * @endcode
 * - Introduces all library symbols into global scope, including literals
 */
namespace com::saxbophone::arby {
    namespace PRIVATE {
        template <std::size_t BITS>
        struct GetTypeForSize {
            using Type = void;
        };
        template <>
        struct GetTypeForSize<8> {
            using Type = std::uint8_t;
        };
        template <>
        struct GetTypeForSize<16> {
            using Type = std::uint16_t;
        };
        template <>
        struct GetTypeForSize<32> {
            using Type = std::uint32_t;
        };
        template <>
        struct GetTypeForSize<64> {
            using Type = std::uint64_t;
        };
        template <typename T> requires (not std::numeric_limits<T>::is_signed)
        struct GetNextBiggerType {
            using Type = typename GetTypeForSize<std::numeric_limits<T>::digits * 2>::Type;
        };
        template <typename T> requires (not std::numeric_limits<T>::is_signed)
        struct GetNextSmallerType {
            using Type = typename GetTypeForSize<std::numeric_limits<T>::digits / 2>::Type;
        };

        /*
         * uses compile-time template logic to pick StorageType and OverflowType:
         * - picks unsigned int if its range is less than that of uintmax_t
         * - otherwise, picks the next type smaller than unsigned int/uintmax_t (very unlikely)
         */
        struct StorageTraits {
            using StorageType = std::conditional<
                (std::numeric_limits<unsigned int>::digits < std::numeric_limits<uintmax_t>::digits),
                unsigned int,
                GetNextSmallerType<unsigned int>::Type
            >::type;
            using OverflowType = GetNextBiggerType<StorageType>::Type;
        };

        // returns ceil(logₐ(n))
        constexpr std::size_t fit(uintmax_t n, uintmax_t a) {
            std::size_t remainder;
            std::size_t exponent = 0;
            do {
                remainder = n / a;
                n = remainder;
                exponent++;
            } while (n > 0);
            return exponent;
        }
        // returns xⁿ
        constexpr uintmax_t exp(uintmax_t x, uintmax_t n) {
            if (n == 0) {
                return 1;
            } else {
                return x * exp(x, n - 1);
            }
        }
    }
    // end of PRIVATE

    /**
     * @brief Arbitrary-precision unsigned integer type
     * @details This is named after \f$\mathbb{N}\f$, the set of Natural numbers,
     * which this type models
     * @note `std::numeric_limits<Nat>` is specialised such that most of the
     * members of that type are implemented to describe the traits of this type.
     * @note Exceptions include any members of std::numeric_limits<> which
     * describe a finite number of digits or a maximmum value, neither of which
     * apply to this type as it is unbounded.
     * @exception std::logic_error may be thrown from most methods when the
     * result of an operation leaves a Nat object with leading zero digits in
     * its internal representation. Such cases are the result of bugs in this
     * code and should be reported as such.
     */
    class Nat {
    public:
        /**
         * @brief The type used to store the digits of this Nat object
         * @note The exact native type used for this is platform-specific:
         * - Normally, it is the same as `unsigned int`
         * - However, in the unlikely event that `unsigned int` is not smaller
         * than `uintmax_t`, we pick the next smaller type (typically `unsigned short`)
         */
        using StorageType = PRIVATE::StorageTraits::StorageType;
        /**
         * @brief This is the smallest type guaranteed to be able to store the
         * result of any product or sum of two values of the type used to store
         * the number's digits.
         * @note Consequently, this is the type used to represent Nat::BASE as
         * that value is +1 beyond the upper bound for the type used to store
         * the digits.
         */
        using OverflowType = PRIVATE::StorageTraits::OverflowType;
    private:
        static constexpr std::size_t BITS_PER_DIGIT = std::numeric_limits<StorageType>::digits;
        static constexpr std::size_t BITS_BETWEEN = std::numeric_limits<OverflowType>::digits - std::numeric_limits<StorageType>::digits;
        // validates the digits array
        constexpr void _validate_digits() const {
            #ifndef NDEBUG // only run checks in debug mode
            if (_digits.empty()) {
                throw std::logic_error("no digits in internal representation");
            }
            if (_digits.size() > 1 and _digits.front() == 0) {
                throw std::logic_error("leading zeroes in internal representation");
            }
            #endif
        }
        // removes leading zeroes from the digits array
        constexpr void _remove_leading_zeroes() {
            while (_digits.size() > 1 and _digits.front() == 0) {
                _digits.pop_front();
            }
        }
    public:
        /**
         * @brief The number base used internally to store the value
         * @details This is the radix that the digits are encoded in
         */
        static constexpr OverflowType BASE = (OverflowType)std::numeric_limits<StorageType>::max() + 1;
        /**
         * @brief Defaulted equality operator for Nat objects
         * @param rhs other Nat object to compare against
         * @returns `true` if objects are equal, otherwise `false`
         * @note Worst-case complexity: @f$ \mathcal{O(n)} @f$
         */
        constexpr bool operator==(const Nat& rhs) const = default;
        /**
         * @brief three-way-comparison operator defines all relational operators
         * @param rhs other Nat object to compare against
         * @returns std::strong_ordering object for comparison
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        constexpr auto operator<=>(const Nat& rhs) const {
            // use size to indicate ordering if they differ
            if (_digits.size() != rhs._digits.size()) {
                return _digits.size() <=> rhs._digits.size();
            }
            // otherwise compare the elements until a mismatch is found
            auto it = _digits.begin();
            auto rhs_it = rhs._digits.begin();
            for (; it != _digits.end(); it++, rhs_it++) {
                if (*it != *rhs_it) {
                    return *it <=> *rhs_it;
                }
            }
            return std::strong_ordering::equal;
        }
        /**
         * @brief Default constructor, initialises to numeric value `0`
         */
        constexpr Nat() : _digits{0} {
            _validate_digits();
        }
        /**
         * @brief Integer-constructor, initialises with the given integer value
         * @param value value to initialise with
         */
        constexpr Nat(uintmax_t value) : _digits(PRIVATE::fit(value, Nat::BASE)) {
            // fill out digits in big-endian order
            uintmax_t power = PRIVATE::exp(Nat::BASE, _digits.size() - 1);
            for (auto& digit : _digits) {
                digit = (StorageType)(value / power);
                value %= power;
                power /= Nat::BASE;
            }
            _validate_digits();
        }
        /**
         * @brief Digits-constructor, initialises Nat using the given digits
         * @tparam Container A container type exposing an STL-like API that
         * provides begin(), end(), and empty() at a minimum
         * @param digits the digits to initialise the Nat object from, these
         * should be encoded in base Nat::BASE (this corresponds to max
         * StorageType value)
         * @pre `digits` is not empty
         * @throws std::invalid_argument when `digits` is empty
         */
        template <template<typename...> class Container, typename... Ts>
        constexpr Nat(const Container<StorageType, Ts...>& digits) {
            if (std::empty(digits)) {
                throw std::invalid_argument("cannot construct Nat object with empty digits sequence");
            }
            for (const auto& digit : digits) {
                _digits.push_back(digit);
            }
            _remove_leading_zeroes();
        }
        /**
         * @overload
         * @remarks Overload for constructing from `codlili::list` of digits
         */
        constexpr Nat(const codlili::list<StorageType>& digits) : _digits(digits) {
            if (std::empty(digits)) {
                throw std::invalid_argument("cannot construct Nat object with empty digits sequence");
            }
            _remove_leading_zeroes();
        }
        /**
         * @overload
         * @remarks Overload for constructing from `std::initializer_list` of digits
         */
        constexpr Nat(std::initializer_list<StorageType> digits) : _digits(digits) {
            if (std::empty(digits)) {
                throw std::invalid_argument("cannot construct Nat object with empty digits sequence");
            }
            _remove_leading_zeroes();
        }
        /**
         * @brief Constructor-like static method, creates Nat from floating point value
         * @returns Nat with the value of the given float, with the fractional part truncated off
         * @param value Positive floating point value to initialise with
         * @throws std::domain_error when `value < 0` or when `value` is not a
         * finite number.
         */
        static Nat from_float(long double value) {
            // prevent initialising from negative values
            if (value < 0) {
                throw std::domain_error("Nat cannot be negative");
            }
            // prevent initialising from ±inf or NaN
            if (not std::isfinite(value)) {
                throw std::domain_error("Nat cannot be Infinite or NaN");
            }
            Nat output;
            if (value < 1) { return output; } // output is already zero
            while (value > 0) {
                StorageType digit = (StorageType)std::fmod(value, Nat::BASE);
                output._digits.push_front(digit);
                value /= Nat::BASE;
                // truncate the fractional part of the floating-point value
                value = std::trunc(value);
            }
            output._digits.pop_back(); // first digit is zero-placeholder, now overwritten
            output._validate_digits();
            return output;
        }
        /**
         * @brief String-constructor, initialises from string decimal value
         * @param digits string containing the digits of the value to initialise
         * with, written in decimal
         */
        Nat(std::string digits);
    private:
        // private helper method to abstract the common part of the casting op
        template <typename T>
        constexpr T _cast_to() const {
            T accumulator = 0;
            // read digits out in big-endian order, shifting as we go
            for (auto digit : _digits) {
                accumulator *= Nat::BASE;
                accumulator += digit;
            }
            return accumulator;
        }
    public:
        /**
         * @returns Value of this Nat object cast to uintmax_t
         * @throws std::range_error when Nat value is out of range for
         * uintmax_t
         */
        explicit constexpr operator uintmax_t() const {
            // prevent overflow of uintmax_t
            if (*this > std::numeric_limits<uintmax_t>::max()) {
                throw std::range_error("value too large for uintmax_t");
            }
            return this->_cast_to<uintmax_t>();
        }
        /**
         * @returns Value of this Nat object cast to long double
         */
        explicit constexpr operator long double() const {
            return this->_cast_to<long double>();
        }
        /**
         * @returns Value of this Nat object cast to float
         */
        explicit constexpr operator float() const {
            return this->_cast_to<float>();
        }
        /**
         * @returns Value of this Nat object cast to double
         */
        explicit constexpr operator double() const {
            return this->_cast_to<double>();
        }
        /**
         * @returns Value of this Nat object cast to any numeric type
         * @tparam To The data type to cast to
         */
        template <typename To>
        explicit constexpr operator To() const {
            // prevent overflow of To if it's a bounded type
            if constexpr (std::numeric_limits<To>::is_bounded) {
                if (*this > std::numeric_limits<To>::max()) {
                    throw std::range_error("value too large for destination type");
                }
            }
            // take a short-cut if destination type is bounded and is not bigger than largest digit value
            if constexpr (std::numeric_limits<To>::is_bounded and std::numeric_limits<To>::max() <= (BASE - 1)) {
                // at this point, out-of-bounds has already been checked. Just return last digit
                return (To)_digits.back();
            } else {
                return this->_cast_to<To>();
            }
        }
        /**
         * @brief custom ostream operator that allows class Nat to be printed
         * with std::cout and friends
         * @note Complexity is @f$ \mathcal{O(terrible)} @f$
         */
        friend std::ostream& operator<<(std::ostream& os, const Nat& object);
        /**
         * @returns string representing the value of this Nat, in decimal
         */
        explicit operator std::string() const;
        /**
         * @brief prefix increment
         * @returns new value of Nat object after incrementing
         * @note Best-case complexity: @f$ \mathcal{O(1)} @f$
         * @note Worst-case complexity: @f$ \mathcal{O(n)} @f$
         */
        constexpr Nat& operator++() {
            // increment least significant digit then rollover remaining digits as needed
            for (auto it = _digits.rbegin(); it != _digits.rend(); ++it) {
                // only contine to next digit if incrementing this one rolls over
                if (++(*it) != 0) {
                    break;
                }
            }
            // if last digit is zero, we need another one
            if (_digits.front() == 0) {
                _digits.push_front(1);
            }
            _validate_digits();
            return *this; // return new value by reference
        }
        /**
         * @brief postfix increment
         * @returns old value of Nat object before incrementing
         * @note Best-case complexity: @f$ \mathcal{O(1)} @f$
         * @note Worst-case complexity: @f$ \mathcal{O(n)} @f$
         */
        constexpr Nat operator++(int) {
            Nat old = *this; // copy old value
            operator++();  // prefix increment
            return old;    // return old value
        }
        /**
         * @brief prefix decrement
         * @returns new value of Nat object after decrementing
         * @throws std::underflow_error when value of Nat is `0`
         * @note Best-case complexity: @f$ \mathcal{O(1)} @f$
         * @note Worst-case complexity: @f$ \mathcal{O(n)} @f$
         */
        constexpr Nat& operator--() {
            if (_digits.front() == 0) { // front = 0 means value is zero since no leading zeroes allowed
                throw std::underflow_error("arithmetic underflow: can't decrement unsigned zero");
            } else {
                // decrement least significant digit then borrow from remaining digits as needed
                for (auto it = _digits.rbegin(); it != _digits.rend(); ++it) {
                    // only continue to next digit if decrementing this one rolls over
                    if (--(*it) != std::numeric_limits<StorageType>::max()) {
                        break;
                    }
                }
                // remove leading zeroes
                if (_digits.size() > 1 and _digits.front() == 0) {
                    _digits.pop_front();
                }
            }
            _validate_digits();
            return *this; // return new value by reference
        }
        /**
         * @brief postfix decrement
         * @returns old value of Nat object before decrementing
         * @throws std::underflow_error when value of Nat is `0`
         * @note Best-case complexity: @f$ \mathcal{O(1)} @f$
         * @note Worst-case complexity: @f$ \mathcal{O(n)} @f$
         */
        constexpr Nat operator--(int) {
            Nat old = *this; // copy old value
            operator--();  // prefix decrement
            return old;    // return old value
        }
        /**
         * @brief addition-assignment
         * @details Adds other value to this Nat and assigns the result to self
         * @param rhs value to add to this Nat
         * @returns resulting object after addition-assignment
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        constexpr Nat& operator+=(Nat rhs) {
            // both args being zero is a no-op, guard against this
            if (not (_digits.front() == 0 and rhs._digits.front() == 0)) {
                // make sure this and rhs are the same size, fill with leading zeroes if needed
                if (rhs._digits.size() > _digits.size()) {
                    _digits.push_front(rhs._digits.size() - _digits.size(), 0);
                } else if (_digits.size() > rhs._digits.size()) {
                    rhs._digits.push_front(_digits.size() - rhs._digits.size(), 0);
                }
                // work backwards up the digits vector of the rhs
                StorageType carry = 0; // carries are stored here on overflow
                auto rhs_it = rhs._digits.rbegin();
                for (auto it = _digits.rbegin(); it != _digits.rend(); it++, rhs_it++) {
                    OverflowType addition = (OverflowType)*it + *rhs_it + carry;
                    // downcast to chop off any more significant bits
                    // (effectively cheap modulo because we know OverflowType is twice the width of StorageType)
                    *it = (StorageType)addition;
                    // update the carry with the value in the top significant bits
                    carry = (StorageType)(addition >> BITS_BETWEEN);
                }
                // if carry is non-zero, then add it to the next most significant digit, expanding size of this if needed
                if (carry != 0) {
                    _digits.push_front(carry);
                }
            }
            _validate_digits();
            return *this; // return the result by reference
        }
        /**
         * @brief Addition operator for Nat
         * @param lhs,rhs operands for the addition
         * @returns sum of lhs + rhs
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        friend constexpr Nat operator+(Nat lhs, const Nat& rhs) {
            lhs += rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }
        /**
         * @brief subtraction-assignment
         * @details Subtracts other value from this Nat and assigns the result to self
         * @param rhs value to subtract from this Nat
         * @returns resulting object after subtraction-assignment
         * @throws std::underflow_error when rhs is bigger than this
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        constexpr Nat& operator-=(Nat rhs) {
            // TODO: detect underflow early?
            // rhs being a zero is a no-op, guard against this
            if (rhs._digits.front() != 0) {
                // make sure this and rhs are the same size, fill with leading zeroes if needed
                if (rhs._digits.size() > _digits.size()) {
                    _digits.push_front(rhs._digits.size() - _digits.size(), 0);
                } else if (_digits.size() > rhs._digits.size()) {
                    rhs._digits.push_front(_digits.size() - rhs._digits.size(), 0);
                }
                // work backwards up the digits vector of the rhs
                bool borrow = false; // transfers borrows up when triggered
                auto rhs_it = rhs._digits.rbegin();
                for (auto it = _digits.rbegin(); it != _digits.rend(); it++, rhs_it++) {
                    // this will underflow correctly in a way that means we can get the remainder off the bottom bits
                    OverflowType subtraction = (OverflowType)*it - *rhs_it - borrow;
                    // downcast to chop off any more significant bits
                    // (effectively cheap modulo because we know OverflowType is twice the width of StorageType)
                    *it = (StorageType)subtraction;
                    // detect any borrow that is needed
                    borrow = subtraction > std::numeric_limits<StorageType>::max();
                }
                // if borrow is non-zero, then an arithmetic underflow occurred
                if (borrow) {
                    throw std::underflow_error("arithmetic underflow: subtrahend bigger than minuend");
                }
            }
            // remove any leading zeroes
            _remove_leading_zeroes();
            _validate_digits();
            return *this; // return the result by reference
        }
        /**
         * @brief Subtraction operator for Nat
         * @param lhs,rhs operands for the subtraction
         * @returns result of lhs - rhs
         * @throws std::underflow_error when rhs is bigger than lhs
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        friend constexpr Nat operator-(Nat lhs, const Nat& rhs) {
            lhs -= rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }
        /**
         * @brief multiplication-assignment
         * @details Multiplies this Nat by other value and assigns the result to self
         * @param rhs value to multiply this Nat by
         * @returns resulting object after multiplication-assignment
         * @note Complexity: @f$ \mathcal{O(n^2)} @f$
         */
        constexpr Nat& operator*=(const Nat& rhs) {
            Nat product = *this * rhs; // uses friend *operator
            // assign product's digits back to our digits
            _digits = product._digits;
            return *this; // return the result by reference
        }
    private: // private helper methods for multiplication operator
        constexpr bool is_power_of_2() const {
            // TODO: optimise this --check leading digit is power of 2 and trailing digits are all zero
            return *this == Nat(1) << (bit_length() - 1);
        }
    public:
        /**
         * @brief Multiplication operator for Nat
         * @param lhs,rhs operands for the multiplication
         * @returns product of lhs * rhs
         * @note Complexity: @f$ \mathcal{O(n^2)} @f$
         */
        friend constexpr Nat operator*(const Nat& lhs, const Nat& rhs) {
            // init product to zero
            Nat product;
            // either operand being zero always results in zero, so only run the algorithm if they're both non-zero
            if (lhs._digits.front() == 0 or rhs._digits.front() == 0) {
                return product;
            }
            // optimisation using bitshifting when multiplying by binary powers
            if (rhs.is_power_of_2()) { return lhs << (rhs.bit_length() - 1); }
            if (lhs.is_power_of_2()) { return rhs * lhs; }
            // multiply each digit from lhs with each digit from rhs
            std::size_t l = 0; // manual indices to track which digit we are on,
            std::size_t r = 0; // as codlili's iterators are not random-access
            for (auto lhs_digit : lhs._digits) {
                // reset r index as it cycles through multiple times
                r = 0;
                for (auto rhs_digit : rhs._digits) {
                    // cast lhs to OverflowType to make sure both operands get promoted to avoid wrap-around overflow
                    OverflowType multiplication = (OverflowType)lhs_digit * rhs_digit;
                    // create a new Nat with this intermediate result and add trailing places as needed
                    Nat intermediate = multiplication;
                    // we need to remap the indices as the digits are stored big-endian
                    std::size_t shift_amount = (lhs._digits.size() - 1 - l) + (rhs._digits.size() - 1 - r);
                    // add that many trailing zeroes to intermediate's digits
                    intermediate._digits.push_back(shift_amount, 0);
                    // finally, add it to lhs as an accumulator
                    product += intermediate;
                    // increment manual indices
                    r++;
                }
                l++;
            }
            product._validate_digits();
            return product;
        }
    private: // private helper methods for divmod() TODO: move to anonymous namespace near definition of divmod()
        // function that shifts up rhs to be just big enough to be smaller than lhs
        // TODO: rewrite this to use bit-shifting for speed
        static constexpr Nat get_max_shift(const Nat& lhs, const Nat& rhs) {
            // how many places can we shift rhs left until it's the same width as lhs?
            std::size_t wiggle_room = lhs._digits.size() - rhs._digits.size();
            // provisionally perform the shift up
            Nat shift = 1;
            shift._digits.push_back(wiggle_room, 0);
            // drag back down wiggle_room while shifted rhs > lhs
            while (rhs * shift > lhs) {
                shift._digits.pop_back();
            }
            return shift;
        }
        // uses leading 1..2 digits of lhs and leading digits of rhs to estimate how many times it goes in
        static constexpr OverflowType estimate_division(const Nat& lhs, const Nat& rhs) {
            OverflowType denominator = (OverflowType)rhs._digits.front();
            // if any of the other digits of rhs are non-zero...
            if (std::any_of(++rhs._digits.begin(), rhs._digits.end(), [](StorageType digit){ return digit != 0; })) {
                // increment denominator, we don't know what those other digits are so we have to assume denominator
                // is closer in value to denominator+1 and estimate accordingly, by deliberately underestimating...
                denominator++;
            }
            if (lhs._digits.front() >= denominator) { // use lhs[0] / rhs[0] only
                return (OverflowType)lhs._digits.front() / denominator;
            } else { // use lhs[0..1] / rhs[0]
                // chop off all but the leading two digits of lhs to get the numerator
                Nat leading_digits = lhs;
                // NOTE: we can guarantee that lhs will not be shorter than 2 digits here ONLY because the caller will
                // not call this method if lhs < rhs AND to get to this branch, the first digit of lhs is less than that
                // of rhs. These facts taken together prove that lhs is at least 2 digits long at this point.
                leading_digits._digits.resize(2);
                OverflowType numerator = (OverflowType)(uintmax_t)leading_digits;
                return (numerator / denominator);
            }
        }
    public:
        /**
         * @brief division and modulo all-in-one, equivalent to C/C++ div() and Python divmod()
         * @param lhs,rhs operands for the division/modulo operation
         * @returns DivisionResult of {quotient, remainder}
         * @throws std::domain_error when rhs is zero
         * @todo Work out time-complexity
         */
        friend constexpr DivisionResult<Nat> divmod(const Nat& lhs, const Nat& rhs);
        /**
         * @brief division-assignment
         * @details Divides this Nat by other value and stores result to this
         * @note This implements floor-division, returning the quotient only
         * @param rhs value to divide this Nat by
         * @returns resulting object after division-assignment
         * @throws std::domain_error when rhs is zero
         * @todo Work out time-complexity
         */
        constexpr Nat& operator/=(const Nat& rhs) {
            Nat quotient = *this / rhs; // uses friend /operator
            // assign quotient's digits back to our digits
            _digits = quotient._digits;
            return *this; // return the result by reference
        }
        /**
         * @brief Division operator for Nat
         * @note This implements floor-division, returning the quotient only
         * @param lhs,rhs operands for the division
         * @returns quotient of lhs / rhs
         * @todo Work out time-complexity
         */
        friend constexpr Nat operator/(Nat lhs, const Nat& rhs) {
            auto [quotient, discard] = divmod(lhs, rhs);
            return quotient;
        }
        /**
         * @brief modulo-assignment
         * @details Modulo-divides this Nat by other value and stores result to this
         * @note This returns the modulo/remainder of the division operation
         * @param rhs value to modulo-divide this Nat by
         * @returns resulting object after modulo-assignment
         * @throws std::domain_error when rhs is zero
         * @todo Work out time-complexity
         */
        constexpr Nat& operator%=(const Nat& rhs) {
            Nat remainder = *this % rhs; // uses friend %operator
            // assign remainder's digits back to our digits
            _digits = remainder._digits;
            return *this; // return the result by reference
        }
        /**
         * @brief Modulo operator for Nat
         * @note This implements modulo-division, returning the remainder only
         * @param lhs,rhs operands for the division
         * @returns remainder of lhs / rhs
         * @throws std::domain_error when rhs is zero
         * @todo Work out time-complexity
         */
        friend constexpr Nat operator%(Nat lhs, const Nat& rhs) {
            auto [discard, remainder] = divmod(lhs, rhs);
            return remainder;
        }
        /**
         * @brief bitwise OR-assignment
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        constexpr Nat& operator|=(const Nat& rhs) {
            // add additional digits to this if fewer than rhs
            if (_digits.size() < rhs._digits.size()) {
                _digits.push_front(rhs._digits.size() - _digits.size(), 0); // add leading zeroes
            }
            auto it = _digits.begin();
            auto rhs_it = rhs._digits.begin();
            // if this has more digits than rhs, skip them (OR with implicit 0)
            if (_digits.size() > rhs._digits.size()) {
                for (std::size_t i = 0; i < (_digits.size() - rhs._digits.size()); i++) {
                    it++;
                }
            }
            for (; it != _digits.end() and rhs_it != rhs._digits.end(); it++, rhs_it++) {
                *it |= *rhs_it;
            }
            _validate_digits();
            return *this;
        }
        /**
         * @brief bitwise OR operator for Nat
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        friend constexpr Nat operator|(Nat lhs, const Nat& rhs) {
            lhs |= rhs; // reuse member operator
            return lhs;
        }
        /**
         * @brief bitwise AND-assignment
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        constexpr Nat& operator&=(const Nat& rhs) {
            /*
             * if rhs has fewer digits than this, we can remove this' leading
             * digits because they would be AND'ed with implicit zero which is
             * always zero
             */
            std::size_t lhs_size = _digits.size();
            std::size_t rhs_size = rhs._digits.size();
            if (lhs_size > rhs_size) {
                for (std::size_t i = 0; i < lhs_size - rhs_size; i++) {
                    _digits.pop_front();
                }
            }
            auto it = _digits.begin();
            auto rhs_it = rhs._digits.begin();
            // if rhs has more digits than this, skip them (AND with implicit 0)
            if (rhs._digits.size() > _digits.size()) {
                for (std::size_t i = 0; i < (rhs._digits.size() - _digits.size()); i++) {
                    rhs_it++;
                }
            }
            for (; it != _digits.end() and rhs_it != rhs._digits.end(); it++, rhs_it++) {
                *it &= *rhs_it;
            }
            // remove any leading zeroes
            _remove_leading_zeroes();
            _validate_digits();
            return *this;
        }
        /**
         * @brief bitwise AND operator for Nat
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        friend constexpr Nat operator&(Nat lhs, const Nat& rhs) {
            lhs &= rhs; // reuse member operator
            return lhs;
        }
        /**
         * @brief bitwise XOR-assignment
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        constexpr Nat& operator^=(const Nat& rhs) {
            Nat result = *this ^ rhs; // reuse friend function
            // re-assign digits to this
            _digits = result._digits;
            return *this;
        }
        /**
         * @brief bitwise XOR operator for Nat
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        friend constexpr Nat operator^(Nat lhs, const Nat& rhs) {
            Nat result;
            auto lhs_it = lhs._digits.begin();
            auto rhs_it = rhs._digits.begin();
            std::size_t l = lhs._digits.size();
            std::size_t r = rhs._digits.size();
            // consume only one side when it has more unprocessed digits than the other
            while (lhs_it != lhs._digits.end() or rhs_it != rhs._digits.end()) {
                if (l > r) {
                    result._digits.push_back(*lhs_it); // XOR with zero = self
                    l--;
                    lhs_it++;
                } else if (r > l) {
                    result._digits.push_back(*rhs_it); // XOR with zero = self
                    r--;
                    rhs_it++;
                } else { // otherwise, consume both sides
                    result._digits.push_back(*lhs_it ^ *rhs_it);
                    l--;
                    r--;
                    lhs_it++;
                    rhs_it++;
                }
            }
            // remove any leading zeroes
            result._remove_leading_zeroes();
            result._validate_digits();
            return result;
        }
        /**
         * @brief bitwise left-shift assignment
         * @details Bits are never shifted out, instead the object is enlargened
         * to make them fit.
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        constexpr Nat& operator<<=(uintmax_t n) {
            // break the shift up into whole-digit and part-digit shifts
            auto wholes = n / BITS_PER_DIGIT;
            auto parts = n % BITS_PER_DIGIT;
            // shift up by whole number of digits first
            _digits.push_back(wholes, 0);
            // handle the sub-digit shift next
            if (parts > 0) {
                // add another digit at the top end to accommodate the shift
                _digits.push_front(0);
                // shift up each digit into a bucket twice the size (to not lose top bits)
                for (auto it = ++_digits.begin(); it != _digits.end(); it++) { // second element
                    OverflowType bucket = *it;
                    bucket <<= parts; // do the shift into bucket
                    *it = (StorageType)bucket; // overwrite original value with lower bits in bucket
                    // write upper part of the bucket
                    bucket >>= BITS_PER_DIGIT;
                    it--;
                    *it |= bucket; // OR is to make sure we preserve any already-written bits
                    it++;
                }
            }
            // XXX: why do we require this? Shift operation isn't supposed to leave any leading zeroes...
            if (_digits.front() == 0) {
                _digits.pop_front();
            }
            _validate_digits(); // TODO: remove when satisfied not required
            return *this;
        }
        /**
         * @brief bitwise left-shift for Nat
         * @details Bits are never shifted out, instead the object is enlargened
         * to make them fit.
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        friend constexpr Nat operator<<(Nat lhs, uintmax_t rhs) {
            lhs <<= rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }
        /**
         * @brief bitwise right-shift assignment
         * @details Bits are shifted out rightwards and the object may be shrunk
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        constexpr Nat& operator>>=(uintmax_t n) {
            // cap n to be no more than total bits in number
            if (n > this->bit_length()) { n = this->bit_length(); }
            // break the shift up into whole-digit and part-digit shifts
            auto wholes = n / BITS_PER_DIGIT;
            auto parts = n % BITS_PER_DIGIT;
            // shift down by whole number of digits first
            for (uintmax_t i = 0; i < wholes; i++) {
                _digits.pop_back();
            }
            // handle the sub-digit shift next
            if (parts > 0) {
                for (auto it = _digits.rbegin(); it != _digits.rend(); ) {
                    *it >>= parts;
                    auto prev = it++;
                    if (it != _digits.rend()) {
                        *prev |= (*it << (BITS_PER_DIGIT - parts));
                    }
                }
            }
            // replace digits array with zero if empty
            if (_digits.empty()) {
                _digits = {0};
            }
            // needed in some cases, probably when the intial whole-digit shift leaves a small value which then turns 0
            _remove_leading_zeroes();
            _validate_digits();
            return *this;
        }
        /**
         * @brief bitwise right-shift for Nat
         * @details Bits are shifted out rightwards and the object may be shrunk
         * @note Complexity: @f$ \mathcal{O(n)} @f$
         */
        friend constexpr Nat operator>>(Nat lhs, uintmax_t rhs) {
            lhs >>= rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }
        /**
         * @brief contextual conversion to bool (behaves same way as int)
         * @returns `false` when value is `0`, otherwise `true`
         * @note Complexity: @f$ \mathcal{O(1)} @f$
         */
        explicit constexpr operator bool() const {
            // zero is false --all other values are true
            return _digits.front() != 0; // assuming no leading zeroes
        }
        /**
         * @returns size by number of digits
         */
        constexpr std::size_t digit_length() const {
            return _digits.size();
        }
        /**
         * @returns size by number of bytes needed to store the number's digits
         * @note this can be less than \f$ digits \times sizeof(digit) \f$
         */
        constexpr std::size_t byte_length() const {
            // this is how many bytes are needed to store the digits
            std::size_t bytes_for_digits = _digits.size() * sizeof(StorageType);
            // reduce size if leading digit is not full occupancy
            std::size_t leading_occupancy = PRIVATE::fit(_digits.front(), 256);
            bytes_for_digits -= (sizeof(StorageType) - leading_occupancy);
            return bytes_for_digits;
        }
        /**
         * @returns size by number of bits needed to store the number's value
         * @note this can be less than \f$ bytes \times 8 \f$
         */
        constexpr std::size_t bit_length() const {
            // this is how many bits are needed to store the digits
            std::size_t bits_for_digits = _digits.size() * sizeof(StorageType) * 8;
            // reduce size if leading digit is not full occupancy
            std::size_t leading_occupancy = PRIVATE::fit(_digits.front(), 2);
            bits_for_digits -= (sizeof(StorageType) * 8 - leading_occupancy);
            return bits_for_digits;
        }
        /**
         * @returns a copy of the underlying digits that make up this Nat value
         */
        constexpr codlili::list<StorageType> digits() const {
            return _digits;
        }
        /**
         * @brief Calculates integer log of `x` in `base` as bounds of \f$log_b(x)\f$
         * @code
         * auto [floor, ceil] = ilog(b, x);
         * @endcode
         * @param base base to use for \f$b\f$
         * @param x value to use for \f$x\f$
         * @pre \f$b\geq2\f$
         * @pre \f$x\geq1\f$
         * @throws std::domain_error when preconditions are violated
         * @returns Interval of \f$[floor, ceil]\f$ for \f$log_b(x)\f$
         * @post \f$floor \leq ceil\f$
         * @remarks When \f$floor = ceil\f$:
         * - \f$log_b(x)\in\mathbb{N}\f$
         * @remarks Otherwise:
         * - \f$log_b(x)\in\mathbb{R}\f$
         * @note Complexity: @f$ \mathcal{O}(n^2log(n)) @f$
         * @relates com::saxbophone::arby::Nat
         */
        friend constexpr Interval<uintmax_t> ilog(const Nat& base, const Nat& x);
    private:
        std::string _stringify_for_base(std::uint8_t base) const;

        codlili::list<StorageType> _digits;
    };

    /**
     * @addtogroup math-support Math Support Functions
     * @{
     */

    // define and lift scope of divmod() friend from ADL into arby's scope
    constexpr DivisionResult<Nat> divmod(const Nat& lhs, const Nat& rhs) {
        // division by zero is undefined
        if (rhs._digits.front() == 0) {
            throw std::domain_error("division by zero");
        }
        if (lhs._digits.front() == 0) { return {lhs, lhs}; } // zero shortcut
        // optimisation using bitshifting when dividing by binary powers
        if (rhs.is_power_of_2()) {
            auto width = rhs.bit_length();
            // the remainder is the digits that are shifted out, so bitmask for them
            auto bitmask = (Nat(1) << (width - 1)) - 1;
            Nat quotient = lhs >> (width - 1);
            Nat remainder = lhs & bitmask;
            quotient._validate_digits();
            remainder._validate_digits();
            return {quotient, remainder};
        }
        // this will gradually accumulate the calculated quotient
        Nat quotient;
        // this will gradually decrement with each subtraction
        Nat remainder = lhs;
        // while we have any chance in subtracting further from it
        while (remainder >= rhs) {
            // exponent denotes a raw value describing how many places we can shift rhs up by
            Nat exponent = Nat::get_max_shift(remainder, rhs);
            // estimate how many times it goes in and subtract this many of rhs
            Nat estimate = Nat::estimate_division(remainder, rhs);
            // we'll actually be subtracting rhs shifted by exponent
            Nat shifted_rhs = rhs * exponent;
            if (remainder >= (estimate * shifted_rhs)) {
                remainder -= estimate * shifted_rhs;
                quotient += estimate * exponent;
            }
            // our estimate deliberately underestimates how many times shifted rhs can go into remainder
            // here we subtract further rounds of shifted_rhs if possible
            if (remainder >= (shifted_rhs)) {
                remainder -= (shifted_rhs);
                quotient += exponent;
            }
            // NOTE: this is guaranteed to terminate eventually because the last value that shifted_rhs will take
            // will be rhs without a shift, i.e. rhs * 1, subtraction of which from the remainder is guaranteed to
            // terminate.
        }
        quotient._validate_digits();
        remainder._validate_digits();
        return {quotient, remainder};
    }

    /**
     * @returns base raised to the power of exponent
     * i.e. for base as \f$b\f$ and exponent as \f$x\f$: \f$b^x\f$
     * @param base,exponent parameters for the base and exponent
     * @todo Work out time-complexity
     * @relates com::saxbophone::arby::Nat
     */
    constexpr Nat ipow(const Nat& base, uintmax_t exponent) {
        // use divide-and-conquer recursion to break up huge powers into products of smaller powers
        // exponent = 0 is our base case to terminate the recursion
        if (exponent == 0) { return 1; }
        // exponent = 1 is an additional base case mainly to prevent a redundant level of recursion to 0
        if (exponent == 1) { return base; }
        // exponent = 2 is our final base case, as it seems a waste to leave it to the catch-all case below
        if (exponent == 2) { return base * base; }
        auto quotient = exponent / 2;
        auto remainder = exponent % 2;
        // instead of calculating x^n, do x^(n/2)
        Nat power = ipow(base, quotient);
        power *= power;
        // and multiply by base again if n was odd
        if (remainder == 1) {
            power *= base;
        }
        return power;
    }

    // define and lift scope of ilog() friend from ADL into arby's scope
    constexpr Interval<uintmax_t> ilog(const Nat& base, const Nat& x) {
        if (base < 2) { throw std::domain_error("ilog: base cannot be < 2"); }
        if (x < 1) { throw std::domain_error("ilog: x cannot be < 1"); }
        // if base is 2, count the bits instead
        if (base == 2) {
            auto count = x.bit_length();
            if (x.is_power_of_2()) {
                return {count - 1}; // 1 followed by count-1 many zeroes
            } else {
                return {count - 1, count};
            }
        }
        // if base is a power of 2, we can count how many n-bit chunks there are
        if (base.is_power_of_2()) {
            auto b = ilog(2, base).floor;
            auto xl = ilog(2, x);
            return {xl.floor / b, xl.ceil / b + (xl.ceil % b > 0)};
        }
        // find the smallest power of base that is just >= than x
        Nat power = 1;
        uintmax_t floor = 0;
        uintmax_t exponent = 0;
        while (power < x) {
            power *= base;
            floor = exponent++; // increment and store old value in floor
        }
        // floor = ceil when power = x
        return {power == x ? exponent : floor, exponent};
    }

    /**
     * @brief Calculates integer root \f$[floor, ceil] = \sqrt[n]{x}\f$
     * @returns Interval of floor and ceiling of the given root
     * @pre \f$n\neq0\f$
     * @pre \f$floor\leqceil\f$ for returned Interval
     * @remarks When \f$floor = ceil\f$:
     * - \f$l\sqrt[n]{x}\in\mathbb{N}\f$
     * @remarks Otherwise:
     * - \f$\sqrt[n]{x}\in\mathbb{R}\f$
     */
    constexpr Interval<Nat> iroot(uintmax_t n, const Nat& x) {
        if (n == 0) { throw std::domain_error("0th root is undefined"); }
        if (x < 2) { return x; } // any root of 0 or 1 is always 0 or 1
        if (n == 1) { return x; } // 1th root of anything is itself
        // use the bit-length of x to derive an estimate for nth root magnitude
        auto w = ilog(2, x);
        // then derive floor and ceiling of 2**w/n
        auto floor = ipow(2, w.floor / n);
        auto ceil = ipow(2, w.ceil / n + (w.ceil % n > 0));
        // the answer lies somewhere between 2**floor and 2**ceil
        // use binary search over that interval to home in on the real answer
        while (ceil - floor > 1) {
            auto estimate = (floor + ceil) / 2;
            auto trial = ipow(estimate, n);
            if (trial == x) { return estimate; } // exact match
            if (trial > x) { ceil = estimate; } // too high
            if (trial < x) { floor = estimate; } // too low
        }
        return {floor, ceil};
    }

    /** @} */

    /**
     * @brief Various custom user-defined-literals for creating arby objects
     * @note You need to introduce this namespace into global scope with
     * `using namespace com::saxbophone::arby::literals` in order to be able
     * to use these literals in your code e.g. `arby::Nat f = 12345_nat`
     * This can be done without bringing the whole of arby into global scope
     * and these literals are provided in a sub-namespace for this exact reason
     * @note If you introduce namespace com::saxbophone::arby into global scope,
     * you don't need to also introduce this literals namespace --arby introduces
     * this one automatically.
     */
    namespace literals {
        /**
         * @brief raw user-defined-literal for Nat class
         * @param literal the literal
         * @returns Corresponding arby::Nat value
         * @note we use a raw literal in this case because as the Nat type is
         * unbounded, we want to support a potentially infinite number of digits,
         * or certainly more than can be stored in unsigned long long...
         * @relates com::saxbophone::arby::Nat
         */
        constexpr Nat operator "" _nat(const char* literal) {
            // detect number base
            std::uint8_t base = 10; // base-10 is the fallback base
            if (literal[0] == '0' and literal[1] != 0) { // first digit 0, second non-null, maybe a 0x/0b prefix?
                switch (literal[1]) {
                case 'X': // hexadecimal
                case 'x':
                    base = 16;
                    // advance string pointer to skip the prefix
                    literal = literal + 2;
                    break;
                case 'B': // binary
                case 'b':
                    base = 2;
                    literal = literal + 2;
                    break;
                default: // not allowed --we don't support 0-prefixed octal literals or anything else
                    throw std::invalid_argument("invalid arby::Nat literal");
                }
            }
            Nat value; // accumulator
            // consume digits
            while (*literal != 0) { // until null-terminator is found
                std::uint8_t digit = (std::uint8_t)*literal; // get character
                // when dealing with digits, subtract 32 from any after 'Z' to convert lowercase to upper
                if (digit > 'Z') { digit -= 32; }
                // calculate digit's value, handling the two contiguous ranges of 0-9 and A-F
                std::uint8_t digit_value = digit >= 'A' ? (digit - 'A') + 10 : digit - '0';
                // add to accumulator and then shift it up
                value *= base;
                value += digit_value;
                literal++; // next character
            }
            return value;
        }
    }

    // introduce literals namespace into the scope of arby namespace
    using namespace literals;
}

// adding template specialisation to std::numeric_limits<> for arby::Nat
template <>
class std::numeric_limits<com::saxbophone::arby::Nat> {
public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false; // N/A
    static constexpr bool has_quiet_NaN = false; // N/A
    static constexpr bool has_signaling_NaN = false; // N/A
    static constexpr bool has_denorm = false; // N/A
    static constexpr bool has_denorm_loss = false; // N/A
    static constexpr std::float_round_style round_style = std::round_toward_zero;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_bounded = false; // an unbounded type!
    static constexpr bool is_modulo = false; // Nat increases number of digits on overflow. Underflow is undefined.
    static constexpr int digits = 0; // N/A --no hard limit
    static constexpr int digits10 = 0; // N/A --no hard limit
    static constexpr int max_digits10 = 0; // N/A --no hard limit
    static constexpr int radix = 2; // NOTE: no longer stores Nat::BASE as radix must be an int, but BASE can overflow int
    static constexpr int min_exponent = 0; // N/A
    static constexpr int min_exponent10 = 0; // N/A
    static constexpr int max_exponent = 0; // N/A
    static constexpr int max_exponent10 = 0; // N/A
    static constexpr bool traps = true; // we haven't yet implemented division, but there are no plans to specially handle division by zero
    static constexpr bool tinyness_before = false; // N/A
    // these methods should be made constexpr when constexpr std::vector is widely supported
    static constexpr com::saxbophone::arby::Nat min() { return 0; };
    static constexpr com::saxbophone::arby::Nat lowest() { return 0; };
    static constexpr com::saxbophone::arby::Nat max() { return 0; }; // N/A --no hard limit on maximum value
    static constexpr com::saxbophone::arby::Nat epsilon() { return 0; } // N/A
    static constexpr com::saxbophone::arby::Nat round_error() { return 0; } // N/A
    static constexpr com::saxbophone::arby::Nat infinity() { return 0; } // N/A
    static constexpr com::saxbophone::arby::Nat quiet_NaN() { return 0; } // N/A
    static constexpr com::saxbophone::arby::Nat signaling_NaN() { return 0; } // N/A
    static constexpr com::saxbophone::arby::Nat denorm_min() { return 0; } // N/A
};

#endif // include guard
