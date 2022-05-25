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
