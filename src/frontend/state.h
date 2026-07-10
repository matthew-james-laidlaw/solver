#pragma once

#include <frontend/token.h>

#include <span>

namespace solver
{

/** @brief A non-owning, cheaply-constructible collection of tokens to parse. Controls
 * access to the underlying tokens.
 */
class State
{
private:

    std::span<Token const> m_source;

public:

    State(std::span<Token const> source);

    auto Done() const -> bool;
    auto Peek() const -> Token;
    auto Advance() const -> State;
};

} // namespace solver
