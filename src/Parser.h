#pragma once

#include <Expression.h>

#include <string>

auto Parse(std::string const& source) -> Expression*;
