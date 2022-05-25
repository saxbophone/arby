#include <cmath>
#include <cstdint>

#include <limits>
#include <stdexcept>

#include <catch2/catch.hpp>

#include <arby/math.hpp>
#include <arby/Nat.hpp>

using namespace com::saxbophone;
using namespace com::saxbophone::arby::literals;

TEST_CASE("arby::ilog(base<2, nonzero) throws std::domain_error", "[math-support][ilog]") {
    arby::Nat base = GENERATE(0_nat, 1_nat);
    CHECK_THROWS_AS(arby::ilog(base, 1), std::domain_error);
}

TEST_CASE("arby::ilog(base>1, 0) throws std::domain_error", "[math-support][ilog]") {
    CHECK_THROWS_AS(arby::ilog(2_nat, 0_nat), std::domain_error);
}
