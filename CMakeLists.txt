cmake_minimum_required(VERSION 3.29.6)

project(
    "SOLVER"
    VERSION "0.1.0"
    LANGUAGES "CXX"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(SOLVER_BUILD_TESTS "Build this project's unit tests" ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
include(cpm_modules)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

# --- external dependencies ---
cpm_add_cli11()
cpm_add_googletest()
cpm_add_spdlog()

enable_testing()
include(GoogleTest)

if(SOLVER_BUILD_TESTS)
    add_executable(unit_tests)
    target_link_libraries(unit_tests GTest::gtest_main)
endif()

# --- source code ---
add_subdirectory(libs)
add_subdirectory(apps)

gtest_discover_tests(unit_tests)
