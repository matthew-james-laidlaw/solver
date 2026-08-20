#include <combinator/parser.h>
#include <combinator/result.h>
#include <grammar.h>
#include <lexer.h>
#include <parser.h>
#include <polynomial.h>
#include <solver/solver.h>
#include <token.h>

#include <format>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

namespace solver
{

auto Parse(const std::vector<Token>& source) -> std::expected<Polynomial, Error>
{
    std::span<const Token> state(source);
    auto result = EquationParser()(state);
    if (!result.Succeeded()) {
        return std::unexpected(Error(Error::Type::ParserError, result.Message()));
    }
    return Polynomial::FromTerms(result.Value());
}

auto Parse(const std::string& source) -> std::expected<Polynomial, Error>
{
    auto lexed = Lex(source);
    if (!lexed) {
        return std::unexpected(lexed.error());
    }

    return Parse(*lexed);
}

} // namespace solver
