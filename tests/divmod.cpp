#include <cmath>
#include <limits>
#include <stdexcept>

#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

using namespace com::saxbophone;

TEST_CASE("Assignment-division by zero to arby::Nat raises domain_error", "[division]") {
    arby::Nat numerator = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK_THROWS_AS(numerator /= arby::Nat(0), std::domain_error);
}

TEST_CASE("Division of arby::Nat by zero raises domain_error", "[division]") {
    arby::Nat numerator = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK_THROWS_AS(numerator / arby::Nat(0), std::domain_error);
}

TEST_CASE("Assignment-modulo by zero to arby::Nat raises domain_error", "[modulo]") {
    arby::Nat numerator = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK_THROWS_AS(numerator %= arby::Nat(0), std::domain_error);
}

TEST_CASE("Modulo of arby::Nat by zero raises domain_error", "[modulo]") {
    arby::Nat numerator = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK_THROWS_AS(numerator % arby::Nat(0), std::domain_error);
}

TEST_CASE("divmod of arby::Nat by zero raises domain_error", "[divmod]") {
    arby::Nat numerator = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK_THROWS_AS(arby::Nat::divmod(numerator, arby::Nat(0)), std::domain_error);
}

TEST_CASE("Assignment-division of zero by any non-zero arby::Nat returns zero", "[division]") {
    arby::Nat numerator = arby::Nat(0);
    uintmax_t denominator = GENERATE(take(1000, random((uintmax_t)1, (uintmax_t)arby::Nat::BASE)));

    numerator /= denominator;
    CHECK((uintmax_t)numerator == 0);
}

TEST_CASE("Division of zero by any non-zero arby::Nat returns zero", "[division]") {
    arby::Nat numerator = arby::Nat(0);
    uintmax_t denominator = GENERATE(take(1000, random((uintmax_t)1, (uintmax_t)arby::Nat::BASE)));

    CHECK((uintmax_t)(numerator / denominator) == 0);
}

TEST_CASE("Assignment-modulo of zero by any non-zero arby::Nat returns zero", "[modulo]") {
    arby::Nat numerator = arby::Nat(0);
    uintmax_t denominator = GENERATE(take(1000, random((uintmax_t)1, (uintmax_t)arby::Nat::BASE)));

    numerator %= denominator;
    CHECK((uintmax_t)numerator == 0);
}

TEST_CASE("Modulo of zero by any non-zero arby::Nat returns zero", "[modulo]") {
    arby::Nat numerator = arby::Nat(0);
    uintmax_t denominator = GENERATE(take(1000, random((uintmax_t)1, (uintmax_t)arby::Nat::BASE)));

    CHECK((uintmax_t)(numerator % denominator) == 0);
}

TEST_CASE("divmod of zero by any non-zero arby::Nat returns zero quotient and remainder", "[divmod]") {
    arby::Nat numerator = arby::Nat(0);
    uintmax_t denominator = GENERATE(take(1000, random((uintmax_t)1, (uintmax_t)arby::Nat::BASE)));

    auto [quotient, remainder] = arby::Nat::divmod(numerator, denominator);

    CHECK((uintmax_t)quotient == 0);
    CHECK((uintmax_t)remainder == 0);
}

TEST_CASE("Assignment-division by small non-zero arby::Nat to non-zero arby::Nat", "[division]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Nat::BASE)));
    arby::Nat lhs = numerator;
    arby::Nat rhs = denominator;
    numerator /= denominator;

    lhs /= rhs;

    CHECK((uintmax_t)lhs == numerator);
}

TEST_CASE("Division of non-zero arby::Nat by small non-zero arby::Nat", "[division]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Nat::BASE)));
    arby::Nat lhs = numerator;
    arby::Nat rhs = denominator;

    CHECK((uintmax_t)(lhs / rhs) == (numerator / denominator));
}

TEST_CASE("Assignment-modulo by small non-zero arby::Nat to non-zero arby::Nat", "[modulo]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Nat::BASE)));
    arby::Nat lhs = numerator;
    arby::Nat rhs = denominator;
    numerator %= denominator;

    lhs %= rhs;

    CHECK((uintmax_t)lhs == numerator);
}

TEST_CASE("Modulo of non-zero arby::Nat by small non-zero arby::Nat", "[modulo]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Nat::BASE)));
    arby::Nat lhs = numerator;
    arby::Nat rhs = denominator;

    CHECK((uintmax_t)(lhs % rhs) == (numerator % denominator));
}

TEST_CASE("divmod of non-zero arby::Nat by small non-zero arby::Nat", "[divmod]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, (uintmax_t)arby::Nat::BASE)));
    arby::Nat lhs = numerator;
    arby::Nat rhs = denominator;

    auto [quotient, remainder] = arby::Nat::divmod(lhs, rhs);

    CHECK((uintmax_t)quotient == numerator / denominator);
    CHECK((uintmax_t)remainder == numerator % denominator);
}

TEST_CASE("Assignment-division by non-zero arby::Nat to non-zero arby::Nat", "[division]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Nat lhs = numerator;
    arby::Nat rhs = denominator;
    numerator /= denominator;

    lhs /= rhs;

    CHECK((uintmax_t)lhs == numerator);
}

TEST_CASE("Division of non-zero arby::Nat by non-zero arby::Nat", "[division]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Nat lhs = numerator;
    arby::Nat rhs = denominator;

    CHECK((uintmax_t)(lhs / rhs) == (numerator / denominator));
}

TEST_CASE("Assignment-modulo by non-zero arby::Nat to non-zero arby::Nat", "[modulo]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Nat lhs = numerator;
    arby::Nat rhs = denominator;
    numerator %= denominator;

    lhs %= rhs;

    CHECK((uintmax_t)lhs == numerator);
}

TEST_CASE("Modulo of non-zero arby::Nat by non-zero arby::Nat", "[modulo]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Nat lhs = numerator;
    arby::Nat rhs = denominator;

    CHECK((uintmax_t)(lhs % rhs) == (numerator % denominator));
}

TEST_CASE("divmod of non-zero arby::Nat by non-zero arby::Nat", "[divmod]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    uintmax_t denominator = GENERATE(take(100, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));
    arby::Nat lhs = numerator;
    arby::Nat rhs = denominator;

    auto [quotient, remainder] = arby::Nat::divmod(lhs, rhs);

    CHECK((uintmax_t)quotient == numerator / denominator);
    CHECK((uintmax_t)remainder == numerator % denominator);
}

// extra

TEST_CASE("Division of much smaller arby::Nat by much larger arby::Nat", "[division]") {
    uintmax_t numerator = GENERATE(take(100, random((uintmax_t)0, (uintmax_t)arby::Nat::BASE)));
    // ensure denominator is greater than numerator
    uintmax_t denominator = GENERATE_COPY(take(100, random(numerator + 1, std::numeric_limits<uintmax_t>::max())));
    arby::Nat lhs = numerator;
    arby::Nat rhs = denominator;

    // answer should always be zero
    CHECK((uintmax_t)(lhs / rhs) == 0);
}
