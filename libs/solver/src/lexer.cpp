#include <frontend/lexer.h>

#include <cstddef>
#include <stdexcept>
#include <string_view>

using namespace std::string_literals;

namespace
{

auto TrimLeadingWhitespace(std::string_view& state) -> void
{
    const auto next_non_whitespace = state.find_first_not_of(" \t\n\r\f\v");
    size_t prefix = (next_non_whitespace == std::string_view::npos ? state.size()
                                                                   : next_non_whitespace);
    state.remove_prefix(prefix);
}

auto LexFunction(std::string_view& state) -> solver::Token
{
    auto result = solver::Token{
        .type = solver::Token::Type::Function,
        .lexeme = "f(x)",
    };
    state.remove_prefix("f(x)"s.size());
    return result;
}

auto LexEquals(std::string_view& state) -> solver::Token
{
    auto result = solver::Token{
        .type = solver::Token::Type::Equals,
        .lexeme = "=",
    };
    state.remove_prefix(1);
    return result;
}

auto LexPlus(std::string_view& state) -> solver::Token
{
    auto result = solver::Token{
        .type = solver::Token::Type::Plus,
        .lexeme = "+",
    };
    state.remove_prefix(1);
    return result;
}

auto LexMinus(std::string_view& state) -> solver::Token
{
    auto result = solver::Token{
        .type = solver::Token::Type::Minus,
        .lexeme = "-",
    };
    state.remove_prefix(1);
    return result;
}

auto LexCaret(std::string_view& state) -> solver::Token
{
    auto result = solver::Token{
        .type = solver::Token::Type::Caret,
        .lexeme = "^",
    };
    state.remove_prefix(1);
    return result;
}

auto LexNumber(std::string_view& state) -> solver::Token
{
    auto number_end = state.find_first_not_of("0123456789");
    auto lexeme = ""s;
    if (number_end == std::string_view::npos) {
        lexeme = state;
        state = "";
    }
    else {
        lexeme = state.substr(0, number_end);
        state.remove_prefix(number_end);
    }
    return solver::Token{
        .type = solver::Token::Type::Number,
        .lexeme = lexeme,
    };
}

auto LexVariable(std::string_view& state) -> solver::Token
{
    auto result = solver::Token{
        .type = solver::Token::Type::Variable,
        .lexeme = "x",
    };
    state.remove_prefix(1);
    return result;
}

} // namespace

namespace solver
{

auto Lex(const std::string& source) -> std::vector<solver::Token>
{
    auto state = std::string_view(source);
    auto tokens = std::vector<solver::Token>{};

    while (!state.empty()) {
        TrimLeadingWhitespace(state);
        if (state.empty()) {
            break;
        }

        if (state.starts_with("f(x)")) {
            tokens.push_back(LexFunction(state));
        }
        else if (state[0] == '=') {
            tokens.push_back(LexEquals(state));
        }
        else if (state[0] == '+') {
            tokens.push_back(LexPlus(state));
        }
        else if (state[0] == '-') {
            tokens.push_back(LexMinus(state));
        }
        else if (state[0] == '^') {
            tokens.push_back(LexCaret(state));
        }
        else if ('0' <= state[0] && state[0] <= '9') {
            tokens.push_back(LexNumber(state));
        }
        else if (state[0] == 'x') {
            tokens.push_back(LexVariable(state));
        }
        else {
            throw std::runtime_error("lexer error");
        }
    }

    return tokens;
}

} // namespace solver
