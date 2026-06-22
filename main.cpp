#include <functional>
#include <typeinfo>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <any>

template <typename A>
using Parser = std::function<std::optional<std::pair<A, std::string_view>>(std::string_view)>;

auto Satisfy(std::function<bool(char)> pred) -> Parser<char>
{
    return [pred](std::string_view in) -> std::optional<std::pair<char, std::string_view>>
    {
        if (!in.empty() && pred(in.front()))
        {
            return std::pair{in.front(), in.substr(1)};
        }
        return std::nullopt;
    };
}

auto Character(char c) -> Parser<char>
{
    return Satisfy([c](char x) { return x == c; });
}

auto Characters(std::string matcher) -> Parser<std::string>
{
    return [matcher](std::string_view source) -> std::optional<std::pair<std::string, std::string_view>>
    {
        if (source.size() >= matcher.size() && source.substr(0, matcher.size()) == matcher)
        {
            return std::pair{matcher, source.substr(matcher.size())};
        }
        return std::nullopt;
    };
}

auto Digit() -> Parser<char>
{
    return Satisfy([](char x) { return x >= '0' && x <= '9'; });
}

struct Expression
{
    virtual ~Expression() = default;
};

struct Unary : public Expression
{
    char operation;
    Expression* operand;

    Unary(char operation, Expression* operand)
        : operation(operation), operand(operand)
    {}
};

struct Literal : public Expression
{
    std::any value;

    Literal(std::any value)
        : value(value)
    {}
};

auto Number() -> Parser<Expression*>
{
    return [](std::string_view source) -> std::optional<std::pair<Expression*, std::string_view>>
    {
        std::string text;

        while (auto d = Digit()(source))
        {
            text += d->first;
            source = d->second;
        }

        if (text.empty())
        {
            return std::nullopt;
        }

        return std::pair{new Literal{std::stoi(text)}, source};
    };
}

auto Boolean() -> Parser<Expression*>
{
    return [](std::string_view source) -> std::optional<std::pair<Expression*, std::string_view>> {
        if (auto t = Characters("true")(source))  { return std::pair<Expression*, std::string_view>{new Literal{true},  t->second}; }
        if (auto f = Characters("false")(source)) { return std::pair<Expression*, std::string_view>{new Literal{false}, f->second}; }
        return std::nullopt;
    };
}

auto String() -> Parser<Expression*>
{
    return [](std::string_view source) -> std::optional<std::pair<Expression*, std::string_view>>
    {
        // expect an opening quote
        if (source.empty() || source.front() != '"')
        {
            return std::nullopt;
        }

        source = source.substr(1);
        std::string text;

        // consume characters until a closing quote is reached
        while (!source.empty() && source.front() != '"')
        {
            text += source.front();
            source = source.substr(1);
        }

        // source ended before finding a closing quote, return nothing
        if (source.empty())
        {
            return std::nullopt;
        }

        // consume the closing quote before continuing parsing
        source = source.substr(1);

        return std::pair{new Literal{text}, source};
    };
}

auto Primary() -> Parser<Expression*>
{
    return [](std::string_view in) -> std::optional<std::pair<Expression*, std::string_view>>
    {
        if (auto n = Number()(in))  { return n; }
        if (auto b = Boolean()(in)) { return b; }
        if (auto s = String()(in))  { return s; }
        return std::nullopt;
    };
}

auto ParseUnary() -> Parser<Expression*>
{
    return [](std::string_view in) -> std::optional<std::pair<Expression*, std::string_view>>
    {
        for (char op : {'!', '-'})
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
        return Primary()(in);
    };
}

auto Show(Expression* e) -> void
{
    if (auto* lit = dynamic_cast<Literal*>(e))
    {
        if (auto* i = std::any_cast<int>(&lit->value))
            std::cout << "Number(" << *i << ")";
        else if (auto* b = std::any_cast<bool>(&lit->value))
            std::cout << "Bool(" << (*b ? "true" : "false") << ")";
        else if (auto* s = std::any_cast<std::string>(&lit->value))
            std::cout << "String(\"" << *s << "\")";
        else
            std::cout << "Literal(<unknown type: " << lit->value.type().name() << ">)";
    }
    else if (auto* un = dynamic_cast<Unary*>(e))
    {
        std::cout << "Unary(" << un->operation << ", ";
        Show(un->operand);
        std::cout << ")";
    }
    else
    {
        std::cout << "<unknown Expression>";
    }
}

auto main() -> int
{
    for (std::string_view s : {"42", "-42", "!true", "!!true", "-\"oops\"", "!"})
    {
        auto r = ParseUnary()(s);
        std::cout << "\"" << s << "\" -> ";
        if (r) { Show(r->first); std::cout << " (leftover \"" << r->second << "\")\n"; }
        else    std::cout << "parse error\n";
    }
}
