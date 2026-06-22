cmake_minimum_required(VERSION 4.2.0)

project(SOLVER LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(solver
    src/main.cpp
)

target_include_directories(solver
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
