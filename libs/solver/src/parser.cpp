#include <backend/polynomial.h>
#include <combinator/parser.h>
#include <combinator/result.h>
#include <combinator/token.h>
#include <frontend/grammar.h>
#include <frontend/lexer.h>
#include <frontend/parser.h>

#include <format>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

namespace solver
{

auto Parse(const std::vector<Token>& source) -> Polynomial
{
    std::span<const Token> state(source);
    auto result = EquationParser(state);
    if (!result.Succeeded()) {
        throw std::runtime_error(result.Message());
    }
    return Polynomial(result.Value());
}

auto Parse(const std::string& source) -> Polynomial
{
    auto lexed = Lex(source);
    return Parse(lexed);
}

} // namespace solver
