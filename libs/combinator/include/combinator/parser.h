#pragma once

#include <combinator/result.h>
#include <combinator/state.h>

#include <format>
#include <functional>
#include <string>

namespace solver
{

/** @brief Any callable that returns a Result given an input State. The combinator system
 * composes parsers responsible for portions of a grammar to parse the full grammar.
 */
template <typename T>
class Parser
{
private:

    std::string m_name;

    using ParseFn = std::function<Result<T>(State)>;
    ParseFn m_parse_fn;

public:

    Parser(std::string name, ParseFn fn)
        : m_name(name), m_parse_fn(std::move(fn))
    {}

    auto operator()(State state) const -> Result<T>
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

        return Parser<U>(
            std::string("map(") + parser_name + ")",
            [map_fn, parse_fn, parser_name](State state) -> Result<U>
            {
                try {
                    auto result = parse_fn(state);
                    if (!result.Succeeded()) {
                        return Result<U>::Failure(result.Rest(), result.Message());
                    }
                    return Result<U>::Success(map_fn(result.Value()), result.Rest());
                }
                catch (...) {
                    return Result<U>::Failure(
                        state,
                        std::format("internal error while parsing {}", parser_name));
                }
            });
    }
};

} // namespace solver
