#pragma once

#include <combinator/token.h>

#include <span>

namespace combinator
{

/** @brief A non-owning, cheaply-constructible collection of tokens to parse.
 */
class State
{
private:

    std::span<const Token> m_source;

public:

    State(std::span<const Token> source);

    auto Done() const -> bool;
    auto Peek() const -> Token;
    auto Advance() const -> State;
};

} // namespace combinator
