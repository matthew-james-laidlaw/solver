#pragma once

#include <expression.h>
#include <token.h>

#include <vector>

namespace solver
{

auto Parse(std::vector<Token> const& tokens) -> std::unique_ptr<Expression>;

} // namespace solver
