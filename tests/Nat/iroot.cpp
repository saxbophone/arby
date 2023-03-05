#include <cmath>
#include <cstdint>

#include <limits>

#include <catch2/catch.hpp>

#include <arby/Interval.hpp>
#include <arby/Nat.hpp>

using namespace com::saxbophone;

// std::pow() is not accurate for large powers and we need exactness
// TODO: put this in a helper function accessible to all tests
static uintmax_t integer_pow(uintmax_t base, uintmax_t exponent) {
    // 1 to the power of anything is always 1
    if (base == 1) {
        return 1;
    }
    uintmax_t power = 1;
    for (uintmax_t i = 0; i < exponent; i++) {
        power *= base;
    }
    return power;
}

// TODO: test cases for any root of 0 or 1, always equalling 0 or 1

TEST_CASE("Integer root of perfect square/cube/n-power arby::Nat", "[math-support][iroot]") {
    uintmax_t base = GENERATE(take(100, random((uintmax_t)2, (uintmax_t)100)));
    // XXX: exponent needs to be highly constrained to avoid calculations taking excessive time or memory overflow
    uintmax_t exponent = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)100)));
    auto perfect_power = arby::ipow(base, exponent);
    arby::Interval<arby::Nat> expected(base);

    auto root = arby::iroot(exponent, perfect_power);

    CHECK(root == expected);
}
