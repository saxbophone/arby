#include <limits>
#include <sstream>
#include <string>

#include <catch2/catch.hpp>

#include <arby/Nat.hpp>
#include <arby/math.hpp>

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
                {
                    arby::pow(2, 1000),
                    "10715086071862673209484250490600018105614048117055336074437503883703510511249361224931983788156958581275946729175531468251871452856923140435984577574698574803934567774824230985421074605062371141877954182153046474983581941267398767559165543946077062914571196477686542167660429831652624386837205668069376"
                },
                {
                    arby::pow(2, 2000),
                    "114813069527425452423283320117768198402231770208869520047764273682576626139237031385665948631650626991844596463898746277344711896086305533142593135616665318539129989145312280000688779148240044871428926990063486244781615463646388363947317026040466353970904996558162398808944629605623311649536164221970332681344168908984458505602379484807914058900934776500429002716706625830522008132236281291761267883317206598995396418127021779858404042159853183251540889433902091920554957783589672039160081957216630582755380425583726015528348786419432054508915275783882625175435528800822842770817965453762184851149029376"
                },
                {
                    arby::pow(2, 4000),
                    "13182040934309431001038897942365913631840191610932727690928034502417569281128344551079752123172122033140940756480716823038446817694240581281731062452512184038544674444386888956328970642771993930036586552924249514488832183389415832375620009284922608946111038578754077913265440918583125586050431647284603636490823850007826811672468900210689104488089485347192152708820119765006125944858397761874669301278745233504796586994514054435217053803732703240283400815926169348364799472716094576894007243168662568886603065832486830606125017643356469732407252874567217733694824236675323341755681839221954693820456072020253884371226826844858636194212875139566587445390068014747975813971748114770439248826688667129237954128555841874460665729630492658600179338272579110020881228767361200603478973120168893997574353727653998969223092798255701666067972698906236921628764772837915526086464389161570534616956703744840502975279094087587298968423516531626090898389351449020056851221079048966718878943309232071978575639877208621237040940126912767610658141079378758043403611425454744180577150855204937163460902512732551260539639221457005977247266676344018155647509515396711351487546062479444592779055555421362722504575706910949376"
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
