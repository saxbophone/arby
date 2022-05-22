#include <cstdint>

#include <limits>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone;

TEST_CASE("(bool)arby::Nat converts zero to false", "[misc]") {
    CHECK_FALSE(arby::Nat(0));
}

TEST_CASE("(bool)arby::Nat converts any non-zero value to true", "[misc]") {
    arby::Nat value = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    CHECK(value);
}
