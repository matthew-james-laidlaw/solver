#pragma once

#include <string>

namespace solver
{

struct Token
{
    enum class Type
    {
        Function, Equals,    // keywords
        Plus, Minus, Caret,  // operators
        Number, Variable,    // primaries
        Nothing,
    };

    Type type;
    std::string lexeme;
};

auto operator==(Token const& lhs, Token const& rhs) -> bool;

} // namespace solver
