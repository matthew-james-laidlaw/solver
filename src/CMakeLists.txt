add_library(solver_lib
    Expression.cpp
    Parser.cpp
    Solver.cpp
)

target_include_directories(solver_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
