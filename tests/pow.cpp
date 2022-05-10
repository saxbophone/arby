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

// std::pow() is not accurate for large powers and we need exactness
static uintmax_t integer_pow(uintmax_t base, uintmax_t exponent) {
    // 1 to the power of anything is always 1
    if (base == 1) {
        return 1;
    }
    uintmax_t power = 1;
    for (uintmax_t i = 0; i < exponent; i++) {
        power *= base;
    }
    return power;
}

TEST_CASE("Non-zero arby::Uint raised to the power of non-zero arby::Uint", "[pow]") {
    // base needs to be severely constrained if we are to have any reasonable prospect of getting some large exponents
    auto base = GENERATE(take(10000, random((uintmax_t)1, (uintmax_t)256)));
    // use log-n to find out the maximmum number exponent we can raise base to to fit in uintmax_t range
    // XXX: we're actually using max/2 range to make sure that we don't overflow uintmax_t due to log() inaccuracies
    uintmax_t MAX = (uintmax_t)(std::log(std::numeric_limits<uintmax_t>::max() / 2) / std::log(base));
    auto exponent = GENERATE_COPY(take(1, random((uintmax_t)1, MAX)));

    CAPTURE(base, exponent);

    CHECK((uintmax_t)arby::Uint::pow(base, exponent) == integer_pow(base, exponent));
}
