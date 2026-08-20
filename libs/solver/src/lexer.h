#pragma once

#include <solver/solver.h>
#include <token.h>

#include <expected>
#include <string>
#include <vector>

namespace solver
{

/** @brief Transform a source string into a collection of tokens.
 */
auto Lex(const std::string& source) -> std::expected<std::vector<Token>, Error>;

} // namespace solver
