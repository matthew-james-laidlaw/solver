#include <gtest/gtest.h>

#include <parser/parser.h>
#include <parser/token.h>

using namespace solver;

TEST(ParserTests, parse_power_no_exponent)
{
    auto source = std::vector<Token>
    {
        { .type = Token::Type::Variable, .lexeme = "x" }
    };

    State state(source);

    auto result = power(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), 1);
    ASSERT_EQ(mono.Exponent(), 1);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_power_with_exponent)
{
    auto source = std::vector<Token>
    {
        { .type = Token::Type::Variable, .lexeme = "x" },
        { .type = Token::Type::Caret, .lexeme = "^" },
        { .type = Token::Type::Number, .lexeme = "2" },
    };

    State state(source);

    auto result = power(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), 1);
    ASSERT_EQ(mono.Exponent(), 2);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_term_constant)
{
    auto source = std::vector<Token>
    {
        { .type = Token::Type::Number, .lexeme = "5" },
    };

    State state(source);

    auto result = term(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), 5);
    ASSERT_EQ(mono.Exponent(), 0);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_term_variable)
{
    auto source = std::vector<Token>
    {
        { .type = Token::Type::Variable, .lexeme = "x" },
    };

    State state(source);

    auto result = term(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), 1);
    ASSERT_EQ(mono.Exponent(), 1);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_term_monomial)
{
    auto source = std::vector<Token>
    {
        { .type = Token::Type::Number, .lexeme = "5" },
        { .type = Token::Type::Variable, .lexeme = "x" },
        { .type = Token::Type::Caret, .lexeme = "^" },
        { .type = Token::Type::Number, .lexeme = "2" },
    };

    State state(source);

    auto result = term(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), 5);
    ASSERT_EQ(mono.Exponent(), 2);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_unary_positive)
{
    auto source = std::vector<Token>
    {
        { .type = Token::Type::Number, .lexeme = "5" },
        { .type = Token::Type::Variable, .lexeme = "x" },
        { .type = Token::Type::Caret, .lexeme = "^" },
        { .type = Token::Type::Number, .lexeme = "2" },
    };

    State state(source);

    auto result = unary(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), 5);
    ASSERT_EQ(mono.Exponent(), 2);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_unary_negative)
{
    auto source = std::vector<Token>
    {
        { .type = Token::Type::Minus, .lexeme = "-" },
        { .type = Token::Type::Number, .lexeme = "5" },
        { .type = Token::Type::Variable, .lexeme = "x" },
        { .type = Token::Type::Caret, .lexeme = "^" },
        { .type = Token::Type::Number, .lexeme = "2" },
    };

    State state(source);

    auto result = unary(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.Coefficient(), -5);
    ASSERT_EQ(mono.Exponent(), 2);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_expression_term)
{
    auto source = std::vector<Token>
    {
        { .type = Token::Type::Number, .lexeme = "5" },
        { .type = Token::Type::Variable, .lexeme = "x" },
        { .type = Token::Type::Caret, .lexeme = "^" },
        { .type = Token::Type::Number, .lexeme = "2" },
    };

    State state(source);

    auto result = expression(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.size(), 1);
    ASSERT_EQ(mono[0].Coefficient(), 5);
    ASSERT_EQ(mono[0].Exponent(), 2);

    EXPECT_TRUE(result.Rest().Done());
}

TEST(ParserTests, parse_expression_plus)
{
    auto source = std::vector<Token>
    {
        { .type = Token::Type::Number, .lexeme = "5" },
        { .type = Token::Type::Variable, .lexeme = "x" },
        { .type = Token::Type::Caret, .lexeme = "^" },
        { .type = Token::Type::Number, .lexeme = "2" },
        { .type = Token::Type::Plus, .lexeme = "+" },
        { .type = Token::Type::Number, .lexeme = "6" },
        { .type = Token::Type::Variable, .lexeme = "x" },
        { .type = Token::Type::Caret, .lexeme = "^" },
        { .type = Token::Type::Number, .lexeme = "3" },
    };

    State state(source);

    auto result = expression(state);

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
    auto source = std::vector<Token>
    {
        { .type = Token::Type::Number, .lexeme = "5" },
        { .type = Token::Type::Variable, .lexeme = "x" },
        { .type = Token::Type::Caret, .lexeme = "^" },
        { .type = Token::Type::Number, .lexeme = "2" },
        { .type = Token::Type::Minus, .lexeme = "-" },
        { .type = Token::Type::Number, .lexeme = "6" },
        { .type = Token::Type::Variable, .lexeme = "x" },
        { .type = Token::Type::Caret, .lexeme = "^" },
        { .type = Token::Type::Number, .lexeme = "3" },
    };

    State state(source);

    auto result = expression(state);

    ASSERT_TRUE(result.Succeeded());

    auto mono = result.Value();

    ASSERT_EQ(mono.size(), 2);
    ASSERT_EQ(mono[0].Coefficient(), 5);
    ASSERT_EQ(mono[0].Exponent(), 2);
    ASSERT_EQ(mono[1].Coefficient(), -6);
    ASSERT_EQ(mono[1].Exponent(), 3);

    EXPECT_TRUE(result.Rest().Done());
}
