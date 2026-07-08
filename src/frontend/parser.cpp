#include <frontend/grammar.h>
#include <frontend/lexer.h>
#include <frontend/parser.h>

#include <span>

namespace solver
{

auto Canonicalize(std::vector<Monomial> monomials) -> std::vector<Monomial>
{
    int order = 0;
    for (auto term : monomials) {
        order = std::max(order, term.Exponent());
    }

    std::vector<Monomial> polynomial(order + 1, Monomial(0, 0));
    for (int i = 0; i < (int)polynomial.size(); ++i) {
        polynomial[i] = Monomial(0, i);
    }

    for (auto term : monomials) {
        if (term.Exponent() < 0) {
            return {};
        }
        polynomial[term.Exponent()] = polynomial[term.Exponent()] + term.Coefficient();
    }

    return polynomial;
}

auto Parse(std::vector<Token> const& source) -> std::vector<Monomial>
{
    std::span<Token const> state(source);
    auto result = EquationParser(state);
    if (!result.Succeeded()) {
        return {};
    }
    return Canonicalize(result.Value());
}

auto Parse(std::string const& source) -> std::vector<Monomial>
{
    auto lexed = Lex(source);
    return Parse(lexed);
}

} // namespace solver
