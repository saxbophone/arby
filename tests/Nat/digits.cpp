#include <compare>
#include <limits>
#include <stdexcept>
#include <string>

#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

using namespace com::saxbophone;

TEST_CASE("Nat digits") {
    // generate digits arrays of size 1..8
    std::size_t size = GENERATE(take(1000, random(1u, 8u)));
    auto raw_digits = GENERATE_COPY(
        take(
            1,
            chunk(
                size,
                random(
                    (arby::Nat::StorageType)0,
                    std::numeric_limits<arby::Nat::StorageType>::max()
                )
            )
        )
    );
    // copy raw_digits into a codlili::List
    codlili::List<arby::Nat::StorageType> digits;
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
