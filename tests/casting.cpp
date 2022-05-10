#include <limits>
#include <stdexcept>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone;

TEST_CASE("Casting arby::Uint to uintmax_t", "[casting]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK((uintmax_t)arby::Uint(value) == value);
}

TEST_CASE("Casting arby::Uint with value higher than UINT_MAX to uintmax_t throws overflow_error", "[casting]") {
    arby::Uint value = arby::Uint(std::numeric_limits<uintmax_t>::max()) + 1;

    CHECK_THROWS_AS((uintmax_t)value, std::overflow_error);
}
