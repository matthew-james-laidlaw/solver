#include <CLI/CLI.hpp>

#include <parser.h>
#include <solver/solver.h>

#include <complex>
#include <exception>
#include <format>
#include <iostream>
#include <string>
#include <vector>

auto Near(double value, double target) -> bool
{
    return std::abs(value - target) < 1e-9;
}

auto PrintSolution(std::complex<double> solution) -> void
{
    double re = solution.real();
    double im = solution.imag();

    if (Near(re, 0.0) && !Near(im, 0.0)) {
        std::cout << std::format("{}i", im);
    }
    else if (!Near(re, 0.0) && Near(im, 0.0)) {
        std::cout << re;
    }
    else if (Near(re, 0.0) && Near(im, 0.0)) {
        std::cout << "0";
    }
    else {
        if (im < 0.0) {
            std::cout << std::format("{} - {}i", re, -im);
        }
        else {
            std::cout << std::format("{} + {}i", re, im);
        }
    }
}

auto main(int argc, char** argv) -> int
{
    auto app = CLI::App{"Polynomial solver"};

    // clang-format off

    std::string function;
    app.add_option("function", function, "The polynomial to solve")
        ->required();

    // clang-format on

    CLI11_PARSE(app, argc, argv);

    std::cout << std::format("Solving for: '{}'", function) << '\n';

    try {
        auto solutions = solver::Solve(function);

        if (!solutions) {
            std::cerr << "solver failed with error: " << solutions.error().message
                      << '\n';
            return 1;
        }

        if (solutions->empty()) {
            std::cout << "There are no solutions!\n";
        }
        else {
            std::cout << "Solutions:\n";
            for (auto solution : *solutions) {
                PrintSolution(solution);
                std::cout << ", ";
            }
            std::cout << '\n';
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    catch (...) {
        std::cerr << "caught unknown exception\n";
        return 1;
    }

    return 0;
}
