#pragma once

#include <backend/polynomial.h>

#include <format>
#include <string>
#include <vector>

namespace combinator
{

struct Token;

} // namespace combinator

namespace solver
{

/** @brief Parses an equation from an already-tokenized source.
 */
auto Parse(const std::vector<combinator::Token>& source) -> Polynomial;

/** @brief Parses an equation directly from a source string.
 */
auto Parse(const std::string& source) -> Polynomial;

} // namespace solver
