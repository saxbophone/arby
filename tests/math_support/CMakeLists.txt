add_library(
    math-support OBJECT
        pow.cpp
)
target_link_libraries(math-support PRIVATE tests-config)
target_precompile_headers(math-support PRIVATE <arby/math.hpp>)
