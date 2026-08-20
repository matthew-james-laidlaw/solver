#pragma once

#include <polynomial.h>
#include <solver/solver.h>

#include <expected>
#include <format>
#include <string>
#include <vector>

namespace solver
{

struct Token;

/** @brief Parses an equation from an already-tokenized source.
 */
auto Parse(const std::vector<Token>& source) -> std::expected<Polynomial, Error>;

/** @brief Parses an equation directly from a source string.
 */
auto Parse(const std::string& source) -> std::expected<Polynomial, Error>;

} // namespace solver
