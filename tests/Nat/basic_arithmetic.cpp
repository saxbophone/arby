#include <compare>
#include <limits>
#include <stdexcept>
#include <string>

#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

using namespace com::saxbophone;

TEST_CASE("std::numeric_limits<arby::Nat>", "[numeric-limits]") {
    CHECK(std::numeric_limits<arby::Nat>::is_specialized);
    CHECK_FALSE(std::numeric_limits<arby::Nat>::is_signed);
    CHECK(std::numeric_limits<arby::Nat>::is_integer);
    CHECK(std::numeric_limits<arby::Nat>::is_exact);
    CHECK_FALSE(std::numeric_limits<arby::Nat>::has_infinity); // N/A
    CHECK_FALSE(std::numeric_limits<arby::Nat>::has_quiet_NaN); // N/A
    CHECK_FALSE(std::numeric_limits<arby::Nat>::has_signaling_NaN); // N/A
    CHECK_FALSE(std::numeric_limits<arby::Nat>::has_denorm); // N/A
    CHECK_FALSE(std::numeric_limits<arby::Nat>::has_denorm_loss); // N/A
    CHECK(std::numeric_limits<arby::Nat>::round_style == std::round_toward_zero);
    CHECK_FALSE(std::numeric_limits<arby::Nat>::is_iec559);
    CHECK_FALSE(std::numeric_limits<arby::Nat>::is_bounded);
    CHECK_FALSE(std::numeric_limits<arby::Nat>::is_modulo);
    CHECK(std::numeric_limits<arby::Nat>::digits == 0); // N/A
    CHECK(std::numeric_limits<arby::Nat>::digits10 == 0); // N/A
    CHECK(std::numeric_limits<arby::Nat>::max_digits10 == 0); // N/A
    // calculate the bit width of the next-lowest type, its place value is the radix
    CHECK(std::numeric_limits<arby::Nat>::radix == 1u << ((std::numeric_limits<int>::digits + 1) / 2));
    CHECK(std::numeric_limits<arby::Nat>::min_exponent == 0); // N/A
    CHECK(std::numeric_limits<arby::Nat>::min_exponent10 == 0); // N/A
    CHECK(std::numeric_limits<arby::Nat>::max_exponent == 0); // N/A
    CHECK(std::numeric_limits<arby::Nat>::max_exponent10 == 0); // N/A
    CHECK(std::numeric_limits<arby::Nat>::traps);
    CHECK_FALSE(std::numeric_limits<arby::Nat>::tinyness_before); // N/A
    // can't uncomment these tests until Nat has an equality operator and can be ctor() from int
    CHECK(std::numeric_limits<arby::Nat>::min() == 0);
    CHECK(std::numeric_limits<arby::Nat>::lowest() == 0);
    CHECK(std::numeric_limits<arby::Nat>::max() == 0); // N/A
    CHECK(std::numeric_limits<arby::Nat>::epsilon() == 0); // N/A
    CHECK(std::numeric_limits<arby::Nat>::round_error() == 0); // N/A
    CHECK(std::numeric_limits<arby::Nat>::infinity() == 0); // N/A
    CHECK(std::numeric_limits<arby::Nat>::quiet_NaN() == 0); // N/A
    CHECK(std::numeric_limits<arby::Nat>::signaling_NaN() == 0); // N/A
    CHECK(std::numeric_limits<arby::Nat>::denorm_min() == 0); // N/A
}

TEST_CASE("arby::Nat(uintmax_t random) and (uintmax_t)arby::Nat", "[ctor]") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    arby::Nat output(input);

    CHECK((uintmax_t)output == input);
}

TEST_CASE("arby::Nat(0) and (uintmax_t)arby::Nat", "[ctor]") {
    arby::Nat output(0);

    CHECK((uintmax_t)output == 0);
}

TEST_CASE("arby::Nat(UINT_MAX) and (uintmax_t)arby::Nat", "[ctor]") {
    arby::Nat output(std::numeric_limits<uintmax_t>::max());

    CHECK((uintmax_t)output == std::numeric_limits<uintmax_t>::max());
}

TEST_CASE("arby::Nat(std::string)", "[ctor]") {
    uintmax_t value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    std::string string = std::to_string(value);

    arby::Nat object = string;

    CHECK((uintmax_t)object == value);
}

TEST_CASE("constexpr arby::Nat", "[constexpr]") {
    WARN("No constexpr tests written yet");
}

