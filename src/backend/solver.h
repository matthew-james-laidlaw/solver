#pragma once

#include <backend/monomial.h>

#include <complex>

namespace solver
{

using Solution = std::vector<std::complex<double>>;

/** @brief Finds all solutions for any polynomial with order <= 4.
 */
auto Solve(const Polynomial& equation) -> Solution;

} // namespace solver
