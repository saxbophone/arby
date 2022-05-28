#include <limits>
#include <sstream>
#include <string>

#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

using namespace com::saxbophone;
using namespace com::saxbophone::arby::literals;

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
                {3498724987239842389479823489732498743289748932_nat, "3498724987239842389479823489732498743289748932"},
                {380049108309123901839081238891391892371893_nat, "380049108309123901839081238891391892371893"},
                {482489187408103891319414741005849139821481478074917040_nat, "482489187408103891319414741005849139821481478074917040"},
                {
                    4383473194188901823013000000001038213890189301387213712031203129381203812973183018109301293810_nat,
                    "4383473194188901823013000000001038213890189301387213712031203129381203812973183018109301293810"
                },
                {
                    348043020149049820481084013929310390000000000010001000101010239189777777777774032932032404910921929187129000001009109101000000000000000_nat,
                    "348043020149049820481084013929310390000000000010001000101010239189777777777774032932032404910921929187129000001009109101000000000000000"
                },
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
                {0_nat, "0"},
                {0x123456789_nat, "123456789"},
                {0xcafebabe3362, "cafebabe3362"},
                {0x100f32a8d1_nat, "100f32a8d1"},
                {0x900100390_nat, "900100390"},
                {0xf503, "f503"},
                {
                    0x4923749849184791479374891938794371c9c79c789e787c9ee87e98798d798d7c987a987879898789799c00034249_nat,
                    "4923749849184791479374891938794371c9c79c789e787c9ee87e98798d798d7c987a987879898789799c00034249"
                },
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
