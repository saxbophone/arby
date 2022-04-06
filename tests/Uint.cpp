#include <limits>

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
    CHECK(std::numeric_limits<arby::Uint>::radix == 2); // should be UINT_MAX+1 but won't fit in an int!
    CHECK(std::numeric_limits<arby::Uint>::min_exponent == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::min_exponent10 == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::max_exponent == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::max_exponent10 == 0); // N/A
    CHECK(std::numeric_limits<arby::Uint>::traps);
    CHECK_FALSE(std::numeric_limits<arby::Uint>::tinyness_before); // N/A
    // can't uncomment these tests until Uint has an equality operator and can be ctor() from int
    // CHECK(std::numeric_limits<arby::Uint>::min() == 0);
    // CHECK(std::numeric_limits<arby::Uint>::lowest() == 0);
    // CHECK(std::numeric_limits<arby::Uint>::max() == 0); // N/A
    // CHECK(std::numeric_limits<arby::Uint>::epsilon() == 0); // N/A
    // CHECK(std::numeric_limits<arby::Uint>::round_error() == 0); // N/A
    // CHECK(std::numeric_limits<arby::Uint>::infinity() == 0); // N/A
    // CHECK(std::numeric_limits<arby::Uint>::quiet_NaN() == 0); // N/A
    // CHECK(std::numeric_limits<arby::Uint>::signaling_NaN() == 0); // N/A
    // CHECK(std::numeric_limits<arby::Uint>::denorm_min() == 0); // N/A
}
