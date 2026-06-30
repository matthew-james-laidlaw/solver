#include <Parser.h>

#include <cctype>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

// -----------------------------------------------------------------------------
// AST interface this file expects from <Parser.h>
//
//   struct Expression { ... };                                   // base
//   struct Term   : Expression { double coeff; int exponent; };  // coeff * x^exponent
//   struct Unary  : Expression { char op; Expression* operand; };          // -p
//   struct Binary : Expression { char op; Expression* lhs, * rhs; };       // a + b ...
//
// Term encapsulates every leaf of the grammar:
//     number    n      ->  Term{ n,   0 }   (exponent 0 == constant)
//     variable  x      ->  Term{ 1.0, 1 }
//     monomial  n x    ->  Term{ n,   1 }
//
// IMPORTANT: Term::coeff must be floating-point (double), because the grammar
// admits decimals like 2.5. If your Term currently stores an int coefficient,
// widen it to double or 2.5 cannot be represented. exponent is only ever 0 or 1
// here, since the grammar has no exponentiation rule.
//
// Unary still wraps negation so "-(x + 1)" works; Binary builds the +,-,*,/
// nodes. The old Equality node is not produced ("f(x) =" is a fixed prefix).
// -----------------------------------------------------------------------------

namespace
{

// State is now a character stream, not a token stream.
using State = std::string_view;

template <typename T>
struct InnerParseResult
{
    T result;
    State rest;
};

template <typename T>
using ParseResult = std::optional<InnerParseResult<T>>;

template <typename T>
using Parser = std::function<ParseResult<T>(State)>;

using Predicate = std::function<bool(char)>;

auto Satisfy(Predicate pred) -> Parser<char>
{
    return [pred](State source) -> ParseResult<char>
    {
        if (source.empty() || !pred(source.front()))
        {
            return std::nullopt;
        }

        return InnerParseResult<char>{
            .result = source.front(),
            .rest = source.substr(1),
        };
    };
}

auto Character(char expected) -> Parser<char>
{
    return Satisfy([expected](char actual) { return actual == expected; });
}

auto OneOf(std::initializer_list<char> chars) -> Parser<char>
{
    // Copy into an owning string: an initializer_list is a non-owning view whose
    // backing array dies at the end of the full-expression, so capturing it by
    // value (as the original did) dangles once the returned parser is invoked.
    std::string set(chars.begin(), chars.end());

    return [set](State source) -> ParseResult<char>
    {
        if (source.empty() || set.find(source.front()) == std::string::npos)
        {
            return std::nullopt;
        }

        return InnerParseResult<char>{
            .result = source.front(),
            .rest = source.substr(1),
        };
    };
}

// Matches a fixed literal string, e.g. ExpectString("f(x)=").
// Returns the matched text as a view into the *input* (like ParseNumber), so it
// stays valid for as long as the input does.
auto ExpectString(std::string_view expected) -> Parser<std::string_view>
{
    // Own the needle: capturing the string_view by value would dangle if a caller
    // passed a temporary std::string (same trap OneOf had).
    return [expected = std::string(expected)](State source) -> ParseResult<std::string_view>
    {
        State s = source;
        for (char c : expected)
        {
            auto r = Character(c)(s);
            if (!r)
            {
                return std::nullopt;
            }
            s = r->rest;
        }

        State matched = source.substr(0, source.size() - s.size());
        return InnerParseResult<std::string_view>{ .result = matched, .rest = s };
    };
}

// Generic left-associative chain:  sub { op sub }
// Used for both the additive and the multiplicative levels.
auto ParseLeftAssociative(
    State source,
    Parser<Expression*> const& sub,
    std::initializer_list<char> ops)
    -> ParseResult<Expression*>
{
    auto lhs = sub(source);
    if (!lhs)
    {
        return std::nullopt;
    }

    auto* expr = lhs->result;
    auto rest = lhs->rest;

    auto op_parser = OneOf(ops);
    while (auto op = op_parser(rest))
    {
        auto rhs = sub(op->rest);
        if (!rhs)
        {
            return std::nullopt;
        }

        expr = new Binary{ op->result, expr, rhs->result };
        rest = rhs->rest;
    }

    return InnerParseResult<Expression*>{ .result = expr, .rest = rest };
}

// Forward declarations: primary refers back to expression (via parentheses),
// so the grammar functions are mutually recursive.
auto ParseExpression() -> Parser<Expression*>;
auto ParseTerm()       -> Parser<Expression*>;
auto ParseFactor()     -> Parser<Expression*>;
auto ParsePrimary()    -> Parser<Expression*>;
auto ParseNumber()     -> Parser<Expression*>;
auto ParseVariable()   -> Parser<Expression*>;
auto ParseDigit()      -> Parser<char>;

// digit ::= "0" | ... | "9"
auto ParseDigit() -> Parser<char>
{
    return Satisfy([](char c)
    {
        return std::isdigit(static_cast<unsigned char>(c)) != 0;
    });
}

// number ::= digit { digit } [ "." digit { digit } ]
auto ParseNumber() -> Parser<Expression*>
{
    return [](State source) -> ParseResult<Expression*>
    {
        State s = source;
        auto digit = ParseDigit();

        // integer part: at least one digit required
        if (!digit(s))
        {
            return std::nullopt;
        }
        while (auto d = digit(s))
        {
            s = d->rest;
        }

        // optional fractional part: only commit to the '.' if a digit follows,
        // otherwise leave the '.' in the stream (so "2." yields the number 2).
        if (!s.empty() && s.front() == '.')
        {
            if (auto d = digit(s.substr(1)))
            {
                s = d->rest;
                while (auto dd = digit(s))
                {
                    s = dd->rest;
                }
            }
        }

        State text = source.substr(0, source.size() - s.size());
        try
        {
            double value = std::stod(std::string(text));
            return InnerParseResult<Expression*>{ .result = new Term{ value, 0 }, .rest = s };
        }
        catch (std::exception const&)   // out_of_range on absurdly large literals
        {
            return std::nullopt;
        }
    };
}

// variable ::= "x"
auto ParseVariable() -> Parser<Expression*>
{
    return [](State source) -> ParseResult<Expression*>
    {
        if (auto r = Character('x')(source))
        {
            return InnerParseResult<Expression*>{ .result = new Term{ 1.0, 1 }, .rest = r->rest };
        }
        return std::nullopt;
    };
}

// primary ::= number [ variable ] | variable | "(" expression ")"
auto ParsePrimary() -> Parser<Expression*>
{
    return [](State source) -> ParseResult<Expression*>
    {
        // number [ variable ]
        if (auto num = ParseNumber()(source))
        {
            // A number immediately followed by x is a monomial (3x, 2.5x).
            // num->result is a Term we just made with exponent 0; promote it in
            // place to coeff * x by setting exponent 1 — no extra node needed.
            if (auto var = Character('x')(num->rest))
            {
                static_cast<Term*>(num->result)->m_exponent = 1;
                return InnerParseResult<Expression*>{ .result = num->result, .rest = var->rest };
            }
            return num;   // bare number: Term{value, 0}
        }

        // variable
        if (auto var = ParseVariable()(source))
        {
            return var;
        }

        // "(" expression ")"
        if (auto open = Character('(')(source))
        {
            auto inner = ParseExpression()(open->rest);
            if (!inner)
            {
                return std::nullopt;
            }
            auto close = Character(')')(inner->rest);
            if (!close)
            {
                return std::nullopt;
            }
            return InnerParseResult<Expression*>{ .result = inner->result, .rest = close->rest };
        }

        return std::nullopt;
    };
}

// factor ::= [ "-" ] primary
auto ParseFactor() -> Parser<Expression*>
{
    return [](State source) -> ParseResult<Expression*>
    {
        if (auto neg = Character('-')(source))
        {
            auto operand = ParsePrimary()(neg->rest);
            if (!operand)
            {
                return std::nullopt;
            }
            return InnerParseResult<Expression*>{
                .result = new Unary{ '-', operand->result },
                .rest = operand->rest,
            };
        }
        return ParsePrimary()(source);
    };
}

// term ::= factor { ("*" | "/") factor }
auto ParseTerm() -> Parser<Expression*>
{
    return [](State source)
    {
        return ParseLeftAssociative(source, ParseFactor(), { '*', '/' });
    };
}

// expression ::= term { ("+" | "-") term }
auto ParseExpression() -> Parser<Expression*>
{
    return [](State source)
    {
        return ParseLeftAssociative(source, ParseTerm(), { '+', '-' });
    };
}

// function ::= "f(x)" "=" expression
auto ParseFunction() -> Parser<Expression*>
{
    return [](State source) -> ParseResult<Expression*>
    {
        auto prefix = ExpectString("f(x)=")(source);
        if (!prefix)
        {
            return std::nullopt;
        }

        // The "f(x) =" prefix is fixed and carries no parseable data, so the
        // function's value is simply its right-hand expression.
        return ParseExpression()(prefix->rest);
    };
}

} // namespace

auto Parse(std::string const& source) -> Expression*
{
    // Whitespace is never significant in this grammar, so strip it once up front
    // and parse the compact character stream. (This replaces the old whitespace
    // tokenizer, which cannot express things like "3x" or "(x-1)".)
    std::string compact;
    compact.reserve(source.size());
    for (char c : source)
    {
        if (!std::isspace(static_cast<unsigned char>(c)))
        {
            compact += c;
        }
    }

    auto result = ParseFunction()(compact);

    // Require the whole input to be consumed, so trailing garbage like
    // "f(x)=2+" or "f(x)=2)" is rejected instead of silently accepted.
    if (!result || !result->rest.empty())
    {
        throw std::runtime_error("parser error");
    }

    return result->result;
}
