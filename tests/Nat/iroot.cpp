#include <cmath>
#include <cstdint>

#include <limits>
#include <stdexcept>

#include <catch2/catch.hpp>

#include <arby/Interval.hpp>
#include <arby/Nat.hpp>

using namespace com::saxbophone;

TEST_CASE("0th root of any arby::Nat value raises domain_error") {
    uintmax_t base = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    CHECK_THROWS_AS(arby::iroot(0, base), std::domain_error);
}

TEST_CASE("Integer root of arby::Nat of value 0 or 1 always returns same value regardless of the root", "[math-support][iroot]") {
    uintmax_t exponent = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    CHECK(arby::iroot(exponent, 0) == arby::Interval<arby::Nat>(0));
    CHECK(arby::iroot(exponent, 1) == arby::Interval<arby::Nat>(1));
}

TEST_CASE("1th root of any arby::Nat value always returns same value", "[math-support][iroot]") {
    uintmax_t base = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    CHECK(arby::iroot(1, base) == arby::Interval<arby::Nat>(base));
}

TEST_CASE("Integer root of perfect square/cube/n-power arby::Nat", "[math-support][iroot]") {
    uintmax_t base = GENERATE(take(100, random((uintmax_t)2, (uintmax_t)100)));
    // XXX: exponent needs to be highly constrained to avoid calculations taking excessive time or memory overflow
    uintmax_t exponent = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)100)));
    auto perfect_power = arby::ipow(base, exponent);
    arby::Interval<arby::Nat> expected(base);

    auto root = arby::iroot(exponent, perfect_power);

    CAPTURE(base, exponent, root.floor, root.ceil, expected.floor, expected.ceil);

    CHECK(root == expected);
}

TEST_CASE("arby::Nat Integer root of arbitrary base and exponents gives floor and ceil of Real answer", "[math-support][iroot]") {
    uintmax_t base = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t exponent = GENERATE((uintmax_t)2, (uintmax_t)100);
    double real_root = std::pow(base, 1.0 / exponent);
    arby::Interval<arby::Nat> expected(arby::Nat::from_float(std::floor(real_root)), arby::Nat::from_float(std::ceil(real_root)));

    auto root = arby::iroot(exponent, base);

    CAPTURE(base, exponent, root.floor, root.ceil, expected.floor, expected.ceil);

    CHECK(root == expected);
}
