#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

TEST_CASE("Custom literals for Nat are available when namespace com::saxbophone::arby is imported", "[namespaces]") {
    using namespace com::saxbophone::arby;

    CHECK(12345678_nat == 12345678);
}

TEST_CASE("Custom literals for Nat are available when namespace com::saxbophone::arby::literals is imported", "[namespaces]") {
    using namespace com::saxbophone::arby::literals;

    CHECK(12345678_nat == 12345678);
}
