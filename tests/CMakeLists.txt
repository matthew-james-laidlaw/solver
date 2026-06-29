add_executable(solver_tests
    ParserTests.cpp
    SolverTests.cpp
)

target_link_libraries(solver_tests solver_lib gtest_main)
