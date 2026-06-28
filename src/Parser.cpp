#pragma once

#include <Parser.h>

#include <functional>
#include <optional>
#include <span>
#include <stdexcept>

namespace
{

using State = std::span<std::string>;

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
		if (source.empty() || !pred(source.front().front()))
		{
			return std::nullopt;
		}

		return InnerParseResult {
            .result = source.front().front(),
            .rest = source.subspan(1),
        };
	};
}

auto Character(char expected) -> Parser<char>
{
	return Satisfy([expected](char actual)
	{
		return actual == expected;
	});
}

auto ParseTerm() -> Parser<Expression*>
{
    return [](State source) -> ParseResult<Expression*>
    {
        if (source.empty() || source.front().empty())
            return std::nullopt;

        std::string_view tok = source.front();

        auto all_digits = [](std::string_view s) -> bool
        {
            if (s.empty())
                return false;
            for (char c : s)
                if (!std::isdigit(static_cast<unsigned char>(c)))
                    return false;
            return true;
        };

        auto to_int = [](std::string_view s, int& out) -> bool
        {
            try { out = std::stoi(std::string(s)); return true; }
            catch (const std::exception&) { return false; }  // invalid_argument / out_of_range
        };

        std::size_t pos = 0;

        // 1. optional leading minus
        bool negative = false;
        if (pos < tok.size() && tok[pos] == '-')
        {
            negative = true;
            ++pos;
        }

        // 2. optional coefficient
        std::size_t coeff_start = pos;
        while (pos < tok.size() && std::isdigit(static_cast<unsigned char>(tok[pos])))
            ++pos;

        bool has_coeff = (pos > coeff_start);
        int coeff = 1;
        if (has_coeff && !to_int(tok.substr(coeff_start, pos - coeff_start), coeff))
            return std::nullopt;
        if (negative)
            coeff = -coeff;

        // 3. optional variable part
        int exponent = 0;          // 0 == no x (pure constant)
        bool has_x = false;
        if (pos < tok.size() && tok[pos] == 'x')
        {
            has_x = true;
            exponent = 1;
            ++pos;

            // optional ^N
            if (pos < tok.size())
            {
                if (tok[pos] != '^')
                    return std::nullopt;
                ++pos;

                std::string_view exp_part = tok.substr(pos);
                std::string_view digits  = exp_part;
                if (!digits.empty() && digits.front() == '-')
                    digits.remove_prefix(1);

                if (!all_digits(digits) || !to_int(exp_part, exponent))
                    return std::nullopt;

                pos = tok.size();
            }
        }

        // Must be a coefficient, a variable, or both — and fully consumed.
        if ((!has_coeff && !has_x) || pos != tok.size())
            return std::nullopt;

        return InnerParseResult<Expression*>{
            .result = new Term{coeff, exponent},
            .rest = source.subspan(1),
        };
    };
}

auto OneOf(std::initializer_list<char> chars) -> Parser<char>
{
    return [chars](State source)
        -> ParseResult<char>
    {
        for (char c : chars)
        {
            if (auto r = Character(c)(source))
            {
                return InnerParseResult {
                    .result = c,
                    .rest = r->rest,
                };
            }
        }

        return std::nullopt;
    };
}

template<typename SubParser>
auto ParseLeftAssociative(
    State source,
    SubParser sub,
    std::initializer_list<char> ops)
    -> ParseResult<Expression*>
{
    auto lhs = sub(source);
    if (!lhs)
    {
        return std::nullopt;
    }

    auto expr = lhs->result;
    auto rest = lhs->rest;

    while (auto op = OneOf(ops)(rest))
    {
        auto rhs = sub(op->rest);
        if (!rhs)
        {
            return std::nullopt;
        }

        expr = new Binary{op->result, expr, rhs->result};
        rest = rhs->rest;
    }

    return InnerParseResult {
        .result = expr,
        .rest = rest,
    };
}

auto ParseMultiplicative() -> Parser<Expression*>
{
    return [](State source)
    {
        return ParseLeftAssociative(
            source,
            ParseTerm(),
            {'*', '/'}
        );
    };
}

auto ParseAdditive() -> Parser<Expression*>
{
    return [](State source)
    {
        return ParseLeftAssociative(
            source,
            ParseMultiplicative(),
            {'+', '-'}
        );
    };
}

auto ParseEquality() -> Parser<Expression*>
{
    return [](State source) -> ParseResult<Expression*>
    {
        auto lhs = ParseAdditive()(source);
        if (!lhs)
        {
            return std::nullopt;
        }

        if (auto c = Character('=')(lhs->rest))
        {
            if (auto rhs = ParseTerm()(c->rest))
            {
                return InnerParseResult<Expression*> {
                    .result = new Equality{ lhs->result, rhs->result },
                    .rest = rhs->rest,
                };
            }
            return std::nullopt;
        }

        return lhs;
    };
}

auto Tokenize(std::string const& source) -> std::vector<std::string>
{
    std::vector<std::string> tokens;
    std::string current;

    for (char c : source)
    {
        if (std::isspace(static_cast<unsigned char>(c)))
        {
            if (!current.empty())
            {
                tokens.push_back(current);
                current.clear();
            }
        }
        else
        {
            current += c;
        }
    }

    if (!current.empty())
    {
        tokens.push_back(current);
    }

    return tokens;
}

} // namespace

auto Parse(std::string const& source) -> Expression*
{
    auto tokens = Tokenize(source);
    auto result = ParseEquality()(tokens);
    if (!result)
    {
        throw std::runtime_error("parser error");
    }
    return result->result;
}
