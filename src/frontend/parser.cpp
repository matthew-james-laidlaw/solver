#include <frontend/parser.h>

namespace solver
{

auto Canonicalize(std::vector<Monomial> monomials) -> std::vector<Monomial>
{
    int order = 0;
    for (auto term : monomials)
    {
        order = std::max(order, term.Exponent());
    }

    std::vector<Monomial> polynomial(order, Monomial(0, 0));
    for (int i = 0; i < order; ++i)
    {
        polynomial[i] = Monomial(0, i);
    }

    for (auto term : monomials)
    {
        if (term.Exponent() < 0)
        {
            return {};
        }

        polynomial[term.Exponent()].AddCoefficient(term.Coefficient());
    }

    return polynomial;
}

auto Parse(std::vector<Token> const& source) -> std::vector<Monomial>
{
    std::span<const Token> state(source);
    auto result = EquationParser(state);
    if (!result.Succeeded())
    {
        return {};
    }
    return Canonicalize(result.Value());
}

} // namespace solver
