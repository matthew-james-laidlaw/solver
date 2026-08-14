#pragma once

#include <backend/monomial.h>

#include <string>
#include <vector>

namespace solver
{

struct Token;

/** @brief Parses an equation from an already-tokenized source.
 */
auto Parse(const std::vector<Token>& source) -> std::vector<Monomial>;

/** @brief Parses an equation directly from a source string.
 */
auto Parse(const std::string& source) -> std::vector<Monomial>;

} // namespace solver
