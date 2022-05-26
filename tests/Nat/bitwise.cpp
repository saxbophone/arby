#include <cstdint>

#include <limits>

#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

using namespace com::saxbophone;

TEST_CASE("arby::Nat bitwise assignment-OR", "[bitwise]") {
    uintmax_t value = GENERATE(take(100, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t other = GENERATE(take(100, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Nat object = value;
    value |= other;

    object |= other;

    CHECK((uintmax_t)object == value);
}

TEST_CASE("arby::Nat bitwise OR") {
    WARN("No tests");
}

TEST_CASE("arby::Nat bitwise assignment-AND", "[bitwise]") {
    uintmax_t value = GENERATE(take(100, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t other = GENERATE(take(100, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Nat object = value;
    value &= other;

    object &= other;

    CHECK((uintmax_t)object == value);
}

TEST_CASE("arby::Nat bitwise AND") {
    WARN("No tests");
}

TEST_CASE("arby::Nat bitwise assignment-XOR", "[bitwise]") {
    uintmax_t value = GENERATE(take(100, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t other = GENERATE(take(100, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Nat object = value;
    value ^= other;

    object ^= other;

    CHECK((uintmax_t)object == value);
}

TEST_CASE("arby::Nat bitwise XOR", "[bitwise]") {
    WARN("No tests");
}

TEST_CASE("arby::Nat bitwise COMPLEMENT", "[bitwise]") {
    WARN("No tests");
}
