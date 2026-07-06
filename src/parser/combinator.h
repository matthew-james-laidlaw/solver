#pragma once

#include <parser/result.h>

#include <functional>

namespace solver
{
    
/** @brief Any callable that returns a @ref Result given an input @ref State. The combinator
 *         system composes parsers responsible for portions of a grammar to parse the full grammar.
 */
template <typename T>
using Parser = std::function<Result<T>(State)>;

/** @brief Parser that attempts the given predicate on the next element in the state. Advances the
 *         state on success, otherwise returns an error message.
 */
template <typename Predicate>
auto Satisfy(Predicate predicate) -> Parser<Token>
{
    return [=](State state) -> Result<Token>
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
    };
}

/** @brief Parser that attempts each of the given sub-parsers in order. Expecting all of them to
 *         succeed.
 */
template <typename T>
auto Sequence(std::vector<Parser<T>> parsers) -> Parser<std::vector<T>>
{
    return [=](State state) -> Result<std::vector<T>>
    {
        auto original_state = state;
        auto results = std::vector<T>{};

        for (auto const& parser : parsers)
        {
            auto result = parser(state);
            if (!result.Succeeded())
            {
                return Result<std::vector<T>>::Failure(original_state, "parser failure (sequence)");
            }
            results.push_back(result.Value());
            state = result.Rest();
        }

        return Result<std::vector<T>>::Success(results, state);
    };
}

/** @brief Parser that attempts each of the given sub-parsers until one of them passes. Fails if
 *         none of the parsers succeed.
 */
template <typename T>
auto Choice(std::vector<Parser<T>> const& parsers) -> Parser<T>
{
    return [=](State state) -> Result<T>
    {
        for (auto const& parser : parsers)
        {
            auto result = parser(state);
            if (result.Succeeded())
            {
                return result;
            }
        }
        return Result<T>::Failure(state, "parser failure (choice)");
    };
}

/** @brief Parser that attempts the given sub-parser indefinitely until it fails. Allows for no
 *         matches.
 */
template <typename T>
auto ZeroOrMore(Parser<T> parser) -> Parser<std::vector<T>>
{
    return [=](State state) -> Result<std::vector<T>>
    {
        std::vector<T> values;

        for (auto result = parser(state); result.Succeeded(); state = result.Rest(), result = parser(state))
        {
            values.push_back(result.Value());
        }

        return Result<std::vector<T>>::Success(values, state);
    };
}

/** @brief Parser that attempts the given sub-parser indefinitely until it fails. Requires at least
 *         one match
 */
template <typename T>
auto OneOrMore(Parser<T> parser) -> Parser<std::vector<T>>
{
    return [=](State state) -> Result<std::vector<T>>
    {
        std::vector<T> values;

        auto first_result = parser(state);
        if (!first_result.Succeeded())
        {
            return Result<std::vector<T>>::Failure(state, "parser failure (one-or-more)");
        }
        values.push_back(first_result.Value());
        state = first_result.Rest();

        for (auto result = parser(state); result.Succeeded(); state = result.Rest(), result = parser(state))
        {
            values.push_back(result.Value());
        }

        return Result<std::vector<T>>::Success(values, state);
    };
}

/** @brief Parser that attempts the given sub-parser once. Allowing for no match.
 */
template <typename T>
auto Maybe(Parser<T> parser) -> Parser<std::optional<T>>
{
    return [=](State state) -> Result<std::optional<T>>
    {
        auto result = parser(state);
        if (!result.Succeeded())
        {
            return Result<std::optional<T>>::Success(std::nullopt, state);
        }
        return Result<std::optional<T>>::Success(result.Value(), result.Rest());
    };
}

template <typename T, typename F>
auto Map(Parser<T> parser, F mapper) -> Parser<std::invoke_result_t<F, T const&>>
{
    using U = std::invoke_result_t<F, T const&>;
    return [=](State state) -> Result<U>
    {
        auto r = parser(state);
        if (!r.Succeeded())
        {
            return Result<U>::Failure(state, "parser failure (map)");
        }
        return Result<U>::Success(mapper(r.Value()), r.Rest());
    };
}

/** @brief Parser that runs two sub-parsers in sequence, passes the state from the first into the
 *         second, then folds both values into one result with the given callable. Restores the
 *         original state on failure.
 */
template <typename T1, typename T2, typename F>
auto Combine(Parser<T1> a, Parser<T2> b, F combiner) -> Parser<std::invoke_result_t<F, T1, T2>>
{
    using R = std::invoke_result_t<F, T1, T2>;

    return [=](State state) -> Result<R>
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

        return Result<R>::Success(combiner(result_a.Value(), result_b.Value()), result_b.Rest());
    };
}

template <typename A, typename B>
auto SkipThen(Parser<A> a, Parser<B> b) -> Parser<B>
{
    return Combine(a, b, [](A const&, B b) { return b; });
}

template <typename A, typename B, typename... Rest>
auto SkipThen(Parser<A> a, Parser<B> b, Rest... rest)
{
    return SkipThen(SkipThen(a, b), rest...);
}

} // namespace solver
