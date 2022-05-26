#include <cstdint>

#include <limits>

#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

using namespace com::saxbophone;

TEST_CASE("arby::Nat bitwise assignment-OR", "[bitwise]") {
    uintmax_t value = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t other = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Nat object = value;
    value |= other;

    object |= other;

    CHECK((uintmax_t)object == value);
}

TEST_CASE("arby::Nat bitwise OR") {
    uintmax_t lhs = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t rhs = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t result = lhs | rhs;
    arby::Nat object_lhs = lhs;
    arby::Nat object_rhs = rhs;

    CHECK((uintmax_t)(object_lhs | object_rhs) == result);
}

TEST_CASE("arby::Nat bitwise assignment-AND", "[bitwise]") {
    uintmax_t value = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t other = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Nat object = value;
    value &= other;

    object &= other;

    CHECK((uintmax_t)object == value);
}

TEST_CASE("arby::Nat bitwise AND") {
    uintmax_t lhs = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t rhs = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t result = lhs & rhs;
    arby::Nat object_lhs = lhs;
    arby::Nat object_rhs = rhs;

    CHECK((uintmax_t)(object_lhs & object_rhs) == result);
}
TEST_CASE("arby::Nat bitwise assignment-XOR", "[bitwise]") {
    uintmax_t value = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t other = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Nat object = value;
    CAPTURE(value, other);
    value ^= other;

    object ^= other;
    CAPTURE(value, other);
    CHECK((uintmax_t)object == value);
}

TEST_CASE("arby::Nat bitwise XOR", "[bitwise]") {
    uintmax_t lhs = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t rhs = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t result = lhs ^ rhs;
    arby::Nat object_lhs = lhs;
    arby::Nat object_rhs = rhs;

    CHECK((uintmax_t)(object_lhs ^ object_rhs) == result);
}
