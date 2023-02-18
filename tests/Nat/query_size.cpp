#include <catch2/catch.hpp>

#include <arby/Nat.hpp>
#include <arby/math.hpp>

using namespace com::saxbophone;

TEST_CASE("arby::Nat.digit_length() returns the number of digits used to store the value", "[query-size]") {
    auto digits = GENERATE(range(1u, 10u));

    arby::Nat value = arby::ipow(arby::Nat::BASE, digits - 1);

    CHECK(value.digit_length() == digits);
}

TEST_CASE("arby::Nat.byte_length() returns the number of bytes needed to store the value", "[query-size]") {
    auto digits = GENERATE(range(1u, 10u));

    arby::Nat value = arby::ipow(256, digits - 1);

    CHECK(value.byte_length() == digits);
}

TEST_CASE("arby::Nat.bit_length() returns the number of bits needed to store the value", "[query-size]") {
    auto digits = GENERATE(range(1u, 100u));

    arby::Nat value = arby::ipow(2, digits - 1);

    CHECK(value.bit_length() == digits);
}
