#include <frontend/grammar.h>

#include <backend/monomial.h>
#include <combinator/combinator.h>
#include <frontend/token.h>

#include <optional>
#include <string>
#include <vector>

namespace solver
{

/* ------------------------- *
 |     Parser Primitives     |
 * ------------------------- */

auto Variable() -> const combinator::Parser<Token, Token>&
{
    static auto parser = combinator::Expect<Token>(Token::Type::Variable, "a variable");
    return parser;
}

auto Caret() -> const combinator::Parser<Token, Token>&
{
    static auto parser = combinator::Expect<Token>(Token::Type::Caret, "'^'");
    return parser;
}

auto Plus() -> const combinator::Parser<Token, Token>&
{
    static auto parser = combinator::Expect<Token>(Token::Type::Plus, "'+'");
    return parser;
}

auto Minus() -> const combinator::Parser<Token, Token>&
{
    static auto parser = combinator::Expect<Token>(Token::Type::Minus, "'-'");
    return parser;
}

auto Function() -> const combinator::Parser<Token, Token>&
{
    static auto parser = combinator::Expect<Token>(Token::Type::Function, "'f(x)'");
    return parser;
}

auto Equals() -> const combinator::Parser<Token, Token>&
{
    static auto parser = combinator::Expect<Token>(Token::Type::Equals, "'='");
    return parser;
}

namespace
{

auto ToInt(Token token) -> int
{
    return std::stoi(token.lexeme);
}

} // namespace

auto Number() -> const combinator::Parser<int, Token>&
{
    static auto parser = combinator::Expect<Token>(Token::Type::Number, "a number").Map(ToInt);
    return parser;
}

/* --------------------- *
 |     Grammar Rules     |
 * --------------------- */

auto PowerParser() -> const combinator::Parser<Monomial, Token>&
{
    static auto parser =
        (Variable() >> Maybe(Caret() >> Number()))
        .Map([](std::optional<int> exponent) -> Monomial
        {
            return Monomial::Variable(exponent.value_or(1));
        });
    return parser;
}

auto TermParser() -> const combinator::Parser<Monomial, Token>&
{
    static auto parser =
        (Number() & Maybe(PowerParser()))
        .Map([](auto&& args) -> Monomial
        {
            auto& [coefficient, power] = args;
            return power ? Monomial(coefficient, power->Exponent()) : Monomial::Constant(coefficient);
        })
        | PowerParser();
    return parser;
}

auto UnaryParser() -> const combinator::Parser<Monomial, Token>&
{
    static auto parser =
        (Maybe(Minus()) & TermParser())
        .Map([](auto&& args) -> Monomial
        {
            auto& [minus, term] = args;
            return minus ? -term : term;
        });
    return parser;
}

auto BinaryParser() -> const combinator::Parser<Monomial, Token>&
{
    static auto parser =
        ((Plus() | Minus()) & UnaryParser())
        .Map([](auto&& args) -> Monomial
        {
            auto& [op, monomial] = args;
            return op.type == Token::Type::Minus ? -monomial : monomial;
        });
    return parser;
}

auto ExpressionParser() -> const combinator::Parser<std::vector<Monomial>, Token>&
{
    static auto parser =
        (UnaryParser() & *BinaryParser())
        .Map([](auto&& args) -> std::vector<Monomial>
        {
            auto [first, rest] = args;
            rest.insert(rest.begin(), first);
            return rest;
        });
    return parser;
}

auto EquationParser() -> const combinator::Parser<std::vector<Monomial>, Token>&
{
    static auto parser = Function() >> Equals() >> ExpressionParser();
    return parser;
}

} // namespace solver
