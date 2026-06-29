#pragma once

#include <Expression.h>

#include <complex>
#include <vector>

using Solution = std::vector<std::complex<double>>;

auto Solve(Equality const& equation) -> Solution;
