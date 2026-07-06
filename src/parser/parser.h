#pragma once

#include <parser/combinator.h>
#include <parser/expression.h>
#include <parser/token.h>

namespace solver
{

auto Variable = Satisfy([](Token::Type t)
{
    return t == Token::Type::Variable;
});

auto Number = Satisfy([](Token::Type t)
{
    return t == Token::Type::Number;
}).Map([](Token const& token)
{
    return std::stoi(token.lexeme);
});

auto Caret = Satisfy([](Token::Type t)
{
    return t == Token::Type::Caret;
});

auto Plus = Satisfy([](Token::Type t)
{
    return t == Token::Type::Plus;
});

auto Minus = Satisfy([](Token::Type t)
{
    return t == Token::Type::Minus;
});

auto Function = Satisfy([](Token::Type t)
{
    return t == Token::Type::Function;
});

auto Equals = Satisfy([](Token::Type t)
{
    return t == Token::Type::Equals;
});

/** @brief Parses the grammar rule:
 * 
 *         power ::= variable [ "^" number ]
 *
 *         If a variable is present without an exponent, we return the monomial x^1. If
 *         an exponent is present, we return x^n.
 */
auto PowerParser =
    (Variable >> Maybe(Caret >> Number))
    .Map([](std::optional<int> exponent) -> Monomial
    {
        return Monomial::Variable(exponent.value_or(1));
    });

/** @brief Parses the grammar rule:
 * 
 *         term ::= number [ power ] | power
 * 
 *         If a coefficient is present without a power, we return the monomial cx^0. If
 *         both are present, we return cx^n. If no coefficient is present we return x^n.
 */
auto TermParser =
    (Number & Maybe(PowerParser))
    .Map([](auto&& args) -> Monomial
    {
        auto& [coefficient, power] = args;
        return power ? Monomial(coefficient, power->Exponent()) : Monomial::Constant(coefficient);
    })
    | PowerParser;

/** @brief Parses the grammar rule:
 * 
 *         unary ::= [ "-" ] term
 * 
 *         If a minus is present we negate whatever is parsed from 'term'. Otherwise, we
 *         simply pass through the parsed term.
 */
auto UnaryParser =
    (Maybe(Minus) & TermParser)
    .Map([](auto&& args) -> Monomial
    {
        auto& [minus, term] = args;
        return minus ? -term : term;
    });

auto binary_term =
    Combine(
        Plus | Minus,
        UnaryParser,
        [](Token op, Monomial term)
        {
            if (op.type == Token::Type::Minus)
            {
                term = Monomial(-term.Coefficient(), term.Exponent());
            }

            return term;
        }
    );

auto ExpressionParser =
    Combine(
        UnaryParser,
        ZeroOrMore(binary_term),
        [](Monomial first, std::vector<Monomial> rest)
        {
            rest.insert(rest.begin(), first);
            return rest;
        }
    );

auto equation = Sequence<Token>({Function, Equals}) >> ExpressionParser;

inline auto Canonicalize(std::vector<Monomial> monomials) -> std::vector<Monomial>
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

        polynomial[term.Exponent()].m_coefficient += term.Coefficient();
    }

    return polynomial;
}

inline auto Parse(std::vector<Token> const& source) -> std::vector<Monomial>
{
    std::span<const Token> state(source);
    auto result = equation(state);
    if (!result.Succeeded())
    {
        return {};
    }
    return Canonicalize(result.Value());
}

} // namespace solver
