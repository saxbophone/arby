#include <limits>
#include <stdexcept>
#include <vector>

#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

using namespace com::saxbophone;
using namespace com::saxbophone::arby::literals;
using StorageType = arby::Nat::StorageType;

TEST_CASE("Nat digits - std::initializer_list", "") {
    std::initializer_list<StorageType> digits = {
        1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u
    };
    // also put those digits into a std::vector instance so we can compare them
    std::vector<StorageType> original(digits);

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

TEST_CASE("Nat digits - std::vector", "",) {
    // generate digits arrays of size 1..8
    std::size_t size = GENERATE(take(1000, random(1u, 8u)));
    auto raw_digits = GENERATE_COPY(
        take(
            1,
            chunk(
                size,
                random(
                    (StorageType)1, // not zero to prevent leading zeroes
                    std::numeric_limits<StorageType>::max()
                )
            )
        )
    );
    // copy raw_digits into a container of std::vector
    std::vector<StorageType> digits;
    for (const auto& item : raw_digits) {
        digits.push_back(item);
    }

    SECTION("Create Nat from digits") {
        arby::Nat from_digits(digits);

        SECTION("Get digits back out of Nat") {
            auto digits_back = from_digits.digits();

            CHECK(digits_back == digits);
        }
    }
}

#pragma GCC diagnostic pop

TEMPLATE_TEST_CASE(
    "Nat init from digits leading zero elision", "",
    std::initializer_list<StorageType>,
    std::vector<StorageType>
) {
    CHECK(arby::Nat(TestType({0, 1, 2, 3})).digits() == std::vector<StorageType>({1, 2, 3}));
    CHECK(arby::Nat(TestType({1, 2, 3})).digits() == std::vector<StorageType>({1, 2, 3}));
    CHECK(arby::Nat(TestType({0})).digits() == std::vector<StorageType>({0}));
    CHECK(arby::Nat(TestType({0, 0, 0, 9, 7, 5, 1, 2, 0, 0, 0, 2, 0})).digits() == std::vector<StorageType>({9, 7, 5, 1, 2, 0, 0, 0, 2, 0}));
    CHECK(arby::Nat(TestType({0, 0, 0})).digits() == std::vector<StorageType>({0}));
}

TEMPLATE_TEST_CASE(
    "Creating Nat from empty digits iterable throws std::invalid_argument", "",
    std::initializer_list<StorageType>,
    std::vector<StorageType>
) {
    CHECK_THROWS_AS(arby::Nat(TestType()), std::invalid_argument);
}
