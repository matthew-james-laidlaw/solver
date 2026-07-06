#include <frontend/lexer.h>

#include <stdexcept>
#include <string_view>

using namespace std::string_literals;

namespace solver
{

auto Lex(std::string const& source) -> std::vector<Token>
{
    auto state = std::string_view(source);
    auto tokens = std::vector<Token>{};

    while (!state.empty())
    {
        auto next_non_whitespace = state.find_first_not_of(" \0");
        if (next_non_whitespace == std::string_view::npos)
        {
            break;
        }
        state = state.substr(next_non_whitespace);

        if (state.starts_with("f(x)"))
        {
            tokens.push_back({
                .type = Token::Type::Function,
                .lexeme = "f(x)",
            });
            state.remove_prefix("f(x)"s.size());
        }
        else if (state[0] == '=')
        {
            tokens.push_back({
                .type = Token::Type::Equals,
                .lexeme = "=",
            });
            state.remove_prefix(1);
        }
        else if (state[0] == '+')
        {
            tokens.push_back({
                .type = Token::Type::Plus,
                .lexeme = "+",
            });
            state.remove_prefix(1);
        }
        else if (state[0] == '-')
        {
            tokens.push_back({
                .type = Token::Type::Minus,
                .lexeme = "-",
            });
            state.remove_prefix(1);
        }
        else if (state[0] == '^')
        {
            tokens.push_back({
                .type = Token::Type::Caret,
                .lexeme = "^",
            });
            state.remove_prefix(1);
        }
        else if ('0' <= state[0] && state[0] <= '9')
        {
            auto number_end = state.find_first_not_of("0123456789");
            auto lexeme = ""s;
            if (number_end == std::string_view::npos)
            {
                lexeme = state;
                state = "";
            }
            else
            {
                lexeme = state.substr(0, number_end);
                state.remove_prefix(number_end);
            }
            tokens.push_back({
                .type = Token::Type::Number,
                .lexeme = lexeme,
            });
        }
        else if (state[0] == 'x')
        {
            tokens.push_back({
                .type = Token::Type::Variable,
                .lexeme = "x",
            });
            state.remove_prefix(1);
        }
        else
        {
            throw std::runtime_error("lexer error");
        }
    }

    return tokens;
}

} // namespace solver
