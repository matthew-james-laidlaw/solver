#pragma once

#include <complex>
#include <string>
#include <vector>

namespace solver
{

using Solution = std::vector<std::complex<double>>;

/** @brief Finds all solutions for any polynomial with order <= 4.
 * @param[in] equation A sum of terms of the form cx^n where c and n are integers and n is
 * a non-negative integer. Provided as a string.
 * @returns A list of the solutions to the provided equation.
 */
auto Solve(const std::string& equation) -> Solution;

} // namespace solver
