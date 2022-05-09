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

TEST_CASE("divmod of arby::Uint by zero raises domain_error", "[divmod]") {
    arby::Uint numerator = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK_THROWS_AS(arby::Uint::divmod(numerator, arby::Uint(0)), std::domain_error);
}

TEST_CASE("Assignment-division of zero by any non-zero arby::Uint returns zero", "[division]") {
    arby::Uint numerator = arby::Uint(0);
    uintmax_t denominator = GENERATE(take(1000, random((uintmax_t)1, (uintmax_t)arby::Uint::BASE)));

    numerator /= denominator;
    CHECK((uintmax_t)numerator == 0);
}

TEST_CASE("Division of zero by any non-zero arby::Uint returns zero", "[division]") {
    arby::Uint numerator = arby::Uint(0);
    uintmax_t denominator = GENERATE(take(1000, random((uintmax_t)1, (uintmax_t)arby::Uint::BASE)));

    CHECK((uintmax_t)(numerator / denominator) == 0);
}

TEST_CASE("Assignment-modulo of zero by any non-zero arby::Uint returns zero", "[modulo]") {
    arby::Uint numerator = arby::Uint(0);
    uintmax_t denominator = GENERATE(take(1000, random((uintmax_t)1, (uintmax_t)arby::Uint::BASE)));

    numerator %= denominator;
    CHECK((uintmax_t)numerator == 0);
}

TEST_CASE("Modulo of zero by any non-zero arby::Uint returns zero", "[modulo]") {
    arby::Uint numerator = arby::Uint(0);
    uintmax_t denominator = GENERATE(take(1000, random((uintmax_t)1, (uintmax_t)arby::Uint::BASE)));

    CHECK((uintmax_t)(numerator % denominator) == 0);
}

TEST_CASE("divmod of zero by any non-zero arby::Uint returns zero quotient and remainder", "[divmod]") {
    arby::Uint numerator = arby::Uint(0);
    uintmax_t denominator = GENERATE(take(1000, random((uintmax_t)1, (uintmax_t)arby::Uint::BASE)));

    auto [quotient, remainder] = arby::Uint::divmod(numerator, denominator);

    CHECK((uintmax_t)quotient == 0);
    CHECK((uintmax_t)remainder == 0);
}

TEST_CASE("Assignment-division by small non-zero arby::Uint to non-zero arby::Uint", "[division]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Uint::BASE)));
    arby::Uint lhs = numerator;
    arby::Uint rhs = denominator;
    numerator /= denominator;

    lhs /= rhs;

    CHECK((uintmax_t)lhs == numerator);
}

TEST_CASE("Division of non-zero arby::Uint by small non-zero arby::Uint", "[division]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Uint::BASE)));
    arby::Uint lhs = numerator;
    arby::Uint rhs = denominator;

    CHECK((uintmax_t)(lhs / rhs) == (numerator / denominator));
}

TEST_CASE("Assignment-modulo by small non-zero arby::Uint to non-zero arby::Uint", "[modulo]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Uint::BASE)));
    arby::Uint lhs = numerator;
    arby::Uint rhs = denominator;
    numerator %= denominator;

    lhs %= rhs;

    CHECK((uintmax_t)lhs == numerator);
}

TEST_CASE("Modulo of non-zero arby::Uint by small non-zero arby::Uint", "[modulo]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Uint::BASE)));
    arby::Uint lhs = numerator;
    arby::Uint rhs = denominator;

    CHECK((uintmax_t)(lhs % rhs) == (numerator % denominator));
}

TEST_CASE("divmod of non-zero arby::Uint by small non-zero arby::Uint", "[divmod]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Uint::BASE)));
    arby::Uint lhs = numerator;
    arby::Uint rhs = denominator;

    auto [quotient, remainder] = arby::Uint::divmod(lhs, rhs);

    CHECK((uintmax_t)quotient == numerator / denominator);
    CHECK((uintmax_t)remainder == numerator % denominator);
}

TEST_CASE("Assignment-division by non-zero arby::Uint to non-zero arby::Uint", "[division]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Uint lhs = numerator;
    arby::Uint rhs = denominator;
    numerator /= denominator;

    lhs /= rhs;

    CHECK((uintmax_t)lhs == numerator);
}

TEST_CASE("Division of non-zero arby::Uint by non-zero arby::Uint", "[division]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Uint lhs = numerator;
    arby::Uint rhs = denominator;

    CHECK((uintmax_t)(lhs / rhs) == (numerator / denominator));
}

TEST_CASE("Assignment-modulo by non-zero arby::Uint to non-zero arby::Uint", "[modulo]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Uint lhs = numerator;
    arby::Uint rhs = denominator;
    numerator %= denominator;

    lhs %= rhs;

    CHECK((uintmax_t)lhs == numerator);
}

TEST_CASE("Modulo of non-zero arby::Uint by non-zero arby::Uint", "[modulo]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Uint lhs = numerator;
    arby::Uint rhs = denominator;

    CHECK((uintmax_t)(lhs % rhs) == (numerator % denominator));
}

TEST_CASE("divmod of non-zero arby::Uint by non-zero arby::Uint", "[divmod]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Uint lhs = numerator;
    arby::Uint rhs = denominator;

    auto [quotient, remainder] = arby::Uint::divmod(lhs, rhs);

    CHECK((uintmax_t)quotient == numerator / denominator);
    CHECK((uintmax_t)remainder == numerator % denominator);
}

// extra

TEST_CASE("Division of much smaller arby::Uint by much larger arby::Uint", "[division]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)0, (uintmax_t)arby::Uint::BASE)));
    // ensure denominator is greater than numerator
    uintmax_t denominator = GENERATE_COPY(take(100, random(numerator + 1, std::numeric_limits<uintmax_t>::max())));
    arby::Uint lhs = numerator;
    arby::Uint rhs = denominator;

    // answer should always be zero
    CHECK((uintmax_t)(lhs / rhs) == 0);
}
