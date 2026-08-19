#include <combinator/state.h>
#include <combinator/token.h>

namespace combinator
{

State::State(std::span<const Token> source)
    : m_source(source)
{}

auto State::Done() const -> bool
{
    return m_source.empty();
}

auto State::Peek() const -> Token
{
    return m_source.front();
}

auto State::Advance() const -> State
{
    return m_source.subspan(1);
}

} // namespace combinator
