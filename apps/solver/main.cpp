#include <CLI/CLI.hpp>
#include <backend/solver.h>
#include <frontend/parser.h>

#include <iostream>
#include <string>

auto main(int argc, char** argv) -> int
{
    auto app = CLI::App{"Polynomial solver"};

    // clang-format off

    std::string function;
    app.add_option("-f,--function", function, "The polynomial to solve")
        ->required();

    // clang-format on

    CLI11_PARSE(app, argc, argv);

    try {
        auto tokens = solver::Parse(function);
        auto solution = solver::Solve(tokens);
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
