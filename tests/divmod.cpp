#include <cmath>
#include <limits>
#include <stdexcept>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone;

TEST_CASE("Assignment-division by zero to arby::Uint raises domain_error", "[division]") {
    arby::Uint numerator = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK_THROWS_AS(numerator /= arby::Uint(0), std::domain_error);
}

TEST_CASE("Division of arby::Uint by zero raises domain_error", "[division]") {
    arby::Uint numerator = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK_THROWS_AS(numerator / arby::Uint(0), std::domain_error);
}

TEST_CASE("Assignment-modulo by zero to arby::Uint raises domain_error", "[modulo]") {
    arby::Uint numerator = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK_THROWS_AS(numerator %= arby::Uint(0), std::domain_error);
}

TEST_CASE("Modulo of arby::Uint by zero raises domain_error", "[modulo]") {
    arby::Uint numerator = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK_THROWS_AS(numerator % arby::Uint(0), std::domain_error);
}
