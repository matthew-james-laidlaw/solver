#include <Solver.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace
{

using Polynomial = std::vector<std::complex<double>>;

auto operator+(Polynomial const& lhs, Polynomial const& rhs) -> Polynomial
{
    auto sum = lhs;

    if (rhs.size() > sum.size())
    {
        sum.resize(rhs.size());
    }

    for (size_t i = 0; i < rhs.size(); ++i)
    {
        sum[i] += rhs[i];
    }
    
    return sum;
}

auto operator-(Polynomial const& lhs, Polynomial const& rhs) -> Polynomial
{
    auto sum = lhs;
    
    if (rhs.size() > sum.size())
    {
        sum.resize(rhs.size());
    }

    for (size_t i = 0; i < rhs.size(); ++i)
    {
        sum[i] -= rhs[i];
    }
    
    return sum;
}

auto operator*(Polynomial const& lhs, Polynomial const& rhs) -> Polynomial
{
    throw std::runtime_error("not implemented");
}

auto operator/(Polynomial const& lhs, Polynomial const& rhs) -> Polynomial
{
    throw std::runtime_error("not implemented");
}

auto operator-(Polynomial const& operand) -> Polynomial
{
    throw std::runtime_error("not implemented");
}

auto MakePolynomial(Term const& term) -> Polynomial
{
    if (term.Exponent() < 0)
    {
        throw std::runtime_error("only positive order polynomials are allowed");
    }

    Polynomial poly(term.Exponent() + 1, 0);
    poly[term.Exponent()] = term.Coefficient();

    return poly;
}

auto Reduce(Expression const* expr) -> Polynomial
{
    if (auto term = dynamic_cast<Term const*>(expr))
    {
        return MakePolynomial(*term);
    }
    else if (auto bin = dynamic_cast<Binary const*>(expr))
    {
        auto lhs = Reduce(bin->Left());
        auto rhs = Reduce(bin->Right());
        switch (bin->Op())
        {
        case '+':
            return lhs + rhs;
        case '-':
            return lhs - rhs;
        case '*':
            return lhs * rhs;
        case '/':
            return lhs / rhs;
        default:
            throw std::runtime_error("unexpected binary operator");
        }
    }
    else if (auto un = dynamic_cast<Unary const*>(expr))
    {
        auto operand = Reduce(un->Operand());
        switch (un->Op())
        {
        case '-':
            return -operand;
        default:
            throw std::runtime_error("unexpected unary operator");
        }
    }
    else
    {
        throw std::runtime_error("unexpected expression type");
    }
}

auto Reduce(Equality const& equation) -> Polynomial
{
    auto lhs = Reduce(equation.Left());
    auto rhs = Reduce(equation.Right());
    return lhs - rhs;
}

auto SolveTrivial(Polynomial const& poly) -> Solution
{
    if (poly.size() == 1)
    {
        return {};
    }
    else if (poly.size() == 2)
    {
        auto a = poly[1];
        auto b = poly[0];
        return { -b / a };
    }
    else
    {
        throw std::runtime_error("expected a constant or linear equation");
    }
}

auto SolveQuadratic(Polynomial const& poly) -> Solution
{
    if (poly.size() != 3)
    {
        throw std::runtime_error("expected a quadratic equation");
    }
    
    auto a = poly[2];
    auto b = poly[1];
    auto c = poly[0];

    auto discriminant = std::sqrt(std::complex<double>((b * b) - (4.0 * a * c)));

    auto x1 = (-b + discriminant) / (2.0 * a);
    auto x2 = (-b - discriminant) / (2.0 * a);

    return { x1, x2 };
}

auto SolveCubic(Polynomial const& poly) -> Solution
{
    if (poly.size() != 4)
    {
        throw std::runtime_error("expected a cubic equation");
    }

    auto a = poly[3];
    auto b = poly[2];
    auto c = poly[1];
    auto d = poly[0];

    auto delta_0 = (b * b) - (3.0 * a * c);
    auto delta_1 = (2.0 * b * b * b) - (9.0 * a * b * c) + (27.0 * a * a * d);

    auto C = std::pow((delta_1 + std::sqrt((delta_1 * delta_1) - (4.0 * delta_0 * delta_0 * delta_0))) / 2.0, 1.0/3.0);

    auto epsilon_0 = 1.0;
    auto epsilon_1 = (-1.0 + std::sqrt(std::complex<double>(-3.0))) / 2.0;
    auto epsilon_2 = (-1.0 - std::sqrt(std::complex<double>(-3.0))) / 2.0;

    auto x0 = (-1.0 / (3.0 * a)) * (b + (epsilon_0 * C) + (delta_0 / (epsilon_0 * C)));
    auto x1 = (-1.0 / (3.0 * a)) * (b + (epsilon_1 * C) + (delta_0 / (epsilon_1 * C)));
    auto x2 = (-1.0 / (3.0 * a)) * (b + (epsilon_2 * C) + (delta_0 / (epsilon_2 * C)));

    return { x0, x1, x2 };
}

auto SolveQuartic(Polynomial const& poly) -> Solution
{
    throw std::runtime_error("not implemented");
}

auto Solve(Polynomial const& poly) -> Solution
{
    switch (poly.size())
    {
        case 1:
        case 2:
            return SolveTrivial(poly);
        case 3:
            return SolveQuadratic(poly);
        case 4:
            return SolveCubic(poly);
        case 5:
            return SolveQuartic(poly);
        default:
            throw std::runtime_error("cannot solve for a polynomial of given order");
    }
}

} // namespace

auto Solve(Equality const& equation) -> Solution
{
    auto poly = Reduce(equation);
    auto solution = Solve(poly);

    std::sort(solution.begin(), solution.end(),
    [](std::complex<double> const& a, std::complex<double> const& b)
    {
        if (a.real() != b.real())
        {
            return a.real() < b.real();
        }
        return a.imag() < b.imag();
    });

    return solution;
}
