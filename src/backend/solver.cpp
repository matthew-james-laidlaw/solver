#include <backend/solver.h>

#include <complex>
#include <stdexcept>

using namespace std::complex_literals;

namespace solver
{

auto SolveFirstOrder(const Polynomial& equation) -> Solution
{
    if (equation.size() != 2) {
        throw std::runtime_error("expected a first order polynomial");
    }
    auto a = (double)equation[1];
    auto b = (double)equation[0];
    return {-b / a};
}

auto SolveSecondOrder(const Polynomial& equation) -> Solution
{
    if (equation.size() != 3) {
        throw std::runtime_error("expected a second order polynomial");
    }

    auto a = static_cast<std::complex<double>>(equation[2]);
    auto b = static_cast<std::complex<double>>(equation[1]);
    auto c = static_cast<std::complex<double>>(equation[0]);

    auto b2 = b * b;
    auto discriminant = 4.0 * a * c;

    auto x_0 = (-b + std::sqrt(b2 - discriminant)) / (a * 2.0);
    auto x_1 = (-b - std::sqrt(b2 - discriminant)) / (a * 2.0);

    return {x_0, x_1};
}

auto SolveThirdOrder(const Polynomial& equation) -> Solution
{
    if (equation.size() != 4) {
        throw std::runtime_error("expected a third order polynomial");
    }

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
        C = cube_root(delta_1 + std::sqrt(delta_1_2 - 3.0 * delta_0_3));
        if (C == 0.0) {
            C = cube_root(delta_1 - std::sqrt(delta_1_2 - 3.0 * delta_0_3));
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

auto SolveFourthOrder(const Polynomial& equation) -> Solution
{
    if (equation.size() != 5) {
        throw std::runtime_error("expected a fourth order polynomial");
    }
    throw std::runtime_error("fourth order solving not implemented");
}

auto Solve(const Polynomial& equation) -> Solution
{
    switch (equation.size()) {
    case 1:
        return {}; // constant function has no roots
    case 2:
        return SolveFirstOrder(equation);
    case 3:
        return SolveSecondOrder(equation);
    case 4:
        return SolveThirdOrder(equation);
    case 5:
        return SolveFourthOrder(equation);
    default:
        throw std::runtime_error("No general solution for polynomials of order >= 4. "
                                 "Iterative solution is not implemented");
    }
}

} // namespace solver
