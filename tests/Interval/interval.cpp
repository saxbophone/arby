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

TEMPLATE_TEST_CASE("Interval from floor,ceil", "[interval]", int, unsigned long, arby::Nat) {
    arby::Interval<TestType> input(99, 133);

    CHECK(input.floor == 99);
    CHECK(input.ceil == 133);
}

TEMPLATE_TEST_CASE("Structured binding from Interval", "[interval]", int, unsigned long, arby::Nat) {
    arby::Interval<TestType> input(99, 133);

    auto [floor, ceil] = input;

    CHECK(floor == 99);
    CHECK(ceil == 133);
}

TEMPLATE_TEST_CASE("Interval with same floor and ceil", "[interval]", int, unsigned long, arby::Nat) {
    arby::Interval<TestType> input(999);

    CHECK(input.floor == 999);
    CHECK(input.ceil == 999);
}

TEST_CASE("Interval<T> can be cast to Interval<U> where (T)U is valid", "[interval]") {
    using T = unsigned int;
    using U = long;

    arby::Interval<T> input(987, 232);

    arby::Interval<U> output = input;

    CHECK(output.floor == 987);
    CHECK(output.ceil == 232);
}

TEST_CASE("Interval can be compared to Interval", "[interval]") {
    arby::Interval<int> a(64, -523);
    arby::Interval<int> b = a;

    CHECK(a == b);
}

// NOTE: at this stage there is no support guaranteed for binding references
