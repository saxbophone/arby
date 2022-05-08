#include <limits>
#include <sstream>
#include <string>

#include <catch2/catch.hpp>

#include <arby/arby.hpp>

using namespace com::saxbophone;

TEST_CASE("Using std::ostream << arby::Uint generates decimal string of value", "[stringification]") {
    auto values = GENERATE(
        table<arby::Uint, std::string>(
            {
                {arby::Uint(), "0"},
                {arby::Uint(123456789), "123456789"},
                {arby::Uint(19743358), "19743358"},
                {arby::Uint(std::numeric_limits<uintmax_t>::max()), std::to_string(std::numeric_limits<uintmax_t>::max())},
                {arby::Uint(491274912), "491274912"},
                {arby::Uint(900100390), "900100390"},
                {arby::Uint(503), "503"},
            }
        )
    );
    std::ostringstream os;

    os << std::get<0>(values);

    CHECK(os.str() == std::get<1>(values));
}

TEST_CASE("Using std::ostream << with random arby::Uint generates decimal string of value", "[stringification]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Uint arb = value;
    std::string expected_str = std::to_string(value);
    std::ostringstream os;

    os << arb;

    CHECK(os.str() == expected_str);
}

TEST_CASE("(std::string)arby::Uint generates same string as std::cout << std::dec << arby::Uint", "[stringification]") {
    auto value = GENERATE(take(1000, random((uintmax_t)0, std::numeric_limits<uintmax_t>::max())));
    arby::Uint arb = value;
    std::ostringstream os;
    os << std::dec << arb;
    std::string expected_str = os.str();

    CHECK((std::string)arb == expected_str);
}
