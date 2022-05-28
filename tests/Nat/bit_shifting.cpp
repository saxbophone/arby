#include <cstdint>

#include <limits>

#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

using namespace com::saxbophone;
using namespace com::saxbophone::arby::literals;

TEST_CASE("arby::Nat left bit-shift", "[bit-shifting]") {
    auto [lhs, rhs, result] = GENERATE(
        table<arby::Nat, arby::Nat, arby::Nat>(
            {
                {0b1110101_nat, 23, 0b111010100000000000000000000000_nat},
                {0b0_nat, 13, 0b0_nat},
                {0b1101_nat, 1, 0b11010_nat},
                {0b10001011_nat, 0, 0b10001011_nat},
                {0b10101110001_nat, 4, 0b101011100010000_nat},
                {0b1_nat, 70, 0b10000000000000000000000000000000000000000000000000000000000000000000000_nat},
            }
        )
    );

    arby::Nat shifted = lhs << rhs;

    CHECK(shifted == result);
}

TEST_CASE("arby::Nat left bit-shift assignment", "[bit-shifting]") {
    auto [lhs, rhs, result] = GENERATE(
        table<arby::Nat, arby::Nat, arby::Nat>(
            {
                {0b1110101_nat, 23, 0b111010100000000000000000000000_nat},
                {0b0_nat, 13, 0b0_nat},
                {0b1101_nat, 1, 0b11010_nat},
                {0b10001011_nat, 0, 0b10001011_nat},
                {0b10101110001_nat, 4, 0b101011100010000_nat},
                {0b1_nat, 70, 0b10000000000000000000000000000000000000000000000000000000000000000000000_nat},
            }
        )
    );

    lhs <<= rhs;

    CHECK(lhs == result);
}

TEST_CASE("arby::Nat right bit-shift", "[bit-shifting]") {
    auto [lhs, rhs, result] = GENERATE(
        table<arby::Nat, arby::Nat, arby::Nat>(
            {
                {0b10001101_nat, 3, 0b10001_nat},
                {0b111011001001001010101110101010_nat, 19, 0b11101100100_nat},
                {0b10000000110100000000011101101000_nat, 54, 0b0_nat},
                {0b10011001010_nat, 0, 0b10011001010_nat},
                {0b1101011000011000_nat, 8, 0b11010110_nat},
                {0b11111111111111111111111111111111111111111111111111111111111111111111111111111111_nat, 70, 0b1111111111_nat}
            }
        )
    );

    arby::Nat shifted = lhs >> rhs;

    CHECK(shifted == result);
}

TEST_CASE("arby::Nat right bit-shift assignment", "[bit-shifting]") {
    auto [lhs, rhs, result] = GENERATE(
        table<arby::Nat, arby::Nat, arby::Nat>(
            {
                {0b10001101_nat, 3, 0b10001_nat},
                {0b111011001001001010101110101010_nat, 19, 0b11101100100_nat},
                {0b10000000110100000000011101101000_nat, 54, 0b0_nat},
                {0b10011001010_nat, 0, 0b10011001010_nat},
                {0b1101011000011000_nat, 8, 0b11010110_nat},
                {0b11111111111111111111111111111111111111111111111111111111111111111111111111111111_nat, 70, 0b1111111111_nat}
            }
        )
    );

    lhs >>= rhs;

    CHECK(lhs == result);
}
