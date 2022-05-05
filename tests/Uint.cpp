#include <compare>
#include <limits>
#include <stdexcept>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone;

TEST_CASE("std::numeric_limits<arby::Uint>") {
    CHECK(std::numeric_limits<arby::Uint>::is_specialized);
    CHECK_FALSE(std::numeric_limits<arby::Uint>::is_signed);
    CHECK(std::numeric_limits<arby::Uint>::is_integer);
    CHECK(std::numeric_limits<arby::Uint>::is_exact);
    CHECK_FALSE(std::numeric_limits<arby::Uint>::has_infinity); // N/A
    CHECK_FALSE(std::numeric_limits<arby::Uint>::has_quiet_NaN); // N/A
    CHECK_FALSE(std::numeric_limits<arby::Uint>::has_signaling_NaN); // N/A
    CHECK_FALSE(std::numeric_limits<arby::Uint>::has_denorm); // N/A
    CHECK_FALSE(std::numeric_limits<arby::Uint>::has_denorm_loss); // N/A
    CHECK(std::numeric_limits<arby::Uint>::round_style == std::round_toward_zero);
    CHECK_FALSE(std::numeric_limits<arby::Uint>::is_iec559);
    CHECK_FALSE(std::numeric_limits<arby::Uint>::is_bounded);
    CHECK_FALSE(std::numeric_limits<arby::Uint>::is_modulo);
    CHECK(std::numeric_limits<arby::Uint>::digits == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::digits10 == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::max_digits10 == 0); // N/A
    // calculate the bit width of the next-lowest type, its place value is the radix
    CHECK(std::numeric_limits<arby::Uint>::radix == 1u << ((std::numeric_limits<int>::digits + 1) / 2));
    CHECK(std::numeric_limits<arby::Uint>::min_exponent == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::min_exponent10 == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::max_exponent == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::max_exponent10 == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::traps);
    CHECK_FALSE(std::numeric_limits<arby::Uint>::tinyness_before); // N/A
    // can't uncomment these tests until Uint has an equality operator and can be ctor() from int
    CHECK(std::numeric_limits<arby::Uint>::min() == 0);
    CHECK(std::numeric_limits<arby::Uint>::lowest() == 0);
    CHECK(std::numeric_limits<arby::Uint>::max() == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::epsilon() == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::round_error() == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::infinity() == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::quiet_NaN() == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::signaling_NaN() == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::denorm_min() == 0); // N/A
}

TEST_CASE("arby::Uint(uintmax_t random) and (uintmax_t)arby::Uint") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    arby::Uint output(input);

    CHECK((uintmax_t)output == input);
}

TEST_CASE("arby::Uint(0) and (uintmax_t)arby::Uint") {
    arby::Uint output(0);

    CHECK((uintmax_t)output == 0);
}

TEST_CASE("arby::Uint(UINT_MAX) and (uintmax_t)arby::Uint") {
    arby::Uint output(std::numeric_limits<uintmax_t>::max());

    CHECK((uintmax_t)output == std::numeric_limits<uintmax_t>::max());
}

// only inlude these tests if we have library support for constexpr vector
#ifdef __cpp_lib_constexpr_vector
constexpr uintmax_t use_uint() {
    return (uintmax_t)arby::Uint();
}

constexpr uintmax_t use_uint(std::size_t) {
    return (uintmax_t)arby::Uint(0);
}

TEST_CASE("constexpr arby::Uint()") {
    STATIC_CHECK(use_uint() == 0);
}

TEST_CASE("constexpr arby::Uint(0)") {
    STATIC_CHECK(use_uint(0) == 0);
}
#endif

TEST_CASE("arby::Uint prefix increment") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max() - 1)));

    arby::Uint original = input;
    arby::Uint changed = ++original;

    CHECK((uintmax_t)original == input + 1);
    CHECK((uintmax_t)changed == input + 1);
}

TEST_CASE("arby::Uint postfix increment") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max() - 1)));

    arby::Uint original = input;
    arby::Uint previous = original++;

    CHECK((uintmax_t)original == input + 1);
    CHECK((uintmax_t)previous == input);
}

TEST_CASE("arby::Uint prefix decrement") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    arby::Uint original = input;
    arby::Uint changed = --original;

    CHECK((uintmax_t)original == input - 1);
    CHECK((uintmax_t)changed == input - 1);
}

TEST_CASE("arby::Uint postfix decrement") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    arby::Uint original = input;
    arby::Uint previous = original--;

    CHECK((uintmax_t)original == input - 1);
    CHECK((uintmax_t)previous == input);
}

// NOTE: no need for increment overflow tests as Uint doesn't overflow --it expands as necessary
// TODO: consider writing a test case that applies very large size-increasing operations (maybe exponents)
// on Uint to force a failure condition when memory runs out an an exception is thrown.

TEST_CASE("arby::Uint prefix decrement underflow") {
    arby::Uint input = 0;
    // underflow should throw an exception
    CHECK_THROWS_AS(--input, std::underflow_error);
}

TEST_CASE("arby::Uint postfix decrement underflow") {
    arby::Uint input = 0;
    // underflow should throw an exception
    CHECK_THROWS_AS(input--, std::underflow_error);
}

TEST_CASE("arby::Uint three-way-comparison with arby::Uint") {
    auto values = GENERATE(take(1000, chunk(2, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max()))));
    arby::Uint lhs = values[0];
    arby::Uint rhs = values[1];
    // determine the ordering between the two raw values
    std::strong_ordering comparison = values[0] <=> values[1];

    // the two Uint instances should have the same three-way-comparison
    CHECK((lhs <=> rhs) == comparison);
}
