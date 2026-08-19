#include <gtest/gtest.h>

#include <combinator/token.h>
#include <frontend/lexer.h>

#include <stdexcept>
#include <string>
#include <vector>

using namespace std::string_literals;
using namespace solver;

TEST(LexerTests, lex_function)
{
    auto source = "f(x)"s;

    auto expected = std::vector<combinator::Token>{
        {.type = combinator::Token::Type::Function, .lexeme = "f(x)"},
    };

    auto actual = Lex(source);
    ASSERT_EQ(expected, actual);
}

TEST(LexerTests, lex_equals)
{
    auto source = "="s;

    auto expected = std::vector<combinator::Token>{
        {.type = combinator::Token::Type::Equals, .lexeme = "="},
    };

    auto actual = Lex(source);
    ASSERT_EQ(expected, actual);
}

TEST(LexerTests, lex_plus)
{
    auto source = "+"s;

    auto expected = std::vector<combinator::Token>{
        {.type = combinator::Token::Type::Plus, .lexeme = "+"},
    };

    auto actual = Lex(source);
    ASSERT_EQ(expected, actual);
}

TEST(LexerTests, lex_minus)
{
    auto source = "-"s;

    auto expected = std::vector<combinator::Token>{
        {.type = combinator::Token::Type::Minus, .lexeme = "-"},
    };

    auto actual = Lex(source);
    ASSERT_EQ(expected, actual);
}

TEST(LexerTests, lex_caret)
{
    auto source = "^"s;

    auto expected = std::vector<combinator::Token>{
        {.type = combinator::Token::Type::Caret, .lexeme = "^"},
    };

    auto actual = Lex(source);
    ASSERT_EQ(expected, actual);
}

TEST(LexerTests, lex_number)
{
    auto source = "123"s;

    auto expected = std::vector<combinator::Token>{
        {.type = combinator::Token::Type::Number, .lexeme = "123"},
    };

    auto actual = Lex(source);
    ASSERT_EQ(expected, actual);
}

TEST(LexerTests, lex_variable)
{
    auto source = "x"s;

    auto expected = std::vector<combinator::Token>{
        {.type = combinator::Token::Type::Variable, .lexeme = "x"},
    };

    auto actual = Lex(source);
    ASSERT_EQ(expected, actual);
}

TEST(LexerTests, lex_empty_source)
{
    auto source = ""s;

    auto expected = std::vector<combinator::Token>{};

    auto actual = Lex(source);
    ASSERT_EQ(expected, actual);
}

TEST(LexerTests, lex_polynomial)
{
    auto source = "f(x) = 3x^2 + 5x - 7"s;

    auto expected = std::vector<combinator::Token>{
        {.type = combinator::Token::Type::Function, .lexeme = "f(x)"},
        {.type = combinator::Token::Type::Equals, .lexeme = "="},
        {.type = combinator::Token::Type::Number, .lexeme = "3"},
        {.type = combinator::Token::Type::Variable, .lexeme = "x"},
        {.type = combinator::Token::Type::Caret, .lexeme = "^"},
        {.type = combinator::Token::Type::Number, .lexeme = "2"},
        {.type = combinator::Token::Type::Plus, .lexeme = "+"},
        {.type = combinator::Token::Type::Number, .lexeme = "5"},
        {.type = combinator::Token::Type::Variable, .lexeme = "x"},
        {.type = combinator::Token::Type::Minus, .lexeme = "-"},
        {.type = combinator::Token::Type::Number, .lexeme = "7"},
    };

    auto actual = Lex(source);
    ASSERT_EQ(expected, actual);
}

TEST(LexerTests, lex_failure)
{
    auto source = "z";
    ASSERT_THROW(Lex(source), std::runtime_error);
}
