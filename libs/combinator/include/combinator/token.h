#pragma once

#include <string>

namespace combinator
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

auto operator==(const Token& lhs, const Token& rhs) -> bool;

} // namespace combinator
