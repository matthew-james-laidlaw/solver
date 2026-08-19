#pragma once

#include <span>

namespace combinator
{

/** @brief A non-owning, cheaply-constructible collection of tokens to parse.
 */
template <typename TokenType>
class State
{
private:

    std::span<const TokenType> m_source;

public:

    State(std::span<const TokenType> source)
        : m_source(source)
    {}

    auto Done() const -> bool
    {
        return m_source.empty();
    }

    auto Peek() const -> TokenType
    {
        return m_source.front();
    }

    auto Advance() const -> State
    {
        return m_source.subspan(1);
    }
};

} // namespace combinator