TEST_CASE("arby::Nat prefix increment 0", "[basic-arithmetic]") {
    arby::Nat original;
    arby::Nat changed = ++original;

    CHECK((uintmax_t)original == 1);
    CHECK((uintmax_t)changed == 1);
}

TEST_CASE("arby::Nat postfix increment 0", "[basic-arithmetic]") {
    arby::Nat original;
    arby::Nat previous = original++;

    CHECK((uintmax_t)original == 1);
    CHECK((uintmax_t)previous == 0);
}

TEST_CASE("arby::Nat prefix increment", "[basic-arithmetic]") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max() - 1)));

    arby::Nat original = input;
    arby::Nat changed = ++original;

    CHECK((uintmax_t)original == input + 1);
    CHECK((uintmax_t)changed == input + 1);
}

TEST_CASE("arby::Nat postfix increment", "[basic-arithmetic]") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max() - 1)));

    arby::Nat original = input;
    arby::Nat previous = original++;

    CHECK((uintmax_t)original == input + 1);
    CHECK((uintmax_t)previous == input);
}

TEST_CASE("arby::Nat prefix increment requiring additional digits", "[basic-arithmetic]") {
    // setting input value to BASE-1 means an increment will add another digit
    arby::Nat original = arby::Nat::BASE - 1;
    arby::Nat changed = ++original;

    CHECK((uintmax_t)original == arby::Nat::BASE);
    CHECK((uintmax_t)changed == arby::Nat::BASE);
}

TEST_CASE("arby::Nat postfix increment requiring additional digits", "[basic-arithmetic]") {
    // setting input value to BASE-1 means an increment will add another digit
    arby::Nat original = arby::Nat::BASE - 1;
    arby::Nat previous = original++;

    CHECK((uintmax_t)original == arby::Nat::BASE);
    CHECK((uintmax_t)previous == arby::Nat::BASE - 1);
}

TEST_CASE("arby::Nat decrement 1", "[basic-arithmetic]") {
    arby::Nat one = 1;

    SECTION("prefix") {
        CHECK(--one == 0);
    }
    SECTION("postfix") {
        CHECK(one-- == 1);
    }
}

TEST_CASE("arby::Nat prefix decrement", "[basic-arithmetic]") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    arby::Nat original = input;
    arby::Nat changed = --original;

    CHECK((uintmax_t)original == input - 1);
    CHECK((uintmax_t)changed == input - 1);
}

TEST_CASE("arby::Nat postfix decrement", "[basic-arithmetic]") {
    uintmax_t input = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    arby::Nat original = input;
    arby::Nat previous = original--;

    CHECK((uintmax_t)original == input - 1);
    CHECK((uintmax_t)previous == input);
}

TEST_CASE("arby::Nat prefix decrement requiring digit removal", "[basic-arithmetic]") {
    // setting input value to BASE means a decrement will remove a digit
    arby::Nat original = arby::Nat::BASE;
    arby::Nat changed = --original;

    CHECK((uintmax_t)original == arby::Nat::BASE - 1);
    CHECK((uintmax_t)changed == arby::Nat::BASE - 1);
}

TEST_CASE("arby::Nat postfix decrement requiring digit removal", "[basic-arithmetic]") {
    // setting input value to BASE means a decrement will remove a digit
    arby::Nat original = arby::Nat::BASE;
    arby::Nat previous = original--;

    CHECK((uintmax_t)original == arby::Nat::BASE - 1);
    CHECK((uintmax_t)previous == arby::Nat::BASE);
}

// NOTE: no need for increment overflow tests as Nat doesn't overflow --it expands as necessary
// TODO: consider writing a test case that applies very large size-increasing operations (maybe exponents)
// on Nat to force a failure condition when memory runs out an an exception is thrown.

TEST_CASE("arby::Nat prefix decrement underflow", "[basic-arithmetic]") {
    arby::Nat input = 0;
    // underflow should throw an exception
    CHECK_THROWS_AS(--input, std::underflow_error);
}

TEST_CASE("arby::Nat postfix decrement underflow", "[basic-arithmetic]") {
    arby::Nat input = 0;
    // underflow should throw an exception
    CHECK_THROWS_AS(input--, std::underflow_error);
}

