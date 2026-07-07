#pragma once

#include <backend/monomial.h>
#include <combinator/combinator.h>
#include <frontend/token.h>

#include <algorithm>
#include <span>
#include <vector>

namespace solver
{

inline auto Variable = Satisfy([](Token::Type t) { return t == Token::Type::Variable; });
inline auto Caret = Satisfy([](Token::Type t) { return t == Token::Type::Caret; });
inline auto Plus = Satisfy([](Token::Type t) { return t == Token::Type::Plus; });
inline auto Minus = Satisfy([](Token::Type t) { return t == Token::Type::Minus; });
inline auto Function = Satisfy([](Token::Type t) { return t == Token::Type::Function; });
inline auto Equals = Satisfy([](Token::Type t) { return t == Token::Type::Equals; });

inline auto ToInt = [](Token token) -> int { return std::stoi(token.lexeme); };
inline auto Number = Satisfy([](Token::Type t){ return t == Token::Type::Number; }).Map(ToInt);

/** @brief Parses the grammar rule:
 * 
 *         power ::= variable [ "^" number ]
 *
 *         If a variable is present without an exponent, we return the monomial x^1. If
 *         an exponent is present, we return x^n.
 */
inline auto PowerParser =
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
inline auto TermParser =
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
inline auto UnaryParser =
    (Maybe(Minus) & TermParser)
    .Map([](auto&& args) -> Monomial
    {
        auto& [minus, term] = args;
        return minus ? -term : term;
    });

/** @brief Parses the (pseudo) grammar rule:
 * 
 *         ( "+" | "-" ) unary
 * 
 *         Given a '+', simply passes through the parsed monomial. Given a '-', negates
 *         the parsed monomial.
 * 
 *         This rule does not exist in the grammar but is a subset of the expression
 *         parsing rule:
 * 
 *         expression ::= unary { ( "+" | "-" ) unary }
 */
inline auto BinaryParser =
    ((Plus | Minus) & UnaryParser)
    .Map([](auto&& args) -> Monomial
    {
        auto& [op, monomial] = args;
        return op.type == Token::Type::Minus ? -monomial : monomial;
    });

/** @brief Parses the grammar rule:
 * 
 *         expression ::= unary { ( "+" | "-" ) unary }
 * 
 *         Parses at least one monomial and right folds chained arithmetic operators,
 *         combining individual monomials into a collection of them.
 */
inline auto ExpressionParser =
    (UnaryParser & *BinaryParser)
    .Map([](auto&& args) -> std::vector<Monomial>
    {
        auto [first, rest] = args;
        rest.insert(rest.begin(), first);
        return rest;
    });

/** @brief Parses the grammar rule:
 *
 *         equation ::= "f(x)" "=" expression
 * 
 *         Checks for the presence of a leading "f(x) = ..." and returns the parsed
 *         expression.
 */
inline auto EquationParser = Function >> Equals >> ExpressionParser;

/** @brief Parse an equation
 */
auto Parse(std::vector<Token> const& source) -> std::vector<Monomial>;

} // namespace solver
