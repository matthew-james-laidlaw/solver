#pragma once

#include <frontend/token.h>

#include <string>
#include <vector>

namespace solver
{

/** @brief Transform a source string into a collection of tokens.
 */
auto Lex(const std::string& source) -> std::vector<Token>;

} // namespace solver
