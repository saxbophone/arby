#include <cmath>
#include <limits>

#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

using namespace com::saxbophone;

TEST_CASE("arby::Nat assignment-multiplication by arby::Nat(0)", "[multiplication]") {
    SECTION("0 *= 0") {
        arby::Nat multiplier = 0;
        const arby::Nat multiplicand = 0;

        multiplier *= multiplicand;

        CHECK((uintmax_t)multiplier == 0);
    }
    SECTION("0 *= random") {
        arby::Nat multiplier = 0;
        const arby::Nat multiplicand = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

        multiplier *= multiplicand;

        CHECK((uintmax_t)multiplier == 0);
    }
    SECTION("random *= 0") {
        arby::Nat multiplier = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
        const arby::Nat multiplicand = 0;

        multiplier *= multiplicand;

        CHECK((uintmax_t)multiplier == 0);
    }
}

TEST_CASE("arby::Nat multiplication by arby::Nat(0)", "[multiplication]") {
    SECTION("0 * 0") {
        arby::Nat multiplier = 0;
        arby::Nat multiplicand = 0;

        arby::Nat product = multiplier * multiplicand;

        CHECK((uintmax_t)product == 0);
    }
    SECTION("0 * random") {
        arby::Nat multiplier = 0;
        arby::Nat multiplicand = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

        arby::Nat product = multiplier * multiplicand;

        CHECK((uintmax_t)product == 0);
    }
    SECTION("random * 0") {
        arby::Nat multiplier = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
        arby::Nat multiplicand = 0;

        arby::Nat product = multiplier * multiplicand;

        CHECK((uintmax_t)product == 0);
    }
}

TEST_CASE("arby::Nat assignment-multiplication by arby::Nat", "[multiplication]") {
    // maximum factor value, to ensure we don't overflow uintmax_t so we can measure the result
    const uintmax_t MAX = (uintmax_t)std::sqrt(std::numeric_limits<uintmax_t>::max());
    uintmax_t multiplier = GENERATE_COPY(take(100, random((uintmax_t)0, MAX)));
    uintmax_t multiplicand = GENERATE_COPY(take(100, random((uintmax_t)0, MAX)));
    arby::Nat lhs = multiplier;
    arby::Nat rhs = multiplicand;
    // expected value
    multiplier *= multiplicand;

    lhs *= rhs;

    CHECK((uintmax_t)lhs == multiplier);
}

TEST_CASE("arby::Nat multiplication by arby::Nat", "[multiplication]") {
    // maximum factor value, to ensure we don't overflow uintmax_t so we can measure the result
    const uintmax_t MAX = (uintmax_t)std::sqrt(std::numeric_limits<uintmax_t>::max());
    uintmax_t multiplier = GENERATE_COPY(take(100, random((uintmax_t)0, MAX)));
    uintmax_t multiplicand = GENERATE_COPY(take(100, random((uintmax_t)0, MAX)));
    arby::Nat lhs = multiplier;
    arby::Nat rhs = multiplicand;
    // expected value
    uintmax_t product = multiplier * multiplicand;

    CHECK((uintmax_t)(lhs * rhs) == product);
}

// regression tests for multiplying by powers of two

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

TEST_CASE("multiply arby::Nat by a power of two", "[multiplication]") {
    uintmax_t power = GENERATE(range((uintmax_t)0, (uintmax_t)std::numeric_limits<uintmax_t>::digits / 2));
    uintmax_t rhs = integer_pow(2, power);
    uintmax_t lhs = GENERATE_COPY(take(100, random((uintmax_t)0, rhs)));

    auto product = arby::Nat(lhs) * arby::Nat(rhs);

    CHECK(product == lhs * rhs);
}
