#include <gtest/gtest.h>

#include <combinator/combinator.h>

using namespace solver;
using Type = Token::Type;

auto Is(Token::Type type)
{
    return Satisfy([type](Token::Type t) { return t == type; });
}

TEST(CombinatorTests, satisfy_predicate_succeeds)
{
    auto source = std::vector<Token>
    {
        { .type = Type::Function, .lexeme = "f(x)" },
    };
    auto state = State(source);

    auto result = Is(Type::Function)(state);

    ASSERT_TRUE(result.Succeeded());
    ASSERT_EQ(result.Value(), source[0]);
    ASSERT_TRUE(result.Rest().Done());
}

TEST(CombinatorTests, satisfy_predicate_fails)
{
    auto source = std::vector<Token>
    {
        { .type = Type::Function, .lexeme = "f(x)" },
    };
    auto state = State(source);

    auto result = Is(Type::Plus)(state);

    ASSERT_FALSE(result.Succeeded());
    ASSERT_EQ(result.Message(), "unexpected token");
    ASSERT_EQ(result.Rest().Peek(), source[0]);
}

TEST(CombinatorTests, satisfy_empty_state)
{
    auto source = std::vector<Token>{};
    auto state = State(source);

    auto result = Is(Type::Plus)(state);

    ASSERT_FALSE(result.Succeeded());
    ASSERT_EQ(result.Message(), "unexpected end of input");
    ASSERT_TRUE(result.Rest().Done());
}

TEST(CombinatorTests, satisfy_predicate_advances_state)
{
    auto source = std::vector<Token>
    {
        { .type = Type::Function, .lexeme = "f(x)" },
        { .type = Type::Equals, .lexeme = "=" },
    };
    auto state = State(source);

    auto result = Is(Type::Function)(state);

    ASSERT_TRUE(result.Succeeded());
    ASSERT_EQ(result.Value(), source[0]);
    ASSERT_EQ(result.Rest().Peek(), source[1]);
}

TEST(CombinatorTests, choice_matches_first_parser)
{
    auto source = std::vector<Token>
    {
        { .type = Type::Function, .lexeme = "f(x)" },
    };
    auto state = State(source);

    auto result = (Is(Type::Function) | Is(Type::Plus))(state);

    ASSERT_TRUE(result.Succeeded());
    ASSERT_EQ(result.Value(), source[0]);
    ASSERT_TRUE(result.Rest().Done());
}

TEST(CombinatorTests, choice_matches_last_parser)
{
    auto source = std::vector<Token>
    {
        { .type = Type::Function, .lexeme = "f(x)" },
    };
    auto state = State(source);

    auto result = (Is(Type::Plus) | Is(Type::Function))(state);

    ASSERT_TRUE(result.Succeeded());
    ASSERT_EQ(result.Value(), source[0]);
    ASSERT_TRUE(result.Rest().Done());
}

TEST(CombinatorTests, choice_matches_no_parsers)
{
    auto source = std::vector<Token>
    {
        { .type = Type::Function, .lexeme = "f(x)" },
    };
    auto state = State(source);

    auto result = (Is(Type::Plus) | Is(Type::Minus))(state);

    ASSERT_FALSE(result.Succeeded());
    ASSERT_EQ(result.Message(), "parser failure (choice)");
    ASSERT_EQ(result.Rest().Peek(), source[0]);
}

TEST(CombinatorTests, zero_or_more_matches_once)
{
    auto source = std::vector<Token>
    {
        { .type = Type::Number, .lexeme = "1" },
        { .type = Type::Variable, .lexeme = "x" },
    };
    auto state = State(source);

    auto result = (*(Is(Type::Number)))(state);

    ASSERT_TRUE(result.Succeeded());
    ASSERT_EQ(result.Value().size(), 1);
    ASSERT_EQ(result.Value()[0], source[0]);
    ASSERT_EQ(result.Rest().Peek(), source[1]);
}

TEST(CombinatorTests, zero_or_more_matches_multiple_times)
{
    auto source = std::vector<Token>
    {
        { .type = Type::Number, .lexeme = "1" },
        { .type = Type::Number, .lexeme = "2" },
        { .type = Type::Number, .lexeme = "3" },
        { .type = Type::Variable, .lexeme = "x" },
    };
    auto state = State(source);

    auto result = (*(Is(Type::Number)))(state);

    ASSERT_TRUE(result.Succeeded());
    ASSERT_EQ(result.Value().size(), 3);
    ASSERT_EQ(result.Value()[0], source[0]);
    ASSERT_EQ(result.Value()[1], source[1]);
    ASSERT_EQ(result.Value()[2], source[2]);
    ASSERT_EQ(result.Rest().Peek(), source[3]);
}

TEST(CombinatorTests, zero_or_more_matches_none)
{
    auto source = std::vector<Token>
    {
        { .type = Type::Number, .lexeme = "1" },
    };
    auto state = State(source);

    auto result = (*(Is(Type::Plus)))(state);

    ASSERT_TRUE(result.Succeeded());
    ASSERT_TRUE(result.Value().empty());
    ASSERT_EQ(result.Rest().Peek(), source[0]);
}

TEST(CombinatorTests, maybe_matches)
{
    auto source = std::vector<Token>
    {
        { .type = Type::Number, .lexeme = "1" },
    };
    auto state = State(source);

    auto result = Maybe(Is(Type::Number))(state);

    ASSERT_TRUE(result.Succeeded());
    ASSERT_TRUE(result.Value().has_value());
    ASSERT_EQ(result.Value().value(), source[0]);
    ASSERT_TRUE(result.Rest().Done());
}

TEST(CombinatorTests, maybe_does_not_match)
{
    auto source = std::vector<Token>
    {
        { .type = Type::Number, .lexeme = "1" },
    };
    auto state = State(source);

    auto result = Maybe(Is(Type::Plus))(state);

    ASSERT_TRUE(result.Succeeded());
    ASSERT_FALSE(result.Value().has_value());
    ASSERT_EQ(result.Rest().Peek(), source[0]);
}

TEST(CombinatorTests, map_succeeds)
{
    auto source = std::vector<Token>
    {
        { .type = Type::Number, .lexeme = "1" },
    };
    auto state = State(source);

    auto result = Is(Type::Number).Map([](Token token) -> int
    {
        return std::stoi(token.lexeme);
    })(state);

    ASSERT_TRUE(result.Succeeded());
    ASSERT_EQ(result.Value(), 1);
    ASSERT_TRUE(result.Rest().Done());
}

TEST(CombinatorTests, map_fails)
{
    auto source = std::vector<Token>
    {
        { .type = Type::Number, .lexeme = "1" },
    };

    auto state = State(source);

    auto result = Is(Type::Plus).Map([](Token token) -> int
    {
        return std::stoi(token.lexeme);
    })(state);

    ASSERT_FALSE(result.Succeeded());
    ASSERT_EQ(result.Message(), "parser failure (map)");
    ASSERT_EQ(result.Rest().Peek(), source[0]);
}
