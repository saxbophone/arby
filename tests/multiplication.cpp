#include <cmath>
#include <limits>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone;

TEST_CASE("arby::Uint assignment-multiplication by arby::Uint(0)", "[multiplication]") {
    SECTION("0 *= 0") {
        arby::Uint multiplier = 0;
        const arby::Uint multiplicand = 0;

        multiplier *= multiplicand;

        CHECK((uintmax_t)multiplier == 0);
    }
    SECTION("0 *= random") {
        arby::Uint multiplier = 0;
        const arby::Uint multiplicand = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

        multiplier *= multiplicand;

        CHECK((uintmax_t)multiplier == 0);
    }
    SECTION("random *= 0") {
        arby::Uint multiplier = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
        const arby::Uint multiplicand = 0;

        multiplier *= multiplicand;

        CHECK((uintmax_t)multiplier == 0);
    }
}

TEST_CASE("arby::Uint multiplication by arby::Uint(0)", "[multiplication]") {
    SECTION("0 * 0") {
        arby::Uint multiplier = 0;
        arby::Uint multiplicand = 0;

        arby::Uint product = multiplier * multiplicand;

        CHECK((uintmax_t)product == 0);
    }
    SECTION("0 * random") {
        arby::Uint multiplier = 0;
        arby::Uint multiplicand = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

        arby::Uint product = multiplier * multiplicand;

        CHECK((uintmax_t)product == 0);
    }
    SECTION("random * 0") {
        arby::Uint multiplier = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
        arby::Uint multiplicand = 0;

        arby::Uint product = multiplier * multiplicand;

        CHECK((uintmax_t)product == 0);
    }
}

TEST_CASE("arby::Uint assignment-multiplication by arby::Uint", "[multiplication]") {
    // maximum factor value, to ensure we don't overflow uintmax_t so we can measure the result
    const uintmax_t MAX = std::sqrt(std::numeric_limits<uintmax_t>::max());
    uintmax_t multiplier = GENERATE(take(100, random((uintmax_t)0, MAX)));
    uintmax_t multiplicand = GENERATE(take(100, random((uintmax_t)0, MAX)));
    arby::Uint lhs = multiplier;
    arby::Uint rhs = multiplicand;
    // expected value
    multiplier *= multiplicand;

    lhs *= rhs;

    CHECK((uintmax_t)lhs == multiplier);
}

TEST_CASE("arby::Uint multiplication by arby::Uint", "[multiplication]") {
    // maximum factor value, to ensure we don't overflow uintmax_t so we can measure the result
    const uintmax_t MAX = std::sqrt(std::numeric_limits<uintmax_t>::max());
    uintmax_t multiplier = GENERATE(take(100, random((uintmax_t)0, MAX)));
    uintmax_t multiplicand = GENERATE(take(100, random((uintmax_t)0, MAX)));
    arby::Uint lhs = multiplier;
    arby::Uint rhs = multiplicand;
    // expected value
    uintmax_t product = multiplier * multiplicand;

    CHECK((uintmax_t)(lhs * rhs) == product);
}
