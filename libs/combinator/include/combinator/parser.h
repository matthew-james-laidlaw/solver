#pragma once

#include <combinator/result.h>
#include <combinator/state.h>

#include <format>
#include <functional>
#include <stdexcept>
#include <string>

namespace combinator
{

/** @brief Any callable that returns a Result given an input State. The combinator system
 * composes parsers responsible for portions of a grammar to parse the full grammar.
 */
template <typename T, typename TokenType>
class Parser
{
private:

    std::string m_name;

    using ParseFn = std::function<Result<T, TokenType>(State<TokenType>)>;
    ParseFn m_parse_fn;

public:

    Parser(std::string name, ParseFn fn)
        : m_name(name), m_parse_fn(std::move(fn))
    {}

    auto operator()(State<TokenType> state) const -> Result<T, TokenType>
    {
        return m_parse_fn(state);
    }

    auto Name() const -> std::string
    {
        return m_name;
    }

    template <typename MapFn>
    auto Map(MapFn map_fn) const
    {
        using U = std::invoke_result_t<MapFn, const T&>;

        auto parse_fn = m_parse_fn;
        auto parser_name = m_name;

        return Parser<U, TokenType>(
            std::string("map(") + parser_name + ")",
            [map_fn, parse_fn,
             parser_name](State<TokenType> state) -> Result<U, TokenType>
            {
                try {
                    auto result = parse_fn(state);
                    if (!result.Succeeded()) {
                        return Result<U, TokenType>::Failure(result.Rest(),
                                                             result.Message());
                    }
                    return Result<U, TokenType>::Success(map_fn(result.Value()),
                                                         result.Rest());
                }
                catch (...) {
                    return Result<U, TokenType>::Failure(
                        state,
                        std::format("internal error while parsing {}", parser_name));
                }
            });
    }
};

} // namespace combinator
