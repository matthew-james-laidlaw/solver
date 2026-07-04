#include <token.h>

namespace parser
{

auto operator==(Token const& lhs, Token const& rhs) -> bool
{
    return lhs.type == rhs.type && lhs.lexeme == rhs.lexeme;
}

} // namespace parser
