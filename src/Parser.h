#pragma once

#include <Expression.h>

#include <functional>
#include <optional>
#include <string>
#include <span>
#include <vector>

template <typename T>
using ParseResult = std::optional<std::pair<T, std::span<std::string>>>;

template <typename T>
using Parser = std::function<ParseResult<T>(std::span<std::string>)>;

using Predicate = std::function<bool(char)>;

inline auto Satisfy(Predicate pred) -> Parser<char>
{
	return [pred](std::span<std::string> source) -> ParseResult<char>
	{
		if (source.empty() || !pred(source.front().front()))
		{
			return std::nullopt;
		}
		return std::pair{ source.front().front(), source.subspan(1) };
	};
}

inline auto Character(char expected) -> Parser<char>
{
	return Satisfy([expected](char actual)
	{
		return actual == expected;
	});
}

inline auto ParseTerm() -> Parser<Expression*>
{
    return [](std::span<std::string> source) -> ParseResult<Expression*>
    {
        if (source.empty() || source.front().empty())
        {
            return std::nullopt;
        }

        auto current = source.front();
        auto pos = current.find_first_not_of("-0123456789");

        std::string number_part;
        std::optional<char> variable;

        if (pos == std::string::npos)
        {
            number_part = current;
        }
        else
        {
            number_part = current.substr(0, pos);
            if (current.size() - pos != 1 || !std::isalpha(current[pos]))
            {
                return std::nullopt;
            }
            variable = current[pos];
        }

        int number = 1;
        if (number_part == "-")
        {
            number = -1;
        }
        else if (!number_part.empty())
        {
            number = std::stoi(number_part);
        }

        return std::pair{ new Term{number, variable}, source.subspan(1) };
    };
}

inline auto OneOf(std::initializer_list<char> chars)
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

inline auto ParseMultiplicative() -> Parser<Expression*>
{
    return [](std::span<std::string> source)
    {
        return ParseLeftAssociative(
            source,
            ParseTerm(),
            {'*', '/'}
        );
    };
}

inline auto ParseAdditive() -> Parser<Expression*>
{
    return [](std::span<std::string> source)
    {
        return ParseLeftAssociative(
            source,
            ParseMultiplicative(),
            {'+', '-'}
        );
    };
}

inline auto ParseEquality() -> Parser<Expression*>
{
    return [](std::span<std::string> source) -> ParseResult<Expression*>
    {
        auto lhs = ParseAdditive()(source);
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
