#include <catch2/catch.hpp>

#include <arby/Public.hpp>

using namespace com::saxbophone::arby;
// test case to check network construction and train movement
TEST_CASE("Library works") {
    REQUIRE(library_works());
}
