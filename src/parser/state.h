#pragma once

#include <parser/token.h>

#include <span>
#include <vector>

namespace solver
{

/** @brief A non-owning, cheaply-constructible collection of tokens to parse. Controls access to
 *         the underlying tokens.
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

} // namespace solver
