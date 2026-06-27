add_library(solver_lib
    Parser.cpp
)

target_include_directories(solver_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
