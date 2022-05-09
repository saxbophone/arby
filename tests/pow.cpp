#include <cmath>
#include <cstdint>

#include <limits>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone;

TEST_CASE("Any arby::Uint raised to the power of zero returns 1", "[pow]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK(arby::Uint::pow(value, 0) == 1);
}

TEST_CASE("Zero raised to the power of any non-zero arby::Uint returns 0", "[pow]") {
    auto value = GENERATE(take(1000, random((uintmax_t)1, std::numeric_limits<uintmax_t>::max())));

    CHECK(arby::Uint::pow(0, value) == 0);
}

TEST_CASE("Non-zero arby::Uint raised to the power of non-zero arby::Uint", "[pow]") {
    // base needs to be severely constrained if we are to have any reasonable prospect of getting some large exponents
    auto base = GENERATE(take(1000, random((uintmax_t)1, (uintmax_t)256)));
    // use log-n to find out the maximmum number exponent we can raise base to to fit in uintmax_t range
    uintmax_t MAX = (uintmax_t)(std::log(std::numeric_limits<uintmax_t>::max()) / std::log(base));
    auto exponent = GENERATE_COPY(take(1, random((uintmax_t)1, MAX)));

    CAPTURE(base, exponent);

    CHECK((uintmax_t)arby::Uint::pow(base, exponent) == (uintmax_t)std::pow(base, exponent));
}
