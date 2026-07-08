#pragma once

#include <string>

namespace solver
{

struct Token
{
    enum class Type
    {
        Function,
        Equals,
        Plus,
        Minus,
        Caret,
        Number,
        Variable,
        Nothing,
    };

    Type type;
    std::string lexeme;
};

auto operator==(Token const& lhs, Token const& rhs) -> bool;

} // namespace solver
