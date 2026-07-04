#pragma once

#include <state.h>

#include <optional>
#include <string>

namespace parser
{

/** @brief The outcome of running a parser. On success, holds a value and the remaining state. On
 *         failure, holds the remaining state and an error message.
 */
template <typename T>
class Result
{
private:

    bool m_ok;
    std::optional<T> m_value;
    State m_state;
    std::string m_message;

    Result(bool ok, std::optional<T> value, State state, std::string message)
        : m_ok(ok), m_value(value), m_state(state), m_message(message)
    {}

public:

    static auto Success(T value, State state) -> Result
    {
        return Result(true, std::move(value), state, "");
    }

    static auto Failure(State state, std::string message) -> Result
    {
        return Result(false, std::nullopt, state, message);
    }

    auto Succeeded() const -> bool
    {
        return m_ok;
    }

    auto Value() const -> T const&
    {
        return *m_value;
    }

    auto Value() -> T&
    {
        return *m_value;
    }

    auto Rest() -> State
    {
        return m_state;
    }

    auto Message() const -> std::string const&
    {
        return m_message;
    }

};

} // namespace parser
