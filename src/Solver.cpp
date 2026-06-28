#include <Solver.h>

#include <stdexcept>

namespace
{

using Polynomial = std::vector<int>;

auto operator+(Polynomial const& lhs, Polynomial const& rhs) -> Polynomial
{
    throw std::runtime_error("not implemented");
}

auto operator-(Polynomial const& lhs, Polynomial const& rhs) -> Polynomial
{
    throw std::runtime_error("not implemented");
}

auto operator*(Polynomial const& lhs, Polynomial const& rhs) -> Polynomial
{
    throw std::runtime_error("not implemented");
}

auto operator/(Polynomial const& lhs, Polynomial const& rhs) -> Polynomial
{
    throw std::runtime_error("not implemented");
}

auto Reduce(Expression const& expr) -> Polynomial
{
    throw std::runtime_error("not implemented");
}

auto Reduce(Equality const& equation) -> Polynomial
{
    auto lhs = Reduce(*equation.Left());
    auto rhs = Reduce(*equation.Right());
    return lhs - rhs;
}

auto Solve(Polynomial const& poly) -> std::vector<int>
{
    throw std::runtime_error("not implemented");
}

} // namespace

auto Solve(Equality const& equation) -> std::vector<int>
{
    auto poly = Reduce(equation);
    return Solve(poly);
}
