// clang-format off

#include <combinator/combinator.h>
#include <combinator/parser.h>
#include <combinator/result.h>
#include <combinator/state.h>

namespace solver
{

auto Expect(Token::Type type, std::string_view what) -> Parser<Token>
{
    return Parser<Token>(
        std::string(what),
        [=](State state) -> Result<Token>
        {
            if (state.Done()) {
                return Result<Token>::Failure(
                    state,
                    std::format("parser error: unexpected end of input, expected '{}'", what));
            }

            auto current = state.Peek();
            if (current.type != type) {
                return Result<Token>::Failure(
                    state,
                    std::format("parser error: expected '{}', got '{}'", what, current.lexeme));
            }

            return Result<Token>::Success(current, state.Advance());
        });
}

} // namespace solver

// clang-format on
