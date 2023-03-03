#include <utility>

#include <catch2/catch.hpp>

#include <arby/Interval.hpp>
#include <arby/Nat.hpp>

using namespace com::saxbophone;

TEMPLATE_TEST_CASE("default Interval", "[interval]", int, unsigned long, arby::Nat) {
    arby::Interval<TestType> def;

    CHECK(def.floor == 0);
    CHECK(def.ceil == 0);
}

TEMPLATE_TEST_CASE("Interval from pair", "[interval]", int, unsigned long, arby::Nat) {
    std::pair<TestType, TestType> input = {16, 132};

    arby::Interval<TestType> output = input;

    CHECK(output.floor == 16);
    CHECK(output.ceil == 132);
}

TEMPLATE_TEST_CASE("Interval from floor,ceil", "[interval]", int, unsigned long, arby::Nat) {
    arby::Interval<TestType> input(99, 133);

    CHECK(input.floor == 99);
    CHECK(input.ceil == 133);
}

TEMPLATE_TEST_CASE("Convert Interval to pair", "[interval]", int, unsigned long, arby::Nat) {
    arby::Interval<TestType> input(99, 133);

    std::pair<TestType, TestType> output = input;

    CHECK(output.first == 99);
    CHECK(output.second == 133);
}

TEMPLATE_TEST_CASE("Structured binding from Interval", "[interval]", int, unsigned long, arby::Nat) {
    arby::Interval<TestType> input(99, 133);

    auto [floor, ceil] = input;

    CHECK(floor == 99);
    CHECK(ceil == 133);
}
