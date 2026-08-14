add_executable(solver main.cpp)
target_link_libraries(solver libsolver spdlog::spdlog CLI11::CLI11)
