#include <compare>
#include <limits>
#include <stdexcept>
#include <string>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone;

TEST_CASE("std::numeric_limits<arby::Uint>", "[numeric-limits]") {
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

TEST_CASE("arby::Uint(uintmax_t random) and (uintmax_t)arby::Uint", "[ctor]") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    arby::Uint output(input);

    CHECK((uintmax_t)output == input);
}

TEST_CASE("arby::Uint(0) and (uintmax_t)arby::Uint", "[ctor]") {
    arby::Uint output(0);

    CHECK((uintmax_t)output == 0);
}

TEST_CASE("arby::Uint(UINT_MAX) and (uintmax_t)arby::Uint", "[ctor]") {
    arby::Uint output(std::numeric_limits<uintmax_t>::max());

    CHECK((uintmax_t)output == std::numeric_limits<uintmax_t>::max());
}

TEST_CASE("arby::Uint(std::string)", "[ctor]") {
    uintmax_t value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    std::string string = std::to_string(value);

    arby::Uint object = string;

    CHECK((uintmax_t)object == value);
}

TEST_CASE("constexpr arby::Uint", "[constexpr]") {
    WARN("No constexpr tests written yet");
}

TEST_CASE("arby::Uint prefix increment", "[basic-arithmetic]") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max() - 1)));

    arby::Uint original = input;
    arby::Uint changed = ++original;

    CHECK((uintmax_t)original == input + 1);
    CHECK((uintmax_t)changed == input + 1);
}

TEST_CASE("arby::Uint postfix increment", "[basic-arithmetic]") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max() - 1)));

    arby::Uint original = input;
    arby::Uint previous = original++;

    CHECK((uintmax_t)original == input + 1);
    CHECK((uintmax_t)previous == input);
}

TEST_CASE("arby::Uint prefix decrement", "[basic-arithmetic]") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    arby::Uint original = input;
    arby::Uint changed = --original;

    CHECK((uintmax_t)original == input - 1);
    CHECK((uintmax_t)changed == input - 1);
}

TEST_CASE("arby::Uint postfix decrement", "[basic-arithmetic]") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    arby::Uint original = input;
    arby::Uint previous = original--;

    CHECK((uintmax_t)original == input - 1);
    CHECK((uintmax_t)previous == input);
}

// NOTE: no need for increment overflow tests as Uint doesn't overflow --it expands as necessary
// TODO: consider writing a test case that applies very large size-increasing operations (maybe exponents)
// on Uint to force a failure condition when memory runs out an an exception is thrown.

TEST_CASE("arby::Uint prefix decrement underflow", "[basic-arithmetic]") {
    arby::Uint input = 0;
    // underflow should throw an exception
    CHECK_THROWS_AS(--input, std::underflow_error);
}

TEST_CASE("arby::Uint postfix decrement underflow", "[basic-arithmetic]") {
    arby::Uint input = 0;
    // underflow should throw an exception
    CHECK_THROWS_AS(input--, std::underflow_error);
}

TEST_CASE("arby::Uint three-way-comparison with arby::Uint using known values", "[basic-arithmetic]") {
    auto values = GENERATE(
        table<uintmax_t, uintmax_t, std::strong_ordering>(
            {
                {0, 0, std::strong_ordering::equal},
                {1, 0, std::strong_ordering::greater},
                {0, 1, std::strong_ordering::less},
                {27, 19, std::strong_ordering::greater},
                {2, 87, std::strong_ordering::less},
                {3, 5, std::strong_ordering::less},
                {5, 3, std::strong_ordering::greater},
                {261, 261, std::strong_ordering::equal},
                // now for some other much larger values guaranteed to pass a digit-boundary
                {arby::Uint::BASE + 1, arby::Uint::BASE + 1, std::strong_ordering::equal},
                {arby::Uint::BASE + 20, arby::Uint::BASE + 19, std::strong_ordering::greater},
                {arby::Uint::BASE + 33, arby::Uint::BASE + 62, std::strong_ordering::less},
            }
        )
    );
    arby::Uint lhs = std::get<0>(values);
    arby::Uint rhs = std::get<1>(values);

    // the two Uint instances should have the same three-way-comparison
    CHECK((lhs <=> rhs) == std::get<2>(values));
}

TEST_CASE("arby::Uint three-way-comparison with arby::Uint using random values", "[basic-arithmetic]") {
    auto values = GENERATE(take(1000, chunk(2, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max()))));
    arby::Uint lhs = values[0];
    arby::Uint rhs = values[1];
    // determine the ordering between the two raw values
    std::strong_ordering comparison = values[0] <=> values[1];

    // the two Uint instances should have the same three-way-comparison
    CHECK((lhs <=> rhs) == comparison);
}

TEST_CASE("Assignment-addition of arby::Uint to arby::Uint", "[basic-arithmetic]") {
    // choose values in the range half of max uint so as to be sure the result is representable as uint for comparison purposes
    auto values = GENERATE(take(1000, chunk(2, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max() / 2))));
    arby::Uint lhs = values[0];
    const arby::Uint rhs = values[1];
    // determine what the result of addition between the raw values should be
    values[0] += values[1];

    // do the assignment-addition
    lhs += rhs;

    // check the result
    CHECK((uintmax_t)lhs == values[0]);
}

TEST_CASE("Addition of arby::Uint and arby::Uint", "[basic-arithmetic]") {
    // choose values in the range half of max uint so as to be sure the result is representable as uint for comparison purposes
    auto values = GENERATE(take(1000, chunk(2, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max() / 2))));
    arby::Uint lhs = values[0];
    arby::Uint rhs = values[1];
    // determine what the result of addition between the raw values should be
    uintmax_t addition = values[0] + values[1];

    // do the addition
    arby::Uint result = lhs + rhs;

    // check the result
    CHECK((uintmax_t)result == addition);
}

