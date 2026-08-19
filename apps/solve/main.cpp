#include <CLI/CLI.hpp>

#include <parser.h>
#include <solver/solver.h>

#include <complex>
#include <exception>
#include <format>
#include <iostream>
#include <string>
#include <vector>

auto main(int argc, char** argv) -> int
{
    auto app = CLI::App{"Polynomial solver"};

    // clang-format off

    std::string function;
    app.add_option("-f,--function", function, "The polynomial to solve")
        ->required();

    // clang-format on

    CLI11_PARSE(app, argc, argv);

    std::cout << std::format("Solving for: '{}'", function) << '\n';

    try {
        auto solutions = solver::Solve(function);

        if (solutions.empty()) {
            std::cout << "There are no solutions!" << '\n';
        }
        else {
            std::cout << "Solutions:" << '\n';
            for (auto solution : solutions) {
                std::cout << solution << '\n';
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    catch (...) {
        std::cerr << "caught unknown exception" << '\n';
        return 1;
    }

    return 0;
}
