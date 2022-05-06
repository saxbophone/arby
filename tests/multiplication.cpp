#include <limits>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone;

TEST_CASE("arby::Uint assignment-multiplication by arby::Uint(0)", "[multiplication]") {
    SECTION("0 *= 0") {
        arby::Uint multiplier = 0;
        arby::Uint multiplicand = 0;

        multiplier *= multiplicand;

        CHECK((uintmax_t)multiplier == 0);
    }
    SECTION("0 *= random") {
        arby::Uint multiplier = 0;
        arby::Uint multiplicand = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

        multiplier *= multiplicand;

        CHECK((uintmax_t)multiplier == 0);
    }
    SECTION("random *= 0") {
        arby::Uint multiplier = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
        arby::Uint multiplicand = 0;

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
