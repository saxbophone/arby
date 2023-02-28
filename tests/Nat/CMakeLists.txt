add_library(
    Nat OBJECT
        basic_arithmetic.cpp
        bit_shifting.cpp
        bitwise.cpp
        casting.cpp
        digits.cpp
        divmod.cpp
        ilog.cpp
        ipow.cpp
        misc.cpp
        multiplication.cpp
        namespaces.cpp
        query_size.cpp
        self_assignment.cpp
        stringification.cpp
        user_defined_literals.cpp
)
target_link_libraries(Nat PRIVATE tests-config)
target_precompile_headers(Nat PRIVATE <arby/Nat.hpp>)
