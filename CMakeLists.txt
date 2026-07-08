cmake_minimum_required(VERSION 3.29.6)
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
# if(WIN32)
#     add_compile_options(/W4 /WX)
# else()
#     add_compile_options(-Wall -Wextra -pedantic -Werror)
# endif()

# --- Libraries ---

set(PARSER_SOURCES
    src/backend/monomial.cpp
    src/frontend/lexer.cpp
    src/frontend/parser.cpp
    src/frontend/state.cpp
    src/frontend/token.cpp
)

add_library(parser ${PARSER_SOURCES})
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(parser PRIVATE -fsanitize=address,fuzzer)

# --- Testing ---

# enable_testing()
# include(GoogleTest)

# add_executable(unit_tests
#     tests/unit/combinator_tests.cpp
#     tests/unit/lexer_tests.cpp
#     tests/unit/parser_tests.cpp
# )
# target_link_libraries(unit_tests parser gtest_main)
# gtest_discover_tests(unit_tests)

add_executable(fuzz_tests
    tests/fuzz/fuzz_parser.cpp
)
target_link_libraries(fuzz_tests PRIVATE parser)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(fuzz_tests PRIVATE -fsanitize=fuzzer,address)
    target_link_options(fuzz_tests PRIVATE -fsanitize=fuzzer,address)
endif()
