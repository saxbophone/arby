#include <cmath>

#include <limits>
#include <stdexcept>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone;

TEST_CASE("Casting arby::Uint to uintmax_t", "[casting]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK((uintmax_t)arby::Uint(value) == value);
}

TEST_CASE("Casting arby::Uint with value higher than UINT_MAX to uintmax_t throws range_error", "[casting]") {
    arby::Uint value = arby::Uint(std::numeric_limits<uintmax_t>::max()) + 1;

    CHECK_THROWS_AS((uintmax_t)value, std::range_error);
}

TEST_CASE("Casting arby::Uint to long double", "[casting]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK((long double)arby::Uint(value) == (long double)value);
}

TEST_CASE("arby::Uint::from_float() with negative value throws std::domain_error") {
    auto value = GENERATE(
        take(1000,
            random(
                std::numeric_limits<long double>::lowest(),
                -std::numeric_limits<long double>::denorm_min()
            )
        )
    );

    CHECK_THROWS_AS(arby::Uint::from_float(value), std::domain_error);
}

TEST_CASE("arby::Uint::from_float() with non-finite value throws std::domain_error") {
    // need IEC 559 float to be sure that qNaN, sNan, ±Inf all exist
    if (std::numeric_limits<long double>::is_iec559) {
        auto value = GENERATE(
            -std::numeric_limits<long double>::infinity(),
            +std::numeric_limits<long double>::infinity(),
            std::numeric_limits<long double>::quiet_NaN(),
            std::numeric_limits<long double>::signaling_NaN()
        );

        CHECK_THROWS_AS(arby::Uint::from_float(value), std::domain_error);
    } else {
        WARN("Can't test for non-finite values because long double isn't IEC 559!");
    }
}

TEST_CASE("arby::Uint::from_float() with positive value") {
    auto power = GENERATE(0.125, 0.25, 0.5, 1, 2, 4, 8);
    auto value = GENERATE_COPY(
        take(100,
            random(
                0.0L,
                std::pow((long double)std::numeric_limits<uintmax_t>::max(), power)
            )
        )
    );

    CHECK((long double)arby::Uint::from_float(value) == Approx(std::trunc(value)));
}
