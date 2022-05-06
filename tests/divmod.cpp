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

TEST_CASE("divmod of arby::Uint by zero raises domain_error", "[division][modulo]") {
    arby::Uint numerator = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK_THROWS_AS(arby::Uint::divmod(numerator, arby::Uint(0)), std::domain_error);
}

TEST_CASE("Assignment-division of zero by any non-zero arby::Uint returns zero", "[division]") {
    arby::Uint numerator = arby::Uint(0);
    arby::Uint denominator = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    numerator /= denominator;
    CHECK((uintmax_t)numerator == 0);
}

TEST_CASE("Division of zero by any non-zero arby::Uint returns zero", "[division]") {
    arby::Uint numerator = arby::Uint(0);
    arby::Uint denominator = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    CHECK((uintmax_t)(numerator / denominator) == 0);
}

TEST_CASE("Assignment-modulo of zero by any non-zero arby::Uint returns zero", "[modulo]") {
    arby::Uint numerator = arby::Uint(0);
    arby::Uint denominator = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    numerator %= denominator;
    CHECK((uintmax_t)numerator == 0);
}

TEST_CASE("Modulo of zero by any non-zero arby::Uint returns zero", "[modulo]") {
    arby::Uint numerator = arby::Uint(0);
    arby::Uint denominator = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    CHECK((uintmax_t)(numerator % denominator) == 0);
}

TEST_CASE("divmod of zero by any non-zero arby::Uint returns zero quotient and remainder", "[division][modulo]") {
    arby::Uint numerator = arby::Uint(0);
    arby::Uint denominator = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    auto [quotient, remainder] = arby::Uint::divmod(numerator, denominator);

    CHECK((uintmax_t)quotient == 0);
    CHECK((uintmax_t)remainder == 0);
}
