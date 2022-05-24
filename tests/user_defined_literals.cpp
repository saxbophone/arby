#include <cmath>
#include <cstdint>

#include <limits>

#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

using namespace com::saxbophone;
using namespace com::saxbophone::arby::literals;

TEST_CASE("Initialising arby::Nat objects with decimal _nat user-defined-literal", "[user-defined-literal]") {
    auto [literal, object] = GENERATE(
        table<arby::Nat, arby::Nat>(
            {
                {0_nat, 0},
                {10_nat, 10},
                {123_nat, 123},
                {1234_nat, 1234},
                {10000_nat, 10000},
                {100000_nat, 100000},
                {123456_nat, 123456},
                {9999999_nat, 9999999},
                {56213870_nat, 56213870},
            }
        )
    );

    CHECK(literal == object);
}

TEST_CASE("Initialising arby::Nat objects with hexadecimal _nat user-defined-literal", "[user-defined-literal]") {
    auto [literal, object] = GENERATE(
        table<arby::Nat, arby::Nat>(
            {
                {0x1172443_nat, 0x1172443},
                {0x19721871_nat, 0x19721871},
                {0x0_nat, 0x0},
                {0xf3c1d28a_nat, 0xf3c1d28a},
                {0x01234567_nat, 0x01234567},
                {0x89abcdef_nat, 0x89abcdef},
                {0xffffffff_nat, 0xffffffff},
                {0xf00000e1_nat, 0xf00000e1},
            }
        )
    );

    CHECK(literal == object);
}

TEST_CASE("Initialising arby::Nat objects with binary _nat user-defined-literal", "[user-defined-literal]") {
    auto [literal, object] = GENERATE(
        table<arby::Nat, arby::Nat>(
            {
                {0b0_nat, 0b0},
                {0b1_nat, 0b1},
                {0b11010010_nat, 0b11010010},
                {0b1111111111111111111111111111_nat, 0b1111111111111111111111111111},
                {0b1010011010100011110101110101_nat, 0b1010011010100011110101110101},
            }
        )
    );

    CHECK(literal == object);
}

TEST_CASE("Malformed octal _nat literal throws std::invalid_argument") {
    CHECK_THROWS_AS(01234_nat, std::invalid_argument);
}
