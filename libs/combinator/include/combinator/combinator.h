// clang-format off

#pragma once

#include <combinator/parser.h>
#include <combinator/result.h>
#include <combinator/state.h>

#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace combinator
{

/** @brief Parser that attempts to match the given token type on the next element in the
 * state. Advances the state on success, otherwise returns an error message.
 */
template <typename TokenType>
auto Expect(typename TokenType::Type type, std::string_view what) -> Parser<TokenType, TokenType>
{
    return Parser<TokenType, TokenType>(
        std::string(what),
        [=](State<TokenType> state) -> Result<TokenType, TokenType>
        {
            if (state.Done()) {
                return Result<TokenType, TokenType>::Failure(
                    state,
                    std::format("parser error: unexpected end of input, expected '{}'", what));
            }

            auto current = state.Peek();
            if (current.type != type) {
                return Result<TokenType, TokenType>::Failure(
                    state,
                    std::format("parser error: expected '{}', got '{}'", what, current.lexeme));
            }

            return Result<TokenType, TokenType>::Success(current, state.Advance());
        });
}

/** @brief Parser that attempts the given parser once. Allowing for no match.
 */
template <typename T, typename TokenType>
auto Maybe(Parser<T, TokenType> parser) -> Parser<std::optional<T>, TokenType>
{
    return Parser<std::optional<T>, TokenType>(
        std::string("maybe(") + parser.Name() + ")",
        [=](State<TokenType> state) -> Result<std::optional<T>, TokenType>
        {
            auto result = parser(state);
            if (!result.Succeeded()) {
                return Result<std::optional<T>, TokenType>::Success(std::nullopt, state);
            }
            return Result<std::optional<T>, TokenType>::Success(result.Value(), result.Rest());
        });
}

/** @brief "Many" parser that attempts the given parser indefinitely until it fails.
 * Allows for no matches.
 */
template <typename T, typename TokenType>
auto operator*(Parser<T, TokenType> parser) -> Parser<std::vector<T>, TokenType>
{
    return Parser<std::vector<T>, TokenType>(
        std::string("many(") + parser.Name() + ")",
        [=](State<TokenType> state) -> Result<std::vector<T>, TokenType>
        {
            std::vector<T> values;

            for (auto result = parser(state); result.Succeeded();) {
                values.push_back(result.Value());
                state = result.Rest();
                result = parser(state);
            }

            return Result<std::vector<T>, TokenType>::Success(values, state);
        });
}

/** @brief "Fold right" parser that matches two parsers, ignoring the result of the first.
 */
template <typename A, typename B, typename TokenType>
auto operator>>(Parser<A, TokenType> a, Parser<B, TokenType> b) -> Parser<B, TokenType>
{
    return (a & b).Map(
        [](auto&& args)
        {
            auto& [_, rhs] = args;
            return rhs;
        });
}

/** @brief "Choice" parser that attempts both parsers returning the first to succeed.
 * Fails if neither parser succeeds.
 */
template <typename T, typename TokenType>
auto operator|(Parser<T, TokenType> a, Parser<T, TokenType> b) -> Parser<T, TokenType>
{
    return Parser<T, TokenType>(
        std::string("(") + a.Name() + " | " + b.Name() + ")",
        [=](State<TokenType> state) -> Result<T, TokenType>
        {
            auto first = a(state);
            if (first.Succeeded()) {
                return first;
            }

            auto second = b(state);
            if (second.Succeeded()) {
                return second;
            }

            return Result<T, TokenType>::Failure(
                state,
                std::format("expected one of: ['{}' | '{}'], got neither", a.Name(), b.Name()));
        });
}

/** @brief "Combine" parser that runs two parsers in sequence, then folds both
 * results into a tuple.
 */
template <typename A, typename B, typename TokenType>
auto operator&(Parser<A, TokenType> a, Parser<B, TokenType> b) -> Parser<std::tuple<A, B>, TokenType>
{
    using R = std::tuple<A, B>;

    return Parser<R, TokenType>(
        std::string("(") + a.Name() + " & " + b.Name() + ")",
        [=](State<TokenType> state) -> Result<R, TokenType>
        {
            auto result_a = a(state);
            if (!result_a.Succeeded()) {
                return Result<R, TokenType>::Failure(result_a.Rest(), result_a.Message());
            }

            auto result_b = b(result_a.Rest());
            if (!result_b.Succeeded()) {
                return Result<R, TokenType>::Failure(result_b.Rest(), result_b.Message());
            }

            return Result<R, TokenType>::Success({result_a.Value(), result_b.Value()}, result_b.Rest());
        });
}

} // namespace combinator

// clang-format on
