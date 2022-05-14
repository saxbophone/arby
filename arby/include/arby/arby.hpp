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

#ifdef __cpp_lib_constexpr_vector
// only define Uint as a constexpr class if there is library support for constexpr std::vector
#define constexprvector constexpr
#else
// otherwise define the special macro as inline to keep the inline semantics of all things that use it
// this is needed because it can cause linkage issues with duplicated symbols otherwise
#define constexprvector inline /**< Specifies `constexpr` if std::vector is `constexpr`, otherwise `inline` */
#endif

namespace com::saxbophone::arby {
    /**
     * @brief Arbitrary-precision unsigned integer type
     * @note `std::numeric_limits<Uint>` is specialised such that most of the
     * members of that type are implemented to describe the traits of this type.
     * @note Exceptions include any members which describe a finite number of digits
     * or a maximmum value, neither of which apply to this type as it is unbounded.
     */
    class Uint {
    private:
        using StorageType = GetStorageType<int>::StorageType;
        using OverflowType = GetStorageType<int>::OverflowType;
    public:
        /**
         * @brief The number base used internally to store the value
         * @details This is the radix that the digits are encoded in
         */
        static constexpr int BASE = (int)std::numeric_limits<StorageType>::max() + 1;
        /**
         * @brief Defaulted equality operator for Uint objects
         * @param rhs other Uint object to compare against
         * @returns `true` if objects are equal, otherwise `false`
         */
        constexprvector bool operator==(const Uint& rhs) const = default;
        /**
         * @brief three-way-comparison operator defines all relational operators
         * @param rhs other Uint object to compare against
         * @returns std::strong_ordering object for comparison
         */
        constexprvector auto operator<=>(const Uint& rhs) const {
            // use size to indicate ordering if they differ
            if (_digits.size() != rhs._digits.size()) {
                return _digits.size() <=> rhs._digits.size();
            } else { // otherwise compare the elements until a mismatch is found
                return _digits <=> rhs._digits;
            }
        }
        /**
         * @brief Default constructor, initialises to numeric value `0`
         */
        constexprvector Uint() : Uint(0) {}
        /**
         * @brief Value-constructor, initialises with the given value
         * @param value value to initialise with
         */
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
        /**
         * @brief String-constructor, initialises from string decimal value
         * @param digits string containing the digits of the value to initialise
         * with, written in decimal
         * @warning Unimplemented
         */
        Uint(std::string digits);
        /**
         * @returns Value of this Uint object cast to uintmax_t
         * @throws std::overflow_error when Uint value is out of range for
         * uintmax_t
         */
        explicit constexprvector operator uintmax_t() const {
            // prevent overflow of uintmax_t
            if (*this > std::numeric_limits<uintmax_t>::max()) {
                throw std::overflow_error("value too large for uintmax_t");
            }
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
        /**
         * @brief custom ostream operator that allows this class to be printed
         * with std::cout and friends
         * @param os stream to output to
         * @param object Uint to print
         */
        friend std::ostream& operator<<(std::ostream& os, const Uint& object);
        /**
         * @returns string representing the value of this Uint, in decimal
         */
        explicit operator std::string() const;
        /**
         * @brief prefix increment
         * @returns new value of Uint object after incrementing
         */
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
        /**
         * @brief postfix increment
         * @returns old value of Uint object before incrementing
         */
        constexprvector Uint operator++(int) {
            Uint old = *this; // copy old value
            operator++();  // prefix increment
            return old;    // return old value
        }
        /**
         * @brief prefix decrement
         * @returns new value of Uint object after decrementing
         * @throws std::underflow_error when value of Uint is `0`
         */
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
        /**
         * @brief postfix decrement
         * @returns old value of Uint object before decrementing
         * @throws std::underflow_error when value of Uint is `0`
         */
        constexprvector Uint operator--(int) {
            Uint old = *this; // copy old value
            operator--();  // prefix decrement
            return old;    // return old value
        }
        /**
         * @brief addition-assignment
         * @details Adds other value to this Uint and assigns the result to self
         * @param rhs value to add to this Uint
         * @returns resulting object after addition-assignment
         */
        constexprvector Uint& operator+=(Uint rhs) {
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
        /**
         * @brief Addition operator for Uint
         * @param lhs,rhs operands for the addition
         * @returns sum of lhs + rhs
         */
        friend constexprvector Uint operator+(Uint lhs, const Uint& rhs) {
            lhs += rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }
        /**
         * @brief subtraction-assignment
         * @details Subtracts other value from this Uint and assigns the result to self
         * @param rhs value to subtract from this Uint
         * @returns resulting object after subtraction-assignment
         */
        constexprvector Uint& operator-=(Uint rhs) {
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
            // remove any leading zeroes
            while (_digits.size() > 0 and _digits.front() == 0) {
                _digits.erase(_digits.begin());
            }
            return *this; // return the result by reference
        }
        /**
         * @brief Subtraction operator for Uint
         * @param lhs,rhs operands for the subtraction
         * @returns result of lhs - rhs
         */
        friend constexprvector Uint operator-(Uint lhs, const Uint& rhs) {
            lhs -= rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }
        /**
         * @brief multiplication-assignment
         * @details Multiplies this Uint by other value and assigns the result to self
         * @param rhs value to multiply this Uint by
         * @returns resulting object after multiplication-assignment
         */
        constexprvector Uint& operator*=(Uint rhs) {
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
        /**
         * @brief Multiplication operator for Uint
         * @param lhs,rhs operands for the multiplication
         * @returns product of lhs * rhs
         */
        friend constexprvector Uint operator*(Uint lhs, const Uint& rhs) {
            lhs *= rhs; // reuse compound assignment
            return lhs; // return the result by value (uses move constructor)
        }
    private: // private helper methods for Uint::divmod()
        // function that shifts up rhs to be just big enough to be smaller than lhs
        static constexprvector Uint get_max_shift(const Uint& lhs, const Uint& rhs) {
            // how many places can we shift rhs left until it's the same width as lhs?
            std::size_t wiggle_room = lhs._digits.size() - rhs._digits.size();
            // drag back down wiggle_room if a shift is requested but lhs[0] < rhs[0]
            if (wiggle_room > 0 and lhs._digits[0] < rhs._digits[0]) {
                wiggle_room--;
            }
            Uint shift = 1;
            shift._digits.insert(shift._digits.end(), wiggle_room, 0);
            return shift;
        }
        // uses leading 1..2 digits of lhs and leading digits of rhs to estimate how many times it goes in
        static constexprvector OverflowType estimate_division(const Uint& lhs, const Uint& rhs) {
            OverflowType denominator = (OverflowType)rhs._digits[0];
            // if any of the other digits of rhs are non-zero...
            if (std::any_of(rhs._digits.begin() + 1, rhs._digits.end(), [](StorageType digit){ return digit != 0; })) {
                // increment denominator, we don't know what those other digits are so we have to assume denominator
                // is closer in value to denominator+1 and estimate accordingly, by deliberately underestimating...
                denominator++;
            }
            if (lhs._digits[0] >= denominator) { // use lhs[0] / rhs[0] only
                return (OverflowType)lhs._digits[0] / denominator;
            } else { // use lhs[0..1] / rhs[0]
                // chop off all but the leading two digits of lhs to get the numerator
                Uint leading_digits = lhs;
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
         * @returns tuple of {quotient, remainder}
         */
        static constexprvector std::tuple<Uint, Uint> divmod(const Uint& lhs, const Uint& rhs) {
            // division by zero is undefined
            if (rhs._digits.size() == 0) {
                throw std::domain_error("division by zero");
            }
            // this will gradually accumulate the calculated quotient
            Uint quotient;
            // this will gradually decrement with each subtraction
            Uint remainder = lhs;
            // while we have any chance in subtracting further from it
            while (remainder >= rhs) {
                // exponent denotes a raw value describing how many places we can shift rhs up by
                Uint exponent = Uint::get_max_shift(remainder, rhs);
                // estimate how many times it goes in and subtract this many of rhs
                Uint estimate = Uint::estimate_division(remainder, rhs);
                // we'll actually be subtracting rhs shifted by exponent
                Uint shifted_rhs = rhs * exponent;
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
            return {quotient, remainder};
        }
        /**
         * @brief division-assignment
         * @details Divides this Uint by other value and stores result to this
         * @note This implements floor-division, returning the quotient only
         * @param rhs value to divide this Uint by
         * @returns resulting object after division-assignment
         */
        constexprvector Uint& operator/=(const Uint& rhs) {
            Uint quotient = *this / rhs; // uses friend /operator
            // assign quotient's digits back to our digits
            _digits = quotient._digits;
            return *this; // return the result by reference
        }
        /**
         * @brief Division operator for Uint
         * @note This implements floor-division, returning the quotient only
         * @param lhs,rhs operands for the division
         * @returns quotient of lhs / rhs
         */
        friend constexprvector Uint operator/(Uint lhs, const Uint& rhs) {
            Uint quotient;
            std::tie(quotient, std::ignore) = Uint::divmod(lhs, rhs);
            return quotient;
        }
        /**
         * @brief modulo-assignment
         * @details Modulo-divides this Uint by other value and stores result to this
         * @note This returns the modulo/remainder of the division operation
         * @param rhs value to modulo-divide this Uint by
         * @returns resulting object after modulo-assignment
         */
        constexprvector Uint& operator%=(const Uint& rhs) {
            Uint remainder = *this % rhs; // uses friend %operator
            // assign remainder's digits back to our digits
            _digits = remainder._digits;
            return *this; // return the result by reference
        }
        /**
         * @brief Modulo operator for Uint
         * @note This implements modulo-division, returning the remainder only
         * @param lhs,rhs operands for the division
         * @returns remainder of lhs / rhs
         */
        friend constexprvector Uint operator%(Uint lhs, const Uint& rhs) {
            Uint remainder;
            std::tie(std::ignore, remainder) = Uint::divmod(lhs, rhs);
            return remainder;
        }
        /**
         * @returns base raised to the power of exponent
         * @param base,exponent parameters for the base and exponent
         */
        static constexprvector Uint pow(const Uint& base, const Uint& exponent) {
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
            auto [quotient, remainder] = Uint::divmod(exponent, 2);
            // instead of calculating x^n, do x^(n/2)
            Uint power = Uint::pow(base, quotient);
            power *= power;
            // and multiply by base again if n was odd
            if (remainder == 1) {
                power *= base;
            }
            return power;
        }
        // XXX: unimplemented shift operators commented out until implemented
        // // left-shift-assignment
        // constexprvector Uint& operator<<=(const Uint& n) {
        //     // TODO: implement
        //     return *this;
        // }
        // // left-shift
        // friend constexprvector Uint operator<<(Uint lhs, const Uint& rhs) {
        //     lhs <<= rhs; // reuse compound assignment
        //     return lhs; // return the result by value (uses move constructor)
        // }
        // // right-shift-assignment
        // constexprvector Uint& operator>>=(const Uint& n) {
        //     // TODO: implement
        //     return *this;
        // }
        // // right-shift
        // friend constexprvector Uint operator>>(Uint lhs, const Uint& rhs) {
        //     lhs <<= rhs; // reuse compound assignment
        //     return lhs; // return the result by value (uses move constructor)
        // }
        /**
         * @brief contextual conversion to bool (behaves same way as int)
         * @returns `false` when value is `0`, otherwise `true`
         */
        explicit constexprvector operator bool() const {
            // zero is false --all other values are true
            return _digits.size() > 0; // zero is encoded as empty digits array
        }
    private:
        std::string _stringify_for_base() const;

        std::vector<StorageType> _digits;
    };

    /**
     * @brief raw user-defined-literal for Uint class
     * @param literal the literal
     * @returns Corresponding arby::Uint value
     * @note we use a raw literal in this case because as the Uint type is
     * unbounded, we want to support a potentially infinite number of digits,
     * or certainly more than can be stored in unsigned long long...
     */
    constexprvector Uint operator "" _uarb(const char* literal) {
        // we can't use strlen or std::string to get the length becuase neither are constexpr
        std::size_t length = 0;
        while (literal[length] != 0) { // search for the null-terminator
            length++;
        }
        Uint value;
        // go through character by character, adding them to the final value
        Uint power = Uint::pow(10, length - 1);
        for (std::size_t i = 0; i < length; i++) {
            auto digit = literal[i] - '0';
            value += (uintmax_t)digit * power;
            power /= 10;
        }
        return value;
    }
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
