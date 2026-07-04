#pragma once

#include <token.h>

#include <string>
#include <vector>

namespace parser
{

auto Lex(std::string const& source) -> std::vector<Token>;

} // namespace parser
