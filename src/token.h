#pragma once

#include <string>

namespace parser
{

struct Token
{
    enum class Type
    {
        Function, Equals,    // keywords
        Plus, Minus, Carat,  // operators
        Number, Variable,    // primaries
    };

    Type type;
    std::string lexeme;
};

auto operator==(Token const& lhs, Token const& rhs) -> bool;

} // namespace parser
