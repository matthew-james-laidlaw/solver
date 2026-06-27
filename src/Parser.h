#pragma once

#include <Expression.h>

#include <functional>
#include <optional>
#include <string>
#include <span>
#include <vector>

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

auto Satisfy(Predicate pred) -> Parser<char>;
auto Character(char expected) -> Parser<char>;
auto OneOf(std::initializer_list<char> chars) -> Parser<char>;

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

auto ParseTerm() -> Parser<Expression*>;
auto ParseMultiplicative() -> Parser<Expression*>;
auto ParseAdditive() -> Parser<Expression*>;
auto ParseEquality() -> Parser<Expression*>;
