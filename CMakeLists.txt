cmake_minimum_required(VERSION 4.2.1)
project(SOLVER LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

# --- Compiler Options ---
if(WIN32)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# --- Libraries ---

add_library(parser
    src/parser/lexer.cpp
    src/parser/state.cpp
    src/parser/token.cpp
    src/parser/expression.cpp
)
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# --- Testing ---

enable_testing()
include(GoogleTest)

add_executable(unit_tests
    tests/combinator_tests.cpp
    tests/lexer_tests.cpp
    tests/parser_tests.cpp
)

target_link_libraries(unit_tests parser gtest_main)
gtest_discover_tests(unit_tests)
