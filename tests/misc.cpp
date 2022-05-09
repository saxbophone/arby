#include <cstdint>

#include <limits>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone;

TEST_CASE("(bool)arby::Uint converts zero to false", "[misc]") {
    CHECK_FALSE(arby::Uint(0));
}

TEST_CASE("(bool)arby::Uint converts any non-zero value to true", "[misc]") {
    arby::Uint value = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    CHECK(value);
}
