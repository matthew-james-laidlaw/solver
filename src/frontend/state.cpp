#include <frontend/state.h>

namespace solver
{

State::State(std::span<Token const> source)
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

} // namespace solver
