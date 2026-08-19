#pragma once

#include <combinator/combinator.h>
#include <monomial.h>
#include <token.h>

#include <vector>

namespace solver
{

/* ------------------------- *
 |     Parser Primitives     |
 * ------------------------- */

auto Variable() -> const combinator::Parser<Token, Token>&;
auto Caret() -> const combinator::Parser<Token, Token>&;
auto Plus() -> const combinator::Parser<Token, Token>&;
auto Minus() -> const combinator::Parser<Token, Token>&;
auto Function() -> const combinator::Parser<Token, Token>&;
auto Equals() -> const combinator::Parser<Token, Token>&;
auto Number() -> const combinator::Parser<int, Token>&;

/* --------------------- *
 |     Grammar Rules     |
 * --------------------- */

/** @brief Parses the grammar rule:
 *
 *         power ::= variable [ "^" number ]
 *
 *         If a variable is present without an exponent, we return the monomial x^1. If
 *         an exponent is present, we return x^n.
 */
auto PowerParser() -> const combinator::Parser<Monomial, Token>&;

/** @brief Parses the grammar rule:
 *
 *         term ::= number [ power ] | power
 *
 *         If a coefficient is present without a power, we return the monomial cx^0. If
 *         both are present, we return cx^n. If no coefficient is present we return x^n.
 */
auto TermParser() -> const combinator::Parser<Monomial, Token>&;

/** @brief Parses the grammar rule:
 *
 *         unary ::= [ "-" ] term
 *
 *         If a minus is present we negate whatever is parsed from 'term'. Otherwise, we
 *         simply pass through the parsed term.
 */
auto UnaryParser() -> const combinator::Parser<Monomial, Token>&;

/** @brief Parses the (pseudo) grammar rule:
 *
 *         ( "+" | "-" ) unary
 *
 *         This rule does not explicitly exist in the grammar but is a subset of the
 *         expression parsing rule:
 *
 *         expression ::= unary { ( "+" | "-" ) unary }
 */
auto BinaryParser() -> const combinator::Parser<Monomial, Token>&;

/** @brief Parses the grammar rule:
 *
 *         expression ::= unary { ( "+" | "-" ) unary }
 *
 *         Parses at least one monomial and right folds chained arithmetic operators,
 *         combining individual monomials into a collection of them.
 */
auto ExpressionParser() -> const combinator::Parser<std::vector<Monomial>, Token>&;

/** @brief Parses the grammar rule:
 *
 *         equation ::= "f(x)" "=" expression
 *
 *         Checks for the presence of a leading "f(x) = ..." and returns the parsed
 *         expression.
 */
auto EquationParser() -> const combinator::Parser<std::vector<Monomial>, Token>&;

} // namespace solver
