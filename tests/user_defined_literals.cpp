#include <cmath>
#include <cstdint>

#include <limits>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone::arby;

TEST_CASE("Initialising arby::Uint objects with _uarb user-defined-literal", "[user-defined-literal]") {
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
