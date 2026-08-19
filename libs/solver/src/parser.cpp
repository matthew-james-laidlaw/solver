#include <combinator/parser.h>
#include <combinator/result.h>
#include <grammar.h>
#include <lexer.h>
#include <parser.h>
#include <polynomial.h>
#include <token.h>

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
    auto result = EquationParser()(state);
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
