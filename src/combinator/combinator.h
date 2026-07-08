#pragma once

#include <frontend/result.h>
#include <frontend/state.h>

#include <functional>
#include <optional>
#include <tuple>
#include <type_traits>
#include <vector>

namespace solver
{

/** @brief Any callable that returns a Result given an input State. The combinator system
 *         composes parsers responsible for portions of a grammar to parse the full grammar.
 */
template <typename T>
class Parser
{
private:
    
    using Fn = std::function<Result<T>(State)>;
    Fn m_fn;

public:

    Parser(Fn fn)
        : m_fn(std::move(fn))
    {}

    auto operator()(State state) const -> Result<T>
    {
        return m_fn(state);
    }

    template <typename F>
    auto Map(F mapper) const
    {
        using U = std::invoke_result_t<F, T const&>;

        auto fn = m_fn;

        return Parser<U>(
            [mapper, fn](State state)
            {
                auto r = fn(state);

                if (!r.Succeeded())
                {
                    return Result<U>::Failure(state, "parser failure (map)");
                }

                return Result<U>::Success(
                    mapper(r.Value()),
                    r.Rest());
            });
    }

};

/** @brief Parser that attempts the given predicate on the next element in the state.
 *         Advances the state on success, otherwise returns an error message.
 */
template <typename Predicate>
auto Satisfy(Predicate predicate) -> Parser<Token>
{
    return Parser<Token>([=](State state) -> Result<Token>
    {
        if (state.Done())
        {
            return Result<Token>::Failure(state, "unexpected end of input");
        }

        if (!predicate(state.Peek().type))
        {
            return Result<Token>::Failure(state, "unexpected token");
        }

        return Result<Token>::Success(state.Peek(), state.Advance());
    });
}

/** @brief Parser that attempts the given parser once. Allowing for no match.
 */
template <typename T>
auto Maybe(Parser<T> parser) -> Parser<std::optional<T>>
{
    return Parser<std::optional<T>>([=](State state) -> Result<std::optional<T>>
    {
        auto result = parser(state);
        if (!result.Succeeded())
        {
            return Result<std::optional<T>>::Success(std::nullopt, state);
        }
        return Result<std::optional<T>>::Success(result.Value(), result.Rest());
    });
}

/** @brief "Many" parser that attempts the given parser indefinitely until it fails.
 *         Allows for no matches.
 */
template <typename T>
auto operator*(Parser<T> parser) -> Parser<std::vector<T>>
{
    return Parser<std::vector<T>>([=](State state) -> Result<std::vector<T>>
    {
        std::vector<T> values;

        for (auto result = parser(state); result.Succeeded(); state = result.Rest(), result = parser(state))
        {
            values.push_back(result.Value());
        }

        return Result<std::vector<T>>::Success(values, state);
    });
}

/** @brief "Fold right" parser that matches two parsers, ignoring the result of the first.
 */
template <typename A, typename B>
auto operator>>(Parser<A> a, Parser<B> b) -> Parser<B>
{
    return (a & b).Map([](auto&& args)
    {
        auto& [_, rhs] = args;
        return rhs;
    });
}

/** @brief "Choice" parser that attempts each of the given parsers in sequences until one
 *         of them passes. Fails if none of the parsers succeed.
 */
template <typename T>
auto operator|(Parser<T> a, Parser<T> b) -> Parser<T>
{
    return Parser<T>([=](State state) -> Result<T>
    {
        for (auto const& parser : { a, b })
        {
            auto result = parser(state);
            if (result.Succeeded())
            {
                return result;
            }
        }
        return Result<T>::Failure(state, "parser failure (choice)");
    });
}

/** @brief "Combine" parser that runs two parsers in sequence, then folds both results
 *         into a tuple.
 */
template <typename A, typename B>
auto operator&(Parser<A> a, Parser<B> b) -> Parser<std::tuple<A, B>>
{
    using R = std::tuple<A, B>;

    return Parser<R>([=](State state) -> Result<R>
    {
        auto result_a = a(state);
        if (!result_a.Succeeded())
        {
            return Result<R>::Failure(state, "parser failure (combine)");
        }

        auto result_b = b(result_a.Rest());
        if (!result_b.Succeeded())
        {
            return Result<R>::Failure(state, "parser failure (combine)");
        }

        return Result<R>::Success({ result_a.Value(), result_b.Value() }, result_b.Rest());
    });
}

} // namespace solver
