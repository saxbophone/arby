add_library(DivisionResult OBJECT division_result.cpp)
target_link_libraries(DivisionResult PRIVATE tests-config)
target_precompile_headers(DivisionResult PRIVATE <arby/DivisionResult.hpp> <arby/Interval.hpp> <arby/Nat.hpp>)
