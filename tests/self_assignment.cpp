#include <cmath>
#include <cstdint>

#include <limits>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone;

TEST_CASE("Addition self-assignment", "[self-assignment]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max() / 2)));
    arby::Uint arb = value;
    value += value;

    arb += arb;

    CHECK((uintmax_t)arb == value);
}

TEST_CASE("Subtraction self-assignment", "[self-assignment]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Uint arb = value;

    arb -= arb;

    CHECK((uintmax_t)arb == 0);
}

TEST_CASE("Multiplication self-assignment", "[self-assignment]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, (uintmax_t)std::sqrt(std::numeric_limits<uintmax_t>::max()))));
    arby::Uint arb = value;
    value *= value;

    arb *= arb;

    CHECK((uintmax_t)arb == value);
}

TEST_CASE("Division self-assignment", "[self-assignment]") {
    auto value = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Uint arb = value;

    arb /= arb;

    CHECK((uintmax_t)arb == 1);
}

TEST_CASE("Modulo self-assignment", "[self-assignment]") {
    auto value = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Uint arb = value;

    arb %= arb;

    CHECK((uintmax_t)arb == 0);
}
