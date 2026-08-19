#include <gtest/gtest.h>

#include <combinator/parser.h>
#include <combinator/result.h>
#include <combinator/state.h>
#include <grammar.h>
#include <monomial.h>
#include <parser.h>
#include <polynomial.h>
#include <token.h>

#include <span>
#include <string>
#include <vector>

using namespace solver;

TEST(ParserTests, parse_power_no_exponent)
{
    auto source = std::vector<Token>{{.type = Token::Type::Variable, .lexeme = "x"}};

    combinator::State<Token> state(source);

    auto result = PowerParser()(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), 1);
    ASSERT_EQ(mono.Exponent(), 1);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_power_with_exponent)
{
    auto source = std::vector<Token>{
        {.type = Token::Type::Variable, .lexeme = "x"},
        {.type = Token::Type::Caret, .lexeme = "^"},
        {.type = Token::Type::Number, .lexeme = "2"},
    };

    combinator::State<Token> state(source);

    auto result = PowerParser()(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), 1);
    ASSERT_EQ(mono.Exponent(), 2);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_term_constant)
{
    auto source = std::vector<Token>{
        {.type = Token::Type::Number, .lexeme = "5"},
    };

    combinator::State<Token> state(source);

    auto result = TermParser()(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), 5);
    ASSERT_EQ(mono.Exponent(), 0);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_term_variable)
{
    auto source = std::vector<Token>{
        {.type = Token::Type::Variable, .lexeme = "x"},
    };

    combinator::State<Token> state(source);

    auto result = TermParser()(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), 1);
    ASSERT_EQ(mono.Exponent(), 1);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_term_monomial)
{
    auto source = std::vector<Token>{
        {.type = Token::Type::Number, .lexeme = "5"},
        {.type = Token::Type::Variable, .lexeme = "x"},
        {.type = Token::Type::Caret, .lexeme = "^"},
        {.type = Token::Type::Number, .lexeme = "2"},
    };

    combinator::State<Token> state(source);

    auto result = TermParser()(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), 5);
    ASSERT_EQ(mono.Exponent(), 2);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_unary_positive)
{
    auto source = std::vector<Token>{
        {.type = Token::Type::Number, .lexeme = "5"},
        {.type = Token::Type::Variable, .lexeme = "x"},
        {.type = Token::Type::Caret, .lexeme = "^"},
        {.type = Token::Type::Number, .lexeme = "2"},
    };

    combinator::State<Token> state(source);

    auto result = UnaryParser()(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), 5);
    ASSERT_EQ(mono.Exponent(), 2);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_unary_negative)
{
    auto source = std::vector<Token>{
        {.type = Token::Type::Minus, .lexeme = "-"},
        {.type = Token::Type::Number, .lexeme = "5"},
        {.type = Token::Type::Variable, .lexeme = "x"},
        {.type = Token::Type::Caret, .lexeme = "^"},
        {.type = Token::Type::Number, .lexeme = "2"},
    };

    combinator::State<Token> state(source);

    auto result = UnaryParser()(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), -5);
    ASSERT_EQ(mono.Exponent(), 2);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_expression_term)
{
    auto source = std::vector<Token>{
        {.type = Token::Type::Number, .lexeme = "5"},
        {.type = Token::Type::Variable, .lexeme = "x"},
        {.type = Token::Type::Caret, .lexeme = "^"},
        {.type = Token::Type::Number, .lexeme = "2"},
    };

    combinator::State<Token> state(source);

    auto result = ExpressionParser()(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.size(), 1);
    ASSERT_EQ(mono[0].Coefficient(), 5);
    ASSERT_EQ(mono[0].Exponent(), 2);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_expression_plus)
{
    auto source = std::vector<Token>{
        {.type = Token::Type::Number, .lexeme = "5"},
        {.type = Token::Type::Variable, .lexeme = "x"},
        {.type = Token::Type::Caret, .lexeme = "^"},
        {.type = Token::Type::Number, .lexeme = "2"},
        {.type = Token::Type::Plus, .lexeme = "+"},
        {.type = Token::Type::Number, .lexeme = "6"},
        {.type = Token::Type::Variable, .lexeme = "x"},
        {.type = Token::Type::Caret, .lexeme = "^"},
        {.type = Token::Type::Number, .lexeme = "3"},
    };

    combinator::State<Token> state(source);

    auto result = ExpressionParser()(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.size(), 2);
    ASSERT_EQ(mono[0].Coefficient(), 5);
    ASSERT_EQ(mono[0].Exponent(), 2);
    ASSERT_EQ(mono[1].Coefficient(), 6);
    ASSERT_EQ(mono[1].Exponent(), 3);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_expression_minus)
{
    auto source = std::vector<Token>{
        {.type = Token::Type::Number, .lexeme = "5"},
        {.type = Token::Type::Variable, .lexeme = "x"},
        {.type = Token::Type::Caret, .lexeme = "^"},
        {.type = Token::Type::Number, .lexeme = "2"},
        {.type = Token::Type::Minus, .lexeme = "-"},
        {.type = Token::Type::Number, .lexeme = "6"},
        {.type = Token::Type::Variable, .lexeme = "x"},
        {.type = Token::Type::Caret, .lexeme = "^"},
        {.type = Token::Type::Number, .lexeme = "3"},
    };

    combinator::State<Token> state(source);

    auto result = ExpressionParser()(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.size(), 2);
    ASSERT_EQ(mono[0].Coefficient(), 5);
    ASSERT_EQ(mono[0].Exponent(), 2);
    ASSERT_EQ(mono[1].Coefficient(), -6);
    ASSERT_EQ(mono[1].Exponent(), 3);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_polynomial)
{
    // f(x) = 5x^2 - 6x^3 + 4x^2
    auto source = std::vector<Token>{
        {.type = Token::Type::Function, .lexeme = "f(x)"},
        {.type = Token::Type::Equals, .lexeme = "="},
        {.type = Token::Type::Number, .lexeme = "5"},
        {.type = Token::Type::Variable, .lexeme = "x"},
        {.type = Token::Type::Caret, .lexeme = "^"},
        {.type = Token::Type::Number, .lexeme = "2"},
        {.type = Token::Type::Minus, .lexeme = "-"},
        {.type = Token::Type::Number, .lexeme = "6"},
        {.type = Token::Type::Variable, .lexeme = "x"},
        {.type = Token::Type::Caret, .lexeme = "^"},
        {.type = Token::Type::Number, .lexeme = "3"},
        {.type = Token::Type::Plus, .lexeme = "+"},
        {.type = Token::Type::Number, .lexeme = "4"},
        {.type = Token::Type::Variable, .lexeme = "x"},
        {.type = Token::Type::Caret, .lexeme = "^"},
        {.type = Token::Type::Number, .lexeme = "2"},
    };

    auto polynomial = Parse(source);

    // polynomial creation will have sorted and combined like terms and added 0 terms for
    // powers of 0 and 1.
    ASSERT_EQ(polynomial.Size(), 4);
    ASSERT_EQ(polynomial[0], 0);
    ASSERT_EQ(polynomial[1], 0);
    ASSERT_EQ(polynomial[2], 9);
    ASSERT_EQ(polynomial[3], -6);
}
