cmake_minimum_required(VERSION 4.2.0)

project(SOLVER LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)

FetchContent_MakeAvailable(googletest)

add_subdirectory(src)
add_subdirectory(tests)
