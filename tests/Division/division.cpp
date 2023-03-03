#include <utility>

#include <catch2/catch.hpp>

#include <arby/Division.hpp>
#include <arby/Interval.hpp>
#include <arby/Nat.hpp>

using namespace com::saxbophone;

TEMPLATE_TEST_CASE("default Division", "[division-result]", int, unsigned long, arby::Nat) {
    arby::Division<TestType> def;

    // NOTE: such a Division result is actually invalid (no division can produce this result)
    CHECK(def.quotient == 0);
    CHECK(def.remainder == 0);
}

TEMPLATE_TEST_CASE("Division from quotient,remainder", "[division-result]", int, unsigned long, arby::Nat) {
    arby::Division<TestType> input(122, 76);

    CHECK(input.quotient == 122);
    CHECK(input.remainder == 76);
}

TEMPLATE_TEST_CASE("Structured binding from Division", "[division-result]", int, unsigned long, arby::Nat) {
    arby::Division<TestType> input(133, 99);

    auto [quotient, remainder] = input;

    CHECK(quotient == 133);
    CHECK(remainder == 99);
}

TEST_CASE("Division<T> can be cast to Division<U> where (T)U is valid", "[division-result]") {
    using T = unsigned int;
    using U = long;

    arby::Division<T> input(987, 232);

    arby::Division<U> output = input;

    CHECK(output.quotient == 987);
    CHECK(output.remainder == 232);
}

TEMPLATE_TEST_CASE("Get ceiling and floor of Division", "[division-result]", int, unsigned long, arby::Nat) {
    arby::Division<TestType> between(988, 66);

    CHECK(between.floor() == 988);
    CHECK(between.ceil() == 989);

    arby::Division<TestType> whole(123, 0);

    CHECK(whole.floor() == 123);
    CHECK(whole.ceil() == 123);
}

// NOTE: at this stage there is no support guaranteed for binding references
