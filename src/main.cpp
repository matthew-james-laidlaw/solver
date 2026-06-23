#include <Expression.h>
#include <Parser.h>

#include <iostream>

auto ParsePrimary() -> Parser<Expression*>
{
    return [](std::span<std::string> source) -> ParseResult<Expression*>
    {
        if (auto n = Number()(source))
        {
            return n;
        }
        if (auto b = Boolean()(source))
        {
            return b;
        }
        if (auto s = String()(source))
        {
            return s;
        }
        return std::nullopt;
    };
}

auto ParseUnary() -> Parser<Expression*>
{
	return [](std::span<std::string> source) -> ParseResult<Expression*>
    {
        for (char op : { '!', '-' })
        {
            if (auto c = Character(op)(source))
            {
                if (auto rhs = ParseUnary()(c->second))
                {
                    return std::pair{new Unary{op, rhs->first}, rhs->second};
                }
                return std::nullopt;
            }
        }
        return ParsePrimary()(source);
    };
}

auto OneOf(std::initializer_list<char> chars)
    -> Parser<char>
{
    return [chars](std::span<std::string> source)
        -> ParseResult<char>
    {
        for (char c : chars)
        {
            if (auto r = Character(c)(source))
            {
                return std::pair{c, r->second};
            }
        }

        return std::nullopt;
    };
}

template<typename SubParser>
auto ParseLeftAssociative(
    std::span<std::string> source,
    SubParser sub,
    std::initializer_list<char> ops)
    -> ParseResult<Expression*>
{
    auto lhs = sub(source);
    if (!lhs)
    {
        return std::nullopt;
    }

    auto expr = lhs->first;
    auto rest = lhs->second;

    while (auto op = OneOf(ops)(rest))
    {
        auto rhs = sub(op->second);
        if (!rhs)
        {
            return std::nullopt;
        }

        expr = new Binary{op->first, expr, rhs->first};
        rest = rhs->second;
    }

    return std::pair{expr, rest};
}

auto ParseFactor() -> Parser<Expression*>
{
    return [](std::span<std::string> source)
    {
        return ParseLeftAssociative(
            source,
            ParseUnary(),
            {'*', '/'});
    };
}

auto ParseTerm() -> Parser<Expression*>
{
    return [](std::span<std::string> source)
    {
        return ParseLeftAssociative(
            source,
            ParseFactor(),
            {'+', '-'});
    };
}

auto ParseEquality() -> Parser<Expression*>
{
    return [](std::span<std::string> source) -> ParseResult<Expression*>
    {
        auto lhs = ParseTerm()(source);
        if (!lhs)
        {
            return std::nullopt;
        }

        if (auto c = Character('=')(lhs->second))
        {
            if (auto rhs = ParseTerm()(c->second))
            {
                return std::pair{ new Equality{ lhs->first, rhs->first }, rhs->second };
            }
            return std::nullopt;
        }

        return lhs;
    };
}

auto Show(Expression* e) -> void
{
    if (auto* lit = dynamic_cast<Literal*>(e))
    {
        if (auto* i = std::any_cast<int>(&lit->value))
        {
            std::cout << "Number(" << *i << ")";
        }
        else if (auto* b = std::any_cast<bool>(&lit->value))
        {
            std::cout << "Bool(" << (*b ? "true" : "false") << ")";
        }
        else if (auto* s = std::any_cast<std::string>(&lit->value))
        {
            std::cout << "String(\"" << *s << "\")";
        }
        else
        {
            std::cout << "Literal(<unknown type: " << lit->value.type().name() << ">)";
        }
    }
    else if (auto* un = dynamic_cast<Unary*>(e))
    {
        std::cout << "Unary(" << un->op << ", ";
        Show(un->operand);
        std::cout << ")";
    }
    else if (auto* bin = dynamic_cast<Binary*>(e))
    {
        std::cout << "Binary(" << bin->op << ", ";
        Show(bin->lhs);
        std::cout << ", ";
        Show(bin->rhs);
        std::cout << ")";
    }
    else
    {
        std::cout << "<unknown Expression>";
    }
}

auto PrintTokens(std::span<std::string> const& tokens) -> void
{
    for (auto const& token : tokens)
    {
        std::cout << '"' << token << "\", ";
    }
    std::cout << '\n';
}

auto main() -> int
{
    std::vector<std::vector<std::string>> tests = {
        // { "42" },
        // { "-", "42" },
        // { "!", "true" },
        // { "!", "!", "true" },
        // { "-", "\"", "oops", "\""},
        // { "!" },
        // { "1", "+", "2" },
        // { "3", "*", "2", "+", "1"},
        { "x", "+", "5", "=", "8" },
    };

    for (auto& test : tests)
    {
        PrintTokens(test);
        auto term = ParseEquality()(std::span(test));
        if (term)
        {
            Show(term->first);
        }
        else
        {
            std::cout << "parse error";
        }
        std::cout << "\n\n";
    }
	return 0;
}
