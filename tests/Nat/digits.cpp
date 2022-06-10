#include <compare>
#include <limits>
#include <stdexcept>
#include <string>

#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

using namespace com::saxbophone;

TEST_CASE("Nat digits - std::initializer_list", "") {
    std::initializer_list<arby::Nat::StorageType> digits = {
        1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u
    };
    // also put those digits into a codlili::List instance so we can compare them
    codlili::List<arby::Nat::StorageType> original(digits);

    SECTION("Create Nat from digits") {
        arby::Nat from_digits(digits);

        SECTION("Get digits back out of Nat") {
            auto digits_back = from_digits.digits();

            CHECK(digits_back == original);
        }
    }
}

// Catch2 v2.x has a bug where template product test case triggers
// -Wsign-conversion: https://github.com/catchorg/Catch2/issues/2348
#pragma warning (disable : 4068 ) /* disable unknown pragma warnings for this file when on MSVC */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"

TEMPLATE_PRODUCT_TEST_CASE(
    "Nat digits", "",
    (codlili::List, std::vector), arby::Nat::StorageType
) {
    // generate digits arrays of size 1..8
    std::size_t size = GENERATE(take(1000, random(1u, 8u)));
    auto raw_digits = GENERATE_COPY(
        take(
            1,
            chunk(
                size,
                random(
                    (arby::Nat::StorageType)1, // not zero to prevent leading zeroes
                    std::numeric_limits<arby::Nat::StorageType>::max()
                )
            )
        )
    );
    // copy raw_digits into a container of TestType
    TestType digits;
    for (const auto& item : raw_digits) {
        digits.push_back(item);
    }
    // also read digits out into a codlili::List instance so we can compare them
    codlili::List<arby::Nat::StorageType> original;
    for (const auto& item : digits) {
        original.push_back(item);
    }

    SECTION("Create Nat from digits") {
        arby::Nat from_digits(digits);

        SECTION("Get digits back out of Nat") {
            auto digits_back = from_digits.digits();

            CHECK(digits_back == original);
        }
    }
}

#pragma GCC diagnostic pop

TEST_CASE("Nat init from digits leading zero elision") {
    CHECK(arby::Nat({0, 1, 2, 3}).digits() == codlili::List<arby::Nat::StorageType>({1, 2, 3}));
    CHECK(arby::Nat({1, 2, 3}).digits() == codlili::List<arby::Nat::StorageType>({1, 2, 3}));
    CHECK(arby::Nat({0}).digits() == codlili::List<arby::Nat::StorageType>({0}));
    CHECK(arby::Nat({}).digits() == codlili::List<arby::Nat::StorageType>({}));
    CHECK(arby::Nat({0, 0, 0, 9, 7, 5, 1, 2, 0, 0, 0, 2, 0}).digits() == codlili::List<arby::Nat::StorageType>({9, 7, 5, 1, 2, 0, 0, 0, 2, 0}));
    CHECK(arby::Nat({0, 0, 0}).digits() == codlili::List<arby::Nat::StorageType>({0}));
}
