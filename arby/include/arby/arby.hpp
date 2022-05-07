/**
 * @file
 * This is a sample public compilation unit.
 *
 * @author Your Name <your.email.address@goes.here>
 * @date Creation/Edit Date
 *
 * @copyright Copyright information goes here
 *
 * @copyright
 * Copyright information can span multiple paragraphs if needed, such as if you
 * use a well-known software license for which license header text (to be
 * placed in locations like these) are provided by the license custodians.
 *
 */

#ifndef COM_SAXBOPHONE_ARBY_ARBY_HPP
#define COM_SAXBOPHONE_ARBY_ARBY_HPP

#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <compare>
#include <limits>
#include <string>
#include <stdexcept>
#include <tuple>
#include <vector>

#include <iostream>


namespace {
    /*
     * these template specialisations are used for selecting the unsigned type
     * to use for storing the digits of our arbitrary-size numbers
     *
     * the template specialisations pick the corresponding unsigned type and the
     * next-smallest unsigned type for the given signed type, this is to ensure
     * that the number base is not out of range of int on any given system, and
     * means we can report it from the radix() method in numeric_limits<>
     * (which returns int).
     *
     * - OverflowType denotes the unsigned equivalent of the given signed type,
     * this can safely store MAX*MAX of the next-lowest unsigned type, so is
     * useful to store the intermediate results for multiplication and addition.
     * - StorageType denotes the next-lowest unsigned type. This is the type
     * which is used to store the digits of the arbitrary-size number.
     * - BITS_BETWEEN denotes the number of bits needed to shift StorageType
     * up to OverflowType
     */
    template <typename T>
    struct GetStorageType {
        using OverflowType = void;
        using StorageType = void;
        static constexpr std::size_t BITS_BETWEEN = 0;
    };

    template <>
    struct GetStorageType<std::int64_t> {
        using OverflowType = std::uint64_t;
        using StorageType = std::uint32_t;
        static constexpr std::size_t BITS_BETWEEN = 32;
    };

    template <>
    struct GetStorageType<std::int32_t> {
        using OverflowType = std::uint32_t;
        using StorageType = std::uint16_t;
        static constexpr std::size_t BITS_BETWEEN = 16;
    };

    template <>
    struct GetStorageType<std::int16_t> {
        using OverflowType = std::uint16_t;
        using StorageType = std::uint8_t;
        static constexpr std::size_t BITS_BETWEEN = 8;
    };

