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

auto Satisfy(Predicate pred) -> Parser<char>
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

auto Character(char expected) -> Parser<char>
{
	return Satisfy([expected](char actual)
	{
		return actual == expected;
	});
}

auto Characters(std::string expected) -> Parser<std::string>
{
	return [expected](std::span<std::string> actual) -> ParseResult<std::string>
	{
        if (actual.empty() || actual.front() != expected)
        {
            return std::nullopt;
        }
		return std::pair{ expected, actual.subspan(1) };
	};
}

auto Digit() -> Parser<char>
{
	return Satisfy([](char actual)
	{
		return '0' <= actual && actual <= '9';
	});
}

auto Number() -> Parser<Expression*>
{
    return [](std::span<std::string> source) -> ParseResult<Expression*>
    {
        if (source.empty() || source[0].empty())
        {
            return std::nullopt;
        }

        for (auto c : source[0])
        {
            bool valid = '0' <= c && c <= '9';
            if (!valid)
            {
                return std::nullopt;
            }
        }

        return std::pair{ new Literal{std::stoi(source[0])}, source.subspan(1)};
    };
}

auto Boolean() -> Parser<Expression*>
{
    return [](std::span<std::string> source) -> ParseResult<Expression*>
    {
        if (auto t = Characters("true")(source)) { return std::pair<Expression*, std::span<std::string>>{ new Literal{ true }, t->second}; }
        if (auto f = Characters("false")(source)) { return std::pair<Expression*, std::span<std::string>>{ new Literal{ false }, f->second}; }
        return std::nullopt;
    };
}

auto String() -> Parser<Expression*>
{
    return [](std::span<std::string> source) -> ParseResult<Expression*>
    {
        if (source.size() < 3 || source.front().empty())
        {
            return std::nullopt;
        }

        // expect opening and closing quotes
        if (source[0][0] != '"' || source[2][0] != '"')
        {
            return std::nullopt;
        }

        return std::pair{ new Literal(source[1]), source.subspan(1)};
    };
}
