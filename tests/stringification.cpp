#include <limits>
#include <sstream>
#include <string>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone;
using namespace com::saxbophone::arby;

TEST_CASE("Using std::ostream << arby::Nat generates decimal string of value", "[stringification]") {
    auto values = GENERATE(
        table<arby::Nat, std::string>(
            {
                {arby::Nat(), "0"},
                {arby::Nat(123456789), "123456789"},
                {arby::Nat(19743358), "19743358"},
                {arby::Nat(std::numeric_limits<uintmax_t>::max()), std::to_string(std::numeric_limits<uintmax_t>::max())},
                {arby::Nat(491274912), "491274912"},
                {arby::Nat(900100390), "900100390"},
                {arby::Nat(503), "503"},
            }
        )
    );
    std::ostringstream os;

    os << std::get<0>(values);

    CHECK(os.str() == std::get<1>(values));
}

TEST_CASE("Using std::ostream << with random arby::Nat generates decimal string of value", "[stringification]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Nat arb = value;
    std::string expected_str = std::to_string(value);
    std::ostringstream os;

    os << arb;

    CHECK(os.str() == expected_str);
}

TEST_CASE("Using std::ostream << std::hex << arby::Nat generates hexadecimal string of value", "[stringification]") {
    auto values = GENERATE(
        table<arby::Nat, std::string>(
            {
                {0_uarb, "0"},
                {0x123456789_uarb, "123456789"},
                {0xcafebabe3362, "cafebabe3362"},
                {0x100f32a8d1_uarb, "100f32a8d1"},
                {0x900100390_uarb, "900100390"},
                {0xf503, "f503"},
            }
        )
    );
    std::ostringstream generated;

    generated << std::hex << std::get<0>(values);

    CHECK(generated.str() == std::get<1>(values));
}

TEST_CASE("Using std::ostream with number base specifier and random arby::Nat generates string of value in the specified base", "[stringification]") {
    auto base = GENERATE(std::dec, std::oct, std::hex);
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Nat arb = value;
    std::ostringstream generated, expected;
    expected << base << value;
    CAPTURE(base, value);

    generated << base << arb;

    CHECK(generated.str() == expected.str());
}

TEST_CASE("(std::string)arby::Nat generates same string as std::cout << std::dec << arby::Nat", "[stringification]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Nat arb = value;
    std::ostringstream os;
    os << std::dec << arb;
    std::string expected_str = os.str();

    CHECK((std::string)arb == expected_str);
}
