#include <parser.h>
#include <polynomial.h>
#include <solver/solver.h>

#include <cmath>
#include <complex>
#include <stdexcept>

using namespace std::complex_literals;

namespace
{

auto SolveFirstOrder(const solver::Polynomial& equation) -> solver::Solution
{
    auto a = (double)equation[1];
    auto b = (double)equation[0];
    return {-b / a};
}

auto SolveSecondOrder(const solver::Polynomial& equation) -> solver::Solution
{
    auto a = static_cast<std::complex<double>>(equation[2]);
    auto b = static_cast<std::complex<double>>(equation[1]);
    auto c = static_cast<std::complex<double>>(equation[0]);

    auto b2 = b * b;
    auto discriminant = 4.0 * a * c;

    auto x_0 = (-b + std::sqrt(b2 - discriminant)) / (a * 2.0);
    auto x_1 = (-b - std::sqrt(b2 - discriminant)) / (a * 2.0);

    return {x_0, x_1};
}

auto SolveThirdOrder(const solver::Polynomial& equation) -> solver::Solution
{
    auto a = static_cast<std::complex<double>>(equation[3]);
    auto b = static_cast<std::complex<double>>(equation[2]);
    auto c = static_cast<std::complex<double>>(equation[1]);
    auto d = static_cast<std::complex<double>>(equation[0]);

    auto delta_0 = (b * b) - (3.0 * a * c);
    auto delta_1 = (2.0 * b * b * b) - (9.0 * a * b * c) + (27.0 * a * a * d);

    auto cube_root = [](std::complex<double> expression) -> std::complex<double>
    { return std::pow(expression, 1.0 / 3.0); };

    auto delta_0_3 = delta_0 * delta_0 * delta_0;
    auto delta_1_2 = delta_1 * delta_1;

    std::complex<double> C;
    if (delta_0 == 0.0 && delta_1 == 0.0) {
        auto root = -b / (3.0 * a);
        return {root, root, root};
    }
    else {
        C = cube_root((delta_1 + std::sqrt(delta_1_2 - 4.0 * delta_0_3)) / 2.0);
        if (C == 0.0) {
            C = cube_root((delta_1 - std::sqrt(delta_1_2 - 4.0 * delta_0_3)) / 2.0);
        }
    }

    auto xi_0 = std::complex<double>(1.0);
    auto xi_1 = std::complex<double>(-1.0, std::sqrt(3.0)) / 2.0;
    auto xi_2 = std::complex<double>(-1.0, -std::sqrt(3.0)) / 2.0;

    auto x_0 = (-1.0 / (3.0 * a)) * (b + xi_0 * C + delta_0 / (xi_0 * C));
    auto x_1 = (-1.0 / (3.0 * a)) * (b + xi_1 * C + delta_0 / (xi_1 * C));
    auto x_2 = (-1.0 / (3.0 * a)) * (b + xi_2 * C + delta_0 / (xi_2 * C));

    return {x_0, x_1, x_2};
}

auto SolvePolynomial(const solver::Polynomial& equation)
    -> std::expected<solver::Solution, solver::Error>
{
    switch (equation.Size()) {
    case 1:
        return {}; // constant function has no roots
    case 2:
        return SolveFirstOrder(equation);
    case 3:
        return SolveSecondOrder(equation);
    case 4:
        return SolveThirdOrder(equation);
    case 5:
        return std::unexpected(
            solver::Error(solver::Error::Type::NotImplemented,
                          "solving for polynomials of order 4 is not implemented"));
    default:
        return std::unexpected(
            solver::Error(solver::Error::Type::NotImplemented,
                          "solving for polynomials of order > 4 is not implemented"));
    }
}

} // namespace

namespace solver
{

auto Solve(const std::string& equation) -> std::expected<Solution, Error>
{
    auto polynomial = Parse(equation);
    if (!polynomial) {
        return std::unexpected(polynomial.error());
    }

    auto solutions = SolvePolynomial(*polynomial);
    return solutions;
}

} // namespace solver
