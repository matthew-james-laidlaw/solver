#include <Expression.h>
#include <Parser.h>

#include <iostream>

auto ParsePrimary() -> Parser<Expression*>
{
    return [](std::span<std::string> in) -> ParseResult<Expression*>
    {
        if (auto n = Number()(in))
        {
            return n;
        }
        if (auto b = Boolean()(in))
        {
            return b;
        }
        if (auto s = String()(in))
        {
            return s;
        }
        return std::nullopt;
    };
}

auto ParseUnary() -> Parser<Expression*>
{
	return [](std::span<std::string> in) -> ParseResult<Expression*>
    {
        for (char op : { '!', '-' })
        {
            if (auto o = Character(op)(in))
            {
                if (auto rest = ParseUnary()(o->second))
                {
                    return std::pair{new Unary{op, rest->first}, rest->second};
                }
                return std::nullopt;
            }
        }
        return ParsePrimary()(in);
    };
}

auto ParseFactor() -> Parser<Expression*>
{
    return [](std::span<std::string> in) -> ParseResult<Expression*>
    {
        auto lhs = ParseUnary()(in);
        if (!lhs)
        {
            return std::nullopt;
        }

        for (char op : { '/', '*' })
        {
            if (auto o = Character(op)(lhs->second))
            {
                if (auto rest = ParseUnary()(o->second))
                {
                    return std::pair{ new Binary{ op, lhs->first, rest->first }, rest->second };
                }
                return std::nullopt;
            }
        }

        return lhs;
    };
}

auto ParseTerm() -> Parser<Expression*>
{
    return [](std::span<std::string> in) -> ParseResult<Expression*>
    {
        auto lhs = ParseFactor()(in);
        if (!lhs)
        {
            return std::nullopt;
        }

        for (char op : { '-', '+' })
        {
            if (auto o = Character(op)(lhs->second))
            {
                if (auto rest = ParseFactor()(o->second))
                {
                    return std::pair{ new Binary{ op, lhs->first, rest->first }, rest->second };
                }
                return std::nullopt;
            }
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
    else if (auto* bi = dynamic_cast<Binary*>(e))
    {
        std::cout << "Binary(" << bi->op << ", ";
        Show(bi->lhs);
        std::cout << ", ";
        Show(bi->rhs);
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
        { "42" },
        { "-", "42" },
        { "!", "true" },
        { "!", "!", "true" },
        { "-", "\"", "oops", "\""},
        { "!" },
        { "1", "+", "2" },
    };

    for (auto& test : tests)
    {
        auto r = ParseTerm()(std::span(test));
        PrintTokens(test);
        if (r)
        {
            Show(r->first);
        }
        else
        {
            std::cout << "parse error";
        }
        std::cout << "\n\n";
    }
	return 0;
}
