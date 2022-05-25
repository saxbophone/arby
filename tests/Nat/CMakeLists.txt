add_library(
    Nat OBJECT
        basic_arithmetic.cpp
        casting.cpp
        divmod.cpp
        misc.cpp
        multiplication.cpp
        namespaces.cpp
        pow.cpp
        self_assignment.cpp
        stringification.cpp
        user_defined_literals.cpp
)
target_link_libraries(Nat PRIVATE tests-config)
