#include <cmath>
#include <cstdint>

#include <limits>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone::arby;

TEST_CASE("Initialising arby::Uint objects with decimal _uarb user-defined-literal", "[user-defined-literal]") {
    auto [literal, object] = GENERATE(
        table<Uint, Uint>(
            {
                {0_uarb, 0},
                {10_uarb, 10},
                {123_uarb, 123},
                {1234_uarb, 1234},
                {10000_uarb, 10000},
                {100000_uarb, 100000},
                {123456_uarb, 123456},
                {9999999_uarb, 9999999},
                {56213870_uarb, 56213870},
            }
        )
    );

    CHECK(literal == object);
}

TEST_CASE("Initialising arby::Uint objects with hexadecimal _uarb user-defined-literal", "[user-defined-literal]") {
    auto [literal, object] = GENERATE(
        table<Uint, Uint>(
            {
                {0x1172443_uarb, 0x1172443},
                {0x19721871_uarb, 0x19721871},
                {0x0_uarb, 0x0},
                {0xf3c1d28a_uarb, 0xf3c1d28a},
                {0x01234567_uarb, 0x01234567},
                {0x89abcdef_uarb, 0x89abcdef},
                {0xffffffff_uarb, 0xffffffff},
                {0xf00000e1_uarb, 0xf00000e1},
            }
        )
    );

    CHECK(literal == object);
}

TEST_CASE("Initialising arby::Uint objects with binary _uarb user-defined-literal", "[user-defined-literal]") {
    auto [literal, object] = GENERATE(
        table<Uint, Uint>(
            {
                {0b0_uarb, 0b0},
                {0b1_uarb, 0b1},
                {0b11010010_uarb, 0b11010010},
                {0b1111111111111111111111111111_uarb, 0b1111111111111111111111111111},
                {0b1010011010100011110101110101_uarb, 0b1010011010100011110101110101},
            }
        )
    );

    CHECK(literal == object);
}

TEST_CASE("Malformed octal _uarb literal throws std::invalid_argument") {
    CHECK_THROWS_AS(01234_uarb, std::invalid_argument);
}
