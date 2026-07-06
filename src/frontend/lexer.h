#pragma once

#include <frontend/token.h>

#include <string>
#include <vector>

namespace solver
{

auto Lex(std::string const& source) -> std::vector<Token>;

} // namespace solver
