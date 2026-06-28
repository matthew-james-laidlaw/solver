#pragma once

#include <Expression.h>

#include <vector>

/** @brief Solve for x in the given equality
 *  @returns All of the integer solutions for x.
 */
auto Solve(Equality const& equation) -> std::vector<int>;
