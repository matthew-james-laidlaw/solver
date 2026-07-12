#include <frontend/token.h>

namespace solver
{

auto operator==(const Token& lhs, const Token& rhs) -> bool
{
    return lhs.type == rhs.type && lhs.lexeme == rhs.lexeme;
}

} // namespace solver