TEST_CASE("arby::Nat three-way-comparison with arby::Nat using known values", "[basic-arithmetic]") {
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
                {arby::Nat::BASE + 1, arby::Nat::BASE + 1, std::strong_ordering::equal},
                {arby::Nat::BASE + 20, arby::Nat::BASE + 19, std::strong_ordering::greater},
                {arby::Nat::BASE + 33, arby::Nat::BASE + 62, std::strong_ordering::less},
            }
        )
    );
    arby::Nat lhs = std::get<0>(values);
    arby::Nat rhs = std::get<1>(values);

    // the two Nat instances should have the same three-way-comparison
    CHECK((lhs <=> rhs) == std::get<2>(values));
}

TEST_CASE("arby::Nat three-way-comparison with arby::Nat using random values", "[basic-arithmetic]") {
    auto values = GENERATE(take(1000, chunk(2, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max()))));
    arby::Nat lhs = values[0];
    arby::Nat rhs = values[1];
    // determine the ordering between the two raw values
    std::strong_ordering comparison = values[0] <=> values[1];

    // the two Nat instances should have the same three-way-comparison
    CHECK((lhs <=> rhs) == comparison);
}

TEST_CASE("Assignment-addition of arby::Nat to arby::Nat", "[basic-arithmetic]") {
    // choose values in the range half of max uint so as to be sure the result is representable as uint for comparison purposes
    auto values = GENERATE(take(1000, chunk(2, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max() / 2))));
    arby::Nat lhs = values[0];
    const arby::Nat rhs = values[1];
    // determine what the result of addition between the raw values should be
    values[0] += values[1];

    // do the assignment-addition
    lhs += rhs;

    // check the result
    CHECK((uintmax_t)lhs == values[0]);
}

TEST_CASE("Addition of arby::Nat and arby::Nat", "[basic-arithmetic]") {
    // choose values in the range half of max uint so as to be sure the result is representable as uint for comparison purposes
    auto values = GENERATE(take(1000, chunk(2, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max() / 2))));
    arby::Nat lhs = values[0];
    arby::Nat rhs = values[1];
    // determine what the result of addition between the raw values should be
    uintmax_t addition = values[0] + values[1];

    // do the addition
    arby::Nat result = lhs + rhs;

    // check the result
    CHECK((uintmax_t)result == addition);
}

TEST_CASE("Assignment-addition of much smaller arby::Nat to arby::Nat", "[basic-arithmetic]") {
    // choose values in the range half of max uint so as to be sure the result is representable as uint for comparison purposes
    auto bigger = GENERATE(take(100, random((uintmax_t)0, (uintmax_t)std::numeric_limits<unsigned int>::max() / 2)));
    auto smaller = GENERATE(take(100, random((uintmax_t)0, (uintmax_t)arby::Nat::BASE)));
    arby::Nat lhs = bigger;
    const arby::Nat rhs = smaller;
    // determine what the result of addition between the raw values should be
    bigger += smaller;

    // do the assignment-addition
    lhs += rhs;

    // check the result
    CHECK((uintmax_t)lhs == bigger);
}

TEST_CASE("Addition of arby::Nat and much smaller arby::Nat", "[basic-arithmetic]") {
    // choose values in the range half of max uint so as to be sure the result is representable as uint for comparison purposes
    auto bigger = GENERATE(take(100, random((uintmax_t)0, (uintmax_t)std::numeric_limits<unsigned int>::max() / 2)));
    auto smaller = GENERATE(take(100, random((uintmax_t)0, (uintmax_t)arby::Nat::BASE)));
    CAPTURE(bigger, smaller);
    arby::Nat lhs = bigger;
    arby::Nat rhs = smaller;
    // determine what the result of addition between the raw values should be
    uintmax_t addition = bigger + smaller;

    // do the addition
    arby::Nat result = lhs + rhs;

    // check the result
    CHECK((uintmax_t)result == addition);
}

TEST_CASE("arby::Nat + 0") {
    arby::Nat value = GENERATE((uintmax_t)0, std::numeric_limits<uintmax_t>::max());

    CHECK((value + 0) == value);
}

TEST_CASE("arby::Nat += 0") {
    uintmax_t value = GENERATE((uintmax_t)0, std::numeric_limits<uintmax_t>::max());
    arby::Nat object = value;

    object += 0;

    CHECK((uintmax_t)object == value);
}

TEST_CASE("Assignment-subtraction of arby::Nat from arby::Nat", "[basic-arithmetic]") {
    auto minuend = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    // ensure subtrahend is never bigger than minuend so we don't underflow
    auto subtrahend = GENERATE_COPY(take(100, random((uintmax_t)1, minuend)));
    arby::Nat lhs = minuend;
    const arby::Nat rhs = subtrahend;
    // determine what the result of subtraction between the raw values should be
    minuend -= subtrahend;

    // do the assignment-subtraction
    lhs -= rhs;

    // check the result
    CHECK((uintmax_t)lhs == minuend);
}

