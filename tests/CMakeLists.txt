CPMFindPackage(
    NAME Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.9
    EXCLUDE_FROM_ALL YES
)

add_executable(tests)
target_sources(
    tests PRIVATE
        main.cpp
        basic_arithmetic.cpp
        casting.cpp
        divmod.cpp
        misc.cpp
        multiplication.cpp
        pow.cpp
        self_assignment.cpp
        stringification.cpp
        user_defined_literals.cpp
)
target_link_libraries(
    tests
    PRIVATE
        arby-compiler-options  # tests use same compiler options as main project
        arby
        Catch2::Catch2  # unit testing framework
)

enable_testing()

# auto-discover and add Catch2 tests from unit tests program
include(CTest)
include(Catch)

catch_discover_tests(tests WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