    // returns ceil(logₐ(n))
    constexpr std::size_t fit(uintmax_t n, uintmax_t a) {
        // n = 0 is the exception --we don't use any digits at all for 0
        if (n == 0) {
            return 0;
        }
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

// only define Uint as a constexpr class if there is library support for constexpr std::vector
#ifdef __cpp_lib_constexpr_vector
#define constexprvector constexpr
#else
#define constexprvector
#endif

namespace com::saxbophone::arby {
    class Uint {
    public:
        using StorageType = GetStorageType<int>::StorageType;

        static constexpr int BASE = (int)std::numeric_limits<StorageType>::max() + 1;

        constexprvector bool operator==(const Uint& rhs) const = default;

        // three-way-comparison operator defines all relational operators
        constexprvector auto operator<=>(const Uint& rhs) const {
            // use size to indicate ordering if they differ
            if (_digits.size() != rhs._digits.size()) {
                return _digits.size() <=> rhs._digits.size();
            } else { // otherwise compare the elements until a mismatch is found
                return _digits <=> rhs._digits;
            }
        }

        constexprvector Uint() : Uint(0) {}

        constexprvector Uint(uintmax_t value) : _digits(fit(value, Uint::BASE)) {
            if (_digits.size() > 0) {
                // fill out digits in big-endian order
                uintmax_t power = exp(Uint::BASE, _digits.size() - 1);
                for (auto& digit : _digits) {
                    digit = (StorageType)(value / power);
                    value %= power;
                    power /= Uint::BASE;
                }
            }
        }

        Uint(std::string digits);

        explicit constexprvector operator uintmax_t() const {
            uintmax_t accumulator = 0;
            uintmax_t current_radix = 1;
            // digits are stored in big-endian order, but we read them out in little-endian
            // TODO: loops like this make my head hurt. Let's read it out in big-endian order instead
            for (auto digit = _digits.rbegin(); digit != _digits.rend(); ++digit) {
                accumulator += *digit * current_radix;
                current_radix *= Uint::BASE;
            }
            return accumulator;
        }

        explicit operator std::string() const;
        // prefix increment
        constexprvector Uint& operator++() {
            // empty digits vector (means value is zero) is a special case
            if (_digits.size() == 0) {
                _digits.push_back(1);
            } else {
                // increment least significant digit
                auto it = _digits.rbegin();
                (*it)++;
                // increment remaining digits (rollover) as needed
                while (it != _digits.rend() and *it == 0) { // last digit overflowed to zero
                    ++it; // increment index
                    (*it)++; // increment digit
                }
                // if last digit is zero, we need another one
                if (_digits.front() == 0) {
                    _digits.insert(_digits.begin(), 1);
                }
            }
            return *this; // return new value by reference
        }
        // postfix increment
        constexprvector Uint operator++(int) {
            Uint old = *this; // copy old value
            operator++();  // prefix increment
            return old;    // return old value
        }
        // prefix decrement
        constexprvector Uint& operator--() {
            // empty digits vector (means value is zero) is a special case
            if (_digits.size() == 0) {
                throw std::underflow_error("arithmetic underflow: can't decrement unsigned zero");
            } else {
                // decrement least significant digit
                auto it = _digits.rbegin();
                (*it)--;
                // decrement remaining digits (borrow) as needed
                while (it != _digits.rend() and *it == std::numeric_limits<StorageType>::max()) { // last digit overflowed to zero
                    ++it; // increment index
                    (*it)--; // decrement digit
                }
                // if last digit is zero, remove it
                if (_digits.front() == 0) {
                    _digits.erase(_digits.begin());
                }
            }
            return *this; // return new value by reference
        }
        // postfix decrement
        constexprvector Uint operator--(int) {
            Uint old = *this; // copy old value
            operator--();  // prefix decrement
            return old;    // return old value
        }
        // addition-assignment
        constexprvector Uint& operator+=(Uint rhs) {
            using OverflowType = GetStorageType<int>::OverflowType;
            // either arg being a zero is a no-op, guard against this
            if (_digits.size() != 0 or rhs._digits.size() != 0) {
                // make sure this and rhs are the same size, fill with leading zeroes if needed
                if (rhs._digits.size() > _digits.size()) {
                    _digits.insert(_digits.begin(), rhs._digits.size() - _digits.size(), 0);
                } else if (_digits.size() > rhs._digits.size()) {
                    rhs._digits.insert(rhs._digits.begin(), _digits.size() - rhs._digits.size(), 0);
                }
                // work backwards up the digits vector of the rhs
                StorageType carry = 0; // carries are stored here on overflow
                for (std::size_t i = _digits.size(); i --> 0; ) {
                    OverflowType addition = (OverflowType)_digits[i] + rhs._digits[i] + carry;
                    // downcast to chop off any more significant bits
                    // (effectively cheap modulo because we know OverflowType is twice the width of StorageType)
                    _digits[i] = (StorageType)addition;
                    // update the carry with the value in the top significant bits
                    carry = (StorageType)(addition >> GetStorageType<int>::BITS_BETWEEN);
                }
                // if carry is non-zero, then add it to the next most significant digit, expanding size of this if needed
                if (carry != 0) {
                    _digits.insert(_digits.begin(), carry);
                }
            }
            return *this; // return the result by reference
        }
        // addition
        friend constexprvector Uint operator+(Uint lhs, const Uint& rhs) {
            lhs += rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }
        // subtraction-assignment
        constexprvector Uint& operator-=(Uint rhs) {
            using OverflowType = GetStorageType<int>::OverflowType;
            // TODO: detect underflow early?
            // rhs being a zero is a no-op, guard against this
            if (rhs._digits.size() != 0) {
                // make sure this and rhs are the same size, fill with leading zeroes if needed
                if (rhs._digits.size() > _digits.size()) {
                    _digits.insert(_digits.begin(), rhs._digits.size() - _digits.size(), 0);
                } else if (_digits.size() > rhs._digits.size()) {
                    rhs._digits.insert(rhs._digits.begin(), _digits.size() - rhs._digits.size(), 0);
                }
                // work backwards up the digits vector of the rhs
                bool borrow = false; // transfers borrows up when triggered
                for (std::size_t i = _digits.size(); i --> 0; ) {
                    // this will underflow correctly in a way that means we can get the remainder off the bottom bits
                    OverflowType subtraction = (OverflowType)_digits[i] - rhs._digits[i] - borrow;
                    // downcast to chop off any more significant bits
                    // (effectively cheap modulo because we know OverflowType is twice the width of StorageType)
                    _digits[i] = (StorageType)subtraction;
                    // detect any borrow that is needed
                    borrow = subtraction > std::numeric_limits<StorageType>::max();
                }
                // if borrow is non-zero, then an arithmetic underflow occurred
                if (borrow) {
                    throw std::underflow_error("arithmetic underflow: subtrahend bigger than minuend");
                }
            }
            return *this; // return the result by reference
        }
        // subtraction
        friend constexprvector Uint operator-(Uint lhs, const Uint& rhs) {
            lhs -= rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }
        // multiplication-assignment
        constexprvector Uint& operator*=(const Uint& rhs) {
            using OverflowType = GetStorageType<int>::OverflowType;
            // either operand being zero always results in zero
            if (_digits.size() == 0 or rhs._digits.size() == 0) {
                _digits.clear();
            } else {
                // store a copy of this for use as lhs
                const Uint lhs = *this;
                // reset this to zero
                _digits.clear();
                // multiply each digit from lhs with each digit from rhs
                for (std::size_t l = 0; l < lhs._digits.size(); l++) {
                    for (std::size_t r = 0; r < rhs._digits.size(); r++) {
                        OverflowType multiplication = lhs._digits[l] * rhs._digits[r];
                        // create a new Uint with this intermediate result and add trailing places as needed
                        Uint intermediate = multiplication;
                        // we need to remap the indices as the digits are stored big-endian
                        std::size_t shift_amount = (lhs._digits.size() - 1 - l) + (rhs._digits.size() - 1 - r);
                        // add that many trailing zeroes to intermediate's digits
                        intermediate._digits.insert(intermediate._digits.end(), shift_amount, 0);
                        // finally, add it to this as an accumulator
                        *this += intermediate;
                    }
                }
            }
            return *this; // return the result by reference
        }
        // multiplication
        friend constexprvector Uint operator*(Uint lhs, const Uint& rhs) {
            lhs *= rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }
        // division and modulo all-in-one, equivalent to C/C++ div() and Python divmod()
        // returns tuple of {quotient, remainder}
        static constexprvector std::tuple<Uint, Uint> divmod(const Uint& lhs, const Uint& rhs) {
            // using OverflowType = GetStorageType<int>::OverflowType;
            // division by zero is undefined
            if (rhs._digits.size() == 0) {
                throw std::domain_error("division by zero");
            }
            // when denominator is longer than numerator, answer must be zero
            if (rhs._digits.size() > lhs._digits.size()) {
                return {arby::Uint(0), lhs};
            }
            // this will gradually accumulate the calculated quotient
            Uint quotient;
            // this will gradually decrement with each subtraction
            Uint remainder = lhs;
            // how many places can we shift rhs left until it's the same width as lhs?
            std::size_t wiggle_room = lhs._digits.size() - rhs._digits.size();
            // drag back down wiggle_room if a shift is requested but lhs[0] < rhs[0]
            if (wiggle_room > 0 and lhs._digits[0] < rhs._digits[0]) {
                wiggle_room--;
            }
            // now, shift up rhs by this amount and see how many times it goes into lhs
            Uint shift = 1;
            shift._digits.insert(shift._digits.end(), wiggle_room, 0);
            // while we haven't shifted out any of rhs' original digits
            while (shift._digits.size() >= rhs._digits.size()) {
                std::cout << "while (" << shift._digits.size() << " >= " << rhs._digits.size() << ") {" << std::endl;
                Uint chunk = rhs * shift;
                std::cout << (uintmax_t)chunk << " = " << (uintmax_t)rhs << " * " << (uintmax_t)shift << ";" << std::endl;
                std::cout << "while (" << (uintmax_t)remainder << " >= " << (uintmax_t)chunk << ") ?" << std::endl;
                while (remainder >= chunk) {
                    std::cout << "while (" << (uintmax_t)remainder << " >= " << (uintmax_t)chunk << ") {" << std::endl;
                    std::cout << (uintmax_t)remainder << " -= " << (uintmax_t)chunk << ";" << std::endl;
                    std::cout << (uintmax_t)quotient << " += " << (uintmax_t)shift << ";" << std::endl;
                    remainder -= chunk;
                    quotient += shift;
                    std::cout << "}" << std::endl;
                }
                // shift out one of shift's digits
                shift._digits.pop_back();
                std::cout << "}" << std::endl;
            }
            return {quotient, remainder};
            // for (std::size_t s = max_shift; s --> 0; ) {
            //     // this is the digit of lhs that the front digit of rhs will line up with
            //     std::size_t lhs_target = max_shift - 1 - s;
            //     // this is like an exponent, denoting the magnitude of the number of places we've shifted
            //     Uint shift = 1;
            //     // shift it leftwards by inserting s many zero digits at the end
            //     shift._digits.insert(shift._digits.end(), s, 0);
            //     // a good initial estimate of the lower-bound for number of times it goes in is given thus
            //     Uint estimate = remainder._digits[0] / ((OverflowType)rhs._digits[0] + 1);
            //     // subtract this many of rhs from remainder and increment quotient accordingly
            //     remainder -= rhs * estimate * shift;
            //     quotient += estimate * shift;
            //     // subtract any additional rounds that will fit
            //     while (remainder >= rhs * shift) {
            //         remainder -= rhs * shift;
            //         quotient += shift;
            //     }
            //     std::cout << (uintmax_t)lhs << " " << (uintmax_t)quotient << std::endl;
            // }
            // return {quotient, remainder};
        }
        // division-assignment
        constexprvector Uint& operator/=(const Uint& rhs) {
            Uint quotient = *this / rhs; // uses friend /operator
            // assign quotient's digits back to our digits
            _digits = quotient._digits;
            return *this; // return the result by reference
        }
        // division
        friend constexprvector Uint operator/(Uint lhs, const Uint& rhs) {
            Uint quotient;
            std::tie(quotient, std::ignore) = Uint::divmod(lhs, rhs);
            return quotient;
        }
        // modulo-assignment
        constexprvector Uint& operator%=(const Uint& rhs) {
            Uint remainder = *this % rhs; // uses friend %operator
            // assign remainder's digits back to our digits
            _digits = remainder._digits;
            return *this; // return the result by reference
        }
        // modulo
        friend constexprvector Uint operator%(Uint lhs, const Uint& rhs) {
            Uint remainder;
            std::tie(std::ignore, remainder) = Uint::divmod(lhs, rhs);
            return remainder;
        }
        // left-shift-assignment
        constexprvector Uint& operator<<=(const Uint& n) {
            // TODO: implement
            return *this;
        }
        // left-shift
        friend constexprvector Uint operator<<(Uint lhs, const Uint& rhs) {
            lhs <<= rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }
        // right-shift-assignment
        constexprvector Uint& operator>>=(const Uint& n) {
            // TODO: implement
            return *this;
        }
        // right-shift
        friend constexprvector Uint operator>>(Uint lhs, const Uint& rhs) {
            lhs <<= rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }
        // contextual conversion to bool (behaves same way as int)
        explicit constexprvector operator bool() const {
            // TODO: implement
            return false;
        }
        // unary minus
        // D'oh! This type is unsigned. It's tricky to know exactly what unary
        // minus means when applied to unsigned, but this SO answer explains:
        // https://stackoverflow.com/a/14065392/6177253
        constexprvector Uint operator-() const {
            // TODO: implement
            return *this;
        }
    private:
        std::vector<StorageType> _digits;
    };
}

// adding template specialisation to std::numeric_limits<> for arby::Uint
template <>
class std::numeric_limits<com::saxbophone::arby::Uint> {
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
    static constexpr bool is_modulo = false; // Uint increases number of digits on overflow. Underflow is undefined.
    static constexpr int digits = 0; // N/A --no hard limit
    static constexpr int digits10 = 0; // N/A --no hard limit
    static constexpr int max_digits10 = 0; // N/A --no hard limit
    static constexpr int radix = com::saxbophone::arby::Uint::BASE; // NOTE: this is the radix used for each digit, all of which are binary
    static constexpr int min_exponent = 0; // N/A
    static constexpr int min_exponent10 = 0; // N/A
    static constexpr int max_exponent = 0; // N/A
    static constexpr int max_exponent10 = 0; // N/A
    static constexpr bool traps = true; // we haven't yet implemented division, but there are no plans to specially handle division by zero
    static constexpr bool tinyness_before = false; // N/A
    // these methods should be made constexpr when constexpr std::vector is widely supported
    static constexprvector com::saxbophone::arby::Uint min() { return 0; };
    static constexprvector com::saxbophone::arby::Uint lowest() { return 0; };
    static constexprvector com::saxbophone::arby::Uint max() { return 0; }; // N/A --no hard limit on maximum value
    static constexprvector com::saxbophone::arby::Uint epsilon() { return 0; } // N/A
    static constexprvector com::saxbophone::arby::Uint round_error() { return 0; } // N/A
    static constexprvector com::saxbophone::arby::Uint infinity() { return 0; } // N/A
    static constexprvector com::saxbophone::arby::Uint quiet_NaN() { return 0; } // N/A
    static constexprvector com::saxbophone::arby::Uint signaling_NaN() { return 0; } // N/A
    static constexprvector com::saxbophone::arby::Uint denorm_min() { return 0; } // N/A
};

// avoid polluting global macro namespace by tidying up our macros after us
#undef constexprvector

#endif // include guard
