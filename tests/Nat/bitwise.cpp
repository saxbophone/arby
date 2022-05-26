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

TEST_CASE("arby::Nat bitwise OR", "[bitwise]") {
    uintmax_t lhs = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t rhs = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t result = lhs | rhs;
    arby::Nat object_lhs = lhs;
    arby::Nat object_rhs = rhs;

    CHECK((uintmax_t)(object_lhs | object_rhs) == result);
}

TEST_CASE("arby::Nat bitwise OR with large and small value", "[bitwise]") {
    uintmax_t large = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t small = GENERATE(take(200, random((uintmax_t)0, (uintmax_t)255)));
    uintmax_t result = large | small;
    arby::Nat large_object = large;
    arby::Nat small_object = small;

    CHECK((uintmax_t)(large_object | small_object) == result);
    CHECK((uintmax_t)(small_object | large_object) == result);
}

TEST_CASE("arby::Nat bitwise OR with zero", "[bitwise]") {
    uintmax_t value = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Nat object = value;

    // OR with zero should equal self
    CHECK((object | 0) == object);
}

TEST_CASE("arby::Nat bitwise OR with hardcoded values", "[bitwise]") {
    auto [lhs, rhs, expected] = GENERATE(
        table<arby::Nat, arby::Nat, arby::Nat>(
            {
                {0b1101000001001010111111001, 0b00000011111111, 0b1101000001001010111111111},
                {0x637981823345789012923acbde4184921008, 0x93f393c3e3d3a34c4c9420000, 0x63798182334d7fb93ebe3ffbfec5cdd21008},
                {10226483191214161820, 112, 10226483191214161916}
            }
        )
    );

    arby::Nat result = lhs | rhs;

    CHECK(result == expected);
}

TEST_CASE("arby::Nat bitwise assignment-AND", "[bitwise]") {
    uintmax_t value = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t other = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Nat object = value;
    value &= other;

    object &= other;

    CHECK((uintmax_t)object == value);
}

TEST_CASE("arby::Nat bitwise AND", "[bitwise]") {
    uintmax_t lhs = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t rhs = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t result = lhs & rhs;
    arby::Nat object_lhs = lhs;
    arby::Nat object_rhs = rhs;

    CHECK((uintmax_t)(object_lhs & object_rhs) == result);
}

TEST_CASE("arby::Nat bitwise AND with large and small value", "[bitwise]") {
    uintmax_t large = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t small = GENERATE(take(200, random((uintmax_t)0, (uintmax_t)255)));
    uintmax_t result = large & small;
    arby::Nat large_object = large;
    arby::Nat small_object = small;

    CHECK((uintmax_t)(large_object & small_object) == result);
    CHECK((uintmax_t)(small_object & large_object) == result);
}

TEST_CASE("arby::Nat bitwise AND with zero", "[bitwise]") {
    uintmax_t value = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Nat object = value;

    // AND with zero should equal zero
    CHECK((object & 0) == 0);
}

TEST_CASE("arby::Nat bitwise AND with hardcoded values", "[bitwise]") {
    auto [lhs, rhs, expected] = GENERATE(
        table<arby::Nat, arby::Nat, arby::Nat>(
            {
                {0b1101000001001010111111001, 0b00000011111111, 0b11111001},
                {0x637981823345789012923acbde4184921008, 0x93f393c3e3d3a34c4c9420000, 0x138101012380a144080020000},
                {10226483191214161820, 112, 16}
            }
        )
    );

    arby::Nat result = lhs & rhs;

    CHECK(result == expected);
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

TEST_CASE("arby::Nat bitwise XOR with large and small value", "[bitwise]") {
    uintmax_t large = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    uintmax_t small = GENERATE(take(200, random((uintmax_t)0, (uintmax_t)255)));
    uintmax_t result = large ^ small;
    arby::Nat large_object = large;
    arby::Nat small_object = small;

    CHECK((uintmax_t)(large_object ^ small_object) == result);
    CHECK((uintmax_t)(small_object ^ large_object) == result);
}

TEST_CASE("arby::Nat bitwise XOR with zero", "[bitwise]") {
    uintmax_t value = GENERATE(take(200, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Nat object = value;

    // XOR with zero should equal self
    CHECK((object ^ 0) == object);
}

TEST_CASE("arby::Nat bitwise XOR with hardcoded values", "[bitwise]") {
    auto [lhs, rhs, expected] = GENERATE(
        table<arby::Nat, arby::Nat, arby::Nat>(
            {
                {0b1101000001001010111111001, 0b00000011111111, 0b1101000001001010100000110},
                {0x637981823345789012923acbde4184921008, 0x93f393c3e3d3a34c4c9420000, 0x63798182334c47a92eac07f1ea854dd01008},
                {10226483191214161820, 112, 10226483191214161900}
            }
        )
    );

    arby::Nat result = lhs ^ rhs;

    CHECK(result == expected);
}
