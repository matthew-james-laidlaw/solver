// clang-format off

#pragma once

#include <combinator/parser.h>
#include <combinator/result.h>
#include <combinator/state.h>

#include <format>
#include <functional>
#include <optional>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace solver
{

/** @brief Parser that attempts to match the given token type on the next element in the
 * state. Advances the state on success, otherwise returns an error message.
 */
inline auto Expect(Token::Type type, std::string_view what) -> Parser<Token>
{
    return Parser<Token>(
        std::string(what),
        [=](State state) -> Result<Token>
        {
            if (state.Done()) {
                return Result<Token>::Failure(
                    state,
                    std::format("parser error: unexpected end of input, expected '{}'", what));
            }

            auto current = state.Peek();
            if (current.type != type) {
                return Result<Token>::Failure(
                    state,
                    std::format("parser error: expected '{}', got '{}'", what, current.lexeme));
            }

            return Result<Token>::Success(current, state.Advance());
        });
}

/** @brief Parser that attempts the given parser once. Allowing for no match.
 */
template <typename T>
auto Maybe(Parser<T> parser) -> Parser<std::optional<T>>
{
    return Parser<std::optional<T>>(
        std::string("maybe(") + parser.Name() + ")",
        [=](State state) -> Result<std::optional<T>>
        {
            auto result = parser(state);
            if (!result.Succeeded()) {
                return Result<std::optional<T>>::Success(std::nullopt, state);
            }
            return Result<std::optional<T>>::Success(result.Value(), result.Rest());
        });
}

/** @brief "Many" parser that attempts the given parser indefinitely until it fails.
 * Allows for no matches.
 */
template <typename T>
auto operator*(Parser<T> parser) -> Parser<std::vector<T>>
{
    return Parser<std::vector<T>>(
        std::string("many(") + parser.Name() + ")",
        [=](State state) -> Result<std::vector<T>>
        {
            std::vector<T> values;

            for (auto result = parser(state); result.Succeeded();) {
                values.push_back(result.Value());
                state = result.Rest();
                result = parser(state);
            }

            return Result<std::vector<T>>::Success(values, state);
        });
}

/** @brief "Fold right" parser that matches two parsers, ignoring the result of the first.
 */
template <typename A, typename B>
auto operator>>(Parser<A> a, Parser<B> b) -> Parser<B>
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
template <typename T>
auto operator|(Parser<T> a, Parser<T> b) -> Parser<T>
{
    return Parser<T>(
        std::string("(") + a.Name() + " | " + b.Name() + ")",
        [=](State state) -> Result<T>
        {
            auto first = a(state);
            if (first.Succeeded()) {
                return first;
            }

            auto second = b(state);
            if (second.Succeeded()) {
                return second;
            }

            return Result<T>::Failure(
                state,
                std::format("expected one of: ['{}' | '{}'], got neither", a.Name(), b.Name()));
        });
}

/** @brief "Combine" parser that runs two parsers in sequence, then folds both
 * results into a tuple.
 */
template <typename A, typename B>
auto operator&(Parser<A> a, Parser<B> b) -> Parser<std::tuple<A, B>>
{
    using R = std::tuple<A, B>;

    return Parser<R>(
        std::string("(") + a.Name() + " & " + b.Name() + ")",
        [=](State state) -> Result<R>
        {
            auto result_a = a(state);
            if (!result_a.Succeeded()) {
                return Result<R>::Failure(result_a.Rest(), result_a.Message());
            }

            auto result_b = b(result_a.Rest());
            if (!result_b.Succeeded()) {
                return Result<R>::Failure(result_b.Rest(), result_b.Message());
            }

            return Result<R>::Success({result_a.Value(), result_b.Value()}, result_b.Rest());
        });
}

} // namespace solver

// clang-format on
