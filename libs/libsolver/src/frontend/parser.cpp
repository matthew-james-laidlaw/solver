#include <backend/monomial.h>
#include <combinator/combinator.h>
#include <frontend/grammar.h>
#include <frontend/lexer.h>
#include <frontend/parser.h>
#include <frontend/result.h>
#include <frontend/token.h>

#include <algorithm>
#include <span>
#include <stdexcept>

namespace solver
{

/** @brief Sort and combine terms into a single list of monomials
 * who's size is the order of the polynomial.
 */
auto Canonicalize(std::vector<Monomial> monomials) -> Polynomial
{
    int order = 0;
    for (auto term : monomials) {
        order = std::max(order, term.Exponent());
    }

    Polynomial polynomial(order + 1, 0);
    for (auto term : monomials) {
        if (term.Exponent() >= 0) {
            polynomial[term.Exponent()] += term.Coefficient();
        }
    }

    return polynomial;
}

auto Parse(const std::vector<Token>& source) -> Polynomial
{
    std::span<const Token> state(source);
    auto result = EquationParser(state);
    if (!result.Succeeded()) {
        throw std::runtime_error(result.Message());
    }
    return Canonicalize(result.Value());
}

auto Parse(const std::string& source) -> Polynomial
{
    auto lexed = Lex(source);
    return Parse(lexed);
}

} // namespace solver
