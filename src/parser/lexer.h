#pragma once

#include <parser/token.h>

#include <string>
#include <vector>

namespace solver
{

auto Lex(std::string const& source) -> std::vector<Token>;

} // namespace solver
