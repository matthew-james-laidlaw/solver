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

auto ToInt = [](Token const& t)
{
    return std::stoi(t.lexeme);
};

auto power =
    Combine(
        Variable,
        Maybe(Caret >> Number.map(ToInt)),
        [](Token const&, std::optional<int> exp)
        {
            return Monomial{1, exp.value_or(1)};
        }
    );

auto term =
        Combine(
            Number.map(ToInt),
            Maybe(power),
            [](int coefficient, std::optional<Monomial> power)
            {
                if (power)
                {
                    return Monomial(coefficient, power->Exponent());
                }
                else
                {
                    return Monomial(coefficient, 0);
                }
            }
        ) | power;

auto unary = 
    Combine(
        Maybe(Minus),
        term,
        [](std::optional<Token> minus, Monomial m)
        {
            if (minus)
            {
                return Monomial(-m.Coefficient(), m.Exponent());
            }
            else
            {
                return m;
            }
        }
    );

// auto binary_term = ((Plus | Minus) & unary)

auto binary_term =
    Combine(
        Plus | Minus,
        unary,
        [](Token op, Monomial term)
        {
            if (op.type == Token::Type::Minus)
            {
                term = Monomial(-term.Coefficient(), term.Exponent());
            }

            return term;
        }
    );

auto expression =
    Combine(
        unary,
        ZeroOrMore(binary_term),
        [](Monomial first, std::vector<Monomial> rest)
        {
            rest.insert(rest.begin(), first);
            return rest;
        }
    );

auto equation = Sequence<Token>({Function, Equals}) >> expression;

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