TEST_CASE("Subtraction of arby::Nat from arby::Nat", "[basic-arithmetic]") {
    auto minuend = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    // ensure subtrahend is never bigger than minuend so we don't underflow
    auto subtrahend = GENERATE_COPY(take(100, random((uintmax_t)1, minuend)));
    arby::Nat lhs = minuend;
    arby::Nat rhs = subtrahend;
    // determine what the result of subtraction between the raw values should be
    uintmax_t subtraction = minuend - subtrahend;

    // do the subtraction
    arby::Nat result = lhs - rhs;

    // check the result
    CHECK((uintmax_t)result == subtraction);
}

TEST_CASE("Assignment-subtraction of much smaller arby::Nat from arby::Nat", "[basic-arithmetic]") {
    auto minuend = GENERATE(take(100, random((uintmax_t)arby::Nat::BASE, std::numeric_limits<uintmax_t>::max())));
    // ensure subtrahend is never bigger than minuend so we don't underflow
    auto subtrahend = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Nat::BASE)));
    arby::Nat lhs = minuend;
    const arby::Nat rhs = subtrahend;
    // determine what the result of subtraction between the raw values should be
   minuend -= subtrahend;

    // do the assignment-subtraction
    lhs -= rhs;

    // check the result
    CHECK((uintmax_t)lhs == minuend);
}

TEST_CASE("Subtraction of arby::Nat from much smaller arby::Nat", "[basic-arithmetic]") {
    auto minuend = GENERATE(take(100, random((uintmax_t)arby::Nat::BASE, std::numeric_limits<uintmax_t>::max())));
    // ensure subtrahend is never bigger than minuend so we don't underflow
    auto subtrahend = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Nat::BASE)));
    arby::Nat lhs = minuend;
    arby::Nat rhs = subtrahend;
    // determine what the result of subtraction between the raw values should be
    uintmax_t subtraction = minuend - subtrahend;

    // do the subtraction
    arby::Nat result = lhs - rhs;

    // check the result
    CHECK((uintmax_t)result == subtraction);
}

TEST_CASE("Attempt at non-zero assignment-subtraction from arby::Nat(0) raises underflow_error", "[basic-arithmetic]") {
    arby::Nat zero;
    auto subtrahend = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Nat rhs = subtrahend;

    CHECK_THROWS_AS(zero -= rhs, std::underflow_error);
}

TEST_CASE("Attempt at non-zero subtraction from arby::Nat(0) raises underflow_error", "[basic-arithmetic]") {
    arby::Nat zero;
    auto subtrahend = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Nat rhs = subtrahend;

    CHECK_THROWS_AS(zero - rhs, std::underflow_error);
}

TEST_CASE("Assignment-subtraction of arby::Nat(0) from arby::Nat(0)", "[basic-arithmetic]") {
    arby::Nat lhs, rhs;
    REQUIRE((uintmax_t)lhs == 0);
    REQUIRE((uintmax_t)rhs == 0);

    lhs -= rhs;

    CHECK((uintmax_t)lhs == 0);
}

TEST_CASE("Subtraction of arby::Nat(0) from arby::Nat(0)", "[basic-arithmetic]") {
    arby::Nat lhs, rhs;
    REQUIRE((uintmax_t)lhs == 0);
    REQUIRE((uintmax_t)rhs == 0);

    CHECK((uintmax_t)(lhs - rhs) == 0);
}

TEST_CASE("arby::Nat - 0") {
    arby::Nat value = GENERATE((uintmax_t)0, std::numeric_limits<uintmax_t>::max());

    CHECK((value - 0) == value);
}

TEST_CASE("arby::Nat -= 0") {
    uintmax_t value = GENERATE((uintmax_t)0, std::numeric_limits<uintmax_t>::max());
    arby::Nat object = value;

    object -= 0;

    CHECK((uintmax_t)object == value);
}

TEST_CASE("arby::Nat subtraction resulting in zero") {
    arby::Nat value = GENERATE(1u, 1000u, std::numeric_limits<uintmax_t>::max());

    SECTION("assignment-subtraction") {
        value -= value;
        CHECK(value == 0);
    }
    SECTION("subtraction") {
        CHECK(value - value == 0);
    }
}
