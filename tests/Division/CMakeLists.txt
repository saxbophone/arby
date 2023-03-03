add_library(Division OBJECT division.cpp)
target_link_libraries(Division PRIVATE tests-config)
target_precompile_headers(Division PRIVATE <arby/Division.hpp> <arby/Interval.hpp>)
