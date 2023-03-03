add_library(Interval OBJECT interval.cpp)
target_link_libraries(Interval PRIVATE tests-config)
target_precompile_headers(Interval PRIVATE <arby/Interval.hpp> <arby/Nat.hpp>)