TEST_CASE("Assignment-addition of much smaller arby::Uint to arby::Uint", "[basic-arithmetic]") {
    // choose values in the range half of max uint so as to be sure the result is representable as uint for comparison purposes
    auto bigger = GENERATE(take(100, random((uintmax_t)0, (uintmax_t)std::numeric_limits<unsigned int>::max() / 2)));
    auto smaller = GENERATE(take(100, random((uintmax_t)0, (uintmax_t)arby::Uint::BASE)));
    arby::Uint lhs = bigger;
    const arby::Uint rhs = smaller;
    // determine what the result of addition between the raw values should be
    bigger += smaller;

    // do the assignment-addition
    lhs += rhs;

    // check the result
    CHECK((uintmax_t)lhs == bigger);
}

TEST_CASE("Addition of arby::Uint and much smaller arby::Uint", "[basic-arithmetic]") {
    // choose values in the range half of max uint so as to be sure the result is representable as uint for comparison purposes
    auto bigger = GENERATE(take(100, random((uintmax_t)0, (uintmax_t)std::numeric_limits<unsigned int>::max() / 2)));
    auto smaller = GENERATE(take(100, random((uintmax_t)0, (uintmax_t)arby::Uint::BASE)));
    CAPTURE(bigger, smaller);
    arby::Uint lhs = bigger;
    arby::Uint rhs = smaller;
    // determine what the result of addition between the raw values should be
    uintmax_t addition = bigger + smaller;

    // do the addition
    arby::Uint result = lhs + rhs;

    // check the result
    CHECK((uintmax_t)result == addition);
}

TEST_CASE("Assignment-subtraction of arby::Uint from arby::Uint", "[basic-arithmetic]") {
    auto minuend = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    // ensure subtrahend is never bigger than minuend so we don't underflow
    auto subtrahend = GENERATE_COPY(take(100, random((uintmax_t)1, minuend)));
    arby::Uint lhs = minuend;
    const arby::Uint rhs = subtrahend;
    // determine what the result of subtraction between the raw values should be
    minuend -= subtrahend;

    // do the assignment-subtraction
    lhs -= rhs;

    // check the result
    CHECK((uintmax_t)lhs == minuend);
}

TEST_CASE("Subtraction of arby::Uint from arby::Uint", "[basic-arithmetic]") {
    auto minuend = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    // ensure subtrahend is never bigger than minuend so we don't underflow
    auto subtrahend = GENERATE_COPY(take(100, random((uintmax_t)1, minuend)));
    arby::Uint lhs = minuend;
    arby::Uint rhs = subtrahend;
    // determine what the result of subtraction between the raw values should be
    uintmax_t subtraction = minuend - subtrahend;

    // do the subtraction
    arby::Uint result = lhs - rhs;

    // check the result
    CHECK((uintmax_t)result == subtraction);
}

TEST_CASE("Assignment-subtraction of much smaller arby::Uint from arby::Uint", "[basic-arithmetic]") {
    auto minuend = GENERATE(take(100, random((uintmax_t)arby::Uint::BASE, std::numeric_limits<uintmax_t>::max())));
    // ensure subtrahend is never bigger than minuend so we don't underflow
    auto subtrahend = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Uint::BASE)));
    arby::Uint lhs = minuend;
    const arby::Uint rhs = subtrahend;
    // determine what the result of subtraction between the raw values should be
   minuend -= subtrahend;

    // do the assignment-subtraction
    lhs -= rhs;

    // check the result
    CHECK((uintmax_t)lhs == minuend);
}

TEST_CASE("Subtraction of arby::Uint from much smaller arby::Uint", "[basic-arithmetic]") {
    auto minuend = GENERATE(take(100, random((uintmax_t)arby::Uint::BASE, std::numeric_limits<uintmax_t>::max())));
    // ensure subtrahend is never bigger than minuend so we don't underflow
    auto subtrahend = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Uint::BASE)));
    arby::Uint lhs = minuend;
    arby::Uint rhs = subtrahend;
    // determine what the result of subtraction between the raw values should be
    uintmax_t subtraction = minuend - subtrahend;

    // do the subtraction
    arby::Uint result = lhs - rhs;

    // check the result
    CHECK((uintmax_t)result == subtraction);
}

TEST_CASE("Attempt at non-zero assignment-subtraction from arby::Uint(0) raises underflow_error", "[basic-arithmetic]") {
    arby::Uint zero;
    auto subtrahend = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Uint rhs = subtrahend;

    CHECK_THROWS_AS(zero -= rhs, std::underflow_error);
}

TEST_CASE("Attempt at non-zero subtraction from arby::Uint(0) raises underflow_error", "[basic-arithmetic]") {
    arby::Uint zero;
    auto subtrahend = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Uint rhs = subtrahend;

    CHECK_THROWS_AS(zero - rhs, std::underflow_error);
}

TEST_CASE("Assignment-subtraction of arby::Uint(0) from arby::Uint(0)", "[basic-arithmetic]") {
    arby::Uint lhs, rhs;
    REQUIRE((uintmax_t)lhs == 0);
    REQUIRE((uintmax_t)rhs == 0);

    lhs -= rhs;

    CHECK((uintmax_t)lhs == 0);
}

TEST_CASE("Subtraction of arby::Uint(0) from arby::Uint(0)", "[basic-arithmetic]") {
    arby::Uint lhs, rhs;
    REQUIRE((uintmax_t)lhs == 0);
    REQUIRE((uintmax_t)rhs == 0);

    CHECK((uintmax_t)(lhs - rhs) == 0);
}
