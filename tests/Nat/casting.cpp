#include <cmath>

#include <limits>
#include <stdexcept>

#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

using namespace com::saxbophone;

TEST_CASE("Casting arby::Nat to uintmax_t", "[casting]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK((uintmax_t)arby::Nat(value) == value);
}

TEST_CASE("Casting arby::Nat with value higher than UINT_MAX to uintmax_t throws range_error", "[casting]") {
    arby::Nat value = arby::Nat(std::numeric_limits<uintmax_t>::max()) + 1;

    CHECK_THROWS_AS((uintmax_t)value, std::range_error);
}

TEST_CASE("Casting arby::Nat to float", "[casting]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    // float is so imprecise for large values that we need to cast the input too
    CHECK((float)arby::Nat((long double)(float)value) == (float)value);
}

TEST_CASE("Casting arby::Nat to double", "[casting]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK((double)arby::Nat(value) == (double)value);
}

TEST_CASE("Casting arby::Nat to long double", "[casting]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));

    CHECK((long double)arby::Nat(value) == (long double)value);
}

TEMPLATE_TEST_CASE(
    "Casting arby::Nat with value higher than max to other type throws range_error", "[casting]",
    std::uint8_t, std::int8_t, std::uint16_t, std::int16_t, std::uint32_t, std::int32_t
) {
    arby::Nat value = arby::Nat((uintmax_t)std::numeric_limits<TestType>::max()) + 1;

    CHECK_THROWS_AS((TestType)value, std::range_error);
}

TEST_CASE("Casting arby::Nat to other type - uint8_t", "[casting]",) {
    auto value = GENERATE(take(1000, random((std::uint16_t)0, (std::uint16_t)std::numeric_limits<std::uint8_t>::max())));

    CHECK((std::uint8_t)arby::Nat((uintmax_t)value) == (std::uint8_t)value);
}

TEST_CASE("Casting arby::Nat to other type - int8_t", "[casting]",) {
    auto value = GENERATE(take(1000, random((std::int16_t)0, (std::int16_t)std::numeric_limits<std::int8_t>::max())));

    CHECK((std::int8_t)arby::Nat((uintmax_t)value) == (std::int8_t)value);
}

TEMPLATE_TEST_CASE(
    "Casting arby::Nat to other type", "[casting]",
    std::uint16_t, std::int16_t, std::uint32_t, std::int32_t
) {
    auto value = GENERATE(take(1000, random((TestType)0, std::numeric_limits<TestType>::max())));

    CHECK((TestType)arby::Nat((uintmax_t)value) == (TestType)value);
}

TEST_CASE("arby::Nat::from_float() with negative value throws std::domain_error") {
    auto value = GENERATE(
        take(1000,
            random(
                std::numeric_limits<long double>::lowest(),
                -std::numeric_limits<long double>::denorm_min()
            )
        )
    );

    CHECK_THROWS_AS(arby::Nat::from_float(value), std::domain_error);
}

TEST_CASE("arby::Nat::from_float() with non-finite value throws std::domain_error") {
    // need IEC 559 float to be sure that qNaN, sNan, ±Inf all exist
    if (std::numeric_limits<long double>::is_iec559) {
        auto value = GENERATE(
            -std::numeric_limits<long double>::infinity(),
            +std::numeric_limits<long double>::infinity(),
            std::numeric_limits<long double>::quiet_NaN(),
            std::numeric_limits<long double>::signaling_NaN()
        );

        CHECK_THROWS_AS(arby::Nat::from_float(value), std::domain_error);
    } else {
        WARN("Can't test for non-finite values because long double isn't IEC 559!");
    }
}

TEST_CASE("arby::Nat::from_float() with value between 0 and 1") {
    // NOTE: top range is the last float value > 1
    long double zero_ish = GENERATE(take(1000, random(0.0L, std::nextafter(1.0L, 0.0L))));
    CAPTURE(zero_ish);

    arby::Nat object = arby::Nat::from_float(zero_ish);

    // value should be correct
    CHECK((long double)object == Approx(std::trunc(zero_ish)));
}

TEST_CASE("arby::Nat::from_float() with positive value") {
    auto power = GENERATE(0.125, 0.25, 0.5, 1, 2, 4, 8);
    auto value = GENERATE_COPY(
        take(100,
            random(
                0.0L,
                std::pow((long double)std::numeric_limits<uintmax_t>::max(), power)
            )
        )
    );
    CAPTURE(power, value);

    arby::Nat object = arby::Nat::from_float(value);

    // value should be correct
    CHECK((long double)object == Approx(std::trunc(value)));
}

TEST_CASE("arby::Nat can be constructed from unsigned types smaller than uintmax_t - uint8_t",) {
    // workaround for Catch bug: https://github.com/catchorg/Catch2/issues/2433
    std::uint16_t input = GENERATE(take(1000, random((std::uint16_t)0x00, (std::uint16_t)0xFF)));

    arby::Nat object = (std::uint8_t)input;

    CHECK((uintmax_t)object == input);
}

TEMPLATE_TEST_CASE(
    "arby::Nat can be constructed from unsigned types smaller than uintmax_t",
    "",
    std::uint16_t, std::uint32_t, std::uint64_t
) {
    TestType input = GENERATE(take(1000, random((TestType)0, std::numeric_limits<TestType>::max())));

    arby::Nat object = input;

    CHECK((uintmax_t)object == input);
}
