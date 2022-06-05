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
        BENCHMARK("Native") {
            TestType w = 0;
            TestType x = std::numeric_limits<TestType>::max() / 2;
            TestType y = std::numeric_limits<TestType>::max() - 1;
            TestType z = std::numeric_limits<TestType>::max();
            w++;
            x++;
            y++;
            z++;
            return;
        };

        BENCHMARK("Arby") {
            arby::Nat a = 0;
            arby::Nat b = std::numeric_limits<TestType>::max() / 2;
            arby::Nat c = std::numeric_limits<TestType>::max() - 1;
            arby::Nat d = std::numeric_limits<TestType>::max();
            a++;
            b++;
            c++;
            d++;
            return;
        };
    }

    SECTION("Decrement") {
        BENCHMARK("Native") {
            TestType w = 1;
            TestType x = std::numeric_limits<TestType>::max() / 3;
            TestType y = std::numeric_limits<TestType>::max() / 2;
            TestType z = std::numeric_limits<TestType>::max();
            w--;
            x--;
            y--;
            z--;
            return;
        };

        BENCHMARK("Arby") {
            arby::Nat a = 1;
            arby::Nat b = std::numeric_limits<TestType>::max() / 3;
            arby::Nat c = std::numeric_limits<TestType>::max() / 2;
            arby::Nat d = std::numeric_limits<TestType>::max();
            a--;
            b--;
            c--;
            d--;
            return;
        };
    }
}
