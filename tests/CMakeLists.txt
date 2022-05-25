CPMFindPackage(
    NAME Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.9
    EXCLUDE_FROM_ALL YES
)

# common option-propagating target to use for test suite sub-targets
add_library(tests-config INTERFACE)
target_link_libraries(
    tests-config
    INTERFACE
        arby-compiler-options  # tests use same compiler options as main project
        arby
        Catch2::Catch2  # unit testing framework
)

# every sub-part of the test suite
add_subdirectory(math_support)
add_subdirectory(Nat)

# test executable wraps everything together
add_executable(tests)
target_sources(
    tests PRIVATE
        main.cpp
        $<TARGET_OBJECTS:Nat>
)
target_link_libraries(
    tests PRIVATE
        arby-compiler-options  # tests use same compiler options as main project
        arby
        Catch2::Catch2  # unit testing framework
)

enable_testing()

# auto-discover and add Catch2 tests from unit tests program
include(CTest)
include(Catch)

# TODO: we might consider having one separate test executable per part, but not for now
catch_discover_tests(tests WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
