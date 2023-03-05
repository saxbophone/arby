#include <cmath>
#include <cstdint>

#include <limits>
#include <stdexcept>
#include <utility>

#include <catch2/catch.hpp>

#include <arby/Interval.hpp>
#include <arby/Nat.hpp>

using namespace com::saxbophone;
using namespace com::saxbophone::arby::literals;

TEST_CASE("arby::ilog(base<2, nonzero) throws std::domain_error", "[math-support][ilog]") {
    arby::Nat base = GENERATE(0_nat, 1_nat);
    CHECK_THROWS_AS(arby::ilog(base, 1), std::domain_error);
}

TEST_CASE("arby::ilog(base>1, 0) throws std::domain_error", "[math-support][ilog]") {
    CHECK_THROWS_AS(arby::ilog(2_nat, 0_nat), std::domain_error);
}

// uses the C++ stdlib to take a short-cut in calculating expected results in tests
static std::pair<uintmax_t, uintmax_t> integer_log(uintmax_t base, uintmax_t x) {
    // use the log/log trick to get any base log
    long double l = std::log(x) / std::log(base);
    // take the floor and ceiling, cast to integer
    return {
        (uintmax_t)std::floor(l), (uintmax_t)std::ceil(l)
    };
}

TEST_CASE("arby::ilog(base>1, nonzero) with small base", "[math-support][ilog]") {
    // don't use a base that's too small to get a variety of results
    uintmax_t base = GENERATE(take(100, random((uintmax_t)2, (uintmax_t)1000)));
    uintmax_t x = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    auto [expected_floor, expected_ceil] = integer_log(base, x);

    auto [result_floor, result_ceil] = arby::ilog(base, x);

    CAPTURE(base, x);
    CHECK(result_floor == expected_floor);
    CHECK(result_ceil == expected_ceil);
}

TEST_CASE("arby::ilog(base>1, nonzero)", "[math-support][ilog]") {
    uintmax_t base = GENERATE(take(100, random((uintmax_t)2, std::numeric_limits<uintmax_t>::max())));
    uintmax_t x = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    auto [expected_floor, expected_ceil] = integer_log(base, x);

    auto [result_floor, result_ceil] = arby::ilog(base, x);

    CAPTURE(base, x);
    CHECK(result_floor == expected_floor);
    CHECK(result_ceil == expected_ceil);
}

TEST_CASE("arby::ilog() with hardcoded values", "[math-support][ilog]") {
    auto [base, x, expected_floor, expected_ceil] = GENERATE(
        table<uintmax_t, uintmax_t, uintmax_t, uintmax_t>(
            {
                {2, 1, 0, 0},
                {3, 10, 2, 3},
                {4, 5, 1, 2},
                {33, 10, 0, 1},
                {2068, 1, 0, 0},
                {2068, 2, 0, 1},
                {10, 317, 2, 3},
                {3, 317, 5, 6}
            }
        )
    );

    auto [result_floor, result_ceil] = arby::ilog(base, x);

    CAPTURE(base, x);
    CHECK(result_floor == expected_floor);
    CHECK(result_ceil == expected_ceil);
}

// regression tests for base-2 log, because this is special-cased for performance reasons
TEST_CASE("arby::ilog(2, x) regression test", "[math-support][ilog]") {
    uintmax_t x = GENERATE(take(100, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    auto real_log = std::log2(x);
    arby::Interval<uintmax_t> expected(std::floor(real_log), std::ceil(real_log));

    auto result = arby::ilog(2, x);

    CHECK(result == expected);
}
