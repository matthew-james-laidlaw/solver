#pragma once

#include <backend/monomial.h>

#include <vector>

namespace solver
{

/** @brief A polynomial is a collection of monomials where each index in the list
 * corresponds to an exponent value. E.g., 3x^2 + 5x - 7 is stored as { -7, 5, 3 }.
 */
using Polynomial = std::vector<int>;

/** @brief Sort and combine a list of arbitrary monomials into a single polynomial.
 */
auto Canonicalize(std::vector<Monomial> monomials) -> Polynomial;

} // namespace solver
