#include <cstdint>
#include <limits>
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

#include <arby/Nat.hpp>

using namespace com::saxbophone;

TEMPLATE_TEST_CASE(
    "Benchmark Nat against various native types", "[!benchmark]",
    unsigned short, unsigned int, unsigned long, std::uintmax_t
) {
    SECTION("Increment") {
        TestType w = (TestType)0;
        TestType x = (TestType)(std::numeric_limits<TestType>::max() / 2);
        TestType y = (TestType)(std::numeric_limits<TestType>::max() - 1);
        TestType z = std::numeric_limits<TestType>::max();
        arby::Nat a = w, b = x, c = y, d = z;

        BENCHMARK("Native") {
            w++;
            x++;
            y++;
            z++;
            return;
        };

        BENCHMARK("Arby") {
            a++;
            b++;
            c++;
            d++;
            return;
        };
    }

    SECTION("Decrement") {
        TestType w = (TestType)1;
        TestType x = (TestType)(std::numeric_limits<TestType>::max() / 3);
        TestType y = (TestType)(std::numeric_limits<TestType>::max() / 2);
        TestType z = std::numeric_limits<TestType>::max();
        arby::Nat a = w, b = x, c = y, d = z;

        BENCHMARK("Native") {
            w--;
            x--;
            y--;
            z--;
            return;
        };

        BENCHMARK("Arby") {
            a--;
            b--;
            c--;
            d--;
            return;
        };
    }
}
